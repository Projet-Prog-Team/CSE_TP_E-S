#include "stdes.h"

#define MAX_BUF 2048

IOBUF_FILE *iobuf_open(const char *nom, char mode)
{
    IOBUF_FILE* buf_file=malloc(sizeof(IOBUF_FILE));
    if(strcmp(nom,"stdout")==0 || strcmp(nom,"stderr")==0)
    {
        /*if(strcmp(nom,"stdout")==0)
        {
            buf_file->fd=open(0,O_WRONLY);
        }
        else
        {
            buf_file->fd=open(1,O_WRONLY);
        }*/

    }
    else
    {
        if(mode=='W')
        {
            buf_file->fd=open(nom,O_WRONLY);

        }
        else
        {
            buf_file->fd=open(nom,O_RDONLY);
        }
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
    int offset=0;
    int j=0;
    if(f != NULL && f->fd != -1 && f->buf != NULL && f->read_size!=0)
    {
        //write if buf isn't empty
        if(f->mode == O_WRONLY && f->length!=0)
        {
            write(f->fd,f->buf,f->length);
            f->length=0;
        }
        f->mode=O_RDONLY;

        for(i =0 ;i < nbelem ; i++)
        {
            if(f->length+taille >=MAX_BUF || f->read_size<=0) 
            {
                //write the diff
                j=0;
                for(j=f->length;j<MAX_BUF && f->length!=0;j++)
                {
                    tmp[(j-(f->length))+taille*i]=f->buf[j];
                }
                offset=j-(f->length)+taille*i;
                //if we've reach the limit of buf
                f->read_size=read(f->fd,f->buf,MAX_BUF);
                f->length=0;
            }
            if(f->read_size <= 0)
                    break;
            if(f->read_size<taille)
            {
                //if we're near the EOF
                //for each byte with a max size of taille
                printf("off: %i",offset);
                for(int k=f->length;k<f->length+f->read_size-offset;k++)
                    tmp[((k-(f->length))+taille*i)+offset]=f->buf[k];
                f->length+=f->read_size;
                break;
            }
            else
            {
                //for each byte with a max size of taille
                for(int k=f->length;k<f->length+taille-offset;k++)
                {
                    tmp[((k-(f->length))+taille*i)+offset]=f->buf[k];
                }
                f->length+=(taille-offset);
            }
            
            f->read_size-=(taille-offset);
            
        }
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

/*int fecriref (IOBUF_FILE *f, const char *format, ...)
{
    va_list args;
    
    int i=0;
    int l_i=0;
    int d_tmp;
    char c_tmp;
    char* s_tmp;
    int j=0;
    int nb=0;
    //count args
    while(format[i]!='\0')
    {
        if(format[i]=='%'&& format[i+1]!='\0' )
            if(format[i+1]=='d'||format[i+1]=='s'||format[i+1]=='c')
                nb++;
        i++;
    }
    i=0;
    va_start(args);


    while(format[i]!='\0')
    {
        if(format[i]=='%' && format[i+1]!='\0' )
        {
            iobuf_write(&format[l_i],1,i,f);
            l_i=i;
            switch(format[i+1])
            {
                case 'd':
                    d_tmp = va_arg(args,int);
                    iobuf_write(&d_tmp,sizeof(int),1,f);
                break;

                case 'c':
                    c_tmp = va_arg(args,int);
                    iobuf_write(&c_tmp,sizeof(char),1,f);
                break;

                case 's':
                    s_tmp = va_arg(args,char*);
                    j=0;
                    while(s_tmp[j]!='\0')
                        j++;
                    iobuf_write(&d_tmp,sizeof(char),j,f);
                break;                
            }
            i++;
        }
        i++;
    }
    if(l_i!=i)
        iobuf_write(&format[l_i],1,i,f);
    
    va_end(args);
    return 1;
}

int ecriref (const char *format, ...)
{

}*/