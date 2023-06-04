
#include "bbs.h"

/*  다음의 프로그램은 라인에디터입니다. */

int currentlinenum;  /* 라인넘버를 표시하기 위한 변수 */
struct line *t_start, *t_now, *t_end, *t_tmp;
struct dis_list edit_title;
int lineeditor(char *direc,char *fhead)
{
    FILE *fp1;
    char buf[80];
    struct dis_list edit_title;
    int i,key;
    time_t t;
    struct tm *tp,*localtime();

    for(i=0;i<3;i++) {
	printf("\r\n      +---------+---------+---------+-----");
	printf("\r\n 제목: ");
	keyinput(edit_title.title,35);
	if(edit_title.title[0]=='\0') {
	    printf("\r\n ** 제목을 집어넣으세요 **");
	}
	else break;
    }
    if(edit_title.title[0]=='\0') {
	printf("\r\n **취소하셨습니다**");pressreturn();
	reprint=YEA;
	return -1;
    }
    time(&t);
    sprintf(edit_title.bfilename,"%s%d.txt",fhead,t);
    if(writetextfile(direc,edit_title.bfilename)==1) {
	strcpy(edit_title.id,pf.id);
	strcpy(edit_title.name,pf.name);
	sprintf(buf,"%s/%s",direc,edit_title.bfilename);
	edit_title.line=seehowmanyline(buf);
	edit_title.look=0;
	edit_title.feel=0;
	time(&t); edit_title.date=t;
	sprintf(buf,"%s/%s",direc,fhead);
	if((fp1=fopen(buf,"r+"))==NULL) {
	    fp1=fopen(buf,"a+");
	}
	fseek(fp1,0,2);
	edit_title.num=ftell(fp1) / sizeof(struct dis_list)+1;
	top_num=edit_title.num;
	fwrite((char*)&edit_title,sizeof(struct dis_list),1,fp1);
	fclose(fp1);
	printf("\r\n 글을 게시하였습니다.");pressreturn();
	now_num=top_num;
	key=1;
    }
    if(key==1) return edit_title.num;
    else return -1;
}
    

    
/* 처음 게시물 작성할 때의 라인 에디터 */
int line_editor(directy)
char *directy;
{
    int key,i;
    char buf[80],buf1[30];
    FILE *fp1;
    FILE *fp2;
    struct dis_list edit_title;

    for (i=0;i<3;i++) {
    	printf("\r\n 제목 : ");
    	lineinput(edit_title.title,40,YEA);
    	if(edit_title.title[0]=='\0') {
		printf("\r\n ** 제목을 집어넣으세요. **");
    	}
	else break;
    }
    if(edit_title.title[0]=='\0') {
	printf("\r\n **취소하셨습니다.**"); pressreturn();
	reprint=YEA;
	return 1;
    }
    if(ma_write() == 1) {
	if(nmenu->form!='C'&& nmenu->form!='O') { /* 동호회가 아닌곳에서의 게시판 */
	    sprintf(edit_title.bfilename,\
	      "%s%d.txt",nmenu->ccode,time(&edit_title.date));
	}
	else { /* 동호회나 사무실에서의 게시판 */
	    sprintf(edit_title.bfilename,\
	      "%s%d.txt",cnmenu->ccode,time(&edit_title.date));
	}
	sprintf(buf,"%s/%s",directy,edit_title.bfilename);
        fp1 = fopen(buf,"a+"); /* 화일 포지션은 맨 끝 */
        edit_title.position = 0; /* 항상 처음을 가리킨다*/
        edit_title.line = 0;
	if(nmenu->form!='T') writetextheader(fp1);
        t_now = t_start;
        while(t_now->right != '\0') {
	    fputs(t_now->text,fp1);
            fputc('\r',fp1);
            fputc('\n',fp1);
            ++edit_title.line;
            t_now = t_now->right;
        }
        edit_title.size = ftell(fp1) ;    /* 내용 길이 */
        fclose(fp1);
        sprintf(edit_title.id,"%s",pf.id);
        sprintf(edit_title.name,"%s",pf.name);
        edit_title.look = 0;
	edit_title.feel = 0;
        if (nmenu->form!='C'&& nmenu->form!='O'){
	    sprintf(buf,"%s/%s",directy,nmenu->ccode);
	}
	else sprintf(buf,"%s/%s",directy,cnmenu->ccode);

        if((fp1=fopen(buf,"r+")) == NULL) {
            fp1=fopen(buf,"a+");
        }
        fseek(fp1,0,SEEK_END); /* 화일헤더 맨 끝으로 돌려야한다. */
        edit_title.num = ftell(fp1) / sizeof(struct dis_list) + 1;
        top_num = edit_title.num;
        fwrite((char*)&edit_title,sizeof(struct dis_list),1,fp1);
	fclose(fp1);

        now_num = top_num;
	key = 1;
    }
    unlink_text();
    if (key==1) return edit_title.num; /*게시물 번호를 넘긴다. */
    else return 0;

}

int visual_editor(direc)
char *direc;
{
    int key;
    char buf[30],buf1[30];
    FILE *fp1;
    FILE *fp2;
    struct dis_list edit_title;

    printf("\r\n 제목 : ");
    lineinput(edit_title.title,40,YEA);
    if(edit_title.title[0]=='\0') {
        printf("\r\n ** 취소하셨습니다. **"); pressreturn();
        reprint=YEA;
        return 1;
    }
    sprintf(edit_title.bfilename,"%s%d.txt",nmenu->ccode,time(&edit_title.date));
    ioctl(0,TCSETAF,&systerm);
    sprintf(buf,"vi %s/%s",direc,edit_title.bfilename);
    system(buf);
    ioctl(0,TCSETAF,&mbuf);

    sprintf(buf,"%s/%s",direc,edit_title.bfilename);
    fp1 = fopen(buf,"r"); /* 화일 포지션은 맨 끝 */
    edit_title.position = 0; /* 항상 처음을 가리킨다*/
    edit_title.line = 0;
    fseek(fp1,0,2);
    edit_title.size = ftell(fp1) ;    /* 내용 길이 */
    fclose(fp1);
    sprintf(edit_title.id,"%s",pf.id);
    sprintf(edit_title.name,"%s",pf.name);
    edit_title.look = 0;
    edit_title.feel = 0;
    sprintf(buf,"%s/%s",direc,nmenu->ccode);
    if((fp1=fopen(buf,"r+")) == NULL) {
        fp1=fopen(buf,"a+");
    }
    fseek(fp1,0,SEEK_END); /* 화일헤더 맨 끝으로 돌려야한다. */
    edit_title.num = ftell(fp1) / sizeof(struct dis_list) + 1;
    top_num = edit_title.num;
    fwrite((char*)&edit_title,sizeof(struct dis_list),1,fp1);
    fclose(fp1);

    return edit_title.num; /*게시물 번호를 넘긴다. */

}
uploadtext(direc)
char*direc;
{
    int key;
    char buf[30],buf1[30];
    FILE *fp1;
    FILE *fp2;
    struct dis_list edit_title;

    printf("\r\n 제목 : ");
    lineinput(edit_title.title,40,YEA);
    if(edit_title.title[0]=='\0') {
        printf("\r\n ** 취소하셨습니다. **"); pressreturn();
        reprint=YEA;
        return 1;
    }
    printf("\r\n 업로드할 파일명을 써주세요.");
    printf("\r\n 업로드화일명: "); keyinput(edit_title.bfilename,30);
    if(buf[0]=='\0') {
	printf("\r\n 취소되었습니다.");
	return;
    }
    ioctl(0,TCSETAF,&systerm);
    chdir(direc);
    sprintf(buf,"rz");
    system(buf);
    chdir(getenv("HOME"));
    ioctl(0,TCSETAF,&mbuf);

    sprintf(buf,"%s/%s",direc,edit_title.bfilename);
    fp1 = fopen(buf,"r"); /* 화일 포지션은 맨 끝 */
    edit_title.position = 0; /* 항상 처음을 가리킨다*/
    edit_title.line = 0;
    fseek(fp1,0,2);
    edit_title.size = ftell(fp1) ;    /* 내용 길이 */
    edit_title.line = ftell(fp1);
    fclose(fp1);
    sprintf(edit_title.id,"%s",pf.id);
    sprintf(edit_title.name,"%s",pf.name);
    edit_title.look = 0;
    edit_title.feel = 0;
    sprintf(buf,"%s/%s",direc,nmenu->ccode);
    if((fp1=fopen(buf,"r+")) == NULL) {
        fp1=fopen(buf,"a+");
    }
    fseek(fp1,0,SEEK_END); /* 화일헤더 맨 끝으로 돌려야한다. */
    edit_title.num = ftell(fp1) / sizeof(struct dis_list) + 1;
    top_num = edit_title.num;
    fwrite((char*)&edit_title,sizeof(struct dis_list),1,fp1);
    fclose(fp1);

    return edit_title.num; /*게시물 번호를 넘긴다. */

}

/* 게시물 수정할 때의 라인 에디터 */
reedit(l_num)
int l_num;
{
    char buf[80];
    int i;

    if(pf.level >= cfg.del || !strcmp(l_title.id,pf.id)) {
	printf("\r\n 무엇을 수정하시겠습니까?");
	printf("\r\n 1.본문(text) 2.제목(title) 3.thread(reserved)");
	lineinput(buf,5,YEA);
	if(buf[0] =='1') i = edit_text();
	else if(buf[0] =='2') /*edit_title()*/;
	else if(buf[0] =='3') /* edit_thread(); */ ;

    }
    unlink_text();
    return i; /* edit_text에서의 relay된 i */
}	

int edit_text()
{
    FILE *fp2;
    char buf[80];
    int i;


    sprintf(buf,"boards/%s",l_title.bfilename); 
    if ( (fp2 = fopen(buf,"a")) == NULL) {
	printf("\r\n 게시물이 저장된 화일이 없습니다.");
 	pressreturn();
	return -1;
    }
 
    read_text(fp2); /* 텍스트구조체로 읽어온다. */    
    while(1) {
        printf("\r\n\r등록[S] 취소[Q] 읽기[L] 수정[E] 추가[A] 삽입[I] 삭제[D] :"); 
        lineinput(buf,10,YEA);
        if(buf[0] == 's' || buf[0] == 'S') {  
	    text_write(fp2);
	    printf("\r\n수정되었습니다.");
	    i=1;
	    break; 
	}
        else if(buf[0] == 'l' || buf[0] == 'L') { text_list(buf); }
        else if(buf[0] == 'a' || buf[0] == 'A') { text_append(); }
        else if(buf[0] == 'e' || buf[0] == 'E') { text_edit(buf); }
        else if(buf[0] == 'd' || buf[0] == 'D') { text_delete(buf); }
        else if(buf[0] == 'i' || buf[0] == 'I') { text_insert(buf); }
        else if(buf[0] == 'q' || buf[0] == 'Q') {
            printf("\r\n확실합니까(y/N)? ");
            lineinput(buf,2,YEA);
            if(buf[0] == 'y' || buf[0] == 'Y') {
		printf("\r\n 수정취소입니다.");
		i=0;
                break;
            }
        }
    }
    fclose(fp2);
    return(i);  /* i=1이면 수정이 되었다는 소리임 */
}

int text_write(fp2)
FILE *fp2;
{
    l_title.line=0;
    t_now=t_start;
    while(t_now->right != '\0') {
	fputs(t_now->text,fp2);
	fputc('\r',fp2);
	fputc('\n',fp2);
	++l_title.line;
	t_now=t_now->right;
    }
    l_title.size=ftell(fp2);
}


int ma_write()  /* 쓰기 입력 루틴 */
{
    int i;
    char buf[80];
    FILE *fp1;
    t_start = (struct line *)malloc(sizeof(struct line));
    t_start->left = '\0';
    t_now = t_start;
    t_end = t_now;

    currentlinenum=1;
    text_append();
    while(1) {
        printf("\r\n\r등록[S] 취소[Q] 읽기[L] 수정[E] 추가[A] 삽입[I] 삭제[D] : ");
        lineinput(buf,10,YEA);
        if(buf[0] == 's' || buf[0] == 'S') { i = 1; break; }
        else if(buf[0] == 'l' || buf[0] == 'L') { text_list(buf); }
        else if(buf[0] == 'a' || buf[0] == 'A') { text_append(); }
        else if(buf[0] == 'e' || buf[0] == 'E') { text_edit(buf); }
        else if(buf[0] == 'd' || buf[0] == 'D') { text_delete(buf); }
        else if(buf[0] == 'i' || buf[0] == 'I') { text_insert(buf); }
        else if(buf[0] == 'q' || buf[0] == 'Q') {
            printf("\r\n확실합니까(y/N)? ");

            lineinput(buf,1,YEA);
            if(buf[0] == 'y' || buf[0] == 'Y') {
                i = 2;
                break;
            }
        }
    }
    return(i);
}

void text_append()
{
    printf("\r\n\r첫칸에 '.'을 찍으면 끝납니다.\r\n\n");
    printf("\r\n    +---------+---------+---------+---------+---------+---------+---------+");
    printf("\r\n");
    while(1) {
	printf("%3d:",currentlinenum);
        lineinput(t_now->text,79,YEA);
        if((t_now->text[0] == '.' && t_now->text[1] == '\0') || t_now->text[0] == 0x1a) {
            t_now->right = '\0';
            break;
        }
        printf("\r\n");
        t_end = (struct line *)malloc(sizeof(struct line));

        t_now->right = t_end;
        t_end->left = t_now;
        t_now = t_end;
	currentlinenum++;
    }
}

void text_list(buf)
char *buf;
{
    int i = 0, j, l = 1;
    buf[0] = ' ';
    i = atoi(buf);

    t_now = t_start;
    if(i > 0) {
        --i;
        for(j = 0; j < i; j++) {
            ++l;
            t_now = t_now->right;
            if(t_now->right == '\0') {
                t_now = t_now->left;
                break;
           }
        }
    }
    while(t_now->right != '\0') {
        if(i < CRT) {
            printf("\r\n%3d : %s",l++,t_now->text);

            t_now = t_now->right;
            ++i;
        }
        else {
	    currentlinenum=i;
            printf("\r\n계속[엔터] 중지[P] : ");
            lineinput(buf,1,YEA);
            if(buf[0] == 'p' || buf[0] == 'P') {
                break;
            }
            i = 0;
        }
    }
}
void text_edit(buf)
char *buf;
{
    int i, j;
    buf[0] = ' ';
    i= atoi(buf);
    if(i== 0) {
        printf("\r\n줄번호 : ");
        lineinput(buf,4,YEA);
        i= atoi(buf);
    }
    t_now = t_start;
    if(i> 0) {
        --i;
        for(j = 0; j < i; j++) {
            t_now = t_now->right;
            if(t_now->right == '\0') {
                t_now = t_now->left;
                break;
            }
        }
        printf("\r\n%3d:%s",i+1,t_now->text);
        printf("\r\n%3d:");        lineinput(t_now->text,79,YEA);
    }
}

void text_delete(buf)
char *buf;
{
    int i, j, chk = 1;
    buf[0] = ' ';
    i = atoi(buf);
    if(i == 0) {
        printf("\r\n줄번호 : ");
        lineinput(buf,4,YEA);
        i = atoi(buf);
    }
    t_now = t_start;

    if(i > 0) {
        for(j = 1; j < i; j++) {
            t_now = t_now->right;
            if(t_now->right == '\0') {
                chk = 0;
                printf("\r\n해당 번호 없음\n\r");
                break;
            }
        }
        if(chk) {
            if(t_now->left == '\0') {
                t_start = t_start->right;
                t_start->left = '\0';
            }
            else {
                t_tmp = t_now->left;
                t_tmp->right = t_now->right;
                t_tmp = t_now->right;
                t_tmp->left = t_now->left;
            }
            free(t_now);
        }

    }
    currentlinenum--;
}
void text_insert(buf)
char *buf;
{
    int i, j, chk = 1;
    buf[0] = ' ';
    i = atoi(buf);
    if(i == 0) {
        printf("\r\n줄번호 : ");
        lineinput(buf,4,YEA);
        i = atoi(buf);
    }
    t_now = t_start;
    if(i > 0) {
        for(j = 1; j < i; j++) {
            t_now = t_now->right;
            if(t_now->right == '\0') {
                chk = 0;
                printf("\r\n해당 번호 없음\r\n");
                break;
            }
        }
        if(chk) {

            printf("\r\n"); printf("%3d:",i);
            t_tmp = (struct line *)malloc(sizeof(struct line));
            lineinput(t_tmp->text,79,YEA);
            if(t_now->left == '\0') {
                t_now->left = t_tmp;
                t_tmp->left = '\0';
            }
            else {
                t_tmp->right = t_now;
                t_now = t_now->left;
                t_tmp->left = t_now;
                t_now->right = t_tmp;
                t_now = t_tmp->right;
                t_now->left = t_tmp;
            }
        }
    }
    currentlinenum++;
}
void unlink_text()
{
    t_now = t_start;
    while(t_now->right != '\0') {
        t_now = t_now->right;
        free(t_start);
        t_start = t_now;
    }
    free(t_now);
}

read_text(fp2)
FILE *fp2;
{
    char c;
    int i;
    currentlinenum=1;

    t_start = (struct line *) malloc(sizeof(struct line));
    t_start->left = '\0';
    t_now = t_start;
    t_end = t_now;
   
    while((c = fgetc(fp2)) != EOF) {
	i = 0;
	if ( c == '\r' ) t_now=t_now->right;
        if ( c =='\n') ;
        if (c!='\n' && c!='\r') {
	    t_now->text[i]=c;
	    i++;
 	    currentlinenum++;	
	}
    }
    t_now->right= '\0';
}	 
writetextheader(FILE *file)
{
    char buf[80];
    sprintf(buf,"\r\n작성자 : %s(%s) ",pf.name,pf.id);
    fputs(buf,file);
fputs("\r\n-------------------------------------------------------------------------------\r\n",file);
}

/* 라인에디터 끝 */
