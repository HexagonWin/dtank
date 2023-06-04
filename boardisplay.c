/* ���ο� �Խ��� �����̴�. */
#include "bbs.h"

int displaycontent(int,int,char*,char*,char);

/* �̰��� �⺻ ����̴�. */
show_board(char *direc,char *fhead,struct commands cmdtable[])
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
    li[0]='\0'; ln[0]='\0';
    lt[0]='\0'; ld[0]='\0';
    if(!fseek(fp1,0,2)) {
	now_num=top_num=ftell(fp1) /sizeof(struct dis_list);
    }
    else {
	printf("\r\n ȭ���� ������ �ʽ��ϴ�.");
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
	    printf("%s",HELPLINE);
	    printf("\r\n %s>> ",fhead);
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
	else if(!strncmp(buf,"pr",2)) {
	    parser(buf); num=atoi(argument[1]);
	    displaycontent(num,1,direc,fhead,'l');
	    reprint=YEA;
	}
	else {
	    docommand(buf,cmdtable);
	}
 	if(buf[0]=='\0') {
	    if(now_num>0) { now_num=now_num-CRT;}
	    if(now_num<0) now_num=1;
	    reprint=YEA;
	}
	if(esc_flag) break;
    }
}

titlelist(char*direc, char*fhead)
{
    FILE *fp1;
    int i,linenum;
    char buf[255];
    struct tm *tp;
    linenum = now_num;
    if(linenum<=10 ) linenum=10;
    sprintf(buf,"%s/%s",direc,fhead);
    if((fp1=fopen(buf,"r"))==NULL) {
	printf("\r\n ȭ���� ������ �ʽ��ϴ�. �üٿ��� �����ּ���.");
   	printf("\r\n"); pressreturn();
	reprint=NA;
    }
    fseek(fp1,0,2); i = ftell(fp1) /sizeof(struct dis_list);
    if(i<=linenum) linenum=i;
    if(nmenu->form=='f' || nmenu->form=='F') {
	printf("�ڷ�Խ���(%s)",fhead);printf("%s",FTITLELINE); 
    }
    else if(nmenu->form =='l' || nmenu->form=='L') {
	printf("�Խ���(%s)",fhead);printf("%s",TITLELINE);
    }
    else if(nmenu->form=='C' && (cnmenu->form=='L'||cnmenu->form=='l')) { 
	printf("�Խ���(%s)",fhead);printf("%s",TITLELINE);
    }
    else if(nmenu->form=='C' && (cnmenu->form=='F'||cnmenu->form=='f')) {
	printf("�ڷ�Խ���(%s)",fhead);printf("%s",FTITLELINE);
    }
    else {
	printf("(%s)",fhead); printf("%s",TITLELINE);
    }
    checkbufferint=1;
    for(i=0;i<CRT;i++) {
	if(fseek(fp1,(linenum-1)*sizeof(struct dis_list),0)) {
	    break;
	}
	else {
	    fread((char*)&l_title,sizeof(struct dis_list),1,fp1);
	    checkbufferint=showtitle(l_title);
	    if(!checkbufferint) i--;
	}
	linenum--;
    }
    fclose(fp1);
}

showtitle(struct dis_list title)
{
    struct tm *tp;
    int check=0;
    if(!check) check=compare_li(title,li);
    if(!check) check=compare_lt(title,lt);
    if(!check) check=compare_ld(title,ld);
    if(!check) check=compare_ln(title,ln);

    if(li[0]=='\0' && lt[0]=='\0' && ld[0]=='\0'&&ln[0]=='\0') check=1;

    if(nmenu->form=='l'|| nmenu->form=='L') {
	if((title.look>=0)&&check) {
	    tp = localtime(&title.date);
            printf("\r\n%5d %-8s %-8s %02d/%02d %3d %3d %-38s", 
              title.num,title.name,title.id,tp->tm_mon+1,tp->tm_mday,
              title.look,title.line,title.title);
	    return 1;
	}
	else if(title.look <=0) {
	    if(pf.level >=cfg.del) {
		printf("\r\n �����Ǿ���.");
	    }
	    return 1;
	}
    }
    else if(nmenu->form =='f' || nmenu->form =='F') {
        if((title.look>=0)&&check) {
            tp = localtime(&title.date);
            printf("\r\n%5d %-8s %-12s %10d %-38s",
              title.num,title.id,title.filename,
              title.file_size,title.title);
            return 1;
        }
        else if(title.look <=0) {
            if(pf.level >=cfg.del) {
                printf("\r\n �����Ǿ���.");
            }
	    return 1;
        }
    }
    else if(nmenu->form=='C' && (cnmenu->form=='L'||cnmenu->form=='l')) {
	if((title.look>=0)&&check) {
	    tp = localtime(&title.date);
            printf("\r\n%5d %-8s %-8s %02d/%02d %3d %3d %-38s", 
              title.num,title.name,title.id,tp->tm_mon+1,tp->tm_mday,
              title.look,title.line,title.title);
            return 1;
        }
	else if(title.look <=0) {
	    return 1;
	}
    }
    else if(nmenu->form=='C' &&(cnmenu->form=='F'||cnmenu->form=='f')) {
        if((title.look>=0)&&check) {
            tp = localtime(&title.date);
            printf("\r\n%5d %-8s %-12s %10d %-38s",
              title.num,title.id,title.filename,
              title.file_size,title.title);
            return 1;
        }
        else if(title.look <=0) {
            if(pf.level >=cfg.del) {
                printf("\r\n �����Ǿ���.");
            }
            return 1;
        }
    }
    else {
        if((title.look>=0)&&check) {
            tp = localtime(&title.date);
            printf("\r\n%5d %-8s %-8s %02d/%02d %3d %3d %-38s", 
              title.num,title.name,title.id,tp->tm_mon+1,tp->tm_mday,
              title.look,title.line,title.title);
            return 1;
        }
        else if(title.look <=0) {
            return 1;
        }
    }
    return 0;
}

/* bid�� � �Խ������� �˷��ش�. */
int displaycontent(int num,int fl, char *direc,char *fhead,char bid)
{
    FILE *fp1;
    char buf[80];
    int i;
    int prquit;
    int cmdlen; /* �Է¸� ����� ���� �� ����ϴ� ��ɱ��� */
    int startline=0; /* ���� �� ���� ���� */
    int nextcontent=0; /* �� ���۰� 0�� �ƴϸ� ���� ���� �� �д´�. */
    int howmany,endline;
    int LINELEN=13; /* 13�پ� ǥ�õȴ� */
    struct dis_list r_title;
    prquit=fl; /* ���� ���� ������ pr�̸� pr���Ŀ� �Խù� �б⿡�� ������*/
    sprintf(buf,"%s/%s",direc, fhead);
    fp1 = fopen(buf,"r");
    fseek(fp1,0,2);howmany=ftell(fp1)/sizeof(struct dis_list);
    fclose(fp1);
    if(num<1 || num>howmany) {
	mess_dis(2);
	reprint=NA;
    }
    else {
	reprint=YEA;
	sprintf(buf,"%s/%s",direc,fhead);
	fp1 = fopen(buf,"r");
	fseek(fp1,(num-1)*sizeof(struct dis_list),0);
	fread((char*)&r_title,sizeof(struct dis_list),1,fp1);
	fclose(fp1);
	sprintf(buf,"%s/%s",direc,r_title.bfilename);
	endline=seehowmanyline(buf); /* �Խù��� �� ���� ��*/

	if((r_title.look>=0) || (pf.level > cfg.del)) {
	    /* �б⿡ ���� ���̴�. */
	    esc_sub=0;
	    while(1) {
		if(fl) {
		    printf("\r\n[ĸ�ĸ� �غ��ϰ� ������ ��������]");
		    keyinput(buf,1);printf("\r\n");
		    sprintf(buf,"%s/%s",direc,r_title.bfilename);
		    show_file(buf,0,endline);
		    printf("\r\n[ĸ�ĸ� ��ġ�ð� ������ ��������]");
		    keyinput(buf,1);
		    reprint=YEA;
		    fl=0;
		    if(prquit) {reprint=YEA;esc_sub=1;}
		}
		else if (reprint) {
		    cls();
		    sprintf(buf,"%s/%s",direc,r_title.bfilename);
		    disheader(startline,r_title);
		    show_file(buf,startline,startline+LINELEN);
		    printf("%s",HELPLINE);
		    printf("\r\n %d>> ",endline);
		    reprint=NA;
		}
		else {
		    for(i=0;i< cmdlen;i++) {
			printf("\b \b");
		    }
		}
 	        keyinput(buf,80); cmdlen=strlen(buf);
		if(!strcmp(buf,"pr")) { fl=1;}

		else if(buf[0]=='p' || buf[0]=='P') {
		    nextcontent=0;
		    esc_sub=1;
		}
		else if (buf[0]=='f' || buf[0]=='F'||buf[0]=='\0') {
		    if((startline+LINELEN) >endline) {
			reprint=YEA;
		    }
		    else {
			startline=startline+LINELEN;
			reprint=YEA;
		    }
		}
		else if(buf[0]=='b' || buf[0]=='B') {
		    if(startline<LINELEN) {
			startline=0;
			reprint=YEA;
		    }
		    else {
			startline=startline-LINELEN;
			reprint=YEA;
		    }
		}
		else if(buf[0]=='a' || buf[0]=='A') {
		    nextcontent=num+1;
		    esc_sub=1;
		}
	   	else if(buf[0]=='n' || buf[0]=='N') {
		    nextcontent=num-1;
		    esc_sub=1;
		}
		else {
		    if(bid=='l' || bid=='L') {
		  	docommand(buf,contentcmd);
		    }
		    if(bid=='f' || bid=='F') {
			docommand(buf,fcontentcmd);
		    }
		}
		if(esc_sub) break;
	    }
	    if( (pf.level > cfg.del) && (r_title.look < 0)) {
		printf("\r\n �����Ͻðھ��?(y/N)? ");
		keyinput(buf,1);
		if(buf[0]=='y' || buf[0]=='Y') {
		    r_title.look=1;
		    printf("\r\n �����Ǿ����.");
		}
	    }
	    /* ��ȸ�� ���� */
	    r_title.look++;
	    sprintf(buf,"%s/%s",direc,fhead);
	    fp1=fopen(buf,"r+");
	    fseek(fp1,(num-1)*sizeof(struct dis_list),0);
	    fwrite((char*)&r_title,sizeof(struct dis_list),1,fp1);
	    fclose(fp1);	
	    reprint=YEA;
	}
	else{
	    mess_dis(5);
	    reprint=NA;
	}
    }
    return nextcontent; /* 0�� ������ ���뺸�⿡�� ������. */
}
    
disheader(int start, struct dis_list title)
{
    struct tm *tp,*localtime();
    int i;
    tp = localtime(&title.date);
    if(nmenu->form=='l' || nmenu->form=='L'|| \
      (nmenu->form=='C' && (cnmenu->form=='l' || cnmenu->form=='L'))) {
        printf("\r\n����: %-40s    �ټ�: %d/%d",title.title,start,title.line);
        printf("\r\n��ȣ: %4d    �����: %-8s [%-8s] %2d/%02d/%02d  ��ȸ:%3d",
          title.num,title.name,title.id,tp->tm_year,tp->tm_mon+1,
          tp->tm_mday,title.look);
        printf("\r\n");for(i=0;i<80;i++) printf("-");printf("\r\n");
    }
    else if(nmenu->form=='f' || nmenu->form=='F'|| \
      (nmenu->form=='C' && (cnmenu->form=='F'||cnmenu->form=='f'))) {
        printf("��ȣ : %4d  ����� : %-8s [%-8s]   ȭ�� : %s�ټ� : %d/%d",
          title.num,title.name,title.id,title.filename,start,\
          title.line);
        printf("\r\n��¥ : 19%2d-%02d-%02d %02d:%02d:%02d",
          tp->tm_year,tp->tm_mon+1,tp->tm_mday,tp->tm_hour,tp->tm_min,\
          tp->tm_sec);
        printf("�ٿ� : %dũ�� : %d\r\n���� : %s",
          title.down,title.file_size,title.title);
        printf("\r\n");for (i=0;i<80;i++) printf("-");
    }
    else {
        printf("\r\n����: %-40s    �ټ�: %d/%d",title.title,start,title.line);
        printf("\r\n��ȣ: %4d    �����: %-8s [%-8s] %2d/%02d/%02d  ��ȸ:%3d",
          title.num,title.name,title.id,tp->tm_year,tp->tm_mon+1,
          tp->tm_mday,title.look);
        printf("\r\n");for(i=0;i<80;i++) printf("-");printf("\r\n");
    }

}
	    
		    

compare_li(struct dis_list title,char*who)
{
    if(who[0]=='\0') {
	return 0;
    }
    if(!strncmp(title.id,who,strlen(title.id))) {
	return 1;
    }
    return 0;
}
compare_lt(struct dis_list title,char*what)
{
    int i,j,k;

    if(what[0]=='\0') {
	return 0;
    }
    j=strlen(what);
    k=strlen(title.title)-j+1;
    for(i=0;i<k;i++) {
	if(!strncmp(what,&title.title[i],j)) {
	    return 1;
	}
    }
    return 0;
}
compare_ln(struct dis_list title,char*what)
{
    int i,j,k;

    if(what[0]=='\0') {
        return 0;
    }
    j=strlen(what);
    k=strlen(title.filename)-j+1;
    for(i=0;i<k;i++) {
        if(!strncmp(what,&title.filename[i],j)) {
            return 1;
        }
    }
    return 0;
}
compare_ld(struct dis_list title,char *date)
{
    return 0;
}
   
/* �Խ����� �ٸ� �Խ������� �ű�� �Լ��̴� */
/*  title�� ccode�Խ������� �ű�� ccode�� �Ű��� ���� �����ڵ� */
/* circle�� ��ȣȸ�ΰ� �ƴѰ��� ���� ���� 1�̸� ��ȣȸ 0�̸� ��ü */
move_board(char*from,struct dis_list boards,char*direc, char *ccode)
{
    FILE *fp1;
    char tmp[80],buf[80],bfilename[80];
    time_t t;
    struct tm *tp,*localtime();

    if(1) {
	sprintf(buf,"%s/%s",direc,ccode);
        if( (fp1=fopen(buf,"r"))==NULL) {
	    printf("\r\n �׷� �����ڵ�� �����ϴ�.");
	    return 0;
	}
	fclose(fp1);
	if( (fp1=fopen(buf,"r+"))!=NULL) {
	    fseek(fp1,0,2); boards.num=ftell(fp1)/sizeof(struct dis_list)+1;
	    time(&t);boards.date=t;sprintf(bfilename,boards.bfilename);
	    sprintf(boards.bfilename,"%s%d.txt",ccode,t);
	    sprintf(buf,"cp %s/%s %s/%s",from,bfilename,\
	      direc,boards.bfilename);
	    system(buf);
	    fseek(fp1,0,2);
	    fwrite((char*)&boards,sizeof(struct dis_list),1,fp1);
	    fclose(fp1);
	} 
	else return 0;
    }
    return 1; 
}
 
