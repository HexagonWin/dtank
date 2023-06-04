
#include "bbs.h"

int esc_chat,esc_chatting; /* esc_chat은 대기실에서, esc_chatting는 
				채팅방에서 나가는 매개변수이다. */
#define SIZE 300
#define MODE 0666
/* 채팅 메인 프로그램 */
chat()
{
    FILE *fp1;
    char buf[80];
    struct chatin mychatid;
    struct roomname cell; 
    mycellnum = 0;     
    cell.howmany=0;
    cls();

    sprintf(time_chk,"tmp/STAT.%s",tty);
    tfp = fopen(time_chk,"w");
    fputc('4',tfp);  /* 채팅중이라는 것을 알림 */
    fclose(tfp);

    if(( fp1 = fopen("tmp/ROOMINFO","r+"))==NULL) {
	fp1 = fopen("tmp/ROOMINFO","a");
    }
    fread((char*)&cell,sizeof(struct roomname),1,fp1);
    cell.flag =1;
    cell.howmany++;
    cell.passwd[0] = '\0';
    strcpy(cell.name,"라운지");
    fseek(fp1,0,0); fwrite((char*)&cell,sizeof(struct roomname),1,fp1);
    fclose(fp1);
    if((fp1=fopen("tmp/ROOM.0","r+"))==NULL) {
	fp1 = fopen("tmp/ROOM.0","a");
    }
    fseek(fp1,0,2);
    sprintf(mychatid.id,"%s",pf.id);
    sprintf(mychatid.name,"%s",pf.name);
    sprintf(mychatid.tty,"%s",tty);
    fwrite((char*)&mychatid,sizeof(struct chatin),1,fp1);
    fclose(fp1);
    showcell(); /* 라운지를 보여준다. */
    esc_chat = 0;
    while(1) {
	printf("%s",CHATHELP);
	keyinput(buf,80,YEA);
	domenucommand(buf,chatloungecmd);
 	if (esc_chat) {
	    deluser(0);
	    break;
	}
    }
    sprintf(time_chk,"tmp/STAT.%s",tty);
    tfp = fopen(time_chk,"w");
    fputc('0',tfp);  /* 보통상태로 환원 */
    fclose(tfp);

    reprint=YEA;
    return;

}

/* 각방의 상태를 보여준다. */
showcell()
{
    FILE *fp1,*fp2;
    char buf[80];
    struct roomname cell;
    struct chatin chatpf;
    int i,culm,linenum,howmany,many,num;
    fp1 = fopen("tmp/ROOMINFO","r");
    fseek(fp1,0,2); many = ftell(fp1) / sizeof(struct roomname);
    fseek(fp1,0,0);
    printf("\r\n");for(i=0;i<80;i++) printf("-");
    for(num=0;num<many;num++) {
	sprintf(buf,"tmp/ROOM.%d",num);
	if((fp2 = fopen(buf,"r"))!=NULL) {
	    fseek(fp1,num*sizeof(struct roomname),0);
 	    fread((char*)&cell,sizeof(struct roomname),1,fp1);
	    if(1/*cell.howmany*/) {
		/* 인원수는 ROOM.?화일을 참조하는 것이 확실하다.*/
	        fseek(fp2,0,2); howmany = ftell(fp2) / sizeof(struct chatin);
	        fseek(fp2,0,0);
		if(num ==0 ) {
		    printf("\r\n 라운지(대기실) %d명",howmany);
		}
 	        else if(cell.passwd[0]=='\0') {
		    printf("\r\n %d번방(공개) [%s] 인원:%d",num,cell.name,cell.howmany);
	        }
	        else  {
		    printf("\r\n %d번방(비공개) [%s] 인원:%d",num,cell.name,cell.howmany);
	        }
	        for (i = 0; i<howmany; i++) {
		    fread((char*)&chatpf,sizeof(struct chatin),1,fp2);
		    printf("\r\n %s[%s]",chatpf.id,chatpf.name);
	        }
	    }
	    printf("\r\n");for(i=0;i<80;i++) printf("-");
	}
    }
    printf("\r\n");
    fclose(fp2);
    fclose(fp1);

}
	
/* 회원을 대화방에 추가한다. */
adduser(cellnum)
int cellnum;
{
    FILE *fp1;
    char buf[80];
    int howmany;
    struct roomname cell;
    struct chatin chatpf;
    sprintf(chatpf.id,"%s",pf.id);
    sprintf(chatpf.name,"%s",pf.name);
    sprintf(chatpf.tty,"%s",tty);
    sprintf(buf,"tmp/ROOM.%d",cellnum); /* 대화방에 사람을 집어넣고 */
    if((fp1 = fopen(buf,"r+")) ==NULL) {
	fp1 = fopen(buf,"a");
    }
    fseek(fp1,0,2);
    fwrite((char*)&chatpf,sizeof(struct chatin),1,fp1);
    fseek(fp1,0,2);
    howmany = ftell(fp1) /sizeof(struct chatin);
    fclose(fp1);
    if((fp1 = fopen("tmp/ROOMINFO","r+"))==NULL) {
        fp1 = fopen("tmp/ROOMINFO","a");
    }
    fseek(fp1,cellnum*sizeof(struct roomname),0);
    fread((char*)&cell,sizeof(struct roomname),1,fp1);
    cell.howmany = howmany; /* 방화일을 조사해 인원 수 재조정 */
    fseek(fp1,cellnum*sizeof(struct roomname),0);
    fwrite((char*)&cell,sizeof(struct roomname),1,fp1);
    fclose(fp1);
}

deluser(cellnum)
int cellnum;
{
    FILE *fp1;
    char buf[80];
    int i,j,howmany,num=0;
    struct chatin *chatid;
    struct roomname cell;
    sprintf(buf,"tmp/ROOM.%d",cellnum);
    if((fp1= fopen(buf,"r+")) ==NULL) {
	fp1 = fopen(buf,"a");
    }
    fseek(fp1,0,SEEK_END);
    howmany = ftell(fp1) / sizeof(struct chatin) ;
    if((chatid= (struct chatin*)calloc(howmany,sizeof(struct chatin))) ==\
NULL) {
        printf("\r\n 메모리 부족 "); pressreturn();
        exit(1);
    }
    fseek(fp1,0,0);
    num = 0;
    for(i = 0 ; i<howmany; i++) {
        fread((char*)&chatid[i],sizeof(struct chatin),1,fp1);
        if(strcmp(chatid[i].tty,tty)) {
	    printf("\r\n %s[%s]님이 대화방에 남습니다.",chatid[i].name,chatid[i].tty);
	    num++;
        }
    }
    fclose(fp1);
    sprintf(buf,"tmp/ROOM.%d",cellnum);
    unlink(buf);
    if((fp1 = fopen("tmp/ROOMINFO","r+")) ==NULL) {
	fp1 = fopen("tmp/ROOMINFO","a");
    }
    fseek(fp1,cellnum*sizeof(struct roomname),SEEK_SET);
    fread((char*)&cell,sizeof(struct roomname),1,fp1);
    cell.howmany = num;   /* 방의 사람수를 줄인다. */
    fseek(fp1,cellnum*sizeof(struct roomname),SEEK_SET);
    fwrite((char*)&cell,sizeof(struct roomname),1,fp1);
    fclose(fp1);
    if(cell.howmany) {
        sprintf(buf,"tmp/ROOM.%d",cellnum);
        if((fp1 = fopen(buf,"r+"))==NULL) {
            fp1= fopen(buf,"a");
        }
        for (j=0; j<howmany;j++) {
            if(strcmp(chatid[j].tty,tty))
		fwrite((char*)&chatid[j],sizeof(struct chatin),1,fp1);
        }
        fclose(fp1);
    }
    free(chatid);
    return;
}

chatting_in(shmid,cellnum)
int shmid;
int cellnum;
{
    int i;
    chatcheck = 1; mycellnum = cellnum;/*전역변수 */
    printf("\r\n %d 번 방에 들어오셨습니다.",cellnum);
    printf("\r\n");for(i=0;i<80;i++) printf("-");
    deluser(0); /*  대기실에 있는 자신을 지운다. */
    adduser(cellnum); /* 채팅방에 자신을 쓴다. */
    chatstart(shmid,cellnum); /* 채팅을 시작한다. */
    deluser(cellnum); /* 채팅방에서 자신을 지운다. */
    adduser(0); /* 대기실에 자신을 다시 써넣는다. */
    chatcheck = 0; mycellnum = 0;     
    return;
}

chatstart(shmid,cellnum)
int shmid;
int cellnum;
{
    FILE *fp1;
    int i,j,fount;
    int ps;
    char buf[80],message[200];
    char *command;
    struct chatin chatpf;

    printf("\r\n 도움말을 보시려면 '/h'를 치세요.");
    sprintf(buf,"\r\n %s님이 들어오셨습니다.",pf.id);
    shmaddr = shmat(shmid,NULL,MODE);
    strcpy(shmaddr,buf);
    shmdt(shmaddr);
    esc_chatting = 0;
    sprintf(buf,"tmp/ROOM.%d",cellnum);
    fp1 = fopen(buf,"r");
    while(fread((char*)&chatpf,sizeof(struct chatin),1,fp1)) {
	if(!strcmp(chatpf.tty,tty)) break;
    }
    fclose(fp1);

    if((ps = fork()) !=0 ) {
	while(1) {
	    keyinput(buf,200);
	    printf("\r\n");
	    if(buf[0] == '/') {
		j = strlen(buf);
		command =(char*) &buf[1];
		domenucommand(command,chattingcmd);
	    }
	    else {  /* 대화 내용을 공유메모리로 보낸다. */
		sprintf(message,"%s(%s) : %s",chatalias,pf.id,buf);
		shmaddr = shmat(shmid,NULL,MODE);
		strcpy(shmaddr,message);
		shmdt(shmaddr);
	    }
	
	    if (esc_chatting) {
		sprintf(buf,"\r\n %s님이 퇴장하십니다.",pf.id);
		shmaddr = shmat(shmid,NULL,MODE);
		strcpy(shmaddr,buf);
		shmdt(shmaddr);
		kill(ps,SIGTERM);
		return;
	    }
	}
    }
    else {
	sprintf(buf,"%d",cellnum);
	execl("bin/chatt","chatt",buf,tty,NULL);
    }

}


inviting(id)
char *id;
{
    int i,j,cellnum;
    char *tmp;
    char buf[80];
    FILE *fp1,*fp2;
    int fout;
    struct chatin chatpf;
    struct roomname cell;
    
    j = strlen(id);
    for(i=0; i<j; i++) {
	if (id[i]==' ') {
	    id[i] = '\0';
	    break;
	}
    }
    fp1 = fopen("tmp/WHOISNOW","r");
    while(fread((char*)&work,sizeof(struct nowwork),1,fp1)) {
	if(!strcmp(work.id,id)) {
	    if(work.chose=='*') {
		printf("\r\n 초대 거부 상태입니다.");
	    }
	    else {
		sprintf(buf,"/dev/%s",work.tty);
		fout = open(buf,1);
		for(i=0;i<6;i++) {
		    sprintf(buf,"tmp/ROOM.%d",i);
		    fp2 = fopen(buf,"r");
		/* 초청한 사람의 방을 찾아내는 코드 */
		    while(fread((char*)&chatpf,sizeof(struct chatin),1,fp2)) {
			if(!strcmp(chatpf.tty,tty)) {
			    cellnum = i;
			    break;
			}
		    }
		    fclose(fp2);
		}
		fp2 = fopen("tmp/ROOMINFO","r");
		fseek(fp2,cellnum*sizeof(struct roomname),0);
		fread((char*)&cell,sizeof(struct roomname),1,fp2);
	 	if(cell.passwd[0]=='\0') {
                    sprintf(buf,\
		"\r\n\007*****(공개)%d번방:%-8s 초대합니다*****\r\n",\
                        cellnum,pf.id);
		}
		else {
                    sprintf(buf,\
			"\r\n\007*****(비공개)%d번방:%-8s 암호:%-8s*****\r\n",\
                        cellnum,pf.id,cell.passwd);
		}
	 	write(fout,buf,strlen(buf));
		close(fout);
		printf("\r\n %s 회원을 초대했습니다.",id);
	    }
	}		
    }



}	
