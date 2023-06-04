
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
            printf("\r\n희망하는 아이디를 쓰세요(한글도 돼요).");
            printf("\r\n--3~8자로 하세요.--");
            printf("\n\r아이디 : ");
            lineinput(pf.id,8,YEA);
            if(strlen(pf.id) > 2) {
                flag &= 0xfffffffe;
                if((fp1 = fopen("bin/PASSWDS","r")) != NULL) {
                    while(fread((char*)&tmp_pf,sizeof(struct idst),1,fp1)) {
                        if(!strcmp(tmp_pf.id,pf.id)) {
                            printf("\r\n'%s'는 다른사람이 신청하여 사용중인 아이디 입니다.",tmp_pf.id);
                            flag |= 0x00000001;
                            break;
                        }
                    }
                }
		fclose(fp1);
                if((fp1=fopen("bin/PASSWDS.TMP","r")) !=NULL) {
		    while(fread((char*)&tmp_pf,sizeof(struct idst),1,fp1)) {
		 	if(!strcmp(tmp_pf.id,pf.id)) {
			    printf("\r\n'%s'는 다른사람이 가입신청한 아이디입니다.",tmp_pf.id);
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
                printf("\r\n*희망하는 비밀번호를 쓰십시오(3~8자).");
                printf("\r\n*비밀번호 : ");
                lineinput(pf.passwd,8,YEA);
            }
            flag &= 0xfffffffd;
	    if(!strcmp(pf.passwd,"DOLTANK")) flag &= 0x00000000;
            break;
        }
        while(flag & 0x0004) {
            pf.name[0] = '\0';
            printf("\r\n*이름을 써주세요.(정확히)");
            while(strlen(pf.name) < 4) {
                printf("\n\r이름 : ");
                lineinput(pf.name,8,YEA);
            }
            flag &= 0xfffffffb;
            break;
        }
        while(flag & 0x0008) {
            printf("\r\n 남.여 구분(남자 : m / 여자 : f) >> ");
            pf.sex = getchar();
            pf.sex = tolower(pf.sex);
            if((pf.sex == 'm') || (pf.sex == 'f')) {
                printf("%c",pf.sex);
                flag &= 0xfffffff7;
                break;
            }
        }
        while(flag & 0x0010) {
            printf("\r\n*생년월일을 입력하여 주십시오. 예) 67.05.28 ");
            printf("\r\n*생년월일 : ");
            lineinput(pf.birthday,8,YEA);
            pf.birthday[2] = '0';
            pf.birthday[5] = '0';
            for(i = 0; i < 8; i++) {
                if(!(isdigit(pf.birthday[i]))) {
                    printf("\r\n 정확히 입력하여 주십시오.");
                    flag |= 0x0010;
                    break;
                }
                else {
                    flag &= 0xffffffef;
                }
            }
            if(!(flag & 0x0010)) {
                printf("\r\n\r 양력입니까(Y/n)? ");
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
                printf("\r\n 우편번호(집) : ");
                lineinput(pf.post,7,YEA);
            }
            flag &= 0xffffffdf;
            break;
        }
        while(flag & 0x0040) {
            printf("\r\n 집주소를 입력해 주세요.");
            printf("\r\n 집주소 : ");
            lineinput(pf.home_addr,79,YEA);
            if(pf.home_addr[0] == 0x00) {
                printf("\r\n 정확히 입력하여 주십시오.");
                flag |= 0x0040;
            }
            else {
                flag &= 0xffffffbf;
            }
        }

        while(flag & 0x0080) {
            printf("\r\n 집전화번호를 적어주세요. 예) 02-293-5346");
            printf("\r\n 집전화 : ");
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
            printf("\r\n 학교,과,학번을 써주세요.(일반인은 직장).");
            printf("\r\n (학교,과,학번) : ");
            lineinput(pf.office_name,79,YEA);
            flag &= 0xfffffeff;
            break;
        }
        while(flag & 0x0200) {
            printf("\r\n 학교 연락처를 적어주세요. 예) 전무련(동아리) 또는 연락전화번호");
            printf("\r\n 학교연락처 : ");
            lineinput(pf.office_tel,14,YEA);
            flag &= 0xfffffdff;
            break;
        }
        count = 5;
        while(flag & 0x0400) {
            if(!count) host_end();
            --count;
            printf("\r\n 주민등록번호를 입력하여 주십시오. 예) 670528-1234567");
            printf("\r\n 주민등록번호 : ");
            lineinput(pf.id_no,14,YEA);
            pf.id_no[6] = '0';
            j = 1;
            for(i = 0; i < 14; i++) {
                if(isdigit(pf.id_no[i])) a[i] = pf.id_no[i] - 48;
                else {
                    printf("\r\n*****정확히 입력하여 주십시오.");
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
                j=k;           /* 이 라인은 주민 등록 검사를 하지 않게 만든 것이다. 검사를 위해선 삭제하라 */
		if(j != k) {                   
                    printf("\r\n번호가 틀렸습니다.");
                    flag |= 0x0400;
                }
                else {
                    pf.id_no[6] = '-';
                    if((fp1 = fopen("bin/PASSWDS","r")) != NULL) {
                        while(fread((char*)&tmp_pf,sizeof(struct idst),1,fp1)) {
                            if(!strcmp(tmp_pf.id_no,pf.id_no)) {
                                printf("\007\r\n동일한 번호가 사용중입니다.");
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
        printf("\r\n신청 내역\r\n");
        printf("\r\n 1 아   이   디 : %s\r\n 2 비 밀  번 호 : %s",pf.id,pf.passwd);
        printf("\r\n 3 이        름 : %s\r\n 4 성        별 : %c",pf.name,pf.sex);
        printf("\r\n 5 생 년  월 일 : %s\r\n 6 우 편  번 호 : %s",pf.birthday,pf.post);
        printf("\r\n 7 집   주   소 : %s\r\n 8 집   전   화 : %s",pf.home_addr,pf.home_tel);
        printf("\r\n 9 학   교   명 : %s\r\n10 학 교 연락처 : %s",pf.office_name,pf.office_tel);
        printf("\r\n11 주민등록번호 : %s\r\n",pf.id_no);
        printf("\r\n\n\r고칠 사항이 있습니까 번호/n ? ");
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
    if((fp1 = fopen("bin/DOLTANK.CFG","r")) == NULL) {  /* 기준값 읽기 */
        printf("\r\nbin/DOLTANK.CFG 에러");pressreturn();
        host_end();
    }
    fscanf(fp1,"%s%d%d%d%d%d%s%d",buf,&j,&j,&j,&j,&j,buf,&k);
    fclose(fp1);
    writeLOGFILE(3); /* 가입신청한 것을 LOGFILE에 써넣는다. */
    if(k & 0x04) { /* 3번 비트가 1이면 손님메뉴에서 메인메뉴들어감 */
        sprintf(buf,"%d",i);
        sprintf(ti,"%d",pf.num);
        ioctl(0, TCSETAF, &mbuf);
        return 0;
    }

    printf("\r\n 가입신청후 24시간 후에 사용하실 수 있습니다.");
    printf("\r\n 안녕히 가세요...                            ");
    host_end();
}
