import java.util.Random;

public class GenJavaRandomGT {
    public static void main(String[] args) {
        StringBuilder sb = new StringBuilder();
        for (long seed : new long[]{0L, 1L, 42L, 4490L, 123456789L, -987654321L}) {
            Random r = new Random(seed);
            sb.append(seed).append('|');
            for (int i = 0; i < 8; i++) sb.append(r.nextInt()).append(',');
            sb.append('|');
            for (int b : new int[]{2, 3, 5, 10, 100, 1000}) {
                Random r2 = new Random(seed);
                for (int i = 0; i < 5; i++) sb.append(r2.nextInt(b)).append(',');
                sb.append(';');
            }
            sb.append('|');
            Random r3 = new Random(seed);
            for (int i = 0; i < 4; i++) sb.append(r3.nextLong()).append(',');
            sb.append('|');
            Random r4 = new Random(seed);
            for (int i = 0; i < 4; i++) sb.append(r4.nextDouble()).append(',');
            sb.append('\n');
        }
        System.out.print(sb);
    }
}
