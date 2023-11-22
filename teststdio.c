#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	FILE *f1;
	FILE *f2;
	char* c;
	int taille=1;
	c =malloc(taille);
	if (argc != 3)
		exit(-1);
	//fstdout=iobuf_open ("stdout", 'W');
	//fstderr=iobuf_open ("stderr", 'W');
	f1 = fopen(argv[1], "r");
	if (f1 == NULL)
		exit (-1);

	f2 = fopen(argv[2], "w");
	if (f2 == NULL)
		exit (-1);
    int d;
    int r;
    char * f = malloc(200);
    r=fscanf(f1,"%s",f);
    f[199]='\0';
    printf("%d %d %s",d,r,f);
    fclose(f1);
    fclose(f2);
}