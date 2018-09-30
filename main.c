#define _GNU_SOURCE
#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>


#define MAX_LINE_LENGTH 512
#define MAX_SUGGESTIONS 50

void read_lines(int argc, char** argv);
void add_line(const char* line);
void get_suggestions(char* input, char** suggestions, size_t num_suggestions);

int main(int argc, char** argv) {
  bool done = false;
  
  // Initialize our weird input method
  initscr();
  
  // Don't display typed characters
  noecho();
  
  // Read in input lines from any files passed in as command line arguments
  read_lines(argc, argv);
  
  // Repeatedly read in user input until the program quits
  while(!done) {
    // Clear the screen
    clear();
    
    // Use a fixed-length buffer to read in a line
    char buffer[MAX_LINE_LENGTH];
    
    // Set the buffer to be an empty string
    buffer[0] = '\0';
    
    // Track the current index in the string
    size_t index = 0;
    
    // Track the last character typed
    int c;
    
    do {
      // Print the string we've read so far
      mvprintw(0, 0, "> %s", buffer);
      
      // Wait for a character
      c = getch();
      
      // Clear previous suggestions
      clear();
      
      // Was the character a backspace?
      if(c == KEY_BACKSPACE || c == KEY_DC || c == 127) {
        // Go back one space and overwrite the last character with a zero
        if(index > 0) {
          index--;
          mvprintw(0, 2+index, " ");
          buffer[index] = '\0';
        }
      } else {
        // Add the character to the string
        buffer[index] = c;
        
        // Move to the next character
        index++;
        
        // Put a null character at the end
        buffer[index] = '\0';
      }
      
      // Get suggested strings
      char* suggestions[MAX_SUGGESTIONS];
      get_suggestions(buffer, suggestions, MAX_SUGGESTIONS);
      
      for(int i=0; i<MAX_SUGGESTIONS; i++) {
        if(suggestions[i] != NULL) {
          mvprintw(i+2, 0, "%d. %s", i+1, suggestions[i]);
        }
      }
      
    } while(c != '\n' && index < MAX_LINE_LENGTH-1);
    
    // If we get a blank line (newline only), quit. Otherwise add the line to our history.
    if(strlen(buffer) == 1) {
      done = true;
    } else {
      add_line(buffer);
    }
  }
  
  // Shut down cleanly
  endwin();
  
  return 0;
}



void read_lines(int argc, char** argv) {
  // Loop over all command line arguments to the program
  for(int i=1; i<argc; i++) {
    // Open the file and read it line by line
    FILE* f = fopen(argv[i], "r");
    
    if(f == NULL) {
      fprintf(stderr, "Unable to open file %s\n", argv[i]);
      continue;
    }
    
    char* line = NULL;
    size_t line_length;
    ssize_t bytes_read;
    
    // Read until we run out of lines
    while((bytes_read = getline(&line, &line_length, f)) != -1) {
      // Does the line end in a newline? If so, remove it.
      char* pos = strchr(line, '\n');
      if(pos != NULL) {
        *pos = '\0';
      }
      
      // Is the line at least one character long?
      if(strlen(line) > 0) {
        add_line(line);
      }
    }
    
    if(line != NULL) {
      free(line);
    }
  
    // Close the file
    fclose(f);
  }
}
