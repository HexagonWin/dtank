/*  간단하고 쓸만한 텍스트 에디터 */
#include "bbs.h"

struct eline {
      char text[170];
      int num;
      struct eline *next;
      struct eline *prior;
      };
struct eline *begin;
struct eline *last;
struct eline *dls_store(),*find();

void patchup(), delete(), list(),save(),load();

int writetextfile(char *direc,char *filename)
{
     char buf[100], choice, fname[80];
     int linenum=1;

     begin=NULL;
     last=NULL;   /* zero length list */
     printf(\
"\r\n++0----+----1----+----2----+----3----+----4----+----5----+----6----+----7");
     enter(1); /* 처음 들어갈 때에는 1행부터 시작해본다. */

     do {
      choice=menu_select();
      switch(choice){
         case 1: printf("수정 삽입할 라인을 적어주세요: ");
             keyinput(buf,10);
             linenum=atoi(buf);
             enter(linenum);
             break;
         case 2: delete();
             break;
         case 3: list();
             break;
         case 4: sprintf(buf,"%s/%s",direc,filename);
             save(buf);
             return 1; /* inform that text is saved. */
         case 6: return 0;  /* inform that text is cancelled */
      }
     } while(1);
}

/* Select a menu option */
menu_select()
{
     char buf[80];
     int c;

    do {
	printf("\r\n 1.삽입 수정 2.라인삭제 3.보여주기 4.저장 6.취소 >> ");
        printf("\r\n Enter your choice: ");
        keyinput(buf,10);
        c=atoi(buf);
    } while(c<0 || c>6);
    return c;
}

/* Insert text beginning at specified line number. */
enter(int linenum)
{
    struct eline *info;
    
    for(;;){
	/* get memory to hold new line */
        info=(struct eline *) malloc(sizeof(struct eline));
        if(!info) {
            printf("\nOut of memory");
            return 0;
        }

        printf("\r\n%d:",linenum);
        keyinput(info->text,80);
        info->num=linenum;

       /* Enter a blank line to stop entry;
          otherwise insert into the list and fix up
          the line numbers using patchup.
       */
        if(!strcmp(info->text,".")) {
	    break;
	}
	if(info->text[0]=='\0') {
	    info->text[0]==' '; info->text[1]=='\0';
	}
        if(1 /* *info->text */){
            if(find(linenum))
                patchup(linenum,1); /* fix up old line nums */
            if(*info->text)
                begin=dls_store(info);
        }
        linenum++;
    }      /* entry loop */
    return linenum;
}

void patchup(int n,int incr)
    {
    struct eline *i;

    i=find(n);

    while(i){
        i->num=i->num+incr;
        i=i->next;
    }
    }
    struct eline *dls_store(i)
    struct eline *i;
    {
       struct eline *old, *p;

       if(last==NULL) {
          i->next=NULL;
          i->prior=NULL;
          last=i;
          return i;
       }

       p=begin;

       old=NULL;
       while(p) {
           if(p->num < i->num) {
               old=p;
               p=p->next;
           }
           else{
            if(p->prior){
                  (p->prior)->next=i;
                  i->next=p;
                  p->prior=i;
                  return begin;
            }
            i->next=p;
            i->prior=NULL;
            p->prior=i;
            return i;
           }
       }
       old->next=i;
       i->next=NULL;
       i->prior=old;
       last=i;
       return begin;
    }

    /* delete a line */
    void delete()
    {
     struct eline *info;
     char buf[80];
     int linenum;

     printf("\r\n삭제할 라인을 적어주세요: ");
     keyinput(buf,10);
     linenum=atoi(buf);
     info=find(linenum);
     if(info) {
          if(begin==info) {
              begin=info->next;
              if(begin)
                  begin->prior=NULL;
              else last=NULL;
          }
          else {
              info->prior->next=info->next;
              if(info!=last)
                   info->next->prior=info->prior;
              else last=info->prior;
          }
  free(info);
          patchup(linenum+1,-1);
     }
    }
    /* find a line of text */
    struct eline *find(int linenum)
    {
      struct eline *info;

      info = begin;
      while(info) {
      if(linenum == info->num)
        return info;
      info=info->next;
      }
      return NULL;
    }
    /* list the text */
    void list()
    {
       struct eline *info;
       info=begin;

       while(info) {
       printf("\r\n %d: %s",info->num,info->text);
       info=info->next;
}
       printf("\r\n");
    }

    /* save the file */
    void save(fname)
    char *fname;
    {
      struct eline *info;
      char *p;
      FILE *fp;

      if((fp=fopen(fname,"wb"))==NULL) {
     printf("cannot open file\n");
     exit(0);
      }
      printf("\r\n 등록하고 있습니다.");

      info=begin;
      while(info) {
      p=info->text;
      while(*p)
         putc(*p++,fp);
      putc('\r',fp);
      putc('\n',fp);
      info=info->next;
      }
      fclose(fp);
 }

 void load(char *fname)
 {
     register int size,lnct;
     struct eline *info, *temp;
     char *p;
     FILE *fp;

     if((fp=fopen(fname, "rb"))==NULL) {
     printf("cannot open file\n");
     return;
     }
     while(begin) {
       temp = begin;
       begin = begin->next;
       free(temp);
     }
     printf("\nloading file\n");

     size=sizeof(struct eline);
     begin=(struct eline *)malloc(size);
     if(!begin) {
      printf("Out of memory\n");
      return;
     }
     info = begin;
     p=info->text;
     lnct=1;
     while((*p=getc(fp))!=EOF) {
        p++;
        while((*p=getc(fp))!='\r')
          p++;
        getc(fp);
        *p='\0';
        info->num=lnct++;
        info->next=(struct eline *)malloc(size);
        if(!info->next) {
           printf("Out of memory\n");
           return;
        }
        info->prior=temp;
        temp=info;
        info=info->next;
        p=info->text;
     }
     temp->next=NULL;
     last=temp;
     free(info);
     begin->prior=NULL;
     fclose(fp);
 }

