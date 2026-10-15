# -*- coding: utf-8 -*-
"""Exact port of java.util.Random — SWGoH's client battle sim draws every
chance/targeting roll from `JavaRandom` (dump line 317578: a 48-bit LCG with
Java's `next(bits)`/`nextInt(bound)` semantics), seeded by the server's
battle_seed.  Reproducing it bit-exactly is the precondition for offline
battle replay.

Verified against ground truth from the local JDK (tools/gen_java_random_gt.py).
"""


class JavaRandom:
    _MULT = 0x5DEECE66D
    _ADD = 0xB
    _MASK = (1 << 48) - 1

    def __init__(self, seed=0):
        self.seed = 0
        self.set_seed(seed)

    def set_seed(self, seed):
        self.seed = (seed ^ self._MULT) & self._MASK

    def next(self, bits):
        self.seed = (self.seed * self._MULT + self._ADD) & self._MASK
        return self.seed >> (48 - bits)          # unsigned; bits <= 32

    def next_int(self, bound=None):
        """nextInt() / nextInt(bound), Java semantics incl. the rejection loop
        for non-power-of-two bounds."""
        if bound is None:
            v = self.next(32)
            return v - (1 << 32) if v >= (1 << 31) else v   # signed int32
        if bound <= 0:
            raise ValueError("bound must be positive")
        if bound & -bound == bound:              # power of two
            return (bound * self.next(31)) >> 31
        while True:
            bits = self.next(31)
            val = bits % bound
            if bits - val + (bound - 1) < (1 << 31):   # no int32 overflow retry
                return val

    def _next_signed32(self):
        v = self.next(32)
        return v - (1 << 32) if v >= (1 << 31) else v

    def next_long(self):
        # Java: ((long)next(32) << 32) + next(32) -- both halves SIGNED ints
        return (self._next_signed32() << 32) + self._next_signed32()

    def next_double(self):
        return ((self.next(26) << 27) + self.next(27)) / float(1 << 53)
