
#include "bbs.h"

myinfo()    /* ���� ȸ�� ���� */
{
    int i, j, k, select, back, key=1;
    char buf[80], ch;
    FILE *fp1;
    struct idst pfpf; /* ȸ�� �������� ���� ȸ���������� */

    if(guest) {  /* �մ��� ��쿡�� */
	printf("\r\n �մ��� �ڱ������� ������ �� �����ϴ�.");
	printf("\r\n �üٿ��� ���� ��û�ϼ���. ");
	nmenu=nmenu->upp;	
	reprint=YEA;
    }
    fp1 = fopen("bin/PASSWDS","r");  
    if(fseek(fp1,pf_fos,0)) { /* pf_fos�� ��ü���� ���� ���� */
        printf("\r\n�б� ����");
        bye();
    }
    fread((char*)&pfpf,sizeof(struct idst),1,fp1);
    if(strcmp(pfpf.id,pf.id)) {
        printf("\r\nID�� Ʋ���ϴ�");
        bye();
    }
    fclose(fp1);
    my_pf();  /* ���� ������ �����ش�. */
    while(1) {
        if(!key) {
            for(i = 0; i < back; i++) {
                putchar('\b'); putchar(' '); putchar('\b');
            }
        }
        key = 1;
        keyinput(buf,20);
        back = strlen(buf);
        if(buf[0] == '1' && buf[1] == '\0') {
            change_info();  /* ������ �ٲ۴�. */
        }
        else if(buf[0] == '2' && buf[1] == '\0') {
            change_flag();   /* �÷��� �ٲ۴� */
        }
        else if(buf[0] == 'h' || buf[0] == 'H') {
            logo("abchelp.hlp");
        }
        else if(buf[0] == 'p' || buf[0] == 'P') {

            if((fp1 = fopen("bin/PASSWDS","r+")) == NULL) {
                printf("\r\nPASSWDS ����");
            }
            fseek(fp1,pf_fos,SEEK_SET);
            fwrite((char*)&pfpf,sizeof(struct idst),1,fp1);
            fclose(fp1);
            break;
        }
        else if((buf[0] == 'g' || buf[0] == 'G') && (buf[1] == 'o' || buf[1] == 'O')) {
            if(!go_select(buf)) {
                mess_dis(1);
                key = 0;
            }
            else {
                if((fp1 = fopen("bin/PASSWDS","r+")) == NULL) {
                    printf("\r\nPASSWDS ����");
                }
                fseek(fp1,pf_fos,SEEK_SET);
                fwrite((char*)&pfpf,sizeof(struct idst),1,fp1);
                fclose(fp1);
                break;
            }
        }
        else if(buf[0] == 'x' || buf[0] == 'X') {   /* ���α׷� ���� */
            printf("\r\n���Ḧ �Ͻðڽ��ϱ�(Y/n)? ");
            keyinput(buf,1);
            if(!(buf[0] == 'n' || buf[0] == 'N')) {
                if((fp1 = fopen("bin/PASSWDS","r+")) == NULL) {
                    printf("\r\n������ ȭ���� ������ �ʽ��ϴ�.");
                }
                fseek(fp1,pf_fos,SEEK_SET);
                fwrite((char*)&pfpf,sizeof(struct idst),1,fp1);
                fclose(fp1);
                end_flag = 1;
                host_end();
            }
            my_pf();
            key = 1;
        }
        else { key = 0;}
        messagecheck();

        sprintf(buf,"tmp/mail.%s",pf.id);
        if((fp1 = fopen(buf,"r")) != NULL) {
            cls();
            while((ch = fgetc(fp1)) != EOF) {

                putchar(ch);
            }
            fclose(fp1);
            unlink(buf);
            printf("\r\n\n����Ű�� ġ�ʽÿ�.");
            keyinput(buf,1);
            key = 1;
            my_pf();
        }
    }
}

void my_pf() /* ���� ���� ǥ�� */
{
    int i, j;
    char buf[2];
    struct tm *tp, *localtime();
    printf("   �ڱ��������� : abc");
    printf("\r\n\n    1 ��   ��   �� : %s",pf.id);
    printf("\r\n *  2 �� ��  �� ȣ : ");
    j = strlen(pf.passwd);
    for(i = 0; i < j; i++) putchar('*');
    printf("\r\n    3 ��        �� : %s",pf.name);
    if(pf.flag & 0x0001) printf("\r\n   "); else printf("\r\n * ");
    printf(" 4 ��        �� : %c",pf.sex);
    if(pf.flag & 0x0002) printf("\r\n   "); else printf("\r\n * ");
    printf(" 5 �� ��  �� �� : %s",pf.birthday);
    if(pf.flag & 0x0004) printf("\r\n   "); else printf("\r\n * ");
    tp = localtime(&pf.inday);
    printf(" 6 �� ��  �� �� : %2d�� %02d�� %02d��",tp->tm_year,tp->tm_mon+1,tp->tm_mday);
    if(pf.flag & 0x0008) printf("\r\n   "); else printf("\r\n * ");
    tp = localtime(&pf.logout);
    printf(" 7 ������������ : %2d�� %02d�� %02d�� %02d�� %02d��",tp->tm_year,
    tp->tm_mon+1,tp->tm_mday,tp->tm_hour,tp->tm_min);
    if(pf.flag & 0x0010) printf("\r\n   "); else printf("\r\n * ");
    printf(" 8 �� ��  �� ȣ : %s",pf.post);
    if(pf.flag & 0x0020) printf("\r\n   "); else printf("\r\n * ");
    printf(" 9 ��   ��   �� : %s",pf.home_addr);
    if(pf.flag & 0x0040) printf("\r\n   "); else printf("\r\n * ");
    printf("10 ��   ��   ȭ : %s",pf.home_tel);
    if(pf.flag & 0x0080) printf("\r\n   "); else printf("\r\n * ");
    printf("11 ��   ��   �� : %s",pf.office_name);
    if(pf.flag & 0x0100) printf("\r\n   "); else printf("\r\n * ");
    printf("12 �� �� ����ó : %s",pf.office_tel);
    if(pf.flag & 0x0200) printf("\r\n   "); else printf("\r\n * ");
    printf("13 �ֹε�Ϲ�ȣ : %s",pf.id_no);
    if(pf.flag & 0x0400) printf("\r\n   "); else printf("\r\n * ");
    printf("14 ��        �� : %d\r\n   15 �� ��  �� �� : ",pf.level);
    if(pf.flag & 0x1000) printf("���\r\n");
    else printf("����\r\n");
    printf(" * 16 ��   ��   �� : %x\r\n",pf.circle);
    printf(" * 17 �� ��  Ƚ �� : %dȸ\r\n",pf.logcount);
    printf(" * 18 �� �̿� �ð� : %d��\r\n",pf.totaltime);
    tp = localtime(&pf.lasttime);
    printf(" * 19 �� ��  �� �� : 19%2d�� %02d�� %02d�� %02d�� %02d��",tp->tm_year,

    tp->tm_mon+1,tp->tm_mday,tp->tm_hour,tp->tm_min);
    printf("\r\n\n'*'ǥ�ð� �ִ°��� ������Դϴ�.");
    printf("\r\n��������[1] ����/����� ����[2] �����޴�[P] ����[X] ����޴�[GO]\r\n���� >> ");
}

void change_info()   /* ���� ���� ���� */
{
    int i, j, k, l, a[14];
    char buf[80], tmp[80], ch,passwd[9];
    FILE *fp1;
    struct idst changepf;

    while(1) {
        printf("\r\n\r������ ���� ��ȣ/N ? ");
        keyinput(buf,2);
        if(buf[0] == '2' && buf[1] == '\0') {
            printf("\r\n������ ��й�ȣ : ");
            get_passwd(buf,8);
            if(!strcmp(buf,pf.passwd)) {
                printf("\r\n���ο� ��й�ȣ �Է� : ");
                get_passwd(buf,8);
                printf("\r\n���ο� ��й�ȣ Ȯ�� : ");
                get_passwd(passwd,8);
                if(!strcmp(buf,passwd)) {
                    sprintf(pf.passwd,"%s",passwd);
                }
                else {

                    printf("\r\n��й�ȣ ������ ��� �Ǿ����ϴ�.");
                }
            }
            else {
                printf("\r\n��й�ȣ�� Ʋ���ϴ�.");
            }
        }
        else if(buf[0] == '5' && buf[1] == '\0') {
            i = 1;
            while(i) {
                printf("\r\n������� : ");
                keyinput(tmp,8);
                tmp[2] = '0';
                tmp[5] = '0';
                for(j = 0; j < 8; j++) {
                    if(!(isdigit(tmp[j]))) {
                        printf("\r\n��Ȯ�� �Է��Ͽ� �ֽʽÿ�.");
                        i = 1;
                        break;
                    }
                    else i = 0;
                }

                if(!i) {
                    printf("\r\n\r����Դϱ� Y/n ? ");
                    keyinput(buf,1);
                    if(buf[0] == 'n' || buf[0] == 'N') {
                        pf.birthday[9] = '-';
                    }
                    else {
                        tmp[9] = '+';
                    }
                    tmp[2] = '.';
                    tmp[5] = '.';
                    tmp[8] = '(';
                    tmp[10] = ')';
                    tmp[11] = 0x00;
                    sprintf(pf.birthday,"%s",tmp);
                }
            }
        }
        else if(buf[0] == '8' && buf[1] == '\0') {
            printf("\r\n�����ȣ : ");
            keyinput(pf.post,7);

            pf.post[3] = '-';
        }
        else if(buf[0] == '9' && buf[1] == '\0') {
            i = 1;
            while(i) {
                printf("\r\n\r���ּ� : ");
                keyinput(pf.home_addr,79);
                if(pf.home_addr[0] == 0x00) {
                    printf("\r\n��Ȯ�� �Է��Ͽ� �ֽʽÿ�.");
                    i = 1;
                }
                else i = 0;
            }
        }
        else if(buf[0] == '1' && buf[1] == '0') {
            i = 1;
            while(i) {
                printf("\r\n����ȭ : ");
                keyinput(pf.home_tel,14);
                j = strlen(pf.home_tel);
                for(k = 0; k < j; k++) {
                    if(!isdigit(pf.home_tel[k])) {

                        if(pf.home_tel[k] != '-') {
                            i = 1;
                            break;
                        }
                        else i = 0;
                    }
                }
            }
        }
        else if(buf[0] == '1' && buf[1] == '1' && buf[2] == '\0') {
            printf("\r\n\r����� : ");
            keyinput(pf.office_name,79);
        }
        else if(buf[0] == '1' && buf[1] == '2' && buf[2] == '\0') {
            printf("\r\n������ȭ : ");
            keyinput(pf.office_tel,14);
        }
        else if(buf[0] == '1' && buf[1] == '3' && buf[2] == '\0') {
            l = 1;
            while(l) {
                l = 0;
                printf("\r\n�ֹε�Ϲ�ȣ : ");
                keyinput(pf.id_no,14);
                pf.id_no[6] = '0';
                j = 1;
                for(i = 0; i < 14; i++) {
                    if(isdigit(pf.id_no[i])) a[i] = pf.id_no[i] - 48;
                    else {
                        printf("\r\n��Ȯ�� �Է��Ͽ� �ֽʽÿ�.");
                        j = 0; l = 1;
                        break;
                    }
                }
                if(j) {
        j = a[0]*2+a[1]*3+a[2]*4+a[3]*5+a[4]*6+a[5]*7+a[7]*8+a[8]*9+a[9]*2+a[10]*3+a[11]*4+a[12]*5;
                    j = j % 11;
                    k = 11 - j;
                    if(k > 9) k = k % 10;
                    j = pf.id_no[13] - 48;
                    if(j != k) {
                        printf("\r\n��ȣ�� Ʋ�Ƚ��ϴ�.");
                        l = 1;
                    }
                    else {
                        l = 0;
                        pf.id_no[6] = '-';
                        break;
                    }
                }
            }
        }
        else if(buf[0] == '1' && buf[1] == '5' && buf[2] == '\0') {
            if(pf.flag & 0x00001000) {
                pf.flag &= 0xffffefff;
                printf("\r\n����");
            }
            else {
                pf.flag |= 0x00001000;
                printf("\r\n���");
            }
        }
        else if(buf[0] == 'n' || buf[0] == 'N' || buf[0] == '\0') {
            my_pf();
            break;
        }
        else {
            printf("\r\n�ش��ȣ�� �����ų �� �����ϴ�.\r\n");
        }
    }
    printf("\r\n �ٲ������� �����Ͻðڽ��ϱ�?(y/N)>> ");
    keyinput(buf,5);
    if(buf[0]=='y' || buf[0]=='Y') {
    	fp1 = fopen("bin/PASSWDS","r+");
	if(fseek(fp1,pf_fos,0)) {
	    printf("\r\n �б� ����");
	    bye();
   	}
	fread((char*)&changepf,sizeof(struct idst),1,fp1);
	if(strcmp(changepf.id,pf.id)) {
	    printf("\r\n ���̵� �ٸ��ϴ�(�ɰ��� ����)");
	    bye();
	}
	fwrite((char*)&pf,sizeof(struct idst),1,fp1);
	fclose(fp1);
    }

}

void change_flag()   /* ���� ����� ���� */
{
    char ch[3];
    while(1) {
        printf("\r\n �ٲٽ� ����(��ȣ/N)? ");
        keyinput(ch,2);
        if(ch[0] == '4' && ch[1] == '\0') {
            printf("\r\n���� ");
            if(pf.flag & 0x00000001) {
                pf.flag &= 0xfffffffe;
                printf("�����");
            }
            else {
                pf.flag |= 0x00000001;
                printf("����");
            }
        }
        else if(ch[0] == '5' && ch[1] == '\0') {
            printf("\r\n������� ");
            if(pf.flag & 0x00000002) {
                pf.flag &= 0xfffffffd;
                printf("�����");
            }

            else {
                pf.flag |= 0x00000002;
                printf("����");
            }
        }
        else if(ch[0] == '6' && ch[1] == '\0') {
            printf("\r\n�������� ");
            if(pf.flag & 0x00000004) {
                pf.flag &= 0xfffffffb;
                printf("�����");
            }
            else {
                pf.flag |= 0x00000004;
                printf("����");
            }
        }
        else if(ch[0] == '7' && ch[1] == '\0') {
            printf("\r\n������������ ");
            if(pf.flag & 0x00000008) {
                pf.flag &= 0xfffffff7;
                printf("�����");
            }
            else {
                pf.flag |= 0x00000008;
                printf("����");
            }
        }
        else if(ch[0] == '8' && ch[1] == '\0') {
            printf("\r\n�����ȣ ");
            if(pf.flag & 0x00000010) {
                pf.flag &= 0xffffffef;
                printf("�����");
            }
            else {
                pf.flag |= 0x00000010;
                printf("����");
            }
        }
        else if(ch[0] == '9' && ch[1] == '\0') {
            printf("\r\n���ּ� ");
            if(pf.flag & 0x00000020) {
                pf.flag &= 0xffffffdf;
                printf("�����");
            }
            else {

                pf.flag |= 0x00000020;
                printf("����");
            }
        }
        else if(ch[0] == '1' && ch[1] == '0' && ch[2] == '\0') {
            printf("\r\n����ȭ ");
            if(pf.flag & 0x00000040) {
                pf.flag &= 0xffffffbf;
                printf("�����");
            }
            else {
                pf.flag |= 0x00000040;
                printf("����");
            }
        }
        else if(ch[0] == '1' && ch[1] == '1' && ch[2] == '\0') {
            printf("\r\n����� ");
            if(pf.flag & 0x00000080) {
                pf.flag &= 0xffffff7f;

                printf("�����");
            }
            else {
                pf.flag |= 0x00000080;
                printf("����");
            }
        }
        else if(ch[0] == '1' && ch[1] == '2' && ch[2] == '\0') {
            printf("\r\n������ȭ ");
            if(pf.flag & 0x00000100) {
                pf.flag &= 0xfffffeff;
                printf("�����");
            }
            else {
                pf.flag |= 0x00000100;
                printf("����");
            }
        }
        else if(ch[0] == '1' && ch[1] == '3' && ch[2] == '\0') {

            printf("\r\n�ֹε�Ϲ�ȣ ");
            if(pf.flag & 0x00000200) {
                pf.flag &= 0xfffffdff;
                printf("�����");
            }
            else {
                pf.flag |= 0x00000200;
                printf("����");
            }
        }
        else if(ch[0] == '1' && ch[1] == '4' && ch[2] == '\0') {
            printf("\r\n��� ");
            if(pf.flag & 0x00000400) {
                pf.flag &= 0xfffffbff;
                printf("�����");
            }
            else {
                pf.flag |= 0x00000400;

                printf("����");
            }
        }
        else if(((ch[0] == 'n' || ch[0] == 'N') && ch[1] == '\0') || ch[0] == '\0') {
            my_pf();
            break;
        }
        else {
            printf("\r\n�ش��ȣ�� �����ų �� �����ϴ�.\r\n");
        }
    }
}

display_pf(struct idst whom)
{
    char buf[80];
    struct tm *tp, *localtime();
    printf("\r\n %s(%s) %s",whom.id,whom.name,whom.id_no);
    printf("\r\n ����(%c) %s",whom.sex,whom.office_name);
}
changepasswd()
{
    char buf[80], buf2[80];
    cls();
    printf("\r\n �н����带 �ٲߴϴ�.");
    printf("\r\n ������ �н����带 ����־��ּ���.");
    printf("\r\n ���� �н�����: ");keyinput(buf,10);
    if(!strcmp(pf.passwd,buf)) {
	printf("\r\n ���ο� �н�����: ");keyinput(buf,10);
	printf("\r\n �ٽ��ѹ� �־��ּ���: ");keyinput(buf2,10);
	if(!strcmp(buf,buf2)) {
	    change_passwd(pf,buf);
	    printf("\r\n �н����尡 �ٲ�����ϴ�.");pressreturn();
	}
	else {
	    printf("\r\n �����߽��ϴ�."); pressreturn();
	}
    }
    else {
	printf("\r\n �н����尡 Ʋ���ϴ�.");pressreturn();
    }
}
changeaddress()
{
    FILE *fp1;
    char buf[80], buf2[80];
    int fos;
    struct idst temp;
    cls();
    printf("\r\n �ּҸ� �ٲߴϴ�.");
    fp1 = fopen("bin/PASSWDS","r");
    fos=find_pfoffset("bin/PASSWDS",pf);
    fseek(fp1,fos,0);
    fread((char*)&temp,sizeof(struct idst),1,fp1);
    fclose(fp1);
    printf("\r\n ������ �ּ�: %s",temp.home_addr);
    printf("\r\n ����� �ּ�: "); keyinput(buf,80);
    if(buf[0]!='\0') {
        change_address(pf,buf);
        printf("\r\n �ּҰ� ����Ǿ����ϴ�.");pressreturn();
    }
    else { printf("\r\n ����ϼ̽��ϴ�."); pressreturn(); 
    }
}

change_passwd(struct idst whom,char *passwdchange)
{
    FILE *fp1;
    int fos;
    struct idst changepf;

    fos=find_pfoffset("bin/PASSWDS",whom);
    if((fp1= fopen("bin/PASSWDS","r+"))!=NULL) {
	fseek(fp1,fos,0);
    	fread((char*)&changepf,sizeof(struct idst),1,fp1);
     	sprintf(changepf.passwd,"%s",passwdchange);
  	fseek(fp1,fos,0);
	fwrite((char*)&changepf,sizeof(struct idst),1,fp1);
    }
}
change_address(struct idst whom,char *address)
{
    FILE *fp1;
    int fos;
    struct idst changepf;

    fos=find_pfoffset("bin/PASSWDS",whom);
    if((fp1= fopen("bin/PASSWDS","r+"))!=NULL) {
        fseek(fp1,fos,0);
        fread((char*)&changepf,sizeof(struct idst),1,fp1);
        sprintf(changepf.home_addr,"%s",address);
        fseek(fp1,fos,0);
        fwrite((char*)&changepf,sizeof(struct idst),1,fp1);
    }
}

find_pfoffset(char *passwdfile,struct idst whom)
{
    char buf[80];
    int i,fos,howmany;
    FILE *fp1;
    struct idst temp;

    if( (fp1=fopen(passwdfile,"r")) !=NULL) {
	fseek(fp1,0,2); howmany= ftell(fp1)/sizeof(struct idst);
    }
    else {
 	return -1;
    }
    for(i=0;i<howmany;i++) {
	fseek(fp1,i*sizeof(struct idst),0);
	fread((char*)&temp,sizeof(struct idst),1,fp1);
	if(!strcmp(temp.id,whom.id)) {
	    fos=i*sizeof(struct idst);
	    break;
	}
    }
    return fos;
}
  
