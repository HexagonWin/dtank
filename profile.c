
#include "bbs.h"

myinfo()    /* 나의 회원 정보 */
{
    int i, j, k, select, back, key=1;
    char buf[80], ch;
    FILE *fp1;
    struct idst pfpf; /* 회원 정보에서 읽을 회원정보버퍼 */

    if(guest) {  /* 손님인 경우에는 */
	printf("\r\n 손님은 자기정보를 변경할 수 없습니다.");
	printf("\r\n 시솝에게 직접 요청하세요. ");
	nmenu=nmenu->upp;	
	reprint=YEA;
    }
    fp1 = fopen("bin/PASSWDS","r");  
    if(fseek(fp1,pf_fos,0)) { /* pf_fos는 전체에서 쓰일 변수 */
        printf("\r\n읽기 실폐");
        bye();
    }
    fread((char*)&pfpf,sizeof(struct idst),1,fp1);
    if(strcmp(pfpf.id,pf.id)) {
        printf("\r\nID가 틀립니다");
        bye();
    }
    fclose(fp1);
    my_pf();  /* 나의 정보를 보여준다. */
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
            change_info();  /* 정보를 바꾼다. */
        }
        else if(buf[0] == '2' && buf[1] == '\0') {
            change_flag();   /* 플랙을 바꾼다 */
        }
        else if(buf[0] == 'h' || buf[0] == 'H') {
            logo("abchelp.hlp");
        }
        else if(buf[0] == 'p' || buf[0] == 'P') {

            if((fp1 = fopen("bin/PASSWDS","r+")) == NULL) {
                printf("\r\nPASSWDS 에러");
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
                    printf("\r\nPASSWDS 에러");
                }
                fseek(fp1,pf_fos,SEEK_SET);
                fwrite((char*)&pfpf,sizeof(struct idst),1,fp1);
                fclose(fp1);
                break;
            }
        }
        else if(buf[0] == 'x' || buf[0] == 'X') {   /* 프로그램 종료 */
            printf("\r\n종료를 하시겠습니까(Y/n)? ");
            keyinput(buf,1);
            if(!(buf[0] == 'n' || buf[0] == 'N')) {
                if((fp1 = fopen("bin/PASSWDS","r+")) == NULL) {
                    printf("\r\n프로필 화일이 열리지 않습니다.");
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
            printf("\r\n\n엔터키를 치십시오.");
            keyinput(buf,1);
            key = 1;
            my_pf();
        }
    }
}

void my_pf() /* 나의 정보 표시 */
{
    int i, j;
    char buf[2];
    struct tm *tp, *localtime();
    printf("   자기정보관리 : abc");
    printf("\r\n\n    1 아   이   디 : %s",pf.id);
    printf("\r\n *  2 비 밀  번 호 : ");
    j = strlen(pf.passwd);
    for(i = 0; i < j; i++) putchar('*');
    printf("\r\n    3 성        명 : %s",pf.name);
    if(pf.flag & 0x0001) printf("\r\n   "); else printf("\r\n * ");
    printf(" 4 성        별 : %c",pf.sex);
    if(pf.flag & 0x0002) printf("\r\n   "); else printf("\r\n * ");
    printf(" 5 생 년  월 일 : %s",pf.birthday);
    if(pf.flag & 0x0004) printf("\r\n   "); else printf("\r\n * ");
    tp = localtime(&pf.inday);
    printf(" 6 가 입  일 자 : %2d년 %02d월 %02d일",tp->tm_year,tp->tm_mon+1,tp->tm_mday);
    if(pf.flag & 0x0008) printf("\r\n   "); else printf("\r\n * ");
    tp = localtime(&pf.logout);
    printf(" 7 마지막접속일 : %2d년 %02d월 %02d일 %02d시 %02d분",tp->tm_year,
    tp->tm_mon+1,tp->tm_mday,tp->tm_hour,tp->tm_min);
    if(pf.flag & 0x0010) printf("\r\n   "); else printf("\r\n * ");
    printf(" 8 우 편  번 호 : %s",pf.post);
    if(pf.flag & 0x0020) printf("\r\n   "); else printf("\r\n * ");
    printf(" 9 집   주   소 : %s",pf.home_addr);
    if(pf.flag & 0x0040) printf("\r\n   "); else printf("\r\n * ");
    printf("10 집   전   화 : %s",pf.home_tel);
    if(pf.flag & 0x0080) printf("\r\n   "); else printf("\r\n * ");
    printf("11 학   교   명 : %s",pf.office_name);
    if(pf.flag & 0x0100) printf("\r\n   "); else printf("\r\n * ");
    printf("12 학 교 연락처 : %s",pf.office_tel);
    if(pf.flag & 0x0200) printf("\r\n   "); else printf("\r\n * ");
    printf("13 주민등록번호 : %s",pf.id_no);
    if(pf.flag & 0x0400) printf("\r\n   "); else printf("\r\n * ");
    printf("14 등        급 : %d\r\n   15 편 지  수 신 : ",pf.level);
    if(pf.flag & 0x1000) printf("허용\r\n");
    else printf("금지\r\n");
    printf(" * 16 동   아   리 : %x\r\n",pf.circle);
    printf(" * 17 접 속  횟 수 : %d회\r\n",pf.logcount);
    printf(" * 18 총 이용 시간 : %d초\r\n",pf.totaltime);
    tp = localtime(&pf.lasttime);
    printf(" * 19 이 용  기 간 : 19%2d년 %02d월 %02d일 %02d시 %02d분",tp->tm_year,

    tp->tm_mon+1,tp->tm_mday,tp->tm_hour,tp->tm_min);
    printf("\r\n\n'*'표시가 있는것은 비공개입니다.");
    printf("\r\n정보변경[1] 공개/비공개 변경[2] 상위메뉴[P] 종료[X] 가기메뉴[GO]\r\n선택 >> ");
}

void change_info()   /* 나의 정보 변경 */
{
    int i, j, k, l, a[14];
    char buf[80], tmp[80], ch,passwd[9];
    FILE *fp1;
    struct idst changepf;

    while(1) {
        printf("\r\n\r변경할 내용 번호/N ? ");
        keyinput(buf,2);
        if(buf[0] == '2' && buf[1] == '\0') {
            printf("\r\n현재의 비밀번호 : ");
            get_passwd(buf,8);
            if(!strcmp(buf,pf.passwd)) {
                printf("\r\n새로운 비밀번호 입력 : ");
                get_passwd(buf,8);
                printf("\r\n새로운 비밀번호 확인 : ");
                get_passwd(passwd,8);
                if(!strcmp(buf,passwd)) {
                    sprintf(pf.passwd,"%s",passwd);
                }
                else {

                    printf("\r\n비밀번호 변경이 취소 되었습니다.");
                }
            }
            else {
                printf("\r\n비밀번호가 틀립니다.");
            }
        }
        else if(buf[0] == '5' && buf[1] == '\0') {
            i = 1;
            while(i) {
                printf("\r\n생년월일 : ");
                keyinput(tmp,8);
                tmp[2] = '0';
                tmp[5] = '0';
                for(j = 0; j < 8; j++) {
                    if(!(isdigit(tmp[j]))) {
                        printf("\r\n정확히 입력하여 주십시오.");
                        i = 1;
                        break;
                    }
                    else i = 0;
                }

                if(!i) {
                    printf("\r\n\r양력입니까 Y/n ? ");
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
            printf("\r\n우편번호 : ");
            keyinput(pf.post,7);

            pf.post[3] = '-';
        }
        else if(buf[0] == '9' && buf[1] == '\0') {
            i = 1;
            while(i) {
                printf("\r\n\r집주소 : ");
                keyinput(pf.home_addr,79);
                if(pf.home_addr[0] == 0x00) {
                    printf("\r\n정확히 입력하여 주십시오.");
                    i = 1;
                }
                else i = 0;
            }
        }
        else if(buf[0] == '1' && buf[1] == '0') {
            i = 1;
            while(i) {
                printf("\r\n집전화 : ");
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
            printf("\r\n\r직장명 : ");
            keyinput(pf.office_name,79);
        }
        else if(buf[0] == '1' && buf[1] == '2' && buf[2] == '\0') {
            printf("\r\n직장전화 : ");
            keyinput(pf.office_tel,14);
        }
        else if(buf[0] == '1' && buf[1] == '3' && buf[2] == '\0') {
            l = 1;
            while(l) {
                l = 0;
                printf("\r\n주민등록번호 : ");
                keyinput(pf.id_no,14);
                pf.id_no[6] = '0';
                j = 1;
                for(i = 0; i < 14; i++) {
                    if(isdigit(pf.id_no[i])) a[i] = pf.id_no[i] - 48;
                    else {
                        printf("\r\n정확히 입력하여 주십시오.");
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
                        printf("\r\n번호가 틀렸습니다.");
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
                printf("\r\n금지");
            }
            else {
                pf.flag |= 0x00001000;
                printf("\r\n허용");
            }
        }
        else if(buf[0] == 'n' || buf[0] == 'N' || buf[0] == '\0') {
            my_pf();
            break;
        }
        else {
            printf("\r\n해당번호는 변경시킬 수 없습니다.\r\n");
        }
    }
    printf("\r\n 바뀐정보를 저장하시겠습니까?(y/N)>> ");
    keyinput(buf,5);
    if(buf[0]=='y' || buf[0]=='Y') {
    	fp1 = fopen("bin/PASSWDS","r+");
	if(fseek(fp1,pf_fos,0)) {
	    printf("\r\n 읽기 실패");
	    bye();
   	}
	fread((char*)&changepf,sizeof(struct idst),1,fp1);
	if(strcmp(changepf.id,pf.id)) {
	    printf("\r\n 아이디가 다릅니다(심각한 에러)");
	    bye();
	}
	fwrite((char*)&pf,sizeof(struct idst),1,fp1);
	fclose(fp1);
    }

}

void change_flag()   /* 공개 비공개 변경 */
{
    char ch[3];
    while(1) {
        printf("\r\n 바꾸실 내용(번호/N)? ");
        keyinput(ch,2);
        if(ch[0] == '4' && ch[1] == '\0') {
            printf("\r\n성별 ");
            if(pf.flag & 0x00000001) {
                pf.flag &= 0xfffffffe;
                printf("비공개");
            }
            else {
                pf.flag |= 0x00000001;
                printf("공개");
            }
        }
        else if(ch[0] == '5' && ch[1] == '\0') {
            printf("\r\n생년월일 ");
            if(pf.flag & 0x00000002) {
                pf.flag &= 0xfffffffd;
                printf("비공개");
            }

            else {
                pf.flag |= 0x00000002;
                printf("공개");
            }
        }
        else if(ch[0] == '6' && ch[1] == '\0') {
            printf("\r\n가입일자 ");
            if(pf.flag & 0x00000004) {
                pf.flag &= 0xfffffffb;
                printf("비공개");
            }
            else {
                pf.flag |= 0x00000004;
                printf("공개");
            }
        }
        else if(ch[0] == '7' && ch[1] == '\0') {
            printf("\r\n마지막접속일 ");
            if(pf.flag & 0x00000008) {
                pf.flag &= 0xfffffff7;
                printf("비공개");
            }
            else {
                pf.flag |= 0x00000008;
                printf("공개");
            }
        }
        else if(ch[0] == '8' && ch[1] == '\0') {
            printf("\r\n우편번호 ");
            if(pf.flag & 0x00000010) {
                pf.flag &= 0xffffffef;
                printf("비공개");
            }
            else {
                pf.flag |= 0x00000010;
                printf("공개");
            }
        }
        else if(ch[0] == '9' && ch[1] == '\0') {
            printf("\r\n집주소 ");
            if(pf.flag & 0x00000020) {
                pf.flag &= 0xffffffdf;
                printf("비공개");
            }
            else {

                pf.flag |= 0x00000020;
                printf("공개");
            }
        }
        else if(ch[0] == '1' && ch[1] == '0' && ch[2] == '\0') {
            printf("\r\n집전화 ");
            if(pf.flag & 0x00000040) {
                pf.flag &= 0xffffffbf;
                printf("비공개");
            }
            else {
                pf.flag |= 0x00000040;
                printf("공개");
            }
        }
        else if(ch[0] == '1' && ch[1] == '1' && ch[2] == '\0') {
            printf("\r\n직장명 ");
            if(pf.flag & 0x00000080) {
                pf.flag &= 0xffffff7f;

                printf("비공개");
            }
            else {
                pf.flag |= 0x00000080;
                printf("공개");
            }
        }
        else if(ch[0] == '1' && ch[1] == '2' && ch[2] == '\0') {
            printf("\r\n직장전화 ");
            if(pf.flag & 0x00000100) {
                pf.flag &= 0xfffffeff;
                printf("비공개");
            }
            else {
                pf.flag |= 0x00000100;
                printf("공개");
            }
        }
        else if(ch[0] == '1' && ch[1] == '3' && ch[2] == '\0') {

            printf("\r\n주민등록번호 ");
            if(pf.flag & 0x00000200) {
                pf.flag &= 0xfffffdff;
                printf("비공개");
            }
            else {
                pf.flag |= 0x00000200;
                printf("공개");
            }
        }
        else if(ch[0] == '1' && ch[1] == '4' && ch[2] == '\0') {
            printf("\r\n등급 ");
            if(pf.flag & 0x00000400) {
                pf.flag &= 0xfffffbff;
                printf("비공개");
            }
            else {
                pf.flag |= 0x00000400;

                printf("공개");
            }
        }
        else if(((ch[0] == 'n' || ch[0] == 'N') && ch[1] == '\0') || ch[0] == '\0') {
            my_pf();
            break;
        }
        else {
            printf("\r\n해당번호는 변경시킬 수 없습니다.\r\n");
        }
    }
}

display_pf(struct idst whom)
{
    char buf[80];
    struct tm *tp, *localtime();
    printf("\r\n %s(%s) %s",whom.id,whom.name,whom.id_no);
    printf("\r\n 성별(%c) %s",whom.sex,whom.office_name);
}
changepasswd()
{
    char buf[80], buf2[80];
    cls();
    printf("\r\n 패스워드를 바꿉니다.");
    printf("\r\n 현재의 패스워드를 집어넣어주세요.");
    printf("\r\n 현재 패스워드: ");keyinput(buf,10);
    if(!strcmp(pf.passwd,buf)) {
	printf("\r\n 새로운 패스워드: ");keyinput(buf,10);
	printf("\r\n 다시한번 넣어주세요: ");keyinput(buf2,10);
	if(!strcmp(buf,buf2)) {
	    change_passwd(pf,buf);
	    printf("\r\n 패스워드가 바뀌었습니다.");pressreturn();
	}
	else {
	    printf("\r\n 실패했습니다."); pressreturn();
	}
    }
    else {
	printf("\r\n 패스워드가 틀립니다.");pressreturn();
    }
}
changeaddress()
{
    FILE *fp1;
    char buf[80], buf2[80];
    int fos;
    struct idst temp;
    cls();
    printf("\r\n 주소를 바꿉니다.");
    fp1 = fopen("bin/PASSWDS","r");
    fos=find_pfoffset("bin/PASSWDS",pf);
    fseek(fp1,fos,0);
    fread((char*)&temp,sizeof(struct idst),1,fp1);
    fclose(fp1);
    printf("\r\n 현재의 주소: %s",temp.home_addr);
    printf("\r\n 변경될 주소: "); keyinput(buf,80);
    if(buf[0]!='\0') {
        change_address(pf,buf);
        printf("\r\n 주소가 변경되었습니다.");pressreturn();
    }
    else { printf("\r\n 취소하셨습니다."); pressreturn(); 
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
  
