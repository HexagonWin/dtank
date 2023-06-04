
#include "bbs.h"
#include "ccommands.h"
/*  동호회 안에서의 명령어 */
/*  M,m 서브메뉴위치와 chatting위치에서의 명령어 받는 함수 */
cdomenucommand(cmd,cmdtable)
char *cmd;
struct commands cmdtable[];
{
    int i;
    int len;
        for(i=0;i<100;i++) {   
            if(cmdtable[i].cmdname==NULL) break;
            len=strlen((char*)cmdtable[i].cmdname);
            if(!strncmp((char*)cmdtable[i].cmdname,cmd,len)&&\
	 pf.level>=cmdtable[i].level) {
                (*cmdtable[i].cmdfunc)(cmd);
                break;
            }
	    else reprint=NA;
        }
}
/* 메뉴 형태 F,f L,l 에서의 명령어 받는 함수 */
cdocommand(cmd,cmdtable)
char *cmd;
struct commands cmdtable[];
{
    int i;
    int len;
    
        for(i=0;i<100;i++) {   
            if(cmdtable[i].cmdname==NULL) break;
            len=strlen((char*)cmdtable[i].cmdname);
            if(!strncmp((char*)cmdtable[i].cmdname,cmd,len) && \
		pf.level >= cmdtable[i].level ) {
                (*cmdtable[i].cmdfunc)(cmd);
                break;
            }
        }
}
cwriteboard()
{
    int num;
    FILE *fp1;
    char buf[80];
    struct dis_list title;

    if((cnmenu->form == 'L' && cnmenu->w <= pf.level) ||
	(cnmenu->form == 'l' && ((circlecheck) || cnmenu->w == 0))) {
	printf\
	("\r\n 어떤 에디터를 사용하시겠습니까?(1:라인 2:vi 3:Z모뎀 0:취소) ");
	keyinput(buf,3);
	if(buf[0] =='1') {
	    sprintf(buf,"circle/%s/boards",nmenu->ccode);
	    num=lineeditor(buf,cnmenu->ccode);
	}
	else if(buf[0] =='2') {
	    sprintf(buf,"circle/%s/boards",nmenu->ccode);
	    num = visual_editor(buf);
	}
	else if(buf[0] =='3') {
	    printf("\r\n 아스키파일만 됩니다.");
	    sprintf(buf,"circle/%s/boards",nmenu->ccode);
	    uploadtext(buf);
	}
	else if(buf[0] =='0') {
	    reprint=YEA;
	    printf("\r\n 취소되었습니다. ");
	    return ;
	}
	else {
	    printf("\r\n 라인에디터로 하겠습니다.");
	    sprintf(buf,"circle/%s/boards",nmenu->ccode);
	    num = lineeditor(buf,cnmenu->ccode);
	}
	reprint=num;
        now_num = top_num = num;
    }
    else if((cnmenu->form == 'F' && \
	(!strcmp(pf.id,"sysop")||!strcmp(pf.id,"darker"))) || \
        (cnmenu->form == 'f' && \
	(!strcmp(pf.id,"sysop")||!strcmp(pf.id,"darker")) ) ) {
	sprintf(buf,"circle/%s/fboards",nmenu->ccode);
	num=line_editor(buf);
	reprint=num;
	now_num=top_num=num;
    }
    else {
        mess_dis(4);
        reprint=NA;
	return;
    }
    level += 1; /* 글을 쓰므로 오르는 레벨 */
    if (cnmenu->form=='l'||cnmenu->form=='L') 
	sprintf(buf,"circle/%s/boards/%s",nmenu->ccode,cnmenu->ccode);
    else if (cnmenu->form=='f' || cnmenu->form=='F')
	sprintf(buf,"circle/%s/fboards/%s",nmenu->ccode,cnmenu->ccode);
    if((fp1=fopen(buf,"r"))==NULL) {
	printf("\r\n 화일헤더가 안 열립니다.");
	pressreturn(); return;
    }
    fseek(fp1,(num-1)*sizeof(struct dis_list),0);
    fread((char*)&title,sizeof(struct dis_list),1,fp1);
    report("write",title);
    fclose(fp1);
}




/* -----------------------------동호회 안에서의 명령어 */
cuppermenu()
{
    esc_flag=esc_sub=1;
    if (!strcmp(cnmenu->ccode,"dongari")) {
	esc_circle=1;
	upermenu=1; 
   }
    cnmenu=cnmenu->upp;
    reprint=YEA;
    relogo=NA;
}
cnextmenu()
{
    if(cnmenu->neqp !=NULL) {
        cnmenu=cnmenu->neqp;
        if((cnmenu->form=='R' || cnmenu->form=='E')\
	 && (cnmenu->w > pf.level)) {
            cnmenu=cnmenu->upp;
        }
        esc_flag=1;
        reprint=YEA;
    }
    else {
        mess_dis(1);
        reprint=NA;
    }
}

cpremenu()
{
    if(cnmenu->aeqp!=NULL) {
        cnmenu=cnmenu->aeqp;
        if((cnmenu->form=='R' || cnmenu->form == 'E') &&\
	 (cnmenu->w > pf.level)) {
            cnmenu=cnmenu->upp;
        }
        esc_flag=1;
        reprint=YEA; 
   }
    else {
        mess_dis(1);
        reprint=NA;
    }
}
/* 동호회 자료실 관련 */
cupfile()
{
    FILE *fp1;
    char buf[80],buf2[80];

    sprintf(time_chk,"tmp/STAT.%s",tty);
    tfp = fopen(time_chk,"w");
    fputc('1',tfp);
    fclose(tfp);
    sprintf(buf,"circle/%s/files",nmenu->ccode);
    sprintf(buf2,"circle/%s/fboards",nmenu->ccode);
    
    uploadfile(buf,buf2,cnmenu->ccode);
    sprintf(time_chk,"tmp/STAT.%s",tty);
    fputc('0',tfp);
    fclose(tfp);
}

cdownfile(cmd)
char *cmd;
{
    char buf[80],buf2[80];

    if( (cnmenu->form=='f' && !circlecheck) ) {
	printf("\r\n 자격이 없습니다. ");
	return;
    }
    sprintf(time_chk,"tmp/STAT.%s",tty);
    tfp = fopen(time_chk,"w");
    fputc('1',tfp); /* 다운로드 중이라는 것을 알림 */
    fclose(tfp);
    sprintf(buf,"circle/%s/fboards",nmenu->ccode);
    sprintf(buf2,"circle/%s/files",nmenu->ccode);
    downloadfile(cmd,buf,cnmenu->ccode,buf2);
    sprintf(time_chk,"tmp/STAT.%s",tty);
    tfp = fopen(time_chk,"w");
    fputc('0',tfp); /* 업로드 중이라는 것을 알림 */
    fclose(tfp);
}
/* 동호회에서 지원되는 go명령 */

cgotomenu(cmd)
char *cmd;
{
    if(go_select(cmd)) { /* 동호회 밖에서 검사해서 있으면 동호회를 
				나간다. */
        esc_circle = 2;
	esc_flag=1;
        reprint=YEA;
    }
    else {
        mess_dis(1);
        reprint = NA;
    }
}
/* 동호회 게시판을 정리하는 명령이다. */
cboardsort()
{
    char buf[80];
    char buf2[80];
    sprintf(buf,"%s",cnmenu->ccode);
    if(cnmenu->form=='l'||cnmenu->form=='L') {
	sprintf(buf2,"circle/%s/boards",nmenu->ccode);
	board_sort(buf2,buf);
    }
}
cfilesort()
{
    char direc[80];
    char fhead[80];
    char filedir[80];
    sprintf(fhead,"%s",cnmenu->ccode);
    if(cnmenu->form=='f' || cnmenu->form=='F') {
	sprintf(direc,"circle/%s/fboards",nmenu->ccode);
	sprintf(filedir,"circle/%s/files",nmenu->ccode);
	files_sort(direc,fhead,filedir);
    }
}
delcboard(char *cmd)
{
    char buf[80],direc[80],fhead[80];
    int cnum=0; /* howmany commands are inputted. */
    int num,i;

    cnum = parser(cmd);
    
    for (i=0;i<cnum;i++) {
	sprintf(direc,"circle/%s/boards",nmenu->ccode);
	strcpy(fhead,cnmenu->ccode);
  	num = atoi(argument[i]);
	del_text(direc,fhead,num);
    }
}
delcfboard(char *cmd)
{
    char buf[80],direc[80],fhead[80];
    int cnum=0;
    int num,i,start,end;
    cnum = parser(cmd);
    if((cnum>2) && !strcmp(argument[2],"-")) {
        start=atoi(argument[1]);
        end=atoi(argument[3])+1;
        printf("\r\n %d에서 %d까지 삭제합니다.",start,end);
        for(i=start;i<end;i++) {
            sprintf(direc,"circle/%s/fboards",nmenu->ccode);
            strcpy(fhead,cnmenu->ccode);
            if(del_text(direc,fhead,i)) {
                printf("\r\n %d가 삭제되었습니다.",i);
            }
        }
    }           
    else {
        for (i=0;i<cnum;i++) {
            sprintf(direc,"circle/%s/fboards",nmenu->ccode);
            strcpy(fhead,cnmenu->ccode);
            num = atoi(argument[i]);
            if(del_text(direc,fhead,num)) {
                printf("\r\n %d가 삭제되었습니다.",num);
            }
        }
    }
}
	
cmakemenu()
{
    make_cmenu(0);
}
removeclogo()
{
    char buf[80],tmp[80];
    printf("\r\n 제거할 로고화일명은: ");keyinput(buf,20);
    sprintf(tmp,"mv circle/%s/%s circle/%s/%s.backup",nmenu->ccode,buf,\
      nmenu->ccode,buf);
    system(tmp);
}
rzclogo()
{
    char buf[80];
    sprintf(buf,"circle/%s",nmenu->ccode);
    chdir(buf);
    system("rz");
    chdir(getenv("HOME"));
}

cmoveboard(char *cmd)
{
    char buf[80],ccode[80],from[80],direc[80];
    FILE *fp1;
    int cnum,num,circle,howmany;
    struct dis_list boards;
    cnum = parser(cmd);
    sprintf(buf,"boards/%s",nmenu->ccode);
    fp1=fopen(buf,"r");
    fseek(fp1,0,2); howmany = ftell(fp1)/sizeof(struct dis_list);
    fclose(fp1);
    num = atoi(argument[1]);
    if((num >0) && (num <= howmany)) {
        sprintf(buf,"boards/%s",nmenu->ccode);
        fp1=fopen(buf,"r");
        fseek(fp1,(num-1)*sizeof(struct dis_list),0);
        fread((char*)&boards,sizeof(struct dis_list),1,fp1);
        fclose(fp1);

        printf("\r\n 목적지의 가기코드를 적어주세요: ");
        keyinput(ccode,10);
        if(ccode[0]=='\0') {
            printf("\r\n 취소하셨습니다.");
            return 0;
        }
        printf("\r\n 목적지가 동호회게시판인가요?(y/N) ");
        keyinput(buf,2);
        if(buf[0]=='y' || buf[0]=='Y') { 
            printf("\r\n 동아리 가기코드를 적어주세요: ");keyinput(buf,10);
	    sprintf(direc,"circle/%s/boards",buf);
	}
        else {
	    sprintf(direc,"boards");
	}
	sprintf(from,"circle/%s/boards",nmenu->ccode);

        if(move_board(from,boards,direc,ccode)) {
            printf("\r\n 자료를 옮겼습니다.");
        }
        else return 0;
        /* 게시물을 삭제한다 */
    }
    else {
        printf("\r\n 범위가 안 맞습니다.");
        return 0;
    }
    return 1;
}

