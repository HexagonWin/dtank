
#include "bbs.h"
#include "commands.h"
/*  M,m 서브메뉴위치와 chatting위치에서의 명령어 받는 함수 */
domenucommand(cmd,cmdtable)
char *cmd;
struct commands cmdtable[];
{
    int i;
    int len;
        for(i=0;i<100;i++) {   
            if(cmdtable[i].cmdname==NULL) break;
            len=strlen((char*)cmdtable[i].cmdname);
            if(!strncmp((char*)cmdtable[i].cmdname,cmd,len)&&\
	 pf.level>=cmdtable[i].level) {
                (*cmdtable[i].cmdfunc)(cmd);
                break;
            }
	    else reprint=NA;
        }
}

/* 메뉴 형태 F,f L,l 에서의 명령어 받는 함수 */
docommand(cmd,cmdtable)
char *cmd;
struct commands cmdtable[];
{
    int i;
    int len;
    int num;
    int result=1;
    char fileheader[20],bid;
    num=atoi(cmd);
    sprintf(fileheader,"%s",nmenu->ccode);        
    bid=nmenu->form;
    if(num==0) {   
        for(i=0;i<100;i++) {   
            if(cmdtable[i].cmdname==NULL) break;
            len=strlen((char*)cmdtable[i].cmdname);
            if(!strncmp((char*)cmdtable[i].cmdname,cmd,len) && \
		pf.level >= cmdtable[i].level ) {
                (*cmdtable[i].cmdfunc)(cmd);
                break;
            }
        }
    }
}

boardli(char *cmd)
{
    int howmany;
    howmany = parser(cmd);
    sprintf(li,"%s",argument[1]);
    sprintf(lt,"\0");
    sprintf(ld,"\0");sprintf(ln,"\0");
    reprint=YEA;
}
boardlt(char *cmd)
{ 
    int howmany;
    howmany=parser(cmd);
    sprintf(lt,"%s",argument[1]);
    sprintf(li,"\0");
    sprintf(ld,"\0");sprintf(ln,"\0");
    reprint=YEA;
}
boardln(char *cmd)
{
    int howmany;
    howmany = parser(cmd);
    sprintf(ln,"%s",argument[1]);
    sprintf(li,"\0");
    sprintf(ld,"\0"); sprintf(lt,"\0");
    reprint=YEA;
}

boardld(char *cmd)
{
    int howmany;
    howmany=parser(cmd);
    printf("\r\n 지원 안됩니다.");
    sprintf(li,"\0");
    sprintf(lt,"\0");sprintf(ln,"\0");
    reprint=NA;
}
     


uppermenu()
{
    if (nmenu->form!='C') nmenu=nmenu->upp;
    else cmenu=cmenu->upp;
    esc_flag=esc_sub=1;
    reprint=YEA;
    relogo=NA;
}
quitboard()
{
    esc_flag=esc_sub=1;
    upermenu=1;
    reprint=YEA;
}
quitmail()
{
    esc_flag=esc_sub=1;
    upermenu=1;
    reprint=YEA;
}
quitfboard()
{
    esc_flag=esc_sub=1;
    upermenu=1;
    reprint=YEA;
}

writeboard()
{
    int num;
    FILE *fp1;
    char buf[80];
    struct dis_list title;

    if((nmenu->form == 'L' && nmenu->w <= pf.level) ||
	(nmenu->form == 'l' && ((nmenu->w & pf.circle) || nmenu->w == 0))) {
	printf("\r\n 작성방법(1,에디터사용, 3,zmodem, 0.취소) : ");
	keyinput(buf,3);
	if(buf[0] =='1') 
	    num=lineeditor("boards",nmenu->ccode);
	else if(buf[0] =='2') 
	    num = visual_editor("boards");
	else if(buf[0] =='3') {
	    printf("\r\n 아스키파일만 됩니다.");
	    uploadtext("boards");
	}
	else if(buf[0] =='0') {
	    reprint=YEA;
	    printf("\r\n 취소되었습니다. ");
	    return ;
	}
	else {
	    printf("\r\n 라인에디터로 하겠습니다.");
	    num = lineeditor("boards",nmenu->ccode);
	}
	reprint=num;
        now_num = top_num = num;
    }
    else if((nmenu->form == 'F' && (!strcmp(pf.id,"sysop")||!strcmp(pf.id,"darker"))) ||
        (nmenu->form == 'f' && (!strcmp(pf.id,"sysop")||!strcmp(pf.id,"darker")) ) ) {
	num=lineeditor("fboards",nmenu->ccode);
	reprint=num;
	now_num=top_num=num;
    }
    else if(nmenu->form=='T') {
	num=line_editor("TRASH");
	reprint=num;
	now_num=top_num = num;
    }
    else {
        mess_dis(4);
        reprint=NA;
	return;
    }
    level += 1; /* 글을 쓰므로 오르는 레벨 */
    if (nmenu->form=='l'||nmenu->form=='L') 
	sprintf(buf,"boards/%s",nmenu->ccode);
    else if (nmenu->form=='f' || nmenu->form=='F')
	sprintf(buf,"fboards/%s",nmenu->ccode);
    if((fp1=fopen(buf,"r"))==NULL) {
	printf("\r\n 화일헤더가 안 열립니다.");
	pressreturn(); return;
    }
    fseek(fp1,(num-1)*sizeof(struct dis_list),0);
    fread((char*)&title,sizeof(struct dis_list),1,fp1);
    report("write",title);
    fclose(fp1);

}

listtitle(cmd)
char *cmd;
{
    char *tmp;
    int i;
    tmp = &cmd[2];
    i=atoi(tmp);
    if(i) now_num=i;
    else now_num=top_num;
    reprint =YEA;
    sprintf(ln,"\0");
    sprintf(li,"\0");
    sprintf(lt,"\0");
    sprintf(ld,"\0");
}   

nexttitle()
{
    now_num=now_num-CRT;
    if(now_num>0) reprint=YEA;
    else {now_num=0;reprint=NA;}
} 
pretitle()
{
    now_num=now_num+CRT;
    if(now_num>top_num) now_num=top_num;
    if(now_num>0) {
	reprint = YEA;
    }
    else {
	mess_dis(3);
	reprint = NA;
    }
}

    
gototop()
{
    mtop=nmenu=menu;
    esc_flag=1;
    esc_circle=1;
    reprint=YEA;
}

byebye()
{
    host_end_yn();
    reprint = YEA;
}

edittext(cmd)
char *cmd;
{
    FILE *fp1;
    char *tmp,buf[20],buf2[20],direc[30];
    int i;
    tmp = &cmd[4];
    i = atoi(tmp);
    if (i==0) i = now_num;
    printf("\r\n %d 를 수정하겠습니다.",i);

    if(nmenu->form=='f' || nmenu->form=='F')
	strcpy(direc,"fboards");
    else if(nmenu->form=='l' || nmenu->form=='L')
    	strcpy(direc,"boards");
    
    sprintf(buf,"%s/%s",direc,nmenu->ccode);    
    if((fp1=fopen(buf,"r"))==NULL) printf("\r\n 화일이 안 열려요.");
    fseek(fp1,(i-1)*sizeof(struct dis_list),0);
    fread((char*)&l_title,sizeof(struct dis_list),1,fp1);
    fclose(fp1);
    if(i>0 && i<=top_num) {
	if(pf.level>=cfg.del || !strcmp(l_title.id,pf.id)) {
	    printf("\r\n 어떤 것을 수정하시겠습니까?(0;취소,1;제목 2;본문)");
	    lineinput(buf,10,YEA);
	    if(buf[0] =='2') {
		edit_text();
                sprintf(buf,"%s/%s",direc,nmenu->ccode);
                fp1=fopen(buf,"r+");
                fseek(fp1,(i-1)*sizeof(struct dis_list),0);
                fwrite((char*)&l_title,sizeof(struct dis_list),1,fp1);
                fclose(fp1);
		reprint=YEA;
	    }
	    else if(buf[0] =='1') {
		printf("\r\n현재의 제목: %s",l_title.title);
		printf("\r\n새로운 제목: ");
		lineinput(buf,40,YEA);
		if(buf[0]=='\0') printf("\r\n 제목이 NULL 입니다(유의)");
		printf("\r\n확실합니까?(y/N)");
		lineinput(buf2,10,YEA);
		if(buf2[0]=='y' || buf2[0]=='Y') {
 		    strncpy(l_title.title,buf,39);
		    sprintf(buf,"%s/%s",direc,nmenu->ccode);
		    fp1=fopen(buf,"r+");
		    fseek(fp1,(i-1)*sizeof(struct dis_list),0);
		    fwrite((char*)&l_title,sizeof(struct dis_list),1,fp1);
		    fclose(fp1);
		    printf("\r\n 수정되었습니다.");
		    reprint=YEA;
		}
		else printf("\r\n 수정취소요..");
	    }
	    else {
		printf("\r\n 취소합니다.");
		reprint=YEA;
	    }
	}
    }	    
    pressreturn();
}


premenu()
{
    if(nmenu->aeqp!=NULL) {
	nmenu=nmenu->aeqp;
	if((nmenu->form=='R' || nmenu->form == 'E') && (nmenu->w > pf.level)) {
	    nmenu=nmenu->upp;
	}
	esc_flag=1;
	reprint=YEA; 
   }
    else {
	mess_dis(1);
	reprint=NA;
    }
}

nextmenu()
{
    if(nmenu->neqp !=NULL) {
	nmenu=nmenu->neqp;
	if((nmenu->form=='R' || nmenu->form=='E') && (nmenu->w > pf.level)) {
	    nmenu=nmenu->upp;
	}
	esc_flag=1;
	reprint=YEA;
    }
    else {
	mess_dis(1);
	reprint=NA;
    }
}

gotomenu(cmd)
char *cmd;
{
    if(go_select(cmd)) {
	esc_flag = 1;
	esc_circle=1;
	esc_chat=1;
	reprint=YEA;
    }
    else {
	mess_dis(1);
	reprint = NA;
    }
}
/* 일반 편지 지우기이다 (조회수만 -로 만든다)*/
delmail(char *cmd)
{
    char buf[80],direc[80],fhead[80];
    int cnum=0; /* howmany commands are inputted. */
    int num,i;
    cnum = parser(cmd);
    
    for (i=0;i<cnum;i++) {
        strcpy(direc,"letter");
        sprintf(fhead,"%s.mail",pf.id);
        num = atoi(argument[i]);
        if(del_text(direc,fhead,num)) {
            printf("\r\n %d가 삭제되었습니다.",num);
        }       
    }
}

delboard(char *cmd)
{
    char buf[80],direc[80],fhead[80];
    int cnum=0; /* howmany commands are inputted. */
    int num,i;
    cnum = parser(cmd);
    
    for (i=0;i<cnum;i++) {
        strcpy(direc,"boards");
        strcpy(fhead,nmenu->ccode);
        num = atoi(argument[i]);
        if(del_text(direc,fhead,num)) {
	    printf("\r\n %d가 삭제되었습니다.",num);
	}	
    }
}
delfboard(char *cmd)
{
    char buf[80],direc[80],fhead[80];
    int cnum=0; /* howmany commands are inputted. */
    int num,i,start,end;

    cnum = parser(cmd);
    if(pf.level < cfg.del ) {
	printf("\r\n 자료실 삭제 권한이 없습니다.");
	return -1;
    }
    if((cnum>2) &&!strcmp(argument[2],"-")) {
	start=atoi(argument[1]);
	end=atoi(argument[3])+1;
	printf("\r\n %d에서 %d까지 삭제합니다.",start,end);
	for(i=start;i<end;i++) {
	    strcpy(direc,"fboards");
	    strcpy(fhead,nmenu->ccode);
	    if(del_text(direc,fhead,i)) {
		printf("\r\n %d가 삭제되었습니다.",i);
	    }
	}
    }	        
    else {
	for (i=0;i<cnum;i++) {
            strcpy(direc,"fboards");
            strcpy(fhead,nmenu->ccode);
            num = atoi(argument[i]);
            if(del_text(direc,fhead,num)) {
	        printf("\r\n %d가 삭제되었습니다.",num);
	    }
        }
    }
}


help()
{
    if(nmenu->form=='M') logo("mainhelp.hlp",1);
    else if(nmenu->form=='l' || nmenu->form =='L') logo("listhelp.hlp",1);
    else if(nmenu->form=='f' || nmenu->form =='F') logo("filehelp.hlp",1);
    reprint=YEA;
}

feeltext(cmd)
char *cmd;
{
    int i;
    char *tmp,buf[20];
    tmp = &cmd[4];
    i = atoi(tmp);
    if(i==0) i= now_num;
    if(i>0 && i<=top_num) {
	printf("\r\n %d 번 게시물에 공감하십니까?(y/N)>>",i);
	lineinput(buf,1,YEA);
	if(buf[0]=='y' || buf[0] =='Y')
	    l_title.feel++;
	reprint = YEA;
    }
    else {
	mess_dis(2);
	reprint = NA;
    }
}

pfsomeone(cmd)
char *cmd;
{
    reprint=chk_pf(cmd);
    pressreturn();
}

gotoshell()
{
    char buf[80];

    if(pf.level > cfg.sh) {
	printf("\r\n 관리코드: ");
	lineinput(buf,10,NA);
	if(!strcmp(cfg.secret,buf)) {
	    printf("\r\n 쉘로 나갑니다. \r\n");
	    ioctl(0,TCSANOW,&systerm);
	    system("sh");
	    chdir(getenv("HOME"));
	    ioctl(0,TCSANOW,&mbuf);
	}
	reprint=YEA;
    }
    else {
	mess_dis(4);   
	reprint=NA;
    }

}

supersysop()
{
    char buf[80];

    sysop();
    reprint=YEA;
}

redisplay()
{
    reprint=YEA;
}

message(cmd)
char *cmd;
{
    FILE *fp1;
    char buf[80],buff[80];
    char message[80];
    char *whom;
    char terminal[10];
    int i,num;
    int fout;
    int check=0;  /* 사람이 있는가 없는가에 대한 체크이다 */
    struct nowwork work;
    whom = &cmd[3];
    i=0; 
    if( cmd[2]=='\0') {  /* TO 만 쓴 경우이다. */
    	if((fp1=fopen("tmp/WHOISNOW","r")) == NULL) {
		fp1=fopen("tmp/WHOISNOW","a+");
    	} 
    	fseek(fp1,0,0);
    	while(fread((char*)&work,sizeof(struct nowwork),1,fp1)){
		++i;
		printf("\r\n %2d. %9s %9s %10s %6s %c",
			i,work.id,work.name,work.ccode,work.tty,work.chose);
    	}
    	fclose(fp1);
    	printf("\r\n 번호[Return];메시지전송 [c];초대상태변경 >> ");
    	lineinput(buf,10,YEA);
    	num=atoi(buf);
    	if((num<=i) && (num>0)) {
		--num;
		fp1=fopen("tmp/WHOISNOW","r");
		fseek(fp1,num*sizeof(struct nowwork),0);
		fread((char*)&work,sizeof(struct nowwork),1,fp1);
		fclose(fp1);  
		if(work.chose=='*') {
		    printf("\r\n 메시지를 받을 수 없는 사람입니다.");
		    check = 0; 
		}
		else {
		    check = 1;
		    sprintf(terminal,work.tty);
		}
    	}
    	else if(buf[0]=='c' || buf[0]=='C') {
		printf("\r\n");
		change_in();
    	}
    }
    else { /* 아이디가 지정된 경우이다 */
        if((fp1=fopen("tmp/WHOISNOW","r")) == NULL) {
                fp1=fopen("tmp/WHOISNOW","a+");
        } 
	while(fread((char*)&work,sizeof(struct nowwork),1,fp1)) {
	    if (!strcmp(whom,work.id)) {
		if(work.chose=='*') {
		    printf("\r\n 이 사람은 초청을 거부했어요");
		    check = 0;
		}
		else {
		    check =1;
		    sprintf(terminal,work.tty);
		    break;
		}
	    }
	}
    }
    if(check) { /* 찾는 사람이 있으면 메시지를 보낸다.*/
	printf("\r\n 메시지를 쓰세요: ");
	keyinput(buf,40);
	sprintf(message,\
"\033[s\0337\033[23;1H\007\033[7m(%s의메시지)%s\033[0m \0338\033[u",pf.id,buf);
	sprintf(buf,"/dev/%s",terminal);
	fout=open(buf,1);
	write(fout,message,strlen(message));
	close(fout);
	printf("\r\n 메시지를 보냈습니다.");
    }
    pressreturn();
    reprint = YEA;
}
	
editfileheader(cmd)
char *cmd;
{
    FILE *fp1;
    char *tmp,buf[40],buf2[40];
    struct dis_list thisfile;
    int i,change;
    int titlemany;

    tmp = &cmd[7];
    i = atoi(tmp);
    if (i==0) i = now_num;
    printf("\r\n %d 를 수정하겠습니다.",i);
    if (nmenu->form=='l' || nmenu->form=='L') 
    	sprintf(buf,"boards/%s",nmenu->ccode);
    else if (nmenu->form=='f' || nmenu->form =='F')
	sprintf(buf,"fboards/%s",nmenu->ccode);
    else if (nmenu->form=='T')
	sprintf(buf,"TRASH/%s",nmenu->ccode);

    else { 
	printf("\r\n여기는 화일란도 게시판란도 아녀요!!!이상하네");
	pressreturn();
	return;
    }
    if((fp1=fopen(buf,"r+"))==NULL) {
	printf("\r\n 화일이 안 열려요.");
	reprint=NA; return;
    }
    fseek(fp1,0,2); titlemany=ftell(fp1)/ sizeof(struct dis_list);
    if ( i <=0 || i> titlemany) {
	printf("\r\n 숫자가 초과했네요.");pressreturn();reprint=YEA;
	return;
    }
    now_num=i; /* now_num을 i로 바꾼다. */
    fseek(fp1,(i-1)*sizeof(struct dis_list),0);
    fread((char*)&l_title,sizeof(struct dis_list),1,fp1);
    displayfileheader(l_title);
    change=0;
    while(1) {
	printf("바꾸고 싶은 항목은(0=나감;1=수정;)>>");
	lineinput(buf,3,YEA);
	if(buf[0]=='1') {
	    printf("\r\n 지금의 일련번호: %d",l_title.num);
	    printf("\r\n 새로운 일련번호: ");lineinput(buf,29,YEA);
	    if(buf[0]!='\0') l_title.num=atoi(buf);
	    printf("\r\n 지금의 작성자아이디: %s",l_title.id);
	    printf("\r\n 새로운 작성자아이디: ");lineinput(buf,29,YEA);
	    if(buf[0]!='\0') strcpy(l_title.id, buf);
	    printf("\r\n 지금의 작성자이름: %s",l_title.name);
	    printf("\r\n 새로운 작성자이름: ");lineinput(buf,29,YEA);
	    if(buf[0]!='\0') strcpy(l_title.name,buf);
	    printf("\r\n 지금의 화일제목: %s",l_title.title);
	    printf("\r\n 새로운 화일제목: ");lineinput(buf,39,YEA);
	    if(buf[0]!='\0') strcpy(l_title.title,buf);
	    printf("\r\n 지금의 게시물저장화일: %s",l_title.bfilename);
	    printf("\r\n 새로운 게시물저장화일: ");lineinput(buf,29,YEA);
	    if(buf[0]!='\0') strcpy(l_title.bfilename,buf);
	    printf("\r\n 지금의 화일명: %s",l_title.filename);
	    printf("\r\n 바 뀔  화일명: ");lineinput(buf,29,YEA);
	    if(buf[0]!='\0')  strcpy(l_title.filename,buf);
	    change=1; /*바뀌었습니다. */
	}
	else  {
	    printf("\r\n 끝냅니다. "); 
	    break;
	}
	showfileheader(l_title);
    }
    fseek(fp1,(i-1)*sizeof(struct dis_list),0);
    if(change) fwrite((char*)&l_title,sizeof(struct dis_list),1,fp1);	    
    fclose(fp1);
    reprint=NA;

}
/* 화일헤더를 보여준다. */
showfileheader(struct dis_list l_title)
{
            printf("\r\n 지금의 일련번호: %d",l_title.num);
            printf("\r\n 지금의 작성자아이디: %s",l_title.id);
            printf("\r\n 지금의 작성자이름: %s",l_title.name);
            printf("\r\n 지금의 화일제목: %s",l_title.title);
            printf("\r\n 지금의 게시물저장화일: %s",l_title.bfilename);
            printf("\r\n 지금의 화일명: %s",l_title.filename);

}

/* 여기서부터는 화장실 입니다.
---------------------------------------------------------------
*/
trashtext()
{
    FILE *fp1;
    char buf[255];
    int back,i; 
    char direc[20];
    strcpy(direc,"TRASH");
    /* 처음 게시판 들어오면 그 게시판 화일헤더가 만들어져야 한다
        없다면 만들고 있다면.. 있는거다... 이 작업은 display_board에서만
        처음 이루어진다. 왜냐하면.. 화일헤더는 중요한 것이기 때문에
        자주 이루어져서.. 손상이 가면 안되기 때문이다. */
    sprintf(buf,"%s/%s",direc,nmenu->ccode); 
    if((fp1=fopen(buf,"r+"))==NULL) {
        fp1=fopen(buf,"a+");
    }
    li[0]='\0';
    lt[0]='\0';
    if(!fseek(fp1,0,SEEK_END)) {
        now_num=top_num=ftell(fp1)/sizeof(struct dis_list);
    }
    else {
        printf("\r\n 화일이 열리지 않습니다.");
    }
    fclose(fp1);
    back=0; /* 명령어 지울 때 사용할 것임 */
    reprint=YEA;
    esc_flag=0;

    while(1) {
        if(reprint) {
            cls();
            trash_title(direc);
            printf("\r\n --------------------------");
            printf("\r\n %s>> ",nmenu->ccode);
            reprint=NA;
        }
        else {
            for(i=0; i<back; i++) {
                printf("\b \b");
            }
        }
        lineinput(buf,20,YEA);
        back=strlen(buf);
        docommand(buf,boardcmd);
        if(buf[0]=='\0') {   /* 게시물 타이틀보여주기에서 [return]지원 */
            if(now_num>0) { now_num= now_num-CRT;}
            if(now_num<0) now_num=0;
            reprint= YEA;
        }
        if(esc_flag) break;
    }
}


trash_title(direc)
char*direc;
{
    FILE *fp1;
    int i,linenum;
    char buf[255];
    struct tm *tp;

    linenum=now_num;
    if (linenum<=10) linenum=10;

    sprintf(buf,"%s/%s",direc,nmenu->ccode);

    if((fp1=fopen(buf,"r")) ==NULL) {
        printf("\r\n 화일이 열리지 않습니다. 시솝에게 말해주세요.");
        printf("\r\n"); pressreturn();
        reprint=NA; /* 화면이 또 나타나게 하면 안된다. 무한 루프에 빠진다. */
    }
    fseek(fp1,0,2); /* 항상 처음으로 위치를 옮긴다.(확인) */
    i = ftell(fp1) / sizeof(struct dis_list);
    if (i <= linenum) linenum=i;

    printf("여기는 변소예요(냄새가 좀 나지요)?  %s 입니다.",nmenu->ccode);
    printf("\r\n여기는 전혀 제약이 없는 곳입니다\r\n자 화장실 문이 닫힙니다.");
    for(i=0; i<CRT;i++) {
        if(fseek(fp1,(linenum-1)*sizeof(struct dis_list),0)) {
            break;
        }
        else {
            fread((char*)&l_title,sizeof(struct dis_list),1,fp1);
            checkbufferint=display_trashtitle(l_title);
            if(!checkbufferint) i--;
        }
        --linenum;
    }
    fclose(fp1);
}
display_trashtitle(struct dis_list l_title)
{
        struct tm *tp;

            if((l_title.look>=0) || (pf.level >= cfg.del)) {
                if(l_title.look<0) strcpy(l_title.title,"** 삭제 되었음 **");
                tp = localtime(&l_title.date);
                printf("\r\n%4d  %4d%3d%3d %-40s", 
                    l_title.num,
                    l_title.line,l_title.look,l_title.feel,l_title.title);
		return 1;
            }
            return 0;
}
display_tcontent(num,fl,direc)
int num;
int fl;
char *direc;
{
    int ie,count,key,i;
    FILE *fp1,*fp2;
    struct pread {
        long int fbp;
        long int fos;
        int ie;
        struct pread *next;
    } *fbp1, *fbp2;
    char buf[80],fname1[80],fname2[80];
    char c;
    char f;
    long int bfos,nfos;
    int LINELEN=13;
    f= nmenu->form;
    if(num<1 || num>top_num) {
        mess_dis(2);
        reprint=NA;
    }
    else if (1) { 
        reprint=NA;
        now_num=num;  /* 현재의 번호를 게시물 번호로 수정 */
        sprintf(fname1,"%s/%s",direc,nmenu->ccode);
        if((fp1=fopen(fname1,"r+"))==NULL) {
            fp1=fopen(fname1,"a+");
        }
        if(fseek(fp1,(num-1)*sizeof(struct dis_list),0)) {
            printf("\r\nnfseek에러");
        }
        if(!fread((char*)&l_title,sizeof(struct dis_list),1,fp1)) {
            printf("\r\nfread에러 ");
        }
        fclose(fp1);  /* 화일 닫았다. */
        if((l_title.look >=0) || (pf.level > cfg.del)) {
            if(l_title.look >=0) {
                ++l_title.look;
            }
            ie = 0;
            if(fl) {
                count=LINELEN+1;
            }
            else {
                count = -1;
            }
            sprintf(fname2,"%s/%s",direc,l_title.bfilename);
            fp2=fopen(fname2,"r");  /* 화일 읽기 위해 열었다. */
            if((fbp1=(struct pread*)malloc(sizeof(struct pread))) ==NULL) {
                printf("\r\n메모리 부족 ");
            }
            fbp1->next =fbp1;
            fbp1->fbp = ftell(fp2);
            fbp1->fos =0;
            fbp1->ie = 0;
            fbp2=fbp1;
            cls();
            disheader(ie,l_title);
            esc_sub=0;
            fos=0L;
            key=1;
            while(1) {
                if(fos<l_title.size && count && key) {
                    if((c=fgetc(fp2))=='\n') {
                        putchar('\r');
                        --count;
                        ++ie;
                    }
                    printf("%c",c);
                }
                else {
                    key=0;
                    count = LINELEN+1;
                    printf("\r\n 도움말 H ");
                    printf("\r\n >> ");
                    lineinput(buf,20,YEA);
                    if(buf[0] =='p' || buf[0] == 'P') {
                        esc_sub=1;
                        reprint = YEA;
                    }
                    else if(buf[0] =='b' || buf[0] == 'B') {
                        fbp1=fbp1->next;
                        fseek(fp2,fbp1->fbp,0);
                        if(fbp2!=fbp1) {
                            free(fbp2);
                        }
                        fbp2=fbp1;
                        fos = fbp1->fos;
                        ie= fbp1->ie;
                        cls();disheader(ie,l_title);key=1;
                    }
                    else if((buf[0]=='f' || buf[0] =='F'||buf[0]=='\0')&&fos<l_title.size) {
                        if((fbp2=(struct pread *)malloc(sizeof(struct pread))) 
                            ==NULL) {
                            printf("\r\n메모리 부족 ");
                        } 
                        fbp2->fbp = ftell(fp2);
                        fbp2->next =fbp1;
                        fbp2->fos = fos;
                        fbp1= fbp2;
                        fbp1->ie = ie;
                        cls();disheader(ie,l_title); key=1;
                    }
                    else if(buf[0]=='n' || buf[0]=='N') {
                        return (num-1);
                    }
                    else if(buf[0]=='a' || buf[0] =='A') {
                        return (num+1);
                    }
                    else {
                        docommand(buf,tcontentcmd);  
                    }
                }    
                    
                if (esc_sub) break;
                if (fos<l_title.size) fos++;
            }                                            
            while(fbp1->next !=fbp1) {
                fbp1=fbp1->next;
                free(fbp2);
                fbp2=fbp1;
            }
            free(fbp1);
            if((pf.level > cfg.del) && (l_title.look<0)) {
                printf("\r\n 복구하시겠어요?(y/N)? ");
                lineinput(buf,1,YEA);
                if(buf[0] =='Y' || buf[0] =='Y') {
                    l_title.look = abs(l_title.look);
                    printf("\r\n 복구되었어요.");
                }
            }
            fclose(fp2);
            sprintf(fname1,"%s/%s",direc,nmenu->ccode);
            fp1=fopen(fname1,"r+");
            fseek(fp1,(now_num-1)*sizeof(struct dis_list),0);
            fwrite((char*)&l_title,sizeof(struct dis_list),1,fp1);
            fclose(fp1);

            now_num=top_num;
        }
        else {
            mess_dis(2);
            now_num=num;
            reprint = NA;
        }
    }
    else {
        mess_dis(4);
        reprint = NA;
    }
    return 0; /* 0을 보내면... 내용보기에서 나간다. */
}
/*---------------------------------------------------------- 화장실 끝 */

whoisnow()
{
    login_who();
    printf("\r\n");
    pressreturn(); reprint=YEA;
    return;
}


upfile()
{
    FILE *fp1;
    char buf[80];
    sprintf(time_chk,"tmp/STAT.%s",tty);
    tfp = fopen(time_chk,"w");
    fputc('1',tfp); /* 업로드 중이라는 것을 알림 */
    fclose(tfp);
    sprintf(buf,"ftp/%s",nmenu->file);
    uploadfile(buf,"fboards",nmenu->ccode);

    sprintf(time_chk,"tmp/STAT.%s",tty);
    tfp = fopen(time_chk,"w");
    fputc('0',tfp); /* 업로드 중이라는 것을 알림 */
    fclose(tfp);

}

downfile(cmd)
char *cmd;
{
    char buf[80];
    sprintf(time_chk,"tmp/STAT.%s",tty);
    tfp = fopen(time_chk,"w");
    fputc('1',tfp); /* 다운로드 중이라는 것을 알림 */
    fclose(tfp);
    sprintf(buf,"ftp/%s",nmenu->file); 
    downloadfile(cmd,"fboards",nmenu->ccode,buf);
    sprintf(time_chk,"tmp/STAT.%s",tty);
    tfp = fopen(time_chk,"w");
    fputc('0',tfp); /* 업로드 중이라는 것을 알림 */
    fclose(tfp);
}

hideme()
{
	trace = 0;
}
/*
------------------------------------------------------------------------------
이곳부터는 채팅관련명령함수입니다.
------------------------------------------------------------------------------
*/
/*
 방에 들어간다.
*/

joincell(cmd)
char *cmd;
{
    FILE *fp1,*fp2;
    struct roomname cell;

    char *tmp,buf[80];
    int cellnum,howmany;
    int maker=0; /* 방을 만든 사람이면 이것이 1*/
    tmp = &cmd[1];  /* j 1 이런 형식이면 tmp는 j다음의 공백을 가리킨다 */
    cellnum = atoi(tmp);
    if(cellnum <1 || cellnum >5) {
        printf("\r\n 방번호를 선택해주세요.");
        printf("\r\n (1~5,Q:취소)>> ");
        keyinput(buf,10);
        cellnum = atoi(buf);
        if(buf[0] == 'q' || buf[0] =='Q' || cellnum<1 || cellnum>5) {
            printf("\r\n 취소되었습니다.");
            return;
        }
    }
    if((shmid = shmget((key_t)cellnum , SIZE,IPC_CREAT | MODE)) == -1 ) {
        printf("\r\n 대화가 불가합니다.");

        return;
    }
    if((fp1 = fopen("tmp/ROOMINFO","r+")) == NULL) {
	fp1 = fopen("tmp/ROOMINFO","a");
    }
    else {
        fseek(fp1,cellnum*sizeof(struct roomname),SEEK_SET);
	fread((char*)&cell,sizeof(struct roomname),1,fp1);
	sprintf(buf,"tmp/ROOM.%d",cellnum);
	if((fp2 = fopen(buf,"r+")) == NULL) { /* 방화일이 없을 때 */
	    fp2 = fopen(buf,"a"); 
	    cell.howmany = 0;
	    printf("\r\n %d번방을 만듭니다.",cellnum);
	    printf("\r\n 방제목: "); keyinput(cell.name,40);
	    printf("\r\n 방비번: "); keyinput(cell.passwd,8);
	    maker = 1;
	}
	else {   /* 방화일이 있다면 사람이 있는 것이다. */
	    printf("\r\n %d번 방이 개설되어 있습니다.",cellnum);
	}
	fclose(fp2);
	fseek(fp1,cellnum*sizeof(struct roomname),0);
	fwrite((char*)&cell,cellnum*sizeof(struct roomname),1,fp1);
    }
    fclose(fp1);
    fp1 = fopen("tmp/ROOMINFO","r");
    fseek(fp1,cellnum*sizeof(struct roomname),0);
    fread((char*)&cell,sizeof(struct roomname),1,fp1);
    fclose(fp1);
    
    if (maker!=1 && cell.passwd[0] !='\0')   {  /* 비방이면 비번 입력 */
        printf("\r\n 비밀번호: ");
        keyinput(buf,10);
        if(!strcmp(buf,cell.passwd)) {
            chatting_in(shmid,cellnum);
        }
        else {
            printf("\r\n 비밀번호가 틀립니다.");
        }
    }
    else {             /* 공개방이면 그냥 들어감 */
        chatting_in(shmid,cellnum);
    }
    return;
}
 
invitechange()
{
    change_in();
    printf("\r\n");
}

showROOM()
{
    showcell();
    printf("\r\n");
}

helpchat()
{
    cls();
    logo("chat.hlp");
}

exitchat()
{
    deluser(0);  /* 라운지에서 사람을 지운다. */
    esc_chat = 1;
}

gotop()
{
    deluser(0); /* 라운지에서 사람을 지운다. */
    mtop=nmenu=menu;
    esc_chat =1 ;
}

exitchatting()
{
    esc_chatting=1;
}

invite(cmd)
char *cmd;
{
   int i,j;
   char *tmp;
   j = strlen(cmd);
   for(i=2; i<j;i++) {
	if(cmd[i] !=' ') {
	    tmp = &cmd[i];
	    break;
	}
   }
   inviting(tmp);
   printf("\r\n");
}

nickname(cmd)
char *cmd;
{
    printf("\r\n 채팅방에서 쓰일 별명을 적어주세요.");
    printf("\r\n 별명: ");
    keyinput(chatalias,10);
    printf("\r\n 이제부터 %s가 별명입니다.",chatalias);
    printf("\r\n");
}

chatwhoisnow()
{
    login_who();
}
/* 메모를 보내는 명령(메모는 다음 접속때 나오는 메시지다) */
sendmemo(cmd)
char *cmd;
{
    FILE *fp1;
    char *whom,buf[80];
    int check=0;
    struct Memo memopaper;
    struct idst member;

    whom = &cmd[5];
  
    fopen("bin/PASSWDS","r");
    while(fread((char*)&member,sizeof(struct idst),1,fp1)) {
	if(!strcmp(member.id,whom)) {
	    printf("\r\n 회원이 있군요.");
	    check = 1;
	    break;
	}
    }
    if(!check) {
	printf("\r\n 그런 회원은 없습니다.");
	reprint=YEA;
	return;
    }
    sprintf(buf,"tmp/MEMO.%s",whom);
    if((fp1=fopen(buf,"r+"))==NULL) {
	fp1 = fopen(buf,"a+"); 
    }
    fseek(fp1,0,2);
    printf("\r\n 메시지를 써주세요.");
    printf("\r\n 메시지>> ");
    sprintf(memopaper.id,pf.id);
    keyinput(memopaper,40,YEA);
    fwrite((char*)&memopaper,sizeof(struct Memo),1,fp1);
    fclose(fp1);
    reprint=YEA;
}
/* 게시판을 정리하는 명령이다. */
boardsort()
{
    char buf[80];
    char buf2[80];
    sprintf(buf,"%s",nmenu->ccode);
    if(nmenu->form=='l'|| nmenu->form=='L') {
	sprintf(buf2,"boards");
    }
    else {
	sprintf(buf2,"fboards");
    }
    board_sort(buf2,buf);
}
/* 화일게시판을 정리하는 명령이다.
    게시물은 삭제된 것은 완전히 삭제되고,
    화일은 삭제된 것은 ftp/tmp에 저장이 된다.
*/
filesort()
{
    char fheader[80];
    char direc[80];
    char filedir[80];
    sprintf(fheader,"%s",nmenu->ccode);
    sprintf(direc,"fboards");
    sprintf(filedir,"ftp/%s",nmenu->file);
    files_sort(direc,fheader,filedir);
}
mailsort()
{
    char fheader[80];
    char direc[80];
    sprintf(fheader,"%s.mail",pf.id);
    sprintf(direc,"letter");
    mail_sort(direc,fheader);
}
rzlogo()
{
    chdir("logo");
    system("rz");
    chdir(getenv("HOME"));
}
removelogo()
{
    char buf[80],tmp[80];
    printf("\r\n 제거할 로고화일명은: ");keyinput(buf,20);
    sprintf(tmp,"mv logo/%s logo/%s.backup",buf,buf);
    system(tmp);
}
moveboard(char *cmd)
{
    char buf[80],ccode[80],direc[80];
    FILE *fp1;
    int cnum,num,circle,howmany;
    struct dis_list boards;
    cnum = parser(cmd);
    sprintf(buf,"boards/%s",nmenu->ccode);
    fp1=fopen(buf,"r");
    fseek(fp1,0,2); howmany = ftell(fp1)/sizeof(struct dis_list);
    fclose(fp1);
    num = atoi(argument[1]);
    if((num >0) && (num <= howmany)) {
	sprintf(buf,"boards/%s",nmenu->ccode);
	fp1=fopen(buf,"r");
	fseek(fp1,(num-1)*sizeof(struct dis_list),0);
	fread((char*)&boards,sizeof(struct dis_list),1,fp1);
	fclose(fp1);

	printf("\r\n 목적지의 가기코드를 적어주세요: ");
	keyinput(ccode,10);
	if(ccode[0]=='\0') {
	    printf("\r\n 취소하셨습니다.");
	    return 0;
	}
  	printf("\r\n 목적지가 동호회게시판인가요?(y/N) ");
	keyinput(buf,2);
	if(buf[0]=='y' || buf[0]=='Y')  {
	    printf("\r\n 동호회 가기코드는: ");keyinput(buf,10);
	    sprintf(direc,"circle/%s/boards",buf);
	}
	else {
	    sprintf(direc,"boards");
	}
  	if(move_board("boards",boards,direc,ccode)) {
	    printf("\r\n 자료를 옮겼습니다.");
	}
	else return 0;
	/* 게시물을 삭제한다 */
    }
    else {
	printf("\r\n 범위가 안 맞습니다.");
	return 0;
    }
    return 1;
}
movefile(char *cmd)
{
    char buf[80],ccode[80],direc[80],filefrom[80],filedir[80];
    FILE *fp1;
    int cnum,num,circle,howmany;
    struct dis_list boards;
    cnum = parser(cmd);
    sprintf(buf,"fboards/%s",nmenu->ccode);
    fp1=fopen(buf,"r");
    fseek(fp1,0,2); howmany = ftell(fp1)/sizeof(struct dis_list);
    fclose(fp1);
    num = atoi(argument[1]);
    if((num >0) && (num <= howmany)) {
        sprintf(buf,"fboards/%s",nmenu->ccode);
        fp1=fopen(buf,"r");
        fseek(fp1,(num-1)*sizeof(struct dis_list),0);
        fread((char*)&boards,sizeof(struct dis_list),1,fp1);
        fclose(fp1);

        printf("\r\n 목적지의 가기코드를 적어주세요: ");
        keyinput(ccode,10);
        if(ccode[0]=='\0') {
            printf("\r\n 취소하셨습니다.");
            return 0;
        }
        printf("\r\n 목적지가 동호회자료실인가요?(y/N) ");
        keyinput(buf,2);
        if(buf[0]=='y' || buf[0]=='Y')  {
            printf("\r\n 동호회 가기코드는: ");keyinput(buf,10);
            sprintf(direc,"circle/%s/fboards",buf);
	    sprintf(filedir,"circle/%s/files",buf);
	    sprintf(filefrom,"ftp/%s",nmenu->file);
        }
        else {
            sprintf(direc,"fboards");
	    sprintf(filefrom,"ftp/%s",nmenu->file);
	    sprintf(filedir,"ftp/files");
        }
        if(move_file("boards",boards,direc,ccode,filefrom,filedir)) {
            printf("\r\n 자료를 옮겼습니다.");
        }
        else return 0;
        /* 게시물을 삭제한다 */
    }
    else {
        printf("\r\n 범위가 안 맞습니다.");
        return 0;
    }
    return 1;
}
 
