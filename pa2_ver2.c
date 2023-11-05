#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SEQUENCES 5
#define MAX_LENGTH 120

// Return the sum of the given integer array
int sum(int *arr, int size) {
  int sum = 0;
  for (int i = 0; i < size; i++) {
    sum += arr[i];
  }
  return sum;
}

// Return the maximum value of the given integer array
int max(int *arr, int size) {
  int max = arr[0];
  for (int i = 1; i < size; i++) {
    if (arr[i] > max) {
      max = arr[i];
    }
  }
  return max;
}

// Read the input file and store the sequences in seqs
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

typedef struct node {
  int name[MAX_SEQUENCES];
  int lcs_len;
  struct node *next[4];
  struct node *chosen;
} node;

typedef struct memo_node {
  int name[MAX_SEQUENCES];
  struct memo_node *next;
} memo_node;

// functino for check arr cntains -1 
int check_arr(int *arr, int size) {
  for (int i = 0; i < size; i++) {
    if (arr[i] != -1) {
      return 1;
    }
  }
  return 0;
}

// function for recursively building the tree of nodes
void build_tree(node *current, int seqs_num,
                int seqs_char_index_arr[seqs_num][4][MAX_LENGTH]) {

  int arr_A[MAX_SEQUENCES];
  int arr_T[MAX_SEQUENCES];
  int arr_G[MAX_SEQUENCES];
  int arr_C[MAX_SEQUENCES];

  for (int i = 0; i < seqs_num; i++) {
    arr_A[i] = seqs_char_index_arr[i][0][current->name[i]];
    arr_T[i] = seqs_char_index_arr[i][1][current->name[i]];
    arr_G[i] = seqs_char_index_arr[i][2][current->name[i]];
    arr_C[i] = seqs_char_index_arr[i][3][current->name[i]];
  }

 if

  

}
// Find the longest common subsequence of the given sequences
char *find_lcs(char **seqs, int seqs_num) {
  int seqs_char_index_arr[seqs_num][4][MAX_LENGTH];
  memset(seqs_char_index_arr, -1, sizeof(seqs_char_index_arr));

  int start_idx_A, start_idx_T, start_idx_G, start_idx_C;

  for (int i = 0; i < seqs_num; i++) {
    start_idx_A = 0, start_idx_T = 0, start_idx_G = 0, start_idx_C = 0;
    for (int k = 0; k < strlen(seqs[i]); k++) {
      if (seqs[i][k] == 'A') {
        for (int idx = start_idx_A; idx <= k; idx++) {
          seqs_char_index_arr[i][0][idx] = k + 1;
        }
        start_idx_A = k + 1;
      } else if (seqs[i][k] == 'T') {
        for (int idx = start_idx_T; idx <= k; idx++) {
          seqs_char_index_arr[i][1][idx] = k + 1;
        }
        start_idx_T = k + 1;
      } else if (seqs[i][k] == 'G') {
        for (int idx = start_idx_G; idx <= k; idx++) {
          seqs_char_index_arr[i][2][idx] = k + 1;
        }
        start_idx_G = k + 1;
      } else if (seqs[i][k] == 'C') {
        for (int idx = start_idx_C; idx <= k; idx++) {
          seqs_char_index_arr[i][3][idx] = k + 1;
        }
        start_idx_C = k + 1;
      }
    }
  }

  // initialize the root node
  node *root = malloc(sizeof(node));
  for (int i = 0; i < seqs_num; i++) {
    root->name[i] = 0;
  }

  // current node for loop
  node *current = root;

  // recursively build the tree of nodes

  char *lcs;
  return lcs;
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
  for (int i = 0; i < seqs_num; i++) {
    memset(aligned_seqs[i], '-', sizeof(aligned_seqs[i]));
  }

  // align the sequences
  while (lcs_idx < lcs_len) {
    // Check match or not
    for (int i = 0; i < seqs_num; i++) {
      if (seqs[i][seqs_idx[i]] == lcs[lcs_idx]) {
        match[i] = 1;
      } else {
        match[i] = 0;
      }
      printf("%c %c %d\n", seqs[i][seqs_idx[i]], lcs[lcs_idx], match[i]);
    }

    // If all match or all mismatch
    if (sum(match, seqs_num) == seqs_num || sum(match, seqs_num) == 0) {
      for (int i = 0; i < seqs_num; i++) {
        if (seqs_idx[i] < strlen(seqs[i])) {
          aligned_seqs[i][aligned_idx[i]] = seqs[i][seqs_idx[i]];
          seqs_idx[i]++;
        }
        aligned_idx[i]++;
      }
      if (sum(match, seqs_num) == seqs_num) {
        lcs_idx++;
      }
    }

    // If partial match
    else {
      for (int i = 0; i < seqs_num; i++) {
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
  for (int i = 0; i < seqs_num; i++) {
    while (seqs_idx[i] < strlen(seqs[i])) {
      aligned_seqs[i][aligned_idx[i]] = seqs[i][seqs_idx[i]];
      seqs_idx[i]++;
      aligned_idx[i]++;
    }
  }

  // adjust the length of aligned sequences
  int max_alginment_length = max(aligned_idx, seqs_num);
  printf("%d\n", max_alginment_length);
  for (int i = 0; i < seqs_num; i++) {
    printf("%d\n", aligned_idx[i]);
  }

  for (int i = 0; i < seqs_num; i++) {
    aligned_idx[i] = max_alginment_length;
    aligned_seqs[i][max_alginment_length] = '\0';
  }

  // write the output file of the aligned sequences
  FILE *file = fopen("output.txt", "w");
  for (int i = 0; i < seqs_num; i++) {
    fprintf(file, "%s\n", aligned_seqs[i]);
  }

  // Write "*" for the matched positions
  for (int j = 0; j < max_alginment_length; j++) {
    char c = aligned_seqs[0][j];
    int match = 1;
    for (int i = 1; i < seqs_num; i++) {
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
  char *seqs[MAX_SEQUENCES];
  int seqs_num;
  read_input_file(seqs, &seqs_num);
  char *test = find_lcs(seqs, seqs_num);
  char *lcs = "ATCCAT";

  printf("seqs_num: %d\n", seqs_num);
  for (int i = 0; i < seqs_num; i++) {
    printf("%s\n", seqs[i]);
  }
  MSA_write_output_file(seqs, seqs_num, lcs);
  return 0;
}
