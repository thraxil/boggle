#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "stdint.h" /* Replace with <stdint.h> if appropriate */

#define MAX_LINE 100
#define ALPHA_SIZE 26

char *dictfile = "/usr/share/dict/words";

struct trie_node {
  struct trie_node *children[ALPHA_SIZE];
};

/* remove newlines */
void chomp(char *s) {
    while(*s && *s != '\n' && *s != '\r') s++;
    *s = 0;
}

int alpha_position(char c) {
  return c - 'a';
}

void initialize_trie_node(struct trie_node *t) {
  int i;
  for (i=0;i<ALPHA_SIZE;i++) t->children[i] = NULL;  
}

struct trie_node *talloc(void) {
  return (struct trie_node *) malloc(sizeof(struct trie_node));
}

void insert_into_trie(struct trie_node *root, char *string) {
  int position;
  int i;
  struct trie_node *child;
  for (i=0; i<strlen(string); i++) {
    position = alpha_position(string[i]);
    child = root->children[position];
    if (child == NULL) {
      child = talloc();
      initialize_trie_node(child);
      root->children[position] = child;
    } 
    root = child;
  }
}

void display_trie_children(struct trie_node *t) {
  int i;
  for (i=0; i<ALPHA_SIZE;i++) {
    if (t->children[i] != NULL) {
      printf("%c ",i + 'a');
    }
  }
  printf("\n");
}

int search_trie(struct trie_node *root, char *string) {
  int position;
  int i,j;
  for (i=0; i<strlen(string); i++) {
    position = alpha_position(string[i]);
    if (root->children[position] == NULL) {
      if (i == strlen(string) - 1) {
	return 1;
      } else {
	return 0;
      } 
    } else {
      root = root->children[position];
    }
  }
  return 1;
}

/**
 * removes newlines, lowercases
 * returns false if any non-alpha characters
 * are found in the string
 * this is to remove words with apostrophes, dashes,
 * accents, etc.
 */

int normalizeWord(char *s) {
  int i = 0;
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

struct trie_node *readWordList() {
  FILE *fp;
  char line[MAX_LINE];
  int lcount;
  int legal = 0;
  struct trie_node *root;
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
  printf("%s: %d\n","zygote",search_trie(root,"zygote"));
  printf("%s: %d\n","asdf",search_trie(root,"asdf"));
  return root;
}

void main(int argc, char **argv) {
  struct trie_node *root;
  root = readWordList();
  if (root == NULL) {
    printf("root is null\n");
  } else {
    printf("not null\n");
  }
  /*  root = talloc();
  insert_into_trie(root,"foo");
  insert_into_trie(root,"bar"); */
  printf("%s: %d","zygote",search_trie(root,"zygote"));
}
