/*
tern.h by JOEL KATTICARAN.
Date: 28-Feb-2017

Purpose: This is the header file for the tern.c file that contains an implementation of a ternary tree with functions for creating and deleteing a tree as well as adding elements to the tree and finding elements from within the tree.

Based on code from caljim, with modifications/fixes
https://gist.github.com/caljim/4557872

Modifications/Fixes
** Fixed segfaults during search
** Fixed Logical error when searching for a word that already exists.
** Redirected output.
** Added guards for malloc fails.

Exit Codes
1- Memory allocation error.
 */

#ifndef _TERN_H
#define _TERH_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>


enum ntype {NO,YES};

typedef struct tern {
  char word;
  struct tern* lChild;
  struct tern* rChild;
  struct tern* mChild;
  enum ntype type; 
}tern_t;


//creats a tern_t, allocates memory, and initializes it.
tern_t* create_node (char w, enum ntype t);

//inserts a null-terminated word into the tree.
tern_t* insert_node(const char* string, tern_t* node);

//finds the node where the given prefix ends. Helper function for 'search'
tern_t* find_node(const char* string, tern_t* node);

//frees allocated memory. Note: Does not free the root node of the tree.
void delete(tern_t* root, tern_t* node);

//finds the words with prefix 'pattern' and prints out the results to 'out'.
void search(const char* pattern, FILE* out, tern_t* node);


#endif
