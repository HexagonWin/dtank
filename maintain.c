
#include "bbs.h" 

void chk_csysop()
{
    int i;
    char buf1[30],buf2[30];
    FILE *fp1;
    if((fp1 = fopen("bin/CIRCLE","r")) == NULL);
    else {
        for(i = 0; i < 31; i++) {
            fscanf(fp1,"%s",buf1);  /* ��ȣȸ�̸��� �д´�.*/
	    fscanf(fp1,"%s",buf2);  /* ��ȣȸ���� �д´�.*/
            if(!strcmp(buf2,pf.id)) {
                cs = 1;
                cs <<= i;
                printf("\r\n%d��° ���Ƹ� %s�� �����̽ʴϴ�."\
,i+1,buf1);
                pressreturn();
                break;
            }
        }
        fclose(fp1);
    }
}

csysop()
{
    int i, j, k, l;
    char buf[80];
    FILE *fp1;

    if(cs == 0) {
        return;
    }
    else {
        i = cs;
        l = ~cs;
    }
    if(!(cs & nmenu->r)) {
	printf("\r\n �˼��մϴ�. ��ȣȸ ���常�� �� �� �־��.");
	pressreturn();reprint=YEA;
	return; /* �� ��ȣȸ �ü��� �ƴϸ� ���� */ 
    }
    while(1) {
        printf("\r\n ��ȣȸ ��� �޴�");
        printf("\r\n\r\n1. ȸ���� ��ȣȸ�� ���Խ����ּ���.\
\r\n2. ��ȣȸ���� ȸ���� ©���\
\r\n3. ȸ���� �޿� �����غ��ô�.");
        printf("\r\nP. �� ���� ������...\r\n� ���� �ұ��?: ");
        lineinput(buf,1,YEA);
        j = atoi(buf);
        if(buf[0] == '1' || buf[0] == '2') {
            printf("\r\n���̵� : ");
            lineinput(buf,8,YEA);

            if((fp1 = fopen("bin/PASSWDS","r+")) == NULL) {
                printf("\r\n���̵� ȭ���� ������ �ʽ��ϴ�.");
                return;
            }
            k = 1;
            while(fread((char*)&you_pf,sizeof(struct idst),1,fp1)) {
                if(!strcmp(you_pf.id,buf)) {
                    fos = ftell(fp1) - sizeof(struct idst);
                    if(j == 1) {
                        you_pf.circle |= i;
                        printf("\r\n%x",you_pf.circle);
                    }
                    else if(j == 2) {
                        you_pf.circle &= l;

                        printf("\r\n%x",you_pf.circle);
                    }
                    if(fseek(fp1,fos,SEEK_SET)) {
                        printf("\r\nȭ�� ������ ���� ����");
                    }
                    if(fwrite((char*)&you_pf,sizeof(struct idst),1,fp1)) {
                        printf("\r\n���� �Ǿ����ϴ�.");
                    }
                    k =0;
                    break;
                }
            }
            fclose(fp1);
            if(k) {
                printf("\r\n%sȸ���� �����ϴ�.",buf);
                buf[0] = getchar();
            }
        }
        else if(buf[0] == '3') {
            k = CRT;
            fp1 = fopen("bin/PASSWDS","r");
            while(fread((char*)&you_pf,sizeof(struct idst),1,fp1)) {

                if(you_pf.circle & i) {
                    printf("\r\n%8s [%-8s]",you_pf.name,you_pf.id);
                    --k;
                }
                if(!k) {
                    printf("\r\n���[����] ����[S] ����[P] : ");
                    lineinput(buf,1,YEA);
                    if(buf[0] == 's' || buf[0]== 'S') {
                        printf("\r\n");
                        k = -1;
                    }
                    else if(buf[0] == 'p' || buf[0] == 'P') {
			printf("\r\n");
                        break;
                    }
                    else {
                        k = CRT;
                    }
                }
            }
            fclose(fp1);

            buf[0] = getchar();
        }
        else if(buf[0] == 'p' || buf[0] == 'P'){
            break;
        }
    }
}

admit_circle(id,circlenum)
char *id;
int circlenum;
{
    FILE *fp1;
    char buf[80];
    sprintf(buf,"etc/%05d.cir",circlenum);
    if((fp1=fopen(buf,"r+"))==NULL) {
	fp1 = fopen(buf,"a");
    }
    fseek(fp1,0,2);
    fputs(id,fp1);
    fclose(fp1);
}

board_sort(char *direc,char*fhead)
{
    int i,j;
    long int fos;
    char tmp[80],test[80],buf[255];
    FILE *fp1, *fp2, *fp3, *fp4;
    struct dis_list btitle;

    sprintf(buf,"%s/%s",direc,fhead);
    if((fp1=fopen(buf,"r"))==NULL) {
	printf("\r\n %s ������ �ʽ��ϴ�.",buf);
        return;
    }
    sprintf(buf,"tmp/%s",fhead);
    if((fp2=fopen(buf,"w"))==NULL) {
	printf("\r\n %s�� ������ �ʽ��ϴ�.",buf);
	return;
    }
    j = 0;
    fseek(fp1,0,0); fseek(fp2,0,0);
    while(fread((char*)&btitle,sizeof(struct dis_list),1,fp1)) {
	if(btitle.look >=0) {
	    ++j;
	    btitle.num=j;
	    sprintf(tmp,"%s/%s",direc,btitle.bfilename);
 	    sprintf(test,"%s/%s%d.brd",direc,fhead,j);
	    rename(tmp,test);
	    sprintf(btitle.bfilename,"%s%d.brd",fhead,j);
            if(!fwrite((char*)&btitle,sizeof(struct dis_list),1,fp2)) {
                printf("\r\n ���� ���� ");
                return;
            }
	    printf("#");
	}
        else if(btitle.look <0 ) {
	    sprintf(buf,"rm %s/%s",direc,btitle.bfilename);
	    system(buf);
	    printf("\r\n �Խù�ȭ���� �����߽��ϴ�.");
	}
    }
    fclose(fp2);
    fclose(fp1);
    sprintf(tmp,"tmp/%s",fhead);
    sprintf(test,"%s/%s",direc,fhead);
    rename(tmp,test);
    printf("\r\n �Խ��� ������ �������ϴ�.");
    pressreturn();reprint=YEA;
}
mail_sort(char *direc,char*fhead)
{
    int i,j;
    long int fos;
    char tmp[80],test[80],buf[255];
    FILE *fp1, *fp2, *fp3, *fp4;
    struct dis_list btitle;

    sprintf(buf,"%s/%s",direc,fhead);
    if((fp1=fopen(buf,"r"))==NULL) {
        printf("\r\n %s ������ �ʽ��ϴ�.",buf);
        return;
    }
    sprintf(buf,"tmp/%s",fhead);
    if((fp2=fopen(buf,"w"))==NULL) {
        printf("\r\n %s�� ������ �ʽ��ϴ�.",buf);
        return;
    }
    j = 0;
    fseek(fp1,0,0); fseek(fp2,0,0);
    while(fread((char*)&btitle,sizeof(struct dis_list),1,fp1)) {
        if(btitle.look >=0) {
            ++j;
            btitle.num=j;
            sprintf(tmp,"%s/%s",direc,btitle.bfilename);
            sprintf(test,"%s/%s%d.brd",direc,fhead,j);
            rename(tmp,test);
            sprintf(btitle.bfilename,"%s%d.brd",fhead,j);
            if(!fwrite((char*)&btitle,sizeof(struct dis_list),1,fp2)) {
                printf("\r\n ���� ���� ");
                return;
            }
            printf("#");
        }
        else if(btitle.look <0 ) {
            sprintf(buf,"rm %s/%s",direc,btitle.bfilename);
            system(buf);
            printf("\r\n �Խù�ȭ���� �����߽��ϴ�.");
        }
    }
    fclose(fp2);
    fclose(fp1);
    sprintf(tmp,"tmp/%s",fhead);
    sprintf(test,"%s/%s",direc,fhead);
    rename(tmp,test);
    printf("\r\n �Խ��� ������ �������ϴ�.");
    pressreturn();reprint=YEA;
}

files_sort(char *direc,char*fhead,char *filedir)
{
    int i,j;
    long int fos;
    char tmp[80],test[80],buf[255];
    FILE *fp1, *fp2, *fp3, *fp4;
    struct dis_list btitle;

    sprintf(buf,"%s/%s",direc,fhead);
    if((fp1=fopen(buf,"r"))==NULL) {
        printf("\r\n %s ������ �ʽ��ϴ�.",buf);
        return;
    }
    sprintf(buf,"tmp/%s",fhead);
    if((fp2=fopen(buf,"w"))==NULL) {
        printf("\r\n %s�� ������ �ʽ��ϴ�.",buf);
        return;
    }
    j = 0;
    fseek(fp1,0,0); fseek(fp2,0,0);
    while(fread((char*)&btitle,sizeof(struct dis_list),1,fp1)) {
        if(btitle.look >=0) {
            ++j;
            btitle.num=j;
            sprintf(tmp,"%s/%s",direc,btitle.bfilename);
            sprintf(test,"%s/%s%d.brd",direc,fhead,j);
            rename(tmp,test);
            sprintf(btitle.bfilename,"%s%d.brd",fhead,j);
            if(!fwrite((char*)&btitle,sizeof(struct dis_list),1,fp2)) {
                printf("\r\n ���� ���� ");
                return;
            }
            printf("#");
        }
        else if(btitle.look <0 ) {
	    if(btitle.bfilename[0]!='\0') {
                sprintf(buf,"rm %s/%s",direc,btitle.bfilename);
                system(buf);
                printf("\r\n �Խù�ȭ���� �����߽��ϴ�.");
	    }
	    if(btitle.filename[0]!='\0') {
	        sprintf(buf,"mv %s/%s/%s ftp/tmp",\
	          getenv("HOME"),filedir,btitle.filename);
	        system(buf);
		printf("\r\n ȭ���� ftp/tmp�� �Ű���ϴ�.");
	    }
        }
    }
    fclose(fp2);
    fclose(fp1);
    sprintf(tmp,"tmp/%s",fhead);
    sprintf(test,"%s/%s",direc,fhead);
    rename(tmp,test);
    printf("\r\n �Խ��� ������ �������ϴ�.");
    pressreturn();reprint=YEA;
}

