
#include "bbs.h"
/* ��ȣȸ ��� �޴� */
circlesysop()
{
    FILE *fp1;
    char buf[80],buf2[80];
    int howmany;

    printf("\r\n ��ȣȸ ������ %s�Դϴ�.",cmenu->manager);
    /* ������ �ƴϰ� ��ڰ� �ƴϸ� ������� ���Ѵ� */
    if ( strcmp(cmenu->manager,pf.id) && strcmp(pf.id,"sysop") ){
	printf("\r\n ����� ���� �� �����. "); pressreturn();
	return;
    }

    while(1) {
    	printf("\r\n (%s)���Ƹ���ڸ޴��Դϴ�.",nmenu->ccode);
    	printf("\r\n ---------------------------");
    	printf("\r\n 1. ���Ƹ�����ó��");
    	printf("\r\n 2. Ż  ��  ó  ��");
    	printf("\r\n 3. ȸ  ��  ��  ȸ");
    	printf("\r\n 4. logo ȭ�� ����");
    	printf("\r\n 5. BBSCMENUSȭ�� ����� ");
    	printf("\r\n 6. ȸ���鿡�� ����������");
	printf("\r\n 7. ��������ó�� ");
	sprintf(buf,"circle/%s/CUSER.%s.tmp",nmenu->ccode,nmenu->ccode);
	fp1 = fopen(buf,"r");
	fseek(fp1,0,2); howmany = ftell(fp1) / sizeof(struct idst);
	if(fp1==NULL) howmany = 0;
	fclose(fp1);
	printf("\r\n 8. ���Խ�û�ѻ������(%d)",howmany);
	printf("\r\n ���>> ");
	keyinput(buf,10);
	if(buf[0]=='p' || buf[0]=='P') break;
	else if(buf[0]=='1') {
	    printf("\r\n ���Ƹ� ����ó�� �޴��Դϴ�.");
	    permitmember();
	}
	else if(buf[0] =='2') {
	    printf("\r\n Ż��ó���޴��Դϴ�.");
	    printf("\r\n Ż���ų���̵��>> ");
	    keyinput(buf,10);
	    sprintf(buf2,"circle/%s/CUSER.%s",nmenu->ccode,nmenu->ccode);
	    del_user(buf2,buf);
	}
	else if(buf[0]=='3') {
	    printf("\r\n ȸ���� ��ȸ�մϴ�. ");
	    sprintf(buf,"circle/%s/CUSER.%s",nmenu->ccode,nmenu->ccode);
	    show_user(buf);
	}
	else if(buf[0]=='4') {
	    printf("\r\n �ΰ�ȭ���� �����մϴ�.");
	    while(1) {
		printf("\r\n 0:���� 1: �ΰ�ȭ�� ���� 2: �ΰ�ȭ�Ͽø���");
		keyinput(buf,5);
		if(buf[0]=='1') {
		    sprintf(buf,"%s.log",nmenu->ccode);
		    cls();clogo(buf,1);
		}
	  	if(buf[0]=='2') {
		    printf("\r\n\r\n ������ ������ ���� �ʽ��ϴ�.");
		    printf("\r\n �ΰ�ȭ���� �üٿ��� �����ּ���.");
		    printf("\r\n");
		}
		if(buf[0]=='0') {
		    break;
		}
	    }
	}
	else if(buf[0]=='5') {
	    printf("\r\n CMENUSȭ���� ����ϴ�. ");
	    make_cmenu(0);
	}
	else if(buf[0]=='6') {
	    printf("\r\n ȸ���鿡�� ������ �����ϴ�.");
	    printf("\r\n ���� �������� �ʽ��ϴ�.");
	    printf("\r\n");
	}   
	else if(buf[0]=='7') {
	    printf("\r\n\r\n ���� ����ó���Դϴ�");
	    printf("\r\n ���Խ�ų ���̵�: ");keyinput(buf,10);
	    forcemember(buf);
	}
	else if(buf[0]=='8') {
	    sprintf(buf,"circle/%s/CUSER.%s.tmp",nmenu->ccode,nmenu->ccode);
	    show_user(buf);
	} 
    }
}

/* ���Խ�û ó�� */
permitmember()
{
    FILE *fp1,*fp2,*fp3;
    char buf[80];
    struct Memo memo;
    struct idst member;
    int i,howmany;
 
    sprintf(buf,"circle/%s/CUSER.%s.tmp",nmenu->ccode,nmenu->ccode);
    if((fp1=fopen(buf,"r+"))==NULL) {
	printf("\r\n ���Խ�û�� ����� �����ϴ�.");
	return;
    }
    fseek(fp1,0,2); howmany = ftell(fp1)/sizeof(struct idst);
    printf("\r\n �� %d ���� ���Խ�û�� �߽��ϴ�.",howmany);
    sprintf(buf,"circle/%s/APPLYMEMO.%s",nmenu->ccode,nmenu->ccode);
    fp2=fopen(buf,"r+");
    fseek(fp1,0,0);    
    for(i=0;i<howmany;i++) {
	fseek(fp1,i*sizeof(struct idst),0);
	fread((char*)&member,sizeof(struct idst),1,fp1);
	printf("\r\n ���Խ�û��");
	printf("\r\n ���̵�: %s",member.id);
	printf("\r\n �̸�: %s",member.name);
	printf("\r\n �������: %s",member.birthday);
        fread((char*)&memo,sizeof(struct Memo),1,fp2);
	printf("\r\n���Խ�û�޸�:%s",memo.content);
  	printf("\r\n ������ ����Ͻðڽ��ϱ�?(y/N)>>");keyinput(buf,2);
	if(buf[0]=='y'|| buf[0]=='Y') {
	    sprintf(buf,"circle/%s/CUSER.%s",nmenu->ccode,nmenu->ccode);
	    if((fp3=fopen(buf,"r+"))==NULL) {
		fp3=fopen(buf,"a+");
	    }
	    fseek(fp3,0,2);
	    fwrite((char*)&member,sizeof(struct idst),1,fp3);
	    fclose(fp3);
	    printf("\r\n����ó���� �Ͽ����ϴ�.");
	}
    }
    fclose(fp2);
    fclose(fp1);
    printf("\r\n ���Խ�û�� �������ϴ�.");pressreturn();
    sprintf(buf,"circle/%s/CUSER.%s.tmp",nmenu->ccode,nmenu->ccode);
    unlink(buf);
    sprintf(buf,"circle/%s/APPLYMEMO.%s",nmenu->ccode,nmenu->ccode);
    unlink(buf);

}
/* ���� ����ó�� */
forcemember(char*fmember)
{
    FILE *fp1;
    char buf[80];
    struct idst member;
    int howmany,i,check=0;
    
    sprintf(buf,"circle/%s/CUSER.%s.tmp",nmenu->ccode,nmenu->ccode);
    if((fp1=fopen(buf,"r"))!=NULL) {
	printf("\r\n ���Խ�û���� Ȯ���غ��ϴ�.");
	fseek(fp1,0,2); howmany = ftell(fp1) / sizeof(struct idst);
	for (i=0;i<howmany ; i++) {
	    fseek(fp1,i*sizeof(struct idst),0);
	    fread((char*)&member,sizeof(struct idst),1,fp1);
	    if(!strcmp(fmember,member.id)) {
		printf("\r\n ���Խ�û�� �߽��ϴ�.����ó���޴����� ó���ϼ���.");
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
	printf("\r\n ��ȣȸȸ����ܿ��� ���캾�ϴ�.");
	fseek(fp1,0,2); howmany=ftell(fp1)/sizeof(struct idst);
	fseek(fp1,0,0);
	for(i=0;i<howmany;i++) {
	    fseek(fp1,i*sizeof(struct idst),0);
	    fread((char*)&member,sizeof(struct idst),1,fp1);
	    if(!strcmp(fmember,member.id)) {
		printf("\r\n �̹� ȸ���Դϴ�. ����ó���� �ϸ� �ȵ˴ϴ�.");
	   	check =1 ;
		break;
	    }
	}
    }
    fclose(fp1);
    if(check) {
	return;
    }
/* ������� ���� ���Խ�û��, ȸ���� �ƴ� ���̴� */
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
	    printf("\r\n %sȸ���� ������ ã�ҽ��ϴ�.",user.id);
	    check = 1;break;
	}
     }
     fclose(fp1);
     if(check) {
	printf("\r\n ȸ���� ���Խ�ŵ�ϴ�.");
 	sprintf(buf,"circle/%s/CUSER.%s",nmenu->ccode,nmenu->ccode);
	if((fp1=fopen(buf,"r+"))==NULL) {
	    fp1=fopen(buf,"a+");
	}
	fseek(fp1,0,2);
	fwrite((char*)&user,sizeof(struct idst),1,fp1);
	fclose(fp1);
	printf("\r\n ����ó���� ���ƽ��ϴ�.");
    }
    else {
	printf("\r\n �׷� ȸ���� �����ϴ�.");
    }
}
         
showmember()
{
    FILE *fp1;
    char buf[80],whom[10];
    int i,howmany;
    struct idst member;
    while(1) {
	printf("\r\n 0.���� 1.���Լ����� 2.���̵�� 3.�̸����� >>");
	keyinput(buf,10);
	if(buf[0]=='1') {
            sprintf(buf,"circle/%s/CUSER.%s",nmenu->ccode,nmenu->ccode);
            fp1=fopen(buf,"r");
            fseek(fp1,0,2); howmany=ftell(fp1)/sizeof(struct idst);
            fseek(fp1,0,0);
            for(i=0;i<howmany;i++){
	        fread((char*)&member,sizeof(struct idst),1,fp1);
	        printf("\r\n %s�� �����Դϴ�.",member.id);
	        printf("\r\n ���̵�:%s, �̸�:%s, ����:%s",\
		  member.id,member.name,member.birthday);
		printf("\r\n �ߴ��Ͻ÷��� q�� ��������.");
 	        keyinput(buf,1);if(buf[0]=='q') break;
	    }
	    fclose(fp1);
        }
	else if(buf[0]=='2') {
	    printf("\r\nã���÷��¾��̵�: ");keyinput(whom,10);
	    sprintf(buf,"circle/%s/CUSER.%s",nmenu->ccode,nmenu->ccode);
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
            sprintf(buf,"circle/%s/CUSER.%s",nmenu->ccode,nmenu->ccode);
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
	
/* ȸ�� ���Խ�û�޴�*/

circleapply()
{
    char buf[80],tmp[80];
    cls();
    esc_sub=0;
    while(1) {
    	printf("\r\n ���Ƹ����Խ�û���Դϴ�.");
    	printf("\r\n -----------------------");
    	printf("\r\n 1. ������ ��û�մϴ�. ");
    	printf("\r\n 2. Ż���ϰڽ��ϴ�. ");
    	printf("\r\n 3. ȸ���� �����غ��ϴ�.");
	printf("\r\n ���>> ");
	keyinput(buf,10);
	if (buf[0]=='1') {
	    printf("\r\n ������ �ϽǷ��� �Ͻʴϴ�.");
	    printf("\r\n ������ �Ͻðڽ��ϱ�?");
	    keyinput(buf,1,YEA);
	    if(buf[0]=='y' || buf[0]=='Y') {
		gaip(nmenu->ccode); /* nmenu->r�� ��ȣȸ ��ȣ�̴�.*/
	    }
	    else {
		printf("\r\n ������ ���ϼ̽��ϴ�.");
	    }
	}
	else if(buf[0]=='2') {
	    printf("\r\n Ż�� �ϽǷ��� �Ͻʴϴ�.");
	    printf("\r\n Ż�� �Ͻðڽ��ϱ�?(y/N)>>");
	    printf("\r\n ���� �������� �ʽ��ϴ�.");
	    keyinput(buf,1,YEA);
	    if(buf[0]=='y' || buf[0] =='Y') {
	 	taltaoe(nmenu->r); /* nmenu->r�� ��ȣȸ ��ȣ�̴� */
	    }
	    else {
		printf("\r\n Ż�� ����Դϴ�.");
	    }
	}
	else if(buf[0]=='3') {
	    printf("\r\n ��ȣȸ ������� �����ݴϴ�.");
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
	printf("\r\n ����� �̹� ȸ���Դϴ�.");
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
	printf("\r\n ����� �̹� ���Խ�û�� �Ͽ����ϴ�.");
	return;
    }
    sprintf(buf,"circle/%s/APPLYMEMO.%s",nmenu->ccode,circlename);
    if((fp1=fopen(buf,"r+"))==NULL) {
	fp1= fopen(buf,"a+");
    }
    fseek(fp1,0,2);
    printf("\r\n ������������� �����ּ���.(70�ڳ��ܷ�)\r\n");
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
    printf("\r\n ���Խ�û�� �������ϴ�.���Կ��δ� ������ �����մϴ�.");
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
	printf("\r\n �޴�ȭ���� ������ �ʽ��ϴ�.");
	sprintf(buf,"circle/CMENUS.%s",nmenu->ccode);
	if((fp2=fopen(buf,"r"))!=NULL) {
	    printf("\r\n ������ �޴�ȭ���� �ֽ��ϴ�. �������Ͻðڽ��ϱ�?(y/N) ");
	    keyinput(buf,1);
	    if(buf[0]=='y'||buf[0]=='Y') {
	 	convertoldcmenu();
	    }
	}
	else {
            printf("\r\n�޴�ȭ���� ����ðڽ��ϱ�?(y/N)>> ");
            lineinput(buf,1,YEA);
            if(buf[0]=='y' || buf[0]=='Y') make_cmenu(1);
	}
        host_end();
    }
    else {
        if((cmtmp = (struct cmenustruct *)malloc(sizeof(struct cmenustruct))) == NULL) {
            printf("\r\n�޸� ����");
            exit(1);
        }
        cmenu = cmtop = cnmenu = cmtmp;
        fread((char*)cmtmp,sizeof(struct cmenustruct),1,fp1);
        if((cmtmp = (struct cmenustruct *)malloc(sizeof(struct cmenustruct))) == NULL) {
            printf("\r\n�޸𸮰� ����");
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
                printf("\r\n�޸� ����");
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
            printf("\r\n�޸𸮰� �����ϳ׿�.");
            exit(1);
        }
        cmenu=cmtop=cnmenu=cmtmp;
        if((cmtmp=(struct cmenustruct *)malloc(sizeof(struct cmenustruct)))\
         ==NULL) {
            printf("\r\n�޸𸮰� �����ϳ׿�.");
            exit(1);
        }
        cnmenu->neqp=cnmenu->aeqp=NULL;
        cnmenu->upp=cnmenu;
        cnmenu->downp=cmtmp;
        cnmenu->form='M'; cnmenu->level=cnmenu->del=cnmenu->w= cnmenu->r =\
                cnmenu->deep=0;
        strcpy(cnmenu->ccode,"dongari");
        strcpy(cnmenu->list,"���Ƹ�");
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
            printf("\r\nnum ���� F Ÿ��Ʋ      �����ڵ�   �б�  ����    ����    ����");
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
"\r\n\n\r��ȣ ����[P] ����[E] �߰�[A] ����[I] ����[D] ����[S] ����[X] >> ");
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
                    printf("\r\n�޸𸮰� ���ڶ��ϴ�.");
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
                    printf("\r\n�޸𸮰� ���ڶ��ϴ�.");
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
        else if(buf[0] == 'd' || buf[0] == 'D') {  /* �޴� ���� */
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
    printf("\r\n ���ù�ȣ: ");keyinput(buf,5);
    if(buf[0]!='\0') sprintf(cnmenu->choice,buf);
    printf("\r\n\r ���� : ");
    keyinput(buf,1);
    if(buf[0]!='\0') cnmenu->form = buf[0];
    printf("\r\n\r �����ڵ� : ");
    keyinput(buf,9);
    if(buf[0]!='\0') sprintf(cnmenu->ccode,"%s",buf);
    printf("\r\n\r �޴��� : ");
    keyinput(buf,39);
    if(buf[0]!='\0') sprintf(cnmenu->list,"%s",buf);
    if((cnmenu->form == 'M') && (cnmenu->downp == NULL)) {
        sub_ccreat(); /* M�޴� ���� ���� �� */
    }
    else {
        printf("\r\n ���ⷹ�� : ");
        keyinput(buf,8);
        cnmenu->w = atoi(buf);
        printf("\r\n �бⷹ�� : ");
        keyinput(buf,8);
        cnmenu->r = atoi(buf);
        if(cnmenu->form == 'L' || cnmenu->form == 'l' ||
          cnmenu->form == 'V' || cnmenu->form == 'v') {
            printf("\r\n ����/�Խù� : ");
            keyinput(buf,2);
            cnmenu->level = atoi(buf);
        
        }
        else if(nmenu->form == 'F' || nmenu->form == 'f') {
            printf("\r\n����/ȭ�� : ");
            keyinput(buf,8);
            cnmenu->level = atoi(buf);
            printf("\r\n�������� �÷��� : ");
            keyinput(buf,3);
            cnmenu->del = atoi(buf);
            printf("\r\n ȭ����ġ���丮: ");
            lineinput(buf,29,YEA);
            strcpy(cnmenu->file,buf);
        }
        printf("\r\n �Խ��Ǵ����(������Enter): ");
        keyinput(buf,10);
	sprintf(cnmenu->manager,"%s",buf);
    }
}

append_cline()
{
    if((cmtmp = (struct cmenustruct *)\
         malloc(sizeof(struct cmenustruct))) == NULL) {
        printf("\r\n�޸𸮰� ���ڶ��ϴ�.");
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
        printf("\r\n�޸𸮰� ���ڶ��ϴ�.");
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
    else printf("\r\nȭ���� ������ �ʽ��ϴ�.");
}



int chose_cmenu()  /* �޴� ���� �˻� */
{
    int key = 1;
    char buf[80], c, tmp[80], crt[3],inc[10];
    if(cnmenu->form == 'R') {  /* �Խù��̽���ȭ��.�̰��� �⺻������� */
        if(cnmenu->w <= pf.level) {
	    sprintf(buf,"%s.log",cnmenu->ccode);
            clogo(buf,1);
            if(!strcmp(cnmenu->ccode,"chat")) { /* ��ȭ�� */
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
		printf("\r\n ���� ���� ���̵� ��������.");
		printf("\r\n ���̵�(ex. hommage sysop) >> ");
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
        /* �Խù� */
	sprintf(buf,"%s.log",cnmenu->ccode);
        clogo(buf,1);
	sprintf(buf,"circle/%s/boards",nmenu->ccode);
        show_board(buf,cnmenu->ccode,cboardcmd);
	if(upermenu==1) {cnmenu=cnmenu->upp;upermenu=0;}
    }
    else if(cnmenu->form == 'F' || (cnmenu->form == 'f' && circlecheck) ) {  
        /* ȭ�� �Խù� */
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

