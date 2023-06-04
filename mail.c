#include "bbs.h"
/* 이것은 일반 메일용으로 따로 만든 것이다. */

wmail()
{ 
    FILE *fp1;
    char towhom[100];
    char direc[80];
    char buf[80];
    int check=0;
    struct mailist mailgroup;
    printf("\r\n 편지쓰기(WMAIL)");
    printf("\r\n 주소록에 등록된 사람에게 보내려면 @주소록 쓰시면 되고");
    printf("\r\n 다중으로 보내려면 --> 아이디 아이디 아이디 ... 쓰시면 됩니다.");
    printf("\r\n 아이디(예: hommage sysop)>> ");
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
	    printf("\r\n 주소록에 없군요.");pressreturn();
	    return;
	}
    }
    strcpy(direc,"letter");
    wmailto(direc,towhom);
}
/* 이것은 오더메일,인콰이어리메일,기타메일로 구분된 업무용 메일 */
newwmail()
{
    char towhom[100];
    char direc[80];
    char buf[80];
    printf("\r\n 보낼 사람 아이디(예: hommage sysop)>> ");
    keyinput(towhom,100);
    while(1) {
        printf("\r\n 편지의 종류를 고르세요(0:취소 1:order 2:inquiry 3.etc) ");
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
	    printf("\r\n 취소하셨습니다.");
	    return;
	}
    }
    wmailto(direc,towhom);
}
/* 이것은 보통 메일읽기 */
rmail()
{
    char buf[80];
    char direc[80];
    strcpy(direc,"letter");
    sprintf(buf,"%s.mail",pf.id);
    show_mail(direc,buf,mailcmd);
}

/* 이것은 업무용 메일 */
 
newrmail()
{
    char buf[80];
    char direc[80];
    while(1) {
    	printf("\r\n 어떤 편지를 보시겠습니까(0:취소 1:order 2:inquiry 3:etc) ");
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
	    printf("\r\n 취소를 하셨습니다.");
	    return;
	}
    }
    sprintf(buf,"%s.mail",pf.id);
    show_mail(direc,buf,mailcmd);
}

        
/* 편지를 보내는 기본적인 cell function */
    
char mailee[30][20];
wmailto(char *direc, char *towhom)
{
    FILE *fp1;
    int howmany=0,i,num,check;
    char buf[255];
    struct idst userid;
    struct dis_list mailhead;  /* 메일 화일헤더까지 통합한다 */
    time_t t; /* 이것은 시간을 정할 때 쓰는 변수  */ 
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
            printf("\r\n %s는 정상이용자가 아닙니다.",argument[i]);
        }
        fclose(fp1);
    }
    if (howmany==0) {
	printf("\r\n 편지를 보낼 사람이 없습니다.");
  	pressreturn(); return;
    }
    for(i=0;i<3;i++) {   
	printf("\r\n      +---------+---------+---------+");
	printf("\r\n 제목: ");keyinput(buf,30);
	if(buf[0]=='\0') printf("\r\n 제목을 입력하세요.");
	else break;
    }	
    if(buf[0]=='\0') {
	printf("\r\n 편지쓰기를 취소합니다. ");
	pressreturn(); reprint=YEA; return;
    }
    else sprintf(mailhead.title,"%s",buf);

    time(&t);    		
    sprintf(mailfilename,"%s%d.txt",pf.id,t);
    if(!strcmp(direc,"oletter")) { /* 주문 편지일 경우에는 특별한 형식 */
	check=writeorderform(direc,mailfilename);
    }
    else check=writetextfile(direc,mailfilename);
    if(!check) {
	printf("\r\n 편지쓰기를 취소합니다.");
	pressreturn(); reprint=YEA;return;
    }
    for (i=0;i<howmany;i++) {
  	/* 메일 헤더화일 작성 */
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
	printf("\r\n %s님에게 편지를 보냈습니다.",mailee[i]);
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
    
/* cmail은 수정이 많이 필요한 부분이다. 나중에 수정한다 */
void cmail()  /* 보낸 편지 확인 */
{
    int i, top_num, back, key,start,howmany;
    char buf[80],direc[80],fhead[80];
    FILE *fp1;
    int LEN=15; /* 출력되는 목록의 갯수 */
    sprintf(direc,"letter");
    sprintf(fhead,"%s.chk",pf.id);

    if(!check_allletter(direc,fhead)) {
	printf("\r\n 보낸 편지가 없습니다.");
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
            printf("\r\n엔터,f:다음화면 b:이전화면 상위[P] 초기[T] : ");
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
    printf("\r\n 아직 지원되지 않습니다.");
    pressreturn();
}

/* 편지를 읽었는 지 모두 확인해서 헤더화일에 기록한다 
    메일체크에 들어갈 때 한번은 꼭 거치는 단계 */
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
	printf("\r\n 체크 헤더가 없습니다.");
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
    if(check==1) return 1; /* 정상적으로 종료 */
    else return 0;
}

/* 확인 리스트 */
list_chk(char *direc,char *fhead,int start, int end)
{
    FILE *fp1;
    char buf[80];
    int i,howmany;
    struct tm *tp, *localtime();
    struct mail_dis mailchk;

    sprintf(buf,"%s/%s",direc,fhead);
    if((fp1 = fopen(buf,"r"))==NULL) {
	printf("\r\n 헤더가 안 열립니다.");
	return 0;
    }
    fseek(fp1,0,2);howmany=ftell(fp1) / sizeof(struct mail_dis);
    fseek(fp1,0,0);
    printf("  아이디   보낸날짜  수신여부           제      목\r\n");
    for(i = start; i < end; i++) {
        if(i>howmany) {
            break;
        }
        else {
	    fseek(fp1,(i-1)*sizeof(struct mail_dis),0);
            fread((char*)&mailchk,sizeof(struct mail_dis),1,fp1);
            tp = localtime(&mailchk.tf);
            printf("\r\n %-8s  %2d월%2d일  ",mailchk.id,
            tp->tm_mon+1,tp->tm_mday);
            if(mailchk.date == 0) {
                printf("[안읽음]%s",mailchk.title);
            }
            else {
                tp = localtime(&mailchk.date);
                printf("**읽음**%s",mailchk.title);
  /* %2d월%2d일 %s",tp->tm_mon+1,tp->tm_mday,mailchk.title); */
            }
        }
    }
    fclose(fp1);
    return i;  /* 마지막 번호를 리턴한다 */
}


dis_top()
{
    printf("");
    printf("%d번 %-9s %s\r\n\n",letter.num,letter.id,letter.title);
}

/* 주소록 작성이다 */
editmailist(char*cmd)
{
    FILE *fp1;
    char buf[80],buf2[80];
    struct mailist mailgroup;
    int fpos;

    sprintf(buf,"user/mail%s.mil",pf.id); /* 각자의 주소록을 연다. */
    if((fp1 = fopen(buf,"r+"))==NULL) {
	fp1 = fopen(buf,"a+");
    }
    if(fp1==NULL) {
	printf("\r\n 파일이 안 열립니다.");
	return;
    }
    while(1) {
	printf("\r\n 메일링리스트메뉴입니다.");
	printf("\r\n 하실일을 골라주세요. ");
	printf("\r\n (1) 수정 (2) 만들기 (3) 보여주기 (0)빠져나가기");
	printf("\r\n 하실일>> ");
 	keyinput(buf,10);
	if(buf[0]=='1') {
	    printf("\r\n 수정할 그룹을 적어주세요.>> ");
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
			printf("\r\n 수정메뉴입니다.");
			printf("\r\n (1) 추가 (2) 삭제 (0) 나가기");
			printf("\r\n >> ");
			keyinput(buf,10);
			if(buf[0]=='1') {
			    if(strlen(mailgroup.towhom)>990) {
				printf("\r\n 인원을 초과했습니다. ");
				break;
			    }
			    printf("\r\n 추가할 인원을 적어주세요.");
			    printf("\r\n 추가>> ");
			    keyinput(buf,80);
			    sprintf(buf2," %s",buf);
			    strcat(mailgroup.towhom,buf2);
			    fseek(fp1,fpos,0);
			    fwrite((char*)&mailgroup,sizeof(struct mailist),\
			      1,fp1);
			    printf("\r\n 추가가 끝났습니다.");
			}
			if(buf[0]=='2') {
			    printf("\r\n 아직 지원되지 않습니다.");
			}
			if(buf[0]=='0') {
			    printf("\r\n 나갑니다.");
			    break;
			}
		    }
		}
	    }
	    printf("\r\n 수정메뉴에서 나갑니다.");
	}
	else if(buf[0]=='2') {
	    while(1) {
	        printf("\r\n 주소록 이름-> ");
	        keyinput(buf,10);
		fseek(fp1,0,0);
		/* 주소록에 있는 것인가 확인한다. */
		while(fread((char*)&mailgroup,sizeof(struct mailist),1,fp1)) {
		    sprintf(buf2,"@%s",buf);
		    if(!strcmp(buf2,mailgroup.group)) {
			printf("\r\n 주소록에 같은 그룹이 있습니다.");
			printf("\r\n 주소록만들기에서 나갑니다.");
			pressreturn();
			return;
		    }
		}
	        if(buf[0]!='\0') {
		    sprintf(mailgroup.group,"@%s",buf);
		    printf("\r\n 주소록에 추가될 사람을 써주세요.");
		    printf("\r\n (e.g hommage sysop) >> ");
		    keyinput(buf,80);
		    sprintf(mailgroup.towhom,"%s",buf);
		    fseek(fp1,0,2);
		    fwrite((char*)&mailgroup,sizeof(struct mailist),1,fp1);
		    printf("\r\n 주소록에 추가했습니다.");
		    break;
		}
	        printf("\r\n 주소록 입력을 하시겠습니까?(y/N) ");
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
	    printf("\r\n 나갑니다.");
	    break;
	}
    }
    fclose(fp1);
    printf("\r\n 주소록관리에서 나갑니다.");
    pressreturn();

}

editMailist(char*cmd)
{
    FILE *fp1;
    char buf[80],buf2[80];
    struct mailist mailgroup;
    int fpos;

    sprintf(buf,"user/MAILIST"); /* 전체주소록 연다. */

    if((fp1 = fopen(buf,"r+"))==NULL) {
        fp1 = fopen(buf,"a+");
    }
    if(fp1==NULL) {
        printf("\r\n 파일이 안 열립니다.");
        return;
    }
    while(1) {
        printf("\r\n 메일링리스트메뉴입니다.");
        printf("\r\n 하실일을 골라주세요. ");
        printf("\r\n (1) 수정 (2) 만들기 (3) 보여주기 (0)빠져나가기");
        printf("\r\n 하실일>> ");
        keyinput(buf,10);
        if(buf[0]=='1') {
            printf("\r\n 수정할 그룹을 적어주세요.>> ");
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
                        printf("\r\n 수정메뉴입니다.");
                        printf("\r\n (1) 추가 (2) 삭제 (0) 나가기");
                        printf("\r\n >> ");
                        keyinput(buf,10);
                        if(buf[0]=='1') {
                            if(strlen(mailgroup.towhom)>990) {
                                printf("\r\n 인원을 초과했습니다. ");
                                break;
                            }
                            printf("\r\n 추가할 인원을 적어주세요.");
                            printf("\r\n 추가>> ");
                            keyinput(buf,80);
                            sprintf(buf2," %s",buf);
                            strcat(mailgroup.towhom,buf2);
                            fseek(fp1,fpos,0);
                            fwrite((char*)&mailgroup,sizeof(struct mailist),\
                              1,fp1);
                            printf("\r\n 추가가 끝났습니다.");
                        }
                        if(buf[0]=='2') {
                            printf("\r\n 아직 지원되지 않습니다.");
                        }
                        if(buf[0]=='0') {
                            printf("\r\n 나갑니다.");
                            break;
                        }
                    }
                }
            }
            printf("\r\n 수정메뉴에서 나갑니다.");
        }
        else if(buf[0]=='2') {
            while(1) {
                printf("\r\n 주소록 이름-> ");
                keyinput(buf,10);
                fseek(fp1,0,0);
                /* 주소록에 있는 것인가 확인한다. */
                while(fread((char*)&mailgroup,sizeof(struct mailist),1,fp1)) {
                    sprintf(buf2,"!%s",buf);
                    if(!strcmp(buf2,mailgroup.group)) {
                        printf("\r\n 주소록에 같은 그룹이 있습니다.");
                        printf("\r\n 주소록만들기에서 나갑니다.");
                        pressreturn();
                        return;
                    }
                }
                if(buf[0]!='\0') {
                    sprintf(mailgroup.group,"@%s",buf);
                    printf("\r\n 주소록에 추가될 사람을 써주세요.");
                    printf("\r\n (e.g hommage sysop) >> ");
                    keyinput(buf,80);
                    sprintf(mailgroup.towhom,"%s",buf);
                    fseek(fp1,0,2);
                    fwrite((char*)&mailgroup,sizeof(struct mailist),1,fp1);
                    printf("\r\n 주소록에 추가했습니다.");
                    break;
                }
                printf("\r\n 주소록 입력을 하시겠습니까?(y/N) ");
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
            printf("\r\n 나갑니다.");
            break;
        }
    }
    fclose(fp1);
    printf("\r\n 주소록관리에서 나갑니다.");
    pressreturn();

}


