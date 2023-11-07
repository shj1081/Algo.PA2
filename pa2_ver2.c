#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SEQUENCES 5
#define MAX_LENGTH 130

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

int max_index(int *arr, int size) {
  int max = arr[0];
  int max_idx = 0;
  for (int i = 1; i < size; i++) {
    if (arr[i] > max) {
      max = arr[i];
      max_idx = i;
    }
  }
  return max_idx;
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

// node structure for memoization table
typedef struct node {
  int name[MAX_SEQUENCES];
  int lcs_len;
  struct node *next_memo;
  struct node *next[MAX_SEQUENCES];
  struct node *chosen;
} node;

// function for check arr doesn't cntains -1
int check_arr(int *arr, int size) {
  for (int i = 0; i < size; i++) {
    if (arr[i] == -1) {
      return 0;
    }
  }
  return 1;
}

// function for recursively calculating the lcs length
int calc_lcs_len(int seqs_num,
                 int seqs_char_index_arr[MAX_SEQUENCES][4][MAX_LENGTH],
                 node *****memo_table, int *name) {
  // if node is terminal node
  if (!check_arr(name, seqs_num)) {
    /*
    printf("name : %d %d %d %d %d \n", name[0], name[1], name[2], name[3],
           name[4]);
    printf("lcs_len : %d\n", 0);*/
    return 0;
  } else {
    // check if the name is in the memo table
    int match = 0;
    node *finder = memo_table[name[0]][name[1]][name[2]][name[3]];
    while (finder != NULL) {
      if (finder->name[4] == name[4]) {
        match = 1;
        break;
      }
      if (finder->next_memo == NULL) {
        break;
      }
      finder = finder->next_memo;
    }
    // if the name is in the memo table
    if (match) {
      /*printf("name : %d %d %d %d %d \n", name[0], name[1], name[2], name[3],
             name[4]);
      printf("lcs_len : %d\n", finder->lcs_len);*/
      return finder->lcs_len;
    }
    // if the name is not in the memo table
    else {
      node *new_node = malloc(sizeof(node));
      for (int i = 0; i < seqs_num; i++) {
        new_node->name[i] = name[i];
      }

      if (finder == NULL) {
        memo_table[name[0]][name[1]][name[2]][name[3]] = new_node;
      } else {
        finder->next_memo = new_node;
      }

      int next_name[4][MAX_SEQUENCES];
      // check the next node
      for (int i = 0; i < seqs_num; i++) {
        next_name[0][i] = seqs_char_index_arr[i][0][name[i]];
        next_name[1][i] = seqs_char_index_arr[i][1][name[i]];
        next_name[2][i] = seqs_char_index_arr[i][2][name[i]];
        next_name[3][i] = seqs_char_index_arr[i][3][name[i]];
      }

      int temp_max[4];
      for (int i = 0; i < 4; i++) {
        temp_max[i] = calc_lcs_len(seqs_num, seqs_char_index_arr, memo_table,
                                   next_name[i]);
      }
     /*printf("name : %d %d %d %d %d \n", name[0], name[1], name[2], name[3],
             name[4]);
      printf("temp_max : %d %d %d %d\n", temp_max[0], temp_max[1], temp_max[2],
             temp_max[3]);*/
      //printf("lcs_len : %d\n", max(temp_max, 4) + 1);
      new_node->lcs_len = max(temp_max, 4) + 1;
      return max(temp_max, 4) + 1;
    }
  }
}

// function for find the lcs of the given sequences
char *find_lcs(char **seqs, int seqs_num) {
  // build seqs_char_index_arr
  int seqs_char_index_arr[MAX_LENGTH][4][MAX_LENGTH];
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

  // printf seqs_char_index_arr
  for (int i = 0; i < seqs_num; i++) {
    for (int j = 0; j < 4; j++) {
      for (int k = 0; k < strlen(seqs[i]); k++) {
        printf("%d ", seqs_char_index_arr[i][j][k]);
      }
      printf("\n");
    }
    printf("\n");
  }

  // malloc memoization table of 4D array whose element is the pointer of node
  node *****memo_table = malloc(sizeof(node ****) * (MAX_LENGTH));
  for (int i = 0; i < MAX_LENGTH; i++) {
    memo_table[i] = malloc(sizeof(node ***) * (MAX_LENGTH));
    for (int j = 0; j < MAX_LENGTH; j++) {
      memo_table[i][j] = malloc(sizeof(node **) * (MAX_LENGTH));
      for (int k = 0; k < MAX_LENGTH; k++) {
        memo_table[i][j][k] = malloc(sizeof(node *) * (MAX_LENGTH));
      }
    }
  }

  // initialize the root node
  node *root = malloc(sizeof(node));
  for (int i = 0; i < seqs_num; i++) {
    root->name[i] = 0;
  }

  // calculate the lcs length
  int lcs_len =
      calc_lcs_len(seqs_num, seqs_char_index_arr, memo_table, root->name);

  printf("lcs_len: %d\n", lcs_len);
  char *lcs;



  return lcs;
}

// function for aligning the sequences and writing the
// output file
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
  // add null character to the end of the aligned sequences
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

// main function
int main() {
  char *seqs[MAX_SEQUENCES];
  int seqs_num;
  read_input_file(seqs, &seqs_num);
  char *lcs = find_lcs(seqs, seqs_num);
  // MSA_write_output_file(seqs, seqs_num, lcs);
  return 0;
}
