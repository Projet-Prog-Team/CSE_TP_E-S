#include <unistd.h>
#include <stdlib.h>
#include "stdes.h"

int main (int argc, char **argv)
{
  IOBUF_FILE *f1, *f2;
  int n;
  if (argc != 3) exit (-1);

  f1 = iobuf_open (argv[2], 'W');
  if (f1 == NULL) exit (-1);

  n = iobuf_fprintf (f1, "Test char: '%c' and string: '%s' with normal number 12\n", 'a', "bonjour");
  iobuf_fprintf (f1, "Test number: '%d' char written on the above line\n", n);

  for (n=-11;n<11;n++)
    fecriref (f1, "%d ", n);
  fecriref (f1, "\n", n);

  
  iobuf_close (f1);


}
