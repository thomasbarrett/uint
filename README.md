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

# Optional Extension
A possible extension that would use this library is an implementation 
of the Diffie-Hellman key exchange using the Curve25519 group. This is
the default key exchange protocol in TLS 1.3.