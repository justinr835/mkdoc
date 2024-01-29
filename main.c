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
int nspace(char *in, char **out);
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
    free(p1);
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
      int filename_spaces = 0;
      if(strchr(basen, ' ') != NULL) {
        for(int i = 0; i < strlen(basen); i += 1) {
          if(basen[i] == ' ') filename_spaces += 1;
        }
      }
      char *document_filename = malloc(sizeof(char) * (strlen(basen) + 5));
      strcpy(document_filename, basen);
      strcat(document_filename, ".tex");
      char *n_filename = NULL;
      if(filename_spaces != 0) nspace(document_filename, &n_filename);
      char *pdf_filename;
      if(filename_spaces == 0) {
        pdf_filename = malloc(sizeof(char) * (strlen(document_filename) + 1));
        strcpy(pdf_filename, document_filename);
      }
      else {
        pdf_filename = malloc(sizeof(char) * (strlen(n_filename) + 1));
        strcpy(pdf_filename, n_filename);
      }
      pdf_filename[strlen(pdf_filename) - 3] = 'p';
      pdf_filename[strlen(pdf_filename) - 2] = 'd';
      pdf_filename[strlen(pdf_filename) - 1] = 'f';
      char *makefile_buffer;
      if(filename_spaces == 0) makefile_buffer = malloc(sizeof(char) * (strlen(document_filename) + strlen(pdf_filename) + 96));
      else makefile_buffer = malloc(sizeof(char) * (strlen(n_filename) + strlen(pdf_filename) + 96));
      strcpy(makefile_buffer, "CC = pdflatex\nDOC = ");
      if(filename_spaces == 0) strcat(makefile_buffer, document_filename);
      else strcat(makefile_buffer, n_filename);
      strcat(makefile_buffer, "\nPDF = ");
      strcat(makefile_buffer, pdf_filename);
      strcat(makefile_buffer, "\n\n$(PDF): $(DOC)\n\t$(CC) $(DOC)\n\nclean:\n\trm *.aux *.log\n.PHONY: clean");
      FILE *makefile = fopen("Makefile", "w");
      fwrite(makefile_buffer, 1, strlen(makefile_buffer), makefile);
      fclose(makefile); 
      if(n_filename != NULL) free(n_filename);
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

int nspace(char *in, char **out) {
  if(in != NULL) {
    int num_spaces = 0;
    for(int i = 0; i < strlen(in); i += 1) if(in[i] == ' ') num_spaces += 1;
    char *mm = malloc(sizeof(char) * (strlen(in) + num_spaces + 1));
    int j = 0;
    for(int i = 0; i < strlen(in); i += 1) {
      if(in[i] == ' ') {
        mm[j] = '\\';
        j += 1;
        mm[j] = ' ';
      }
      else mm[j] = in[i];
      j += 1;
    }
    *out = malloc(sizeof(char) * (strlen(mm) + 1));
    strcpy(*out, mm);
    free(mm);
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
