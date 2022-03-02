# uint
The uint library implements constant-time arithmetic operations for arbitrary
length unsigned integers suitable for usage in a cryptography library. The
following operations are supported.

- uint_cmp
- uint_add
- uint_sub
- uint_mul
- uint_div
- uint_shl
- uint_shr

Additionally, the uint library implements constant-time arithmetic operations
for elements of a Galois field of prime order. These operations are commonly
used in elliptic curve cryptography. Essentially, these operations are normal
arithmetic operations modulo a prime.

- mod_add 
- mod_sub
- mod_mul
- mod_pow
- mod_inv

# Integer Representation and Precision Restrictions
The uint library implements arbitrary length integers as arrays of uint32_t,
where each integer represents a single "digit" in a base 2^32 number system.
This is effectively a little-endian representation. Thus, only integers that
are a multiple of 64-bits are supported. This is sufficient since most cryptographic
algorithms only use numbers that are a multiple of 32 (256, 512, 1024, etc).

For example, the following integer array with be interpreted as 
a * (2^32)^0 + b * (2^32)^1 + c * (2^32)^2 + d * (2^32)^3
```c
uint64_t n[4] = {a, b, c, d};
```

# Constant Time Guarentees
One important source of weakness in the implementation of cryptographic
algorithms is side-channel attacks. Time is a common side channel. For
example, imagine a scenario in which a server holds a private key and
signs all messages sent to it so that any third party could verify its
authenticity with the servers public key. After sending enough messages
to the server, and timing how long the server took to respond, an attacker
could potentially learn information about the private key itself. If enough
information is revealed, an attacker could brute force compute the server's
private key, breaking the cryptosystem.

To mitigate side channel attacks in all (theoretical) cryptographic
implementations based on this library, all integer operations are
constant time with respect to their inputs and some fixed length k.
For example, computing 64 / 37 will take the same amount of time as
computing 64 / 1.

Ensuring constant time guarentees is a platform specific progress
as detailed [here](https://www.bearssl.org/constanttime.html). In certain
older platforms, shifts, rotations, and multiplication are not constant
time operations. However, in nearly all modern CPUs and platforms, they
are safe to use. Integer division is not a constant time operation even
in modern CPUs as smaller divisors often result in a faster code path.
In all CPUs conditional jumps dependent on the non-constant inputs also
result in code that is not constant time. All operations must be written
without the use of conditional jumps.

# Implementation
## Multiplication
For multiplication, we implement the 'schoolbook' long multiplication
algorithm. Here, we show three possible implementations of the algorithm
that reduce the number of addq and mulq operations used by optimizing
away unnecessary operations. Mainly, we can reduce the number of unnecessary
`addq` operations by minimizing which digits of the intermediate product
we add to the result. 

1. Implementation 1
- addq: 2 * N^3
- mulq: N^2
```
Given two N digit numbers a and b:
    Declare res[N] = 0
    For every digit ai of a
        For every digit bi of b
            Declare tmp[N] = 0
            tmp[i+j:i+j+2] = ai * bi    (0 add, 1 mul)
            res += tmp                  (2 * N add, 0 mul)
    Return res
```

2. Implementation 2
- addq: 5 * N^2 + 3 * N
- mulq: N^2
```
Assume the existance of N digit by 1 digit multiplication
that uses N multiplications and N additions and produces an
N + 1 digit number.

Given two N digit numbers a and b:
    Declare res[2 * N + 1] = 0
    For every digit bi of b
        Declare tmp[2 * N + 1] = 0
        tmp[i:i+N+1] = a * bi          (N + 1 add, N mul)
        res += tmp                     (4 * N + 2 add, 0 mul)
    Return res
```

3. Implementation 3
- addq: 3 * N^2 + 5 * N
- mulq: N^2

```
Given two N digit numbers a and b:
    Declare res[2 * N + 2] = 0
    For every digit bi of b
        Declare tmp[N + 2] = 0
        tmp[0:N+1] = a * bi            (N + 1 add, N mul)
        res[i:i+N+2] += tmp[0:N+2]     (2 * N + 4 add, 0 mul)
    Return res
```

## Division
The division operation is implemented with binary long division. While this
is not the fastest available division algorithm, it is the easiest that I have
found that can be easily translated into constant time operations. The pseudocode
for the binary long division algorithm can be found [here](https://en.wikipedia.org/wiki/Division_algorithm#Long_division).

### Constant Time Value Selection
There is a conditional subtraction in the binary division operation. This
is not constant time:
```
if R >= D then
    R := R − D
    Q(i) := 1
end
```
This can be modified to the better constant time construct. Explanation of
the strange construct can be found [here](https://crypto.stackexchange.com/questions/96614/how-can-i-understand-whether-my-c-implementation-is-constant-time-or-not-i-e-r)
```
B = R >= D
R := R - (~(B - 1) & D)
Q(i) := B
```
Note that one may be tempted to write the following using a condition move instruction. This is not constant time since the D and ZERO variables are
stored in different locations in memory (and thus will be accessed on different cache lines). This opens up the algorithm to a cache-timing attack
vulnerability.
```
ZERO := 0
B = R >= D
R := R - (B ? D: ZERO)
Q(i) := B
```
### Constant Time Bit Access
In order to implement the binary long divison algorithm above, we need a constant
time way to access bits. The naiive way to access the `i`th bit of `a` is the following:

```c
    uint_t wi = i / 32;
    uint_t bi = i % 32;
    return (a[wi] >> bi) & 0x1U
```

This implementation is problematic because the x64 division and modulo instructions are
not actually constant time. Instead, we can explicitly use the left and right shift
operators to compute the word and bit indices.

# Fast Modular Operations
Modular addition and subtract can all be trivially implemented with an
addition, subtract, and comparison operator.

Modular multiplication is trickier to implement that addition and subtraction
since the result of multiplication may be many times greater than the prime
modulo, so the naiive implementation would have to use divison in order to 
compute the result modulo a prime. This is a problem since usually divison
is orders of magnitude slower than multipliation. This is yet to be seen after
implementation.

Barrett's reduction modulo will be used to speed up modular multiplication.
This numerical trick allows special constants associated with prime modulo
to be precomputed so that a modulo operation can be implemented with addition
and multiplication instead of division. Since cryptographic algorithms usually
only need to perform operations in GF(p) for a specific fixed prime p, this
should be sufficient.

## Addition
Modular addition can be implemented simply and efficiently:
```
Given two N digit numbers a and b and prime q:
    c := a + b
    if c >= q:
        c -= q
    return c
```
However, this is not constant time since we use a branch conditional
on the input data. Luckily, this is not to difficulty to solve.
```
Given two N digit numbers a and b and prime q:
    c := a + b
    d := c >= q
    c -= ~(d - 1) & q
    return c
```

## Subtraction
Subtraction can be implemented using a similar idea.
```
Given two N digit numbers a and b and prime q:
    d := a < b
    b += ~(d - 1) & q
    return a - b
```

## Multiplication
Since the result of multiplication is far larger than `p`, we implement
modular multiplication using the following algorithm. Since big integer
division is used, this operation is slower than big integer division.
```
Given two N digit numbers a and b and prime p:
    declare q[N], r[N]
    c[2 * N] = a * b
    (q[N], r[N]) = (c / p, c % p) 
    return r
```

## Exponentiation
We implement modular multiplication use exponentiation by squaring. 
Discussion of this technique can be found [here](https://en.wikipedia.org/wiki/Exponentiation_by_squaring#Montgomery's_ladder_technique)

## Inversion
We use the exponentiation operation to implement the modular inversion.
For any N digit number a and prime p, the inverse of `a` is `a^(p - 1)`.
This is a basic fact from algebra. This is a pretty slow operation... it
may be possible to make faster using a constant-time variant of the gcd
algorithm, such as the one found [here](https://gcd.cr.yp.to/safegcd-20190413.pdf)

# Benchmark:
The benchmark results before optimization.
```
run_uint_cmp: 19150804
run_uint_add: 25506648
run_uint_sub: 21707278
run_uint_mul: 1511724
run_uint_div: 26801
run_mod_add: 3765058
run_mod_sub: 3807269
run_mod_mul: 7245
run_mod_pow: 14
run_mod_inv: 14
```

```
run_uint_cmp: 66705133 // 3.4x
run_uint_add: 79844320 // 3.1x
run_uint_sub: 72368027 // 3.3x
run_uint_mul: 8601356 // 5.7x
run_uint_div: 162563 // 6.0x
run_gfp_add: 23406476 // 6.2x
run_gfp_sub: 24221503 // 6.3x
run_gfp_mul: 1922255 // 265.3x
run_gfp_pow: 3962 // 283x
run_gfp_inv: 3939 // 281x
```

# Example Usage
The x25519.c file implements an example use case of this library: an elliptic curve
diffie-hellman key exchange algorithm. The x25519 library implements Curve25519
elliptic curve group with constant time addition and scalar multiplication group
operations. The scalar multiplication operation is implemented using a montgomery
ladder with a differential addition chain. The explanation and mathematic derivation
of this approach can be found [here](https://martin.kleppmann.com/papers/curve25519.pdf).
[This](https://pypi.org/project/x25519/) python implementation used to test correctness.

I have implemented a simple example program that can be used to test the key exchange.
```
make
./bin/example
```

This will generate a private key by reading 32 bytes from `/dev/random` and calculate
the public key sG where G is the x25519 base point (9, 1) using the x25519 scalar
multiplication implementation. Example output is shown below:
```
sk1: 782e5be7867e3e820f41bfe68449afc70c37c1896fb6df8af61747d89fd81fe5
pk1: 1b82a443f14abebbcb0c4276ddfcf7ee1067a67fa45c26e866346736b8b96536
enter pk2: 
```

Another user can also run the `/bin/example` executable and generate their own key pair.
Both users and share and input each others public keys. They should both get the same
shared secret. Example output is shown below:

```
sk1: 679ee974efd58b50bd56cd8d59adab54114c91fdf650119f974664eb9660b251
pk1: afafb6396efd9d754ac6d5dc0130f98cb965ab437aba1198b20ea1e5f5d47128
enter pk2: cd92df39b14960b85df9e9f10434f38edd1e2d06d9f4eefb4f9ecb62544b666a
secret: df3096fde88e624eddaa78efeb206c71dbbe8db22c6fefe7f71aca0ffc022667
```

The most recent benchmark indicates that we can perform 432 scalar multiplications
per second on the x25519 curve. This is about 70% faster than the x25519 implementation.