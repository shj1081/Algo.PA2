#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 120

int max(int a, int b) {
    return (a > b) ? a : b;
}

// Function to recompute the DP values needed for traceback
void recomputeDP(char *X, char *Y, char *Z, int m, int n, int o, int idx, int L[2][MAX_LEN+1][MAX_LEN+1]) {
    memset(L, 0, sizeof(int) * 2 * (MAX_LEN+1) * (MAX_LEN+1));
    for (int i = 0; i <= m; i++) {
        int currIdx = i % 2;
        for (int j = 0; j <= n; j++) {
            for (int k = 0; k <= o; k++) {
                if (i == 0 || j == 0 || k == 0)
                    L[currIdx][j][k] = 0;
                else if (i == idx) // We only need to recompute the layer at the current i
                    if (X[i-1] == Y[j-1] && X[i-1] == Z[k-1])
                        L[currIdx][j][k] = L[1-currIdx][j-1][k-1] + 1;
                    else
                        L[currIdx][j][k] = max(L[1-currIdx][j][k], max(L[currIdx][j-1][k], L[currIdx][j][k-1]));
            }
        }
    }
}

// Function to find the LCS length and prepare for traceback
int lcsOf3(char *X, char *Y, char *Z, int m, int n, int o, int L[2][MAX_LEN+1][MAX_LEN+1], int *finalIdx) {
    memset(L, 0, sizeof(int) * 2 * (MAX_LEN+1) * (MAX_LEN+1));
    for (int i = 0; i <= m; i++) {
        *finalIdx = i % 2;
        for (int j = 0; j <= n; j++) {
            for (int k = 0; k <= o; k++) {
                if (i == 0 || j == 0 || k == 0)
                    L[*finalIdx][j][k] = 0;
                else if (X[i-1] == Y[j-1] && X[i-1] == Z[k-1])
                    L[*finalIdx][j][k] = L[1-*finalIdx][j-1][k-1] + 1;
                else
                    L[*finalIdx][j][k] = max(L[1-*finalIdx][j][k], max(L[*finalIdx][j-1][k], L[*finalIdx][j][k-1]));
            }
        }
    }
    return L[*finalIdx][n][o];
}

// Function to actually find the LCS string using traceback
void findLCS(char *X, char *Y, char *Z, int m, int n, int o, int L[2][MAX_LEN+1][MAX_LEN+1], char *lcs) {
    int idx, i = m, j = n, k = o;
    int lcsLength = L[i % 2][j][k];
    lcs[lcsLength] = '\0'; // Null-terminate the LCS string

    while (i > 0 && j > 0 && k > 0) {
        idx = i % 2;
        // Recompute previous layer when necessary
        if (i > 1) recomputeDP(X, Y, Z, m, n, o, i-1, L);

        // Check if the current character in X, Y and Z are the same
        if (X[i-1] == Y[j-1] && X[i-1] == Z[k-1]) {
            lcs[--lcsLength] = X[i-1]; // Append current character to LCS
            i--; j--; k--; // Move diagonally in 3D space
        } else if (L[1-idx][j][k] >= L[idx][j-1][k] && L[1-idx][j][k] >= L[idx][j][k-1])
            i--; // Move in the X direction
        else if (L[idx][j-1][k] >= L[idx][j][k-1])
            j--; // Move in the Y direction
        else
            k--; // Move in the Z direction
    }
}

int main() {
    char X[MAX_LEN] = "AGGT12";
    char Y[MAX_LEN] = "12TXAYB";
    char Z[MAX_LEN] = "12XBA";

    int m = strlen(X);
    int n = strlen(Y);
    int o = strlen(Z);
    
    int L[2][MAX_LEN+1][MAX_LEN+1];
    int finalIdx;
    int lcsLength = lcsOf3(X, Y, Z, m, n, o, L, &finalIdx);

    char lcs[MAX_LEN];
    findLCS(X, Y, Z, m, n, o, L, lcs);

    printf("Length of LCS is %d\n", lcsLength);
    printf("LCS of the three strings is '%s'\n", lcs);

    return 0;
}
