
#include "bbs.h"

int esc_chat,esc_chatting; /* esc_chat�� ���ǿ���, esc_chatting�� 
				ä�ù濡�� ������ �Ű������̴�. */
#define SIZE 300
#define MODE 0666
/* ä�� ���� ���α׷� */
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
    fputc('4',tfp);  /* ä�����̶�� ���� �˸� */
    fclose(tfp);

    if(( fp1 = fopen("tmp/ROOMINFO","r+"))==NULL) {
	fp1 = fopen("tmp/ROOMINFO","a");
    }
    fread((char*)&cell,sizeof(struct roomname),1,fp1);
    cell.flag =1;
    cell.howmany++;
    cell.passwd[0] = '\0';
    strcpy(cell.name,"�����");
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
    showcell(); /* ������� �����ش�. */
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
    fputc('0',tfp);  /* ������·� ȯ�� */
    fclose(tfp);

    reprint=YEA;
    return;

}

/* ������ ���¸� �����ش�. */
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
		/* �ο����� ROOM.?ȭ���� �����ϴ� ���� Ȯ���ϴ�.*/
	        fseek(fp2,0,2); howmany = ftell(fp2) / sizeof(struct chatin);
	        fseek(fp2,0,0);
		if(num ==0 ) {
		    printf("\r\n �����(����) %d��",howmany);
		}
 	        else if(cell.passwd[0]=='\0') {
		    printf("\r\n %d����(����) [%s] �ο�:%d",num,cell.name,cell.howmany);
	        }
	        else  {
		    printf("\r\n %d����(�����) [%s] �ο�:%d",num,cell.name,cell.howmany);
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
	
/* ȸ���� ��ȭ�濡 �߰��Ѵ�. */
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
    sprintf(buf,"tmp/ROOM.%d",cellnum); /* ��ȭ�濡 ����� ����ְ� */
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
    cell.howmany = howmany; /* ��ȭ���� ������ �ο� �� ������ */
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
        printf("\r\n �޸� ���� "); pressreturn();
        exit(1);
    }
    fseek(fp1,0,0);
    num = 0;
    for(i = 0 ; i<howmany; i++) {
        fread((char*)&chatid[i],sizeof(struct chatin),1,fp1);
        if(strcmp(chatid[i].tty,tty)) {
	    printf("\r\n %s[%s]���� ��ȭ�濡 �����ϴ�.",chatid[i].name,chatid[i].tty);
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
    cell.howmany = num;   /* ���� ������� ���δ�. */
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
    chatcheck = 1; mycellnum = cellnum;/*�������� */
    printf("\r\n %d �� �濡 �����̽��ϴ�.",cellnum);
    printf("\r\n");for(i=0;i<80;i++) printf("-");
    deluser(0); /*  ���ǿ� �ִ� �ڽ��� �����. */
    adduser(cellnum); /* ä�ù濡 �ڽ��� ����. */
    chatstart(shmid,cellnum); /* ä���� �����Ѵ�. */
    deluser(cellnum); /* ä�ù濡�� �ڽ��� �����. */
    adduser(0); /* ���ǿ� �ڽ��� �ٽ� ��ִ´�. */
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

    printf("\r\n ������ ���÷��� '/h'�� ġ����.");
    sprintf(buf,"\r\n %s���� �����̽��ϴ�.",pf.id);
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
	    else {  /* ��ȭ ������ �����޸𸮷� ������. */
		sprintf(message,"%s(%s) : %s",chatalias,pf.id,buf);
		shmaddr = shmat(shmid,NULL,MODE);
		strcpy(shmaddr,message);
		shmdt(shmaddr);
	    }
	
	    if (esc_chatting) {
		sprintf(buf,"\r\n %s���� �����Ͻʴϴ�.",pf.id);
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
		printf("\r\n �ʴ� �ź� �����Դϴ�.");
	    }
	    else {
		sprintf(buf,"/dev/%s",work.tty);
		fout = open(buf,1);
		for(i=0;i<6;i++) {
		    sprintf(buf,"tmp/ROOM.%d",i);
		    fp2 = fopen(buf,"r");
		/* ��û�� ����� ���� ã�Ƴ��� �ڵ� */
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
		"\r\n\007*****(����)%d����:%-8s �ʴ��մϴ�*****\r\n",\
                        cellnum,pf.id);
		}
		else {
                    sprintf(buf,\
			"\r\n\007*****(�����)%d����:%-8s ��ȣ:%-8s*****\r\n",\
                        cellnum,pf.id,cell.passwd);
		}
	 	write(fout,buf,strlen(buf));
		close(fout);
		printf("\r\n %s ȸ���� �ʴ��߽��ϴ�.",id);
	    }
	}		
    }



}	
