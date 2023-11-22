#include "stdes.h"
#include <stdlib.h>

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
	if (f2 == NULL)
		exit (-1);
/*        vider (stdout);*/

	//can't use without malloc
    int* test_d=malloc(sizeof(int));
	int* test_d2=malloc(sizeof(int));
	char* test_c=malloc(1);//=malloc(1);
    char* test_s = malloc(200);
	char* test_s2 = malloc(200);
    //iobuf_fscanf(f1,"%d",test_d);
	iobuf_printf("Test avec un Lorem Ipsum\n");
	iobuf_printf("Test du char: \n");
    iobuf_fscanf(f1,"%c",test_c);
	iobuf_printf("%c\n",*test_c);
	iobuf_printf("Test du string: \n");
    iobuf_fscanf(f1,"%s",test_s);
	iobuf_printf("%s\n",test_s);
	iobuf_printf("Test du integer: \n");
    iobuf_fscanf(f1,"%d",test_d);
	iobuf_printf("%d\n",*test_d);
	iobuf_close (f1);
	f1 = iobuf_open (argv[1], 'R');
	iobuf_printf("Test de format sans caractère: (lis un entier puis un string puis un entier)\n");
    iobuf_fscanf(f1,"%d%s%d",test_d,test_s,test_d2);
	iobuf_printf("%d %s %d\n",*test_d,test_s,*test_d2);
    iobuf_close (f1);
    iobuf_close (f2);

	return 0;
}
