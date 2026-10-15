# -*- coding: utf-8 -*-
"""Verify offline/java_random.py against JDK ground truth (GenJavaRandomGT)."""
import os
import sys

TOOLS = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.join(os.path.dirname(TOOLS), "offline"))
from java_random import JavaRandom


def s32(v):
    return v - (1 << 32) if v >= (1 << 31) else v


def main():
    gt = os.environ.get("JGT", os.path.join(os.environ["TEMP"], "jgt.txt"))
    lines = open(gt, encoding="utf-8").read().splitlines()
    n_checked = 0
    for line in lines:
        parts = line.split("|")
        seed = int(parts[0])
        nxt = [int(x) for x in parts[1].rstrip(",").split(",")]
        bounds = parts[2].split(";")
        longs = [int(x) for x in parts[3].rstrip(",").split(",")]
        dbls = [float(x) for x in parts[4].rstrip(",").split(",")]

        r = JavaRandom(seed)
        got = [r.next_int() for _ in range(8)]
        assert got == nxt, f"seed {seed} nextInt: {got[:3]} != {nxt[:3]}"
        n_checked += len(got)

        for seg, b in zip(bounds, (2, 3, 5, 10, 100, 1000)):
            r2 = JavaRandom(seed)
            got = [r2.next_int(b) for _ in range(5)]
            want = [int(x) for x in seg.rstrip(",").split(",")]
            assert got == want, f"seed {seed} nextInt({b}): {got} != {want}"
            n_checked += len(got)

        r3 = JavaRandom(seed)
        got = [r3.next_long() for _ in range(4)]
        assert got == longs, f"seed {seed} nextLong mismatch"
        n_checked += 4

        r4 = JavaRandom(seed)
        got = [r4.next_double() for _ in range(4)]
        assert all(abs(a - b) < 1e-15 for a, b in zip(got, dbls)), \
            f"seed {seed} nextDouble mismatch"
        n_checked += 4

    print(f"JAVA RANDOM VERIFY: {n_checked} values across "
          f"{len(lines)} seeds -- ALL MATCH")


if __name__ == "__main__":
    main()
