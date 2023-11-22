#include "stdes.h"
#include <stdlib.h>

int main(int argc, char *argv[])
{
	IOBUF_FILE *f1;
	IOBUF_FILE *f2;
	char* c;
	int taille=1;
	c =malloc(taille);
	if (argc == 4)
		taille = atoi(argv[3]);
	if (argc < 3)
	{
		iobuf_printf("please add a size between 1 and %d",MAX_BUF);
		exit(-1);
	}
	//fstdout=iobuf_open ("stdout", 'W');
	//fstderr=iobuf_open ("stderr", 'W');
	f1 = iobuf_open (argv[1], 'R');
	if (f1 == NULL)
		exit (-1);

	f2 = iobuf_open (argv[2], 'W');

	if (f2 == NULL)
		exit (-1);
	iobuf_printf("Printing an entire file to another one (with a taille=%d): \n !!! be warned, C isn't clear every time, so the last read might have undesirable char!!! \n",taille);
	while (iobuf_read (c, taille, 1, f1) == 1) {
          iobuf_write(c,taille,1,f2);
		  //iobuf_printf("%s",c);
	}
/*        vider (stdout);*/
        iobuf_close (f1);
        iobuf_close (f2);

	return 0;
}
