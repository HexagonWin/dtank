/* 돌탱크호스트의 헤더화일입니다. */
/* 특징이라고 한다면 게시물구조체와 화일란구조체가 똑같다는 것입니다. */

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

#define NA 0
#define YEA 1
char argument[30][20]; /* 크기 20의 30개까지의 인자를 입력받는다. */
int checkbufferint;
int upermenu; /* 이 것은 게시판이나 화일란, 메일에서 빠져 나올 때
			상위 메뉴로 갈 것인가를 알려주는 것이다.
			게시판에서 다음 게시판으로 가는 n이나
			a때문에 필요한 것이다 */

int startoffice; /* 사무실에 처음 들어갈 때 1로 세팅...*/

int highuser; /* 유료회원일경우 1로 세팅 */
int CRT;   /* 화면에 나타날 게시물 줄수 */
int trace; /* 이것은 회원 모니터링을 하느냐에 대한 플랙이다. */
int reprint; /* 화면 재생여부에 관한 매개수 */
int relogo;  /* 로고 재 표시에 대한 여부 */
char genbuf[255]; /* 일반버퍼 간단한 작업이 필요할 때 임시로 쓰는 또다른 일반버퍼이다 */
char tty[15]; /* 접속 포트 */
char tmp_buf[256],temp[256],mypass[9],**comm,*shmaddr,*shmat();
char li[10],ln[20],lt[40],ld[30];  /*게시판 검색용 */
int  start,go,menu_num,level,shmid_id,child_pid,room,shmid,guest;
int  now_num, top_num, menu_num, esc_flag, esc_sub, write_size, cs, end_flag; 

int circlecheck; /* 동호회 부분 체크 매개변수 */

int esc_chat; /* 채팅에서 빠질 때의 매개변수 */
int esc_chatting; /* 채팅방에서 빠질 때의 매개변수 */
int esc_circle; /* 동호회에서 나갈라면 */

int fos, pf_fos; /* id_pf 상에서의 자기 정보 offset */
char time_chk[30];
FILE *tfp;  /* time_chk에서 쓰일 화일 스트림 */
struct idst pf, you_pf;   /* pf는 전체에서 쓰일 자기정보 */
struct menustruct *menu,*nmenu,*mtop,*mtmp;  /* 메뉴구성에 관한 구조체이다
						전역에 걸쳐 필요 */
struct cmenustruct *cmenu,*cnmenu,*cmtop,*cmtmp; /* 동호회 메뉴구성*/

struct dis_list l_title;   /* 게시판 fileheader용이다(펑션간 사용할 때) */ 
struct dis_list f_title;   /* 화일게시판 fileheader용이다(펑션간 사용할 때) */
struct nowwork work;       /* 현재 접속중인 사용자 */
struct lestfile lest;      /* 로그아웃시 사용되는 구조체 */
struct fcfg cfg;           /* 비비에스 기본 config 화일 */
struct mail letter;        /* 편지 fileheader용이다 */ 
struct mail_dis mail_chk;  /* 메일 체크용 fileheader이다. */
struct termio systerm, mbuf;   
struct shmid_ds ss;        /* 채팅용 공유메모리, 차단표시기 */

int mycellnum;  /* 전역변수: 채팅방에서 쓸 자기가 들어간 방이름 */
		/* 접속이 끊길 때 이곳에 있는 자신을 지우고
			접속 끊김을 알린다.*/
int chatcheck;   /* 만일 채팅방(대기실 빼고)에 있었다면 1로 세팅 */
char chatalias[10]; /* 채팅방에서 쓰일 별명 */
static int timed_out;
char indata[30];
extern char *gets();
int host_end(),bye();
int dis_read(),chose_menu();
void get_menu(),logo(),mess(),my_pf(),chk_csysop(),del_mylogin();
void mess_dis(),change_info(),change_flag();
void text_append(),text_list(),text_edit(),text_insert(),text_delete(),unlink_text();
void menu_free(),display_vtx(),host_end_yn();

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2
/* 가장 중요한 특징이다. 모닥불에서의 것을 수정해서 사용하고 있다. */
struct config{              /* 메뉴화일 구성 */
    char form;              /* 메뉴 형태 */
    int level;              /* 레벨 증가분 */
    int del;                /* 삭제 권한 플래그 */
    char ccode[10];          /* 가기 코드 */
    int w;                  /* 쓰기,업 레벨 */
    int r;                  /* 읽기,다운 레벨 */
    char list[40];          /* 메뉴 제목 */
    int deep;               /* 게시물 깊이 */
    struct config *downp;   /* 하위메뉴 위치 */
    struct config *upp;     /* 상위메뉴 위치 */
    struct config *aeqp;    /* 이전메뉴 위치 */
    struct config *neqp;    /* 다음메뉴 위치 */
    char file[30];          /* 화일위치 디렉토리 */
};
/* 새로운 menu struct구조체 */
struct menustruct {
    char choice[5];     /*메뉴를 선택할 때 쓰는 번호나 영문 */
    char form;
    int level;
    int del;
    char ccode[10];
    int w;
    int r;
    char list[40];
    int deep;
    struct menustruct *downp;
    struct menustruct *upp;
    struct menustruct *aeqp;
    struct menustruct *neqp;
    char file[30];
};

/* 동호회 메뉴구성 */
struct cmenustruct {
	char choice[5];
	char form;
	int level;
	int del;
	char ccode[10];
	int w;
	int r;
	char list[40];
	int deep;
	struct cmenustruct *downp;
	struct cmenustruct *upp;
	struct cmenustruct *aeqp;
	struct cmenustruct *neqp;
	char file[30];
	char manager[10];	/* 게시물,화일 관리자 */
};

struct oldcmenustruct {
        char form;
        int level;
        int del;
        char ccode[10];
        int w;
        int r;
        char list[40];
        int deep;
        struct cmenustruct *downp;
        struct cmenustruct *upp;
        struct cmenustruct *aeqp;
        struct cmenustruct *neqp;
        char file[30];
        char manager[10];       /* 게시물,화일 관리자 */
};

/* 회원의 정보이다. */
struct idst {               /* 회원 정보 */
    int num;                /* 회원 일련 번호 */
    char id[9];             /* 아이디 */
    char passwd[9];         /* 비밀번호 */
    char name[9];           /* 성명 */
    int flag;               /* 회원정보 공개 플래그 */
    char sex;               /* 성별 (0x01) 회원구분 */
    char birthday[12];      /* 생년월일 (0x02) */
    time_t inday;           /* 가입일자 (0x04) */
    time_t logout;          /* 마지막 접속일 (0x08) */
    char post[8];           /* 우편번호 (0x10) */
    char home_addr[80];     /* 집주소 (0x20) */
    char home_tel[15];      /* 집전화번호 (0x40) */
    char office_name[80];   /* 직장명 (0x80) */
    char office_tel[15];    /* 직장전화번호 (0x100) */
    char id_no[15];         /* 주민등록번호 (0x200) */
    int circle;             /* 동아리 가입 여부 */
    int level;              /* 레벨 (0x400) */
    char expflag;           /* 확장 플래그 */
    int logcount;           /* 접속횟수 */
    time_t totaltime;       /* 총사용시간기록 */
    time_t lasttime;        /* 유효 사용일 */
};
/* 게시물 정보 화일헤더 */
struct dis_list {           /* 게시물 정보 */
    int num;                /* 일련 번호 */
    char id[10];            /* 작성자 아이디 */
    char name[10];          /* 작성자 이름 */
    time_t date;            /* 작성 일자 */
    int line;               /* 내용 길이(줄수) */
    int look;               /* 조횟수 */
    int feel;               /* 공감횟수(나중사용을 위해 reserved) */
    char title[40];         /* 제목 */
    long int file_size;     /* 화일일경우 사이즈,게시물은 유보 */
    int down;               /* 화일일경우 다운수,게시물은 유보 */
    int position;           /* 내용 포인터(쓸모없게 되어버렸다) */
    int size;               /* 내용 크기 */
    char bfilename[30];	    /* 내용이 저장된 화일이름. */
    char filename[30];      /* 이것은 나중에 예비용 */
    int thread;		    /* thread게시판 쓰기용으로 reserved */
};
/* 죽은 구조체 사용하지 않습니다.*/
struct mail {               /* 편지 */
    time_t tf;              /* 기록 시간 */
    int num;                /* 일련 번호 */
    char T;                 /* 읽기 확인 */
    char id[10];             /* 보낸사람 아이디 */
    int id_num;             /* 보낸사람 일련번호필요없게 되었다 */
    char name[10];           /* 보낸사람 이름 */
    char title[40];         /* 제목 */
    int line;               /* 편지 줄수 */
    long int point;         /* 내용 포인터 필요없게 되었다 */
    long int size;          /* 편지 크기 */
    char bfilename[30];	    /* 편지 게시물 화일네임 */
    char filename[30];      /* 편지 자료물 화일네임 */
};

struct mail_dis {           /* 편지 확인 */
    time_t tf;              /* 기록 시간 */
    char id[10];             /* 받는 사람 아이디 */
    char name[10];           /* 받는 사람 이름 */
    time_t date;            /* 읽은 날짜 */
    char title[40];         /* 보낸 제목 */
};
struct mailist {
    char group[20];
    char towhom[1000];
};

/* fcfg화일은 계속 갱신가능 */
struct fcfg {               /* 초기값 */
    char guide[10];          /* 공지사항 */
    int del;                /* 삭제레벨 */
    int sh;                 /* 쉘사용레벨 */
    int chat;               /* 비공개 대화레벨 */
    int seroom;             /* 비공개대화실갯수 */
    int inc;                /* 접속레벨증가분 */
    char secret[10];        /* 운영자 비밀번호 */
    int flag;               /* 셋팅 플래그(종료, 유효일) */
    int room;               /* 대화실 인원 */
    int genmaxlevel;        /* 일반 사용자의 레벨의 상한(자동증가가멈춘다)*/ 
    
};
/* 현재 접속자의 명단을 보여준다. */
struct nowwork {            /* 접속자 명단 */
    char chose;             /* 초대상태 */
    char id[10];             /* 아이디 */
    char name[10];           /* 이름 */
    char ccode[10];         /* 있는곳 */
    char tty[15];           /* 접속 노드 */
};
/* 접속 기록 화일입니다. */
struct lestfile {           /* 접속 기록 화일 */
    char id[10];             /* 아이디 */
    char name[10];           /* 이름 */
    time_t intime;          /* 접속 시간 */
    time_t outtime;         /* 종료 시간 */
    char tty[15];           /* 접속 노드 */
    char flag;              /* 확장플래그 값 */
};

struct roomname {   /* 대화실상태 */
    int flag;
    int howmany;
    char passwd[10];
    char name[60];
} roominfo;

struct line {    /* 에디터에서 쓰일 구조체(라인 에디터임) */
    struct line *left;   /* 전 줄 */
    struct line *right;  /* 다음 줄 */
    int linenum;         /* 라인넘버 (reserved) */
    char text[80];       /* 줄 내용 */
} *t_start, *t_now, *t_end, *t_tmp;

struct chatin{  /* 채팅 방에서 쓰일 구조체 */
    char id[10];      /* 아이디 */
    char chatid[10]; /*채팅방에서의 채팅별명 */
    char name[10];    /* 이름 */
    char tty[15];    /* 접속 포트 */
};

struct commands {  /* 이것은 명령어 집합이다. */
        char *cmdname;        	/* 코멘드 이름 */
        int (*cmdfunc)(); 	/* 코멘드 펑션 */
        int level;         	/* 사용가능레벨 */
};


struct report {   	/* 이것은 사용기록을 할 때 쓰는 구조체 */
	char date[40];	
	char id[10];
	char tty[15];
	char ccode[10];
	char whatdo[40];
};	
struct Memo {
	char id[10];
	char date[40];
	char content[80];
};

struct Munhun {
 	char title[40]; /* 도서명 */
	char writer[10]; /* 저자명 */
	char translator[10]; /* 역자명 */
	char publisher[10];  /* 출판사 */
	time_t publishdate;  /*  출판연일 */
	int controlnumber;   /* 제어번호 */
};


extern struct commands contentcmd[];
extern struct commands fcontentcmd[];
extern struct commands tcontentcmd[];
/* 채팅방 관련 명령어 일람 */
extern struct commands chatloungecmd[];
extern struct commands chattingcmd[];
/* 동호회 가입신청 일람 */
extern struct commands applycmd[];
extern struct commands menucmd[];
extern struct commands boardcmd[];
extern struct commands filecmd[];
/* 동호회 관련 명령어 일람 */
extern struct commands ccontentcmd[];
extern struct commands cfcontentcmd[];
extern struct commands cmenucmd[];
extern struct commands cboardcmd[];
extern struct commands cfilecmd[];
/* 메일 관련 명령어 일람 */
extern struct commands mailcmd[];

extern char *FTITLELINE,*TITLELINE,*HELPLINE,*CHATHELP;
extern struct pread {
	long int fbp;
	long int fos;
	int ie;
	struct pread *next;
} *fbp1,*fbp2;


