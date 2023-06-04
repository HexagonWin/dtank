/*  이 프로그램은 1994년 8월에 김태욱에 의해서 작성되기 시작해서
    1994년 12월경에 버젼 1.0이 완성되었습니다. 

    돌탱크 전자 게시판 프로그램.

    이 프로그램의 저작권은 김태욱에게 있습니다. 
                                하이텔: hommage
                                나우콤: hommage
                                인터넷: hommage@power3.snu.ac.kr
                                        s_scsc@cd4680.snu.ac.kr

    이 프로그램은 서울대학교 전기공학부의 전자게시판을 위해서 
    작성되었습니다.

    이 프로그램은 공개 소프트웨어의 한 종류인, 채리티웨어입니다.
    ; 당신은 FSF에서 발간한 GNU 선언문 취지아래 이 프로그램을 배포할 수
      있습니다.
    
    이 프로그램의 무료 배포는 리눅스에 한정합니다.
    만일, 당신이 다른 오퍼레이팅 시스템으로의 포팅이나, 상업적 목적에
    사용하고자 한다면, 제작자의 동의와 허락을 얻어야 합니다.

    당신은 이 프로그램과 함께 DOLTANK.txt를 제공받았어야 합니다. 만일
    그러하지 않다면, 
    서울대 전기공학부 전자게시판 운영자에게 알려주십시요.

                                        서울 관악구 신림동
                                        서울대학교 전기공학부
                                        전자게시판 운영자...
------------------------------------------------------------------------- 

    DOLTANK Bulletin Board System.

    Copyright (C) 1994, Kim tae wook, 
                                HiTel: hommage
                                NowCom: hommage
                                Internet: hommage@power3.snu.ac.kr
                                          s_scsc@cd4680.snu.ac.kr    

    This program is for Bulletin Board System of the department of
    the Electric of the SNU.
    This program is a Charityware, a kind of free software.
    
    Charge-free distribution of this is only for the LINUX.
    In case, you are porting this program into the other Operating 
    Systems or you are using it for the purpose of MERCHANTABILITY
    then you should earn the agreement or permitment from the 
    programmer.

    You should have received DOLTANK.txt in addition to this program.
    If you failed in doing so, 
    Write to the sysop of BBS of the Department of the Electric of
    SNU.

                                        Seoul Kwan-Ak Shilim-dong
                                        SNU the department of electric
                                        BBS sysop, Kim tae wook...

*/

#include "bbs.h"

#define TIME 1800L

int isatty(fd)
int fd;
{
    struct termios term;
    return(ioctl(fd,TCGETA,&term) != -1);
}

main()
{
    int ps,loop;
    char c, tty[25], *tmp, *getty, *ttyname();
    FILE *fp1;
    struct stat statbuf;
    struct termio tbuf;
    time_t current, i;
    umask(0111);
    for(ps = 0; ps <= 10; ps++) {
        if(isatty(ps)) {
            getty = ttyname(ps);
            break;
        }
    }
    tmp = &getty[5];
    sprintf(tty,"tmp/STAT.%s",tmp);  /* 이용자의 상태를 숫자로 판단하는 화일*/
    fp1 = fopen(tty,"w");
    fputc('0',fp1);
    fclose(fp1);
    ps = fork();
    if(ps) execl("bin/wooky","wooky",tmp,(char*)0);
    else {
        nice(10);
	sleep(5);

        i = 0;
        while((fp1 = fopen(tty,"r")) == NULL);
        fclose(fp1);
        while(1) {
            fp1 = fopen(tty,"r");
            c = fgetc(fp1);
	    if(fp1==NULL) c=3;  /* 화일을 열수없다면 문제가 심각합니다. */
            fclose(fp1);
            if(c == '1' || c=='4') {   /* 다운,업 중일 때이다. 다운 중일때는
				시간 체크를 하지 않아야겠지요.?*/
                time(&i);
            }
            else if(c == '3') {  /* 뭔가 잘못이 있을 때다. 즉시 중지 */
		ps =getppid();
		kill(ps,SIGHUP);		
                exit(0);
            }
            else {    /* 이것은 평상시 사용할 때다. 0일때이다. */
                sleep(20);
                fstat(0, &statbuf);
                time(&current);
                if(current - (statbuf.st_atime + i) >= TIME) {
                    ps = getppid();
                    printf("\r\n\007키입력이 없어서 자동으로 끊어집니다.\r\n");
                    kill(ps, SIGHUP);
                    exit(0);
                }
                i = 0;
            }
            sleep(1);
        }
    }
}

