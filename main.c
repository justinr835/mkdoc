/* mkdoc by Justin Roach
 * https://github.com/justinr835/mkdoc
 */

#include <stdio.h>
#include <getopt.h>
#include <libgen.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>

int make_project(char *path);
int print_help(void);
int bad_usage(void);

int main(int argc, char **argv) {
  struct option longopts[] = {
    {"help", no_argument, 0, 'h'},
    {0, 0, 0, 0},
  };
  int opt, show_help, longindex;
  while((opt = getopt_long(argc, argv, ":h", longopts, &longindex)) != -1) {
    switch(opt) {
      case 'h':
        show_help = 1;
        break;
      default:
        return bad_usage();
        break;
    }
  }
  if(show_help == 1) return print_help();
  else if(argc == 1 || argc > 2) return bad_usage();
  else {
    int e = make_project(argv[1]);
    if(e != 0) return e;
  }
  return 0;
}

int make_project(char *path) {
  char *p1 = malloc(sizeof(char) * (strlen(path) + 1));
  strcpy(p1, path);
  char *dirn = dirname(p1);
  if(access(dirn, F_OK) != 0) {
    printf("Error: Missing parent directory \"%s\".\n", dirn);
    return 2;
  }
  else {
    chdir(dirn);
    free(p1);
    char *basen = basename(path);
    if(access(basen, F_OK) == 0) {
      printf("Error: \"%s\" already exists.", path);
      return 3;
    }
    else {
      mkdir(basen, S_IRWXU);
      chdir(basen);
      char *document_filename = malloc(sizeof(char) * (strlen(basen) + 5));
      strcpy(document_filename, basen);
      strcat(document_filename, ".tex");
      char *pdf_filename = malloc(sizeof(char) * (strlen(basen) + 5));
      strcpy(pdf_filename, basen);
      strcat(pdf_filename, ".pdf");
      char *makefile_buffer = malloc(sizeof(char) * (strlen(document_filename) + strlen(pdf_filename) + 96));
      strcpy(makefile_buffer, "CC = pdflatex\nDOC = ");
      strcat(makefile_buffer, document_filename);
      strcat(makefile_buffer, "\nPDF = ");
      strcat(makefile_buffer, pdf_filename);
      strcat(makefile_buffer, "\n\n$(PDF): $(DOC)\n\t$(CC) $(DOC)\n\nclean:\n\trm *.aux *.log\n.PHONY: clean");
      FILE *makefile = fopen("Makefile", "w");
      fwrite(makefile_buffer, 1, strlen(makefile_buffer), makefile);
      fclose(makefile); 
      free(pdf_filename);
      free(makefile_buffer);
      char *document_buffer = malloc(sizeof(char) * (75));
      strcpy(document_buffer, "\\documentclass{minimal}\n\n\\begin{document}\n\n\\noindent{hi}\n\n\\end{document}\n");
      FILE *document = fopen(document_filename, "w");
      fwrite(document_buffer, 1, strlen(document_buffer), document);
      fclose(document);
      free(document_buffer);
      free(document_filename);
    }
  }
  return 0;
}

int print_help(void) {
  printf("Usage: mkdoc [name]\n");
  return 0;
}

int bad_usage(void) {
  printf("Bad usage.\n");
  return 1;
}
