#include "bbs.h"
/* direc�� ȭ���� ����� ���丮 
 bdirec�� �Խù����丮 fhead�� �Խù�ȭ����� */
uploadfile(char *direc,char *bdirec,char*fhead)
{
    FILE *fp1,*fp2;
    int i,j;
    char buf[80],buf2[80],tmp[80],ch;
    struct dis_list up_title;
    struct dis_list uf_title;
    int num;
    
    logo("upload.log",0); /* ������ �ʰ� ��� */
    printf("\r\n ȭ���� ���ε��Ϸ��� ��ʴϴ�.");
    printf("\r\n ���ε��� ȭ�ϸ��� ��������(Ʋ���� ����� �ȵ˴ϴ�)");
    printf("\r\n (�ҹ��ڷ� ���ּ���.)");
    printf("\r\n ȭ�ϸ�: ");
    lineinput(up_title.filename,29);
    /* check the file is */
    if(check_file(direc,up_title.filename)) {
	printf("\r\n ȭ���� �����մϴ�.");
	pressreturn();reprint=YEA;
	return;
    } 
    ch = '*';
    if(work.chose==' ') {
	ch = ' ';
        change_in();
    }
    while(1) {
	printf("\r\n\r\n ������ ���������� �����ϼ���.");
	printf("\r\n 0:��� 1:Z��(1) 2:Z��(2) 3:KERMIT U:��ھ��ε�");
	printf("\r\n >> "); lineinput(buf,3,YEA);

	if (buf[0] =='1') {
	    printf("\r\n rz �� ȭ���� �ްڽ��ϴ�.");
	    chdir("ftp");
	    ioctl(0,TCSANOW,&systerm);
	    system("rz -v");
            sprintf(buf,"chmod a+rw *");
            system(buf);
	    ioctl(0,TCSANOW,&mbuf);
	    chdir(getenv("HOME"));
	    break;
	}
	else if (buf[0]=='2') {
	    printf("\r\n rz -e�� ȭ���� �ްڽ��ϴ�.");
	    chdir("ftp");
	    ioctl(0,TCSANOW,&systerm);
	    system("rz -ev");
	    sprintf(buf,"chmod a+rw *");
	    system(buf);
	    ioctl(0,TCSANOW,&mbuf);
	    chdir(getenv("HOME"));
	    
	    break;
	}
	else if (buf[0] =='3') {
	    printf("\r\n kermit���� �ްڽ��ϴ�.");
	    chdir("ftp");
	    ioctl(0,TCSANOW,&systerm);
	    system("kermit -r");
	    sprintf(buf,"chmod a+rw *");
	    system(buf);
	    ioctl(0,TCSANOW,&mbuf);
	    chdir(getenv("HOME"));
	    break;	
	}
	else if (buf[0] =='U') {
	    printf("\r\n ��� ���ε��Դϴ�. ");
	   /* �ϴ� ftp�� �ű��. */
	    printf("\r\n �Ϲ����� ����� �� �����.");
	    ioctl(0,TCSANOW,&systerm);
            sprintf(buf,"chmod a+rw *");
            system(buf);
	    sprintf(buf,"mv sysopfile/%s ftp",up_title.filename); 
	    system(buf);
	    ioctl(0,TCSANOW,&mbuf);
	    chdir(getenv("HOME"));
	    break;
	}
	else if (buf[0]=='0') {
	    printf("\r\n ���߿� ���ε��ϼ���. �ٻڽø�");
	    printf("\r\n �׷� ��ſ� ��ŵǼ���.");
	    pressreturn();
	    reprint=YEA; return;
	}
    } 	    
    if(ch==' ' ) {
	change_in();
    }

    j = strlen(up_title.filename);
    for (i=0;i<j;i++) {
	buf[i] = tolower(up_title.filename[i]);
    }
    buf[i] = '\0';
    sprintf(up_title.filename,"%s",buf);
    sprintf(buf,"ftp/%s",up_title.filename);
    
    if( (fp1=fopen(buf,"r")) == NULL ) {
        j = strlen(up_title.filename);
	for (i = 0;i<j;i++) {
	    buf2[i] = toupper(up_title.filename[i] );
	}
	buf2[i] = '\0';
	sprintf(tmp,"ftp/%s",buf2); /* �빮�ڷ� �� ȭ�� */
	sprintf(buf,"ftp/%s",up_title.filename); /* �ҹ��ڷ� �� ȭ�� */
	printf("\r\n %sȭ���� %sȭ�Ϸ� ��ȯ�մϴ�.",tmp,buf);
	rename(tmp,buf);
	if((fp2 = fopen(buf,"r"))==NULL) { /* �ҹ��ڷ� �ٽ� üũ */
	    printf("\r\n ȭ�������� ���� �ʾҾ��.");
	    printf("\r\n ����� �� �� �����ϴ�.");
	    printf("\r\n ȭ���̸��̳� ������ �߸� �Ǿ����.");
	    printf("\r\n ȭ���̸��� ���ε� ��Ȳ�� �����Ͻø�");
	    printf("\r\n �ִ�� �����Ͽ� ����Ͽ��帮�ڽ��ϴ�.");
            pressreturn(); reprint=YEA; return;
	}
	fclose(fp2);

    }
    fclose(fp1);
    sprintf(buf,"ftp/%s",up_title.filename);
    fp1=fopen(buf,"r");
    fseek(fp1,0,2); up_title.file_size = ftell(fp1);
    pressreturn();printf("\r\n ȭ���� ������ �Ǿ����ϴ�.");
    fclose(fp1);
    printf("\r\n ũ�� %d�̰� ȭ�ϸ� %s�� �ڷ� �½��ϱ�(Y/n)",up_title.file_size,\
	up_title.filename);
    oneinput(buf);
    if(buf[0] == 'n') {
	printf("\r\n �ڷ� ������ �� �� �Ǿ����ϴ�.");
/*	report("uploadfail",up_title); */
	pressreturn(); reprint=YEA; return;
    }
    pressreturn(); printf("\r\n ȭ�ϼ����� ���� �����Դϴ�."); pressreturn();
    if((num=lineeditor(bdirec,fhead))==-1) {
	printf("\r\n ����ϼ̽��ϴ�.");
	return;
    }
    printf("\r\n ȭ���� ����մϴ�(�����丮��)");
    sprintf(buf,"cp ftp/%s %s/%s",up_title.filename,direc,up_title.filename);
    ioctl(0,TCSANOW,&systerm);
    system(buf);
    ioctl(0,TCSANOW,&mbuf);
    sprintf(buf,"%s/%s",direc,up_title.filename);
    printf("\r\n %s�� �ڷḦ �ű�ϴ�.",buf);
    if ( (fp1=fopen(buf,"r")) == NULL ) {
	printf("\r\n ȭ�ϵ���� �� �Ǿ����ϴ�. ***�ɰ�***");
	printf("\r\n ��ڿ��� �˷��ּ���.    ***�ɰ�***");
	ioctl(0,TCSANOW,&systerm); 
        sprintf(buf,"cp ftp/%s ftp/fail/%s",up_title.filename,up_title.filename);
	system(buf);
	sprintf(buf,"rm ftp/%s",up_title.filename);
	ioctl(0,TCSANOW,&mbuf);
	report("registererror",up_title);
	pressreturn(); reprint=YEA; return;
    } 
    fclose(fp1);
    printf("\r\n ȭ���� %s���丮�� ����߽��ϴ�.",direc);
    ioctl(0,TCSANOW,&systerm); 
    sprintf(buf,"rm ftp/%s",up_title.filename);system(buf);
    ioctl(0,TCSANOW,&mbuf);
    sprintf(buf,"%s/%s",bdirec,fhead);
    if((fp1=fopen(buf,"r+")) == NULL ) {
	printf("\r\n ȭ�ϰԽù������ �� �����ϴ�(����ߴ�)");
	report("registererror2",up_title);
	pressreturn(); return;
    }
    fseek(fp1,(num-1)*sizeof(struct dis_list),0);
    fread((char*)&uf_title,sizeof(struct dis_list),1,fp1);
    uf_title.file_size = up_title.file_size;
    strcpy(uf_title.filename,up_title.filename);
    uf_title.down = 0;
    uf_title.look = 0;
    fseek(fp1,(num-1)*sizeof(struct dis_list),0);
    fwrite((char*)&uf_title,sizeof(struct dis_list),1,fp1);
    fclose(fp1);
    report("up",uf_title);
    printf("\r\n ���ε尡 ������ ���������ϴ�.");
    logo("uploadend.log",1);
    return;
}
        	
    	   
downloadfile(char*cmd,char*direc,char*fhead,char*filedir)
{
    FILE *fp1;
    char buf[400];
    int i,j,dnum[10],cnum=0;
    char dfname[10][30],alldfname[300];
    char ch,*tmp;
    struct dis_list down_title;
    sprintf(buf,"\0");sprintf(alldfname,"\0");
    cnum=parser(cmd);
    if(cnum>10) { cnum=10;}
    else if(cnum<=0) { cnum=0;}
    for(i=1;i<cnum;i++) {
	dnum[i]=atoi(argument[i]);
    }
    if (cnum) {
	logo("download.log",0);
	printf("\r\n");
 	for(i=1;i<cnum;i++) {	
	    printf(" %d �� ",dnum[i]);
	}
	printf("\r\n ȭ���� �ٿ�����÷ƴϱ�?");	
	sprintf(buf,"%s/%s",direc,fhead);
	if((fp1 = fopen(buf,"r"))==NULL) {
	    printf("\r\n ȭ������� ���� �� �����ϴ�.");
	    printf("\r\n �������"); pressreturn(); return;
	}
	for(i=1;i<cnum;i++) {
	    fseek(fp1,(dnum[i]-1)*sizeof(struct dis_list),0);
	    fread((char*)&down_title,sizeof(struct dis_list),1,fp1);
	    sprintf(dfname[i],"%s",down_title.filename);
	}
	fclose(fp1);
	for(i=1;i<cnum;i++) {
	    sprintf(buf,"%s/",filedir);
	    strcat(alldfname,buf); 
	    strcat(alldfname,dfname[i]);
	    strcat(alldfname," ");
	    sprintf(buf,"%s/%s",filedir,dfname[i]);
	    fp1 = fopen(buf,"r");
	    if(fp1==NULL) {
	        printf("\r\n %s�ڷᰡ ���ų�",dfname[i]);
		printf("\r\n �ڷ���� ������Դϴ�.");
	        return;
	    }
	}
	while(1) {
            printf("\r\n\r\n ������ ���������� �����ϼ���.");
            printf("\r\n 0:��� 1:Z��(1) 3:KERMIT ");
            printf("\r\n >> "); lineinput(buf,3,YEA);
	    if(buf[0] =='1') {
		printf("\r\n sz ���� �����ϴ�. ");
		printf("\r\n �������������� �����Ű����.");	
		ch = '*';
		if(work.chose==' ') {
		    ch = ' ';
		    change_in();
		}
		sprintf(buf,"sz %s",alldfname);
		printf("\r\n %s",buf);
		ioctl(0,TCSANOW,&systerm);
		system(buf);
		ioctl(0,TCSANOW,&mbuf);
		chdir(getenv("HOME"));
		if(ch==' ') {
		    change_in();
		}
		printf("\r\n\r\n ������ �Ϸ�Ǿ����ϴ�.");
		logo("downloadend.log",1);
/*		report("down",down_title); */
		break;
	    }
	    if (buf[0] == '3') {
		printf("\r\n kermit -s�� �����ϴ�.");
		printf("\r\n kermit�� �����Ű����");
		ch='*';
		if(work.chose==' ') {
		    ch=' ';
		    change_in();
		}
		sprintf(buf,"kermit -s %s",alldfname);
		ioctl(0,TCSANOW,&systerm);
		system(buf);
		ioctl(0,TCSANOW,&mbuf);
		chdir(getenv("HOME"));
		if(ch==' ') {
		    change_in();
		}
		printf("\r\n\r\n ������ �Ϸ�Ǿ����ϴ�.");
		logo("downloadend.log",1);
/*		sprintf(buf,"fboards/%s",nmenu->ccode);
		fp1=fopen(buf,"r+");
		fseek(fp1,(dnum[i]-1)*sizeof(struct dis_list),0);
		fread((char*)&down_title,sizeof(struct dis_list),1,fp1);
		down_title.down++;
		fseek(fp1,(dnum[i]-1)*sizeof(struct dis_list),0);
		fwrite((char*)&down_title,sizeof(struct dis_list),1,fp1);
		fclose(fp1); */
		break;
	    }
	    else if(buf[0] =='0') {
		printf("\r\n �׷���. �ٿ�޾� ���ؿ�. ����մϴ�.");
		pressreturn(); break;
	    }
	} 
    }

}
move_file(char*from,struct dis_list boards,char*direc, char *ccode,char*filefrom,char*filedir)
{
    FILE *fp1;
    char tmp[80],buf[80],bfilename[80];
    time_t t;
    struct tm *tp,*localtime();

    if(1) {
        sprintf(buf,"%s/%s",direc,ccode);
        if( (fp1=fopen(buf,"r"))==NULL) {
            printf("\r\n �׷� �����ڵ�� �����ϴ�.");
            return 0;
        }
        fclose(fp1);
        if( (fp1=fopen(buf,"r+"))!=NULL) {
            fseek(fp1,0,2); boards.num=ftell(fp1)/sizeof(struct dis_list)+1;
            time(&t);boards.date=t;sprintf(bfilename,boards.bfilename);
            sprintf(boards.bfilename,"%s%d.txt",ccode,t);
            sprintf(buf,"cp %s/%s %s/%s",from,bfilename,\
              direc,boards.bfilename);
            system(buf);
	    sprintf(buf,"cp %s/%s %s/%s",filefrom,boards.filename,\
	      filedir,boards.filename);
            fseek(fp1,0,2);
            fwrite((char*)&boards,sizeof(struct dis_list),1,fp1);
            fclose(fp1);
        } 
        else return 0;
    }
    return 1; 
}

