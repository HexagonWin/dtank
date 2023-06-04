/* 비비에스 구버젼 menustruct를 신버젼으로 바꾸는 함수이다 */
#include "bbs.h"

struct config *oldmenu,*oldnmenu,*oldmtop,*oldmtmp;
struct oldcmenustruct *oldcmenu,*oldcnmenu,*oldcmtop, *oldcmtmp;
 
convertoldmenu() 
{
    get_oldmenu();
    printf("\r\n 옛메뉴화일을 읽었습니다.");
    writenewmenu();
    printf("\r\n 새로운 화일을 만들었습니다.");
    Get_Menu();
}
convertoldcmenu()
{
    get_oldcmenu();
    printf("\r\n 옛메뉴화일을 읽었습니다.");
    writenewcmenu();
    printf("\r\n 새로운 화일을 만들었습니다.");
    get_cmenu();
}

get_oldmenu()
{
    int i, j;
    char ch[2];
    FILE *fp1;
    char buf[5];

    if((fp1=fopen("bin/MENUS","r"))!=NULL) {
        if((oldmtmp = (struct config *)malloc(sizeof(struct config))) == NULL) {
            printf("\r\n메모리 부족");
            exit(1);
        }
        oldmenu =oldmtop = oldnmenu = oldmtmp;
        fread((char*)oldmtmp,sizeof(struct config),1,fp1);
        if((oldmtmp = (struct config *)malloc(sizeof(struct config))) == NULL) {
            printf("\r\n메모리가 부족");
            exit(1);
        }
        oldnmenu->neqp = oldnmenu->aeqp = NULL;
        oldnmenu->upp = oldnmenu;
        oldnmenu->downp = oldmtmp;
        while(fread((char*)oldmtmp,sizeof(struct config),1,fp1)) {
            if(oldnmenu->deep < oldmtmp->deep) {
                oldnmenu->downp = oldmtmp;
                oldmtmp->upp = oldnmenu;
                oldnmenu->neqp = oldmtmp->aeqp = NULL;
            }
            else if(oldnmenu->deep == oldmtmp->deep) {
                oldnmenu->downp = NULL;

                oldnmenu->neqp = oldmtmp;
                oldmtmp->upp = oldnmenu->upp;
                oldmtmp->aeqp = oldnmenu;
            }
            else if(oldnmenu->deep > oldmtmp->deep) {
                oldnmenu->downp = NULL;
                oldnmenu->neqp = NULL;
                j = oldnmenu->deep - oldmtmp->deep;
                for(i = 0; i < j; i++) {
                    oldnmenu = oldnmenu->upp;
                }
                oldnmenu->neqp = oldmtmp;
                oldmtmp->upp = oldnmenu->upp;
                oldmtmp->neqp = oldmtmp->downp = NULL;
                oldmtmp->aeqp = oldnmenu;
            }
            oldnmenu = oldmtmp;
            if((oldmtmp=(struct config *)malloc(sizeof(struct config))) == NULL) {
                printf("\r\n메모리 부족");
                exit(1);
            }
        }
        fclose(fp1);
        free(oldmtmp);
    }
}
get_oldcmenu()
{
    int i, j;
    char ch[2];
    FILE *fp1;
    char buf[80];
    sprintf(buf,"circle/CMENUS.%s",nmenu->ccode);
    if((fp1=fopen(buf,"r"))!=NULL) {
        if((oldcmtmp = (struct oldcmenustruct *)malloc(sizeof(struct oldcmenustruct))) == NULL) {
            printf("\r\n메모리 부족");
            exit(1);
        }
        oldcmenu =oldcmtop = oldcnmenu = oldcmtmp;
        fread((char*)oldcmtmp,sizeof(struct oldcmenustruct),1,fp1);
        if((oldcmtmp = (struct oldcmenustruct *)malloc(sizeof(struct oldcmenustruct))) == NULL) {
            printf("\r\n메모리가 부족");
            exit(1);
        }
        oldcnmenu->neqp = oldcnmenu->aeqp = NULL;
        oldcnmenu->upp = oldcnmenu;
        oldcnmenu->downp = oldcmtmp;
        while(fread((char*)oldcmtmp,sizeof(struct oldcmenustruct),1,fp1)) {
            if(oldcnmenu->deep < oldcmtmp->deep) {
                oldcnmenu->downp = oldcmtmp;
                oldcmtmp->upp = oldcnmenu;
                oldcnmenu->neqp = oldcmtmp->aeqp = NULL;
            }
            else if(oldcnmenu->deep == oldcmtmp->deep) {
                oldcnmenu->downp = NULL;
                oldcnmenu->neqp = oldcmtmp;
                oldcmtmp->upp = oldcnmenu->upp;
                oldcmtmp->aeqp = oldcnmenu;
            }
            else if(oldcnmenu->deep > oldcmtmp->deep) {
                oldcnmenu->downp = NULL;
                oldcnmenu->neqp = NULL;
                j = oldcnmenu->deep - oldcmtmp->deep;
                for(i = 0; i < j; i++) {
                    oldcnmenu = oldcnmenu->upp;
                }
                oldcnmenu->neqp = oldcmtmp;
                oldcmtmp->upp = oldcnmenu->upp;
                oldcmtmp->neqp = oldcmtmp->downp = NULL;
                oldcmtmp->aeqp = oldcnmenu;
            }
            oldcnmenu = oldcmtmp;
            if((oldcmtmp=(struct oldcmenustruct *)malloc(sizeof(struct oldcmenustruct))) == NULL) {
                printf("\r\n메모리 부족");
                exit(1);
            }
        }
        fclose(fp1);
        free(oldcmtmp);
    }
}
struct menustruct *smenu;
struct cmenustruct *scmenu;

writenewmenu()
{
    int i = 0,choice;
    FILE *fp1;
    char buf[80],buf2[80],buf3[80];
    if((smenu=(struct menustruct *)malloc(sizeof(struct menustruct)))==NULL) {
	printf("\r\n 메모리 부족 문제 ");
	exit(1);
    }
    oldmtop = oldnmenu = oldmenu;
    if((fp1 = fopen("bin/BBSMENUS","w")) != NULL) {
        oldmtop->deep = 0;
	/* 저장할 구조체 만듬*/ 
	  sprintf(smenu->choice,"%d",choice);
	  smenu->form=oldnmenu->form;
          smenu->level=oldnmenu->level;
          smenu->del=oldnmenu->del;
	  smenu->w=oldnmenu->w;
	  smenu->r=oldnmenu->r;
	  smenu->deep=oldnmenu->deep;
          strcpy(smenu->ccode,oldnmenu->ccode);
          strcpy(smenu->list,oldnmenu->list);
          strcpy(smenu->file,oldnmenu->file);
          smenu->neqp=oldnmenu->neqp;
	  smenu->aeqp=oldnmenu->aeqp;
	  smenu->downp=oldnmenu->downp;
          smenu->upp=oldnmenu->upp; 
   printf("\r\n 자 시작합니다.");
        fwrite((char*)smenu,sizeof(struct menustruct),1,fp1);
        printf("\r\n%8s %s",smenu->ccode,smenu->list);
        oldnmenu = oldnmenu->downp;
        ++i;choice=1;
        while(oldnmenu != oldmtop) {
            oldnmenu->deep = i;
		/*저장할 구조체를 만듬 */
	      sprintf(smenu->choice,"%d",choice);
              smenu->form=oldnmenu->form;
              smenu->level=oldnmenu->level;
              smenu->del=oldnmenu->del;
              smenu->w=oldnmenu->w;
              smenu->r=oldnmenu->r;
              smenu->deep=oldnmenu->deep;
              strcpy(smenu->ccode,oldnmenu->ccode);
              strcpy(smenu->list,oldnmenu->list);
              strcpy(smenu->file,oldnmenu->file);
              smenu->neqp=oldnmenu->neqp;
              smenu->aeqp=oldnmenu->aeqp;
              smenu->downp=oldnmenu->downp;
              smenu->upp=oldnmenu->upp; 

            fwrite((char*)smenu,sizeof(struct menustruct),1,fp1);
            printf("\r\n%3s %8s %s",smenu->choice,smenu->ccode,smenu->list);
            if(oldnmenu->downp == NULL) {
                if(oldnmenu->neqp == NULL) {
                    while(1) {
                        oldnmenu = oldnmenu->upp;
                        --i;
                        if(oldnmenu->neqp != NULL || i == 0) break;
                    }
                    if(i) oldnmenu = oldnmenu->neqp;choice++;
                }
                else {
                    oldnmenu = oldnmenu->neqp;choice++;
                }
            }
            else {
                oldnmenu = oldnmenu->downp;
                ++i;
            }
        }
        fclose(fp1);
    }
    else printf("\r\n화일이 열리지 않습니다.");
}
writenewcmenu()
{
    int i = 0,choice;
    FILE *fp1;
    char buf[80],buf2[80],buf3[80];
    if((scmenu=(struct cmenustruct *)malloc(sizeof(struct cmenustruct)))==NULL) {
        printf("\r\n 메모리 부족 문제 ");
        exit(1);
    }
    oldcmtop = oldcnmenu = oldcmenu;
    sprintf(buf,"circle/BBSCMENUS.%s",nmenu->ccode);
    if((fp1 = fopen(buf,"w")) != NULL) {
        oldcmtop->deep = 0;
        /* 저장할 구조체 만듬*/ 
          sprintf(scmenu->choice,"%d",choice);
          scmenu->form=oldcnmenu->form;
          scmenu->level=oldcnmenu->level;
          scmenu->del=oldcnmenu->del;
          scmenu->w=oldcnmenu->w;
          scmenu->r=oldcnmenu->r;
          scmenu->deep=oldcnmenu->deep;
          strcpy(scmenu->ccode,oldcnmenu->ccode);
          strcpy(scmenu->list,oldcnmenu->list);
          strcpy(scmenu->file,oldcnmenu->file);
	  strcpy(scmenu->manager,oldcnmenu->manager);
          scmenu->neqp=oldcnmenu->neqp;
          scmenu->aeqp=oldcnmenu->aeqp;
          scmenu->downp=oldcnmenu->downp;
          scmenu->upp=oldcnmenu->upp; 

   printf("\r\n 자 시작합니다.");
        fwrite((char*)scmenu,sizeof(struct cmenustruct),1,fp1);
        printf("\r\n%8s %s",scmenu->ccode,scmenu->list);
        oldcnmenu = oldcnmenu->downp;
        ++i;choice=1;
        while(oldcnmenu != oldcmtop) {
            oldcnmenu->deep = i;
                /*저장할 구조체를 만듬 */
              sprintf(scmenu->choice,"%d",choice);
              scmenu->form=oldcnmenu->form;
              scmenu->level=oldcnmenu->level;
              scmenu->del=oldcnmenu->del;
              scmenu->w=oldcnmenu->w;
              scmenu->r=oldcnmenu->r;
              scmenu->deep=oldcnmenu->deep;
              strcpy(scmenu->ccode,oldcnmenu->ccode);
              strcpy(scmenu->list,oldcnmenu->list);
              strcpy(scmenu->file,oldcnmenu->file);
	      strcpy(scmenu->manager,oldcnmenu->manager);
              scmenu->neqp=oldcnmenu->neqp;
              scmenu->aeqp=oldcnmenu->aeqp;
              scmenu->downp=oldcnmenu->downp;
              scmenu->upp=oldcnmenu->upp; 

            fwrite((char*)scmenu,sizeof(struct cmenustruct),1,fp1);
            printf("\r\n%3s %8s %s",scmenu->choice,scmenu->ccode,scmenu->list);
            if(oldcnmenu->downp == NULL) {
                if(oldcnmenu->neqp == NULL) {
                    while(1) {
                        oldcnmenu = oldcnmenu->upp;
                        --i;
                        if(oldcnmenu->neqp != NULL || i == 0) break;
                    }
                    if(i) oldcnmenu = oldcnmenu->neqp;choice++;
                }
                else {
                    oldcnmenu = oldcnmenu->neqp;choice++;
                }
            }
            else {
                oldcnmenu = oldcnmenu->downp;
                ++i;
            }
        }
        fclose(fp1);
    }
    else printf("\r\n화일이 열리지 않습니다.");
}


