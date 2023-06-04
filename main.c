
#include "bbs.h"
rawmode()
{
    struct termio tbuf;
    if (tcgetattr(0, &systerm) == -1) {
        perror("tcgetattr");
        exit(1);
    }
    tbuf.c_cc[4] = 1;
    tbuf.c_cc[5] = 0;
    tbuf.c_iflag = 0;
    tbuf.c_iflag |= IXON;
    tbuf.c_iflag |= IXANY;
    tbuf.c_oflag = 0;
/*    tbuf.c_oflag &= ~OPOST; */
    tbuf.c_lflag &= ~(ICANON | ISIG | ECHO); 
    tbuf.c_cflag &= ~PARENB;
    tbuf.c_cflag &= ~CSIZE;
    tbuf.c_cflag |= CS8;
    ioctl(0, TCSANOW, &tbuf);
    return;
}
/* ���� ���α׷��Դϴ�. */
main(argc, argv)
int argc;
char *argv[];
{
    char* getty;
    char *ttyname();
    struct termios term;
    int i, j, k, select, back, key, ps,check;
    char *tmp, buf[80], *genbuf, ch;
    FILE *fp1, *fp2, *fp3;
    struct tm *tp,*tp2;
    struct tm *localtime();
    int today,howmany;
    struct dis_list board;
    time_t t;
    struct mail letter;
    struct tm *tim();
/* �ñ׳� ó���κ�. �� �κ��� ��ȭ�� ������ ���� ����
	���μ��� ��źκ��� �����ϴ� ���̴�. */
    signal(SIGQUIT, SIG_IGN);
    signal(SIGINT, SIG_IGN);
    signal(SIGTERM, SIG_IGN);
    signal(SIGHUP, host_end);
    signal(SIGSEGV, host_end);
    signal(SIGBUS, host_end);
    signal(SIGCHLD,SIG_IGN);
        signal(SIGPIPE,SIG_IGN);
        signal(SIGALRM,SIG_IGN);
        signal(SIGURG,SIG_IGN);
        signal(SIGTSTP,SIG_IGN);
        signal(SIGTTIN,SIG_IGN);
        signal(SIGTTOU,SIG_IGN);
	signal(SIGPWR,host_end);
	signal(SIGSTOP,SIG_IGN);
	signal(SIGVTALRM,host_end);
	signal(SIGWINCH,SIG_IGN);
	signal(SIGXCPU,SIG_IGN);
	signal(SIGXFSZ,host_end);
    highuser = 0;
    umask(0111); 
    if (tcgetattr(0, &systerm) == -1) {
        perror("tcgetattr");
        exit(1);
    }
    rawmode();  /* �� ĳ���Į ��� */
    if (tcgetattr(0, &systerm) == -1) {
        perror("tcgetattr");
        exit(1);
    }
    umask(0111);
    trace=1;
    level=0;

    sprintf(tty,argv[1]); /* ctime���� ���۵� �μ��� 1��° �μ��� tty�� �ν� */
    printf("����� %s ��Ʈ�� �����ϼ̽��ϴ�.",tty); /* tty�� ��ü���� ���� �� */
/* �޽��� ȭ���� �����. */
    sprintf(buf,"tmp/mail.%s",tty);
    unlink(buf);
/* ���� ȭ�Ͽ� 0�� ����ִ´�. �̰��� ������¶�� ���� �˸��� ���̴�. */
    sprintf(time_chk,"tmp/STAT.%s",tty);
    tfp = fopen(time_chk,"w");
    fputc('0',tfp);
    fclose(tfp);
    if(!strcmp(argv[1],"m")) Make_Menu(1);  /* �޴��� ����� �ɼ� */
    Get_Menu();         /* �޴� �б� */

    readDOLTANKCFG(); /* ����ũ�� �⺻config�� �б� DOLTANK.CFG */

/* buf;��������ccode,cfg.del;�Խù���������,cfg.sh;�ý��۽���뷹��,
    cfg.chat;�������ȭ�ǿ� ������� ���� ����
    cfg.seroom;�������ȭ�Ǽ�,cfg.inc;1ȸ ���ӽ� ���� ������,
    cfg.secret;��ڸ޴��׽ý��� ����й�ȣ
    cfg.flag; ������ 0:���� 1:����
    cfg.room; ��ȭ�� �ѹ��� �ο�����;
*/

    chatcheck = 0;
    cls();
    /*  ���� �ΰ� ����Ѵ� */
    logo("doltank.log",1);
    cls();
    logo("welcome.log",0);    
    /* ���������� ����Ѵ� */
    /* id check�� �ǽ��Ѵ� */
    guest=0; /* �ϴ��� ��ȸ������ ���� */
    guest=id_in();
    if(guest) {
	printf("\r\n\r\n ����� �մ��ڰ����� �����ϼ̽��ϴ�.");
	printf("\r\n \r\n ");
	pressreturn();
    }    
    display_kongzi();
    /* ������ üũ�Ѵ� */
    if((i=check_letter("letter"))) {
	printf("\r\n ������ %d�� �Խ��ϴ�.",i);
	pressreturn();
    }
    writeLOGFILE(1);

    pf_fos=fos;  /* ����ñ��� ������ ���� pf_id������ file offset�̴� */
		/* ���� �մ��̶�� pf_id.tmp������ file offset�̴� */
    i=pf.num;
    if(!guest) {
        sprintf(buf,"bin/PASSWDS");
    }
    else {
        sprintf(buf,"bin/PASSWDS.TMP");
    }
  /* �ٽ� �ѹ� �Ż���� �д´�(Ȯ����) */
    if((fp1 = fopen(buf,"r")) == NULL) {
        printf("\r\nPASSWDS ����");
        err_off();
    }
    fseek(fp1,pf_fos,SEEK_SET);
    fread((char*)&pf,sizeof(struct idst),1,fp1);
    fclose(fp1);
    if(i != pf.num) {
        printf("\r\nȸ�� ��ȣ�� Ʋ���ϴ�.");
        err_off();
    }
    chk_csysop(); /* ��ȣȸ ����ΰ� Ȯ�� */
  /* �ڱ� ���̵�� ���� �����߳� �˻� */
/* �ִ� �� ������ �̹� id_in()�ߴ�. */
    reprint=relogo=1;
    nmenu = menu;

    mtop = menu;
/* �������� ��ɾ� �Է� �κ�... �̰����� �����Դϴ�. */
    cls();
    while(1) {          /* ���� �����Դϴ�. ������ ��ƾ */
        mess(); 
        if(nmenu->form != 'M') {      /*  ���� �޴��� ���� ������ */
            reprint = chose_menu();
        }
        else {         
	    chdir(getenv("HOME"));
	    if(nmenu->r > pf.level) { /* ���� ������ ���� */
		printf("\r\n �װ��� ���� �����Դϴ�.");
		printf("\r\n ������ �����ϴ�.");
		printf("\r\n �üٿ��� ���غ��ʽÿ�.");
		pressreturn();
		nmenu=nmenu->upp;
	    }

            if(reprint) {  /* reprint�� �������� */
                mtop = nmenu;
                mtmp = mtop->downp;
		if (relogo) {
		    relogo==NA;
		    sprintf(buf,"%s.ent",mtop->ccode);
		    logo(buf,1);
		}
		/* �޴�ȭ�� ���  */
                sprintf(buf,"logo/%s.mnu",mtop->ccode);
                if((fp1 = fopen(buf,"r")) == NULL) {
                    cls();
                    printf("\r\n  %10s [%s] ����(%d)",mtop->list,mtop->ccode,mtop->deep);
		    printf("\r\n");for(i=0; i<80; i++) printf("*"); 
                    j = 0;
                    while(mtmp != NULL) {
                        if (mtmp->r < pf.level) {
                            if(mtmp->form=='l' || mtmp->form=='L') {
                                sprintf(buf,"boards/%s",\
                                  mtmp->ccode);
                                fp2=fopen(buf,"r");
                                fseek(fp2,0,2);
                                howmany = ftell(fp2) / sizeof(struct dis_list);
                                fseek(fp2,0,0);
				today=0;
                                while(fread((char*)&board,\
                                sizeof(struct dis_list),1,fp2)) {
                                    time(&t);
                                    tp = localtime(&t);
                                    tp2= localtime(&board.date);
                                    if((tp->tm_year)==(tp2->tm_year) &&\
                                      (tp->tm_mon)==(tp2->tm_mon) &&\
                                      (tp->tm_mday)==(tp2->tm_mday)) today++;
                                }
                           /*     today = howmany - (ftell(fp2)/sizeof(\
                                    struct dis_list)) +1; */
                                if(howmany==0) today=0;
                                printf("\r\n  %3s. %-39s [%9s][%d/%d]",\
                                  mtmp->choice,mtmp->list,mtmp->ccode,today,howmany);
                            }
                            else printf("\r\n  %3s. %-39s [%9s]",\
                                mtmp->choice,mtmp->list,mtmp->ccode);

                        }       
			mtmp=mtmp->neqp;
                    }
                    printf("%s\r\n %s>> ",HELPLINE,nmenu->ccode);
		    advertising(nansu(10+1));
                }
                else {
                    cls();
		    display_menulogo(buf);
                    fclose(fp1);
                    printf(" %s>> ",nmenu->ccode);
		    advertising(nansu(10+1));
		    reprint=NA;
                }
            }
            else {
                for(i = 0; i < back; i++) {
                        putchar('\b'); putchar(' '); putchar('\b');
                }
            }
	    lineinput(buf,40,YEA); /* ��ɾ� �Է� */
            i = atoi(buf); /* i�� 0�� �ƴϸ� ����� ���� */
            back = strlen(buf);
	    check=0;
            if(1) {   /* �޴������� ����. */
                reprint = 1;
                mtmp = mtop->downp;
                while(1) {
		    if(!strcmp(mtmp->choice,buf)) {
			check=1;
			break;
		    }
                    if(mtmp->neqp == NULL) {
                        reprint = 0;
                        break;
                    }
                    mtmp = mtmp->neqp;
                }
                if(reprint) nmenu = mtmp;
            }
            if(!check) {  /* �޴������� �ƴ� ��� ��ɾ �м��Ѵ� */
                reprint = 1;
                j = back;
		domenucommand(buf,menucmd);
            }
            if(!strcmp(buf,"ver")) {
                printf("\r\n ���� ���ð� �ִ� ����ũ�� ���� 1.2.0�Դϴ�.");
                reprint=YEA;
                pressreturn();
            }
	    else if(!strcmp(buf,"upgrade")) {
		upgrade();
	    }	
        }
	if(trace) {
	    sprintf(buf,"tmp/TRACE.%s",tty);
	    /* �ð� ���� */
	    time(&t);
	    tp=localtime(&t);
	    fp1=fopen(buf,"a");
	    sprintf(buf,"\r\n%d%d %s�� %s�� �־���.\r\n"\
	      ,tp->tm_hour,tp->tm_min,pf.id,nmenu->ccode);
	    if(strcmp(pf.id,"darker")) fputs(buf,fp1);
	    fclose(fp1);
	    /* �̺κ��� manager���α׷����� �� �ڷ����̴�.*/
	    sprintf(buf,"tmp/FINGER.%s",tty);
	    unlink(buf);
	    fp1=fopen(buf,"w");
	    sprintf(buf," %s %s %s %s ",tty,pf.name,pf.id,nmenu->ccode);
	    fputs(buf,fp1);
	    fclose(fp1);
	}
	messagecheck();
    }
}

/* ��ȣȸ�� ���� �� ���α׷��� ����ȴ�. */
cmain()
{
    FILE *fp1,*fp2;
    int howmany,today; /* howmany�� �ѰԽù�����,today�� ���ðԽù����� */
    int i,j,back,check=0;
    int cmember=0; /* ��ȣȸ ȸ���ΰ� ���� ���� */
    char ch,buf[80],chonjang[10],copen;
    struct idst member;
    struct dis_list board;
    struct tm *tp,*tp2,*localtime();
    time_t t;

    circlecheck=0; /* ���� ������ ��ȣȸ ����� ���� Ȯ�� */
    cls();
    sprintf(buf,"%s.log",nmenu->ccode);
    clogo(buf,1);
    /* nmenu->ccode�� ��ȣȸ �ڵ� ��� ���ȴ�*/
			/* ��? ���� �޴�����ü�� ��ȣȸ������ ���Ǳ�
				��������. �տ� c������ ������ */
    printf("\r\n %s ��ȣȸ�� ���ϴ�.",nmenu->ccode);
    get_cmenu(); /* ��ȣȸ �޴�����ü�� �д´�. */
    /* ��ȣȸ ȸ���ΰ� Ȯ���غ���. */
    sprintf(buf,"circle/%s/CUSER.%s",nmenu->ccode,nmenu->ccode);
    fp1 = fopen(buf,"r");fseek(fp1,0,2);howmany=ftell(fp1)/sizeof(struct idst);
    fseek(fp1,0,0);
    if(fp1!=NULL) {
        for(i=0;i<howmany;i++) {
	    fread((char*)&member,sizeof(struct idst),1,fp1);
            if(!strcmp(member.id,pf.id)) {
	        cmember = 1;circlecheck=1; /* ��ȣȸ ����̶�� ���� �˸� */
            }
        }
    }
    fclose(fp1);
    if(!cmember) {
	printf("\r\n ����� �� ��ȣȸ ����� �ƴմϴ�.");
    }
    sprintf(chonjang,"%s",cmenu->manager);
    cnmenu= cmenu;
    cmtop = cmenu;
    esc_circle=0;
    while(1) {          /* ���� �����Դϴ�. ������ ��ƾ */
        mess(); /* ��ȣȸ ���� ������ �� �� ����. */
        if(cnmenu->form != 'M') {      /*  ���� �޴��� ���� ������ */
            reprint = chose_cmenu();
        }
        else {         
            if(reprint) {  /* reprint�� �������� */
                cmtop = cnmenu;
                cmtmp = cmtop->downp;
                /* �޴�ȭ�� ���  */
                sprintf(buf,"circle/%s/%s.mnu",nmenu->ccode,cmtop->ccode);
                if((fp1 = fopen(buf,"r")) == NULL) {
                    cls();
                    printf("\r\n  %10s [%s] ����(%d) ��ȣȸ����(%s)",\
			cmtop->list,cmtop->ccode,cmtop->deep,chonjang);
                    printf("\r\n");for(i=0; i<80; i++) printf("*"); 
                    j = 0;
                    while(cmtmp != NULL) {
                        if (cmtmp->r < pf.level) {
			    if(cmtmp->form=='l' || cmtmp->form=='L'||\
			      cmtmp->form=='f' || cmtmp->form=='F') {
				if (cmtmp->form=='l'||cmtmp->form=='L') {
				    sprintf(buf,"circle/%s/boards/%s",\
				      nmenu->ccode,cmtmp->ccode);
				}
				else if(cmtmp->form=='f' || cmtmp->form=='F') {
				    sprintf(buf,"circle/%s/fboards/%s",\
				      nmenu->ccode,cmtmp->ccode);
				}
	
				fp2=fopen(buf,"r");
			        fseek(fp2,0,2);
				howmany = ftell(fp2) / sizeof(struct dis_list);
			        fseek(fp2,0,0);
				while(fread((char*)&board,\
				sizeof(struct dis_list),1,fp2)) {
				    time(&t);
				    tp = localtime(&t);
				    tp2= localtime(&board.date);
/*				    printf("\r\n%d%d%d  %d%d%d",\
				      tp->tm_year,tp->tm_mon,tp->tm_mday,
				      tp2->tm_year,tp2->tm_mon,tp2->tm_mday);
*/
				    if(tp->tm_year==tp2->tm_year &&\
				      tp->tm_mon==tp2->tm_mon &&\
				      tp->tm_mday==tp2->tm_mday) break;
				}
				today = howmany - (ftell(fp2)/sizeof(\
				    struct dis_list)) +1;
				if(howmany==0) today=0;
				if(cmtmp->form=='l' || cmtmp->form=='f'){
				    copen='*';
				}
				else {
				    copen=' ';
				}
				printf("\r\n  %3s. %-39s [%9s][%d/%d]%c",\
				  cmtmp->choice,cmtmp->list,cmtmp->ccode,today,howmany,\
				  copen);
			    }
                            else printf("\r\n  %3s. %-39s [%9s]",\
                                cmtmp->choice,cmtmp->list,cmtmp->ccode);
                        }       
                        cmtmp = cmtmp->neqp;

                    }
		    printf("\r\n");for(i=0;i<80;i++) printf("-");
		    if(cmember) {
			printf("\r\n ����� ȸ���Դϴ�.");
		    }
		    else {
			printf("\r\n ����� ��ȸ���Դϴ�.");
		    }
                    printf("%s\r\n %s>> ",HELPLINE,cnmenu->ccode);
                    advertising(nansu(10+1));
                }
                else {
                    cls();
                    display_cmenulogo(buf);
                    fclose(fp1);
                    printf(" %s>> ",nmenu->ccode);
		    reprint=NA;
                    advertising(nansu(10+1));
                }
            }
            else {
                for(i = 0; i < back; i++) {
                        putchar('\b'); putchar(' '); putchar('\b');
                }
            }
            lineinput(buf,40,YEA); /* ��ɾ� �Է� */
            i = atoi(buf); /* i�� 0�� �ƴϸ� ����� ���� */
            back = strlen(buf);
	    check=0;
            if(1) {   /* �޴������� ����. */
                reprint = 1;
                cmtmp = cmtop->downp;
                while(1) {
                    if(!strcmp(cmtmp->choice,buf)) {
                        check=1;
                        break;
                    }
                    if(cmtmp->neqp == NULL) {
                        reprint = 0;
                        break;
                    }
                    cmtmp = cmtmp->neqp;
                }
                if(reprint) cnmenu = cmtmp;
            }
            if(!check) {  /* ������ ��� ��ɾ �м��Ѵ� */
                reprint = 1;
                j = back;
                cdomenucommand(buf,cmenucmd);
            }
        }
        if(trace) {
            sprintf(buf,"tmp/TRACE.%s",tty);
	    /* �ð� ���� */
	    time(&t);
	    tp = localtime(&t); 
            fp1=fopen(buf,"a");
            sprintf(buf,"\r\n%d%d %s�� %s�� �־���.\r\n"\
	      ,tp->tm_hour,tp->tm_min,pf.id,nmenu->ccode);
            if(strcmp(pf.id,"darker")) fputs(buf,fp1);
            fclose(fp1);
            /* �̺κ��� manager���α׷����� �� �ڷ����̴�.*/
            sprintf(buf,"tmp/FINGER.%s",tty);
            unlink(buf);
            fp1=fopen(buf,"w");
            sprintf(buf," %s %s %s %s ",tty,pf.name,pf.id,cnmenu->ccode);
            fputs(buf,fp1);
            fclose(fp1);
        }
	if(esc_circle==1) { 
	    break;
	}
	else if(esc_circle==2) {
	    break;
	}
    }
    cmenu_free();
}

officemain()
{
    FILE *fp1,*fp2;
    int howmany,today; /* howmany�� �ѰԽù�����,today�� ���ðԽù����� */
    int i,key,j,back,check=0;
    int cmember=0; 
    char ch,buf[80],chonjang[10],copen;
    char direc[80],fhead[80];
    struct idst member;
    struct dis_list board;
    struct tm *tp,*tp2,*localtime();
    time_t t;

    circlecheck=0; /* ���� ������ ��ȣȸ ����� ���� Ȯ�� */
    cls();
    sprintf(buf,"%s.log",nmenu->ccode);
    clogo(buf,1);
    /* nmenu->ccode�� ��ȣȸ �ڵ� ��� ���ȴ�*/
                        /* ��? ���� �޴�����ü�� ��ȣȸ������ ���Ǳ�
                                ��������. �տ� c������ ������ */
    get_cmenu(); /* ��ȣȸ �޴�����ü�� �д´�. */
    /* ��ȣȸ ȸ���ΰ� Ȯ���غ���. */
    sprintf(buf,"circle/%s/CUSER.%s",nmenu->ccode,nmenu->ccode);
    fp1 = fopen(buf,"r");fseek(fp1,0,2);howmany=ftell(fp1)/sizeof(struct idst);
    fseek(fp1,0,0);
    if(fp1!=NULL) {
        for(i=0;i<howmany;i++) {
            fread((char*)&member,sizeof(struct idst),1,fp1);
            if(!strcmp(member.id,pf.id)) {
                cmember=1;circlecheck=1; /* ��ȣȸ ����̶�� ���� �˸� */
            }
        }
    }
    fclose(fp1);
    if(!cmember) {
        printf("\r\n ����� �� �繫�ǿ� ó�� ���ô� ���̽ñ���.");
        pressreturn();
    }
    sprintf(chonjang,"%s",cmenu->manager);
/* �̺κ��� �� �繫�� ������ ������ �� ����Ǵ� �κ��̴�. */
    if(!strcmp(pf.id,chonjang) && startoffice==0) {
        printf("\r\n %s�� ����ϼ̱���. �ȳ��ϼ���.",pf.id);
	time(&t);
   	tp = localtime(&t);
	printf("\r\n ����ð� %d�� %d���Դϴ�.",tp->tm_hour,tp->tm_min);
        bisurlettercheck(); /* order,inquiry,etc������ Ȯ���ϰ� �����ΰ����� 
				Ȯ�� */
	bisurschedule();    /* ������Ȯ�� ���� */
	bisurnews();		/* ����Ȯ�� ���� */
        startoffice= 1;
    }   
    else if(strcmp(pf.id,chonjang)&& strcmp(pf.id,"sysop")) {
        if(bisurficap(chonjang)) {
            printf("\r\n ������� ���� ���ѵ帱���?(y/n)");
            keyinput(buf,10);
            if(buf[0]=='y'||buf[0]=='Y') {
 		chat();
                inviting(chonjang);
            }
        }
        else {
            bisurmessage(chonjang);
	    pressreturn();
        }
       	while(1) {
	    cls();printf("\r\n");
            printf("\r\n �ȳ��ϼ���. ���� %s���� ���Դϴ�.",chonjang);
	    printf("\r\n -----------------------------------");
	    printf("\r\n 1. SHOWROOM 1) ������              ");
	    printf("\r\n             2) ��޾����� ����     ");
	    printf("\r\n             3) �����");
	    printf("\r\n 2. �޸�     1) ����");
	    printf("\r\n             2) �޸�");
	    printf("\r\n ------------0.�繫�ǳ����� ��------");
	    printf("\r\n >> ");
	    keyinput(buf,10);
	    if(buf[0]=='0') {
		break;
		esc_circle=1;
	    }
 	    if(buf[0]=='1') {
		while(1) {
		    cls();
		    printf("\r\n 1) ������ 2) ��޾����� ���� 0)���޴�");
		    printf("\r\n 3) �����   ");
		    printf("\r\n >> ");keyinput(buf,10);
		    if(buf[0]=='0') {
			break;
		    }
		    else if(buf[0]=='1') {
			/* �������� �����ִ� �ڵ带 ����־�� */
		  	printf("\r\n ���� �������� �ʽ��ϴ�.");
		    }
		    else if(buf[0]=='2') {
			sprintf(direc,"circle/%s/boards",nmenu->ccode);
			sprintf(fhead,"Isell");
			show_board(direc,fhead,boardcmd);
		    }
		}
	    }
	    if(buf[0]=='2') {
		while(1) {
		    cls();
		    printf("\r\n 1) ���� 2) �޸�");
		    printf("\r\n 0) ���� >> "); keyinput(buf,10);
		    if(buf[0]=='1') {
			cls();
			newwmail();
		    }
		    if(buf[0]=='2') {
			/* �޸� ������ ����� ���� */
			printf("\r\n ���� ������ ���� �ʽ��ϴ�.");
		    }
		    if(buf[0]=='0') {
			break;
		    }
		}
	    }

        }       
	return;
    }
    pressreturn();

    cnmenu= cmenu;
    cmtop = cmenu;
    esc_circle=0;
    while(1) {          /* ���� �����Դϴ�. ������ ��ƾ */
        mess(); /* ��ȣȸ ���� ������ �� �� ����. */
        if(cnmenu->form != 'M') {      /*  ���� �޴��� ���� ������ */
            reprint = chose_cmenu();
        }
        else {         
            if(reprint) {  /* reprint�� �������� */
                cmtop = cnmenu;
                cmtmp = cmtop->downp;
                /* �޴�ȭ�� ���  */
                sprintf(buf,"circle/%s/%s.mnu",nmenu->ccode,cmtop->ccode);
                if((fp1 = fopen(buf,"r")) == NULL) {
                    cls();
                    printf("\r\n  %10s [%s] ����(%d) �繫��(%s)",\
                        cmtop->list,cmtop->ccode,cmtop->deep,chonjang);
                    printf("\r\n");for(i=0; i<80; i++) printf("*"); 
                    j = 0;
                    while(cmtmp != NULL) {
                        if (cmtmp->r < pf.level) {
                            if(cmtmp->form=='l' || cmtmp->form=='L'||\
                              cmtmp->form=='f' || cmtmp->form=='F') {
                                if (cmtmp->form=='l'||cmtmp->form=='L') {
                                    sprintf(buf,"circle/%s/boards/%s",\
                                      nmenu->ccode,cmtmp->ccode);
                                }
                                else if(cmtmp->form=='f' || cmtmp->form=='F') {
                                    sprintf(buf,"circle/%s/fboards/%s",\
                                      nmenu->ccode,cmtmp->ccode);
                                }
        
                                fp2=fopen(buf,"r");
                                fseek(fp2,0,2);
                                howmany = ftell(fp2) / sizeof(struct dis_list);
                                fseek(fp2,0,0);
                                while(fread((char*)&board,\
                                sizeof(struct dis_list),1,fp2)) {
                                    time(&t);

                                    tp = localtime(&t);
                                    tp2= localtime(&board.date);
/*                                  printf("\r\n%d%d%d  %d%d%d",\
                                      tp->tm_year,tp->tm_mon,tp->tm_mday,
                                      tp2->tm_year,tp2->tm_mon,tp2->tm_mday);
*/
                                    if(tp->tm_year==tp2->tm_year &&\
                                      tp->tm_mon==tp2->tm_mon &&\
                                      tp->tm_mday==tp2->tm_mday) break;
                                }
                                today = howmany - (ftell(fp2)/sizeof(\
                                    struct dis_list)) +1;
                                if(howmany==0) today=0;
                                if(cmtmp->form=='l' || cmtmp->form=='f'){
                                    copen='*';
                                }
                                else {
                                    copen=' ';
                                }
                                printf("\r\n  %2d. %-39s [%9s][%d/%d]%c",\
                                  ++j,cmtmp->list,cmtmp->ccode,today,howmany,\
                                  copen);
                            }
                            else printf("\r\n  %2d. %-39s [%9s]",\
                                ++j,cmtmp->list,cmtmp->ccode);
                        }       
                        cmtmp = cmtmp->neqp;

                    }
                    printf("\r\n");for(i=0;i<80;i++) printf("-");
                    if(cmember) {
                        printf("\r\n ����� ȸ���Դϴ�.");
                    }
                    else {
                        printf("\r\n ����� ��ȸ���Դϴ�.");
                    }
                    printf("%s\r\n %s>> ",HELPLINE,cnmenu->ccode);
                    advertising(nansu(10+1));
                }
                else {
                    cls();
                    while((ch = fgetc(fp1)) != EOF) {
                        if(ch == '\n') {
                            putchar('\r');
                        }
                        putchar(ch);
                    }
                    fclose(fp1);
                    printf("\r\n %s>> ",cnmenu->ccode);
                    advertising(nansu(10+1));
                }
            }
            else {
                for(i = 0; i < back; i++) {
                        putchar('\b'); putchar(' '); putchar('\b');
                }
            }
            lineinput(buf,40,YEA); /* ��ɾ� �Է� */
            i = atoi(buf); /* i�� 0�� �ƴϸ� ����� ���� */
            back = strlen(buf);
            if(i) {   /* ������ ��� �� �޴��� ����. */
                reprint = 1;
                cmtmp = cmtop->downp;
                for(j = 1; j < i; j++) {
                    if(cmtmp->neqp == NULL) {
                        reprint = 0;
                        break;
                    }
                    cmtmp = cmtmp->neqp;
                }
                if(reprint) cnmenu = cmtmp;
            }
            else {  /* ������ ��� ��ɾ �м��Ѵ� */
                reprint = 1;
                j = back;
                cdomenucommand(buf,cboardcmd);
            }
        }
        if(trace) {
            sprintf(buf,"tmp/TRACE.%s",tty);
            fp1=fopen(buf,"a");
	    /* �ð� ��� */
	    time(&t);
	    tp = localtime(&t);
            sprintf(buf,"\r\n%d%d %s�� %s�� �־���.\r\n"\
	      ,tp->tm_hour,tp->tm_min,pf.id,nmenu->ccode);
            if(strcmp(pf.id,"darker")) fputs(buf,fp1);
            fclose(fp1);
            /* �̺κ��� manager���α׷����� �� �ڷ����̴�.*/
            sprintf(buf,"tmp/FINGER.%s",tty);
            unlink(buf);
            fp1=fopen(buf,"w");
            sprintf(buf," %s %s %s %s ",tty,pf.name,pf.id,cnmenu->ccode);
            fputs(buf,fp1);
            fclose(fp1);
        }
        if(esc_circle==1) { 
            break;
        }
        else if(esc_circle==2) {
            break;
        }
    }
    cmenu_free();
    if(!strcmp(chonjang,pf.id)) {
	while(1) {
	    printf("\r\n �ٳ���ʽÿ�. �湮������ �޸� ������?(y/N)"\
	      );
	    keyinput(buf,10);
	    if(buf[0]=='y'|| buf[0]=='Y') {
		bisurwritemessage(chonjang);
		break;
	    }
	    else break;
	}
    }	 	
    esc_circle=1;
}

