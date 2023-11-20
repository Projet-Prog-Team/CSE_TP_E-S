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
            //printf("s1: %s",f->buf);
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
        //CASE of taille > MAX_BUF

        for(i =0 ;i < nbelem ; i++)
        {
            if(f->read_size == -1){
                printf("1 :\n");
                fillbuff(f);
            }
            if(f->length+taille >=MAX_BUF)
            {
                //write the diff
                j=0;
                if(f->length !=0){
                    for(j=f->length;j<MAX_BUF;j++)
                    {
                        //printf("Pos1 : %d\n",(j-(f->length))+taille*i);
                        tmp[(j-(f->length))+taille*i]=f->buf[j];
                    }
                    offset=j-(f->length)+taille*i;
                    //if we've reach the limit of buf
                    printf("2 :\n");
                    fillbuff(f);
                }else{
                    int done = taille;
                    while(done >=MAX_BUF){
                        for(j=f->length;j<MAX_BUF;j++)
                        {
                            //printf("Pos1 : %d\n",(j-(f->length))+taille*i);
                            tmp[(j-(f->length))+taille*i]=f->buf[j];
                            done--;
                            if(done >=MAX_BUF){
                                break;
                            }
                        }
                        if(done <MAX_BUF){
                                fillbuff(f);
                        }
                    }
                    
                    offset=taille - done;
                    //if we've reach the limit of buf
                    fillbuff(f);
                }

            }
            
            if(f->read_size<nbelem*taille)
            {
                //if we're near the EOF
                //for each byte with a max size of taille
                j=0;
                for(j=f->length;j<f->length+f->read_size-offset;j++)
                {
                    tmp[((j-(f->length))+taille*i)+offset]=f->buf[j];
                }
                //if there is not enough data to insert into p, we add 0 after index j 
                fillnull(((j-(f->length))+taille*i),taille*nbelem,tmp);
                f->length+=f->read_size-offset;
                break;
            }
            else
            {         
                //for each byte with a max size of taille
                j=0;
                for(j=f->length;j<f->length+taille-offset && j<f->read_size;j++)
                {
                    tmp[((j-(f->length))+taille*i)+offset]=f->buf[j];
                }
                //if tmp isn't full
                if(i==nbelem-1 && ((j-(f->length))+taille*i)+offset != taille*nbelem)
                {
                    offset=(j-(f->length))+taille*i;
                    printf("3 :\n");
                    fillbuff(f);
                    if(f->read_size>=taille)
                    {
                        for(j=offset;j<taille-offset;j++)
                        {
                            tmp[j]=f->buf[j-offset];
                        }
                    }
                    else
                    {
                        //if there is not enough data to insert into p, we add 0 after index j 
                        j=offset;
                        for(j=offset;j<f->read_size;j++)
                        {
                            tmp[j]=f->buf[j-offset];
                        }
                        
                        fillnull(j,taille*nbelem,tmp);
                    }
                }
                f->length+=(taille-offset);
            }
            
            //f->read_size-=(taille-offset);
            
        }
        return i;
    }
    else
    {
        return 0;
    }
}

void fillbuff(IOBUF_FILE *f)
{
    f->length=0;
    f->read_size=read(f->fd,f->buf,MAX_BUF);
    printf("f-read %d\n",f->read_size);
}

void fillnull(int begin, int end, char* p)
{
    for(int j=begin;j<end;j++)
    {
        p[j]=0;
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

int fecriref (IOBUF_FILE *f, const char *format, ...)
{
    va_list args;
    int i=0;
    int l_i=0;
    int j=0;

    int d_tmp;
    char c_tmp;
    char* s_tmp;
    
    va_start(args,format);


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
                    //iobuf_write(&(atol(d_tmp)),sizeof(char),1,f);
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

}