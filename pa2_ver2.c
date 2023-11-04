#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SEQUENCES 5
#define MAX_LENGTH 120

int sum(int *arr, int seqs_num) {
  int sum = 0;
  for (int i = 0; i < seqs_num; i++) {
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

void read_input_file(char **seqs, int *seqs_num) {
  FILE *file = fopen("input.txt", "r");
  // get the first line as seqs_num
  fscanf(file, "%d", seqs_num);
  // ignore the second line with dollar sign
  fscanf(file, "%*s");
  // read the sequences from 3rd line, and store them in seqs
  for (int i = 0; i < *seqs_num; i++) {
    seqs[i] = malloc(sizeof(char) * MAX_LENGTH);
    fscanf(file, "%s", seqs[i]);
  }
}

void MSA_write_output_file(char **seqs, int seqs_num, char *lcs) {
  int match[seqs_num];
  char aligned_seqs[seqs_num][MAX_LENGTH * seqs_num + 1];
  int seqs_idx[seqs_num];
  memset(seqs_idx, 0, sizeof(seqs_idx));
  int aligned_idx[seqs_num];
  memset(aligned_idx, 0, sizeof(aligned_idx));
  int lcs_len = strlen(lcs);
  int lcs_idx = 0;

  // Initialize the aligned sequences with '-'
  for (int i = 0; i < 3; i++) {
    memset(aligned_seqs[i], '-', sizeof(aligned_seqs[i]));
  }

  // align the sequences
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
        // Increment alignment index for all cases
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

  // write the output file
  FILE *file = fopen("output.txt", "w");
  for (int i = 0; i < 3; i++) {
    fprintf(file, "%s\n", aligned_seqs[i]);  // Write each aligned sequence
  }
  // Write "*" for the matched positions
  for (int j = 0; j < max_alginment_length; j++) {
    char c = aligned_seqs[0][j];
    int match = 1;
    for (int i = 1; i < 3; i++) {
      if (aligned_seqs[i][j] != c) {
        match = 0;
        break;
      }
    }
    fputc(match ? '*' : ' ', file);
  }

  fclose(file);
}

int main() {
  // Read the input file
  char *seqs[MAX_SEQUENCES];  // seqs[0], seqs[1], seqs[2] are the sequences
  int seqs_num;
  read_input_file(seqs, &seqs_num);

  // prints the sequences
  for (int i = 0; i < seqs_num; i++) {
    printf("%s\n", seqs[i]);
  }
  printf("%d", seqs_num);

  char *lcs = "ATCCAT";

  // Align the sequences according to the given LCS
  MSA_write_output_file(seqs, seqs_num, lcs);

  return 0;
}
