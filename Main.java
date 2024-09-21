import java.util.*;

public class Main {
    public static int function(String str_1, String str_2) {
        int m = str_1.length();
        int n = str_2.length();
        int max = 0;

        int[][] table = new int[m + 1][n + 1];

        for (int i = 1; i <= m; i ++){
            for (int j = 1; j <= n; j ++) {
                if (str_1.charAt(i - 1) == str_2.charAt(j - 1)) {
                    table[i][j] = table[i - 1][j - 1] + 1;
                    max = Math.max(max, table[i][j]);
                }
                else {
                    table[i][j] = 0;
                }
            }
        }

        return max;
    }

    public static void main(String[] arg) {
        String str_1 = "abcabcddd";
        String str_2 = "abcabc";

        int result = function(str_1, str_2);
        System.out.println(result);
    }
}
