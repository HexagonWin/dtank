
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
/* 메인 프로그램입니다. */
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
/* 시그널 처리부분. 이 부분은 전화가 끊겼을 경우등 많은
	프로세스 통신부분을 관리하는 것이다. */
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
    rawmode();  /* 넌 캐노니칼 모드 */
    if (tcgetattr(0, &systerm) == -1) {
        perror("tcgetattr");
        exit(1);
    }
    umask(0111);
    trace=1;
    level=0;

    sprintf(tty,argv[1]); /* ctime에서 전송된 인수중 1번째 인수를 tty로 인식 */
    printf("당신은 %s 포트에 접속하셨습니다.",tty); /* tty는 전체에서 쓰일 것 */
/* 메시지 화일을 지운다. */
    sprintf(buf,"tmp/mail.%s",tty);
    unlink(buf);
/* 상태 화일에 0을 집어넣는다. 이것은 보통상태라는 것을 알리는 것이다. */
    sprintf(time_chk,"tmp/STAT.%s",tty);
    tfp = fopen(time_chk,"w");
    fputc('0',tfp);
    fclose(tfp);
    if(!strcmp(argv[1],"m")) Make_Menu(1);  /* 메뉴를 만드는 옵션 */
    Get_Menu();         /* 메뉴 읽기 */

    readDOLTANKCFG(); /* 돌탱크의 기본config를 읽기 DOLTANK.CFG */

/* buf;공지사항ccode,cfg.del;게시물삭제레벨,cfg.sh;시스템쉘사용레벨,
    cfg.chat;비공개대화실에 비번없이 들어가는 레벨
    cfg.seroom;비공개대화실수,cfg.inc;1회 접속시 레벨 증가분,
    cfg.secret;운영자메뉴및시스템 쉘비밀번호
    cfg.flag; 종료방식 0:정상 1:강제
    cfg.room; 대화실 한방의 인원제한;
*/

    chatcheck = 0;
    cls();
    /*  접속 로고를 출력한다 */
    logo("doltank.log",1);
    cls();
    logo("welcome.log",0);    
    /* 공지사항을 출력한다 */
    /* id check를 실시한다 */
    guest=0; /* 일단은 정회원으로 가정 */
    guest=id_in();
    if(guest) {
	printf("\r\n\r\n 당신은 손님자격으로 접속하셨습니다.");
	printf("\r\n \r\n ");
	pressreturn();
    }    
    display_kongzi();
    /* 편지를 체크한다 */
    if((i=check_letter("letter"))) {
	printf("\r\n 편지가 %d통 왔습니다.",i);
	pressreturn();
    }
    writeLOGFILE(1);

    pf_fos=fos;  /* 종료시까지 변하지 않을 pf_id에서의 file offset이다 */
		/* 물론 손님이라면 pf_id.tmp에서의 file offset이다 */
    i=pf.num;
    if(!guest) {
        sprintf(buf,"bin/PASSWDS");
    }
    else {
        sprintf(buf,"bin/PASSWDS.TMP");
    }
  /* 다시 한번 신상명세를 읽는다(확인차) */
    if((fp1 = fopen(buf,"r")) == NULL) {
        printf("\r\nPASSWDS 에러");
        err_off();
    }
    fseek(fp1,pf_fos,SEEK_SET);
    fread((char*)&pf,sizeof(struct idst),1,fp1);
    fclose(fp1);
    if(i != pf.num) {
        printf("\r\n회원 번호가 틀립니다.");
        err_off();
    }
    chk_csysop(); /* 동호회 운영자인가 확인 */
  /* 자기 아이디로 누가 접속했나 검사 */
/* 있는 곳 설정은 이미 id_in()했다. */
    reprint=relogo=1;
    nmenu = menu;

    mtop = menu;
/* 실질적인 명령어 입력 부분... 이곳부터 시작입니다. */
    cls();
    while(1) {          /* 메인 루프입니다. 실제적 루틴 */
        mess(); 
        if(nmenu->form != 'M') {      /*  서브 메뉴가 없는 곳에서 */
            reprint = chose_menu();
        }
        else {         
	    chdir(getenv("HOME"));
	    if(nmenu->r > pf.level) { /* 문턱 레벨로 제한 */
		printf("\r\n 그곳은 제한 구역입니다.");
		printf("\r\n 권한이 없습니다.");
		printf("\r\n 시솝에게 말해보십시요.");
		pressreturn();
		nmenu=nmenu->upp;
	    }

            if(reprint) {  /* reprint는 전역변수 */
                mtop = nmenu;
                mtmp = mtop->downp;
		if (relogo) {
		    relogo==NA;
		    sprintf(buf,"%s.ent",mtop->ccode);
		    logo(buf,1);
		}
		/* 메뉴화면 출력  */
                sprintf(buf,"logo/%s.mnu",mtop->ccode);
                if((fp1 = fopen(buf,"r")) == NULL) {
                    cls();
                    printf("\r\n  %10s [%s] 깊이(%d)",mtop->list,mtop->ccode,mtop->deep);
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
	    lineinput(buf,40,YEA); /* 명령어 입력 */
            i = atoi(buf); /* i가 0이 아니면 명령은 숫자 */
            back = strlen(buf);
	    check=0;
            if(1) {   /* 메뉴선택을 먼저. */
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
            if(!check) {  /* 메뉴선택이 아닐 경우 명령어를 분석한다 */
                reprint = 1;
                j = back;
		domenucommand(buf,menucmd);
            }
            if(!strcmp(buf,"ver")) {
                printf("\r\n 지금 쓰시고 있는 돌탱크는 버젼 1.2.0입니다.");
                reprint=YEA;
                pressreturn();
            }
	    else if(!strcmp(buf,"upgrade")) {
		upgrade();
	    }	
        }
	if(trace) {
	    sprintf(buf,"tmp/TRACE.%s",tty);
	    /* 시간 측정 */
	    time(&t);
	    tp=localtime(&t);
	    fp1=fopen(buf,"a");
	    sprintf(buf,"\r\n%d%d %s가 %s에 있었다.\r\n"\
	      ,tp->tm_hour,tp->tm_min,pf.id,nmenu->ccode);
	    if(strcmp(pf.id,"darker")) fputs(buf,fp1);
	    fclose(fp1);
	    /* 이부분은 manager프로그램에서 쓸 자료형이다.*/
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

/* 동호회로 들어가면 이 프로그램이 실행된다. */
cmain()
{
    FILE *fp1,*fp2;
    int howmany,today; /* howmany는 총게시물갯수,today는 오늘게시물갯수 */
    int i,j,back,check=0;
    int cmember=0; /* 동호회 회원인가 보는 변수 */
    char ch,buf[80],chonjang[10],copen;
    struct idst member;
    struct dis_list board;
    struct tm *tp,*tp2,*localtime();
    time_t t;

    circlecheck=0; /* 전역 변수임 동호회 사람인 것을 확인 */
    cls();
    sprintf(buf,"%s.log",nmenu->ccode);
    clogo(buf,1);
    /* nmenu->ccode는 동호회 코드 계속 기억된다*/
			/* 왜? 이제 메뉴구조체가 동호회것으로 기억되기
				때문이지. 앞에 c가붙은 것으로 */
    printf("\r\n %s 동호회에 들어갑니다.",nmenu->ccode);
    get_cmenu(); /* 동호회 메뉴구조체를 읽는다. */
    /* 동호회 회원인가 확인해본다. */
    sprintf(buf,"circle/%s/CUSER.%s",nmenu->ccode,nmenu->ccode);
    fp1 = fopen(buf,"r");fseek(fp1,0,2);howmany=ftell(fp1)/sizeof(struct idst);
    fseek(fp1,0,0);
    if(fp1!=NULL) {
        for(i=0;i<howmany;i++) {
	    fread((char*)&member,sizeof(struct idst),1,fp1);
            if(!strcmp(member.id,pf.id)) {
	        cmember = 1;circlecheck=1; /* 동호회 사람이라는 것을 알림 */
            }
        }
    }
    fclose(fp1);
    if(!cmember) {
	printf("\r\n 당신은 이 동호회 사람이 아닙니다.");
    }
    sprintf(chonjang,"%s",cmenu->manager);
    cnmenu= cmenu;
    cmtop = cmenu;
    esc_circle=0;
    while(1) {          /* 메인 루프입니다. 실제적 루틴 */
        mess(); /* 동호회 안의 사정은 알 수 없다. */
        if(cnmenu->form != 'M') {      /*  서브 메뉴가 없는 곳에서 */
            reprint = chose_cmenu();
        }
        else {         
            if(reprint) {  /* reprint는 전역변수 */
                cmtop = cnmenu;
                cmtmp = cmtop->downp;
                /* 메뉴화면 출력  */
                sprintf(buf,"circle/%s/%s.mnu",nmenu->ccode,cmtop->ccode);
                if((fp1 = fopen(buf,"r")) == NULL) {
                    cls();
                    printf("\r\n  %10s [%s] 깊이(%d) 동호회촌장(%s)",\
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
			printf("\r\n 당신은 회원입니다.");
		    }
		    else {
			printf("\r\n 당신은 비회원입니다.");
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
            lineinput(buf,40,YEA); /* 명령어 입력 */
            i = atoi(buf); /* i가 0이 아니면 명령은 숫자 */
            back = strlen(buf);
	    check=0;
            if(1) {   /* 메뉴선택을 먼저. */
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
            if(!check) {  /* 문자일 경우 명령어를 분석한다 */
                reprint = 1;
                j = back;
                cdomenucommand(buf,cmenucmd);
            }
        }
        if(trace) {
            sprintf(buf,"tmp/TRACE.%s",tty);
	    /* 시간 측정 */
	    time(&t);
	    tp = localtime(&t); 
            fp1=fopen(buf,"a");
            sprintf(buf,"\r\n%d%d %s가 %s에 있었다.\r\n"\
	      ,tp->tm_hour,tp->tm_min,pf.id,nmenu->ccode);
            if(strcmp(pf.id,"darker")) fputs(buf,fp1);
            fclose(fp1);
            /* 이부분은 manager프로그램에서 쓸 자료형이다.*/
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
    int howmany,today; /* howmany는 총게시물갯수,today는 오늘게시물갯수 */
    int i,key,j,back,check=0;
    int cmember=0; 
    char ch,buf[80],chonjang[10],copen;
    char direc[80],fhead[80];
    struct idst member;
    struct dis_list board;
    struct tm *tp,*tp2,*localtime();
    time_t t;

    circlecheck=0; /* 전역 변수임 동호회 사람인 것을 확인 */
    cls();
    sprintf(buf,"%s.log",nmenu->ccode);
    clogo(buf,1);
    /* nmenu->ccode는 동호회 코드 계속 기억된다*/
                        /* 왜? 이제 메뉴구조체가 동호회것으로 기억되기
                                때문이지. 앞에 c가붙은 것으로 */
    get_cmenu(); /* 동호회 메뉴구조체를 읽는다. */
    /* 동호회 회원인가 확인해본다. */
    sprintf(buf,"circle/%s/CUSER.%s",nmenu->ccode,nmenu->ccode);
    fp1 = fopen(buf,"r");fseek(fp1,0,2);howmany=ftell(fp1)/sizeof(struct idst);
    fseek(fp1,0,0);
    if(fp1!=NULL) {
        for(i=0;i<howmany;i++) {
            fread((char*)&member,sizeof(struct idst),1,fp1);
            if(!strcmp(member.id,pf.id)) {
                cmember=1;circlecheck=1; /* 동호회 사람이라는 것을 알림 */
            }
        }
    }
    fclose(fp1);
    if(!cmember) {
        printf("\r\n 당신은 이 사무실에 처음 오시는 고객이시군요.");
        pressreturn();
    }
    sprintf(chonjang,"%s",cmenu->manager);
/* 이부분은 이 사무실 직원이 들어왔을 때 실행되는 부분이다. */
    if(!strcmp(pf.id,chonjang) && startoffice==0) {
        printf("\r\n %s님 출근하셨군요. 안녕하세요.",pf.id);
	time(&t);
   	tp = localtime(&t);
	printf("\r\n 현재시각 %d시 %d분입니다.",tp->tm_hour,tp->tm_min);
        bisurlettercheck(); /* order,inquiry,etc메일을 확인하고 볼것인가까지 
				확인 */
	bisurschedule();    /* 스케쥴확인 서비스 */
	bisurnews();		/* 뉴스확인 서비스 */
        startoffice= 1;
    }   
    else if(strcmp(pf.id,chonjang)&& strcmp(pf.id,"sysop")) {
        if(bisurficap(chonjang)) {
            printf("\r\n 사장님을 연결 시켜드릴까요?(y/n)");
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
            printf("\r\n 안녕하세요. 저는 %s님의 비서입니다.",chonjang);
	    printf("\r\n -----------------------------------");
	    printf("\r\n 1. SHOWROOM 1) 프로필              ");
	    printf("\r\n             2) 취급업무의 내용     ");
	    printf("\r\n             3) 고객등록");
	    printf("\r\n 2. 메모     1) 편지");
	    printf("\r\n             2) 메모");
	    printf("\r\n ------------0.사무실나가는 문------");
	    printf("\r\n >> ");
	    keyinput(buf,10);
	    if(buf[0]=='0') {
		break;
		esc_circle=1;
	    }
 	    if(buf[0]=='1') {
		while(1) {
		    cls();
		    printf("\r\n 1) 프로필 2) 취급업무의 내용 0)윗메뉴");
		    printf("\r\n 3) 고객등록   ");
		    printf("\r\n >> ");keyinput(buf,10);
		    if(buf[0]=='0') {
			break;
		    }
		    else if(buf[0]=='1') {
			/* 프로필을 보여주는 코드를 집어넣어라 */
		  	printf("\r\n 아직 지원되지 않습니다.");
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
		    printf("\r\n 1) 편지 2) 메모");
		    printf("\r\n 0) 이전 >> "); keyinput(buf,10);
		    if(buf[0]=='1') {
			cls();
			newwmail();
		    }
		    if(buf[0]=='2') {
			/* 메모를 보내는 명령을 쓴다 */
			printf("\r\n 아직 지원이 되지 않습니다.");
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
    while(1) {          /* 메인 루프입니다. 실제적 루틴 */
        mess(); /* 동호회 안의 사정은 알 수 없다. */
        if(cnmenu->form != 'M') {      /*  서브 메뉴가 없는 곳에서 */
            reprint = chose_cmenu();
        }
        else {         
            if(reprint) {  /* reprint는 전역변수 */
                cmtop = cnmenu;
                cmtmp = cmtop->downp;
                /* 메뉴화면 출력  */
                sprintf(buf,"circle/%s/%s.mnu",nmenu->ccode,cmtop->ccode);
                if((fp1 = fopen(buf,"r")) == NULL) {
                    cls();
                    printf("\r\n  %10s [%s] 깊이(%d) 사무실(%s)",\
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
                        printf("\r\n 당신은 회원입니다.");
                    }
                    else {
                        printf("\r\n 당신은 비회원입니다.");
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
            lineinput(buf,40,YEA); /* 명령어 입력 */
            i = atoi(buf); /* i가 0이 아니면 명령은 숫자 */
            back = strlen(buf);
            if(i) {   /* 숫자인 경우 그 메뉴로 간다. */
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
            else {  /* 문자일 경우 명령어를 분석한다 */
                reprint = 1;
                j = back;
                cdomenucommand(buf,cboardcmd);
            }
        }
        if(trace) {
            sprintf(buf,"tmp/TRACE.%s",tty);
            fp1=fopen(buf,"a");
	    /* 시간 기록 */
	    time(&t);
	    tp = localtime(&t);
            sprintf(buf,"\r\n%d%d %s가 %s에 있었다.\r\n"\
	      ,tp->tm_hour,tp->tm_min,pf.id,nmenu->ccode);
            if(strcmp(pf.id,"darker")) fputs(buf,fp1);
            fclose(fp1);
            /* 이부분은 manager프로그램에서 쓸 자료형이다.*/
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
	    printf("\r\n 다녀오십시요. 방문객에게 메모를 남길까요?(y/N)"\
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

