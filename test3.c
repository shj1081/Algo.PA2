#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SEQUENCES 5
#define MAX_LEN 120

int max(int a, int b) { return (a > b) ? a : b; }

// Function to recompute the DP values needed for traceback
void recomputeDP_3(char *X, char *Y, char *Z, int m, int n, int o,
                 int L[2][MAX_LEN + 1][MAX_LEN + 1]) {
  memset(L, 0, sizeof(int) * 2 * (MAX_LEN + 1) * (MAX_LEN + 1));
  for (int i = 0; i <= m; i++) {
    int idx = i % 2;
    for (int j = 0; j <= n; j++) {
      for (int k = 0; k <= o; k++) {
        if (i == 0 || j == 0 || k == 0)
          L[idx][j][k] = 0;
        else if (X[i - 1] == Y[j - 1] && X[i - 1] == Z[k - 1])
          L[idx][j][k] = L[1 - idx][j - 1][k - 1] + 1;
        else
          L[idx][j][k] =
              max(L[1 - idx][j][k], max(L[idx][j - 1][k], L[idx][j][k - 1]));
      }
    }
  }
}

// Function to find the LCS length and prepare for traceback
int lcsOf3(char *X, char *Y, char *Z, int m, int n, int o,
           int L[2][MAX_LEN + 1][MAX_LEN + 1], int *finalIdx) {
  memset(L, 0, sizeof(int) * 2 * (MAX_LEN + 1) * (MAX_LEN + 1));
  for (int i = 0; i <= m; i++) {
    *finalIdx = i % 2;
    for (int j = 0; j <= n; j++) {
      for (int k = 0; k <= o; k++) {
        if (i == 0 || j == 0 || k == 0)
          L[*finalIdx][j][k] = 0;
        else if (X[i - 1] == Y[j - 1] && X[i - 1] == Z[k - 1])
          L[*finalIdx][j][k] = L[1 - *finalIdx][j - 1][k - 1] + 1;
        else
          L[*finalIdx][j][k] =
              max(L[1 - *finalIdx][j][k],
                  max(L[*finalIdx][j - 1][k], L[*finalIdx][j][k - 1]));
      }
    }
  }
  return L[*finalIdx][n][o];
}

// Function to actually find the LCS string using traceback and recomputing DP
char *findLCS_3(char *X, char *Y, char *Z, int m, int n, int o,
              int L[2][MAX_LEN + 1][MAX_LEN + 1]) {
  int lcsLength = L[m % 2][n][o];
  char *lcs = (char *)malloc(sizeof(char) * (MAX_LEN + 1));

  lcs[lcsLength] = '\0';
  int i = m, j = n, k = o;

  // Traceback to find the LCS
  while (i > 0 && j > 0 && k > 0) {
    
    // If current character in X[], Y[] and Z[] are same, then current
    // character is part of LCS
    if (X[i - 1] == Y[j - 1] && X[i - 1] == Z[k - 1]) {
      lcs[--lcsLength] = X[i - 1];       // Put current character in result
      i--, j--, k--;                     // Reduce values of i, j, k
      recomputeDP(X, Y, Z, i, n, o, L);  // Recompute DP for the previous row
    } else {
      // If not same, then find the larger of two and go in the direction of
      // larger value
      if (L[(i - 1) % 2][j][k] >= L[i % 2][j - 1][k] &&
          L[(i - 1) % 2][j][k] >= L[i % 2][j][k - 1]) {
        i--;
        recomputeDP(X, Y, Z, i, n, o, L);  // Recompute DP for the previous row
      } else if (L[i % 2][j - 1][k] >= L[i % 2][j][k - 1]) {
        j--;
      } else {
        k--;
      }
    }
  }

  return lcs;
}

int main() {
  char *seqs[MAX_SEQUENCES];
  int seqs_num;


  int L[2][MAX_LEN + 1][MAX_LEN + 1];
  int finalIdx;
  int lcsLength = lcsOf3(X, Y, Z, m, n, o, L, &finalIdx);

  char *lcs = findLCS(X, Y, Z, m, n, o, L);

  printf("Length of LCS is %d\n", lcsLength);
  printf("LCS of the three strings is '%s'\n", lcs);

  return 0;
}