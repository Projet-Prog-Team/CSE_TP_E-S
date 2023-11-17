#ifndef _STDES_H
#define _STDES_H
#include <stdlib.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
struct _ES_FICHIER
{
    char * buf;
    int length;
    int fd;
    int mode;    
    int read_size;
};
typedef struct _ES_FICHIER IOBUF_FILE;

extern IOBUF_FILE *fstdout;
extern IOBUF_FILE *fstderr;

/* mode: 'R' = lecture, 'W' = écriture */
IOBUF_FILE *iobuf_open(const char *nom, char mode);
int iobuf_close(IOBUF_FILE*f);
int iobuf_read(void *p, unsigned int taille, unsigned int nbelem, IOBUF_FILE *f);
int iobuf_write(const void *p, unsigned int taille, unsigned int nbelem, IOBUF_FILE *f);
int iobuf_dump(IOBUF_FILE *f);

int fecriref (IOBUF_FILE *f, const char *format, ...);
/* directly in stdout */
int ecriref (const char *format, ...);
int fliref (IOBUF_FILE *f, const char *format, ...);

#endif

