/* �� ȭ���� ���ο� ����ȭ���� �д� ���Դϴ�.
*/

#include "bbs.h"
/* who�� ���̵��̴�. profile���丮���� who�� ������ �о
linenum��° ���� ����ϴ� �Լ��̴�.*/
showline(char *who, int linenum)
{
    FILE *fp1;
    char ch,buf[80];
    int i,num,end; 
    sprintf(buf,"profile/%s",who);
    fp1 = fopen(buf,"r");
    if(fp1==NULL) {
	printf("\r\n ã���� �ϴ� ���� �Ұ����� ������.");
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
/* ����ȭ���� �ٽ� ���鶧 ������ ���� ����� ���̴�.*/
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
    printf("\r\n ������?>> "); keyinput(buf,10);
    sprintf(buf2,"����: %s\r", buf);
    fputs(buf2,fp1);
    printf("\r\n �ٷ�� �о�>> "); keyinput(buf,10);
    sprintf(buf2,"�о�: %s\r", buf);
    fputs(buf2,fp1);
    fclose(fp1);
}
newprofile()
{

    char buf[200];
    while(1) {
	printf("\r\n � ���� �ϰڽ��ϱ�?");
	printf("\r\n (1) ����ȭ�ϸ���� (2) ����ȭ�Ϻ��� (0) ������");
	printf("\r\n �Ͻ��� >> "); keyinput(buf,10);
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
    printf("\r\n �����ϴ�.");
}
              	    	 
