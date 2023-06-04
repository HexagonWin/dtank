
#include "bbs.h" 

void chk_csysop()
{
    int i;
    char buf1[30],buf2[30];
    FILE *fp1;
    if((fp1 = fopen("bin/CIRCLE","r")) == NULL);
    else {
        for(i = 0; i < 31; i++) {
            fscanf(fp1,"%s",buf1);  /* 동호회이름을 읽는다.*/
	    fscanf(fp1,"%s",buf2);  /* 동호회장을 읽는다.*/
            if(!strcmp(buf2,pf.id)) {
                cs = 1;
                cs <<= i;
                printf("\r\n%d번째 동아리 %s의 촌장이십니다."\
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
	printf("\r\n 죄송합니다. 동호회 촌장만이 쓸 수 있어요.");
	pressreturn();reprint=YEA;
	return; /* 이 동호회 시솝이 아니면 나감 */ 
    }
    while(1) {
        printf("\r\n 동호회 운영자 메뉴");
        printf("\r\n\r\n1. 회원을 동호회에 가입시켜주세요.\
\r\n2. 동호회에서 회원을 짤라요\
\r\n3. 회원을 쭈욱 나열해봅시다.");
        printf("\r\nP. 자 이제 나가자...\r\n어떤 것을 할까요?: ");
        lineinput(buf,1,YEA);
        j = atoi(buf);
        if(buf[0] == '1' || buf[0] == '2') {
            printf("\r\n아이디 : ");
            lineinput(buf,8,YEA);

            if((fp1 = fopen("bin/PASSWDS","r+")) == NULL) {
                printf("\r\n아이디 화일이 열리지 않습니다.");
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
                        printf("\r\n화일 포인터 변경 실폐");
                    }
                    if(fwrite((char*)&you_pf,sizeof(struct idst),1,fp1)) {
                        printf("\r\n변경 되었습니다.");
                    }
                    k =0;
                    break;
                }
            }
            fclose(fp1);
            if(k) {
                printf("\r\n%s회원이 없습니다.",buf);
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
                    printf("\r\n계속[엔터] 연속[S] 중지[P] : ");
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
	printf("\r\n %s 열리지 않습니다.",buf);
        return;
    }
    sprintf(buf,"tmp/%s",fhead);
    if((fp2=fopen(buf,"w"))==NULL) {
	printf("\r\n %s가 열리지 않습니다.",buf);
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
                printf("\r\n 쓰기 실패 ");
                return;
            }
	    printf("#");
	}
        else if(btitle.look <0 ) {
	    sprintf(buf,"rm %s/%s",direc,btitle.bfilename);
	    system(buf);
	    printf("\r\n 게시물화일을 삭제했습니다.");
	}
    }
    fclose(fp2);
    fclose(fp1);
    sprintf(tmp,"tmp/%s",fhead);
    sprintf(test,"%s/%s",direc,fhead);
    rename(tmp,test);
    printf("\r\n 게시판 정리가 끝났습니다.");
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
        printf("\r\n %s 열리지 않습니다.",buf);
        return;
    }
    sprintf(buf,"tmp/%s",fhead);
    if((fp2=fopen(buf,"w"))==NULL) {
        printf("\r\n %s가 열리지 않습니다.",buf);
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
                printf("\r\n 쓰기 실패 ");
                return;
            }
            printf("#");
        }
        else if(btitle.look <0 ) {
            sprintf(buf,"rm %s/%s",direc,btitle.bfilename);
            system(buf);
            printf("\r\n 게시물화일을 삭제했습니다.");
        }
    }
    fclose(fp2);
    fclose(fp1);
    sprintf(tmp,"tmp/%s",fhead);
    sprintf(test,"%s/%s",direc,fhead);
    rename(tmp,test);
    printf("\r\n 게시판 정리가 끝났습니다.");
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
        printf("\r\n %s 열리지 않습니다.",buf);
        return;
    }
    sprintf(buf,"tmp/%s",fhead);
    if((fp2=fopen(buf,"w"))==NULL) {
        printf("\r\n %s가 열리지 않습니다.",buf);
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
                printf("\r\n 쓰기 실패 ");
                return;
            }
            printf("#");
        }
        else if(btitle.look <0 ) {
	    if(btitle.bfilename[0]!='\0') {
                sprintf(buf,"rm %s/%s",direc,btitle.bfilename);
                system(buf);
                printf("\r\n 게시물화일을 삭제했습니다.");
	    }
	    if(btitle.filename[0]!='\0') {
	        sprintf(buf,"mv %s/%s/%s ftp/tmp",\
	          getenv("HOME"),filedir,btitle.filename);
	        system(buf);
		printf("\r\n 화일을 ftp/tmp로 옮겼습니다.");
	    }
        }
    }
    fclose(fp2);
    fclose(fp1);
    sprintf(tmp,"tmp/%s",fhead);
    sprintf(test,"%s/%s",direc,fhead);
    rename(tmp,test);
    printf("\r\n 게시판 정리가 끝났습니다.");
    pressreturn();reprint=YEA;
}

