
#include "bbs.h"
/* 동호회 운영자 메뉴 */
circlesysop()
{
    FILE *fp1;
    char buf[80],buf2[80];
    int howmany;

    printf("\r\n 동호회 촌장은 %s입니다.",cmenu->manager);
    /* 촌장이 아니고 운영자가 아니면 사용하지 못한다 */
    if ( strcmp(cmenu->manager,pf.id) && strcmp(pf.id,"sysop") ){
	printf("\r\n 당신은 쓰실 수 없어요. "); pressreturn();
	return;
    }

    while(1) {
    	printf("\r\n (%s)동아리운영자메뉴입니다.",nmenu->ccode);
    	printf("\r\n ---------------------------");
    	printf("\r\n 1. 동아리가입처리");
    	printf("\r\n 2. 탈  퇴  처  리");
    	printf("\r\n 3. 회  원  조  회");
    	printf("\r\n 4. logo 화일 수정");
    	printf("\r\n 5. BBSCMENUS화일 만들기 ");
    	printf("\r\n 6. 회원들에게 편지보내기");
	printf("\r\n 7. 강제가입처리 ");
	sprintf(buf,"circle/%s/CUSER.%s.tmp",nmenu->ccode,nmenu->ccode);
	fp1 = fopen(buf,"r");
	fseek(fp1,0,2); howmany = ftell(fp1) / sizeof(struct idst);
	if(fp1==NULL) howmany = 0;
	fclose(fp1);
	printf("\r\n 8. 가입신청한사람보기(%d)",howmany);
	printf("\r\n 명령>> ");
	keyinput(buf,10);
	if(buf[0]=='p' || buf[0]=='P') break;
	else if(buf[0]=='1') {
	    printf("\r\n 동아리 가입처리 메뉴입니다.");
	    permitmember();
	}
	else if(buf[0] =='2') {
	    printf("\r\n 탈퇴처리메뉴입니다.");
	    printf("\r\n 탈퇴시킬아이디는>> ");
	    keyinput(buf,10);
	    sprintf(buf2,"circle/%s/CUSER.%s",nmenu->ccode,nmenu->ccode);
	    del_user(buf2,buf);
	}
	else if(buf[0]=='3') {
	    printf("\r\n 회원을 조회합니다. ");
	    sprintf(buf,"circle/%s/CUSER.%s",nmenu->ccode,nmenu->ccode);
	    show_user(buf);
	}
	else if(buf[0]=='4') {
	    printf("\r\n 로고화일을 수정합니다.");
	    while(1) {
		printf("\r\n 0:나감 1: 로고화일 보기 2: 로고화일올리기");
		keyinput(buf,5);
		if(buf[0]=='1') {
		    sprintf(buf,"%s.log",nmenu->ccode);
		    cls();clogo(buf,1);
		}
	  	if(buf[0]=='2') {
		    printf("\r\n\r\n 아직은 지원이 되질 않습니다.");
		    printf("\r\n 로고화일을 시솝에게 보내주세요.");
		    printf("\r\n");
		}
		if(buf[0]=='0') {
		    break;
		}
	    }
	}
	else if(buf[0]=='5') {
	    printf("\r\n CMENUS화일을 만듭니다. ");
	    make_cmenu(0);
	}
	else if(buf[0]=='6') {
	    printf("\r\n 회원들에게 편지를 보냅니다.");
	    printf("\r\n 아직 지원되지 않습니다.");
	    printf("\r\n");
	}   
	else if(buf[0]=='7') {
	    printf("\r\n\r\n 강제 가입처리입니다");
	    printf("\r\n 가입시킬 아이디: ");keyinput(buf,10);
	    forcemember(buf);
	}
	else if(buf[0]=='8') {
	    sprintf(buf,"circle/%s/CUSER.%s.tmp",nmenu->ccode,nmenu->ccode);
	    show_user(buf);
	} 
    }
}

/* 가입신청 처리 */
permitmember()
{
    FILE *fp1,*fp2,*fp3;
    char buf[80];
    struct Memo memo;
    struct idst member;
    int i,howmany;
 
    sprintf(buf,"circle/%s/CUSER.%s.tmp",nmenu->ccode,nmenu->ccode);
    if((fp1=fopen(buf,"r+"))==NULL) {
	printf("\r\n 가입신청한 사람이 없습니다.");
	return;
    }
    fseek(fp1,0,2); howmany = ftell(fp1)/sizeof(struct idst);
    printf("\r\n 총 %d 명이 가입신청을 했습니다.",howmany);
    sprintf(buf,"circle/%s/APPLYMEMO.%s",nmenu->ccode,nmenu->ccode);
    fp2=fopen(buf,"r+");
    fseek(fp1,0,0);    
    for(i=0;i<howmany;i++) {
	fseek(fp1,i*sizeof(struct idst),0);
	fread((char*)&member,sizeof(struct idst),1,fp1);
	printf("\r\n 가입신청자");
	printf("\r\n 아이디: %s",member.id);
	printf("\r\n 이름: %s",member.name);
	printf("\r\n 생년월일: %s",member.birthday);
        fread((char*)&memo,sizeof(struct Memo),1,fp2);
	printf("\r\n가입신청메모:%s",memo.content);
  	printf("\r\n 가입을 허락하시겠습니까?(y/N)>>");keyinput(buf,2);
	if(buf[0]=='y'|| buf[0]=='Y') {
	    sprintf(buf,"circle/%s/CUSER.%s",nmenu->ccode,nmenu->ccode);
	    if((fp3=fopen(buf,"r+"))==NULL) {
		fp3=fopen(buf,"a+");
	    }
	    fseek(fp3,0,2);
	    fwrite((char*)&member,sizeof(struct idst),1,fp3);
	    fclose(fp3);
	    printf("\r\n가입처리를 하였습니다.");
	}
    }
    fclose(fp2);
    fclose(fp1);
    printf("\r\n 가입신청이 끝났습니다.");pressreturn();
    sprintf(buf,"circle/%s/CUSER.%s.tmp",nmenu->ccode,nmenu->ccode);
    unlink(buf);
    sprintf(buf,"circle/%s/APPLYMEMO.%s",nmenu->ccode,nmenu->ccode);
    unlink(buf);

}
/* 강제 가입처리 */
forcemember(char*fmember)
{
    FILE *fp1;
    char buf[80];
    struct idst member;
    int howmany,i,check=0;
    
    sprintf(buf,"circle/%s/CUSER.%s.tmp",nmenu->ccode,nmenu->ccode);
    if((fp1=fopen(buf,"r"))!=NULL) {
	printf("\r\n 가입신청란을 확인해봅니다.");
	fseek(fp1,0,2); howmany = ftell(fp1) / sizeof(struct idst);
	for (i=0;i<howmany ; i++) {
	    fseek(fp1,i*sizeof(struct idst),0);
	    fread((char*)&member,sizeof(struct idst),1,fp1);
	    if(!strcmp(fmember,member.id)) {
		printf("\r\n 가입신청을 했습니다.가입처리메뉴에서 처리하세요.");
		check =1 ;
		break;
	    }
	}
    }
    if(check) {
	return;
    }
    fclose(fp1);
    sprintf(buf,"circle/%s/CUSER.%s",nmenu->ccode,nmenu->ccode);
    if((fp1=fopen(buf,"r"))!=NULL) {
	printf("\r\n 동호회회원명단에서 살펴봅니다.");
	fseek(fp1,0,2); howmany=ftell(fp1)/sizeof(struct idst);
	fseek(fp1,0,0);
	for(i=0;i<howmany;i++) {
	    fseek(fp1,i*sizeof(struct idst),0);
	    fread((char*)&member,sizeof(struct idst),1,fp1);
	    if(!strcmp(fmember,member.id)) {
		printf("\r\n 이미 회원입니다. 가입처리를 하면 안됩니다.");
	   	check =1 ;
		break;
	    }
	}
    }
    fclose(fp1);
    if(check) {
	return;
    }
/* 여기까지 오면 가입신청도, 회원도 아닌 놈이다 */
    gaipok(fmember);
    pressreturn();
}
        	

gaipok(char*member)
{
    FILE *fp1;
    char buf[80];
    struct idst user;
    int howmany,i,check=0;
 
    sprintf(buf,"bin/PASSWDS");
    fp1 = fopen(buf,"r");
    fseek(fp1,0,2); howmany = ftell(fp1) / sizeof(struct idst);
    fseek(fp1,0,0);
    for(i=0;i<howmany;i++) {
	fread((char*)&user,sizeof(struct idst),1,fp1);
	if(!strcmp(user.id,member)) {
	    printf("\r\n %s회원의 정보를 찾았습니다.",user.id);
	    check = 1;break;
	}
     }
     fclose(fp1);
     if(check) {
	printf("\r\n 회원을 가입시킵니다.");
 	sprintf(buf,"circle/%s/CUSER.%s",nmenu->ccode,nmenu->ccode);
	if((fp1=fopen(buf,"r+"))==NULL) {
	    fp1=fopen(buf,"a+");
	}
	fseek(fp1,0,2);
	fwrite((char*)&user,sizeof(struct idst),1,fp1);
	fclose(fp1);
	printf("\r\n 가입처리를 마쳤습니다.");
    }
    else {
	printf("\r\n 그런 회원은 없습니다.");
    }
}
         
showmember()
{
    FILE *fp1;
    char buf[80],whom[10];
    int i,howmany;
    struct idst member;
    while(1) {
	printf("\r\n 0.나감 1.가입순으로 2.아이디로 3.이름으로 >>");
	keyinput(buf,10);
	if(buf[0]=='1') {
            sprintf(buf,"circle/%s/CUSER.%s",nmenu->ccode,nmenu->ccode);
            fp1=fopen(buf,"r");
            fseek(fp1,0,2); howmany=ftell(fp1)/sizeof(struct idst);
            fseek(fp1,0,0);
            for(i=0;i<howmany;i++){
	        fread((char*)&member,sizeof(struct idst),1,fp1);
	        printf("\r\n %s의 정보입니다.",member.id);
	        printf("\r\n 아이디:%s, 이름:%s, 생일:%s",\
		  member.id,member.name,member.birthday);
		printf("\r\n 중단하시려면 q를 누르세요.");
 	        keyinput(buf,1);if(buf[0]=='q') break;
	    }
	    fclose(fp1);
        }
	else if(buf[0]=='2') {
	    printf("\r\n찾으시려는아이디: ");keyinput(whom,10);
	    sprintf(buf,"circle/%s/CUSER.%s",nmenu->ccode,nmenu->ccode);
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
            sprintf(buf,"circle/%s/CUSER.%s",nmenu->ccode,nmenu->ccode);
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
	
/* 회원 가입신청메뉴*/

circleapply()
{
    char buf[80],tmp[80];
    cls();
    esc_sub=0;
    while(1) {
    	printf("\r\n 동아리가입신청란입니다.");
    	printf("\r\n -----------------------");
    	printf("\r\n 1. 가입을 신청합니다. ");
    	printf("\r\n 2. 탈퇴하겠습니다. ");
    	printf("\r\n 3. 회원을 구경해봅니다.");
	printf("\r\n 명령>> ");
	keyinput(buf,10);
	if (buf[0]=='1') {
	    printf("\r\n 가입을 하실려고 하십니다.");
	    printf("\r\n 가입을 하시겠습니까?");
	    keyinput(buf,1,YEA);
	    if(buf[0]=='y' || buf[0]=='Y') {
		gaip(nmenu->ccode); /* nmenu->r은 동호회 번호이다.*/
	    }
	    else {
		printf("\r\n 가입을 안하셨습니다.");
	    }
	}
	else if(buf[0]=='2') {
	    printf("\r\n 탈퇴를 하실려고 하십니다.");
	    printf("\r\n 탈퇴를 하시겠습니까?(y/N)>>");
	    printf("\r\n 아직 지원되지 않습니다.");
	    keyinput(buf,1,YEA);
	    if(buf[0]=='y' || buf[0] =='Y') {
	 	taltaoe(nmenu->r); /* nmenu->r은 동호회 번호이다 */
	    }
	    else {
		printf("\r\n 탈퇴 취소입니다.");
	    }
	}
	else if(buf[0]=='3') {
	    printf("\r\n 동호회 사람들을 보여줍니다.");
	    sprintf(buf,"circle/%s/CUSER.%s",nmenu->ccode,nmenu->ccode);
	    show_user(buf);
	}
	else {
	    cdomenucommand(buf,applycmd);
	}
	if(esc_sub) break;
    }
    pressreturn();
}

gaip(char*circlename)
{
    FILE *fp1;
    char buf[80];
    struct Memo applymemo;
    int howmany,i,check=0;
    struct idst member;

    sprintf(buf,"circle/%s/CUSER.%s",nmenu->ccode,circlename);
    if((fp1=fopen(buf,"r+"))==NULL) {
	fp1=fopen(buf,"a+");
    }
    fseek(fp1,0,2);howmany=ftell(fp1) / sizeof(struct idst);
    fseek(fp1,0,0);
    for(i=0;i<howmany;i++) {
	fread((char*)&member,sizeof(struct idst),1,fp1);
 	if(!strcmp(member.id,pf.id)) {
	    check=1;
	    break;
	}
    }
    fclose(fp1);
    if(check) {
	printf("\r\n 당신은 이미 회원입니다.");
	return;
    }
    check=0;
    sprintf(buf,"circle/%s/CUSER.%s.tmp",nmenu->ccode,circlename);
    if((fp1=fopen(buf,"r+"))==NULL) {
	fp1=fopen(buf,"a+");
    }
    fseek(fp1,0,2);howmany=ftell(fp1)/sizeof(struct idst);
    fseek(fp1,0,0);
    for(i=0;i<howmany;i++) {
	fread((char*)&member,sizeof(struct idst),1,fp1);
	if(!strcmp(member.id,pf.id)) {
	    check=1;
	    break;
	}
    }
    fclose(fp1);
    if(check) {
	printf("\r\n 당신은 이미 가입신청을 하였습니다.");
	return;
    }
    sprintf(buf,"circle/%s/APPLYMEMO.%s",nmenu->ccode,circlename);
    if((fp1=fopen(buf,"r+"))==NULL) {
	fp1= fopen(buf,"a+");
    }
    fseek(fp1,0,2);
    printf("\r\n 가입희망쪽지를 적어주세요.(70자내외로)\r\n");
    keyinput(applymemo.content,70);
    sprintf(applymemo.id,pf.id);
    fwrite((char*)&applymemo,sizeof(struct Memo),1,fp1);
    fclose(fp1);
 
    sprintf(buf,"circle/%s/CUSER.%s.tmp",nmenu->ccode,circlename);
    if( (fp1=fopen(buf,"r+"))==NULL) {
	fp1 = fopen(buf,"a+");
    }
    fseek(fp1,0,2);
    fwrite((char*)&pf,sizeof(struct idst),1,fp1);
    fclose(fp1);
    printf("\r\n 가입신청이 끝났습니다.가입여부는 촌장이 결정합니다.");
}
taltaoe(int circlenum)
{
}
list_member(int circlenum)
{
}
get_cmenu()
{
    int i, j;
    char ch[2];
    FILE *fp1,*fp2;
    char buf[80];
    sprintf(buf,"circle/BBSCMENUS.%s",nmenu->ccode);
     
    if((fp1 = fopen(buf,"r")) == NULL) {
	printf("\r\n 메뉴화일이 열리지 않습니다.");
	sprintf(buf,"circle/CMENUS.%s",nmenu->ccode);
	if((fp2=fopen(buf,"r"))!=NULL) {
	    printf("\r\n 구버젼 메뉴화일이 있습니다. 컨버팅하시겠습니까?(y/N) ");
	    keyinput(buf,1);
	    if(buf[0]=='y'||buf[0]=='Y') {
	 	convertoldcmenu();
	    }
	}
	else {
            printf("\r\n메뉴화일을 만드시겠습니까?(y/N)>> ");
            lineinput(buf,1,YEA);
            if(buf[0]=='y' || buf[0]=='Y') make_cmenu(1);
	}
        host_end();
    }
    else {
        if((cmtmp = (struct cmenustruct *)malloc(sizeof(struct cmenustruct))) == NULL) {
            printf("\r\n메모리 부족");
            exit(1);
        }
        cmenu = cmtop = cnmenu = cmtmp;
        fread((char*)cmtmp,sizeof(struct cmenustruct),1,fp1);
        if((cmtmp = (struct cmenustruct *)malloc(sizeof(struct cmenustruct))) == NULL) {
            printf("\r\n메모리가 부족");
            exit(1);
        }
        cnmenu->neqp = cnmenu->aeqp = NULL;
        cnmenu->upp = cnmenu;
        cnmenu->downp = cmtmp;
        while(fread((char*)cmtmp,sizeof(struct cmenustruct),1,fp1)) {
            if(cnmenu->deep < cmtmp->deep) {
                cnmenu->downp = cmtmp;
                cmtmp->upp = cnmenu;
                cnmenu->neqp = cmtmp->aeqp = NULL;
            }
            else if(cnmenu->deep == cmtmp->deep) {
                cnmenu->downp = NULL;

                cnmenu->neqp = cmtmp;
                cmtmp->upp = cnmenu->upp;
                cmtmp->aeqp = cnmenu;
            }
            else if(cnmenu->deep > cmtmp->deep) {
                cnmenu->downp = NULL;
                cnmenu->neqp = NULL;
                j = cnmenu->deep - cmtmp->deep;
                for(i = 0; i < j; i++) {
                    cnmenu = cnmenu->upp;
                }
                cnmenu->neqp = cmtmp;
                cmtmp->upp = cnmenu->upp;
                cmtmp->neqp = cmtmp->downp = NULL;
                cmtmp->aeqp = cnmenu;
            }
            cnmenu = cmtmp;
            if((cmtmp=(struct cmenustruct *)malloc(sizeof(struct cmenustruct))) == NULL) {
                printf("\r\n메모리 부족");
                exit(1);
            }
        }
        fclose(fp1);
        free(cmtmp);
    }
}
make_cmenu(int start)
{
    int i, j = 0, key = 1;
    char ch, buf[80];
    if(start) { 
        if((cmtmp=(struct cmenustruct *)malloc(sizeof(struct cmenustruct)))\
         ==NULL) {
            printf("\r\n메모리가 부족하네요.");
            exit(1);
        }
        cmenu=cmtop=cnmenu=cmtmp;
        if((cmtmp=(struct cmenustruct *)malloc(sizeof(struct cmenustruct)))\
         ==NULL) {
            printf("\r\n메모리가 부족하네요.");
            exit(1);
        }
        cnmenu->neqp=cnmenu->aeqp=NULL;
        cnmenu->upp=cnmenu;
        cnmenu->downp=cmtmp;
        cnmenu->form='M'; cnmenu->level=cnmenu->del=cnmenu->w= cnmenu->r =\
                cnmenu->deep=0;
        strcpy(cnmenu->ccode,"dongari");
        strcpy(cnmenu->list,"동아리");
        strcpy(cnmenu->file,"");
        cmtmp->neqp=cmtmp->aeqp=cmtmp->downp=NULL;
        cmtmp->upp=cmtop;
    }
    while(1) {
        if(key) {
	    cls();
            printf("\r\n  %s      [%s]\r\n",cmtop->list,cmtop->ccode);
            cnmenu = cmtop->downp;
            j = 0;
            printf("\r\nnum 선택 F 타이틀      가기코드   읽기  쓰기    삭제    증가");
            while(cnmenu->neqp != NULL) {
                printf("\r\n  %2d %3s %c %10s [%-8s]",\
                        ++j,cnmenu->choice,cnmenu->form,cnmenu->list,cnmenu->ccode);
                printf(" %5d %5d : %x : %d",\
                        cnmenu->w,cnmenu->r,cnmenu->del,cnmenu->level);
                if(nmenu->form == 'f' || nmenu->form == 'F') \
                        printf(" %s",cnmenu->file);
                cnmenu = cnmenu->neqp;
            }
            printf("\r\n  %2d %3s %c %10s [%-8s]",\
                ++j,cnmenu->choice,cnmenu->form,cnmenu->list,cnmenu->ccode);
            printf(" %5d %5d : %x : %d",\
                cnmenu->w,cnmenu->r,cnmenu->del,cnmenu->level);          
            if(nmenu->form == 'f' || nmenu->form == 'F') \
                printf(" %s",cnmenu->file);
            printf(\
"\r\n\n\r번호 상위[P] 수정[E] 추가[A] 삽입[I] 삭제[D] 저장[S] 종료[X] >> ");
        }
        else {
            i = strlen(buf);
            for(j = 0; j < i; j++) {
                putchar('\b'); putchar(' '); putchar('\b');
            }
        }
        key = 1;
        lineinput(buf,10,YEA);
        i = atoi(buf);
        if(i) {
            cnmenu = cmtop->downp;
            for(j = 1; j < i; j++) {
                if(cnmenu->neqp != NULL) cnmenu = cnmenu->neqp;
                else break;
            }
            if(cnmenu->form == 'M') {
                cmtop = cnmenu;
            }
            else key = 0;
        }
        else if(buf[0] == 'p' || buf[0] == 'P') {
            cmtop = cmtop->upp;
        }
        else if(buf[0] == 'e' || buf[0] == 'E') {
            buf[0] = ' ';
            i = atoi(buf);
            if(i) {
                cnmenu = cmtop->downp;
                for(j = 1; j < i; j++) {
                    if(cnmenu->neqp != NULL) {
                        cnmenu = cnmenu->neqp;
                    }
                    else {
                        break;
                    }
                }
            }
            else {
                cnmenu = cmtop;
            }
            printf("\r\n <%s> [%s]",cnmenu->list,cnmenu->ccode);
            edit_cline();
        }
        else if(buf[0] == 'a' || buf[0] == 'A') {
            cnmenu = cmtop->downp;
            while(cnmenu->neqp != NULL) cnmenu = cnmenu->neqp;
            append_cline();
        }
        else if(buf[0] == 'i' || buf[0] == 'I') {
            buf[0] = ' ';

            i = atoi(buf);
            if(i == 1) {
                if((cmtmp = (struct cmenustruct *)\
                malloc(sizeof(struct cmenustruct))) == NULL) {
                    printf("\r\n메모리가 모자랍니다.");
                    exit(1);
                }
                cmtmp->neqp = cmtop->downp;
                cmtmp->downp = NULL;
                cmtop->downp = cmtmp;
                cmtmp->upp = cmtop;
                cnmenu = cmtmp;
                edit_cline();
            }
            else if(i) {
                cnmenu = cmtop->downp;
                for(j = 2; j < i; j++) {
                    if(cnmenu->neqp != NULL) {
                        cnmenu = cnmenu->neqp;
                    }
                    else {
                        break;
                    }
                }
                if((cmtmp = (struct cmenustruct *)\
                        malloc(sizeof(struct cmenustruct))) == NULL) {
                    printf("\r\n메모리가 모자랍니다.");
                    exit(1);
                }
                cmtmp->downp = NULL;
                cmtmp->neqp = cnmenu->neqp;
                cmtmp->upp = cnmenu->upp;
                cnmenu->neqp = cmtmp;
                cnmenu = cmtmp;
                edit_cline();
            }
        }
        else if(buf[0] == 'd' || buf[0] == 'D') {  /* 메뉴 삭제 */
            buf[0] = ' ';
            i = atoi(buf);
            if(i == 1) {
                cnmenu = cmtop->downp;
                cmtop->downp = cnmenu->neqp;
                free(cnmenu);
                if(cmtop->downp == NULL) cmtop = cmtop->upp;
            }
            else if(i > 1) {
                cnmenu = cmtop->downp;
                for(j = 1; j < i; j++) {
                    if(cnmenu->neqp != NULL) {
                        cmtmp = cnmenu;
                        cnmenu = cnmenu->neqp;
                    }
                    else {
                        break;
                    }
                }
                cmtmp->neqp = cnmenu->neqp;
                free(cnmenu);
            }
        }
        else if(buf[0] == 's' || buf[0] == 'S') {
            save_cmenu();
        }
        else if(buf[0] == 'x' || buf[0] == 'X') break;
        else key = 0;

    }
}


edit_cline()
{
    char buf[80];
    printf("\r\n 선택번호: ");keyinput(buf,5);
    if(buf[0]!='\0') sprintf(cnmenu->choice,buf);
    printf("\r\n\r 형태 : ");
    keyinput(buf,1);
    if(buf[0]!='\0') cnmenu->form = buf[0];
    printf("\r\n\r 가기코드 : ");
    keyinput(buf,9);
    if(buf[0]!='\0') sprintf(cnmenu->ccode,"%s",buf);
    printf("\r\n\r 메뉴명 : ");
    keyinput(buf,39);
    if(buf[0]!='\0') sprintf(cnmenu->list,"%s",buf);
    if((cnmenu->form == 'M') && (cnmenu->downp == NULL)) {
        sub_ccreat(); /* M메뉴 새로 만들 때 */
    }
    else {
        printf("\r\n 쓰기레벨 : ");
        keyinput(buf,8);
        cnmenu->w = atoi(buf);
        printf("\r\n 읽기레벨 : ");
        keyinput(buf,8);
        cnmenu->r = atoi(buf);
        if(cnmenu->form == 'L' || cnmenu->form == 'l' ||
          cnmenu->form == 'V' || cnmenu->form == 'v') {
            printf("\r\n 레벨/게시물 : ");
            keyinput(buf,2);
            cnmenu->level = atoi(buf);
        
        }
        else if(nmenu->form == 'F' || nmenu->form == 'f') {
            printf("\r\n레벨/화일 : ");
            keyinput(buf,8);
            cnmenu->level = atoi(buf);
            printf("\r\n삭제권한 플래그 : ");
            keyinput(buf,3);
            cnmenu->del = atoi(buf);
            printf("\r\n 화일위치디렉토리: ");
            lineinput(buf,29,YEA);
            strcpy(cnmenu->file,buf);
        }
        printf("\r\n 게시판담당자(없으면Enter): ");
        keyinput(buf,10);
	sprintf(cnmenu->manager,"%s",buf);
    }
}

append_cline()
{
    if((cmtmp = (struct cmenustruct *)\
         malloc(sizeof(struct cmenustruct))) == NULL) {
        printf("\r\n메모리가 모자랍니다.");
        exit(1);
    }
    cnmenu->neqp = cmtmp;
    cmtmp->upp = cnmenu->upp;
    cnmenu = cmtmp;
    cnmenu->downp = cnmenu->neqp = NULL;
    edit_cline();
}
sub_ccreat()
{
    if((cmtmp = (struct cmenustruct *)\

        malloc(sizeof(struct cmenustruct))) == NULL) {
        printf("\r\n메모리가 모자랍니다.");
        exit(1);
    }
    cnmenu->downp = cmtmp;
    cmtmp->upp = cnmenu;
    cnmenu = cmtmp;
    cnmenu->downp = cnmenu->neqp = NULL;
    edit_cline();
}

save_cmenu()
{
    int i = 0;
    char buf[80],buf2[80],buf3[80];
    FILE *fp1;
    cmtop = cnmenu = cmenu;
    sprintf(buf,"circle/BBSCMENUS.%s",nmenu->ccode);
    sprintf(buf2,"circle/BBSCMENUS.%s.backup",nmenu->ccode);
    sprintf(buf3,"mv %s %s",buf,buf2);
    system(buf3);
    sprintf(buf,"%s/circle/BBSCMENUS.%s",getenv("HOME"),nmenu->ccode);
    if((fp1 = fopen(buf,"a+")) != NULL) {
        cmtop->deep = 0;
        fwrite((char*)cnmenu,sizeof(struct cmenustruct),1,fp1);
        printf("\r\n%8s %s",cnmenu->ccode,cnmenu->list);
        cnmenu = cnmenu->downp;
        ++i;
        while(cnmenu != cmtop) {
            cnmenu->deep = i;
            fwrite((char*)cnmenu,sizeof(struct cmenustruct),1,fp1);
            printf("\r\n%8s %s",cnmenu->ccode,cnmenu->list);
            if(cnmenu->downp == NULL) {
                if(cnmenu->neqp == NULL) {
                    while(1) {
                        cnmenu = cnmenu->upp;
                        --i;
                        if(cnmenu->neqp != NULL || i == 0) break;
                    }
                    if(i) cnmenu = cnmenu->neqp;
                }
                else {
                    cnmenu = cnmenu->neqp;
                }
            }
            else {
                cnmenu = cnmenu->downp;
                ++i;
            }
        }
        fclose(fp1);
    }
    else printf("\r\n화일이 열리지 않습니다.");
}



int chose_cmenu()  /* 메뉴 내용 검사 */
{
    int key = 1;
    char buf[80], c, tmp[80], crt[3],inc[10];
    if(cnmenu->form == 'R') {  /* 게시물이실행화일.이것은 기본제공기능 */
        if(cnmenu->w <= pf.level) {
	    sprintf(buf,"%s.log",cnmenu->ccode);
            clogo(buf,1);
            if(!strcmp(cnmenu->ccode,"chat")) { /* 대화실 */
                chat();
		cnmenu=cnmenu->upp;
            }
            else if(!strcmp(cnmenu->ccode,"Apply")) {
                circleapply();
                cnmenu=cnmenu->upp;
            }
	    else if(!strcmp(cnmenu->ccode,"Manage")) {
		circlesysop();
		cnmenu=cnmenu->upp;
	    }
	    else if(!strcmp(cnmenu->ccode,"newrmail")) {
		cls();
		newrmail();
		if(upermenu==1) {cnmenu=cnmenu->upp;upermenu=0;}
	    }
	    else if(!strcmp(cnmenu->ccode,"newwmail")) {
		cls();
		printf("\r\n 편지 수신 아이디를 적으세요.");
		printf("\r\n 아이디(ex. hommage sysop) >> ");
		keyinput(buf,80);
		newwmail(buf);
		cnmenu=cnmenu->upp;
	    }
	    else if(!strcmp(cnmenu->ccode,"newcmail")) {
		cls();
		newcmail();
		cnmenu=cnmenu->upp;upermenu=0;
	    }
	    else if(!strcmp(cnmenu->ccode,"mailist")) {
		cls();
		editmailist("mail");
		cnmenu= cnmenu->upp;
	    }
	    else if(!strcmp(cnmenu->ccode,"profile")) {
		newprofile();
		cnmenu=cnmenu->upp;
	    }
        }
        else {
            mess_dis(4);
            lineinput(buf,1,YEA);
            cnmenu = cnmenu->upp;
            key = 1;
        }
    }
    else if(cnmenu->form == 'L' || (cnmenu->form == 'l' && circlecheck)) { 
        /* 게시물 */
	sprintf(buf,"%s.log",cnmenu->ccode);
        clogo(buf,1);
	sprintf(buf,"circle/%s/boards",nmenu->ccode);
        show_board(buf,cnmenu->ccode,cboardcmd);
	if(upermenu==1) {cnmenu=cnmenu->upp;upermenu=0;}
    }
    else if(cnmenu->form == 'F' || (cnmenu->form == 'f' && circlecheck) ) {  
        /* 화일 게시물 */
	sprintf(buf,"%s.log",cnmenu->ccode);
        clogo(buf,1);
	sprintf(buf,"circle/%s/fboards",nmenu->ccode);
        show_board(buf,cnmenu->ccode,cfilecmd);
	if(upermenu==1) {cnmenu=cnmenu->upp;upermenu=0;}
    }
    else cnmenu = cnmenu->upp;
    return(key);
}

void cmenu_free()
{
    cnmenu = cmenu->downp;
    while(cnmenu != cmenu) {
        if(cnmenu->neqp != NULL) {
            cnmenu = cnmenu->neqp;
        }
        else {
            if(cnmenu->downp != NULL) {
                cnmenu = cnmenu->downp;
            }
            else {
                cmtmp = cnmenu;
                cnmenu = cnmenu->upp;
                if(cnmenu->downp == cmtmp) {
                    free(cmtmp);
                    cnmenu->downp = NULL;
                }
                else {
                    cnmenu = cnmenu->downp;
                    while(cnmenu->neqp != cmtmp) {
                        cnmenu = cnmenu->neqp;
                    }
                    free(cmtmp);
                    cnmenu->neqp = NULL;
                }
            }
        }
    }
    free(cmenu);
}

