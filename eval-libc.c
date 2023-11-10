#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  FILE *f1; // fichier pour lire
  FILE *f2; // fichier pour ecrire
  char c;

  if (argc != 3)
    exit(-1);

  f1 = fopen(argv[1], "r");
  if (f1 == NULL)
    exit(-1);

  f2 = fopen(argv[2], "w");
  if (f2 == NULL)
    exit(-1);

  while (fread(&c, 1, 1, f1) == 1) {
    fwrite(&c, 1, 1, f2);
  }

  fclose(f1);
  fclose(f2);

  return 0;
}
