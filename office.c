/* �̰��� CSN������ ������ ���� �Լ����̴�. */
#include "bbs.h"

bisurlettercheck()
{
    int i,a,b,c,d,e,f;
    char buf[80];
    a=b=c=d=e=f=i=0;
    a=check_letter("oletter");
    b=check_letter("qletter");
    c=check_letter("eletter");
    i=a+b+c+d+e+f;
    printf("\r\n o=%d q=%d ot=%d mmo=%d h-%d s-%d ��=%d���� �ֽ��ϴ�.",\
      a,b,c,d,e,f,i);
    return i;
}
bisurschedule() 
{
    printf("\r\n ���� �����ȵ� ");
}
bisurnews()
{
    printf("\r\n ���� �����ȵ� ");
}
/* �񼭿� �޽��� */
bisurmessage(char *chonjang)
{
    FILE *fp1;
    char ch;
    char buf[80];
    sprintf(buf,"circle/%s/message",nmenu->ccode);
    if((fp1=fopen(buf,"r"))!=NULL) {
        cls(); printf("\r\n %s���� ���������޽����Դϴ�.",chonjang);
	printf("\r\n ---> ");
        while((ch=fgetc(fp1))!=EOF) {
	    printf("%c",ch);
        }
    }
    fclose(fp1);
}
bisurwritemessage(char *chonjang)
{
    char buf[80],msg[80];
    FILE *fp1;
    printf("\r\n ����� �޸� �����ּ���.");
    printf("\r\n (80�ڳ���)>> ");keyinput(msg,79);
    sprintf(buf,"circle/%s/message",nmenu->ccode);
    if((fp1=fopen(buf,"w+"))!=NULL) {
        fseek(fp1,0,0);
    	fputs(msg,fp1);
    	fclose(fp1);
	printf("\r\n �޽����� ������ϴ�.");
    }
}
bisurficap(char *chonjang)
{
    int check=0;
    check=fi_man(chonjang);
    return check;
}
writeorderform(char *direc,char *filename)
{
    FILE *fp1;
    char buf[80];
    char content[10][40]; /* ���� ���� */    
    char orderform[10][40];
    int i,howmany,num;

    cls();
    sprintf(buf,"user/orderform");
    printf("\033[0;0H");printf("\r\n �ֹ� ���� �ۼ� ");
    if((fp1=fopen(buf,"r"))!=NULL) {
        fseek(fp1,0,0);
        fscanf(fp1,"%d",&howmany);
        for(i=0;i<howmany;i++) {
            fscanf(fp1,"%s",orderform[i]);
            printf("\033[%d;3H",i+3);printf("%d. %s:",i+1,orderform[i]);
        }
    }
    fclose(fp1);
    while(1) {
	printf("\033[20;2H");
        printf("�����ϰ� ���� ��ȣ�� �����ּ���(��:99)(����Է�:a)");
	printf("\033[21;2H");printf(">>               ");
	printf("\033[21;5H");keyinput(buf,5);
	num=atoi(buf);
	if(num==99) {break;}
	else if( buf[0]=='a' || buf[0]=='A') {
	    for(i=0;i<howmany;i++) {
		printf("\033[%d;%dH",i+3,strlen(orderform[i])+8);
		keyinput(buf,40);
		if(buf[0]!='\0') {
		    sprintf(content[i],buf);
		    printf("\033[%d;3H",i+3);printf("%d. %s:",i+1,orderform[i]);
		    printf("\033[%d;%dH",i+3,strlen(orderform[i])+8);
		    printf("%s",content[i]);
		}
	    }
	}
	else if( (num>0) && (num <= howmany)) {
	    printf("\033[%d;%dH",num+2,strlen(orderform[num-1])+8);
	    keyinput(buf,40);
	    if(buf[0]!='\0') {
		sprintf(content[num-1],buf);
	   	printf("\033[%d;3H",num+2);printf("%d. %s:",num,orderform[num-1]);
		printf("\033[%d;%dH",num+2,strlen(orderform[num-1])+8);
		printf("%s",content[num-1]);
	    }
	}
    }
    sprintf(buf,"%s/%s",direc,filename);
    fp1=fopen(buf,"a+");fseek(fp1,0,0);
    for(i=0;i<howmany;i++) {
	sprintf(buf,"\r\n %s : %s",orderform[i],content[i]);
   	fputs(buf,fp1);
    }
    fclose(fp1);
    printf("\r\n �ۼ��� �������ϴ�.");
    return 1;
}
display_orderform(char *direc, char *fhead)
{
    FILE *fp1;
    char buf[80];char content[10][40];
    int i,howmany;
    sprintf(buf,"%s/%s",direc,fhead);
    if((fp1=fopen(buf,"r"))!=NULL) {
	fseek(fp1,0,0);
	fscanf(fp1,"%d",&howmany);
   	for(i=0;i<howmany;i++) {
	    fscanf(fp1,"%s",content[i]);
	    printf("\033[%d;3H",i+3);printf("%s",content[i]);
	}
    }
    fclose(fp1);
}
 
