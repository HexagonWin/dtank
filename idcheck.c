
#include "bbs.h"

struct nowwork work,*chk_work;
time_t buftime;

id_in()
{
    int resultcode,guestcheck=1;
    int logfail, i, j, len;
    time_t t;
    char id[9], passwd[9], ch, buf[80], *tmp;
    struct tm *tp, *localtime();
    FILE *fp1, *fp2;
    tmp = tmp_buf; 
    logfail = 0;
    if((fp1 = fopen("tmp/WHOISNOW","r")) != NULL) {
        fseek(fp1,0L,SEEK_END);
        len = ftell(fp1) / sizeof(struct nowwork) + 1;
        fseek(fp1,0L,SEEK_SET);
        if((chk_work = (struct nowwork *)calloc(len,sizeof(struct nowwork)))\
	  == NULL) {   /* 메모리 확보 */
            perror("\r\n메모리가 부족");
            host_end();
        }
        i = 0;
        while(fread((char*)&work,sizeof(struct nowwork),1,fp1)) {
            if(strcmp(work.tty,tty)) {  /* 유령노드 제거 */
                chk_work[i].chose = work.chose;
                sprintf(chk_work[i].id,"%s",work.id);
                sprintf(chk_work[i].name,"%s",work.name);
                sprintf(chk_work[i].ccode,"%s",work.ccode);
                sprintf(chk_work[i].tty,"%s",work.tty);
                ++i;
            }
        }
        fclose(fp1);
	unlink("tmp/WHOISNOW");
        fp1 = fopen("tmp/WHOISNOW","w");
        for(j = 0; j < i; j++) {
            fwrite((char*)&chk_work[j],sizeof(struct nowwork),1,fp1);
        }
        fclose(fp1);
        free(chk_work); /* 확보 메모리 해제 */
    }
    while(1) {
        printf(\
"\r\n 신규가입을 원하는 분은 이용자번호에 '손님(guest)'을 입력하십시오. ");
        printf("\r\n 이용자번호 : ");
        lineinput(id,8,YEA);
        if((!strcmp(id,"손님")) || \
	  (!strcmp(id,"guest")) || (!strcmp(id,"GUEST"))) {  
            ioctl(0, TCSETAF, &systerm);
            resultcode=guestregister();
            if(resultcode==0) return 1; /* guest=1으로 만들고 비비시작 */
            else {
		perror("\r\n 가입프로그램이 작동하지 않습니다 ");
		host_end();
	    }
        }
        else {
            if(strlen(id) > 2) {
                i = 1; 
                if((fp1 = fopen("tmp/WHOISNOW","r")) != NULL) {
                    while(fread((char*)&work,sizeof(struct nowwork),1,fp1)) {
                        if(!strcmp(id,work.id)) {  
                            printf("\r\n누가 사용하고 있습니다.");
                            printf("\r\n%s에서 사용중입니다.",work.tty);
                        }
                    }
                } 
		fclose(fp1);
                if(i) {
                    i = 1; /* 회원이 있나 체크 */
		    fclose(fp1);
                    if((fp1 = fopen("bin/PASSWDS","r")) == NULL) {
                        printf("\r\n아이디 검색화일이 열리지 않습니다.\r\n");
                        host_end();
                    }
                    if(i) {
			while(fread((char*)&pf,sizeof(struct idst),1,fp1)) {
                            if(!strcmp(id,pf.id)) { /* 아이디 검사 */
                                printf("\r\n %s님은 일반회원이십니다.",pf.id);
                                i = 0;
                                fos = ftell(fp1) - sizeof(struct idst);
                                guestcheck=0;
                                break;
                            }
                        }
		    }
                    fclose(fp1);
                    fp1=fopen("bin/PASSWDS.TMP","r");
                    if(guestcheck && i && fp1!=NULL) { /* 정회원이 아니고 */
			while(fread((char*)&pf,sizeof(struct idst),1,fp1)) {
			    if(!strcmp(id,pf.id)) {
				printf("\r\n 당신은 아직 가입신청 처리중입니다.");
				i=0;
			 	guestcheck=1; /* 손님자격 */	
				fos = ftell(fp1) - sizeof(struct idst);
			    }
			}
		    } 
		    fclose(fp1);
                    if(i) {
                        printf("\r\n해당회원이 존재하지 않습니다.");
                        ++logfail;
                    }
                    else {  /* 비밀번호 입력 */
                        i = 0;
                        len = 9;
                        printf("\r\n 비밀  번호 : ");
			lineinput(passwd,10,NA);
                        if(strcmp(pf.passwd,passwd)) {  /* 비밀번호 검사 */
                            printf("\r\n비밀번호가 틀립니다.");
                            ++logfail;
                        }
                        else if(pf.level < 0) { /* 사용자레벨이 음수일때 */
                            printf("\r\n죄송합니다만 운영자와 상의하십시오.\r\n");

                            sleep(1);
                            host_end();
                        }
                        else {  /* 정상 아이디 입력시 */
                            printf("\r\n\n %s님 반갑습니다.",pf.name);
                            tp = localtime(&pf.logout);
                            printf("\r\n\n 전회 LOGOUT : 19%02d/%02d/%02d  %02d:%02d:%02d",
                              tp->tm_year,tp->tm_mon+1,tp->tm_mday,tp->tm_hour,tp->tm_min,tp->tm_sec);
                            sprintf(work.id,"%s",pf.id);    /* 작업내용 기록 */
                            pf.logout = buftime;
			    time(&t);
                            tp = localtime(&t);
                            printf("\r\n 금일 LOGIN  : 19%02d/%02d/%02d  %02d:%02d:%02d",
                              tp->tm_year,tp->tm_mon+1,tp->tm_mday,tp->tm_hour,tp->tm_min,tp->tm_sec);
                            sprintf(buf,"%02d.%02d(+)",tp->tm_mon+1,tp->tm_mday);
                            tmp = &pf.birthday[3];
                            if(!strcmp(tmp,buf)) {  /* 양력 생일 검사 */
                                logo("birthday.log",1);   /* 생일시에 보내는 로고 */
                            }

                            sprintf(work.name,"%s",pf.name);
                            sprintf(work.ccode,"top");
                            sprintf(work.tty,"%s",tty);
                            work.chose = ' ';
                            fp1 = fopen("tmp/WHOISNOW","a");
                            if(strcmp(pf.id,"darker"))fwrite((char*)&work,sizeof(struct nowwork),1,fp1);
                            fclose(fp1);

			    if(guestcheck) { sprintf(buf,"bin/PASSWDS.TMP"); }
			    else { sprintf(buf,"bin/PASSWDS"); }
			    fp1=fopen(buf,"r+");
                            if(fp1 == NULL) {  
                                printf("\r\n프로필 화일이 열리지 않습니다.");
                                host_end();
                            }
                            fseek(fp1,fos,SEEK_SET);
                            fwrite((char*)&pf,sizeof(struct idst),1,fp1);
                            fclose(fp1);
			    pressreturn();
                            break;
                        }
                    }
                }
            }
            else ++logfail;
            if(logfail > 4) {   /* 아이디 입력 4번 실페시 접속 끊음 */

                printf("\r\n회원아이디와 비밀번호를 확인해 주십시오.\r\n");
                host_end();
            }
        }
    }
    return guestcheck; /* 이것은 손님인가 정회원인가를 가름한다. */ 
}

