#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "algorithm.h"

int main(int argc, char **argv) {
  // Read input parameters: -i for input filepath, -o for output filepath. If no
  // output filepath is given, output will be printed to stdout.
  char *inpath = NULL;
  char *outpath = NULL;
  int c;

  while ((c = getopt(argc, argv, "i:o:")) != -1) {
    switch (c) {
    case 'i':
      inpath = optarg;
      break;
    case 'o':
      outpath = optarg;
      break;

    case '?':
    default:
      return 1;
    }
  }

  if (inpath == NULL) {
    return -EINVAL;
  }

  // Open files
  FILE *infile = fopen(inpath, "r");
  if (infile == NULL) {
    perror("Unable to open file!");
    exit(1);
  }

  FILE *outfile = NULL;
  if (outpath != NULL) {
    outfile = fopen(outpath, "w");
    if (outfile == NULL) {
      perror("Unable to open output file!");
    }
  }

  // Initialize algorithm
  struct algorithm_state state = {0};
  state.parameters.beta1 = -0.203;
  state.parameters.beta2 = 0.468;
  state.parameters.gamma = 0.67;

  fsm_init(&state, 11.4, 1);

  // Read lines of input file, update algorithm, print k
  char *line = NULL;
  size_t len = 0;

  while (getline(&line, &len, infile) != -1) {
    float number = 0;
    sscanf(line, "%f\n", &number);
    int k = update_algorithm(&state, number);
    if (outfile == NULL) {
      printf("%f, %d\n", number, k);
    } else {
      fprintf(outfile, "%f, %d\n", number, k);
    }
  }

  // Close all files and free memory
  fclose(infile);
  if (outfile != NULL) {
    fclose(outfile);
  }
  free(line);
}
