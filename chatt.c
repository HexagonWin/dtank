/* 이것은 독립 프로그램이다. */
/*          chatt.c     공유메모리에 글자를 쓰는 프로그램(중요 알고리즘)   */ 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <ctype.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <termios.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SIZE 300
#define MODE 0666

struct shmid_ds ss;
int host_end();

main(argv,argc)
int argv;
char **argc;
{
    char tmp_buf[256],temp[256],*shmaddr,*shmat(),*tmp,*buf;
    char file[30];
    char c;
    char tty[10];
    FILE *fp1;
    int  shmid_id,pid,room,shmid;
    buf = tmp_buf;
    tmp = temp;

    signal(SIGTERM, (__sighandler_t)host_end);
    signal(SIGHUP, (__sighandler_t)host_end);
    sprintf(tty,argc[2]);
    room = atoi(argc[1]);
    if((shmid=shmget((key_t)room, SIZE, IPC_CREAT | MODE)) == -1)
        exit(0);
    pid = getpid();
    while(1) {  /* 대화 내용 읽기 */
        shmaddr = shmat(shmid, NULL, MODE);
        strcpy(buf,shmaddr);
        shmdt(shmaddr);
        if(!strcmp(tmp,buf));
        else {
            printf("%s\r\n",buf);
            strcpy(tmp,buf);
        }
        shmctl(shmid, IPC_STAT, &ss);
        while(ss.shm_lpid == pid)
        shmctl(shmid, IPC_STAT, &ss);
        sprintf(file,"tmp/STAT.%s",tty);
        fp1 = fopen(file,"r");
	c = fgetc(fp1);
	if(c != '4' ) {   /* 상태 화일이 채팅이 아니면 끝 */
	    break;
 	}
        fclose(fp1);

    }
}

host_end()
{
    exit(0);
}

