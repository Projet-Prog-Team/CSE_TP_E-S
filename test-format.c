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
  char * tmp;

  n = iobuf_fprintf ( f1,"Test char: '%c' and string: '%s' with normal number 12\n", 'a', "bonjour");
  iobuf_fprintf (f1,"Test number: '%d' char written on the above line\n", n);
  iobuf_printf ("Test number: '%d' testing in stdout\n", 2);
  iobuf_printf ("Test number: '%d' testing with much parameters than need\n", 3,tmp,n);
  iobuf_printf ("Test number: '%d' testing with unknown % %n %k\n", 4);

  for (n=-11;n<11;n++)
    iobuf_fprintf (f1, "%d ", n);
  iobuf_fprintf (f1,"\n", n);

  
  iobuf_close (f1);


}
