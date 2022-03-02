#!/usr/bin/python3

import random
import Crypto.Util.number
import x25519
def generate_add_tests(path, n):
    with open(path, 'w', encoding='utf-8') as file:
        for _ in range(n):
            a = random.randint(0, 2 ** 256)
            b = random.randint(0, 2 ** 256)
            c = (a + b) % (2 ** 256)
            file.write(f"0x{a:064x} 0x{b:064x} 0x{c:064x}\n")

def generate_sub_tests(path, n):
    with open(path, 'w', encoding='utf-8') as file:
        for _ in range(n):
            a = random.randint(0, 2 ** 256)
            b = random.randint(0, 2 ** 256)
            c = (a - b) % (2 ** 256)
            file.write(f"0x{a:064x} 0x{b:064x} 0x{c:064x}\n")

def generate_sub_tests(path, n):
    with open(path, 'w', encoding='utf-8') as file:
        for _ in range(n):
            a = random.randint(0, 2 ** 256)
            b = random.randint(0, 2 ** 256)
            c = (a - b) % (2 ** 256)
            file.write(f"0x{a:064x} 0x{b:064x} 0x{c:064x}\n")

def generate_mul_tests(path, n):
    with open(path, 'w', encoding='utf-8') as file:
        for _ in range(n):
            a = random.randint(0, 2 ** 256)
            b = random.randint(0, 2 ** 256)
            c = (a * b) % (2 ** 512)
            file.write(f"0x{a:064x} 0x{b:064x} 0x{c:0128x}\n")

def generate_div_tests(path, n):
    with open(path, 'w', encoding='utf-8') as file:
        for _ in range(n):
            a = random.randint(0, 2 ** 256)
            b = random.randint(0, 2 ** 256)
            c = (a // b)
            d = (a % b)
            file.write(f"0x{a:064x} 0x{b:064x} 0x{c:064x} 0x{d:064x}\n")

def generate_shl_one_tests(path, n):
    with open(path, 'w', encoding='utf-8') as file:
        for _ in range(n):
            a = random.randint(0, 2 ** 256)
            b = (a << 1) % (2 ** 256)
            file.write(f"0x{a:064x} 0x{b:064x}\n")

def generate_shr_one_tests(path, n):
    with open(path, 'w', encoding='utf-8') as file:
        for _ in range(n):
            a = random.randint(0, 2 ** 256)
            b = (a >> 1) % (2 ** 256)
            file.write(f"0x{a:064x} 0x{b:064x}\n")

def generate_shl_limb_tests(path, n):
    with open(path, 'w', encoding='utf-8') as file:
        for _ in range(n):
            a = random.randint(0, 2 ** 256)
            b = random.randint(0, 8)
            c = (a << (b * 32)) % (2 ** 256)
            file.write(f"0x{a:064x} 0x{b:08x} 0x{c:064x}\n")

def generate_shr_limb_tests(path, n):
    with open(path, 'w', encoding='utf-8') as file:
        for _ in range(n):
            a = random.randint(0, 2 ** 256)
            b = random.randint(0, 8)
            c = (a >> (b * 32)) % (2 ** 256)
            file.write(f"0x{a:064x} 0x{b:08x} 0x{c:064x}\n")

def generate_mod_add_tests(path, n):
    with open(path, 'w', encoding='utf-8') as file:
        for _ in range(n):
            p = Crypto.Util.number.getPrime(256)
            a = random.randint(0, p - 1)
            b = random.randint(0, p - 1)
            c = (a + b) % (p)
            file.write(f"0x{p:064x} 0x{a:064x} 0x{b:064x} 0x{c:064x}\n")

def generate_mod_sub_tests(path, n):
    with open(path, 'w', encoding='utf-8') as file:
        for _ in range(n):
            p = Crypto.Util.number.getPrime(256)
            a = random.randint(0, p - 1)
            b = random.randint(0, p - 1)
            c = (a - b) % (p)
            file.write(f"0x{p:064x} 0x{a:064x} 0x{b:064x} 0x{c:064x}\n")

def generate_mod_mul_tests(path, n):
    with open(path, 'w', encoding='utf-8') as file:
        for _ in range(n):
            p = Crypto.Util.number.getPrime(256)
            a = random.randint(0, p - 1)
            b = random.randint(0, p - 1)
            c = (a * b) % (p)
            file.write(f"0x{p:064x} 0x{a:064x} 0x{b:064x} 0x{c:064x}\n")

def generate_mod_pow_tests(path, n):
    with open(path, 'w', encoding='utf-8') as file:
        for _ in range(n):
            p = Crypto.Util.number.getPrime(256)
            a = random.randint(0, p - 1)
            b = random.randint(0, p - 1)
            c = pow(a, b, p)
            file.write(f"0x{p:064x} 0x{a:064x} 0x{b:064x} 0x{c:064x}\n")

def generate_mod_inv_tests(path, n):
    with open(path, 'w', encoding='utf-8') as file:
        for _ in range(n):
            p = Crypto.Util.number.getPrime(256)
            a = random.randint(0, p - 2)
            b = pow(a, p - 2, p)
            file.write(f"0x{p:064x} 0x{a:064x} 0x{b:064x}\n")

def generate_barrett_tests(path, n):
    with open(path, 'w', encoding='utf-8') as file:
        for _ in range(n):
            p = Crypto.Util.number.getPrime(256)
            q = (1 << (2 * p.bit_length())) // p 
            file.write(f"0x{p:064x} 0x{q:072x}\n")

def generate_x25519_double_tests(path, n):
    with open(path, 'w', encoding='utf-8') as file:
        p = (9, 1)
        for _ in range(n):
            q = x25519.djbec.double(p)
            file.write(f"0x{p[0]:064x} 0x{p[1]:064x}\n") 
            p = q

def generate_x25519_add_tests(path, n):
    with open(path, 'w', encoding='utf-8') as file:
        one = (9, 1)
        n0 = one
        n1 = x25519.djbec.double(one)
        for _ in range(n):   
            n2 = x25519.djbec.add(n0, n1, one)
            file.write(f"0x{n0[0]:064x} 0x{n0[1]:064x} 0x{n1[0]:064x} 0x{n1[1]:064x} 0x{n2[0]:064x} 0x{n2[1]:064x}\n") 
            n0 = n1
            n1 = n2

def curve25519_iter(n, base=9):
    one = (base,1)
    two = x25519.djbec.double(one)

    # f(m) evaluates to a tuple
    # containing the mth multiple and the
    # (m+1)th multiple of base.
    pm = one
    pm1 = two
    for i in range(253, -1, -1):
        b = 0 if n & (1 << i) == 0 else 1
        a = x25519.djbec.add(pm, pm1, one)
        d = x25519.djbec.double(pm1 if b else pm)
        pm = a if b else d
        pm1 = d if b else a

    (x, z) = pm

    return (x * x25519.djbec.inv(z)) % x25519.djbec.P

def generate_x25519_mul_tests(path, n):
    with open(path, 'w', encoding='utf-8') as file:
        for i in range(1, n):
            j = 2 ** 254 + i
            e = x25519.djbec.curve25519(j)
            e2 = curve25519_iter(j)
            assert e == e2
            file.write(f"0x{j:064x} 0x{e:064x}\n") 

if __name__ == "__main__":
    # generate_add_tests("tests/testdata/add.txt", 1000)
    # generate_sub_tests("tests/testdata/sub.txt", 1000)
    # generate_mul_tests("tests/testdata/mul.txt", 1000)
    # generate_div_tests("tests/testdata/div.txt", 1000)
    # generate_shl_one_tests("tests/testdata/shl_one.txt", 10)
    # generate_shr_one_tests("tests/testdata/shr_one.txt", 10)
    # generate_shl_limb_tests("tests/testdata/shl_limb.txt", 100)
    # generate_shr_limb_tests("tests/testdata/shr_limb.txt", 100)
    # generate_mod_add_tests("tests/testdata/add_mod.txt", 100)
    # generate_mod_sub_tests("tests/testdata/sub_mod.txt", 100)
    # generate_mod_mul_tests("tests/testdata/mul_mod.txt", 100)
    # generate_mod_pow_tests("tests/testdata/pow_mod.txt", 100)
    # generate_mod_inv_tests("tests/testdata/inv_mod.txt", 100)
    # generate_x25519_double_tests("tests/testdata/x25519_double.txt", 1000)
    # generate_x25519_add_tests("tests/testdata/x25519_add.txt", 1000)
    generate_x25519_mul_tests("tests/testdata/x25519_mul.txt", 1000)