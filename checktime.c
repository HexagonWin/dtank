/*  �� ���α׷��� 1994�� 8���� ���¿� ���ؼ� �ۼ��Ǳ� �����ؼ�
    1994�� 12���濡 ���� 1.0�� �ϼ��Ǿ����ϴ�. 

    ����ũ ���� �Խ��� ���α׷�.

    �� ���α׷��� ���۱��� ���¿��� �ֽ��ϴ�. 
                                ������: hommage
                                ������: hommage
                                ���ͳ�: hommage@power3.snu.ac.kr
                                        s_scsc@cd4680.snu.ac.kr

    �� ���α׷��� ������б� ������к��� ���ڰԽ����� ���ؼ� 
    �ۼ��Ǿ����ϴ�.

    �� ���α׷��� ���� ����Ʈ������ �� ������, ä��Ƽ�����Դϴ�.
    ; ����� FSF���� �߰��� GNU ���� �����Ʒ� �� ���α׷��� ������ ��
      �ֽ��ϴ�.
    
    �� ���α׷��� ���� ������ �������� �����մϴ�.
    ����, ����� �ٸ� ���۷����� �ý��������� �����̳�, ����� ������
    ����ϰ��� �Ѵٸ�, �������� ���ǿ� ����� ���� �մϴ�.

    ����� �� ���α׷��� �Բ� DOLTANK.txt�� �����޾Ҿ�� �մϴ�. ����
    �׷����� �ʴٸ�, 
    ����� ������к� ���ڰԽ��� ��ڿ��� �˷��ֽʽÿ�.

                                        ���� ���Ǳ� �Ÿ���
                                        ������б� ������к�
                                        ���ڰԽ��� ���...
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
    sprintf(tty,"tmp/STAT.%s",tmp);  /* �̿����� ���¸� ���ڷ� �Ǵ��ϴ� ȭ��*/
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
	    if(fp1==NULL) c=3;  /* ȭ���� �������ٸ� ������ �ɰ��մϴ�. */
            fclose(fp1);
            if(c == '1' || c=='4') {   /* �ٿ�,�� ���� ���̴�. �ٿ� ���϶���
				�ð� üũ�� ���� �ʾƾ߰�����.?*/
                time(&i);
            }
            else if(c == '3') {  /* ���� �߸��� ���� ����. ��� ���� */
		ps =getppid();
		kill(ps,SIGHUP);		
                exit(0);
            }
            else {    /* �̰��� ���� ����� ����. 0�϶��̴�. */
                sleep(20);
                fstat(0, &statbuf);
                time(&current);
                if(current - (statbuf.st_atime + i) >= TIME) {
                    ps = getppid();
                    printf("\r\n\007Ű�Է��� ��� �ڵ����� �������ϴ�.\r\n");
                    kill(ps, SIGHUP);
                    exit(0);
                }
                i = 0;
            }
            sleep(1);
        }
    }
}

