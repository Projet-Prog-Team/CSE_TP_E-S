#include "stdes.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX_BUF 2048

IOBUF_FILE *iobuf_open(const char *nom, char mode)
{
    IOBUF_FILE* buf_file=malloc(sizeof(IOBUF_FILE));
    if(mode=='W')
    {
        buf_file->fd=open(nom,O_WRONLY);

    }
    else
    {
        buf_file->fd=open(nom,O_RDONLY);
    }
    buf_file->buf = malloc(MAX_BUF);
    buf_file->length=0;
    buf_file->mode=mode;
    buf_file->read_size=-1;
    return buf_file;
}

int iobuf_close(IOBUF_FILE*f)
{
    if(f != NULL && f->fd != -1 && f->buf != NULL)
    {
        if(f->mode == O_WRONLY && f->length!=0)
        {
            write(f->fd,f->buf,f->length);
            printf("s1: %s",f->buf);
        }
        free(f->buf);
        close(f->fd);
        free(f);
        return 1;
    }
    else
    {
        return 0;
    }
}

int iobuf_read(void *p, unsigned int taille, unsigned int nbelem, IOBUF_FILE *f)
{
    char * tmp = p;
    int i=0;
    if(f != NULL && f->fd != -1 && f->buf != NULL && f->read_size!=0)
    {
        //write if buf isn't empty
        if(f->mode == O_WRONLY && f->length!=0)
        {
            printf("s2:%s",f->buf);
            write(f->fd,f->buf,f->length);
            f->length=0;
        }
        f->mode=O_RDONLY;

        for(i =0 ;i < nbelem ; i++)
        {
            if(f->length+taille >=MAX_BUF || f->length==0) 
            {
                //if we've reach the limit of buf
                f->read_size=read(f->fd,f->buf,MAX_BUF);
                //printf("rs: %li\n",f->read_size);
                f->length=0;
                
            }
            if(f->read_size <= 0)
                    break;
            if(f->read_size<taille)
            {
                //if we're near the EOF

                //for each byte with a max size of taille
                for(int j=f->length;j<f->length+f->read_size;j++)
                    tmp[(j-(f->length))*i]=f->buf[j];
                f->length+=f->read_size;
                break;
            }
            else
            {
                //for each byte with a max size of taille
                for(int j=f->length;j<f->length+taille;j++)
                {
                    //printf("%i %i %i\n",f->length,(j-(f->length))+taille*i,j);
                    tmp[(j-(f->length))+taille*i]=f->buf[j];
                }
                f->length+=taille;
            }
            
            f->read_size-=taille;
            
        }
        //printf("i: %i,s: %s \n",i,tmp);
        return i;
    }
    else
    {
        return 0;
    }
}

int iobuf_write(const void *p, unsigned int taille, unsigned int nbelem, IOBUF_FILE *f)
{
    if(f != NULL && f->fd != -1 && f->buf != NULL)
    {
         if(f->mode == O_RDONLY && f->length!=0)
            f->length=0;
        f->mode=O_WRONLY;
        int i=0;
        for(i;i<nbelem;i++)
        {
            if(f->length+taille>=MAX_BUF)
            {
                //printf("s2:%s",f->buf);
                write(f->fd,f->buf,f->length);
                f->length=0;
            }
            for(int j=f->length;j<f->length+taille;j++)
            {
                f->buf[j]=((char*)p)[(j-(f->length))+taille*i];
            }
            f->length+=taille;
        }
        return i;
    }
    else
    {
        return 0;
    }
}

int iobuf_dump(IOBUF_FILE *f)
{
    if(f != NULL && f->fd != -1 && f->buf != NULL)
    {
        free(f->buf);
        f->buf = malloc(MAX_BUF);
        return 1;
    }
    return 0;
}