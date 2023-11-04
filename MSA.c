#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int sum(int *arr, int size) {
  int sum = 0;
  for (int i = 0; i < size; i++) {
    sum += arr[i];
  }
  return sum;
}

int max(int a, int b) {
  if (a > b) {
    return a;
  }
  return b;
}

void MSA(char **seqs, char *lcs) {
  int match[3];               // k sequences
  char aligned_seqs[3][360];  // k sequences, assuming a max length of 360

  int seqs_idx[3] = {0, 0, 0};     // Indexes for each sequence
  int aligned_idx[3] = {0, 0, 0};  // Indexes for each aligned sequence
  int lcs_len = strlen(lcs);
  int lcs_idx = 0;

  // Initialize the aligned sequences with '-'
  for (int i = 0; i < 3; i++) {
    memset(aligned_seqs[i], '-', sizeof(aligned_seqs[i]));
  }

  while (lcs_idx < lcs_len) {
    // Check match or not
    for (int i = 0; i < 3; i++) {  // k sequences
      if (seqs[i][seqs_idx[i]] == lcs[lcs_idx]) {
        match[i] = 1;
      } else {
        match[i] = 0;
      }
    }

    // If all match or all mismatch
    if (sum(match, 3) == 3 || sum(match, 3) == 0) {
      for (int i = 0; i < 3; i++) {
        if (seqs_idx[i] < strlen(seqs[i])) {
          aligned_seqs[i][aligned_idx[i]] = seqs[i][seqs_idx[i]];
          seqs_idx[i]++;
        }
        aligned_idx[i]++;
      }
      lcs_idx++;
    }
    // If partial match
    else {
      for (int i = 0; i < 3; i++) {
        if (match[i] != 1) {
          aligned_seqs[i][aligned_idx[i]] = seqs[i][seqs_idx[i]];
          seqs_idx[i]++;
        }

        // Increment alignment index for all
        aligned_idx[i]++;
      }
    }
  }
  // Fill in the remaining characters
  for (int i = 0; i < 3; i++) {
    while (seqs_idx[i] < strlen(seqs[i])) {
      aligned_seqs[i][aligned_idx[i]] = seqs[i][seqs_idx[i]];
      seqs_idx[i]++;
      aligned_idx[i]++;
    }
  }
  // adjust the length of aligned sequences
  int max_alginment_length =
      max(max(aligned_idx[0], aligned_idx[1]), aligned_idx[2]);
  for (int i = 0; i < 3; i++) {
    aligned_idx[i] = max_alginment_length;
    aligned_seqs[i][max_alginment_length] = '\0';
  }

  // print the aligned sequences
  for (int i = 0; i < 3; i++) {
    printf("%s\n", aligned_seqs[i]);
  }
}

int main() {
  char *seq0 = "ATTGCCATT";
  char *seq1 = "ATGGCCATT";
  char *seq2 = "ATCCAAT";
  char *seqs[3] = {seq0, seq1, seq2};

  char *lcs = "ATCCAT";

  MSA(seqs, lcs);
  return 0;
}