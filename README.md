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
The uint library implements arbitrary length integers as arrays of uint64_t,
where each integer represents a single "digit" in a base 2^64 number system.
Thus, only integers that are a multiple of 64-bits are supported. This is
sufficient since most cryptographic algorithms only use numbers that are a
multiple of 64 (256, 512, 1024, etc).

For example, the following integer array with be interpreted as 
a * (2^64)^3 + b * (2^64)^2 + c * (2^64)^1 + d * (2^64)^0
```c
uint64_t n = {a, b, c, d};
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

