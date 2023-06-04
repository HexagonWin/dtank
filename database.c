/* 이 프로그램은 하이텔내의 데이타베이스와 비슷하게 만들어주는 함수들입니다 */
#include "bbs.h"

int LEN=15; /* 한번에 보이는 타이틀 수 */

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
        printf("\r\n 화일이 열리지 않습니다.");
    }
    fclose(fp1);
    back = 0;
    reprint=YEA;
    esc_flag = 0;
    while(1) {
        if(key) {
            cls();
            printf("\r\n 종합검색 ");
	    printf("\r\n      검색 분류                         예");
	    printf("\r\n 1) 서명(한글10자, 영문20자이내)... 개화기저항시?");
	    printf("\r\n 2) 저자명......................... 홍길동");
	    printf("\r\n -----------------------------------------");
            printf("\r\n 도움말(H) 상위메뉴(M) 관련단어보기(E 검색어) 검색항목열람(DI) 검색식출련(EX)");
	    printf("\r\n 선택==> ");
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
	    printf("\r\n 서명을 집어넣으세요.");
	    printf("\r\n 서명 : "); keyinput(buf,40);

	    if(strlen(buf)>2) {
	        search_title(direc,fhead,buf);
	    }
	    key=1;
	}
	else if(buf[0]=='2') {
	    printf("\r\n 저자를 집어넣으세요.");
	    printf("\r\n 저자 : ");keyinput(buf,10);
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
    printf("\r\n 추가하실 도서명은: ");keyinput(munhun.title,40);
    printf("\r\n          저자명은: ");keyinput(munhun.writer,10);
    printf("\r\n          역자명은: ");keyinput(munhun.translator,10);
    fseek(fp1,0,2);
    fwrite((char*)&munhun,sizeof(struct Munhun),1,fp1);
    fclose(fp1);    
    return 1;
}
del_munhun(char *direc, char *fhead)
{
    printf("\r\n 아직 지원되지 않습니다.");
}
      
search_title(char *direc, char*fhead,char*content)
{
    FILE *fp1;
    char buf[80];
    struct Munhun munhun;
    int start=1,end=0; /* 스타트는 list할 때 필요하고 end는 리스트의 끝을 알기
			위해 필요 */
    int key,esc_search,howmany=0,i,len,back;
    int esc_munhun=0;

    char *tmp;
    /* 문자열 탐색하는 코드를 집어넣는다  */
    /* howmany에다가 검색된 총 항목수를 집어넣고 */
    /* tmp디렉토리에다가 임시 report header file을 작성한다 */
    
    /* 바로 출력한다 */
    while(1) {
	if(key) {
	    cls();
	    list_munhunbytitle(direc,fhead,content,start,start+LEN);
	    printf("\r\n 번호/명령(H,P,F,B)");printf("\r\n >> ");
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
    int start=1,end=0; /* 스타트는 list할 때 필요하고 end는 리스트의 끝을 알기
                        위해 필요 */
    int key,esc_search,howmany=0,i,len,back;

    int esc_munhun=0;

    char *tmp;
    /* 문자열 탐색하는 코드를 집어넣는다  */
    /* howmany에다가 검색된 총 항목수를 집어넣고 */
    /* tmp디렉토리에다가 임시 report header file을 작성한다 */
    
    /* 바로 출력한다 */
    while(1) {
        if(key) {
	    cls();
            list_munhunbywriter(direc,fhead,start,start+LEN);
            printf("\r\n 번호/명령(H,P,F,B)");printf("\r\n >> ");
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
    printf("\r\n 번호           서      명                       저  자");
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
    printf("\r\n 번호           서      명                       저  자");
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
 
