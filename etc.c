/* 	void mess(); �� ���� tmp/WHOISNOWȭ�Ͽ��ٰ� �ڽ��� ��ġ�� �����ϴ�
			�Լ��Դϴ�. 
	void logo(discode); �� �Լ��� �ΰ�ȭ���� ����մϴ�.
		�ΰ�ȭ�ϵ���
		logo/ ���丮�� ���ֽ��ϴ�. 
		�Ƚ��ڵ嵵 �� �� ������ �������� ����Ŀ� ������ ������
		�� ��µ� �Ŀ� �ƹ�Ű�� ������ �������ɴϴ�.
	int go_select(buf); ����޴�ã�� �Լ��Դϴ�.
		� �޴������� go �޴����ϸ� �� �޴��� ã�ư� �� �ֵ���
		���ݴϴ�. 
	struct tm *tim(); �ð� ���� �Լ��Դϴ�.
		���� �ð��� localtimeȭ�ؼ� �����ͷ� �ǵ����ݴϴ�. 
	err_off(); ������ ���Ḧ ��Ű�� �Լ��Դϴ�.
		exit(1)�� �����մϴ�.
	dis_pf(s_pf); ȸ�� ���� ��ȸ �Լ��Դϴ�.
		������ ���� ����ϴ� ����� �ٸ��ϴ�.
	int host_end(); ���α׷� ���� ��ƾ�Դϴ�.
		tmp/time.tty?? �� 0�� ����־ ��밡���� �͹̳��� �����
		��ȭ���� ����(�������� �ʾ��� ���)�� �����ϰ�
		������ ���ŵǾ��ٸ� ���ΰ� �������ٰ� ��ȭ�ǿ� ����մϴ�.
		WHOISNOW(��������ȸ��)���� �ڽ��� �����
	 	�ڽſ��� �� �޽����� �˻��ϰ�
		���ӽð� ����ð����Բ� �ȳ��� �����ö�� �Ҹ���
		���� �ΰ� ����Ѵ�. 
		�׸��� ���������� �մ��ϰ��� �ڽ��� ����� �������� �ʰ�
		ȸ���� ��쿡�� �ڽ��� ����� �����Ѵ�. ����ÿ�
		����Ƚ���� ������Ű�� �� ���ð��� �����Ѵ�.
		menu�� �޸𸮿��� ���Ž�Ű��,
		���������ϰ�� escape(��)�ڵ带 ���� ��
		ath0���� hook out�Ѵ�.
	void menu_free(); menu������ �޸𸮿��� �����Ѵ�.
	void del_mylogin(); tmp/WHOISNOW���� �ڽ��� �����.
	int bye(); exit(0)�� ���� ���̴�.
	login_who();  ������ ����� �����ش�.
	int chk_pf(buf); ȸ�������� �˻��Ѵ�.
	int change_in(); �ʴ���¸� �����Ѵ�.
	void mess_dis(i); �Խ��ǵ�� ���̴� ���� �޽���
		i�� ���� �޽����� �ٸ��ϴ�.
		1= �ش� �޴� ����
		2= ���� ����
		3= ������ ��ȣ��
		4= ���Ѿ���
	void host_end_yn(); ȣ��Ʈ�� ������ ���� �� ���θ� ����ϴ�.
	dis_size_fn(i); ȭ�� ũ�⸦ ǥ����
	check_letter(); �����ؼ� idüũ�Ŀ� ������ �Գ� üũ..
	cls();  ȭ�� ����� 
*/

#include "bbs.h"


int chose_menu()  /* �޴� ���� �˻� */
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
        else {  /* ������ �ȵɰ�� */
            key = 0;
            mess_dis(4);
        }
        nmenu = nmenu->upp;
    }
    else if(nmenu->form =='P') { /* �ѰԽù� �����ֱ� */
	display_onecontent(0,"page",nmenu->ccode);
	nmenu=nmenu->upp;
    }
    else if(nmenu->form == 'C') { /* ��ȣȸ�� ���� ���� */
	cmain();
        if(upermenu==1) {nmenu=nmenu->upp;upermenu=0;}
    }
    else if(nmenu->form == 'O') { /* �繫�ǿ� ���� ���Դϴ�. */
   	officemain();
	nmenu=nmenu->upp;
    }
    else if(nmenu->form=='S') { /* ���� �˻� */
   	munhun("munhun",nmenu->ccode);
    	nmenu=nmenu->upp;
    }
    else if(nmenu->form == 'R') {  /* �Խù��̽���ȭ��.�̰��� �⺻������� */
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
	    if(!strcmp(nmenu->ccode,"hitel")) { /* ������ ���� */
	    	hitelemul();
		nmenu=nmenu->upp;
	    }
            if(!strcmp(nmenu->ccode,"chat")) { /* ��ȭ�� */
                chat();
		nmenu= nmenu->upp;
            }
            if(!strcmp(nmenu->ccode,"MUD")) {
                mudgame();
                nmenu=nmenu->upp;
            }
            else if(!strcmp(nmenu->ccode,"change")) {   /* ȸ���������� */
		cls();
		if(guest) { 
		    printf("\r\n ����ó���� �ȵ� ���� ����Ͻ� �� �����ϴ�.");
		}
		else myinfo();
		nmenu=nmenu->upp;
            }
            else if(!strcmp(nmenu->ccode,"rmail")) {  /* �����б� */
		cls();
                rmail();
                if(upermenu) {nmenu=nmenu->upp;upermenu=0;}
            }
	    else if(!strcmp(nmenu->ccode,"newrmail")) {  /* ���ο� ���� �б� */
	   	cls();
		newrmail();
		if(upermenu) {nmenu=nmenu->upp;upermenu=0;}
	    }
            else if(!strcmp(nmenu->ccode,"wmail")) {  /* �������� */
		cls();
                wmail();
                nmenu=nmenu->upp;
            }
	    else if(!strcmp(nmenu->ccode,"newwmail")) {  /* ���ο� ���� ���� */
		cls();
		newwmail();
		nmenu=nmenu->upp;
	    }
            else if(!strcmp(nmenu->ccode,"cmail")) { /* �������� üũ */
                cmail();
                nmenu= nmenu->upp;
            }
            else if(!strcmp(nmenu->ccode,"smail")) {    /* ������ ����  */
                mailsort();
                nmenu= nmenu->upp;
            }
	    else if(!strcmp(nmenu->ccode,"mailist")) { /*���κ� �ּҷ� */
		cls();
		editmailist("mail");
		nmenu=nmenu->upp;
	    }
	    else if(!strcmp(nmenu->ccode,"Mailist")) {  /* ��ü �ּҷ� */
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
    else if(nmenu->form == 'L' || nmenu->form == 'l') {   /* �Խù� */
	sprintf(buf,"%s.log",nmenu->ccode);
        logo(buf,1);
        show_board("boards",nmenu->ccode,boardcmd);
	if(upermenu==1) {nmenu=nmenu->upp; upermenu=0;}
    }
    else if(nmenu->form == 'F' || nmenu->form == 'f') {   /* ȭ�� �Խù� */
	sprintf(buf,"%s.log",nmenu->ccode);
        logo(buf,1);
        show_board("fboards",nmenu->ccode,filecmd);
        if(upermenu==1) {nmenu=nmenu->upp; upermenu=0;}
    }
    else if(nmenu->form == 'T' ) {  /* ���� ��� �Դϴ�. */
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
            if(!strcmp(work.tty,tty)) { /* �ڽ��� ���ӳ�带 ã������ */
                sprintf(work.ccode,"%s",nmenu->ccode);
                fseek(fp1,-sizeof(struct nowwork),1);
                fwrite((char*)&work,sizeof(struct nowwork),1,fp1);
                break;
            }
        }
        else {  /* �ڱ��� ��ġǥ�ð� ������ */
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
/* �ΰ�ȭ�� ��� */
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

int go_select(buf)  /* ���� �޴� ã�� */
char *buf;
{
    int i, j;
    char *c;
    j = strlen(buf);    /* ���� ���� */
    buf[0] = ' '; buf[1] = ' ';
    for(i = 0; i < j; i++) {    /* ���鹮�ھƴ� ��ġ ã�� */
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

    if(strcmp(c,mtmp->ccode)) {  /* ������ �޴��� �� ã�� */
        mtmp = mtmp->downp;
        ++i;
        while(menu != mtmp) {
            nmenu->deep = i;
            if(!strcmp(c,mtmp->ccode)) {   /* �޴��� ã������ */
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
    else { /* top �޴��϶� */
        nmenu = menu;
        j = 1;
    }
    relogo=YEA;
    return(j);

}
struct tm *tim()    /* �ð� ���� */
{
    long t;
    struct tm *tp, *localtime();
    time(&t);
    tp = localtime(&t);
    return (tp);
}

err_off()   /* ������ ���� ��ƾ */
{
    exit(1);
}

dis_pf(s_pf)    /* ȸ�� ���� ��ȸ */
struct idst s_pf;
{
    char buf[2];
    struct tm *tp, *localtime();
    printf("\r\n %s ȸ���� �����Դϴ�.",s_pf.id);
    if(pf.level > cfg.sh) printf("\r\n\n   �� ��  �� ȣ : %05d",s_pf.num);
    printf("\r\n   ��        �� : %s",s_pf.name);
    if(s_pf.flag & 0x0001) printf("\r\n   ��        �� : %c",s_pf.sex);
    else if(pf.level >= cfg.sh) printf("\r\n * ��        �� : %c",s_pf.sex);
    if(s_pf.flag & 0x0002) printf("\r\n   �� ��  �� �� : %s",s_pf.birthday);
    else if(pf.level >= cfg.sh) printf("\r\n * �� ��  �� �� : %s",s_pf.birthday);
    tp = localtime(&s_pf.inday);
    if(s_pf.flag & 0x0004)
    printf("\r\n   �� ��  �� �� : %2d�� %02d�� %02d��",tp->tm_year,(tp->tm_mon)+1,tp->tm_mday);
    else if(pf.level >= cfg.sh)
    printf("\r\n * �� ��  �� �� : %2d�� %02d�� %02d��",tp->tm_year,(tp->tm_mon)+1,tp->tm_mday);
    tp = localtime(&s_pf.logout);
    if(s_pf.flag & 0x0008)
    printf("\r\n   ������������ : 19%2d-%02d-%02d  %02d:%02d:%02d",
      tp->tm_year,tp->tm_mon+1,tp->tm_mday,tp->tm_hour,tp->tm_min,tp->tm_sec);
    else if(pf.level >= cfg.sh)
    printf("\r\n * ������������ : 19%2d-%02d-%02d  %02d:%02d:%02d",

      tp->tm_year,tp->tm_mon+1,tp->tm_mday,tp->tm_hour,tp->tm_min,tp->tm_sec);
    if(s_pf.flag & 0x0010) printf("\r\n   �� ��  �� ȣ : %s",s_pf.post);
    else if(pf.level >= cfg.sh) printf("\r\n * �� ��  �� ȣ : %s",s_pf.post);
    if(s_pf.flag & 0x0020) printf("\r\n   ��   ��   �� : %s",s_pf.home_addr);
    else if(pf.level >= cfg.sh) printf("\r\n * ��   ��   �� : %s",s_pf.home_addr);
    if(s_pf.flag & 0x0040) printf("\r\n   ��   ��   ȭ : %s",s_pf.home_tel);
    else if(pf.level >= cfg.sh) printf("\r\n * ��   ��   ȭ : %s",s_pf.home_tel);
    if(s_pf.flag & 0x0080) printf("\r\n   ��   ��   �� : %s",s_pf.office_name);
    else if(pf.level >= cfg.sh) printf("\r\n * ��   ��   �� : %s",s_pf.office_name);
    if(s_pf.flag & 0x0100) printf("\r\n   ������ȭ��ȣ : %s",s_pf.office_tel);
    else if(pf.level >= cfg.sh) printf("\r\n * ������ȭ��ȣ : %s",s_pf.office_tel);
    if(s_pf.flag & 0x0200) printf("\r\n   �ֹε�Ϲ�ȣ : %s",s_pf.id_no);
    else if(pf.level >= cfg.sh) printf("\r\n * �ֹε�Ϲ�ȣ : %s",s_pf.id_no);
    if(s_pf.flag & 0x0400) printf("\r\n   ��        �� : %d",s_pf.level);
    else if(pf.level >= cfg.sh) printf("\r\n * ��        �� : %d",s_pf.level);
    printf("\r\n   �� ��  �� �� : ");
    if(s_pf.flag & 0x1000) printf("���");
    else printf("����");
    if(pf.level > cfg.sh) printf("\r\n * ȸ��  �÷��� : %x",s_pf.expflag);
    if(pf.level > cfg.sh) {
        tp = localtime(&s_pf.lasttime);

        printf("\r\n * �� ��  �� �� : 19%2d-%02d-%02d  %02d:%02d:%02d",
          tp->tm_year,tp->tm_mon+1,tp->tm_mday,tp->tm_hour,tp->tm_min,tp->tm_sec);
    }
}

int host_end()  /* ���α׷� ���� ��ƾ */
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
    if(chatcheck) {                           /* ���� ��ȭ�ǿ� �־��ٸ� */
        sprintf(buf,"\r\n %s�� ���ΰ� ���������ϴ�.",pf.id);
        shmaddr = shmat(shmid, NULL,MODE);
        strcpy(shmaddr, buf);
        shmdt(shmaddr);
    }
    del_mylogin();
    time(&t);  /* ���� �ð� ��� */
    lest.intime = pf.logout; /* records the last logintime */
    lest.outtime = t; /* records the last logout time */
    sprintf(lest.id,"%s",pf.id);
    sprintf(lest.name,"%s",pf.name);
    sprintf(lest.tty,"%s",tty);
    lest.flag = pf.expflag;
    tp = tim();
    writeLOGFILE(2);
    if(end_flag) {
        tp = localtime(&pf.logout); /* ���ӽð�ǥ�� */
        printf("\r\n LOGIN  ... %2d/%02d/%02d   %02d:%02d:%02d",tp->tm_year,
          tp->tm_mon+1,tp->tm_mday,tp->tm_hour,tp->tm_min,tp->tm_sec);
        tp = localtime(&t); /* ����ð�ǥ�� */
        printf("\r\n LOGOUT ... %2d/%02d/%02d   %02d:%02d:%02d",tp->tm_year,
          tp->tm_mon+1,tp->tm_mday,tp->tm_hour,tp->tm_min,tp->tm_sec);
        printf("\r\n\n %s�� �ȳ��� ���ʽÿ�.\r\n",pf.name);

        sprintf(buf,"goodbye.log");
        logo(buf,0);
    }
    i = t - pf.logout;
    sprintf(buf,"tmp/STAT.%s",tty);
    unlink(buf);
    if(guest) { 
	printf("\r\n ����� �մ��̹Ƿ� ����� ������� �ʽ��ϴ�.");
    }

    if(!guest) { /* �մԾƴ� ��� ��� ���� */
	printf("\r\n ����� �����մϴ�. ");
        sprintf(buf,"%s",pf.id);
        fp1 = fopen("bin/PASSWDS","r+");
        if(fseek(fp1,pf_fos,0)) {
            printf("\r\n�б� ����");
            bye();
        }
        fread((char*)&pf,sizeof(struct idst),1,fp1);
        if(strcmp(pf.id,buf)) {
            printf("\r\nID�� Ʋ���ϴ�");
            bye();
        }
        pf.logout = t;
	if( pf.level<50) {
	    pf.level +=level; /*�⺻�� 30���� 50������ ���� */
	    printf("\r\n ������ %d ����ϼ̽��ϴ�.",level);
	}
	else printf("\r\n �Ϲ���ȸ�� �ְ����̽ʴϴ�.");

	if (pf.level > 60) report("highlevel",l_title);

        ++pf.logcount;  /* ����Ƚ�� ���� */
        pf.totaltime = pf.totaltime + i; /* �� ���ð� ��� */
        if(fseek(fp1,pf_fos,0)) {
            printf("\r\n���� ����");

            bye();
        }
        fwrite((char*)&pf,sizeof(struct idst),1,fp1);
        fclose(fp1);
    }
    Menu_Free();
    ioctl(0, TCSETAF, &systerm);
    if(cfg.flag & 1) {   /* ���� ���� */
        signal(SIGHUP, SIG_DFL);
        sprintf(buf,"/dev/%s",tty);
        sleep(1);
        if((i=open(buf,1))==-1) {
            printf("\r\n%sȭ�� ����\r\n",buf);
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
            perror("\r\n�޸𸮰� ����");
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
login_who() /* ������ ��� */
{
    int i = 0;
    FILE *fp1;
    printf("\r\n  ===========>>> ������ ��� <<<===========");
    printf("\r\n  ��ȣ   ���̵�     ��  ��    �ִ°�   ���");
    printf("\r\n  =========================================");
    if((fp1 = fopen("tmp/WHOISNOW","r")) != NULL) {   /* ������ ��� */
        while(fread((char*)&work,sizeof(struct nowwork),1,fp1)) {
            ++i;
            printf("\r\n   %2d   %8s  %8s  %8s %6s %c",
            i,work.id,work.name,work.ccode,work.tty,work.chose);
        }
        fclose(fp1);
        printf("\r\n");
    }
}
int chk_pf(char *cmd) /* ȸ�� ���� �˻� */
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
            if(!strcmp(tmp,you_pf.id)) {    /* �ش�ȸ�� ã�� */
                display_pf(you_pf);
                i = 0;
                break;
            }
        }
        fclose(fp1);
        if(i) { /* �ش� ȸ���� ������� */
            key = 1;
            printf("\r\n�ش� ȸ�� ����\r\n");
        }
    }
    else {  /* pf��ɾ ������� �ڽ��� ������ */
        display_pf(pf);
    }
    return (key);
}

int change_in() /* �ʴ� ���� ���� */
{
    char buf[50]; 
    FILE *fp1;
    sprintf(buf,"%s/tmp/WHOISNOW",getenv("HOME"));
    fp1 = fopen(buf,"r+");
    while(fread((char*)&work,sizeof(struct nowwork),1,fp1)) {
        if(!strcmp(work.id,pf.id)) {
            if(work.chose == ' ') {       /*�ʴ밡���̸� �ʴ�Ұ��� �ٲ� */
                work.chose = 0x2a;
            }
            else {
                work.chose = 0x20;        /* �ʴ�Ұ��� �������� �ٲ� */
            }
            break;
        }
    }
    if(!fseek(fp1,-sizeof(struct nowwork),1)) {
        fwrite((char*)&work,sizeof(struct nowwork),1,fp1);
    }
    fclose(fp1);
    if(work.chose == ' ') printf("\r\n�ʴ� ����");
    else printf("\r\n�ʴ� �ź�");
    printf("\r\n");
}

void mess_dis(i)
int i;
{
    printf("\033[s\0337\033[7m\033[0;60H");
    if(i == 1) {
    	printf("--�ش�޴� ����--");
    }
    else if(i == 2) {
    	printf("  --���� ����--  ");
    }
    else if(i == 3) {
    	printf("--������ ��ȣ��--");
    }
    else if(i == 4) {
    	printf("***�� �� �� ��***");
    }
    else if(i==5) {
    	printf("--�� �� ��   ��--");
    }
    printf("\0338\033[0m\033[u");

}
void host_end_yn()    /* ȣ��Ʈ�� ���� */
{
    char buf[2];
    printf("\r\n���Ḧ �Ͻðڽ��ϱ�(Y/n)? ");
    oneinput(buf);
    if(!(buf[0] == 'n' || buf[0] == 'N')) {
        end_flag = 1;
	writeLOGFILE(4);
        host_end();
    }
}


/* �����Կ� ������ ���� �˻� */
/* �Ϲ� �������̴� letter�� ����Ǵ� */
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
    sprintf(buf,"\007\r\n%2d/%2d/%2d %2d��%2d��%2d�� %s(%s) ",
tp->tm_year,tp->tm_mon+1,tp->tm_mday,tp->tm_hour,tp->tm_min,tp->tm_sec,pf.id,tty);
    fputs(buf,fp1);        
    if (!strncmp(code,"down",strlen(code))) {
        sprintf(buf,"\n%s���� (%s)�� �ٿ��߳�..",nmenu->ccode,listdata.filename);
    }else if (!strncmp(code,"up",strlen(code))) {
        sprintf(buf,"\n%s���� (%s)�� ***���� �ϼ̾��***",nmenu->ccode,listdata.filename);
    }else if (!strncmp(code,"write",strlen(code))) {
	sprintf(buf,"\n%s���� [%s]�� ���̽��ϴ�.",nmenu->ccode,listdata.title);
    }else if (!strncmp(code,"highlevel",strlen(code))) {
	sprintf(buf,"--- ��޻����%s�� �ٳబ���ϴ�.---",pf.id);
    }else if (!strncmp(code,"uploadfail",strlen(code))) {
	sprintf(buf,"\r\n %s���� [%s]�� ���ε��ߴµ� \r\n\
���ε尡 �߸��Ǿ����. ftp���丮�� �ֽ��ϴ�.",nmenu->ccode,listdata.filename);
    }else if (!strncmp(code,"registererror",strlen(code))) {
        sprintf(buf,"\n\n %s ���� ���ε��� ȭ���� ����� �ȵǾ����."\
,nmenu->ccode);
	sprintf(buf,"\n ȭ���̸��� (%s)�Դϴ�. fail���丮�� �ֽ��ϴ�."\
,listdata.filename);
    }else sprintf(buf,"****�׷� �ڵ�� ���� ���մϴ�.****");
    fputs(buf,fp1);
    fclose(fp1);
}


displayfileheader(struct dis_list fileheader)
{
    printf("\r\n");
    printf("\r\n ��ȣ: %d",fileheader.num);
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
        sprintf(buf,"\n%2d/%2d/%2d %2d��%2d��%2d�� %10s(%5s) ",
tp->tm_year,tp->tm_mon+1,tp->tm_mday,tp->tm_hour,tp->tm_min,tp->tm_sec,pf.id,tty);
	fputs(buf,fp1);
		
	switch(code) {
	    case 1: 
		sprintf(buf,"��___��___��");
		break;
	    case 2:
		sprintf(buf,"��________��\n");
		break;
	    case 3:
		sprintf(buf,"****����****");
		break;
	    case 4:
		sprintf(buf,"����α׾ƿ�");
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
/*	printf("\r\n MOO�� ��Ĩ�ϴ�."); */
	pressreturn();
	reprint=YEA;
}
memocheck()
{
    FILE *fp1;
    char buf[80];
    struct Memo memopaper;
    sprintf(buf,"tmp/MEMO.%s",pf.id);
    if((fp1=fopen(buf,"r")) != NULL) { /* �޸� �ִٸ� */
	printf("\r\n �޸� �ֽ��ϴ�.");
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
  
    sprintf(buf,"tmp/MESSAGE.%s",tty);  /* �ڽſ��� �� �޼��� �˻� */
    if((fp1 = fopen(buf,"r")) != NULL) {
        printf("\033[s\0337\033[0;40H\033[7m ");
        while((ch = fgetc(fp1)) != EOF) {
            putchar(ch);
        }
        fclose(fp1);
        unlink(buf);
        printf("\033[0m \0338\033[u");
	reprint =NA; /* ��ǥ���� �ʿ䰡 ����. */
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

/* DOLTANK.CFG�� �о��ش�. 
DOLTANK.CFG�� �ձ��� ����, �� ���ڳ�,���ڴ� ���̴�.
������ �ƹ� �ǹ̸� ���� ���ϰ� ������ ������Ѵ�. ���� ������
�ü��� �ʱⰪ�� ���� �� ������ �� ���̴�. */
readDOLTANKCFG()
{
    FILE *fp1;
    char buf[80];
    if( (fp1=fopen("bin/DOLTANK.CFG","r"))==NULL) {
	printf("\r\n DOLTANK.CFG�� ������ �ʽ��ϴ�.");pressreturn();
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
/* cmd�� �о factor���� argument[30][20]�� ����ִ´�. */

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
/* file�̸��� �Է� �޾Ƽ�, n1����� n2����� ����ϴ� �Լ� */

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
    return linenum; /* ����� �������� ���� �ѹ��� �����ش�.  */
}
/* � ȭ���� �� ���μ��� �˷��ִ� �Լ� */
seehowmanyline(char *fname)
{
    FILE *fp1;
    char c;
    int i,end,linenum=0;
    
    fp1 = fopen(fname,"r");
    if(fp1==NULL) {
   	printf("\r\n ȭ���� ������ �ʽ��ϴ�. ");
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
    printf("\r\n %s�� ó���մϴ�.",buf);
    if ((fp1=fopen(buf,"r+"))==NULL) {
        printf("\r\n ����ó���� ���ȭ���� �����ϴ�(���α׷��ֿ���)");
	pressreturn(); return 0; /* 0�� �����ϴ� ���� ������ ��� */
    }
    fseek(fp1,0,0);
    while(fread((char*)&edittitle,sizeof(struct dis_list),1,fp1)) {
	if(edittitle.num==num) {
	    if(!strcmp(edittitle.id,pf.id) || (pf.level>=cfg.del)) {
		edittitle.look=-1;
		fseek(fp1,-sizeof(struct dis_list),1);
		fwrite((char*)&edittitle,sizeof(struct dis_list),1,fp1);
		printf(\
"\033[s\0337\033[1;60H\033[7m%4d�� ������     \033[0m \0338\033[u",num);
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
/* ȸ���� �ֳ� ã�ƺ��� �Լ��̴� */
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
/* �׵��丮�� �� ȭ���� �ִ°� �˻��Ѵ� */
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
	    if(!strcmp(fhead,"���̵�")) printf("%s",pf.id);
	    else if (!strcmp(fhead,"�̸�")) printf("%s",pf.name);

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
    printf("\r\n ���׷��̵尡 �������ϴ�.");
    chdir(getenv("HOME"));
}
    
