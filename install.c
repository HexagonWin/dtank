/* 이것은 인스톨 프로그램입니다. */

#include <stdio.h>
#include "bbs.h"
rawmode()
{
    struct termio tbuf;
    ioctl(0, TCGETA, &tbuf);
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
    ioctl(0, TCSETAF, &tbuf);
    return;
}

main()
{
	char buf[80];
	char installdir[80];
	char upgradedir[80];	

    system("stty raw");
    system("stty -echo");
    while(1) {
	printf("\r\n 무슨 일을 하실 겁니까?");
	printf("\r\n (1) 업그레이드 (2) 인스톨 (0) 취소");
	printf("\r\n 명령>> ");keyinput(buf,2);
	if(buf[0]=='\0') {
	    printf("\r\n 명령을 집어넣어주세요.");
	}
	else if(buf[0]=='0') {
	    printf("\r\n 자동인스톨명령에서 나갑니다.");
	    break;
	}
	else if(buf[0]=='1') {
	    printf("\r\n 업그레이드를 실행하겠습니다.");
	    printf("\r\n 업그레이드할 비비에스디렉토리명을 적어주세요");
	    printf("\r\n (e.g. /home/bbs) >> "); keyinput(upgradedir,30);
	    if(upgradedir[0]=='\0'){
		printf("\r\n 디렉토리명을 집어넣어야합니다.");
	   	break;
	    }
	    sprintf(buf,"update.sh %s",upgradedir);
	    system("stty -raw");
	    system(buf);
	    system("stty raw");
	    printf("\r\n 업그레이드가 끝났습니다.");
	}
	else if(buf[0]=='2') {
	    printf("\r\n 인스톨을 시작하겠습니다. ");
	    printf("\r\n **** 주의 *****");
	    printf("\r\n 인스톨을 시작하면 비비에스 메뉴화일까지 지워버립니다");
	    printf("\r\n 새로이 비비에스를 만들려는 것이 맞지요(y/N) ");
	    keyinput(buf,2);
	    if(buf[0] !='y' && buf[0] !='Y') break;
	    printf("\r\n 인스톨하실 비비에스디렉토리를 적어주세요.");
	    printf("\r\n (e.g. /home/bbs) >> "); keyinput(installdir,30);
	    if(installdir[0]=='\0') {
		printf("\r\n 디렉토리명을 집어넣어야합니다.");
	    	break;
	    }
	    sprintf(buf,"install.sh %s",installdir);
	    system(buf);
	    printf("\r\n 인스톨이 끝났습니다.");
	}
    }
    printf("\r\n 프로그램을 마칩니다.");
    system("stty -raw");
    system("stty echo");
}
    			
