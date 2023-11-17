#include "stdes.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	IOBUF_FILE *f1;
	IOBUF_FILE *f2;
	char* c;
	int taille=1;
	c =malloc(taille);
	if (argc != 3)
		exit(-1);
	//fstdout=iobuf_open ("stdout", 'W');
	//fstderr=iobuf_open ("stderr", 'W');
	f1 = iobuf_open (argv[1], 'R');
	if (f1 == NULL)
		exit (-1);

	f2 = iobuf_open (argv[2], 'W');
	taille = atoi(argv[3]);
	if (f2 == NULL)
		exit (-1);
	while (iobuf_read (c, taille, 1, f1) == 1) {
          iobuf_write(c,taille,1,f2);
		  //printf("%c",c);
	}
/*        vider (stdout);*/

        iobuf_close (f1);
        iobuf_close (f2);

	return 0;
}
