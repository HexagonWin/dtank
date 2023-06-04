/* 새로운 게시판 보기이다. */
#include "bbs.h"
/* 이것이 기본 모듈이다. */
show_board(char *direc,char *fhead)
{
    FILE *fp1;
    char buf[255];
    int back,i;
    CRT = 15;
    sprintf(buf,"%s/%s",direc,fhead);
    if((fp1=fopen(buf,"r+"))==NULL) {
	fp1=fopen(buf,"a+");
    }
    li[0]='\0';
    lt[0]='\0';
    if(!fseek(fp1,0,2)) {
	now_num=top_num=ftell(fp1) /sizeof(struct dis_list);
    }
    else {
	printf("\r\n 화일이 열리지 않습니다.");
    }
    fclose(fp1);
    back = 0;
    reprint=YEA;
    esc_flag = 0;
    while(1) {
	if(reprint) {
	    cls();
	    printf("\r\n");
  	    titlelist(direc,fhead);
	    printf("%s",HELPLINE);
	    printf("\r\n %s>> ",fhead);
	    reprint=NA;
	}
	else {
	    for(i=0;i<back;i++) {
		printf("\b \b");
	    }
	}
	keyinput(buf,20);
	back = strlen(buf);
	if(nmenu->form =='f' || nmenu->form =='F') {docommand(buf,filecmd);}
	if(nmenu->form=='l' || nmenu->form=='L') {docommand(buf,boardcmd);}
 	if(buf[0]=='\0') {
	    if(now_num>0) { now_num=now_num-CRT;}
	    if(now_num<0) now_num=1;
	    reprint=YEA;
	}
	if(esc_flag) break;
    }
}

titlelist(char*direc, char*fhead)
{
    FILE *fp1;
    int i,linenum;
    char buf[255];
    struct tm *tp;
    linenum = now_num;
    if(linenum<=10 ) linenum=10;
    sprintf(buf,"%s/%s",direc,fhead);
    if((fp1=fopen(buf,"r"))==NULL) {
	printf("\r\n 화일이 열리지 않습니다. 시솝에게 말해주세요.");
   	printf("\r\n"); pressreturn();
	reprint=NA;
    }
    fseek(fp1,0,2); i = ftell(fp1) /sizeof(struct dis_list);
    if(i<=linenum) linenum=i;
    printf("게시판(%s) ",fhead);
    if(nmenu->form=='f' || nmenu->form=='F')
	printf("%s",FTITLELINE);
    else if(nmenu->form =='l' || nmenu->form=='L')
	printf("%s",TITLELINE);
    checkbufferint=1;
    for(i=0;i<CRT;i++) {
	if(fseek(fp1,(linenum-1)*sizeof(struct dis_list),0)) {
	    break;
	}
	else {
	    fread((char*)&l_title,sizeof(struct dis_list),1,fp1);
	    checkbufferint=showtitle(l_title);
	    if(!checkbufferint) i--;
	}
	linenum--;
    }
    fclose(fp1);
}

showtitle(struct dis_list title)
{
    struct tm *tp;
    if(nmenu->form=='l'|| nmenu->form=='L') {
	if((title.look>=0)) {
	    tp = localtime(&title.date);
            printf("\r\n%4d %-8s %02d/%02d/%02d%4d%3d%3d %-40s", 
              title.num,title.id,tp->tm_year,tp->tm_mon+1,tp->tm_mday,
              title.line,title.look,title.feel,title.title);
	    return 1;
	}
	else if(title.look <=0) {
	    if(pf.level >=cfg.del) {
		printf("\r\n 삭제되었음.");
	    }
	    return 1;
	}
    }
    else if(nmenu->form =='f' || nmenu->form =='F') {
        if((title.look>=0)) {
            tp = localtime(&title.date);
            printf("\r\n%4d %-8s %02d/%02d/%02d%4d%3d%3d %-40s",
              title.num,title.id,tp->tm_year,tp->tm_mon+1,tp->tm_mday,
              title.line,title.look,title.feel,title.title);
            return 1;
        }
        else if(title.look <=0) {
            if(pf.level >=cfg.del) {
                printf("\r\n 삭제되었음.");
            }
	    return 1;
        }
    }
	
}




