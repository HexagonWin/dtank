#include "bbs.h"
int display_onecontent(int fl, char *direc,char *fname)
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
    if(1) {
        reprint=YEA;
        sprintf(buf,"%s/%s",direc,fname);
        endline=seehowmanyline(buf); /* 게시물의 총 라인 수*/

        if((r_title.look>=0) || (pf.level > cfg.del)) {
            /* 읽기에 들어가는 것이다. */
            esc_sub=0;
            while(1) {
                if(fl) {
                    printf("\r\n[캡쳐를 준비하고 리턴을 누르세요]");
                    pressreturn();printf("\r\n");
                    sprintf(buf,"%s/%s",direc,fname);
                    show_file(buf,0,endline);
                    printf("\r\n[캡쳐를 마치시고 리턴을 누르세요]");
                    pressreturn();
                    reprint=YEA;
                    fl=0;
                }
                if (reprint) {
                    cls();
                    sprintf(buf,"%s/%s",direc,fname);
		    printf("\r\n%s                    %d/%d",nmenu->list,startline,endline);
		    printf("\r\n");
                    show_file(buf,startline,startline+LINELEN);
                    printf("\r\n 도움말:(f,b,p)");
                    printf("\r\n %d>> ",endline);
                    reprint=NA;
                }
                else {
                    for(i=0;i< cmdlen;i++) {
                        printf("\b \b");
                    }
                }
                keyinput(buf,80); cmdlen=strlen(buf);
                if(!strcmp(buf,"pr")) { fl=1;}

                else if(buf[0]=='p' || buf[0]=='P') {
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
		else if(!strcmp(buf,"edit")) {
		    writetextfile("page",nmenu->ccode);
	   	}
                else {
                        docommand(buf,contentcmd);
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
            reprint=YEA;
        }
        else{
            mess_dis(5);
            reprint=NA;
        }
    }
    return nextcontent; /* 0을 보내면 내용보기에서 나간다. */
}

