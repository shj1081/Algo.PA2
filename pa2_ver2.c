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
  node *node_adrs;
  struct memo_node *next;
} memo_node;

// function for check arr doesn't cntains -1
int check_arr(int *arr, int size) {
  for (int i = 0; i < size; i++) {
    if (arr[i] == -1) {
      return 0;
    }
  }
  return 1;
}

// function for recursively building the tree of nodes using dynamic programming
void build_tree(node *current, node *terminal, memo_node *root_memo,
                memo_node *current_memo, int seqs_num,
                int seqs_char_index_arr[seqs_num][4][MAX_LENGTH + 1]) {
  // check if the current node is in the memoization table.
  // if not , then add the current node to the memoization table
  current_memo = root_memo;
  int memo_match = 0;
  while (current_memo->next != NULL) {
    for (int i = 0; i < seqs_num; i++) {
      if (current_memo->next->name[i] != current->name[i]) {
        break;
      }
      if (i == seqs_num - 1) {
        memo_match = 1;
        break;
      }
    }
    current_memo = current_memo->next;
  }

  if (memo_match == 0) {
    // printf("add to memo\n");
    current_memo->next = malloc(sizeof(memo_node));
    for (int i = 0; i < seqs_num; i++) {
      current_memo->next->name[i] = current->name[i];
    }
    current_memo->next->node_adrs = current;
    current_memo->next->next = NULL;
  }

  int next_name[4][MAX_SEQUENCES];
  for (int i = 0; i < seqs_num; i++) {
    next_name[0][i] = seqs_char_index_arr[i][0][current->name[i]];
    next_name[1][i] = seqs_char_index_arr[i][1][current->name[i]];
    next_name[2][i] = seqs_char_index_arr[i][2][current->name[i]];
    next_name[3][i] = seqs_char_index_arr[i][3][current->name[i]];
  }

  for (int i = 0; i < 4; i++) {
    if (check_arr(next_name[i], seqs_num)) {
      // check if the next node is in the memoization table
      // if it is, then set the next node to the node in the memoization table
      current_memo = root_memo;
      memo_match = 0;
      while (current_memo->next != NULL) {
        for (int j = 0; j < seqs_num; j++) {
          if (current_memo->next->name[j] != next_name[i][j]) {
            break;
          }
          if (j == seqs_num - 1) {
            memo_match = 1;
            break;
          }
        }
        if (memo_match == 1) {
          current->next[i] = current_memo->next->node_adrs;
          break;
        }
        current_memo = current_memo->next;
      }

      // if not in the memoization table, then recursively build the tree
      if (memo_match == 0) {
        current->next[i] = malloc(sizeof(node));
        for (int j = 0; j < seqs_num; j++) {
          current->next[i]->name[j] = next_name[i][j];
        }
        current->next[i]->lcs_len = -1;
        build_tree(current->next[i], terminal, root_memo, current_memo,
                   seqs_num, seqs_char_index_arr);
      }

      // if the next node is the terminal node, link the current node to the
      // terminal node
    } else {
      current->next[i] = terminal;
    }
  }
}

// function for recursively calculating the lcs length
void calculate_lcs(node *current, node *terminal) {
  if (current == terminal) {
    return;
  }

  for (int i = 0; i < 4; i++) {
    if (current->next[i]->lcs_len == -1) {
      calculate_lcs(current->next[i], terminal);
    }
  }

  int max_lcs_len = 0;
  for (int i = 0; i < 4; i++) {
    if (current->next[i] != terminal) {
      if (current->next[i]->lcs_len > max_lcs_len) {
        max_lcs_len = current->next[i]->lcs_len;
        current->chosen = current->next[i];
      }
    }
  }
  current->lcs_len = max_lcs_len + 1;
}

// Find the longest common subsequence of the given sequences
char *find_lcs(char **seqs, int seqs_num) {
  int seqs_char_index_arr[seqs_num][4][MAX_LENGTH + 1];
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

  // initialize the termianl node
  node *terminal = malloc(sizeof(node));
  for (int i = 0; i < seqs_num; i++) {
    terminal->name[i] = -1;
  }
  terminal->lcs_len = 1;

  // initialize the memoization table
  memo_node *root_memo = malloc(sizeof(memo_node));
  for (int i = 0; i < seqs_num; i++) {
    root_memo->name[i] = 0;
  }
  root_memo->node_adrs = root;

  memo_node *current_memo = root_memo;

  // recursively build the tree of nodes
  printf("build\n");
  build_tree(root, terminal, root_memo, current_memo, seqs_num,
             seqs_char_index_arr);

  printf("calculate\n");
  calculate_lcs(root, terminal);
  printf("lcs_len: %d\n", root->lcs_len);
  char *lcs = malloc(sizeof(char) * root->lcs_len);

  // appennd the seqs[0][current->chosen->name[0]-1] to lcs
  int lcs_idx = 0;
  node *current = root;
  while (current != terminal) {
    if (current->chosen != NULL) {
      lcs[lcs_idx] = seqs[0][current->chosen->name[0] - 1];
      lcs_idx++;
      current = current->chosen;
    } else {
      current = current->next[0]; // go to terminal node
    }
  }
  printf("lcs_len: %d\n", root->lcs_len);
  printf("lcs: %s\n", lcs);
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
  char *lcs = find_lcs(seqs, seqs_num);
  // char *lcs = "AACC";

  printf("seqs_num: %d\n", seqs_num);
  for (int i = 0; i < seqs_num; i++) {
    printf("%s\n", seqs[i]);
  }
  MSA_write_output_file(seqs, seqs_num, lcs);
  return 0;
}
