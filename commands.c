
#include "bbs.h"
#include "commands.h"
/*  M,m ����޴���ġ�� chatting��ġ������ ��ɾ� �޴� �Լ� */
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

/* �޴� ���� F,f L,l ������ ��ɾ� �޴� �Լ� */
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
    printf("\r\n ���� �ȵ˴ϴ�.");
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
	printf("\r\n �ۼ����(1,�����ͻ��, 3,zmodem, 0.���) : ");
	keyinput(buf,3);
	if(buf[0] =='1') 
	    num=lineeditor("boards",nmenu->ccode);
	else if(buf[0] =='2') 
	    num = visual_editor("boards");
	else if(buf[0] =='3') {
	    printf("\r\n �ƽ�Ű���ϸ� �˴ϴ�.");
	    uploadtext("boards");
	}
	else if(buf[0] =='0') {
	    reprint=YEA;
	    printf("\r\n ��ҵǾ����ϴ�. ");
	    return ;
	}
	else {
	    printf("\r\n ���ο����ͷ� �ϰڽ��ϴ�.");
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
    level += 1; /* ���� ���Ƿ� ������ ���� */
    if (nmenu->form=='l'||nmenu->form=='L') 
	sprintf(buf,"boards/%s",nmenu->ccode);
    else if (nmenu->form=='f' || nmenu->form=='F')
	sprintf(buf,"fboards/%s",nmenu->ccode);
    if((fp1=fopen(buf,"r"))==NULL) {
	printf("\r\n ȭ������� �� �����ϴ�.");
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
    printf("\r\n %d �� �����ϰڽ��ϴ�.",i);

    if(nmenu->form=='f' || nmenu->form=='F')
	strcpy(direc,"fboards");
    else if(nmenu->form=='l' || nmenu->form=='L')
    	strcpy(direc,"boards");
    
    sprintf(buf,"%s/%s",direc,nmenu->ccode);    
    if((fp1=fopen(buf,"r"))==NULL) printf("\r\n ȭ���� �� ������.");
    fseek(fp1,(i-1)*sizeof(struct dis_list),0);
    fread((char*)&l_title,sizeof(struct dis_list),1,fp1);
    fclose(fp1);
    if(i>0 && i<=top_num) {
	if(pf.level>=cfg.del || !strcmp(l_title.id,pf.id)) {
	    printf("\r\n � ���� �����Ͻðڽ��ϱ�?(0;���,1;���� 2;����)");
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
		printf("\r\n������ ����: %s",l_title.title);
		printf("\r\n���ο� ����: ");
		lineinput(buf,40,YEA);
		if(buf[0]=='\0') printf("\r\n ������ NULL �Դϴ�(����)");
		printf("\r\nȮ���մϱ�?(y/N)");
		lineinput(buf2,10,YEA);
		if(buf2[0]=='y' || buf2[0]=='Y') {
 		    strncpy(l_title.title,buf,39);
		    sprintf(buf,"%s/%s",direc,nmenu->ccode);
		    fp1=fopen(buf,"r+");
		    fseek(fp1,(i-1)*sizeof(struct dis_list),0);
		    fwrite((char*)&l_title,sizeof(struct dis_list),1,fp1);
		    fclose(fp1);
		    printf("\r\n �����Ǿ����ϴ�.");
		    reprint=YEA;
		}
		else printf("\r\n ������ҿ�..");
	    }
	    else {
		printf("\r\n ����մϴ�.");
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
/* �Ϲ� ���� ������̴� (��ȸ���� -�� �����)*/
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
            printf("\r\n %d�� �����Ǿ����ϴ�.",num);
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
	    printf("\r\n %d�� �����Ǿ����ϴ�.",num);
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
	printf("\r\n �ڷ�� ���� ������ �����ϴ�.");
	return -1;
    }
    if((cnum>2) &&!strcmp(argument[2],"-")) {
	start=atoi(argument[1]);
	end=atoi(argument[3])+1;
	printf("\r\n %d���� %d���� �����մϴ�.",start,end);
	for(i=start;i<end;i++) {
	    strcpy(direc,"fboards");
	    strcpy(fhead,nmenu->ccode);
	    if(del_text(direc,fhead,i)) {
		printf("\r\n %d�� �����Ǿ����ϴ�.",i);
	    }
	}
    }	        
    else {
	for (i=0;i<cnum;i++) {
            strcpy(direc,"fboards");
            strcpy(fhead,nmenu->ccode);
            num = atoi(argument[i]);
            if(del_text(direc,fhead,num)) {
	        printf("\r\n %d�� �����Ǿ����ϴ�.",num);
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
	printf("\r\n %d �� �Խù��� �����Ͻʴϱ�?(y/N)>>",i);
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
	printf("\r\n �����ڵ�: ");
	lineinput(buf,10,NA);
	if(!strcmp(cfg.secret,buf)) {
	    printf("\r\n ���� �����ϴ�. \r\n");
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
    int check=0;  /* ����� �ִ°� ���°��� ���� üũ�̴� */
    struct nowwork work;
    whom = &cmd[3];
    i=0; 
    if( cmd[2]=='\0') {  /* TO �� �� ����̴�. */
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
    	printf("\r\n ��ȣ[Return];�޽������� [c];�ʴ���º��� >> ");
    	lineinput(buf,10,YEA);
    	num=atoi(buf);
    	if((num<=i) && (num>0)) {
		--num;
		fp1=fopen("tmp/WHOISNOW","r");
		fseek(fp1,num*sizeof(struct nowwork),0);
		fread((char*)&work,sizeof(struct nowwork),1,fp1);
		fclose(fp1);  
		if(work.chose=='*') {
		    printf("\r\n �޽����� ���� �� ���� ����Դϴ�.");
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
    else { /* ���̵� ������ ����̴� */
        if((fp1=fopen("tmp/WHOISNOW","r")) == NULL) {
                fp1=fopen("tmp/WHOISNOW","a+");
        } 
	while(fread((char*)&work,sizeof(struct nowwork),1,fp1)) {
	    if (!strcmp(whom,work.id)) {
		if(work.chose=='*') {
		    printf("\r\n �� ����� ��û�� �ź��߾��");
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
    if(check) { /* ã�� ����� ������ �޽����� ������.*/
	printf("\r\n �޽����� ������: ");
	keyinput(buf,40);
	sprintf(message,\
"\033[s\0337\033[23;1H\007\033[7m(%s�Ǹ޽���)%s\033[0m \0338\033[u",pf.id,buf);
	sprintf(buf,"/dev/%s",terminal);
	fout=open(buf,1);
	write(fout,message,strlen(message));
	close(fout);
	printf("\r\n �޽����� ���½��ϴ�.");
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
    printf("\r\n %d �� �����ϰڽ��ϴ�.",i);
    if (nmenu->form=='l' || nmenu->form=='L') 
    	sprintf(buf,"boards/%s",nmenu->ccode);
    else if (nmenu->form=='f' || nmenu->form =='F')
	sprintf(buf,"fboards/%s",nmenu->ccode);
    else if (nmenu->form=='T')
	sprintf(buf,"TRASH/%s",nmenu->ccode);

    else { 
	printf("\r\n����� ȭ�϶��� �Խ��Ƕ��� �Ƴ��!!!�̻��ϳ�");
	pressreturn();
	return;
    }
    if((fp1=fopen(buf,"r+"))==NULL) {
	printf("\r\n ȭ���� �� ������.");
	reprint=NA; return;
    }
    fseek(fp1,0,2); titlemany=ftell(fp1)/ sizeof(struct dis_list);
    if ( i <=0 || i> titlemany) {
	printf("\r\n ���ڰ� �ʰ��߳׿�.");pressreturn();reprint=YEA;
	return;
    }
    now_num=i; /* now_num�� i�� �ٲ۴�. */
    fseek(fp1,(i-1)*sizeof(struct dis_list),0);
    fread((char*)&l_title,sizeof(struct dis_list),1,fp1);
    displayfileheader(l_title);
    change=0;
    while(1) {
	printf("�ٲٰ� ���� �׸���(0=����;1=����;)>>");
	lineinput(buf,3,YEA);
	if(buf[0]=='1') {
	    printf("\r\n ������ �Ϸù�ȣ: %d",l_title.num);
	    printf("\r\n ���ο� �Ϸù�ȣ: ");lineinput(buf,29,YEA);
	    if(buf[0]!='\0') l_title.num=atoi(buf);
	    printf("\r\n ������ �ۼ��ھ��̵�: %s",l_title.id);
	    printf("\r\n ���ο� �ۼ��ھ��̵�: ");lineinput(buf,29,YEA);
	    if(buf[0]!='\0') strcpy(l_title.id, buf);
	    printf("\r\n ������ �ۼ����̸�: %s",l_title.name);
	    printf("\r\n ���ο� �ۼ����̸�: ");lineinput(buf,29,YEA);
	    if(buf[0]!='\0') strcpy(l_title.name,buf);
	    printf("\r\n ������ ȭ������: %s",l_title.title);
	    printf("\r\n ���ο� ȭ������: ");lineinput(buf,39,YEA);
	    if(buf[0]!='\0') strcpy(l_title.title,buf);
	    printf("\r\n ������ �Խù�����ȭ��: %s",l_title.bfilename);
	    printf("\r\n ���ο� �Խù�����ȭ��: ");lineinput(buf,29,YEA);
	    if(buf[0]!='\0') strcpy(l_title.bfilename,buf);
	    printf("\r\n ������ ȭ�ϸ�: %s",l_title.filename);
	    printf("\r\n �� ��  ȭ�ϸ�: ");lineinput(buf,29,YEA);
	    if(buf[0]!='\0')  strcpy(l_title.filename,buf);
	    change=1; /*�ٲ�����ϴ�. */
	}
	else  {
	    printf("\r\n �����ϴ�. "); 
	    break;
	}
	showfileheader(l_title);
    }
    fseek(fp1,(i-1)*sizeof(struct dis_list),0);
    if(change) fwrite((char*)&l_title,sizeof(struct dis_list),1,fp1);	    
    fclose(fp1);
    reprint=NA;

}
/* ȭ������� �����ش�. */
showfileheader(struct dis_list l_title)
{
            printf("\r\n ������ �Ϸù�ȣ: %d",l_title.num);
            printf("\r\n ������ �ۼ��ھ��̵�: %s",l_title.id);
            printf("\r\n ������ �ۼ����̸�: %s",l_title.name);
            printf("\r\n ������ ȭ������: %s",l_title.title);
            printf("\r\n ������ �Խù�����ȭ��: %s",l_title.bfilename);
            printf("\r\n ������ ȭ�ϸ�: %s",l_title.filename);

}

/* ���⼭���ʹ� ȭ��� �Դϴ�.
---------------------------------------------------------------
*/
trashtext()
{
    FILE *fp1;
    char buf[255];
    int back,i; 
    char direc[20];
    strcpy(direc,"TRASH");
    /* ó�� �Խ��� ������ �� �Խ��� ȭ������� ��������� �Ѵ�
        ���ٸ� ����� �ִٸ�.. �ִ°Ŵ�... �� �۾��� display_board������
        ó�� �̷������. �ֳ��ϸ�.. ȭ������� �߿��� ���̱� ������
        ���� �̷������.. �ջ��� ���� �ȵǱ� �����̴�. */
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
        printf("\r\n ȭ���� ������ �ʽ��ϴ�.");
    }
    fclose(fp1);
    back=0; /* ��ɾ� ���� �� ����� ���� */
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
        if(buf[0]=='\0') {   /* �Խù� Ÿ��Ʋ�����ֱ⿡�� [return]���� */
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
        printf("\r\n ȭ���� ������ �ʽ��ϴ�. �üٿ��� �����ּ���.");
        printf("\r\n"); pressreturn();
        reprint=NA; /* ȭ���� �� ��Ÿ���� �ϸ� �ȵȴ�. ���� ������ ������. */
    }
    fseek(fp1,0,2); /* �׻� ó������ ��ġ�� �ű��.(Ȯ��) */
    i = ftell(fp1) / sizeof(struct dis_list);
    if (i <= linenum) linenum=i;

    printf("����� ���ҿ���(������ �� ������)?  %s �Դϴ�.",nmenu->ccode);
    printf("\r\n����� ���� ������ ���� ���Դϴ�\r\n�� ȭ��� ���� �����ϴ�.");
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
                if(l_title.look<0) strcpy(l_title.title,"** ���� �Ǿ��� **");
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
        now_num=num;  /* ������ ��ȣ�� �Խù� ��ȣ�� ���� */
        sprintf(fname1,"%s/%s",direc,nmenu->ccode);
        if((fp1=fopen(fname1,"r+"))==NULL) {
            fp1=fopen(fname1,"a+");
        }
        if(fseek(fp1,(num-1)*sizeof(struct dis_list),0)) {
            printf("\r\nnfseek����");
        }
        if(!fread((char*)&l_title,sizeof(struct dis_list),1,fp1)) {
            printf("\r\nfread���� ");
        }
        fclose(fp1);  /* ȭ�� �ݾҴ�. */
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
            fp2=fopen(fname2,"r");  /* ȭ�� �б� ���� ������. */
            if((fbp1=(struct pread*)malloc(sizeof(struct pread))) ==NULL) {
                printf("\r\n�޸� ���� ");
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
                    printf("\r\n ���� H ");
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
                            printf("\r\n�޸� ���� ");
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
                printf("\r\n �����Ͻðھ��?(y/N)? ");
                lineinput(buf,1,YEA);
                if(buf[0] =='Y' || buf[0] =='Y') {
                    l_title.look = abs(l_title.look);
                    printf("\r\n �����Ǿ����.");
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
    return 0; /* 0�� ������... ���뺸�⿡�� ������. */
}
/*---------------------------------------------------------- ȭ��� �� */

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
    fputc('1',tfp); /* ���ε� ���̶�� ���� �˸� */
    fclose(tfp);
    sprintf(buf,"ftp/%s",nmenu->file);
    uploadfile(buf,"fboards",nmenu->ccode);

    sprintf(time_chk,"tmp/STAT.%s",tty);
    tfp = fopen(time_chk,"w");
    fputc('0',tfp); /* ���ε� ���̶�� ���� �˸� */
    fclose(tfp);

}

downfile(cmd)
char *cmd;
{
    char buf[80];
    sprintf(time_chk,"tmp/STAT.%s",tty);
    tfp = fopen(time_chk,"w");
    fputc('1',tfp); /* �ٿ�ε� ���̶�� ���� �˸� */
    fclose(tfp);
    sprintf(buf,"ftp/%s",nmenu->file); 
    downloadfile(cmd,"fboards",nmenu->ccode,buf);
    sprintf(time_chk,"tmp/STAT.%s",tty);
    tfp = fopen(time_chk,"w");
    fputc('0',tfp); /* ���ε� ���̶�� ���� �˸� */
    fclose(tfp);
}

hideme()
{
	trace = 0;
}
/*
------------------------------------------------------------------------------
�̰����ʹ� ä�ð��ø���Լ��Դϴ�.
------------------------------------------------------------------------------
*/
/*
 �濡 ����.
*/

joincell(cmd)
char *cmd;
{
    FILE *fp1,*fp2;
    struct roomname cell;

    char *tmp,buf[80];
    int cellnum,howmany;
    int maker=0; /* ���� ���� ����̸� �̰��� 1*/
    tmp = &cmd[1];  /* j 1 �̷� �����̸� tmp�� j������ ������ ����Ų�� */
    cellnum = atoi(tmp);
    if(cellnum <1 || cellnum >5) {
        printf("\r\n ���ȣ�� �������ּ���.");
        printf("\r\n (1~5,Q:���)>> ");
        keyinput(buf,10);
        cellnum = atoi(buf);
        if(buf[0] == 'q' || buf[0] =='Q' || cellnum<1 || cellnum>5) {
            printf("\r\n ��ҵǾ����ϴ�.");
            return;
        }
    }
    if((shmid = shmget((key_t)cellnum , SIZE,IPC_CREAT | MODE)) == -1 ) {
        printf("\r\n ��ȭ�� �Ұ��մϴ�.");

        return;
    }
    if((fp1 = fopen("tmp/ROOMINFO","r+")) == NULL) {
	fp1 = fopen("tmp/ROOMINFO","a");
    }
    else {
        fseek(fp1,cellnum*sizeof(struct roomname),SEEK_SET);
	fread((char*)&cell,sizeof(struct roomname),1,fp1);
	sprintf(buf,"tmp/ROOM.%d",cellnum);
	if((fp2 = fopen(buf,"r+")) == NULL) { /* ��ȭ���� ���� �� */
	    fp2 = fopen(buf,"a"); 
	    cell.howmany = 0;
	    printf("\r\n %d������ ����ϴ�.",cellnum);
	    printf("\r\n ������: "); keyinput(cell.name,40);
	    printf("\r\n ����: "); keyinput(cell.passwd,8);
	    maker = 1;
	}
	else {   /* ��ȭ���� �ִٸ� ����� �ִ� ���̴�. */
	    printf("\r\n %d�� ���� �����Ǿ� �ֽ��ϴ�.",cellnum);
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
    
    if (maker!=1 && cell.passwd[0] !='\0')   {  /* ����̸� ��� �Է� */
        printf("\r\n ��й�ȣ: ");
        keyinput(buf,10);
        if(!strcmp(buf,cell.passwd)) {
            chatting_in(shmid,cellnum);
        }
        else {
            printf("\r\n ��й�ȣ�� Ʋ���ϴ�.");
        }
    }
    else {             /* �������̸� �׳� �� */
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
    deluser(0);  /* ��������� ����� �����. */
    esc_chat = 1;
}

gotop()
{
    deluser(0); /* ��������� ����� �����. */
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
    printf("\r\n ä�ù濡�� ���� ������ �����ּ���.");
    printf("\r\n ����: ");
    keyinput(chatalias,10);
    printf("\r\n �������� %s�� �����Դϴ�.",chatalias);
    printf("\r\n");
}

chatwhoisnow()
{
    login_who();
}
/* �޸� ������ ���(�޸�� ���� ���Ӷ� ������ �޽�����) */
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
	    printf("\r\n ȸ���� �ֱ���.");
	    check = 1;
	    break;
	}
    }
    if(!check) {
	printf("\r\n �׷� ȸ���� �����ϴ�.");
	reprint=YEA;
	return;
    }
    sprintf(buf,"tmp/MEMO.%s",whom);
    if((fp1=fopen(buf,"r+"))==NULL) {
	fp1 = fopen(buf,"a+"); 
    }
    fseek(fp1,0,2);
    printf("\r\n �޽����� ���ּ���.");
    printf("\r\n �޽���>> ");
    sprintf(memopaper.id,pf.id);
    keyinput(memopaper,40,YEA);
    fwrite((char*)&memopaper,sizeof(struct Memo),1,fp1);
    fclose(fp1);
    reprint=YEA;
}
/* �Խ����� �����ϴ� ����̴�. */
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
/* ȭ�ϰԽ����� �����ϴ� ����̴�.
    �Խù��� ������ ���� ������ �����ǰ�,
    ȭ���� ������ ���� ftp/tmp�� ������ �ȴ�.
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
    printf("\r\n ������ �ΰ�ȭ�ϸ���: ");keyinput(buf,20);
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

	printf("\r\n �������� �����ڵ带 �����ּ���: ");
	keyinput(ccode,10);
	if(ccode[0]=='\0') {
	    printf("\r\n ����ϼ̽��ϴ�.");
	    return 0;
	}
  	printf("\r\n �������� ��ȣȸ�Խ����ΰ���?(y/N) ");
	keyinput(buf,2);
	if(buf[0]=='y' || buf[0]=='Y')  {
	    printf("\r\n ��ȣȸ �����ڵ��: ");keyinput(buf,10);
	    sprintf(direc,"circle/%s/boards",buf);
	}
	else {
	    sprintf(direc,"boards");
	}
  	if(move_board("boards",boards,direc,ccode)) {
	    printf("\r\n �ڷḦ �Ű���ϴ�.");
	}
	else return 0;
	/* �Խù��� �����Ѵ� */
    }
    else {
	printf("\r\n ������ �� �½��ϴ�.");
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

        printf("\r\n �������� �����ڵ带 �����ּ���: ");
        keyinput(ccode,10);
        if(ccode[0]=='\0') {
            printf("\r\n ����ϼ̽��ϴ�.");
            return 0;
        }
        printf("\r\n �������� ��ȣȸ�ڷ���ΰ���?(y/N) ");
        keyinput(buf,2);
        if(buf[0]=='y' || buf[0]=='Y')  {
            printf("\r\n ��ȣȸ �����ڵ��: ");keyinput(buf,10);
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
            printf("\r\n �ڷḦ �Ű���ϴ�.");
        }
        else return 0;
        /* �Խù��� �����Ѵ� */
    }
    else {
        printf("\r\n ������ �� �½��ϴ�.");
        return 0;
    }
    return 1;
}
 
