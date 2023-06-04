#include "bbs.h"
/* direc는 화일이 저장될 디렉토리 
 bdirec는 게시물디렉토리 fhead는 게시물화일헤더 */
uploadfile(char *direc,char *bdirec,char*fhead)
{
    FILE *fp1,*fp2;
    int i,j;
    char buf[80],buf2[80],tmp[80],ch;
    struct dis_list up_title;
    struct dis_list uf_title;
    int num;
    
    logo("upload.log",0); /* 멈추지 않고 출력 */
    printf("\r\n 화일을 업로드하려고 계십니다.");
    printf("\r\n 업로드할 화일명을 넣으세요(틀리면 등록이 안됩니다)");
    printf("\r\n (소문자로 써주세요.)");
    printf("\r\n 화일명: ");
    lineinput(up_title.filename,29);
    /* check the file is */
    if(check_file(direc,up_title.filename)) {
	printf("\r\n 화일이 존재합니다.");
	pressreturn();reprint=YEA;
	return;
    } 
    ch = '*';
    if(work.chose==' ') {
	ch = ' ';
        change_in();
    }
    while(1) {
	printf("\r\n\r\n 전송할 프로토콜을 선택하세요.");
	printf("\r\n 0:취소 1:Z모뎀(1) 2:Z모뎀(2) 3:KERMIT U:운영자업로드");
	printf("\r\n >> "); lineinput(buf,3,YEA);

	if (buf[0] =='1') {
	    printf("\r\n rz 로 화일을 받겠습니다.");
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
	    printf("\r\n rz -e로 화일을 받겠습니다.");
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
	    printf("\r\n kermit으로 받겠습니다.");
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
	    printf("\r\n 운영자 업로드입니다. ");
	   /* 일단 ftp로 옮긴다. */
	    printf("\r\n 일반인은 사용할 수 없어요.");
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
	    printf("\r\n 나중에 업로드하세요. 바쁘시면");
	    printf("\r\n 그럼 즐거운 통신되세요.");
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
	sprintf(tmp,"ftp/%s",buf2); /* 대문자로 된 화일 */
	sprintf(buf,"ftp/%s",up_title.filename); /* 소문자로 된 화일 */
	printf("\r\n %s화일을 %s화일로 변환합니다.",tmp,buf);
	rename(tmp,buf);
	if((fp2 = fopen(buf,"r"))==NULL) { /* 소문자로 다시 체크 */
	    printf("\r\n 화일전송이 되지 않았어요.");
	    printf("\r\n 등록을 할 수 없습니다.");
	    printf("\r\n 화일이름이나 전송이 잘못 되었어요.");
	    printf("\r\n 화일이름과 업로드 상황을 설명하시면");
	    printf("\r\n 최대로 복구하여 등록하여드리겠습니다.");
            pressreturn(); reprint=YEA; return;
	}
	fclose(fp2);

    }
    fclose(fp1);
    sprintf(buf,"ftp/%s",up_title.filename);
    fp1=fopen(buf,"r");
    fseek(fp1,0,2); up_title.file_size = ftell(fp1);
    pressreturn();printf("\r\n 화일이 전송이 되었습니다.");
    fclose(fp1);
    printf("\r\n 크기 %d이고 화일명 %s인 자료 맞습니까(Y/n)",up_title.file_size,\
	up_title.filename);
    oneinput(buf);
    if(buf[0] == 'n') {
	printf("\r\n 자료 전송이 잘 못 되었습니다.");
/*	report("uploadfail",up_title); */
	pressreturn(); reprint=YEA; return;
    }
    pressreturn(); printf("\r\n 화일설명을 쓰실 차례입니다."); pressreturn();
    if((num=lineeditor(bdirec,fhead))==-1) {
	printf("\r\n 취소하셨습니다.");
	return;
    }
    printf("\r\n 화일을 등록합니다(본디렉토리로)");
    sprintf(buf,"cp ftp/%s %s/%s",up_title.filename,direc,up_title.filename);
    ioctl(0,TCSANOW,&systerm);
    system(buf);
    ioctl(0,TCSANOW,&mbuf);
    sprintf(buf,"%s/%s",direc,up_title.filename);
    printf("\r\n %s로 자료를 옮깁니다.",buf);
    if ( (fp1=fopen(buf,"r")) == NULL ) {
	printf("\r\n 화일등록이 안 되었습니다. ***심각***");
	printf("\r\n 운영자에게 알려주세요.    ***심각***");
	ioctl(0,TCSANOW,&systerm); 
        sprintf(buf,"cp ftp/%s ftp/fail/%s",up_title.filename,up_title.filename);
	system(buf);
	sprintf(buf,"rm ftp/%s",up_title.filename);
	ioctl(0,TCSANOW,&mbuf);
	report("registererror",up_title);
	pressreturn(); reprint=YEA; return;
    } 
    fclose(fp1);
    printf("\r\n 화일을 %s디렉토리로 등록했습니다.",direc);
    ioctl(0,TCSANOW,&systerm); 
    sprintf(buf,"rm ftp/%s",up_title.filename);system(buf);
    ioctl(0,TCSANOW,&mbuf);
    sprintf(buf,"%s/%s",bdirec,fhead);
    if((fp1=fopen(buf,"r+")) == NULL ) {
	printf("\r\n 화일게시물헤더가 안 열립니다(등록중단)");
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
    printf("\r\n 업로드가 무사히 마쳐졌습니다.");
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
	    printf(" %d 번 ",dnum[i]);
	}
	printf("\r\n 화일을 다운받으시렵니까?");	
	sprintf(buf,"%s/%s",direc,fhead);
	if((fp1 = fopen(buf,"r"))==NULL) {
	    printf("\r\n 화일헤더를 읽을 수 없습니다.");
	    printf("\r\n 전송취소"); pressreturn(); return;
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
	        printf("\r\n %s자료가 없거나",dfname[i]);
		printf("\r\n 자료실이 폐쇄중입니다.");
	        return;
	    }
	}
	while(1) {
            printf("\r\n\r\n 전송할 프로토콜을 선택하세요.");
            printf("\r\n 0:취소 1:Z모뎀(1) 3:KERMIT ");
            printf("\r\n >> "); lineinput(buf,3,YEA);
	    if(buf[0] =='1') {
		printf("\r\n sz 으로 보냅니다. ");
		printf("\r\n 전송프로토콜을 실행시키세요.");	
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
		printf("\r\n\r\n 전송이 완료되었습니다.");
		logo("downloadend.log",1);
/*		report("down",down_title); */
		break;
	    }
	    if (buf[0] == '3') {
		printf("\r\n kermit -s로 보냅니다.");
		printf("\r\n kermit를 실행시키세요");
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
		printf("\r\n\r\n 전송이 완료되었습니다.");
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
		printf("\r\n 그래요. 다운받아 뭐해요. 취소합니다.");
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
            printf("\r\n 그런 가기코드는 없습니다.");
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

