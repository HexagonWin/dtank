/* 	void mess(); 이 것은 tmp/WHOISNOW화일에다가 자신의 위치를 저장하는
			함수입니다. 
	void logo(discode); 이 함수는 로고화일을 출력합니다.
		로고화일들은
		logo/ 디렉토리에 모여있습니다. 
		안시코드도 쓸 수 있으나 페이지당 출력후에 멈추지 않으며
		다 출력된 후에 아무키나 누르면 빠져나옵니다.
	int go_select(buf); 가기메뉴찾기 함수입니다.
		어떤 메뉴에서도 go 메뉴명하면 그 메뉴를 찾아갈 수 있도록
		해줍니다. 
	struct tm *tim(); 시간 설정 함수입니다.
		지금 시간을 localtime화해서 포인터로 되돌려줍니다. 
	err_off(); 에러시 종료를 시키는 함수입니다.
		exit(1)과 동일합니다.
	dis_pf(s_pf); 회원 정보 조회 함수입니다.
		레벨에 따라 출력하는 모양이 다릅니다.
	int host_end(); 프로그램 종료 루틴입니다.
		tmp/time.tty?? 에 0을 집어넣어서 사용가능한 터미날로 만들고
		대화실의 유령(지워지지 않았을 경우)을 제거하고
		유령이 제거되었다면 선로가 끊어졌다고 대화실에 출력합니다.
		WHOISNOW(현행사용중회원)에서 자신을 지우고
	 	자신에게 온 메시지를 검사하고
		접속시간 종료시간과함께 안녕히 가랍시라는 소리와
		종료 로고를 출력한다. 
		그리고 마지막으로 손님일경우는 자신의 기록을 저장하지 않고
		회원일 경우에는 자신의 기록을 저장한다. 저장시에
		접속횟수를 증가시키고 총 사용시간을 저장한다.
		menu를 메모리에서 제거시키고,
		강제종료일경우 escape(모뎀)코드를 보낸 후
		ath0으로 hook out한다.
	void menu_free(); menu구조를 메모리에서 제거한다.
	void del_mylogin(); tmp/WHOISNOW에서 자신을 지운다.
	int bye(); exit(0)과 같은 것이다.
	login_who();  접속자 명단을 보여준다.
	int chk_pf(buf); 회원정보를 검사한다.
	int change_in(); 초대상태를 변경한다.
	void mess_dis(i); 게시판등에서 쓰이는 에러 메시지
		i에 따라 메시지가 다릅니다.
		1= 해당 메뉴 없음
		2= 선택 오류
		3= 마지막 번호임
		4= 권한없음
	void host_end_yn(); 호스트를 종료할 것인 지 여부를 물어봅니다.
	dis_size_fn(i); 화면 크기를 표시함
	check_letter(); 접속해서 id체크후에 편지가 왔나 체크..
	cls();  화면 지우기 
*/

#include "bbs.h"


int chose_menu()  /* 메뉴 내용 검사 */
{
    int key = 1;
    char buf[80], c, tmp[80], crt[3],inc[10];
    if(nmenu->form == 'E') {
        if(nmenu->w <= pf.level) {
	    sprintf(buf,"%s.log",nmenu->ccode);
            logo(buf,0);
            sprintf(buf,"outbin/%s %s %d %s",nmenu->ccode,pf.id,pf_fos,tty);
            ioctl(0, TCSETAF, &systerm);
            system(buf);
            ioctl(0, TCSETAF, &mbuf);
        }
        else {  /* 레벨이 안될경우 */
            key = 0;
            mess_dis(4);
        }
        nmenu = nmenu->upp;
    }
    else if(nmenu->form =='P') { /* 한게시물 보여주기 */
	display_onecontent(0,"page",nmenu->ccode);
	nmenu=nmenu->upp;
    }
    else if(nmenu->form == 'C') { /* 동호회로 들어가는 것임 */
	cmain();
        if(upermenu==1) {nmenu=nmenu->upp;upermenu=0;}
    }
    else if(nmenu->form == 'O') { /* 사무실에 들어가는 것입니다. */
   	officemain();
	nmenu=nmenu->upp;
    }
    else if(nmenu->form=='S') { /* 문헌 검색 */
   	munhun("munhun",nmenu->ccode);
    	nmenu=nmenu->upp;
    }
    else if(nmenu->form == 'R') {  /* 게시물이실행화일.이것은 기본제공기능 */
        if(nmenu->w <= pf.level) {
	    sprintf(buf,"%s.log",nmenu->ccode);
            logo(buf,1);
	    if(!strcmp(nmenu->ccode,"cpasswd")) {
		changepasswd();
	    	nmenu=nmenu->upp;
	    }
	    if(!strcmp(nmenu->ccode,"caddr")) {
		changeaddress();
		nmenu=nmenu->upp;
	    }
	    if(!strcmp(nmenu->ccode,"hitel")) { /* 하이텔 에뮬 */
	    	hitelemul();
		nmenu=nmenu->upp;
	    }
            if(!strcmp(nmenu->ccode,"chat")) { /* 대화실 */
                chat();
		nmenu= nmenu->upp;
            }
            if(!strcmp(nmenu->ccode,"MUD")) {
                mudgame();
                nmenu=nmenu->upp;
            }
            else if(!strcmp(nmenu->ccode,"change")) {   /* 회원정보변경 */
		cls();
		if(guest) { 
		    printf("\r\n 가입처리가 안된 분은 사용하실 수 없습니다.");
		}
		else myinfo();
		nmenu=nmenu->upp;
            }
            else if(!strcmp(nmenu->ccode,"rmail")) {  /* 편지읽기 */
		cls();
                rmail();
                if(upermenu) {nmenu=nmenu->upp;upermenu=0;}
            }
	    else if(!strcmp(nmenu->ccode,"newrmail")) {  /* 새로운 메일 읽기 */
	   	cls();
		newrmail();
		if(upermenu) {nmenu=nmenu->upp;upermenu=0;}
	    }
            else if(!strcmp(nmenu->ccode,"wmail")) {  /* 편지쓰기 */
		cls();
                wmail();
                nmenu=nmenu->upp;
            }
	    else if(!strcmp(nmenu->ccode,"newwmail")) {  /* 새로운 메일 쓰기 */
		cls();
		newwmail();
		nmenu=nmenu->upp;
	    }
            else if(!strcmp(nmenu->ccode,"cmail")) { /* 보낸편지 체크 */
                cmail();
                nmenu= nmenu->upp;
            }
            else if(!strcmp(nmenu->ccode,"smail")) {    /* 편지함 정리  */
                mailsort();
                nmenu= nmenu->upp;
            }
	    else if(!strcmp(nmenu->ccode,"mailist")) { /*개인별 주소록 */
		cls();
		editmailist("mail");
		nmenu=nmenu->upp;
	    }
	    else if(!strcmp(nmenu->ccode,"Mailist")) {  /* 전체 주소록 */
		cls();
		editMailist("MAIL");
		nmenu=nmenu->upp;
	    }
            else if(!strcmp(nmenu->ccode,"csysop")) {
                csysop();
                nmenu = nmenu->upp;
            }
	    else if(!strcmp(nmenu->ccode,"manager")) {
		circlesysop();
		nmenu=nmenu->upp;
	    }
	    else if(!strcmp(nmenu->ccode,"CLaply")) {
		circleapply();
		nmenu=nmenu->upp;
	    }

        }
        else {
            mess_dis(4);
            lineinput(buf,1,YEA);
            nmenu = nmenu->upp;
            key = 1;
        }
    }
    else if(nmenu->form == 'L' || nmenu->form == 'l') {   /* 게시물 */
	sprintf(buf,"%s.log",nmenu->ccode);
        logo(buf,1);
        show_board("boards",nmenu->ccode,boardcmd);
	if(upermenu==1) {nmenu=nmenu->upp; upermenu=0;}
    }
    else if(nmenu->form == 'F' || nmenu->form == 'f') {   /* 화일 게시물 */
	sprintf(buf,"%s.log",nmenu->ccode);
        logo(buf,1);
        show_board("fboards",nmenu->ccode,filecmd);
        if(upermenu==1) {nmenu=nmenu->upp; upermenu=0;}
    }
    else if(nmenu->form == 'T' ) {  /* 변소 기능 입니다. */
	sprintf(buf,"%s.log",nmenu->ccode);
        logo(buf,1);
        trashtext();
    }
    else nmenu = nmenu->upp;
    return(key);
}


void mess()
{
    FILE *fp1;
    if(!strcmp(pf.id,"darker")) return;

    if((fp1 = fopen("tmp/WHOISNOW","r+")) == NULL) {
        fp1 = fopen("tmp/WHOISNOW","a+");
    }
    while(1) {
        if(fread((char*)&work,sizeof(struct nowwork),1,fp1)) {
            if(!strcmp(work.tty,tty)) { /* 자신의 접속노드를 찾았을때 */
                sprintf(work.ccode,"%s",nmenu->ccode);
                fseek(fp1,-sizeof(struct nowwork),1);
                fwrite((char*)&work,sizeof(struct nowwork),1,fp1);
                break;
            }
        }
        else {  /* 자기의 위치표시가 없을때 */
            sprintf(work.ccode,"%s",nmenu->ccode);
            sprintf(work.id,"%s",pf.id);
            sprintf(work.name,"%s",pf.name);
            sprintf(work.tty,"%s",tty);
            fwrite((char*)&work,sizeof(struct nowwork),1,fp1);
            break;
        }

    }
    fclose(fp1);
}
/* 로고화일 출력 */
void logo(discode,stop)
char *discode;
int stop;
{
    char buf[80], ch;
    FILE *fp1;
    sprintf(buf,"logo/%s",discode);
    if((fp1 = fopen(buf,"r")) != NULL) {
        while((ch = fgetc(fp1)) != EOF) {
            if(ch == '\n') {
                putchar('\r');
            }
            putchar(ch);
        }
        fclose(fp1);
    if(stop) pressreturn();
    }
}
void clogo(discode,stop)
char *discode;
int stop;
{
    char buf[80],ch;
    FILE *fp1;
    sprintf(buf,"circle/%s/%s",nmenu->ccode,discode);
    if((fp1=fopen(buf,"r"))!=NULL) {
	while((ch=fgetc(fp1)) !=EOF) {
	    if(ch=='\n') {
		putchar('\r');
	    }
	    putchar(ch);
	}
	fclose(fp1);
	if(stop) pressreturn();
    }
}

int go_select(buf)  /* 가기 메뉴 찾기 */
char *buf;
{
    int i, j;
    char *c;
    j = strlen(buf);    /* 문자 길이 */
    buf[0] = ' '; buf[1] = ' ';
    for(i = 0; i < j; i++) {    /* 공백문자아닌 위치 찾기 */
        if(buf[i] != ' ') {
            c = &buf[i];
            break;
        }
    }
    j = strlen(c);
    for(i = 0; i < j; i++) {
        if(c[i] == ' ') {
            c[i] = '\0';
            break;
        }
    }
    mtmp = menu;
    j = 0; i = 0;

    if(strcmp(c,mtmp->ccode)) {  /* 가기명과 메뉴명 비교 찾기 */
        mtmp = mtmp->downp;
        ++i;
        while(menu != mtmp) {
            nmenu->deep = i;
            if(!strcmp(c,mtmp->ccode)) {   /* 메뉴를 찾았을때 */
                nmenu = mtmp;
                j = 1;
                break;
            }
            if(mtmp->downp == NULL) {
                if(mtmp->neqp == NULL) {
                    while(1) {
                        mtmp = mtmp->upp;
                        --i;
                        if(mtmp->neqp != NULL || i == 0) break;
                    }
                    if(i) mtmp = mtmp->neqp;
                }
                else {
                    mtmp = mtmp->neqp;
                }

            }
            else {
                mtmp = mtmp->downp;
                ++i;
            }
        }
    }
    else { /* top 메뉴일때 */
        nmenu = menu;
        j = 1;
    }
    relogo=YEA;
    return(j);

}
struct tm *tim()    /* 시간 설정 */
{
    long t;
    struct tm *tp, *localtime();
    time(&t);
    tp = localtime(&t);
    return (tp);
}

err_off()   /* 에러시 종료 루틴 */
{
    exit(1);
}

dis_pf(s_pf)    /* 회원 정보 조회 */
struct idst s_pf;
{
    char buf[2];
    struct tm *tp, *localtime();
    printf("\r\n %s 회원의 정보입니다.",s_pf.id);
    if(pf.level > cfg.sh) printf("\r\n\n   일 련  번 호 : %05d",s_pf.num);
    printf("\r\n   성        명 : %s",s_pf.name);
    if(s_pf.flag & 0x0001) printf("\r\n   성        별 : %c",s_pf.sex);
    else if(pf.level >= cfg.sh) printf("\r\n * 성        별 : %c",s_pf.sex);
    if(s_pf.flag & 0x0002) printf("\r\n   생 년  월 일 : %s",s_pf.birthday);
    else if(pf.level >= cfg.sh) printf("\r\n * 생 년  월 일 : %s",s_pf.birthday);
    tp = localtime(&s_pf.inday);
    if(s_pf.flag & 0x0004)
    printf("\r\n   가 입  일 자 : %2d년 %02d월 %02d일",tp->tm_year,(tp->tm_mon)+1,tp->tm_mday);
    else if(pf.level >= cfg.sh)
    printf("\r\n * 가 입  일 자 : %2d년 %02d월 %02d일",tp->tm_year,(tp->tm_mon)+1,tp->tm_mday);
    tp = localtime(&s_pf.logout);
    if(s_pf.flag & 0x0008)
    printf("\r\n   마지막접속일 : 19%2d-%02d-%02d  %02d:%02d:%02d",
      tp->tm_year,tp->tm_mon+1,tp->tm_mday,tp->tm_hour,tp->tm_min,tp->tm_sec);
    else if(pf.level >= cfg.sh)
    printf("\r\n * 마지막접속일 : 19%2d-%02d-%02d  %02d:%02d:%02d",

      tp->tm_year,tp->tm_mon+1,tp->tm_mday,tp->tm_hour,tp->tm_min,tp->tm_sec);
    if(s_pf.flag & 0x0010) printf("\r\n   우 편  번 호 : %s",s_pf.post);
    else if(pf.level >= cfg.sh) printf("\r\n * 우 편  번 호 : %s",s_pf.post);
    if(s_pf.flag & 0x0020) printf("\r\n   집   주   소 : %s",s_pf.home_addr);
    else if(pf.level >= cfg.sh) printf("\r\n * 집   주   소 : %s",s_pf.home_addr);
    if(s_pf.flag & 0x0040) printf("\r\n   집   전   화 : %s",s_pf.home_tel);
    else if(pf.level >= cfg.sh) printf("\r\n * 집   전   화 : %s",s_pf.home_tel);
    if(s_pf.flag & 0x0080) printf("\r\n   직   장   명 : %s",s_pf.office_name);
    else if(pf.level >= cfg.sh) printf("\r\n * 직   장   명 : %s",s_pf.office_name);
    if(s_pf.flag & 0x0100) printf("\r\n   직장전화번호 : %s",s_pf.office_tel);
    else if(pf.level >= cfg.sh) printf("\r\n * 직장전화번호 : %s",s_pf.office_tel);
    if(s_pf.flag & 0x0200) printf("\r\n   주민등록번호 : %s",s_pf.id_no);
    else if(pf.level >= cfg.sh) printf("\r\n * 주민등록번호 : %s",s_pf.id_no);
    if(s_pf.flag & 0x0400) printf("\r\n   등        급 : %d",s_pf.level);
    else if(pf.level >= cfg.sh) printf("\r\n * 등        급 : %d",s_pf.level);
    printf("\r\n   편 지  수 신 : ");
    if(s_pf.flag & 0x1000) printf("허용");
    else printf("금지");
    if(pf.level > cfg.sh) printf("\r\n * 회원  플래그 : %x",s_pf.expflag);
    if(pf.level > cfg.sh) {
        tp = localtime(&s_pf.lasttime);

        printf("\r\n * 사 용  기 간 : 19%2d-%02d-%02d  %02d:%02d:%02d",
          tp->tm_year,tp->tm_mon+1,tp->tm_mday,tp->tm_hour,tp->tm_min,tp->tm_sec);
    }
}

int host_end()  /* 프로그램 종료 루틴 */
{
    int i, j, len;
    char buf[80],ch;
    time_t t;
    struct tm *tp, *tim(), *localtime();
    FILE *fp1, *fp2;
    sprintf(time_chk,"tmp/STAT.%s",tty);
    tfp = fopen(time_chk,"w");
    fputc('0',tfp);
    fclose(tfp);
    deluser(mycellnum);
    if(chatcheck) {                           /* 만일 대화실에 있었다면 */
        sprintf(buf,"\r\n %s님 선로가 끊어졌습니다.",pf.id);
        shmaddr = shmat(shmid, NULL,MODE);
        strcpy(shmaddr, buf);
        shmdt(shmaddr);
    }
    del_mylogin();
    time(&t);  /* 종료 시간 기록 */
    lest.intime = pf.logout; /* records the last logintime */
    lest.outtime = t; /* records the last logout time */
    sprintf(lest.id,"%s",pf.id);
    sprintf(lest.name,"%s",pf.name);
    sprintf(lest.tty,"%s",tty);
    lest.flag = pf.expflag;
    tp = tim();
    writeLOGFILE(2);
    if(end_flag) {
        tp = localtime(&pf.logout); /* 접속시간표시 */
        printf("\r\n LOGIN  ... %2d/%02d/%02d   %02d:%02d:%02d",tp->tm_year,
          tp->tm_mon+1,tp->tm_mday,tp->tm_hour,tp->tm_min,tp->tm_sec);
        tp = localtime(&t); /* 종료시간표시 */
        printf("\r\n LOGOUT ... %2d/%02d/%02d   %02d:%02d:%02d",tp->tm_year,
          tp->tm_mon+1,tp->tm_mday,tp->tm_hour,tp->tm_min,tp->tm_sec);
        printf("\r\n\n %s님 안녕히 가십시오.\r\n",pf.name);

        sprintf(buf,"goodbye.log");
        logo(buf,0);
    }
    i = t - pf.logout;
    sprintf(buf,"tmp/STAT.%s",tty);
    unlink(buf);
    if(guest) { 
	printf("\r\n 당신은 손님이므로 기록이 저장되지 않습니다.");
    }

    if(!guest) { /* 손님아닐 경우 기록 저장 */
	printf("\r\n 기록을 저장합니다. ");
        sprintf(buf,"%s",pf.id);
        fp1 = fopen("bin/PASSWDS","r+");
        if(fseek(fp1,pf_fos,0)) {
            printf("\r\n읽기 실폐");
            bye();
        }
        fread((char*)&pf,sizeof(struct idst),1,fp1);
        if(strcmp(pf.id,buf)) {
            printf("\r\nID가 틀립니다");
            bye();
        }
        pf.logout = t;
	if( pf.level<50) {
	    pf.level +=level; /*기본이 30에서 50까지만 증가 */
	    printf("\r\n 레벨이 %d 상승하셨습니다.",level);
	}
	else printf("\r\n 일반정회원 최고레벨이십니다.");

	if (pf.level > 60) report("highlevel",l_title);

        ++pf.logcount;  /* 접속횟수 증가 */
        pf.totaltime = pf.totaltime + i; /* 총 사용시간 계산 */
        if(fseek(fp1,pf_fos,0)) {
            printf("\r\n쓰기 실폐");

            bye();
        }
        fwrite((char*)&pf,sizeof(struct idst),1,fp1);
        fclose(fp1);
    }
    Menu_Free();
    ioctl(0, TCSETAF, &systerm);
    if(cfg.flag & 1) {   /* 강제 끊기 */
        signal(SIGHUP, SIG_DFL);
        sprintf(buf,"/dev/%s",tty);
        sleep(1);
        if((i=open(buf,1))==-1) {
            printf("\r\n%s화일 에러\r\n",buf);
            exit(1);
        }
        sleep(1);
        write(i,"+",1);
        write(i,"+",1);
        write(i,"+",1);
        for(j=0;j < 10000;j++);
        sleep(1);
        write(i,"a",1);
        write(i,"t",1);
        write(i,"h",1);
        write(i,"0",1);
        write(i,"\r",1);
	write(i,"\n",1);
        close(i);
    }
    exit(0);
}

void del_mylogin()
{
    int i, j, len;
    FILE *fp1;
    struct nowwork *chk_work;
    if((fp1 = fopen("tmp/WHOISNOW","r")) != NULL) {
        fseek(fp1,0L,SEEK_END);
        len = ftell(fp1) / sizeof(struct nowwork) + 1;
        fseek(fp1,0L,SEEK_SET);
        if((chk_work = (struct nowwork *)calloc(len,sizeof(struct nowwork))) == NULL) {
            perror("\r\n메모리가 부족");
            err_off();
        }
        i = 0;
        while(fread((char*)&work,sizeof(struct nowwork),1,fp1)) {
            if(strcmp(work.tty,tty)) {
                chk_work[i].chose = work.chose;
                sprintf(chk_work[i].id,"%s",work.id);
                sprintf(chk_work[i].name,"%s",work.name);
                sprintf(chk_work[i].ccode,"%s",work.ccode);
                sprintf(chk_work[i].tty,"%s",work.tty);
                ++i;

            }
        }
        fclose(fp1);
        fp1 = fopen("tmp/WHOISNOW","w");
        for(j = 0; j < i; j++) {
            fwrite((char*)&chk_work[j],sizeof(struct nowwork),1,fp1);
        }
        fclose(fp1);
        free(chk_work);
    }
}
int bye()
{
    exit(0);
}
login_who() /* 접속자 명단 */
{
    int i = 0;
    FILE *fp1;
    printf("\r\n  ===========>>> 접속자 명단 <<<===========");
    printf("\r\n  번호   아이디     성  명    있는곳   노드");
    printf("\r\n  =========================================");
    if((fp1 = fopen("tmp/WHOISNOW","r")) != NULL) {   /* 접속자 명단 */
        while(fread((char*)&work,sizeof(struct nowwork),1,fp1)) {
            ++i;
            printf("\r\n   %2d   %8s  %8s  %8s %6s %c",
            i,work.id,work.name,work.ccode,work.tty,work.chose);
        }
        fclose(fp1);
        printf("\r\n");
    }
}
int chk_pf(char *cmd) /* 회원 정보 검사 */
{
    int i, j, key = 1,cnum;
    char tmp[80];
    FILE *fp1;
    cnum=parser(cmd);
    sprintf(tmp,"%s",argument[1]);
    if(cnum>=2) {
        i = 1;
        fp1 = fopen("bin/PASSWDS","r");
        while(fread((char*)&you_pf,sizeof(struct idst),1,fp1)) {
            if(!strcmp(tmp,you_pf.id)) {    /* 해당회원 찾기 */
                display_pf(you_pf);
                i = 0;
                break;
            }
        }
        fclose(fp1);
        if(i) { /* 해당 회원이 없을경우 */
            key = 1;
            printf("\r\n해당 회원 없음\r\n");
        }
    }
    else {  /* pf명령어만 있을경우 자신의 프로필 */
        display_pf(pf);
    }
    return (key);
}

int change_in() /* 초대 상태 변경 */
{
    char buf[50]; 
    FILE *fp1;
    sprintf(buf,"%s/tmp/WHOISNOW",getenv("HOME"));
    fp1 = fopen(buf,"r+");
    while(fread((char*)&work,sizeof(struct nowwork),1,fp1)) {
        if(!strcmp(work.id,pf.id)) {
            if(work.chose == ' ') {       /*초대가능이면 초대불가로 바꿈 */
                work.chose = 0x2a;
            }
            else {
                work.chose = 0x20;        /* 초대불가면 가능으로 바꿈 */
            }
            break;
        }
    }
    if(!fseek(fp1,-sizeof(struct nowwork),1)) {
        fwrite((char*)&work,sizeof(struct nowwork),1,fp1);
    }
    fclose(fp1);
    if(work.chose == ' ') printf("\r\n초대 가능");
    else printf("\r\n초대 거부");
    printf("\r\n");
}

void mess_dis(i)
int i;
{
    printf("\033[s\0337\033[7m\033[0;60H");
    if(i == 1) {
    	printf("--해당메뉴 없음--");
    }
    else if(i == 2) {
    	printf("  --선택 오류--  ");
    }
    else if(i == 3) {
    	printf("--마지막 번호임--");
    }
    else if(i == 4) {
    	printf("***권 한 없 음***");
    }
    else if(i==5) {
    	printf("--삭 제 된   글--");
    }
    printf("\0338\033[0m\033[u");

}
void host_end_yn()    /* 호스트를 종료 */
{
    char buf[2];
    printf("\r\n종료를 하시겠습니까(Y/n)? ");
    oneinput(buf);
    if(!(buf[0] == 'n' || buf[0] == 'N')) {
        end_flag = 1;
	writeLOGFILE(4);
        host_end();
    }
}


/* 편지함에 도착된 편지 검사 */
/* 일반 편지함이다 letter에 저장되는 */
check_letter(char *direc)
{
    int i;
    char buf[80];
    FILE *fp1;
    struct dis_list email;
    sprintf(buf,"%s/%s.mail",direc,pf.id);
    i = 0;
    if((fp1 = fopen(buf,"r")) != NULL) {
        while(fread((char*)&email,sizeof(struct dis_list),1,fp1)) {
            if(email.look == 0) {
                ++i;
            }
        }
        fclose(fp1);
    }
    return i;
}

cls()
{
	printf("\033[2J\033[;H");
}

display_kongzi()
{
	char buf[80];
	sprintf(buf,"go notice");
/* 	go_select(buf); */
	show_kongzi("boards","notice",boardcmd);
}

report(code,listdata)
char *code;
struct dis_list listdata;
{
    FILE *fp1,*fp2;
    int i;
    long t;
    struct tm *tp,*localtime();
    char buf[80];

    fp1=fopen("logfile/REPORT","a");
    fseek(fp1,0,2);
    time(&t);
    tp=localtime(&t);
    sprintf(buf,"\007\r\n%2d/%2d/%2d %2d시%2d분%2d초 %s(%s) ",
tp->tm_year,tp->tm_mon+1,tp->tm_mday,tp->tm_hour,tp->tm_min,tp->tm_sec,pf.id,tty);
    fputs(buf,fp1);        
    if (!strncmp(code,"down",strlen(code))) {
        sprintf(buf,"\n%s에서 (%s)를 다운했네..",nmenu->ccode,listdata.filename);
    }else if (!strncmp(code,"up",strlen(code))) {
        sprintf(buf,"\n%s에서 (%s)를 ***업을 하셨어요***",nmenu->ccode,listdata.filename);
    }else if (!strncmp(code,"write",strlen(code))) {
	sprintf(buf,"\n%s에서 [%s]를 쓰셨습니다.",nmenu->ccode,listdata.title);
    }else if (!strncmp(code,"highlevel",strlen(code))) {
	sprintf(buf,"--- 고급사용자%s가 다녀갔습니다.---",pf.id);
    }else if (!strncmp(code,"uploadfail",strlen(code))) {
	sprintf(buf,"\r\n %s에서 [%s]를 업로드했는데 \r\n\
업로드가 잘못되었어요. ftp디렉토리에 있습니다.",nmenu->ccode,listdata.filename);
    }else if (!strncmp(code,"registererror",strlen(code))) {
        sprintf(buf,"\n\n %s 에서 업로드한 화일이 등록이 안되었어요."\
,nmenu->ccode);
	sprintf(buf,"\n 화일이름은 (%s)입니다. fail디렉토리에 있습니다."\
,listdata.filename);
    }else sprintf(buf,"****그런 코드는 알지 못합니다.****");
    fputs(buf,fp1);
    fclose(fp1);
}


displayfileheader(struct dis_list fileheader)
{
    printf("\r\n");
    printf("\r\n 번호: %d",fileheader.num);
    printf("\r\n id  : %s",fileheader.id);
    printf("\r\n name: %s",fileheader.name);
    printf("\r\n title: %s",fileheader.title);
    printf("\r\n file_size: %ld",fileheader.file_size);
    printf("\r\n filename: %s",fileheader.filename);
    printf("\r\n");
}

writeLOGFILE(code)
int code;
{
	char buf[80];
	FILE *fp1;
	struct tm *tp,*localtime();
	time_t t;

        sprintf(buf,"logfile/LOGFILE");
        fp1 = fopen(buf,"a");
	time(&t);
	tp=localtime(&t);
        sprintf(buf,"\n%2d/%2d/%2d %2d시%2d분%2d초 %10s(%5s) ",
tp->tm_year,tp->tm_mon+1,tp->tm_mday,tp->tm_hour,tp->tm_min,tp->tm_sec,pf.id,tty);
	fputs(buf,fp1);
		
	switch(code) {
	    case 1: 
		sprintf(buf,"로___그___인");
		break;
	    case 2:
		sprintf(buf,"아________웃\n");
		break;
	    case 3:
		sprintf(buf,"****가입****");
		break;
	    case 4:
		sprintf(buf,"정상로그아웃");
		break;
	}
	fputs(buf,fp1);
	fclose(fp1);
}
	
mudgame()
{
	sleep(1);
	ioctl(0,TCSETAF,&systerm);
	system("stty erase ^H"); 
	system("telnet -8 204.252.146.210 4000");
	ioctl(0,TCSETAF,&mbuf);
/*	printf("\r\n MOO을 마칩니다."); */
	pressreturn();
	reprint=YEA;
}
memocheck()
{
    FILE *fp1;
    char buf[80];
    struct Memo memopaper;
    sprintf(buf,"tmp/MEMO.%s",pf.id);
    if((fp1=fopen(buf,"r")) != NULL) { /* 메모가 있다면 */
	printf("\r\n 메모가 있습니다.");
	while(fread((char*)&memopaper,sizeof(struct Memo),1,fp1)){
	    printf("\r\n%s%s%s",memopaper.date,memopaper.id,memopaper.content);
	}
 	pressreturn();
    }
    fclose(fp1);
    sprintf(buf,"tmp/MEMO.%s",pf.id);
    unlink(buf);
}
messagecheck()
{
    FILE *fp1;
    char ch,buf[80];
  
    sprintf(buf,"tmp/MESSAGE.%s",tty);  /* 자신에게 온 메세지 검사 */
    if((fp1 = fopen(buf,"r")) != NULL) {
        printf("\033[s\0337\033[0;40H\033[7m ");
        while((ch = fgetc(fp1)) != EOF) {
            putchar(ch);
        }
        fclose(fp1);
        unlink(buf);
        printf("\033[0m \0338\033[u");
	reprint =NA; /* 재표시할 필요가 없다. */
    }
}

advertising(num)
int num;
{
    FILE *fp1;
    char buf[80],ch;
    int i,j,k;
    int locate;

    
    printf("\033[s\0337\033[24;0H");
    for(i=0;i<80;i++) printf(" ");
    printf("\033[0m\0338\033[u");
    sprintf(buf,"bin/ADVERTISING");
    fp1=fopen(buf,"r");
    fseek(fp1,0,0);
    for (k=0; k<num; k++) {
	fscanf(fp1,"%s",buf);
    }
    locate = 40- strlen(buf)/2 ;
    i++;
    printf("\033[s\0337\033[24;%dH\033[7m ",locate);
    printf("%d.%s",num,buf);
    printf("\033[0m \0338\033[u");
    fclose(fp1);
}

/* DOLTANK.CFG를 읽어준다. 
DOLTANK.CFG의 앞글은 설명, 뒷 숫자나,문자는 값이다.
설명은 아무 의미를 갖지 못하고 공백이 없어야한다. 값의 설명은
시솝이 초기값을 정할 때 도움을 줄 것이다. */
readDOLTANKCFG()
{
    FILE *fp1;
    char buf[80];
    if( (fp1=fopen("bin/DOLTANK.CFG","r"))==NULL) {
	printf("\r\n DOLTANK.CFG가 열리지 않습니다.");pressreturn();
    }
    fscanf(fp1,"%s%d",buf,&cfg.del);
    fscanf(fp1,"%s%d",buf,&cfg.sh);
    fscanf(fp1,"%s%d",buf,&cfg.chat);
    fscanf(fp1,"%s%d",buf,&cfg.seroom);
    fscanf(fp1,"%s%d",buf,&cfg.inc);
    fscanf(fp1,"%s%s",buf,&cfg.secret);
    fscanf(fp1,"%s%d",buf,&cfg.flag);
    fscanf(fp1,"%s%d",buf,&cfg.room);
    fclose(fp1);
    pressreturn();

}

nansu(len)
int len;
{
	int j;
	j = (int)(   ((float)random()*(float)len)/(float)RAND_MAX  );
	return j;
}
/* cmd를 읽어서 factor별로 argument[30][20]에 집어넣는다. */

parser(char*cmd)
{
    int howmany=0,i,j;
    int len;
    len = strlen(cmd);
    for(i=0;i<len;i++) {
        if( cmd[i] != ' ') {
            j=0;
	    while(cmd[i] !=' ' && i < len) {
		argument[howmany][j] = cmd[i];
		i++; j++;
	    }
	    argument[howmany][j]='\0';
	    howmany++;
	}
    }
    return howmany;
}
/* file이름을 입력 받아서, n1행부터 n2행까지 출력하는 함수 */

show_file(char *fname, int n1, int n2)
{
    FILE *fp1;
    int i,linenum=0,end=0;
    char c; 
    fp1 = fopen(fname,"r");
    fseek(fp1,0,2); end=ftell(fp1);
    fseek(fp1,0,0);
    for(i=0;i<end;i++) {
	c=fgetc(fp1);
	if(c=='\r') {
	    linenum++;
	}
	if(linenum>=n1 && linenum<n2) {
	    printf("%c",c);
	}
    }
    return linenum; /* 출력한 마지막행 라인 넘버를 돌려준다.  */
}
/* 어떤 화일의 총 라인수를 알려주는 함수 */
seehowmanyline(char *fname)
{
    FILE *fp1;
    char c;
    int i,end,linenum=0;
    
    fp1 = fopen(fname,"r");
    if(fp1==NULL) {
   	printf("\r\n 화일이 열리지 않습니다. ");
	pressreturn();
	fclose(fp1);
	return 0;
    }
    fseek(fp1,0,2); end=ftell(fp1); fseek(fp1,0,0);
    for(i=0;i<end;i++) {
	c=fgetc(fp1);
    	if(c=='\r') {
	    linenum++;
	}
    }
    return linenum;
}
del_text(char *direc,char *fhead, int num)
{
    FILE *fp1;
    char buf[80];
    int check=0;
    struct dis_list edittitle;
    if (num<0) {
        return 0;
    }
    sprintf(buf,"%s/%s",direc,fhead);
    printf("\r\n %s를 처리합니다.",buf);
    if ((fp1=fopen(buf,"r+"))==NULL) {
        printf("\r\n 삭제처리할 헤더화일이 없습니다(프로그래밍오류)");
	pressreturn(); return 0; /* 0을 리턴하는 것은 실패일 경우 */
    }
    fseek(fp1,0,0);
    while(fread((char*)&edittitle,sizeof(struct dis_list),1,fp1)) {
	if(edittitle.num==num) {
	    if(!strcmp(edittitle.id,pf.id) || (pf.level>=cfg.del)) {
		edittitle.look=-1;
		fseek(fp1,-sizeof(struct dis_list),1);
		fwrite((char*)&edittitle,sizeof(struct dis_list),1,fp1);
		printf(\
"\033[s\0337\033[1;60H\033[7m%4d번 삭제됨     \033[0m \0338\033[u",num);
		reprint=NA;
		level-=1;
		check=1;
	    }
	}
    }
    fclose(fp1);
    if(check){
        return num;
    }
    else {
        return 0;
    }

}
/* 회원이 있나 찾아보는 함수이다 */
fi_man(char *id)
{
    FILE *fp1;
    char buf[80];
    int check=0;
    struct nowwork work;

    sprintf(buf,"tmp/WHOISNOW");
    fp1=fopen(buf,"r");
    while(fread((char*)&work,sizeof(struct nowwork),1,fp1)) {
	if(!strcmp(work.id,id)) {
	    check=1;
	}
    }
    fclose(fp1);
    return check;
}
/* 그디렉토리에 그 화일이 있는가 검사한다 */
check_file(char *direc, char *filename)
{
    FILE *fp1;
    char buf[80];
    sprintf(buf,"%s/%s",direc,filename);
  
    if((fp1=fopen(buf,"r"))==NULL) {
	return 0;
    }
    else {
        return 1;
    }
}

display_menulogo(char *filename)
{
    FILE *fp1,*fp2;
    char buf[80],ch,fhead[20];
    int howmany,i,today;
    time_t t;
    struct tm *tp,*tp2,*localtime();
    struct dis_list board;

    fp1=fopen(filename,"r");
    while((ch=fgetc(fp1))!=EOF) {
	if(ch=='\n') {
	    putchar('\r');
	}
	if(ch=='~' ) {
	    for(i=0;i<10;i++) {
		if((ch=fgetc(fp1))==']') { break;}
		fhead[i]=ch;
	    }
	    fhead[i]='\0';
	    if(!strcmp(fhead,"아이디")) printf("%s",pf.id);
	    else if (!strcmp(fhead,"이름")) printf("%s",pf.name);

	    if(!show_logofactor("boards",fhead)) show_logofactor("fboards",fhead);
	} 
	else putchar(ch);
    }
    fclose(fp1);
}
show_logofactor(char *direc,char *fhead)
{
    char buf[100];
    FILE *fp2;
    int howmany,today;
    time_t t;
    struct tm *tp,*tp2,*localtime();
    struct dis_list board;
    int check=0;
	    sprintf(buf,"%s/%s",direc,fhead);
            if((fp2=fopen(buf,"r"))!=NULL) {
                fseek(fp2,0,2);
                howmany=ftell(fp2)/sizeof(struct dis_list);
                fseek(fp2,0,0);
                today=0;
                while(fread((char*)&board,sizeof(struct dis_list),1,fp2)) {
                    time(&t);
                    tp=localtime(&t);
                    tp2=localtime(&board.date);
                    if((&tp->tm_year)==(&tp2->tm_year) && \
                      (&tp->tm_mon)==(&tp2->tm_mon) &&\
                      (&tp->tm_mday)==(&tp2->tm_mday)) today++;
                    if(howmany==0) today=0;
                }
                printf("%d/%d",today,howmany);
		check=1;
            }
            else { 
		check=0;
	    }
            fclose(fp2);
    return check;
}
display_cmenulogo(char *filename)
{
    FILE *fp1,*fp2;
    char buf[80],buf2[80],ch,fhead[20];
    int howmany,i,today;
    time_t t;
    struct tm *tp,*tp2,*localtime();
    struct dis_list board;

    fp1=fopen(filename,"r");
    while((ch=fgetc(fp1))!=EOF) {
        if(ch=='\n') {
            putchar('\r');
        }
        if(ch=='~' ) {
            for(i=0;i<10;i++) {
                if((ch=fgetc(fp1))==']') { break;}
                fhead[i]=ch;
            }
            fhead[i]='\0';
	    sprintf(buf,"circle/%s/boards",nmenu->ccode);
	    sprintf(buf2,"circle/%s/fboards",nmenu->ccode);
            if(!show_logofactor(buf,fhead)) show_logofactor(buf2,fhead);
        } 
        else putchar(ch);
    }
    fclose(fp1);
}


upgrade()
{
    char buf[80];
    chdir("bin");
    system("mv wooky kyunghee");
    system("rz");
    printf("\r\n 업그레이드가 끝났습니다.");
    chdir(getenv("HOME"));
}
    
