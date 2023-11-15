#include "stdes.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	IOBUF_FILE *f1;
	IOBUF_FILE *f2;
	char* c;
	c =malloc(10);
	if (argc != 3)
		exit(-1);

	f1 = iobuf_open (argv[1], 'R');
	if (f1 == NULL)
		exit (-1);

	f2 = iobuf_open (argv[2], 'W');
	if (f2 == NULL)
		exit (-1);
	while (iobuf_read (c, 1, 1, f1) == 1) {
          iobuf_write(c,1,1,f2);
		  printf(" %s \n",c);
	}
/*        vider (stdout);*/

        iobuf_close (f1);
        iobuf_close (f2);

	return 0;
}
