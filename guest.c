
#include "bbs.h"

struct idst pf, tmp_pf;

guestregister()
{
    int i, j, k, flag, a[14],count;
    char buf[80],ti[14];
    FILE *fp1;
    signal(SIGQUIT, SIG_IGN);
    signal(SIGINT, SIG_IGN);
    signal(SIGTERM, SIG_IGN);
    signal(SIGHUP, (__sighandler_t)host_end);
    signal(SIGSEGV, (__sighandler_t)host_end);
    signal(SIGBUS, (__sighandler_t)host_end);
    ioctl(0, TCGETA, &systerm);
    rawmode();
    umask(0111);
    logo("guest.log",1);
    printf("\033[;H\033[2J");
    pf.circle = 0x00;
    pf.flag = 0xffffffff;
    flag = 0xffffffff;
    while(1) {
        count = 5;
        while(flag & 0x0001) {
            if(!count) host_end();
            --count;
            printf("\r\n����ϴ� ���̵� ������(�ѱ۵� �ſ�).");
            printf("\r\n--3~8�ڷ� �ϼ���.--");
            printf("\n\r���̵� : ");
            lineinput(pf.id,8,YEA);
            if(strlen(pf.id) > 2) {
                flag &= 0xfffffffe;
                if((fp1 = fopen("bin/PASSWDS","r")) != NULL) {
                    while(fread((char*)&tmp_pf,sizeof(struct idst),1,fp1)) {
                        if(!strcmp(tmp_pf.id,pf.id)) {
                            printf("\r\n'%s'�� �ٸ������ ��û�Ͽ� ������� ���̵� �Դϴ�.",tmp_pf.id);
                            flag |= 0x00000001;
                            break;
                        }
                    }
                }
		fclose(fp1);
                if((fp1=fopen("bin/PASSWDS.TMP","r")) !=NULL) {
		    while(fread((char*)&tmp_pf,sizeof(struct idst),1,fp1)) {
		 	if(!strcmp(tmp_pf.id,pf.id)) {
			    printf("\r\n'%s'�� �ٸ������ ���Խ�û�� ���̵��Դϴ�.",tmp_pf.id);
			    flag |=0x00000001;
			    break;
			}
		    }
		}
		fclose(fp1);
            }
        }
        while(flag & 0x0002) {
            pf.passwd[0] = '\0';
            while(strlen(pf.passwd) < 3) {
                printf("\r\n*����ϴ� ��й�ȣ�� ���ʽÿ�(3~8��).");
                printf("\r\n*��й�ȣ : ");
                lineinput(pf.passwd,8,YEA);
            }
            flag &= 0xfffffffd;
	    if(!strcmp(pf.passwd,"DOLTANK")) flag &= 0x00000000;
            break;
        }
        while(flag & 0x0004) {
            pf.name[0] = '\0';
            printf("\r\n*�̸��� ���ּ���.(��Ȯ��)");
            while(strlen(pf.name) < 4) {
                printf("\n\r�̸� : ");
                lineinput(pf.name,8,YEA);
            }
            flag &= 0xfffffffb;
            break;
        }
        while(flag & 0x0008) {
            printf("\r\n ��.�� ����(���� : m / ���� : f) >> ");
            pf.sex = getchar();
            pf.sex = tolower(pf.sex);
            if((pf.sex == 'm') || (pf.sex == 'f')) {
                printf("%c",pf.sex);
                flag &= 0xfffffff7;
                break;
            }
        }
        while(flag & 0x0010) {
            printf("\r\n*��������� �Է��Ͽ� �ֽʽÿ�. ��) 67.05.28 ");
            printf("\r\n*������� : ");
            lineinput(pf.birthday,8,YEA);
            pf.birthday[2] = '0';
            pf.birthday[5] = '0';
            for(i = 0; i < 8; i++) {
                if(!(isdigit(pf.birthday[i]))) {
                    printf("\r\n ��Ȯ�� �Է��Ͽ� �ֽʽÿ�.");
                    flag |= 0x0010;
                    break;
                }
                else {
                    flag &= 0xffffffef;
                }
            }
            if(!(flag & 0x0010)) {
                printf("\r\n\r ����Դϱ�(Y/n)? ");
                lineinput(buf,1,YEA);
                if(buf[0] == 'n' || buf[0] == 'N') {
                    pf.birthday[9] = '-';
                }
                else {
                    pf.birthday[9] = '+';
                }
                pf.birthday[2] = '.';
                pf.birthday[5] = '.';
                pf.birthday[8] = '(';
                pf.birthday[10] = ')';
                pf.birthday[11] = 0x00;
            }
        }
        while(flag & 0x0020) {
            pf.post[0] = '\0';
            while(!atoi(pf.post) || (strlen(pf.post) < 7)) {
                printf("\r\n �����ȣ(��) : ");
                lineinput(pf.post,7,YEA);
            }
            flag &= 0xffffffdf;
            break;
        }
        while(flag & 0x0040) {
            printf("\r\n ���ּҸ� �Է��� �ּ���.");
            printf("\r\n ���ּ� : ");
            lineinput(pf.home_addr,79,YEA);
            if(pf.home_addr[0] == 0x00) {
                printf("\r\n ��Ȯ�� �Է��Ͽ� �ֽʽÿ�.");
                flag |= 0x0040;
            }
            else {
                flag &= 0xffffffbf;
            }
        }

        while(flag & 0x0080) {
            printf("\r\n ����ȭ��ȣ�� �����ּ���. ��) 02-293-5346");
            printf("\r\n ����ȭ : ");
            lineinput(pf.home_tel,14,YEA);
            j = strlen(pf.home_tel);
            for(i = 0; i < j; i++) {
                if(!isdigit(pf.home_tel[i])) {
                    if(pf.home_tel[i] != '-') {
                        flag |= 0x0080;
                        break;
                    }
                    else {
                        flag &= 0xffffff7f;
                   }
                }
            }
        }
        while(flag & 0x0100) {
            printf("\r\n �б�,��,�й��� ���ּ���.(�Ϲ����� ����).");
            printf("\r\n (�б�,��,�й�) : ");
            lineinput(pf.office_name,79,YEA);
            flag &= 0xfffffeff;
            break;
        }
        while(flag & 0x0200) {
            printf("\r\n �б� ����ó�� �����ּ���. ��) ������(���Ƹ�) �Ǵ� ������ȭ��ȣ");
            printf("\r\n �б�����ó : ");
            lineinput(pf.office_tel,14,YEA);
            flag &= 0xfffffdff;
            break;
        }
        count = 5;
        while(flag & 0x0400) {
            if(!count) host_end();
            --count;
            printf("\r\n �ֹε�Ϲ�ȣ�� �Է��Ͽ� �ֽʽÿ�. ��) 670528-1234567");
            printf("\r\n �ֹε�Ϲ�ȣ : ");
            lineinput(pf.id_no,14,YEA);
            pf.id_no[6] = '0';
            j = 1;
            for(i = 0; i < 14; i++) {
                if(isdigit(pf.id_no[i])) a[i] = pf.id_no[i] - 48;
                else {
                    printf("\r\n*****��Ȯ�� �Է��Ͽ� �ֽʽÿ�.");
                    flag |= 0x0400;
                    j = 0;
                    break;
                }
            }
            if(j) {
    j = a[0]*2+a[1]*3+a[2]*4+a[3]*5+a[4]*6+a[5]*7+a[7]*8+a[8]*9+a[9]*2+a[10]*3+a[11]*4+a[12]*5;
                j = j % 11;
                k = 11 - j;
                if(k > 9) k = k % 10;
                j = pf.id_no[13] - 48;
                j=k;           /* �� ������ �ֹ� ��� �˻縦 ���� �ʰ� ���� ���̴�. �˻縦 ���ؼ� �����϶� */
		if(j != k) {                   
                    printf("\r\n��ȣ�� Ʋ�Ƚ��ϴ�.");
                    flag |= 0x0400;
                }
                else {
                    pf.id_no[6] = '-';
                    if((fp1 = fopen("bin/PASSWDS","r")) != NULL) {
                        while(fread((char*)&tmp_pf,sizeof(struct idst),1,fp1)) {
                            if(!strcmp(tmp_pf.id_no,pf.id_no)) {
                                printf("\007\r\n������ ��ȣ�� ������Դϴ�.");
                                flag |= 0x0400;
                                break;
                            }
                            else {
                                flag &= 0xfffffbff;
                            }
                        }
                        fclose(fp1);
                    }
                    else {
                        flag &= 0xfffffbff;
			fclose(fp1);
                        break;
                    }
                }                         
            }
        }
        printf("\r\n��û ����\r\n");
        printf("\r\n 1 ��   ��   �� : %s\r\n 2 �� ��  �� ȣ : %s",pf.id,pf.passwd);
        printf("\r\n 3 ��        �� : %s\r\n 4 ��        �� : %c",pf.name,pf.sex);
        printf("\r\n 5 �� ��  �� �� : %s\r\n 6 �� ��  �� ȣ : %s",pf.birthday,pf.post);
        printf("\r\n 7 ��   ��   �� : %s\r\n 8 ��   ��   ȭ : %s",pf.home_addr,pf.home_tel);
        printf("\r\n 9 ��   ��   �� : %s\r\n10 �� �� ����ó : %s",pf.office_name,pf.office_tel);
        printf("\r\n11 �ֹε�Ϲ�ȣ : %s\r\n",pf.id_no);
        printf("\r\n\n\r��ĥ ������ �ֽ��ϱ� ��ȣ/n ? ");
        lineinput(buf,2,YEA);
        if((buf[0] == 'n') || (buf[0] == 'N')) break;
        else {
            i = atoi(buf);
            switch(i) {
                case 1 : flag = 0x0001; break;
                case 2 : flag = 0x0002; break;
                case 3 : flag = 0x0004; break;
                case 4 : flag = 0x0008; break;
                case 5 : flag = 0x0010; break;
                case 6 : flag = 0x0020; break;
                case 7 : flag = 0x0040; break;
                case 8 : flag = 0x0080; break;
                case 9 : flag = 0x0100; break;
                case 10 : flag = 0x0200; break;
                case 11 : flag = 0x0400; break;
                default : flag = 0x0000;
            }
        }
    }
    time(&pf.inday);
    pf.logout = pf.inday;
    pf.level = 1;
    pf.logcount = 0;
    pf.totaltime = 0;
    logo("guestend.log",1);
    lineinput(buf,1,YEA);
    fp1 = fopen("bin/PASSWDS.TMP","a");
    fos = i = ftell(fp1);
    pf.num = i /sizeof(struct idst);
    fwrite((char*)&pf,sizeof(struct idst),1,fp1);
    fclose(fp1);
    if((fp1 = fopen("bin/DOLTANK.CFG","r")) == NULL) {  /* ���ذ� �б� */
        printf("\r\nbin/DOLTANK.CFG ����");pressreturn();
        host_end();
    }
    fscanf(fp1,"%s%d%d%d%d%d%s%d",buf,&j,&j,&j,&j,&j,buf,&k);
    fclose(fp1);
    writeLOGFILE(3); /* ���Խ�û�� ���� LOGFILE�� ��ִ´�. */
    if(k & 0x04) { /* 3�� ��Ʈ�� 1�̸� �մԸ޴����� ���θ޴��� */
        sprintf(buf,"%d",i);
        sprintf(ti,"%d",pf.num);
        ioctl(0, TCSETAF, &mbuf);
        return 0;
    }

    printf("\r\n ���Խ�û�� 24�ð� �Ŀ� ����Ͻ� �� �ֽ��ϴ�.");
    printf("\r\n �ȳ��� ������...                            ");
    host_end();
}
