#!usr/bin/python3

from uuid import UUID
import argparse

pbin = "./calc.bin"

with open(pbin, "rb") as f:
    # read in 16 bytes from our input payload
    chunk = f.read(16)
    while chunk:
        # if the chunk is less than 16 bytes then we pad the difference (x90)
        if len(chunk) < 16:
            padding = 16 - len(chunk)
            chunk = chunk + (b"\x90" * padding)
        print('"' + str(UUID(bytes_le=chunk)) + '",')
        chunk = f.read(16)
