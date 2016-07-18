#!/usr/bin/env python
from past.builtins import range
import hashlib, sys

h = hashlib.new('sha256')
h.update(sys.argv[1])
res = h.digest()[:6]
for i in range(6):
    sys.stdout.write("%02x " % ord(res[i]))
sys.stdout.write("\n")




