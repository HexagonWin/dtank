
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
	  == NULL) {   /* �޸� Ȯ�� */
            perror("\r\n�޸𸮰� ����");
            host_end();
        }
        i = 0;
        while(fread((char*)&work,sizeof(struct nowwork),1,fp1)) {
            if(strcmp(work.tty,tty)) {  /* ���ɳ�� ���� */
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
        free(chk_work); /* Ȯ�� �޸� ���� */
    }
    while(1) {
        printf(\
"\r\n �ű԰����� ���ϴ� ���� �̿��ڹ�ȣ�� '�մ�(guest)'�� �Է��Ͻʽÿ�. ");
        printf("\r\n �̿��ڹ�ȣ : ");
        lineinput(id,8,YEA);
        if((!strcmp(id,"�մ�")) || \
	  (!strcmp(id,"guest")) || (!strcmp(id,"GUEST"))) {  
            ioctl(0, TCSETAF, &systerm);
            resultcode=guestregister();
            if(resultcode==0) return 1; /* guest=1���� ����� ������ */
            else {
		perror("\r\n �������α׷��� �۵����� �ʽ��ϴ� ");
		host_end();
	    }
        }
        else {
            if(strlen(id) > 2) {
                i = 1; 
                if((fp1 = fopen("tmp/WHOISNOW","r")) != NULL) {
                    while(fread((char*)&work,sizeof(struct nowwork),1,fp1)) {
                        if(!strcmp(id,work.id)) {  
                            printf("\r\n���� ����ϰ� �ֽ��ϴ�.");
                            printf("\r\n%s���� ������Դϴ�.",work.tty);
                        }
                    }
                } 
		fclose(fp1);
                if(i) {
                    i = 1; /* ȸ���� �ֳ� üũ */
		    fclose(fp1);
                    if((fp1 = fopen("bin/PASSWDS","r")) == NULL) {
                        printf("\r\n���̵� �˻�ȭ���� ������ �ʽ��ϴ�.\r\n");
                        host_end();
                    }
                    if(i) {
			while(fread((char*)&pf,sizeof(struct idst),1,fp1)) {
                            if(!strcmp(id,pf.id)) { /* ���̵� �˻� */
                                printf("\r\n %s���� �Ϲ�ȸ���̽ʴϴ�.",pf.id);
                                i = 0;
                                fos = ftell(fp1) - sizeof(struct idst);
                                guestcheck=0;
                                break;
                            }
                        }
		    }
                    fclose(fp1);
                    fp1=fopen("bin/PASSWDS.TMP","r");
                    if(guestcheck && i && fp1!=NULL) { /* ��ȸ���� �ƴϰ� */
			while(fread((char*)&pf,sizeof(struct idst),1,fp1)) {
			    if(!strcmp(id,pf.id)) {
				printf("\r\n ����� ���� ���Խ�û ó�����Դϴ�.");
				i=0;
			 	guestcheck=1; /* �մ��ڰ� */	
				fos = ftell(fp1) - sizeof(struct idst);
			    }
			}
		    } 
		    fclose(fp1);
                    if(i) {
                        printf("\r\n�ش�ȸ���� �������� �ʽ��ϴ�.");
                        ++logfail;
                    }
                    else {  /* ��й�ȣ �Է� */
                        i = 0;
                        len = 9;
                        printf("\r\n ���  ��ȣ : ");
			lineinput(passwd,10,NA);
                        if(strcmp(pf.passwd,passwd)) {  /* ��й�ȣ �˻� */
                            printf("\r\n��й�ȣ�� Ʋ���ϴ�.");
                            ++logfail;
                        }
                        else if(pf.level < 0) { /* ����ڷ����� �����϶� */
                            printf("\r\n�˼��մϴٸ� ��ڿ� �����Ͻʽÿ�.\r\n");

                            sleep(1);
                            host_end();
                        }
                        else {  /* ���� ���̵� �Է½� */
                            printf("\r\n\n %s�� �ݰ����ϴ�.",pf.name);
                            tp = localtime(&pf.logout);
                            printf("\r\n\n ��ȸ LOGOUT : 19%02d/%02d/%02d  %02d:%02d:%02d",
                              tp->tm_year,tp->tm_mon+1,tp->tm_mday,tp->tm_hour,tp->tm_min,tp->tm_sec);
                            sprintf(work.id,"%s",pf.id);    /* �۾����� ��� */
                            pf.logout = buftime;
			    time(&t);
                            tp = localtime(&t);
                            printf("\r\n ���� LOGIN  : 19%02d/%02d/%02d  %02d:%02d:%02d",
                              tp->tm_year,tp->tm_mon+1,tp->tm_mday,tp->tm_hour,tp->tm_min,tp->tm_sec);
                            sprintf(buf,"%02d.%02d(+)",tp->tm_mon+1,tp->tm_mday);
                            tmp = &pf.birthday[3];
                            if(!strcmp(tmp,buf)) {  /* ��� ���� �˻� */
                                logo("birthday.log",1);   /* ���Ͻÿ� ������ �ΰ� */
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
                                printf("\r\n������ ȭ���� ������ �ʽ��ϴ�.");
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
            if(logfail > 4) {   /* ���̵� �Է� 4�� ����� ���� ���� */

                printf("\r\nȸ�����̵�� ��й�ȣ�� Ȯ���� �ֽʽÿ�.\r\n");
                host_end();
            }
        }
    }
    return guestcheck; /* �̰��� �մ��ΰ� ��ȸ���ΰ��� �����Ѵ�. */ 
}

