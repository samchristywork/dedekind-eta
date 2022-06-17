#include <complex.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*
 * An approximation of the Dedekind eta function
 */
double complex dedekind(double r, double i, int iterations) {
  double complex T = r + i * I;
  double complex q = cexp(2 * M_PI * I * T);

  double complex acc = 1;
  for (int n = 1; n < iterations; n++) {
    acc *= 1 - cpow(q, n);
  }

  return cpow(q, 1 / 24) * acc;
}

void usage(char *argv[]) {
  fprintf(stderr,
          "Usage: %s [-n iterations] [-w width] [-h height] [-o outFile]\n"
          " -n\tNumber of iterations.\n"
          " -w\tImage width.\n"
          " -h\tImage height.\n"
          " -o\tOutput file.\n"
          "",
          argv[0]);
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {

  int iterations = 10;

  int width = 100;
  int height = 100;

  FILE *outFile = stdout;

  /*
   * Handle program arguments.
   */
  int opt;
  char *optstring = "n:x:y:o:h";
  while ((opt = getopt(argc, argv, optstring)) != -1) {
    if (opt == 'n') {
      iterations = atoi(optarg);
    }
    if (opt == 'x') {
      width = atoi(optarg);
    }
    if (opt == 'y') {
      height = atoi(optarg);
    }
    if (opt == 'h') {
      usage(argv);
    }
    if (opt == 'o') {
      outFile = fopen(optarg, "wb");
      if (!outFile) {
        perror("fopen");
      }
    }
  }

  if (optind != argc) {
    fprintf(stderr, "Wrong number of arguments.\n");
    usage(argv);
  }

  if (isatty(fileno(outFile))) {
    fprintf(stderr, "Refusing to output to the terminal.\n\n");
    usage(argv);
  }

  unsigned char *p = malloc(width * height * 4);
  bzero(p, width * height * 4);

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      double complex f =
          dedekind((float)x / width / 10., (float)y / height / 10., iterations);
      p[(x + y * width) * 4 + 0] = creal(f) * 255;
      p[(x + y * width) * 4 + 1] = cimag(f) * 255;
      p[(x + y * width) * 4 + 2] = 0;
      p[(x + y * width) * 4 + 3] = 255;
    }
  }

  unsigned char header[18];
  bzero(header, sizeof(header));
  header[2] = 2;
  header[12] = 255 & width;
  header[13] = 255 & (width >> 8);
  header[14] = 255 & height;
  header[15] = 255 & (height >> 8);
  header[16] = 32;
  header[17] = 32;
  fwrite(header, sizeof(header), 1, outFile);
  fwrite(p, width * height * 4, 1, outFile);
  free(p);
}
