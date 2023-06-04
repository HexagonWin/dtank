/* 새로운 게시판 보기이다. */
#include "bbs.h"

int displaymailcontent(int,int,char*,char*,char);

/* 이것이 기본 모듈이다. */
show_mail(char *direc,char *fhead,struct commands cmdtable[])
{
    FILE *fp1;
    char buf[255];
    int back,i,num;
    int result=1;
    char bid;    
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
  	    mailtitlelist(direc,fhead);
	    printf("\r\n 번호/명령(H,F,B,P,T,W,DN,PR,DD,RE,FR,GO,HI,Z,X)");
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
        num = atoi(buf);
	bid=nmenu->form;

    	if(num) {
	    result=1;
	    while(result!=0) {
                result==0;
        	result=displaymailcontent(num,0,direc,fhead,'l');
                num=result;
            }
	} 
	else if(!strncmp(buf,"pr",2)) {
	    parser(buf);num=atoi(argument[1]);
 	    displaycontent(num,1,direc,fhead,'l');
	    reprint=YEA;
	}
	else {
	    docommand(buf,cmdtable);
	}
 	if(buf[0]=='\0') {
	    if(now_num>0) { now_num=now_num-CRT;}
	    if(now_num<0) now_num=1;
	    reprint=YEA;
	}
	if(esc_flag) break;
    }
}

mailtitlelist(char*direc, char*fhead)
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
    printf("\r\n 편지 받기   (RMAIL) ");
    printf(\
"\r\n  No. 발신자ID 이  름   발신일       라인  제    목");
    checkbufferint=1;
    for(i=0;i<CRT;i++) {
	if(fseek(fp1,(linenum-1)*sizeof(struct dis_list),0)) {
	    break;
	}
	else {
	    fread((char*)&l_title,sizeof(struct dis_list),1,fp1);
	    checkbufferint=showmailtitle(l_title);
	    if(!checkbufferint) i--;
	}
	linenum--;
    }
    fclose(fp1);
}

showmailtitle(struct dis_list title)
{
    struct tm *tp;
    int check=0;char read;
    if(!check) check=compare_li(title,li);
    if(!check) check=compare_lt(title,lt);
    if(!check) check=compare_ld(title,ld);
    if(li[0]=='\0' && lt[0]=='\0' && ld[0]=='\0') check=1;

    if(1) {
	if((title.look>=0)&&check) {
	    tp = localtime(&title.date);
	    if(title.look>0) read='R';
	    else read='*';
            printf("\r\n%c%3d  %-8s %-8s %02d/%02d/%02d %3s %3d. %-38s", 
              read,title.num,title.id,title.name,tp->tm_year,tp->tm_mon+1,tp->tm_mday,
              " ",title.line,title.title);
	    return 1;
	}
	else if(title.look <=0) {
	    if(pf.level >=cfg.del) {
		printf("\r\n 삭제되었음.");
	    }
	    return 1;
	}
    }
    return 0;
}
int displaymailcontent(int num,int fl, char *direc,char *fhead,char bid)
{
    FILE *fp1;
    char buf[80];
    int i;
    int cmdlen; /* 입력만 명령을 지울 때 사용하는 명령길이 */
    int startline=0; /* 내용 볼 때의 라인 */
    int nextcontent=0; /* 이 버퍼가 0이 아니면 다음 글을 또 읽는다. */
    int howmany,endline;
    int LINELEN=13; /* 13줄씩 표시된다 */
    struct dis_list r_title;
    sprintf(buf,"%s/%s",direc, fhead);
    fp1 = fopen(buf,"r");
    fseek(fp1,0,2);howmany=ftell(fp1)/sizeof(struct dis_list);
    fclose(fp1);
    if(num<1 || num>howmany) {
        mess_dis(2);
        reprint=NA;
    }
    else {
        reprint=YEA;
        sprintf(buf,"%s/%s",direc,fhead);
        fp1 = fopen(buf,"r");
        fseek(fp1,(num-1)*sizeof(struct dis_list),0);
        fread((char*)&r_title,sizeof(struct dis_list),1,fp1);
        fclose(fp1);
        sprintf(buf,"%s/%s",direc,r_title.bfilename);
        endline=seehowmanyline(buf); /* 게시물의 총 라인 수*/

        if((r_title.look>=0) || (pf.level > cfg.del)) {
            /* 읽기에 들어가는 것이다. */
            esc_sub=0;
            while(1) {
                if(fl) {
                    printf("\r\n [캡쳐를 준비하고 리턴을 누르세요]");
                    pressreturn();
                    show_file(buf,0,endline);
                    printf("\r\n [캡쳐를 마치시고 리턴을 누르세요]");
                    pressreturn();
                    reprint=YEA;
                }
                if (reprint) {
                    cls();
                    sprintf(buf,"%s/%s",direc,r_title.bfilename);
                    dismailheader(startline,r_title);
		    printf("\r\n");
                    show_file(buf,startline,startline+LINELEN);
                    printf("\r\n 번호/명령(H,F,B,P,T,W,DN,PR,GO,HI,Z,X)");
                    printf("\r\n %d>> ",endline);
                    reprint=NA;
                }
                else {
                    for(i=0;i< cmdlen;i++) {
                        printf("\b \b");
                    }
                }
                keyinput(buf,80); cmdlen=strlen(buf);
                if(buf[0]=='p' || buf[0]=='P') {
                    nextcontent=0;
                    esc_sub=1;
                }
                else if (buf[0]=='f' || buf[0]=='F'||buf[0]=='\0') {
                    if((startline+LINELEN) >endline) {
                        reprint=YEA;
                    }
                    else {
                        startline=startline+LINELEN;
                        reprint=YEA;
                    }
                }
                else if(buf[0]=='b' || buf[0]=='B') {
                    if(startline<LINELEN) {
                        startline=0;
                        reprint=YEA;
                    }
                    else {
                        startline=startline-LINELEN;
                        reprint=YEA;
                    }
                }
                else if(buf[0]=='a' || buf[0]=='A') {
                    nextcontent=num+1;
                    esc_sub=1;
                }
                else if(buf[0]=='n' || buf[0]=='N') {
                    nextcontent=num-1;
                    esc_sub=1;
                }
                else {
                    if(bid=='l' || bid=='L') {
                        docommand(buf,contentcmd);
                    }
                    if(bid=='f' || bid=='F') {
                        docommand(buf,fcontentcmd);
                    }
                }
                if(esc_sub) break;
            }
            if( (pf.level > cfg.del) && (r_title.look < 0)) {
                printf("\r\n 복구하시겠어요?(y/N)? ");
                keyinput(buf,1);
                if(buf[0]=='y' || buf[0]=='Y') {
                    r_title.look=1;
                    printf("\r\n 복구되었어요.");
                }
            }
            /* 조회수 증가 */
            r_title.look++;
            sprintf(buf,"%s/%s",direc,fhead);
            fp1=fopen(buf,"r+");
            fseek(fp1,(num-1)*sizeof(struct dis_list),0);
            fwrite((char*)&r_title,sizeof(struct dis_list),1,fp1);
            fclose(fp1);        

            reprint=YEA;
        }
        else{
            mess_dis(5);
            reprint=NA;
        }
    }
    return nextcontent; /* 0을 보내면 내용보기에서 나간다. */
}
    

dismailheader(int start, struct dis_list title)
{
    struct tm *tp,*localtime();
    int i;
    tp = localtime(&title.date);
	printf("\r\n 번호: %2d   라인: %3d",title.num,title.line);
	printf("\r\n 발신일시 : %2d/%2d/%2d %2d:%2d",\
	  tp->tm_year,tp->tm_mon+1,tp->tm_mday,tp->tm_hour,tp->tm_min);
	printf("\r\n 발 신 인 : %10s (%10s)",title.name,title.id);
	printf("\r\n 제    목 : %-38s",title.title);

}
	    
		    
