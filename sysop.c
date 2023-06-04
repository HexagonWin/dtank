/* 슈퍼 운영자의 메뉴입니다. */
/* 운영자 함수 */

#include "bbs.h"

void id_select(),pf_change(),display_profile(),id_sort();
void file_sort(),dis_exp(),name_search(),list_sort();
void chk_handcap();

struct idst bpf;  /* 이곳에서 쓰일 pf buffer이다. */

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
    printf("\r\n\n 관리번호 : ");
    lineinput(buf,10,NA);
    if(strcmp(cfg.secret,buf)) {
        printf("\r\n당신은 운영자가 아닙니다.");
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
	    printf("\r\n     | 이곳은 슈퍼운영자메뉴입니다 |");
	    printf("\r\n     -------------------------------");
            printf("\r\n");
	    printf("\r\n 1. 신입회원의 아이디를 발급해줍니다.");
	    printf("\r\n 2. 아이디를 정리합니다.");
	    printf("\r\n 3. LOGFILE을 열람합니다.");
	    printf("\r\n 4. 회원관리(회원정보열람및변경)");
	    printf("\r\n 5. 게시물정리(유보)");
	    printf("\r\n 6. 셧다운 공고 ");
	    printf("\r\n 7. 편지함 정리");
	    printf("\r\n 8. 메뉴구성");
	    printf("\r\n 9. 유져보여주기");
	    printf("\r\n d. 사용자 지우기");
	    printf("\r\n p. 슈퍼운영자 메뉴에서 나간다.");
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
	    /* 게시물 정리 코드를 집어넣을것.*/
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
	    printf("\r\n 지우실 사용자아이디>> ");keyinput(buf,10);
	    del_user("bin/PASSWDS",buf);
	}

        else if(buf[0] == 'p') {
            break;
        }
        else key = 0;
    }
    Menu_Free(); /* 일단 메뉴를 다시 해제한다음에 */
    Get_Menu(); /* 다시 메뉴를 읽는다 그래서 로그아웃하지
			않고도 바뀐 메뉴를 확인할 수
			있다. */
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
        printf("\r\n가입자가 없습니다.");
    }
    else {
        if((fp2 = fopen("bin/PASSWDS","r+")) == NULL) {
            printf("\r\n처음 가입자");
            fp2 = fopen("bin/PASSWDS","a");
            count = 0;
        }
        else {
            if(fseek(fp2,-sizeof(struct idst),SEEK_END)) {
                printf("\r\n아이디 화일 포인터 변경 실폐");
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
            printf("\r\n\n\r가입을 받아주시겠습니까(y/N)(P는 전메뉴로)? ");
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
                printf("\r\n가입 취소");
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

    printf("\r\n찾는 아이디 : ");
    keyinput(buf,8);
    if((fp1 = fopen("bin/PASSWDS","r+")) == NULL) {
        printf("\r\nID 화일을 열수가 없습니다.");
        host_end();
    }
    i = 1;
    while(fread((char*)&bpf,sizeof(struct idst),1,fp1)) {
        if(!strcmp(buf,bpf.id)) {
            filepos = ftell(fp1) - sizeof(struct idst);
            display_profile();
            while(1) {
                printf("\r\n\r바꿀 내용이 있습니까? (번호/n) ");
                keyinput(buf,2);
                if(buf[0] == '1' && buf[1] == '\0') {
                    printf("\r\n## 아   이   디 : ");
                    keyinput(bpf.id,8);
                }
                else if(buf[0] == '2' && buf[1] == '\0') {
                    printf("\r\n## 비 밀  번 호 : ");
                    keyinput(bpf.passwd,8);
                }
                else if(buf[0] == '3' && buf[1] == '\0') {
                    printf("\r\n\r## 이        름 : ");
                    keyinput(bpf.name,8);
                }
                else if(buf[0] == '4' && buf[1] == '\0') {
                    printf("\r\n## 성        별 : ");
                    bpf.sex = getchar();
                }
                else if(buf[0] == '5' && buf[1] == '\0') {
                    printf("\r\n## 생 년  월 일 : ");
                    keyinput(bpf.birthday,11);
                }
                else if(buf[0] == '7' && buf[1] == '\0') {
                    printf("\r\n## 마지막접속일 : ");
                    time(&bpf.logout);
                }
                else if(buf[0] == '8' && buf[1] == '\0') {
                    printf("\r\n## 우 편  번 호 : ");
                    keyinput(bpf.post,8);
                }
                else if(buf[0] == '9' && buf[1] == '\0') {
                    printf("\r\n\r## 집   주   소 : ");
                    keyinput(bpf.home_addr,79);
                }
                else if(buf[0] == '1' && buf[1] == '0' && buf[2] == '\0') {
                    printf("\r\n## 집   전   화 : ");
                    keyinput(bpf.home_tel,14);
                }
                else if(buf[0] == '1' && buf[1] == '1' && buf[2] == '\0') {
                    printf("\r\n\r## 직   장   명 : ");
                    keyinput(bpf.office_name,79);
                }
                else if(buf[0] == '1' && buf[1] == '2' && buf[2] == '\0') {
                    printf("\r\n## 직장전화번호 : ");
                    keyinput(bpf.office_tel,14);
                }
                else if(buf[0] == '1' && buf[1] == '3' && buf[2] == '\0') {
                    printf("\r\n## 주민등록번호 : ");
                    keyinput(bpf.id_no,14);
                }
                else if(buf[0] == '1' && buf[1] == '4' && buf[2] == '\0') {
                    printf("\r\n## 등        급 : ");
                    keyinput(buf,10);
                    bpf.level = atoi(buf);
                }
                else if(buf[0] == '1' && buf[1] == '5' && buf[2] == '\0') {
                    printf("\r\n## 동   아   리 : ");
                    keyinput(buf,10);
                    bpf.circle = atoi(buf);
                }
                else if(buf[0] == '2' && buf[1] == '0' && buf[2] == '\0') {
                    printf("\r\n## 연 장  기 간 : ");
                    keyinput(buf,3);
                    j = atoi(buf);
                    if(j) bpf.lasttime = time(&bpf.lasttime) + 3600 * 24 * j;
                }
                else if(buf[0] == 'n' || buf[0] == 'N') {
                    break;
                }
            }
            if(fseek(fp1,filepos,SEEK_SET)) {
                printf("\r\n화일 포인터 변경 실폐");
                host_end();
            }
            if(fwrite((char*)&bpf,sizeof(struct idst),1,fp1)) {
                printf("\r\nID수정되었습니다.");
            }
            else {
                printf("\r\nID수정 실폐 ");
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
        printf("\r\n%해당회원이 없습니다.");
    }
    printf("\r\n[Enter]를 치십시요.");
    keyinput(buf,1);
}

int shutdownmessage()
{
    int fout;
    char buf[80], tmp[80];
    struct nowwork mess;
    FILE *fp1;
    printf("\r\n전송 메세지를 쓰십시요.\r\n");
    while(1) {
        keyinput(tmp,60);
        if(tmp[0] != '\0') {
            sprintf(buf,"\033[s\0337\033[23;1H*강제메시지:\033[7m%s\033[0m\0338\033[u",tmp);
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
    printf("\r\n\n1. 아 이 디\r\n2. 이    름\r\n3. 레    벨\r\n4. 일련번호\r\n\n선택 >> ");
    keyinput(buf,1);
    if(buf[0] == '1') {
        flag = 1;
        printf("\r\n아 이 디 : ");
        keyinput(buf,8);
        i = strlen(buf);
    }
    else if(buf[0] == '2') {
        flag = 2;
        printf("\r\n\r이    름 : ");
        keyinput(buf,8);
        i = strlen(buf);
    }
    else if(buf[0] == '3') {
        flag = 3;
        printf("\r\n상위레벨 : ");
        keyinput(buf,10);
        up = atoi(buf);
        printf("\r\n하위레벨 : ");
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
        printf("\r\n번    호 : ");
        keyinput(buf,8);
        i = atoi(buf);
    }
    else {
        return;
    }
    if((fp1 = fopen("bin/PASSWDS","r+")) == NULL) {
        printf("\r\nID 화일을 열수가 없습니다.");
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
            printf("\r\n계속[Enter] 중지 [P] : ");
            keyinput(tmp,4);
            if(tmp[0] == 'p' || temp[0] == 'P') {
                break;
            }
        }
    }
    fclose(fp1);
}

letter_sort()   /* 편지함 정리 */
{
    int i, j, num, max;
    char buf[80], tmp[40];
    time_t delti;
    FILE *fp1, *fp2, *fp3, *fp4;

    printf("\n\r몇일간의 편지를 보관하시겠습니까(Enter는 30일)? ");
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
    printf("\n\r몇일간의 접속자를 남기시겠습니까(Enter는 취소)? ");
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

void display_profile()    /* 회원 정보 조회 */
{
    char buf[2];
    struct tm *tp, *localtime();
    printf("\r\n*  0. 일 련  번 호 : %05d",bpf.num);
    printf("\r\n   1. 아   이   디 : %s",bpf.id);
    printf("\r\n*  2. 비 밀  번 호 : %s",bpf.passwd);
    printf("\r\n   3. 성        명 : %s",bpf.name);
    if(bpf.flag & 0x0001) printf("\r\n   4.");
    else printf("\r\n*  4.");
    printf(" 성        별 : %c",pf.sex);
    if(bpf.flag & 0x0002) printf("\r\n   5.");
    else printf("\r\n*  5.");
    printf(" 생 년  월 일 : %s",bpf.birthday);
    tp = localtime(&bpf.inday);
    if(bpf.flag & 0x0004) printf("\r\n   6.");
    else printf("\r\n*  6.");
    printf(" 가 입  일 자 : %2d년 %02d월 %02d일",tp->tm_year,(tp->tm_mon)+1,tp->tm_mday);
    tp = localtime(&bpf.logout);
    if(bpf.flag & 0x0008) printf("\r\n   7.");
    else printf("\r\n*  7.");
    printf(" 마지막접속일 : 19%2d-%02d-%02d  %02d:%02d:%02d",
      tp->tm_year,tp->tm_mon+1,tp->tm_mday,tp->tm_hour,tp->tm_min,tp->tm_sec);
    if(bpf.flag & 0x0010) printf("\r\n   8.");
    else printf("\r\n*  8.");
    printf(" 우 편  번 호 : %s",bpf.post);
    if(bpf.flag & 0x0020) printf("\r\n   9.");
    else printf("\r\n*  9.");
    printf(" 집   주   소 : %s",bpf.home_addr);
    if(bpf.flag & 0x0040) printf("\r\n  10.");
    else printf("\r\n* 10.");
    printf(" 집   전   화 : %s",bpf.home_tel);
    if(bpf.flag & 0x0080) printf("\r\n  11.");
    else printf("\r\n* 11.");
    printf(" 직   장   명 : %s",bpf.office_name);
    if(bpf.flag & 0x0100) printf("\r\n  12.");
    else printf("\r\n* 12.");
    printf(" 직장전화번호 : %s",bpf.office_tel);
    if(bpf.flag & 0x0200) printf("\r\n  13.");
    else printf("\r\n* 13.");
    printf(" 주민등록번호 : %s",bpf.id_no);
    if(bpf.flag & 0x0400) printf("\r\n  14.");
    else printf("\r\n* 14.");
    printf(" 등        급 : %d",bpf.level);

    printf("\r\n  15. 동   아   리 : %x",bpf.circle);
    printf("\r\n  16. 편 지  수 신 : ");
    if(bpf.flag & 0x1000) printf("허용");
    else printf("금지");
    printf("\r\n* 17. 회원  플래그 : %x",bpf.expflag);
    printf("\r\n* 18. 접 속  횟 수 : %d",bpf.logcount);
    printf("\r\n* 19. 총 이용 시간 : %d",bpf.totaltime);
    tp = localtime(&bpf.lasttime);
    printf("\r\n* 20. 사 용  기 간 : 19%2d-%02d-%02d  %02d:%02d:%02d",
      tp->tm_year,tp->tm_mon+1,tp->tm_mday,tp->tm_hour,tp->tm_min,tp->tm_sec);
}
showLOGFILE()
{

    FILE *fp1;
    char buf[80];
    char data[10];
    long int end;

    
    printf("\r\n\r\n 날짜를 입력해 주세요(yymmdd)? >> ");
    keyinput(buf,6);
    fp1 = fopen("logfile/LOGFILE","r");
    fseek(fp1,0,2); end= ftell(fp1);fseek(fp1,0,0);
    if(buf[0]=='\0') printf("\r\n 취소 ");
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
        printf("\r\n /* 여기는 회원 관리 메뉴입니다.*/ ");
        printf(    "\r\n\r\n 2. 회원 정보 변경");
        printf(    "\r\n >> ");
        keyinput(buf,3);
        if(buf[0] =='1') {
	    printf("\r\n\r\n /* 어떤 방식으로 찾겠습니까?");
	    printf("\r\n  1. 아이디");
	    printf("\r\n  2. 가입순");
	    printf("\r\n  3. 이  름");
	    printf("\r\n  >> ");
	    keyinput(buf,3);
	    fp1 = fopen("bin/PASSWDS","r");
	    fseek(fp1,0,2); howmany = ftell(fp1)/sizeof(struct dis_list);
	    fseek(fp1,0,0);printf("\r\n 총회원은 %d명입니다.",howmany);
	    if (buf[0] =='1') {
	        printf("\r\n 아이디를 입력해주세요.");keyinput(buf,10);
	        for(i = 0; i<howmany;i++) {
	      	    fread((char*)&spf,sizeof(struct dis_list),1,fp1);
	  	    if(!strcmp(spf.id,buf)) {
		        display_profile(spf);
		    }
	        }
	    }
	    else if(buf[0] =='2') {
	        printf("\r\n 가입순번을 말해주세요. 그후로 계속 출력됩니다.");
	        printf("\r\n 가입순번: ");keyinput(buf,10);
	        num = atoi(buf);
	        fseek(fp1,(num-1)*sizeof(struct dis_list),0);
	        for (i = num-1; i<howmany; i++) {
                    fread((char*)&spf,sizeof(struct dis_list),1,fp1);
		    display_profile(spf);
	            printf("\r\n 계속하시겠습니까(Y/n"); keyinput(buf,1);
		    if(buf[0] =='n' || buf[0] =='N') break;
	        }
	    }
	    else if(buf[0]=='3') {
	        printf("\r\n 찾고자 하는 분의 이름을 입력해주세요.");
	        printf("\r\n 이름: ");keyinput(buf,10);
	        for (i=0;i<howmany;i++) {
	  	    fread((char*)&spf,sizeof(struct dis_list),1,fp1);
		    if(!strcmp(spf.name,buf)) {
		        display_profile(spf);
		    }
                }
       	    }
            else printf("\r\n 선택오류입니다.");
        }
        else if(buf[0] =='2') {
        	pf_change();
        }
	else {
	    printf("\r\n 나갑니다.");
	    break;
	}
    }
}
/* 이곳은 내가 새로이 만든 슈퍼운영자 부분이다. */
supermanager()
{
    char buf[80];

    cls();
    while(1) {
        printf("\r\n (%s)유료회원관리메뉴입니다.");
        printf("\r\n ---------------------------");
        printf("\r\n 1. 동아리가입처리");
        printf("\r\n 2. 탈  퇴  처  리");
        printf("\r\n 3. 회  원  조  회");
        printf("\r\n 4. 회원들에게 편지보내기");
        printf("\r\n 명령>> ");
        keyinput(buf,10);
        if(buf[0]=='p' || buf[0]=='P') break;
        else if(buf[0]=='1') {
            printf("\r\n 가입처리 메뉴입니다.");
        }
        else if(buf[0] =='2') {
            printf("\r\n 탈퇴처리메뉴입니다.");
        }
        else if(buf[0]=='3') {
            printf("\r\n 회원을 조회합니다. ");
            show_user("bin/PASSWDS");
        }
        else if(buf[0]=='4') {
            printf("\r\n 회원들에게 편지를 보냅니다.");
            printf("\r\n 아직 지원되지 않습니다.");
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
        printf("\r\n 0.나감 1.가입순으로 2.아이디로 3.이름으로 >>");
        keyinput(buf,10);
        if(buf[0]=='1') {
	    sprintf(buf,"%s",what);
	    fp1 = fopen(buf,"r");
	    fseek(fp1,0,2); howmany = ftell(fp1)/sizeof(struct idst);
	    fclose(fp1);
	    list_user(what,0,10);
	    num = 0;
	    while(1) {
 		printf("\r\n 명령: f:다음 b:이전 0:나감>> ");
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
            printf("\r\n찾으시려는아이디: ");keyinput(whom,10);
            sprintf(buf,"%s",what);
            fp1=fopen(buf,"r");
            fseek(fp1,0,2); howmany = ftell(fp1)/sizeof(struct idst);
            fseek(fp1,0,0);
            for(i=0;i<howmany;i++) {
                fread((char*)&member,sizeof(struct idst),1,fp1);
                if(!strcmp(whom,member.id)) {
                    printf("\r\n %s의 정보입니다.",member.id);
                    printf("\r\n 아이디:%s, 이름:%s, 생일:%s",\
                      member.id,member.name,member.birthday);

                }
            }
            fclose(fp1);
        }
        else if(buf[0]=='3') {
            printf("\r\n조회할이름: ");keyinput(whom,10);
            sprintf(buf,"%s",what);
            fp1=fopen(buf,"r");
            fseek(fp1,0,2); howmany = ftell(fp1)/sizeof(struct idst);
            fseek(fp1,0,0);
            for(i=0;i<howmany;i++) {
                fread((char*)&member,sizeof(struct idst),1,fp1);
                if(!strcmp(whom,member.name)) {
                    printf("\r\n %s의 정보입니다.",member.id);
                    printf("\r\n 아이디:%s, 이름:%s, 생일:%s",\
                      member.id,member.name,member.birthday);
                }
            }
            fclose(fp1);
        }
        else if(buf[0]=='0') {
            printf("\r\n 나갑니다.");printf("\r\n");
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
	printf("\r\n %5d %5d 아이디: %8s, 이름: %4s, 성별:%c 생일: %13s",\
	  i,member.num,member.id,member.name,member.sex,member.birthday);
    }
    fclose(fp1);

}
/* 아이디별로 이용자를 삭제하는 것이다. 레코드순으로 지우는 것은 다음 함수 */
del_user(char*whatfile,char*user)
{
    FILE *fp1;
    char buf[80],buf2[80];
    int i,howmany;
    struct idst *memberlist;

    sprintf(buf,"%s",whatfile);
    fp1 = fopen(buf,"r");
    fseek(fp1,0,2); howmany = ftell(fp1)/sizeof(struct idst);
    printf("\r\n 총가입자수는 %d",howmany);
    if((memberlist = (struct idst *)calloc(howmany,sizeof(struct idst))) == \
      NULL) {
	printf("\r\n 메모리 부족");pressreturn();
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
	    printf("\r\n %s를 삭제했습니다.",memberlist[i].id);
	}
	else {
	    fwrite((char*)&memberlist[i],sizeof(struct idst),1,fp1);
	}
    }
    fclose(fp1);
    printf("\r\n 삭제를 마쳤습니다."); pressreturn();
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
    printf("\r\n 총가입자수는 %d",howmany);
    if((memberlist = (struct idst *)calloc(howmany,sizeof(struct idst))) == \
      NULL) {
        printf("\r\n 메모리 부족");pressreturn();
        exit(1);
    }
    fseek(fp1,0,0);
    for (i=0;i<howmany;i++) {
        fread((char*)&memberlist[i],sizeof(struct idst),1,fp1);
        printf("\r\n %s",memberlist[i].id);
    }
    fseek(fp1,numN*sizeof(struct idst),0);
    fread((char*)&test,sizeof(struct idst),1,fp1);
    printf("\r\n %d번째 %s를 지우시려는 것이 맞습니까?(Y/n)",numN,test.id);
    keyinput(buf,1); 
    if(buf[0]=='n' ||buf[0]=='N') {
	printf("\r\n 취소하였습니다."); pressreturn(); return;
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
            printf("\r\n %d 번째 %s를 삭제했습니다.",i,memberlist[i].id);
        }
        else {
            fwrite((char*)&memberlist[i],sizeof(struct idst),1,fp1);
        }
    }
    fclose(fp1);
    printf("\r\n 삭제를 마쳤습니다."); pressreturn();
    free(memberlist);
}
