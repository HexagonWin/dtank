/* ���� ����� �޴��Դϴ�. */
/* ��� �Լ� */

#include "bbs.h"

void id_select(),pf_change(),display_profile(),id_sort();
void file_sort(),dis_exp(),name_search(),list_sort();
void chk_handcap();

struct idst bpf;  /* �̰����� ���� pf buffer�̴�. */

sysop()
{
    int i, j, key;
    char buf[80], tmp[10], ch;
    FILE *fp1;
    struct termio tbuf;
    ioctl(0,TCGETA, &systerm);
    rawmode();
    umask(0111);
    fclose(fp1);
    printf("\r\n\n ������ȣ : ");
    lineinput(buf,10,NA);
    if(strcmp(cfg.secret,buf)) {
        printf("\r\n����� ��ڰ� �ƴմϴ�.");
        for(i = 0; i < 10; i++) {
            printf("\007");
        }
        host_end();
    }
    mtop=nmenu=menu;
    key = 1;
    while(1) {
        if(key) {
            printf("\r\n     -------------------------------");
	    printf("\r\n     | �̰��� ���ۿ�ڸ޴��Դϴ� |");
	    printf("\r\n     -------------------------------");
            printf("\r\n");
	    printf("\r\n 1. ����ȸ���� ���̵� �߱����ݴϴ�.");
	    printf("\r\n 2. ���̵� �����մϴ�.");
	    printf("\r\n 3. LOGFILE�� �����մϴ�.");
	    printf("\r\n 4. ȸ������(ȸ�����������׺���)");
	    printf("\r\n 5. �Խù�����(����)");
	    printf("\r\n 6. �˴ٿ� ���� ");
	    printf("\r\n 7. ������ ����");
	    printf("\r\n 8. �޴�����");
	    printf("\r\n 9. ���������ֱ�");
	    printf("\r\n d. ����� �����");
	    printf("\r\n p. ���ۿ�� �޴����� ������.");
            printf("\r\n ");
	    printf("\r\n SuperMaintain>> ");
        }
        else {
            i = strlen(buf);
            for(j = 0; j < i; j++) {
                printf("\b \b");
            }
            key = 1;
        }
        keyinput(buf,1);
        buf[0] = tolower(buf[0]);
        if(buf[0] == '1') {
            id_select();
        }
        else if(buf[0] == '2') {
            id_sort();
        }
        else if(buf[0] == '3') {
            showLOGFILE();
        }
        else if(buf[0] == '4') {
                profilemanager();

        }
	else if(buf[0] =='5') {
	    /* �Խù� ���� �ڵ带 ���������.*/
		;
	}
        else if(buf[0] == '6') {
            shutdownmessage();
        }
        else if(buf[0] == '7') {
            letter_sort();
        }
        else if(buf[0] == '8') {
            Make_Menu(0);
        }
	else if(buf[0] =='9') {
	    show_user("bin/PASSWDS");
	}
	else if(buf[0]=='d') {
	    printf("\r\n ����� ����ھ��̵�>> ");keyinput(buf,10);
	    del_user("bin/PASSWDS",buf);
	}

        else if(buf[0] == 'p') {
            break;
        }
        else key = 0;
    }
    Menu_Free(); /* �ϴ� �޴��� �ٽ� �����Ѵ����� */
    Get_Menu(); /* �ٽ� �޴��� �д´� �׷��� �α׾ƿ�����
			�ʰ� �ٲ� �޴��� Ȯ���� ��
			�ִ�. */
    mtop=nmenu=menu;
}


void id_select()
{
    int i, j, count;
    char buf[80];
    FILE *fp1, *fp2;
    struct tm *tp, *localtime();
    printf("\033[;H\033[2J");
    if((fp1 = fopen("bin/PASSWDS.TMP","r")) == NULL) {
        printf("\r\n�����ڰ� �����ϴ�.");
    }
    else {
        if((fp2 = fopen("bin/PASSWDS","r+")) == NULL) {
            printf("\r\nó�� ������");
            fp2 = fopen("bin/PASSWDS","a");
            count = 0;
        }
        else {
            if(fseek(fp2,-sizeof(struct idst),SEEK_END)) {
                printf("\r\n���̵� ȭ�� ������ ���� ����");
                count = 0;
            }
            else {
                fread((char*)&bpf,sizeof(struct idst),1,fp2);
                count = bpf.num + 1;
            }
        }
        i = 1;
        while(fread((char*)&bpf,sizeof(struct idst),1,fp1)) {
            time(&bpf.inday);
            bpf.num = count;
            bpf.circle = 0;
            bpf.logcount = 0;
            bpf.totaltime = 0;
            bpf.level = 10;
            time(&bpf.lasttime);
            bpf.lasttime = bpf.lasttime + 3600 * 720;
            display_profile();
            printf("\r\n\n\r������ �޾��ֽðڽ��ϱ�(y/N)(P�� ���޴���)? ");
            keyinput(buf,1);
            if(buf[0] == 'y' || buf[0] == 'Y') {
                fseek(fp2,0,SEEK_END);
                fwrite((char*)&bpf,sizeof(struct idst),1,fp2);
                ++count;
            }
            else if(buf[0] == 'p') {
                i = 0;
                break;
            }
            else {
                printf("\r\n���� ���");
            }
        }
        fclose(fp2);
        fclose(fp1);
        if(i) unlink("bin/PASSWDS.TMP");
    }
}


void pf_change()
{
    int i, j, k, l, m;
    char buf[80];
    FILE *fp1;
    int filepos;

    printf("\r\nã�� ���̵� : ");
    keyinput(buf,8);
    if((fp1 = fopen("bin/PASSWDS","r+")) == NULL) {
        printf("\r\nID ȭ���� ������ �����ϴ�.");
        host_end();
    }
    i = 1;
    while(fread((char*)&bpf,sizeof(struct idst),1,fp1)) {
        if(!strcmp(buf,bpf.id)) {
            filepos = ftell(fp1) - sizeof(struct idst);
            display_profile();
            while(1) {
                printf("\r\n\r�ٲ� ������ �ֽ��ϱ�? (��ȣ/n) ");
                keyinput(buf,2);
                if(buf[0] == '1' && buf[1] == '\0') {
                    printf("\r\n## ��   ��   �� : ");
                    keyinput(bpf.id,8);
                }
                else if(buf[0] == '2' && buf[1] == '\0') {
                    printf("\r\n## �� ��  �� ȣ : ");
                    keyinput(bpf.passwd,8);
                }
                else if(buf[0] == '3' && buf[1] == '\0') {
                    printf("\r\n\r## ��        �� : ");
                    keyinput(bpf.name,8);
                }
                else if(buf[0] == '4' && buf[1] == '\0') {
                    printf("\r\n## ��        �� : ");
                    bpf.sex = getchar();
                }
                else if(buf[0] == '5' && buf[1] == '\0') {
                    printf("\r\n## �� ��  �� �� : ");
                    keyinput(bpf.birthday,11);
                }
                else if(buf[0] == '7' && buf[1] == '\0') {
                    printf("\r\n## ������������ : ");
                    time(&bpf.logout);
                }
                else if(buf[0] == '8' && buf[1] == '\0') {
                    printf("\r\n## �� ��  �� ȣ : ");
                    keyinput(bpf.post,8);
                }
                else if(buf[0] == '9' && buf[1] == '\0') {
                    printf("\r\n\r## ��   ��   �� : ");
                    keyinput(bpf.home_addr,79);
                }
                else if(buf[0] == '1' && buf[1] == '0' && buf[2] == '\0') {
                    printf("\r\n## ��   ��   ȭ : ");
                    keyinput(bpf.home_tel,14);
                }
                else if(buf[0] == '1' && buf[1] == '1' && buf[2] == '\0') {
                    printf("\r\n\r## ��   ��   �� : ");
                    keyinput(bpf.office_name,79);
                }
                else if(buf[0] == '1' && buf[1] == '2' && buf[2] == '\0') {
                    printf("\r\n## ������ȭ��ȣ : ");
                    keyinput(bpf.office_tel,14);
                }
                else if(buf[0] == '1' && buf[1] == '3' && buf[2] == '\0') {
                    printf("\r\n## �ֹε�Ϲ�ȣ : ");
                    keyinput(bpf.id_no,14);
                }
                else if(buf[0] == '1' && buf[1] == '4' && buf[2] == '\0') {
                    printf("\r\n## ��        �� : ");
                    keyinput(buf,10);
                    bpf.level = atoi(buf);
                }
                else if(buf[0] == '1' && buf[1] == '5' && buf[2] == '\0') {
                    printf("\r\n## ��   ��   �� : ");
                    keyinput(buf,10);
                    bpf.circle = atoi(buf);
                }
                else if(buf[0] == '2' && buf[1] == '0' && buf[2] == '\0') {
                    printf("\r\n## �� ��  �� �� : ");
                    keyinput(buf,3);
                    j = atoi(buf);
                    if(j) bpf.lasttime = time(&bpf.lasttime) + 3600 * 24 * j;
                }
                else if(buf[0] == 'n' || buf[0] == 'N') {
                    break;
                }
            }
            if(fseek(fp1,filepos,SEEK_SET)) {
                printf("\r\nȭ�� ������ ���� ����");
                host_end();
            }
            if(fwrite((char*)&bpf,sizeof(struct idst),1,fp1)) {
                printf("\r\nID�����Ǿ����ϴ�.");
            }
            else {
                printf("\r\nID���� ���� ");
            }
            display_profile();
            i = 0;
            break;
        }
        else {
            i = 1;
        }
    }
    fclose(fp1);
    if(i) {
        printf("\r\n%�ش�ȸ���� �����ϴ�.");
    }
    printf("\r\n[Enter]�� ġ�ʽÿ�.");
    keyinput(buf,1);
}

int shutdownmessage()
{
    int fout;
    char buf[80], tmp[80];
    struct nowwork mess;
    FILE *fp1;
    printf("\r\n���� �޼����� ���ʽÿ�.\r\n");
    while(1) {
        keyinput(tmp,60);
        if(tmp[0] != '\0') {
            sprintf(buf,"\033[s\0337\033[23;1H*�����޽���:\033[7m%s\033[0m\0338\033[u",tmp);
            if((fp1 = fopen("tmp/WHOISNOW","r")) != NULL) {
                while(fread((char*)&mess,sizeof(struct nowwork),1,fp1)) {
                    sprintf(tmp,"/dev/%s",mess.tty);
                    fout = open(tmp,1);
                    write(fout,buf,strlen(buf));
                    close(fout);
                }
            }
            fclose(fp1);
        }
        else {
            break;
        }
    }
}


pf_search()
{
    int i, j, flag, up, down;
    char *buf,*tmp;
    FILE *fp1, *fp2, *fp3;
    buf = tmp_buf;
    tmp = temp;
    printf("\r\n\n1. �� �� ��\r\n2. ��    ��\r\n3. ��    ��\r\n4. �Ϸù�ȣ\r\n\n���� >> ");
    keyinput(buf,1);
    if(buf[0] == '1') {
        flag = 1;
        printf("\r\n�� �� �� : ");
        keyinput(buf,8);
        i = strlen(buf);
    }
    else if(buf[0] == '2') {
        flag = 2;
        printf("\r\n\r��    �� : ");
        keyinput(buf,8);
        i = strlen(buf);
    }
    else if(buf[0] == '3') {
        flag = 3;
        printf("\r\n�������� : ");
        keyinput(buf,10);
        up = atoi(buf);
        printf("\r\n�������� : ");
        keyinput(buf,10);
        down = atoi(buf);
        if(down > up) {
            i = down;
            down = up;
            up = i;
        }
    }
    else if(buf[0] == '4') {
        flag = 4;
        printf("\r\n��    ȣ : ");
        keyinput(buf,8);
        i = atoi(buf);
    }
    else {
        return;
    }
    if((fp1 = fopen("bin/PASSWDS","r+")) == NULL) {
        printf("\r\nID ȭ���� ������ �����ϴ�.");
        host_end();
    }
    j = 0;
    while(fread((char*)&bpf,sizeof(struct idst),1,fp1)) {
        if(flag == 1) {
            if(!strncmp(buf,bpf.id,i)) {
                display_profile();
                j = 1;
            }
        }
        else if(flag == 2) {
            if(!strncmp(buf,bpf.name,i)) {
                display_profile();
                j = 1;
            }
        }
        else if(flag == 3) {
            if(down <= bpf.level && up >=bpf.level) {
                display_profile();
                j = 1;
            }
        }
        else if(flag == 4) {
            if(i == bpf.num) {
                display_profile();
                getchar();
                break;
            }
        }
        if(j) {
            j = 0;
            printf("\r\n���[Enter] ���� [P] : ");
            keyinput(tmp,4);
            if(tmp[0] == 'p' || temp[0] == 'P') {
                break;
            }
        }
    }
    fclose(fp1);
}

letter_sort()   /* ������ ���� */
{
    int i, j, num, max;
    char buf[80], tmp[40];
    time_t delti;
    FILE *fp1, *fp2, *fp3, *fp4;

    printf("\n\r���ϰ��� ������ �����Ͻðڽ��ϱ�(Enter�� 30��)? ");
    keyinput(buf,2);
    i = atoi(buf);
    if(i <= 0) i = 30;
    fp1 = fopen("bin/PASSWDS","r");
    fseek(fp1,0L,SEEK_END);
    max = ftell(fp1) / sizeof(struct idst);
    fclose(fp1);
    time(&delti);
    delti = delti - (i*24*3600);
    for(num = 0; num <= max; num++) {
        sprintf(buf,"letter/%05d.mail",num);
        if((fp1 = fopen(buf,"r")) == NULL);
        else {
            sprintf(buf,"tmp/%05d.mail",num);
            fp2 = fopen(buf,"w");
            sprintf(buf,"letter/%05d.txt",num);
            fp3 = fopen(buf,"r");
            sprintf(buf,"tmp/%05d.txt",num);
            fp4 = fopen(buf,"w");
            i = 0;
            while(fread((char*)&letter,sizeof(struct mail),1,fp1)) {
                if((letter.T != 'd') && (letter.tf > delti)) {
                    letter.num = ++i;
                    fseek(fp3,letter.point,SEEK_SET);
                    letter.point = ftell(fp4);
                    fwrite((char*)&letter,sizeof(struct mail),1,fp2);
                    for(j = 0; j < letter.size; j++) {
                        fputc(fgetc(fp3),fp4);
                    }
                }
            }
            fclose(fp4);
            fclose(fp3);
            fclose(fp2);
            fclose(fp1);
            sprintf(buf,"tmp/%05d.mail",num);
            sprintf(tmp,"letter/%05d.mail",num);
            rename(buf,tmp);
            sprintf(buf,"tmp/%05d.txt",num);
            sprintf(tmp,"letter/%05d.txt",num);
            rename(buf,tmp);
        }
        sprintf(buf,"letter/%05d.chk",num);
        if((fp1 = fopen(buf,"r")) == NULL);
        else {
            sprintf(tmp,"tmp/%05d.chk",num);
            fp2 = fopen(tmp,"w");
            while(fread((char*)&mail_chk,sizeof(struct mail_dis),1,fp1)) {
                if(mail_chk.tf > delti) {
                    fwrite((char*)&mail_chk,sizeof(struct mail_dis),1,fp2);
                }
            }
            fclose(fp2);
            fclose(fp1);
            rename(tmp,buf);
        }
    }
}

void id_sort()
{
    int i;
    char buf[80], olddata[40], newdata[40];
    FILE *fp1, *fp2;
    time_t delti;
    printf("\n\r���ϰ��� �����ڸ� ����ðڽ��ϱ�(Enter�� ���)? ");
    keyinput(buf,3);
    i = atoi(buf);
    if(i > 0) {
        time(&delti);
        delti = delti - (i*24*3600);
        fp1 = fopen("bin/PASSWDS","r");
        fp2 = fopen("tmp/PASSWDS.NEW","w");
        while(fread((char*)&bpf,sizeof(struct idst),1,fp1)) {
            if((bpf.level >= 0) && (bpf.logout >= delti)) {
                fwrite((char*)&bpf,sizeof(struct idst),1,fp2);
            }
            else {
                sprintf(olddata,"letter/%05d.mail",bpf.num);
                unlink(olddata);
                sprintf(olddata,"letter/%05d.txt",bpf.num);
                unlink(olddata);
                sprintf(olddata,"letter/%05d.chk",bpf.num);
                unlink(olddata);
            }
        }
        fclose(fp2);
        fclose(fp1);
        sprintf(olddata,"bin/PASSWDS");
        sprintf(newdata,"bin/PASSWDS.ORG");
        rename(olddata,newdata);
        sprintf(newdata,"tmp/PASSWDS.NEW");
        rename(newdata,olddata);
    }
}

void display_profile()    /* ȸ�� ���� ��ȸ */
{
    char buf[2];
    struct tm *tp, *localtime();
    printf("\r\n*  0. �� ��  �� ȣ : %05d",bpf.num);
    printf("\r\n   1. ��   ��   �� : %s",bpf.id);
    printf("\r\n*  2. �� ��  �� ȣ : %s",bpf.passwd);
    printf("\r\n   3. ��        �� : %s",bpf.name);
    if(bpf.flag & 0x0001) printf("\r\n   4.");
    else printf("\r\n*  4.");
    printf(" ��        �� : %c",pf.sex);
    if(bpf.flag & 0x0002) printf("\r\n   5.");
    else printf("\r\n*  5.");
    printf(" �� ��  �� �� : %s",bpf.birthday);
    tp = localtime(&bpf.inday);
    if(bpf.flag & 0x0004) printf("\r\n   6.");
    else printf("\r\n*  6.");
    printf(" �� ��  �� �� : %2d�� %02d�� %02d��",tp->tm_year,(tp->tm_mon)+1,tp->tm_mday);
    tp = localtime(&bpf.logout);
    if(bpf.flag & 0x0008) printf("\r\n   7.");
    else printf("\r\n*  7.");
    printf(" ������������ : 19%2d-%02d-%02d  %02d:%02d:%02d",
      tp->tm_year,tp->tm_mon+1,tp->tm_mday,tp->tm_hour,tp->tm_min,tp->tm_sec);
    if(bpf.flag & 0x0010) printf("\r\n   8.");
    else printf("\r\n*  8.");
    printf(" �� ��  �� ȣ : %s",bpf.post);
    if(bpf.flag & 0x0020) printf("\r\n   9.");
    else printf("\r\n*  9.");
    printf(" ��   ��   �� : %s",bpf.home_addr);
    if(bpf.flag & 0x0040) printf("\r\n  10.");
    else printf("\r\n* 10.");
    printf(" ��   ��   ȭ : %s",bpf.home_tel);
    if(bpf.flag & 0x0080) printf("\r\n  11.");
    else printf("\r\n* 11.");
    printf(" ��   ��   �� : %s",bpf.office_name);
    if(bpf.flag & 0x0100) printf("\r\n  12.");
    else printf("\r\n* 12.");
    printf(" ������ȭ��ȣ : %s",bpf.office_tel);
    if(bpf.flag & 0x0200) printf("\r\n  13.");
    else printf("\r\n* 13.");
    printf(" �ֹε�Ϲ�ȣ : %s",bpf.id_no);
    if(bpf.flag & 0x0400) printf("\r\n  14.");
    else printf("\r\n* 14.");
    printf(" ��        �� : %d",bpf.level);

    printf("\r\n  15. ��   ��   �� : %x",bpf.circle);
    printf("\r\n  16. �� ��  �� �� : ");
    if(bpf.flag & 0x1000) printf("���");
    else printf("����");
    printf("\r\n* 17. ȸ��  �÷��� : %x",bpf.expflag);
    printf("\r\n* 18. �� ��  Ƚ �� : %d",bpf.logcount);
    printf("\r\n* 19. �� �̿� �ð� : %d",bpf.totaltime);
    tp = localtime(&bpf.lasttime);
    printf("\r\n* 20. �� ��  �� �� : 19%2d-%02d-%02d  %02d:%02d:%02d",
      tp->tm_year,tp->tm_mon+1,tp->tm_mday,tp->tm_hour,tp->tm_min,tp->tm_sec);
}
showLOGFILE()
{

    FILE *fp1;
    char buf[80];
    char data[10];
    long int end;

    
    printf("\r\n\r\n ��¥�� �Է��� �ּ���(yymmdd)? >> ");
    keyinput(buf,6);
    fp1 = fopen("logfile/LOGFILE","r");
    fseek(fp1,0,2); end= ftell(fp1);fseek(fp1,0,0);
    if(buf[0]=='\0') printf("\r\n ��� ");
    else {
	while(1) {
	    fscanf(fp1,"%s",&data);
	    printf("\r\n %s",data);
	    fscanf(fp1,"%s",&data);
	    printf(" %s",data);
	    fscanf(fp1,"%s",&data);
	    printf(" %s",data);
	    fscanf(fp1,"%s",&data);
	    printf(" %s",data);
	    if( ftell(fp1) >= end) break;
	}
    }
}
profilemanager()
{
    int i,num,howmany;
    char buf[80];
    FILE *fp1;
    struct dis_list spf;
    while(1) {
        printf("\r\n /* ����� ȸ�� ���� �޴��Դϴ�.*/ ");
        printf(    "\r\n\r\n 2. ȸ�� ���� ����");
        printf(    "\r\n >> ");
        keyinput(buf,3);
        if(buf[0] =='1') {
	    printf("\r\n\r\n /* � ������� ã�ڽ��ϱ�?");
	    printf("\r\n  1. ���̵�");
	    printf("\r\n  2. ���Լ�");
	    printf("\r\n  3. ��  ��");
	    printf("\r\n  >> ");
	    keyinput(buf,3);
	    fp1 = fopen("bin/PASSWDS","r");
	    fseek(fp1,0,2); howmany = ftell(fp1)/sizeof(struct dis_list);
	    fseek(fp1,0,0);printf("\r\n ��ȸ���� %d���Դϴ�.",howmany);
	    if (buf[0] =='1') {
	        printf("\r\n ���̵� �Է����ּ���.");keyinput(buf,10);
	        for(i = 0; i<howmany;i++) {
	      	    fread((char*)&spf,sizeof(struct dis_list),1,fp1);
	  	    if(!strcmp(spf.id,buf)) {
		        display_profile(spf);
		    }
	        }
	    }
	    else if(buf[0] =='2') {
	        printf("\r\n ���Լ����� �����ּ���. ���ķ� ��� ��µ˴ϴ�.");
	        printf("\r\n ���Լ���: ");keyinput(buf,10);
	        num = atoi(buf);
	        fseek(fp1,(num-1)*sizeof(struct dis_list),0);
	        for (i = num-1; i<howmany; i++) {
                    fread((char*)&spf,sizeof(struct dis_list),1,fp1);
		    display_profile(spf);
	            printf("\r\n ����Ͻðڽ��ϱ�(Y/n"); keyinput(buf,1);
		    if(buf[0] =='n' || buf[0] =='N') break;
	        }
	    }
	    else if(buf[0]=='3') {
	        printf("\r\n ã���� �ϴ� ���� �̸��� �Է����ּ���.");
	        printf("\r\n �̸�: ");keyinput(buf,10);
	        for (i=0;i<howmany;i++) {
	  	    fread((char*)&spf,sizeof(struct dis_list),1,fp1);
		    if(!strcmp(spf.name,buf)) {
		        display_profile(spf);
		    }
                }
       	    }
            else printf("\r\n ���ÿ����Դϴ�.");
        }
        else if(buf[0] =='2') {
        	pf_change();
        }
	else {
	    printf("\r\n �����ϴ�.");
	    break;
	}
    }
}
/* �̰��� ���� ������ ���� ���ۿ�� �κ��̴�. */
supermanager()
{
    char buf[80];

    cls();
    while(1) {
        printf("\r\n (%s)����ȸ�������޴��Դϴ�.");
        printf("\r\n ---------------------------");
        printf("\r\n 1. ���Ƹ�����ó��");
        printf("\r\n 2. Ż  ��  ó  ��");
        printf("\r\n 3. ȸ  ��  ��  ȸ");
        printf("\r\n 4. ȸ���鿡�� ����������");
        printf("\r\n ���>> ");
        keyinput(buf,10);
        if(buf[0]=='p' || buf[0]=='P') break;
        else if(buf[0]=='1') {
            printf("\r\n ����ó�� �޴��Դϴ�.");
        }
        else if(buf[0] =='2') {
            printf("\r\n Ż��ó���޴��Դϴ�.");
        }
        else if(buf[0]=='3') {
            printf("\r\n ȸ���� ��ȸ�մϴ�. ");
            show_user("bin/PASSWDS");
        }
        else if(buf[0]=='4') {
            printf("\r\n ȸ���鿡�� ������ �����ϴ�.");
            printf("\r\n ���� �������� �ʽ��ϴ�.");
            printf("\r\n");
        }   
    }
}

show_user(char* what)
{
    FILE *fp1;
    char buf[80],whom[10];
    int i,j=0,howmany,num;
    struct idst member;
    while(1) {
        printf("\r\n 0.���� 1.���Լ����� 2.���̵�� 3.�̸����� >>");
        keyinput(buf,10);
        if(buf[0]=='1') {
	    sprintf(buf,"%s",what);
	    fp1 = fopen(buf,"r");
	    fseek(fp1,0,2); howmany = ftell(fp1)/sizeof(struct idst);
	    fclose(fp1);
	    list_user(what,0,10);
	    num = 0;
	    while(1) {
 		printf("\r\n ���: f:���� b:���� 0:����>> ");
		keyinput(buf,50);
		i = atoi(buf);
		if(i) {
		    if(i>0 && i<howmany) {
			num = i;
			list_user(what,num,10);
		    }
		}
	    	else if(!strncmp(buf,"dd",2)) {
		    int dnum;
		    parser(buf);
		    dnum=atoi(argument[1]); 
		    del_nthuser("bin/PASSWDS",dnum);
		}
		else if(buf[0]=='f') {
		    num = num+10;
		    if (num-9>howmany ) num = howmany-10;
		    list_user(what,num,10);
		}
		else if (buf[0]=='b') {
		    num = num -10;
		    if(num< 0 ) num = 0;
		    list_user(what,num,10);
		}
		else if(buf[0]=='0') {
		    break;
		}
	    }
        }
        else if(buf[0]=='2') {
            printf("\r\nã���÷��¾��̵�: ");keyinput(whom,10);
            sprintf(buf,"%s",what);
            fp1=fopen(buf,"r");
            fseek(fp1,0,2); howmany = ftell(fp1)/sizeof(struct idst);
            fseek(fp1,0,0);
            for(i=0;i<howmany;i++) {
                fread((char*)&member,sizeof(struct idst),1,fp1);
                if(!strcmp(whom,member.id)) {
                    printf("\r\n %s�� �����Դϴ�.",member.id);
                    printf("\r\n ���̵�:%s, �̸�:%s, ����:%s",\
                      member.id,member.name,member.birthday);

                }
            }
            fclose(fp1);
        }
        else if(buf[0]=='3') {
            printf("\r\n��ȸ���̸�: ");keyinput(whom,10);
            sprintf(buf,"%s",what);
            fp1=fopen(buf,"r");
            fseek(fp1,0,2); howmany = ftell(fp1)/sizeof(struct idst);
            fseek(fp1,0,0);
            for(i=0;i<howmany;i++) {
                fread((char*)&member,sizeof(struct idst),1,fp1);
                if(!strcmp(whom,member.name)) {
                    printf("\r\n %s�� �����Դϴ�.",member.id);
                    printf("\r\n ���̵�:%s, �̸�:%s, ����:%s",\
                      member.id,member.name,member.birthday);
                }
            }
            fclose(fp1);
        }
        else if(buf[0]=='0') {
            printf("\r\n �����ϴ�.");printf("\r\n");
            break;
        }
    }
}

list_user(char*whatfile,int num,int listnum)
{
    FILE *fp1;
    char buf[80];
    int i,howmany,end;
    struct idst member;
 
    sprintf(buf,"%s",whatfile);
    fp1 = fopen(buf,"r");
    fseek(fp1,0,2); howmany = ftell(fp1)/sizeof(struct idst);
    fseek(fp1,0,0);
    if(howmany < num+listnum) end = howmany;
    else end = num+listnum;

    for(i=num; i< end;i++) {
	fseek(fp1,i*sizeof(struct idst),0);
	fread((char*)&member,sizeof(struct idst),1,fp1);
	printf("\r\n %5d %5d ���̵�: %8s, �̸�: %4s, ����:%c ����: %13s",\
	  i,member.num,member.id,member.name,member.sex,member.birthday);
    }
    fclose(fp1);

}
/* ���̵𺰷� �̿��ڸ� �����ϴ� ���̴�. ���ڵ������ ����� ���� ���� �Լ� */
del_user(char*whatfile,char*user)
{
    FILE *fp1;
    char buf[80],buf2[80];
    int i,howmany;
    struct idst *memberlist;

    sprintf(buf,"%s",whatfile);
    fp1 = fopen(buf,"r");
    fseek(fp1,0,2); howmany = ftell(fp1)/sizeof(struct idst);
    printf("\r\n �Ѱ����ڼ��� %d",howmany);
    if((memberlist = (struct idst *)calloc(howmany,sizeof(struct idst))) == \
      NULL) {
	printf("\r\n �޸� ����");pressreturn();
	exit(1);
    }
    fseek(fp1,0,0);
    for (i=0;i<howmany;i++) {
	fread((char*)&memberlist[i],sizeof(struct idst),1,fp1);
	printf("\r\n %s",memberlist[i].id);
    }
    fclose(fp1);
    sprintf(buf,"%s",whatfile);
    sprintf(buf2,"%s.old",whatfile);
    rename(buf,buf2);
    fp1 = fopen(buf,"a+");
    fseek(fp1,0,0);
    for (i=0;i<howmany;i++) {
	if(!strcmp(memberlist[i].id,user)) {
	    printf("\r\n %s�� �����߽��ϴ�.",memberlist[i].id);
	}
	else {
	    fwrite((char*)&memberlist[i],sizeof(struct idst),1,fp1);
	}
    }
    fclose(fp1);
    printf("\r\n ������ ���ƽ��ϴ�."); pressreturn();
    free(memberlist);
}

del_nthuser(char*whatfile,int numN)
{
    FILE *fp1;
    char buf[80],buf2[80];
    int i,howmany;
    struct idst *memberlist;
    struct idst test;

    sprintf(buf,"%s",whatfile);
    fp1 = fopen(buf,"r");
    fseek(fp1,0,2); howmany = ftell(fp1)/sizeof(struct idst);
    printf("\r\n �Ѱ����ڼ��� %d",howmany);
    if((memberlist = (struct idst *)calloc(howmany,sizeof(struct idst))) == \
      NULL) {
        printf("\r\n �޸� ����");pressreturn();
        exit(1);
    }
    fseek(fp1,0,0);
    for (i=0;i<howmany;i++) {
        fread((char*)&memberlist[i],sizeof(struct idst),1,fp1);
        printf("\r\n %s",memberlist[i].id);
    }
    fseek(fp1,numN*sizeof(struct idst),0);
    fread((char*)&test,sizeof(struct idst),1,fp1);
    printf("\r\n %d��° %s�� ����÷��� ���� �½��ϱ�?(Y/n)",numN,test.id);
    keyinput(buf,1); 
    if(buf[0]=='n' ||buf[0]=='N') {
	printf("\r\n ����Ͽ����ϴ�."); pressreturn(); return;
    }
    fclose(fp1);
    sprintf(buf,"%s",whatfile);
    sprintf(buf2,"%s.old",whatfile);
    rename(buf,buf2);
    fp1 = fopen(buf,"a+");
    fseek(fp1,0,0);
    for (i=0;i<howmany;i++) {
	fseek(fp1,i*sizeof(struct idst),0);
        if(i==numN) {
            printf("\r\n %d ��° %s�� �����߽��ϴ�.",i,memberlist[i].id);
        }
        else {
            fwrite((char*)&memberlist[i],sizeof(struct idst),1,fp1);
        }
    }
    fclose(fp1);
    printf("\r\n ������ ���ƽ��ϴ�."); pressreturn();
    free(memberlist);
}
