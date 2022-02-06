#!/usr/bin/python3

import random

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

if __name__ == "__main__":
    generate_add_tests("tests/testdata/add.txt", 1000)
    generate_sub_tests("tests/testdata/sub.txt", 1000)
    generate_mul_tests("tests/testdata/mul.txt", 1000)
    generate_div_tests("tests/testdata/div.txt", 1000)
    generate_shl_one_tests("tests/testdata/shl_one.txt", 10)
    generate_shr_one_tests("tests/testdata/shr_one.txt", 10)
    generate_shl_limb_tests("tests/testdata/shl_limb.txt", 100)
    generate_shr_limb_tests("tests/testdata/shr_limb.txt", 100)