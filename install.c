/* �̰��� �ν��� ���α׷��Դϴ�. */

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
	printf("\r\n ���� ���� �Ͻ� �̴ϱ�?");
	printf("\r\n (1) ���׷��̵� (2) �ν��� (0) ���");
	printf("\r\n ���>> ");keyinput(buf,2);
	if(buf[0]=='\0') {
	    printf("\r\n ����� ����־��ּ���.");
	}
	else if(buf[0]=='0') {
	    printf("\r\n �ڵ��ν����ɿ��� �����ϴ�.");
	    break;
	}
	else if(buf[0]=='1') {
	    printf("\r\n ���׷��̵带 �����ϰڽ��ϴ�.");
	    printf("\r\n ���׷��̵��� ��񿡽����丮���� �����ּ���");
	    printf("\r\n (e.g. /home/bbs) >> "); keyinput(upgradedir,30);
	    if(upgradedir[0]=='\0'){
		printf("\r\n ���丮���� ����־���մϴ�.");
	   	break;
	    }
	    sprintf(buf,"update.sh %s",upgradedir);
	    system("stty -raw");
	    system(buf);
	    system("stty raw");
	    printf("\r\n ���׷��̵尡 �������ϴ�.");
	}
	else if(buf[0]=='2') {
	    printf("\r\n �ν����� �����ϰڽ��ϴ�. ");
	    printf("\r\n **** ���� *****");
	    printf("\r\n �ν����� �����ϸ� ��񿡽� �޴�ȭ�ϱ��� ���������ϴ�");
	    printf("\r\n ������ ��񿡽��� ������� ���� ������(y/N) ");
	    keyinput(buf,2);
	    if(buf[0] !='y' && buf[0] !='Y') break;
	    printf("\r\n �ν����Ͻ� ��񿡽����丮�� �����ּ���.");
	    printf("\r\n (e.g. /home/bbs) >> "); keyinput(installdir,30);
	    if(installdir[0]=='\0') {
		printf("\r\n ���丮���� ����־���մϴ�.");
	    	break;
	    }
	    sprintf(buf,"install.sh %s",installdir);
	    system(buf);
	    printf("\r\n �ν����� �������ϴ�.");
	}
    }
    printf("\r\n ���α׷��� ��Ĩ�ϴ�.");
    system("stty -raw");
    system("stty echo");
}
    			
