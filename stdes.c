#include "stdes.h"

IOBUF_FILE _stdout= {.length=0,.fd=STDOUT_FILENO,.mode='W',.read_size=0};
IOBUF_FILE _stderr= {.length=0,.fd=STDERR_FILENO,.mode='R',.read_size=0};

IOBUF_FILE * stdout=&_stdout;
IOBUF_FILE * stderr=&_stderr;

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
    buf_file->length=0;
    buf_file->mode=mode;
    buf_file->read_size=-1;
    return buf_file;
}

int iobuf_close(IOBUF_FILE*f)
{
    if(f != NULL && f->fd != -1 && f->buf != NULL)
    {
        iobuf_flush(f);
        close(f->fd);
        //free(f);
        return 1;
    }
    else
    {
        return 0;
    }
}


int iobuf_flush(IOBUF_FILE *f)
{
    if(f != NULL && f->fd != -1 && f->buf != NULL)
    {
        if(f->mode == O_WRONLY && f->length!=0)
        {
            write(f->fd,f->buf,f->length);
        }
        f->length=0;
        f->read_size=-1;
        return 1;
    }
    return 0;
}

char* toString(int nb)
{
    int neg = 0;
    int tmp=0;
    int old_tmp=0;
    int i=10;
    int j=0;
    int k=0;
    char inv_char;
    char* result = malloc(31);
    if(nb<0)
    {
        nb=nb*-1;
        neg=1;
    }
    if(nb==0)
    {
        result[0]='0';
    }
    else
    {
        while(nb!=0)
        {
            old_tmp= nb/i;
            tmp=nb-old_tmp*i;
            result[j]=tmp+'0';
            nb=old_tmp;
            j++;
        }
    }

    for(k=0;k<j/2+neg;k++)
    {
        inv_char= result[k+neg];

        result[k+neg]=result[j-(k+1)];
        result[j-(k+1)]=inv_char;
    }
    if(neg==1)
        result[0]='-';
    //result[j+1]='\0';
    return result;
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
            if(f->length+taille >=MAX_BUF || f->read_size<=0) 
            {
                //write the diff
                j=0;
                for(j=f->length;j<MAX_BUF && f->length!=0;j++)
                {
                    //iobuf_printf(" o:%c\n",f->buf[j]);
                    tmp[(j-(f->length))+taille*i]=f->buf[j];
                }
                offset=j-(f->length)+taille*i;
                //if we've reach the limit of buf
                fillbuff(f);
            }
            if(f->read_size <= 0 || f->length>f->read_size)
                    break;
            if(f->read_size<nbelem*taille)
            {
                //if we're near the EOF
                //for each byte with a max size of taille
                j=0;
                for(j=f->length;j<f->length+f->read_size-offset;j++)
                {
                    //iobuf_printf(" a:%c\n",f->buf[j]);
                    tmp[((j-(f->length))+taille*i)+offset]=f->buf[j];
                }
                //if there is not enough data to insert into p, we add 0 after index j 
                //fillnull(((j-(f->length))+taille*i),taille*nbelem,tmp);
                f->length+=f->read_size-offset;
                break;
            }
            else
            {         
                //for each byte with a max size of taille
                j=0;
                
                for(j=f->length;j<f->length+taille-offset && j<f->read_size;j++)
                {
                    //iobuf_printf(" b:%c\n",f->buf[j]);
                    tmp[((j-(f->length))+taille*i)+offset]=f->buf[j];
                }
                //if tmp isn't full
                if(i==nbelem-1 && ((j-(f->length))+taille*i)+offset < taille*nbelem && f->read_size== MAX_BUF)
                {
                    //iobuf_printf("OUI ");
                    //iobuf_printf("OUI %d ",((j-(f->length))+taille*i)+offset);
                    offset=(j-(f->length))+taille*i;
                    fillbuff(f);
                    if(f->read_size>=taille)
                    {
                        for(j=offset;j<taille-offset;j++)
                        {
                            //iobuf_printf(" c:%c\n",tmp[j]);
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
                        
                        //fillnull(j,taille*nbelem,tmp);
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


int iobuf_fprintf (IOBUF_FILE *f, const char *format, ...)
{
    va_list* args=malloc(sizeof(va_list));
    int nb=0;
    va_start(*args,format);
    nb=inner_print(f,format,args);
    va_end(*args);
    char end = '\0';
    //iobuf_write(&end,1,1,f);
    iobuf_flush(f);
    free(args);
    return nb;
}

int inner_print(IOBUF_FILE *f, const char *format,va_list* args)
{
    int i=0;
    int l_i=0;
    int j=0;
    int nb=0;

    int d_tmp;
    char c_tmp;
    char* s_tmp;
    char* dstmp;
    char* tmp_p;
    while(format[i]!='\0')
    {
        if(format[i]=='%' && format[i+1]!='\0' )
        {
            iobuf_write(&format[l_i],1,i-l_i,f);
            
            switch(format[i+1])
            {
                case 'd':
                    d_tmp = va_arg(*args,int);   
                    s_tmp=toString(d_tmp);
                    j=0;
                    while(s_tmp[j]!='\0')
                        j++;
                    iobuf_write(s_tmp,sizeof(char),j,f);
                    nb++;
                break;

                case 'c':
                    c_tmp = va_arg(*args,int);
                    iobuf_write(&c_tmp,sizeof(char),1,f);
                    nb++;
                break;

                case 's':
                    s_tmp = va_arg(*args,char*);
                    j=0;
                    while(s_tmp[j]!='\0' || s_tmp[j]!=0)
                        j++;
                    iobuf_write(s_tmp,sizeof(char),j,f);
                    nb++;
                break;   

                default:     
                   tmp_p=malloc(2);
                   tmp_p[0]='%';
                   tmp_p[1]= format[i+1];
                   iobuf_write(tmp_p,sizeof(char),2,f);        
            }
            i++;
            l_i=i+1;
        }
        i++;
    }

    if(l_i!=i)
        iobuf_write(&format[l_i],1,i-l_i,f);    
    return nb;
}

int iobuf_printf (const char *format, ...)
{
    va_list* args=malloc(sizeof(va_list));
    int nb=0;
    va_start(*args,format);
    nb=inner_print(stdout,format,args);
    va_end(*args);
    char end = '\0';
    //iobuf_write(&end,1,1,stdout);
    iobuf_flush(stdout);
    free(args);
    return nb;
}

int iobuf_fscanf(IOBUF_FILE *f, const char *format, ...)
{
    const int LN_BUF=101;
    int nb_param=0;
    int ln=0;
    int i=0;
    int k=0;
    int b_break=1;
    int readed=0;
    va_list args;
    va_start(args,format);
    int nbvalid=0;
    char tmp;
    char old_tmp;

    char* buf = malloc(LN_BUF);
    char* proc_buf=malloc(LN_BUF);
    int* proc_int;
    char* c_result;
    char* s_result;
    while(format[i]!='\0')
    {
        if(format[i]=='%' && format[i+1]!='\0')
            if(format[i+1]=='d' || format[i+1]=='c' || format[i+1]=='s')
                nb_param++;
        ln++;
        i++;
    }
    i=0;
    if(nb_param*2==ln)
    {
        //if format doesn't have a specific format
        
        while(format[i]!='\0')
        {
            if(format[i]=='%' && format[i+1]!='\0')
                switch(format[i+1])
                {
                    case 'd':
                        k=0;
                        while(b_break>0)
                        {
                            //check there is a number   
                            old_tmp=tmp;
                            iobuf_read(&tmp,1,1,f);
                            if(tmp>='0' && tmp<='9')
                            {
                                proc_buf[k]=tmp;
                                k++;
                            }
                            if(old_tmp>='0' && old_tmp<='9' && (tmp<'0' || tmp>'9'))
                                b_break=0;
                            //fill again
                        }
                        b_break=1;
                        if(k==0)
                        {
                            //if there isn't a single number
                            proc_buf[0]='0';
                            proc_buf[1]='\0';
                        }
                        else
                        {
                            nbvalid++;
                            proc_buf[k]='\0';
                        }
                        proc_int=va_arg(args,int*);
                        *proc_int=atoi(proc_buf);                        
                    break;

                    case 's':
                        k=0;
                        s_result=va_arg(args,char*);
                        iobuf_read(&tmp,1,1,f);
                        while(k!=LN_BUF-1 && tmp!=' ' && tmp!='\t' && tmp!='\n')
                        {
                            s_result[k]=tmp;
                            iobuf_read(&tmp,1,1,f);
                            k++;
                        }
                        
                        if(k!=0)
                        {
                            s_result[k]=tmp;
                            nbvalid++;
                        }
                    break;

                    case 'c':
                        c_result=va_arg(args,char*);
                        if(f->read_size>0)
                        {
                            iobuf_read(c_result,1,1,f);
                            nbvalid++;
                        }
                    break;
                }
            i++;
        }
        return nbvalid;
    }
    else
    {
        //with a specific format
        return 0;
    }
    
}