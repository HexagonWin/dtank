#include "bbs.h"
show_kongzi(char *direc,char *fhead,struct commands cmdtable[])
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
            printf("\r\n");
            titlelist(direc,fhead);
	    printf("\r\n");for(i=0;i<79;i++) printf("-");
            printf("\r\n 그만보기[리턴] ##번호:보기");
            printf("\r\n >> ");
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
                if (nmenu->form=='f' || nmenu->form=='F')  {
                    result=displaycontent(num,0,direc,fhead,bid);
                }
                else if (nmenu->form=='l' || nmenu->form=='L') {
                    result=displaycontent(num,0,direc,fhead,bid);
                }
                else if (!strcmp(nmenu->ccode,"TRASH")) {
                    result=display_tcontent(num,0,"TRASH");
                }
                else {
                    result=displaycontent(num,0,direc,fhead,'l');
                }
                num=result;
            }
        } 
        else {
            docommand(buf,cmdtable);
        }
        if(buf[0]=='\0') {
            esc_flag=1;
        }
        if(esc_flag) break;
    }
}

