/*
tern.c by JOEL KATTICARAN.
Date: 28-Feb-2017

Purpose: This file contains an implementation of a ternary tree with functions for creating and deleteing a tree as well as adding elements to the tree and finding elements from within the tree.

Usage: See header file.

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


#include "tern.h"
#include <stdio.h>
#include <stdlib.h>

tern_t* create_node (char w, enum ntype t)
{
  tern_t *node = (tern_t*)malloc(sizeof (tern_t));
  if (node == NULL){
    perror("Error");
    exit(1);
  }
  node->word = w;
  node->lChild = NULL;
  node->mChild = NULL;
  node->rChild= NULL;
  node->type = t;
  return node;
}

tern_t* insert_node (const char* string, tern_t* node)
{
  int i = strlen(string);

  if(NULL == node)
    node = create_node(string[0], NO);

  if(string[0] < node->word)
    node->lChild = insert_node(string, node->lChild);

  else if(string[0] > node->word)
    node->rChild = insert_node(string, node->rChild);

  else {
    //go one level down the tree if the word has not been found here.
    if(i == 1) {
      node->type = YES;
      return node;
    } else
      node->mChild = insert_node(++string,node->mChild);
  }
  return node;
}



tern_t* find_node(const char* string, tern_t* node)
{
  int i = 0;
  tern_t* currentNode = node;

  while(i < strlen(string)) {
    if(currentNode == NULL)
      break;
    //look to the left of word
    if(string[i] < currentNode->word)
      currentNode = currentNode->lChild;
    //look to the right of word
    else if(string[i] > currentNode->word)
      currentNode = currentNode->rChild;
    //if out of characters, prefix ends on the current node. Now start search
    else {
      if(i++ == strlen(string) - 1)
	return currentNode;
      else
	currentNode = currentNode->mChild;
    }
  }

  return NULL;
}

void deep_search(const char* pattern, tern_t* start, FILE* out)
{

  if(start->type != NO)
    fprintf(out, "%s%c\n",pattern,start->word);

  if(start->lChild != NULL)
    deep_search(pattern, start->lChild, out);

  if(start->rChild != NULL)
    deep_search(pattern, start->rChild, out);

  if(start->mChild != NULL) {
       char *_pattern = (char *)malloc(strlen(pattern) + 2);
     sprintf(_pattern,"%s%c",pattern,start->word);
    deep_search(_pattern,start->mChild, out);
    free(_pattern);
  }
}

void search (const char* pattern, FILE* out, tern_t* node)
{
  //finds the node where the prefix ends.
  tern_t* current = find_node(pattern, node);

  if(NULL == current)
    return;
  else {
    if (current->mChild != NULL)
    deep_search(pattern,current->mChild, out);
  }
}

void delete(tern_t* root, tern_t* node){
  if (node != NULL){
    if (node->lChild != NULL){
      delete(root, node->lChild);
      node->lChild = NULL;
    }
    if (node->rChild != NULL){
      delete(root, node->rChild);
      node->rChild = NULL;
    }
    if (node->mChild != NULL){
      delete(root, node->mChild);
      node->mChild = NULL;
    }
    if((node->lChild == NULL) && (node->rChild==NULL) && (node->mChild==NULL)){
      if (node!=root){
	free(node);
      }
      return;
    }
  }
}
