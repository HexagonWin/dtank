#include "bbs.h"
/* �̰��� �Ϲ� ���Ͽ����� ���� ���� ���̴�. */

wmail()
{ 
    FILE *fp1;
    char towhom[100];
    char direc[80];
    char buf[80];
    int check=0;
    struct mailist mailgroup;
    printf("\r\n ��������(WMAIL)");
    printf("\r\n �ּҷϿ� ��ϵ� ������� �������� @�ּҷ� ���ø� �ǰ�");
    printf("\r\n �������� �������� --> ���̵� ���̵� ���̵� ... ���ø� �˴ϴ�.");
    printf("\r\n ���̵�(��: hommage sysop)>> ");
    keyinput(towhom,100);
    if(towhom[0]=='@' ) {
  	sprintf(buf,"user/mail%s.mil",pf.id);
	fp1 = fopen(buf,"r");
	fseek(fp1,0,0);
	while(fread((char*)&mailgroup,sizeof(struct mailist),1,fp1)) {
	    if(!strcmp(mailgroup.group,towhom)) {
		sprintf(towhom,"%s",mailgroup.towhom);
		check = 1;
		break;
	    }
	}
	if (check==0) {
	    printf("\r\n �ּҷϿ� ������.");pressreturn();
	    return;
	}
    }
    strcpy(direc,"letter");
    wmailto(direc,towhom);
}
/* �̰��� ��������,�����̾����,��Ÿ���Ϸ� ���е� ������ ���� */
newwmail()
{
    char towhom[100];
    char direc[80];
    char buf[80];
    printf("\r\n ���� ��� ���̵�(��: hommage sysop)>> ");
    keyinput(towhom,100);
    while(1) {
        printf("\r\n ������ ������ ������(0:��� 1:order 2:inquiry 3.etc) ");
        keyinput(buf,1);
	if(buf[0]=='1') {
	    strcpy(direc,"oletter");
	    break;
	}
	if(buf[0]=='2') {
	    strcpy(direc,"qletter");
	    break;
	}
	else if(buf[0]=='3') {
	    strcpy(direc,"eletter");
	    break;
	}
	else if(buf[0]=='0') {
	    printf("\r\n ����ϼ̽��ϴ�.");
	    return;
	}
    }
    wmailto(direc,towhom);
}
/* �̰��� ���� �����б� */
rmail()
{
    char buf[80];
    char direc[80];
    strcpy(direc,"letter");
    sprintf(buf,"%s.mail",pf.id);
    show_mail(direc,buf,mailcmd);
}

/* �̰��� ������ ���� */
 
newrmail()
{
    char buf[80];
    char direc[80];
    while(1) {
    	printf("\r\n � ������ ���ðڽ��ϱ�(0:��� 1:order 2:inquiry 3:etc) ");
    	keyinput(buf,1);
	if(buf[0]=='1') {
	    strcpy(direc,"oletter");
	    break;
	}
	else if(buf[0]=='2') {
	    strcpy(direc,"qletter");
	    break;
	}
	else if(buf[0]=='3') {
	    strcpy(direc,"eletter");
	    break;
	}
	else if(buf[0]=='0') {
	    printf("\r\n ��Ҹ� �ϼ̽��ϴ�.");
	    return;
	}
    }
    sprintf(buf,"%s.mail",pf.id);
    show_mail(direc,buf,mailcmd);
}

        
/* ������ ������ �⺻���� cell function */
    
char mailee[30][20];
wmailto(char *direc, char *towhom)
{
    FILE *fp1;
    int howmany=0,i,num,check;
    char buf[255];
    struct idst userid;
    struct dis_list mailhead;  /* ���� ȭ��������� �����Ѵ� */
    time_t t; /* �̰��� �ð��� ���� �� ���� ����  */ 
    struct tm *tp, *localtime();
    struct mail_dis mailchk;
    char mailfilename[30];

    num=parser(towhom);
    for(i=0; i< num;i++) {
   	check=0;
 	fp1 = fopen("bin/PASSWDS","r");
	while(fread((char*)&userid,sizeof(struct idst),1,fp1)) {
	    if(!strcmp(argument[i],userid.id)) {
		sprintf(mailee[howmany],"%s",argument[i]);
		check=1; howmany++;
	    }
	}
        if(check==0) {
            printf("\r\n %s�� �����̿��ڰ� �ƴմϴ�.",argument[i]);
        }
        fclose(fp1);
    }
    if (howmany==0) {
	printf("\r\n ������ ���� ����� �����ϴ�.");
  	pressreturn(); return;
    }
    for(i=0;i<3;i++) {   
	printf("\r\n      +---------+---------+---------+");
	printf("\r\n ����: ");keyinput(buf,30);
	if(buf[0]=='\0') printf("\r\n ������ �Է��ϼ���.");
	else break;
    }	
    if(buf[0]=='\0') {
	printf("\r\n �������⸦ ����մϴ�. ");
	pressreturn(); reprint=YEA; return;
    }
    else sprintf(mailhead.title,"%s",buf);

    time(&t);    		
    sprintf(mailfilename,"%s%d.txt",pf.id,t);
    if(!strcmp(direc,"oletter")) { /* �ֹ� ������ ��쿡�� Ư���� ���� */
	check=writeorderform(direc,mailfilename);
    }
    else check=writetextfile(direc,mailfilename);
    if(!check) {
	printf("\r\n �������⸦ ����մϴ�.");
	pressreturn(); reprint=YEA;return;
    }
    for (i=0;i<howmany;i++) {
  	/* ���� ���ȭ�� �ۼ� */
	strcpy(mailhead.id,pf.id);
	strcpy(mailhead.name,pf.name);
	mailhead.date=t;
	mailhead.look=0;
 	sprintf(mailhead.bfilename,"%s%d.mtx",mailee[i],t);
	sprintf(buf,"cp %s/%s %s/%s",direc,mailfilename,direc,mailhead.bfilename);
	system(buf);

	sprintf(buf,"%s/%s",direc,mailhead.bfilename);
	mailhead.line=seehowmanyline(buf);
	mailhead.feel=0;
 
	sprintf(buf,"%s/%s.mail",direc,mailee[i]);
	if((fp1=fopen(buf,"r+"))==NULL) {
	    fp1 = fopen(buf,"a+");
	}
	fseek(fp1,0,2);
	num= ftell(fp1) / sizeof(struct dis_list) +1;
	mailhead.num=num;
	fwrite( (char*)&mailhead,sizeof(struct dis_list),1,fp1);
	fclose(fp1);
	printf("\r\n %s�Կ��� ������ ���½��ϴ�.",mailee[i]);
	sprintf(buf,"%s/%s.chk",direc,pf.id);
	if((fp1=fopen(buf,"r+"))==NULL) {
	    fp1=fopen(buf,"a+");
	}
	fseek(fp1,0,2);
	strcpy(mailchk.id,mailee[i]);
	strcpy(mailchk.title,mailhead.title);
	mailchk.date=0;
	mailchk.tf=t;
	fwrite((char*)&mailchk,sizeof(struct mail_dis),1,fp1);
	fclose(fp1);
    }
    sprintf(buf,"rm %s/%s",direc,mailfilename);
    system(buf);
    pressreturn();
}
    
/* cmail�� ������ ���� �ʿ��� �κ��̴�. ���߿� �����Ѵ� */
void cmail()  /* ���� ���� Ȯ�� */
{
    int i, top_num, back, key,start,howmany;
    char buf[80],direc[80],fhead[80];
    FILE *fp1;
    int LEN=15; /* ��µǴ� ����� ���� */
    sprintf(direc,"letter");
    sprintf(fhead,"%s.chk",pf.id);

    if(!check_allletter(direc,fhead)) {
	printf("\r\n ���� ������ �����ϴ�.");
	pressreturn();
	return;
    }
    sprintf(buf,"%s/%s",direc,fhead);
    fp1 = fopen(buf,"r");
    fseek(fp1,0,2); howmany = ftell(fp1) / sizeof(struct mail_dis);
 
    back = 0;
    key = 1;
    start = 1;
    while(1) {
        if(key) {
	    cls();
	    list_chk(direc,fhead,start,start+LEN); 
            printf("\r\n����,f:����ȭ�� b:����ȭ�� ����[P] �ʱ�[T] : ");
        }
        else {
            for(i = 0; i < back; i++) {
                putchar('\b'); putchar(' '); putchar('\b');
            }
        }
        keyinput(buf,25);
        back = strlen(buf);
        for(i = 0; i < back; i++) buf[i] = tolower(buf[i]);
        if(!strcmp(buf,"dir")) {
            key = 1;
            list_chk(direc,fhead,1,1+LEN);
        }
        else if(buf[0] == '\0' || buf[0]=='f' || buf[0]=='F') {
            if(howmany > start+LEN) {
                key = 1;
		start = start +LEN;
            }
            else {
                mess_dis(3);
                key = 0;
            }
        }
	else if(buf[0]=='b' || buf[0]=='B') {
	    if( (start-LEN)< 1) {
		key=1;start=1;
	    }
	    else {
		key=1;start=start-LEN;
	    }
	}
        else if(!strcmp(buf,"p")) {
            break;
        }
        else if(!strcmp(buf,"t")) {
            mtop = nmenu = menu;
            esc_sub = 0;
            esc_flag = 1;
            break;
        }
        else {
            key = 0;
        }
    }
}
newcmail() 
{
    printf("\r\n ���� �������� �ʽ��ϴ�.");
    pressreturn();
}

/* ������ �о��� �� ��� Ȯ���ؼ� ���ȭ�Ͽ� ����Ѵ� 
    ����üũ�� �� �� �ѹ��� �� ��ġ�� �ܰ� */
check_allletter(char *direc, char *fhead)
{
    FILE *fp1,*fp2;
    char buf[80];
    int check=0; 
    time_t t;
    struct mail_dis mailchk;
    struct dis_list mailhead;
    sprintf(buf,"%s/%s",direc,fhead);
    if((fp1=fopen(buf,"r+"))==NULL) {
	printf("\r\n üũ ����� �����ϴ�.");
 	pressreturn(); return 0;
    }
    fseek(fp1,0,0);
    while(fread((char*)&mailchk,sizeof(struct mail_dis),1,fp1)) {
	check=0;
	sprintf(buf,"%s/%s.mail",direc,mailchk.id);
	fp2=fopen(buf,"r");
 	while(fread((char*)&mailhead,sizeof(struct dis_list),1,fp2)) {
	    if(mailhead.date==mailchk.tf) {
		if(mailhead.look>0) {
		    time(&t);	    
		    mailchk.date=t;			
		    fseek(fp1,-sizeof(struct mail_dis),1);
		    fwrite((char*)&mailchk,sizeof(struct mail_dis),1,fp1);
	        }
		check=1;
	    }
	}    
	fclose(fp2);
    }
    fclose(fp1);
    if(check==1) return 1; /* ���������� ���� */
    else return 0;
}

/* Ȯ�� ����Ʈ */
list_chk(char *direc,char *fhead,int start, int end)
{
    FILE *fp1;
    char buf[80];
    int i,howmany;
    struct tm *tp, *localtime();
    struct mail_dis mailchk;

    sprintf(buf,"%s/%s",direc,fhead);
    if((fp1 = fopen(buf,"r"))==NULL) {
	printf("\r\n ����� �� �����ϴ�.");
	return 0;
    }
    fseek(fp1,0,2);howmany=ftell(fp1) / sizeof(struct mail_dis);
    fseek(fp1,0,0);
    printf("  ���̵�   ������¥  ���ſ���           ��      ��\r\n");
    for(i = start; i < end; i++) {
        if(i>howmany) {
            break;
        }
        else {
	    fseek(fp1,(i-1)*sizeof(struct mail_dis),0);
            fread((char*)&mailchk,sizeof(struct mail_dis),1,fp1);
            tp = localtime(&mailchk.tf);
            printf("\r\n %-8s  %2d��%2d��  ",mailchk.id,
            tp->tm_mon+1,tp->tm_mday);
            if(mailchk.date == 0) {
                printf("[������]%s",mailchk.title);
            }
            else {
                tp = localtime(&mailchk.date);
                printf("**����**%s",mailchk.title);
  /* %2d��%2d�� %s",tp->tm_mon+1,tp->tm_mday,mailchk.title); */
            }
        }
    }
    fclose(fp1);
    return i;  /* ������ ��ȣ�� �����Ѵ� */
}


dis_top()
{
    printf("");
    printf("%d�� %-9s %s\r\n\n",letter.num,letter.id,letter.title);
}

/* �ּҷ� �ۼ��̴� */
editmailist(char*cmd)
{
    FILE *fp1;
    char buf[80],buf2[80];
    struct mailist mailgroup;
    int fpos;

    sprintf(buf,"user/mail%s.mil",pf.id); /* ������ �ּҷ��� ����. */
    if((fp1 = fopen(buf,"r+"))==NULL) {
	fp1 = fopen(buf,"a+");
    }
    if(fp1==NULL) {
	printf("\r\n ������ �� �����ϴ�.");
	return;
    }
    while(1) {
	printf("\r\n ���ϸ�����Ʈ�޴��Դϴ�.");
	printf("\r\n �Ͻ����� ����ּ���. ");
	printf("\r\n (1) ���� (2) ����� (3) �����ֱ� (0)����������");
	printf("\r\n �Ͻ���>> ");
 	keyinput(buf,10);
	if(buf[0]=='1') {
	    printf("\r\n ������ �׷��� �����ּ���.>> ");
	    keyinput(buf,10);
	    fseek(fp1,0,0);
	    while(fread((char*)&mailgroup,sizeof(struct mailist),1,fp1)) {
		sprintf(buf2,"@%s",buf);
		if(ftell(fp1)) {
		    fpos = ftell(fp1) - sizeof(struct mailist);
		}
		else fpos = 0;

		if(!strcmp(buf2,mailgroup.group)) {
		    while(1) {
			printf("\r\n �����޴��Դϴ�.");
			printf("\r\n (1) �߰� (2) ���� (0) ������");
			printf("\r\n >> ");
			keyinput(buf,10);
			if(buf[0]=='1') {
			    if(strlen(mailgroup.towhom)>990) {
				printf("\r\n �ο��� �ʰ��߽��ϴ�. ");
				break;
			    }
			    printf("\r\n �߰��� �ο��� �����ּ���.");
			    printf("\r\n �߰�>> ");
			    keyinput(buf,80);
			    sprintf(buf2," %s",buf);
			    strcat(mailgroup.towhom,buf2);
			    fseek(fp1,fpos,0);
			    fwrite((char*)&mailgroup,sizeof(struct mailist),\
			      1,fp1);
			    printf("\r\n �߰��� �������ϴ�.");
			}
			if(buf[0]=='2') {
			    printf("\r\n ���� �������� �ʽ��ϴ�.");
			}
			if(buf[0]=='0') {
			    printf("\r\n �����ϴ�.");
			    break;
			}
		    }
		}
	    }
	    printf("\r\n �����޴����� �����ϴ�.");
	}
	else if(buf[0]=='2') {
	    while(1) {
	        printf("\r\n �ּҷ� �̸�-> ");
	        keyinput(buf,10);
		fseek(fp1,0,0);
		/* �ּҷϿ� �ִ� ���ΰ� Ȯ���Ѵ�. */
		while(fread((char*)&mailgroup,sizeof(struct mailist),1,fp1)) {
		    sprintf(buf2,"@%s",buf);
		    if(!strcmp(buf2,mailgroup.group)) {
			printf("\r\n �ּҷϿ� ���� �׷��� �ֽ��ϴ�.");
			printf("\r\n �ּҷϸ���⿡�� �����ϴ�.");
			pressreturn();
			return;
		    }
		}
	        if(buf[0]!='\0') {
		    sprintf(mailgroup.group,"@%s",buf);
		    printf("\r\n �ּҷϿ� �߰��� ����� ���ּ���.");
		    printf("\r\n (e.g hommage sysop) >> ");
		    keyinput(buf,80);
		    sprintf(mailgroup.towhom,"%s",buf);
		    fseek(fp1,0,2);
		    fwrite((char*)&mailgroup,sizeof(struct mailist),1,fp1);
		    printf("\r\n �ּҷϿ� �߰��߽��ϴ�.");
		    break;
		}
	        printf("\r\n �ּҷ� �Է��� �Ͻðڽ��ϱ�?(y/N) ");
	        keyinput(buf,3);
		if(buf[0]!='y' && buf[0]!='Y') {
		    break;
		}
	    }
	}
 	else if(buf[0]=='3') {
	    fseek(fp1,0,0);
	    while(fread((char*)&mailgroup,sizeof(struct mailist),1,fp1)) {
		printf("\r\n %s",mailgroup.group);
		printf(" -> %s", mailgroup.towhom);
	    }
	}
	else if(buf[0]=='0') {
	    printf("\r\n �����ϴ�.");
	    break;
	}
    }
    fclose(fp1);
    printf("\r\n �ּҷϰ������� �����ϴ�.");
    pressreturn();

}

editMailist(char*cmd)
{
    FILE *fp1;
    char buf[80],buf2[80];
    struct mailist mailgroup;
    int fpos;

    sprintf(buf,"user/MAILIST"); /* ��ü�ּҷ� ����. */

    if((fp1 = fopen(buf,"r+"))==NULL) {
        fp1 = fopen(buf,"a+");
    }
    if(fp1==NULL) {
        printf("\r\n ������ �� �����ϴ�.");
        return;
    }
    while(1) {
        printf("\r\n ���ϸ�����Ʈ�޴��Դϴ�.");
        printf("\r\n �Ͻ����� ����ּ���. ");
        printf("\r\n (1) ���� (2) ����� (3) �����ֱ� (0)����������");
        printf("\r\n �Ͻ���>> ");
        keyinput(buf,10);
        if(buf[0]=='1') {
            printf("\r\n ������ �׷��� �����ּ���.>> ");
            keyinput(buf,10);
            fseek(fp1,0,0);
            while(fread((char*)&mailgroup,sizeof(struct mailist),1,fp1)) {
                sprintf(buf2,"!%s",buf);
                if(ftell(fp1)) {
                    fpos = ftell(fp1) - sizeof(struct mailist);
                }
                else fpos = 0;

                if(!strcmp(buf2,mailgroup.group)) {

                    while(1) {
                        printf("\r\n �����޴��Դϴ�.");
                        printf("\r\n (1) �߰� (2) ���� (0) ������");
                        printf("\r\n >> ");
                        keyinput(buf,10);
                        if(buf[0]=='1') {
                            if(strlen(mailgroup.towhom)>990) {
                                printf("\r\n �ο��� �ʰ��߽��ϴ�. ");
                                break;
                            }
                            printf("\r\n �߰��� �ο��� �����ּ���.");
                            printf("\r\n �߰�>> ");
                            keyinput(buf,80);
                            sprintf(buf2," %s",buf);
                            strcat(mailgroup.towhom,buf2);
                            fseek(fp1,fpos,0);
                            fwrite((char*)&mailgroup,sizeof(struct mailist),\
                              1,fp1);
                            printf("\r\n �߰��� �������ϴ�.");
                        }
                        if(buf[0]=='2') {
                            printf("\r\n ���� �������� �ʽ��ϴ�.");
                        }
                        if(buf[0]=='0') {
                            printf("\r\n �����ϴ�.");
                            break;
                        }
                    }
                }
            }
            printf("\r\n �����޴����� �����ϴ�.");
        }
        else if(buf[0]=='2') {
            while(1) {
                printf("\r\n �ּҷ� �̸�-> ");
                keyinput(buf,10);
                fseek(fp1,0,0);
                /* �ּҷϿ� �ִ� ���ΰ� Ȯ���Ѵ�. */
                while(fread((char*)&mailgroup,sizeof(struct mailist),1,fp1)) {
                    sprintf(buf2,"!%s",buf);
                    if(!strcmp(buf2,mailgroup.group)) {
                        printf("\r\n �ּҷϿ� ���� �׷��� �ֽ��ϴ�.");
                        printf("\r\n �ּҷϸ���⿡�� �����ϴ�.");
                        pressreturn();
                        return;
                    }
                }
                if(buf[0]!='\0') {
                    sprintf(mailgroup.group,"@%s",buf);
                    printf("\r\n �ּҷϿ� �߰��� ����� ���ּ���.");
                    printf("\r\n (e.g hommage sysop) >> ");
                    keyinput(buf,80);
                    sprintf(mailgroup.towhom,"%s",buf);
                    fseek(fp1,0,2);
                    fwrite((char*)&mailgroup,sizeof(struct mailist),1,fp1);
                    printf("\r\n �ּҷϿ� �߰��߽��ϴ�.");
                    break;
                }
                printf("\r\n �ּҷ� �Է��� �Ͻðڽ��ϱ�?(y/N) ");
                keyinput(buf,3);
                if(buf[0]!='y' && buf[0]!='Y') {
                    break;
                }
            }
        }
        else if(buf[0]=='3') {
            fseek(fp1,0,0);
            while(fread((char*)&mailgroup,sizeof(struct mailist),1,fp1)) {
                printf("\r\n %s",mailgroup.group);
                printf(" -> %s", mailgroup.towhom);
            }
        }
        else if(buf[0]=='0') {
            printf("\r\n �����ϴ�.");
            break;
        }
    }
    fclose(fp1);
    printf("\r\n �ּҷϰ������� �����ϴ�.");
    pressreturn();

}


