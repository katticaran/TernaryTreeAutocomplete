#include "autocomplete.h"
#include "tern.h"
#include <ncurses.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>


tern_t node = { .word= 'a', .lChild = NULL, .mChild = NULL, .rChild = NULL };
tern_t* ROOT = &node;

/**
 * Add a line of input to our list of previously-seen inputs. Once added, this
 * string could be returned as a suggested input for a future line.
 * 
 * \param line  The line to add to our history
 */
void add_line(char* line) {
  
  int i;
  
  //getting rid of all the newline characters.
  for (i=0;i<strlen(line);i++)
    { if (line[i] == '\n') { line[i] = '\0';}}
  
  //adding the words to the tree
  insert_node(line,ROOT);
}

/**
 * Search our history for lines that begin with `input` and return up to
 * `max_suggestions` of them by writing pointers into the `suggestions` array.
 * If there are fewer than `max_suggestions` suggestions, write NULL into the
 * empty suggestion slots.
 * 
 * \param input             The currently typed line
 * \param suggestions       The array we should write suggestions into
 * \param max_suggestions   The maximum number of suggestions we should return.
 */
void get_suggestions(char* input, const char** suggestions, size_t max_suggestions) {
  // TODO: Write me!

  int i;
  //setting suggestions to null to prevent it from reusing previous results.
  for(int i=0; i<max_suggestions; i++) {
    suggestions[i] = NULL;
  }

  //if the first character is '\0', then then the quit condition is met.
  //so we free allocated memory and delete files.
  if (input[0] == '\n'){
    delete(ROOT, ROOT);
    remove("out.txt");
    remove("test.txt");
    return;
  }

  FILE* out = fopen("out.txt", "w");
  if (out == NULL){
    exit (1);
  }

  //searching the tree for words that begin with 'input'
  //and printing that out to 'out.txt'
  search(input, out, ROOT);
  fclose(out);

  
  out = fopen("out.txt", "r");
  if (out == NULL){
    exit (1);
  }
  
  //used for testing purposes
  FILE* test = fopen("test.txt","w+");
  
  i=0;
  char line[256];
  char* word[max_suggestions];

  //copying all the words to a buffer which will later be given to suggestions
  while (fgets(line, sizeof(line), out) && i<max_suggestions) {
    line[strcspn(line, "\n")] = 0;
    word[i] = malloc(sizeof(char)*256);
    if (word[i] == NULL){
      endwin();
      exit(1);
    }
    fprintf(test,"%s",word[i]);
    strcpy(word[i],line);
    i++;
  }
 
  fclose(out);
  fclose(test);

  //assigning the suggestions
  int j;
  for (j=0;j<i;j++)
    suggestions[j] = word[j];

  //freeing allocated memory.
  for (j =0; j<i; j++){
    free(word[j]);
  }

}
