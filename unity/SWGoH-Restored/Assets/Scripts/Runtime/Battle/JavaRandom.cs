// JavaRandom -- faithful port of java.util.Random, validated bit-exact against
// the JDK (6 seeds x 276 values) and against the game's own IL2CPP binary
// (JavaRandom..ctor 0x180CAC420: seed = (s ^ 0x5DEECE66D) & (2^48-1)).
// SWGoH battles are client-authoritative deterministic sims driven by this RNG.
public class JavaRandom
{
    const long MULT = 0x5DEECE66DL;
    const long ADD = 0xBL;
    const long MASK = (1L << 48) - 1;

    long _seed;

    public JavaRandom(long seed) { SetSeed(seed); }

    public void SetSeed(long seed) { _seed = (seed ^ MULT) & MASK; }

    int Next(int bits)
    {
        _seed = (_seed * MULT + ADD) & MASK;
        return (int)(_seed >> (48 - bits));
    }

    /// Full-range signed 32-bit draw.
    public int NextInt() { return Next(32); }

    /// java.util.Random.nextInt(bound) -- power-of-two shortcut + rejection loop.
    public int NextInt(int bound)
    {
        if (bound <= 0) throw new System.ArgumentOutOfRangeException("bound");
        if ((bound & -bound) == bound)
            return (int)(((long)bound * Next(31)) >> 31);
        int bits, val;
        do { bits = Next(31); val = bits % bound; }
        while (bits - val + (bound - 1) < 0);
        return val;
    }

    public long NextLong()
    {
        // both halves are signed 32-bit in Java semantics
        long hi = (long)Next(32);
        long lo = (long)Next(32);
        return (hi << 32) + lo;
    }

    public double NextDouble()
    {
        return (((long)Next(26) << 27) + Next(27)) * (1.0 / (1L << 53));
    }
}
