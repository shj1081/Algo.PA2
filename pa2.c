#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 100

int max(int a, int b) {
    return (a > b) ? a : b;
}

int lcs(int k, char **seq) {
    int m[MAX_LEN][MAX_LEN][MAX_LEN];
    int i, j, l, len[k];

    for (i = 0; i <= strlen(seq[0]); i++) {
        for (j = 0; j <= strlen(seq[1]); j++) {
            for (l = 0; l <= strlen(seq[2]); l++) {
                if (i == 0 || j == 0 || l == 0) {
                    m[i][j][l] = 0;
                } else if (seq[0][i-1] == seq[1][j-1] && seq[0][i-1] == seq[2][l-1]) {
                    m[i][j][l] = m[i-1][j-1][l-1] + 1;
                } else {
                    m[i][j][l] = max(max(m[i-1][j][l], m[i][j-1][l]), m[i][j][l-1]);
                }
            }
        }
    }

    int len = m[strlen(seq[0])][strlen(seq[1])][strlen(seq[2])];
    char subseq[len+1];
    subseq[len] = '\0';

    i = strlen(seq[0]);
    j = strlen(seq[1]);
    l = strlen(seq[2]);

    while (i > 0 && j > 0 && l > 0) {
        if (seq[0][i-1] == seq[1][j-1] && seq[0][i-1] == seq[2][l-1]) {
            subseq[len-1] = seq[0][i-1];
            i--;
            j--;
            l--;
            len--;
        } else if (m[i-1][j][l] > m[i][j-1][l] && m[i-1][j][l] > m[i][j][l-1]) {
            i--;
        } else if (m[i][j-1][l] > m[i-1][j][l] && m[i][j-1][l] > m[i][j][l-1]) {
            j--;
        } else {
            l--;
        }
    }

    printf("Length of LCS is %d\n", m[strlen(seq[0])][strlen(seq[1])][strlen(seq[2])]);
    printf("Common subsequence is %s\n", subseq);

    return m[strlen(seq[0])][strlen(seq[1])][strlen(seq[2])];
}

int main() {
    char *seq[] = {"AGGT123", "12TX3YB", "12XBA3"};
    int k = 3;

    printf("Length of LCS is %d\n", lcs(k, seq));

    return 0;
}
