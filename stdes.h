#ifndef _STDES_H
#define _STDES_H
#include <stdlib.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define MAX_BUF 2048
struct _ES_FICHIER
{
    char buf[MAX_BUF];
    int length;
    int fd;
    int mode;    
    int read_size;
};
typedef struct _ES_FICHIER IOBUF_FILE;

extern IOBUF_FILE *stdout;
extern IOBUF_FILE *stderr;

/* mode: 'R' = lecture, 'W' = écriture */
IOBUF_FILE *iobuf_open(const char *nom, char mode);
int iobuf_close(IOBUF_FILE*f);
int iobuf_read(void *p, unsigned int taille, unsigned int nbelem, IOBUF_FILE *f);
int iobuf_write(const void *p, unsigned int taille, unsigned int nbelem, IOBUF_FILE *f);
int iobuf_flush(IOBUF_FILE *f);

void fillnull(int begin, int end, char* p);
void fillbuff(IOBUF_FILE *f);
char* toString(int nb);
void inner_print(IOBUF_FILE *f, const char *format,va_list* args);

int iobuf_fprintf(IOBUF_FILE *f, const char *format, ...);
/* directly in stdout */
int iobuf_printf(const char *format, ...);
int iobuf_fscanf(IOBUF_FILE *f, const char *format, ...);

#endif

