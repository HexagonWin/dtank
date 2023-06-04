/* 이 화일은 새로운 프로화일을 읽는 것입니다.
*/

#include "bbs.h"
/* who는 아이디이다. profile디렉토리에서 who의 정보를 읽어서
linenum번째 행을 출력하는 함수이다.*/
showline(char *who, int linenum)
{
    FILE *fp1;
    char ch,buf[80];
    int i,num,end; 
    sprintf(buf,"profile/%s",who);
    fp1 = fopen(buf,"r");
    if(fp1==NULL) {
	printf("\r\n 찾고자 하는 분의 소개서가 없군요.");
    }
    fseek(fp1,0,2); end = ftell(fp1);
    num = 0;
    fseek(fp1,0,0);
    for(i=0;i<end;i++) {
	ch = fgetc(fp1);
	if(num==linenum) {
	    printf("%c",ch);
	}
	if(ch == '\r'|| ch=='\n') {
	    num++;
	}
    }
}

showprofile(char*who)
{
    FILE *fp1;
    char ch,buf[80];
    int i,linenum,end;
    sprintf(buf,"profile/%s",who);
    fp1 = fopen(buf,"r");
    linenum = 0; fseek(fp1,0,2); end = ftell(fp1);
    fseek(fp1,0,0);
    for(i=0;i< end;i++) {
	if((ch=fgetc(fp1))=='\r') {
	    linenum++;
	}
    }
    fclose(fp1);
    for(i=0;i<linenum;i++) {
	showline(who,i);
    }
    
    pressreturn();
}
/* 프로화일을 다시 만들때 완전히 새로 만드는 것이다.*/
makeprofile(char *who )
{
    FILE *fp1;
    char buf[200],buf2[200];
    sprintf(buf,"profile/%s",who);
    unlink(buf);
    if((fp1 = fopen(buf,"r+"))==NULL) {
	fp1 = fopen(buf,"a+");
    }
    fseek(fp1,0,0);
    printf("\r\n 성명은?>> "); keyinput(buf,10);
    sprintf(buf2,"성명: %s\r", buf);
    fputs(buf2,fp1);
    printf("\r\n 다루는 분야>> "); keyinput(buf,10);
    sprintf(buf2,"분야: %s\r", buf);
    fputs(buf2,fp1);
    fclose(fp1);
}
newprofile()
{

    char buf[200];
    while(1) {
	printf("\r\n 어떤 일을 하겠습니까?");
	printf("\r\n (1) 프로화일만들기 (2) 프로화일보기 (0) 나가기");
	printf("\r\n 하실일 >> "); keyinput(buf,10);
        if(buf[0]=='1') {
	    makeprofile(pf.id);
  	}
	if(buf[0]=='2') {
	    showprofile(pf.id);
	}
	if(buf[0] =='0') {
	    break;
	}
    }
    printf("\r\n 나갑니다.");
}
              	    	 
