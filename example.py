# this code supports both python 2 and 3
from binascii import hexlify
import x25519
import time

private_key = b'1' * 32
print(hexlify(private_key))
public_key = x25519.scalar_base_mult(private_key)
print(hexlify(public_key))

start = time.time()
for i in range(432):
    peer_public_key = b'2' * 32
    shared_secret = x25519.scalar_mult(private_key, peer_public_key)
end = time.time()
print(end - start)