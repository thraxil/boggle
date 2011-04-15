#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "stdint.h" /* Replace with <stdint.h> if appropriate */

#define MAX_LINE 100
#define ALPHA_SIZE 26
#define BOARD_SIZE 4

char *dictfile = "/usr/share/dict/words";

typedef struct trie_node *TrieNodePtr;

typedef struct trie_node {
  int terminal;
  TrieNodePtr children[ALPHA_SIZE];
} TrieNode;

/* remove newlines */
void chomp(char *s) {
    while(*s && *s != '\n' && *s != '\r') s++;
    *s = 0;
}

int alpha_position(char c) {
  return c - 'a';
}

void initialize_trie_node(TrieNodePtr t) {
  int i;
  t->terminal = 0;
  for (i=0;i<ALPHA_SIZE;i++) t->children[i] = NULL;  
}

TrieNodePtr talloc(void) {
  return (TrieNodePtr) malloc(sizeof(TrieNode));
}

void insert_into_trie(TrieNodePtr root, char *string) {
  int position;
  int i;
  TrieNodePtr child;
  for (i=0; i<strlen(string); i++) {
    position = alpha_position(string[i]);
    child = root->children[position];
    if (child == NULL) {
      child = talloc();
      initialize_trie_node(child);
      root->children[position] = child;
    } 
    root = child;
    if (i == strlen(string) - 1) {
      root->terminal = 1;
    }
  }
}

void display_trie_children(TrieNodePtr t) {
  int i;
  for (i=0; i<ALPHA_SIZE;i++) {
    if (t->children[i] != NULL) {
      printf("%c ",i + 'a');
    }
  }
  printf("\n");
}

int search_trie(TrieNodePtr root, char *string) {
  int position, i;
  for (i=0; i<strlen(string); i++) {
    position = alpha_position(string[i]);
    if (root->children[position] == NULL) {
      if (i == strlen(string) - 1) return 0;
    } else {
      root = root->children[position];
    }
  }
  return root->terminal;
}

/**
 * removes newlines, lowercases
 * returns false if any non-alpha characters
 * are found in the string
 * this is to remove words with apostrophes, dashes,
 * accents, etc.
 */

int normalizeWord(char *s) {
  int c = 0;
  chomp(s);
  if (strlen(s) < 3) return 0;
  while(*s) {
    c = tolower(*s);
    if (!(c >= 'a' && (c <= 'z'))) return 0;
    *s++ = c;
  }
  return 1;
}

TrieNodePtr readWordList() {
  FILE *fp;
  char line[MAX_LINE];
  int lcount;
  int legal = 0;
  TrieNodePtr root = talloc();
  initialize_trie_node(root);

  fp = fopen(dictfile,"r");
  if (fp == NULL) {
      puts("cannot open file");
      return NULL;
    }
  while( fgets(line, sizeof(line), fp) != NULL ) {
    lcount++;
    legal = normalizeWord(line);
    if (legal) {
      insert_into_trie(root,line);
    } 
  }
 
  fclose(fp);  /* Close the file */
  return root;
}

struct board {
  char data[BOARD_SIZE][BOARD_SIZE];
};

struct board *readBoard(char *filename) {
  struct board *board = (struct board*)malloc(sizeof(struct board));

  FILE *fp;
  int row,col;
  char line[MAX_LINE];
  char c;

  fp = fopen(filename,"r");
  if (fp == NULL) {
    puts("cannot read board");
    return NULL;
  }
  row = 0;
  while( fgets(line, sizeof(line), fp) != NULL ) {
    for(col=0; col<strlen(line); col++) {
      c = line[col];
      if (c >= 'a' && c <= 'z') {
	board->data[row][col] = c;
      }
    }
    row++;
  }
  fclose(fp);
  return board;
}

int main(int argc, char **argv) {
  TrieNodePtr root;
  root = readWordList();
  struct board *board;
  board = readBoard("board.txt");
  if (root == NULL) {
    printf("root is null\n");
  } else {
    printf("%s: %d\n","zygote",search_trie(root,"zygote"));
    printf("%s: %d\n","zygo",search_trie(root,"zygo"));
    printf("%s: %d\n","zygotef",search_trie(root,"zygotef"));
    printf("%s: %d\n","asdf",search_trie(root,"asdf"));
    printf("%s: %d\n","als",search_trie(root,"als"));
    printf("%s: %d\n","also",search_trie(root,"also"));
    printf("%s: %d\n","alsop",search_trie(root,"alsop"));
  }
  return 0;
}
