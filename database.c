/* �� ���α׷��� �����ڳ��� ����Ÿ���̽��� ����ϰ� ������ִ� �Լ����Դϴ� */
#include "bbs.h"

int LEN=15; /* �ѹ��� ���̴� Ÿ��Ʋ �� */

munhun(char *direc,char *fhead)
{
    FILE *fp1;
    char buf[255],c;
    struct Munhun munhun,viewmunhun;
    int back,i,num,howmany,cnum,key;
    int result=1;
    char bid;    
    CRT = 15;
    sprintf(buf,"%s/%s",direc,fhead);
    if((fp1=fopen(buf,"r+"))==NULL) {
        fp1=fopen(buf,"a+");
    }
    if(!fseek(fp1,0,2)) {
        howmany=ftell(fp1) /sizeof(struct Munhun);
    }
    else {
        printf("\r\n ȭ���� ������ �ʽ��ϴ�.");
    }
    fclose(fp1);
    back = 0;
    reprint=YEA;
    esc_flag = 0;
    while(1) {
        if(key) {
            cls();
            printf("\r\n ���հ˻� ");
	    printf("\r\n      �˻� �з�                         ��");
	    printf("\r\n 1) ����(�ѱ�10��, ����20���̳�)... ��ȭ�����׽�?");
	    printf("\r\n 2) ���ڸ�......................... ȫ�浿");
	    printf("\r\n -----------------------------------------");
            printf("\r\n ����(H) �����޴�(M) ���ôܾ��(E �˻���) �˻��׸񿭶�(DI) �˻������(EX)");
	    printf("\r\n ����==> ");
            key=0;
        }
        else {
            for(i=0;i<back;i++) {
                printf("\b \b");
            }
        }
        keyinput(buf,20);
	back=strlen(buf);
  	cnum=parser(buf);
	if(buf[0]=='1') {
	    printf("\r\n ������ �����������.");
	    printf("\r\n ���� : "); keyinput(buf,40);

	    if(strlen(buf)>2) {
	        search_title(direc,fhead,buf);
	    }
	    key=1;
	}
	else if(buf[0]=='2') {
	    printf("\r\n ���ڸ� �����������.");
	    printf("\r\n ���� : ");keyinput(buf,10);
	    if(strlen(buf)>2) {
	        search_writer(direc,fhead,buf);
	    }
	    key=1;
	}
        else {
	    if(!strcmp(buf,"p")) {
		esc_flag=1;
	    }
	    if(!strcmp(buf,"add")) {
		add_munhun(direc,fhead);
		key=1;
	    }

	    if(!strcmp(buf,"del")) {
		del_munhun(direc,fhead);
		key=1;
	    }
        }
        if(esc_flag) break;
    }
}
add_munhun(char *direc,char *fhead)
{
    struct Munhun munhun;
    char buf[80];
    FILE *fp1;
    sprintf(buf,"%s/%s",direc,fhead);
    if((fp1=fopen(buf,"r+"))==NULL) {
	fp1=fopen(buf,"a+");
    }
    printf("\r\n �߰��Ͻ� ��������: ");keyinput(munhun.title,40);
    printf("\r\n          ���ڸ���: ");keyinput(munhun.writer,10);
    printf("\r\n          ���ڸ���: ");keyinput(munhun.translator,10);
    fseek(fp1,0,2);
    fwrite((char*)&munhun,sizeof(struct Munhun),1,fp1);
    fclose(fp1);    
    return 1;
}
del_munhun(char *direc, char *fhead)
{
    printf("\r\n ���� �������� �ʽ��ϴ�.");
}
      
search_title(char *direc, char*fhead,char*content)
{
    FILE *fp1;
    char buf[80];
    struct Munhun munhun;
    int start=1,end=0; /* ��ŸƮ�� list�� �� �ʿ��ϰ� end�� ����Ʈ�� ���� �˱�
			���� �ʿ� */
    int key,esc_search,howmany=0,i,len,back;
    int esc_munhun=0;

    char *tmp;
    /* ���ڿ� Ž���ϴ� �ڵ带 ����ִ´�  */
    /* howmany���ٰ� �˻��� �� �׸���� ����ְ� */
    /* tmp���丮���ٰ� �ӽ� report header file�� �ۼ��Ѵ� */
    
    /* �ٷ� ����Ѵ� */
    while(1) {
	if(key) {
	    cls();
	    list_munhunbytitle(direc,fhead,content,start,start+LEN);
	    printf("\r\n ��ȣ/���(H,P,F,B)");printf("\r\n >> ");
	}
	else {
	    for(i=0;i<back;i++) {
		printf("\b \b");
	    }
	}
	keyinput(buf,10);
	if(buf[0]=='f'||buf[0]=='F') {
	    if(end >(start+LEN)) {
	        start=start+LEN;
	        key=1;
	    }
	}
	else if(buf[0]=='b' || buf[0]=='B') {
	    if((start-LEN) >0) {
	        start=start-LEN;
	        key=1;
	    }
	}
	else if(buf[0]=='p' || buf[0]=='P') {
	    esc_munhun=1;
	}
	
	if(esc_munhun) {
	    break;
	}
    }	 
}
search_writer(char *direc, char*fhead,char*content)
{
    FILE *fp1;
    char buf[80];
    struct Munhun munhun;
    int start=1,end=0; /* ��ŸƮ�� list�� �� �ʿ��ϰ� end�� ����Ʈ�� ���� �˱�
                        ���� �ʿ� */
    int key,esc_search,howmany=0,i,len,back;

    int esc_munhun=0;

    char *tmp;
    /* ���ڿ� Ž���ϴ� �ڵ带 ����ִ´�  */
    /* howmany���ٰ� �˻��� �� �׸���� ����ְ� */
    /* tmp���丮���ٰ� �ӽ� report header file�� �ۼ��Ѵ� */
    
    /* �ٷ� ����Ѵ� */
    while(1) {
        if(key) {
	    cls();
            list_munhunbywriter(direc,fhead,start,start+LEN);
            printf("\r\n ��ȣ/���(H,P,F,B)");printf("\r\n >> ");
        }
        else {
            for(i=0;i<back;i++) {
                printf("\b \b");
            }
        }
        keyinput(buf,10);
        if(buf[0]=='f'||buf[0]=='F') {
            if(end >(start+LEN)) {
                start=start+LEN;
                key=1;
            }
        }
        else if(buf[0]=='b' || buf[0]=='B') {
            if((start-LEN) >0) {
                start=start-LEN;
                key=1;
	    }
	}
	else if(buf[0]=='p' || buf[0]=='P') {
	    esc_munhun=1;
	}
	
	if(esc_munhun) {
	    break;
	}	
    }
}
     
list_munhunbytitle(char *direc,char *fhead,char *title,int start,int end)
{
    FILE *fp1;
    char buf[80];
    int i,j,k,l;
    struct Munhun munhun;
    
    sprintf(buf,"%s/%s",direc,fhead);
    fp1=fopen(buf,"r");
    printf("\r\n ��ȣ           ��      ��                       ��  ��");
    printf("\r\n");for(i=0;i<79;i++) printf("-");
    for(i=start ;i<(end+1) ; i++) {
	if(fseek(fp1,(i-1)*sizeof(struct Munhun),0)) break;
	fread((char*)&munhun,sizeof(struct Munhun),1,fp1);
	j=strlen(title);
	k=strlen(munhun.title)-j+1;
	for(l=0;l<k;l++) {
	    if(!strncmp(title,&munhun.title[l],j)) {
	        printf("\r\n %4d   %-40s %-10s ",i,munhun.title,munhun.writer);
	    }
	}
    }
}
list_munhunbywriter(char *direc,char *fhead,char *writer,int start,int end)
{
    FILE *fp1;
    char buf[80];
    int i,j,k,l;
    struct Munhun munhun;

    sprintf(buf,"%s/%s",direc,fhead);
    fp1=fopen(buf,"r");
    printf("\r\n ��ȣ           ��      ��                       ��  ��");
    printf("\r\n");for(i=0;i<79;i++) printf("-");
    for(i=start ;i<(end+1) ; i++) {
        if(fseek(fp1,(i-1)*sizeof(struct Munhun),0)) break;
        fread((char*)&munhun,sizeof(struct Munhun),1,fp1);
        j=strlen(writer);
        k=strlen(munhun.writer)-j+1;
        for(l=0;l<k;l++) {
            if(!strncmp(writer,&munhun.writer[l],j)) {
                printf("\r\n %4d   %-40s %-10s ",i,munhun.title,munhun.writer);
            }
        }
    }
}
 
