/* ����ũȣ��Ʈ�� ���ȭ���Դϴ�. */
/* Ư¡�̶�� �Ѵٸ� �Խù�����ü�� ȭ�϶�����ü�� �Ȱ��ٴ� ���Դϴ�. */

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
char argument[30][20]; /* ũ�� 20�� 30�������� ���ڸ� �Է¹޴´�. */
int checkbufferint;
int upermenu; /* �� ���� �Խ����̳� ȭ�϶�, ���Ͽ��� ���� ���� ��
			���� �޴��� �� ���ΰ��� �˷��ִ� ���̴�.
			�Խ��ǿ��� ���� �Խ������� ���� n�̳�
			a������ �ʿ��� ���̴� */

int startoffice; /* �繫�ǿ� ó�� �� �� 1�� ����...*/

int highuser; /* ����ȸ���ϰ�� 1�� ���� */
int CRT;   /* ȭ�鿡 ��Ÿ�� �Խù� �ټ� */
int trace; /* �̰��� ȸ�� ����͸��� �ϴ��Ŀ� ���� �÷��̴�. */
int reprint; /* ȭ�� ������ο� ���� �Ű��� */
int relogo;  /* �ΰ� �� ǥ�ÿ� ���� ���� */
char genbuf[255]; /* �Ϲݹ��� ������ �۾��� �ʿ��� �� �ӽ÷� ���� �Ǵٸ� �Ϲݹ����̴� */
char tty[15]; /* ���� ��Ʈ */
char tmp_buf[256],temp[256],mypass[9],**comm,*shmaddr,*shmat();
char li[10],ln[20],lt[40],ld[30];  /*�Խ��� �˻��� */
int  start,go,menu_num,level,shmid_id,child_pid,room,shmid,guest;
int  now_num, top_num, menu_num, esc_flag, esc_sub, write_size, cs, end_flag; 

int circlecheck; /* ��ȣȸ �κ� üũ �Ű����� */

int esc_chat; /* ä�ÿ��� ���� ���� �Ű����� */
int esc_chatting; /* ä�ù濡�� ���� ���� �Ű����� */
int esc_circle; /* ��ȣȸ���� ������� */

int fos, pf_fos; /* id_pf �󿡼��� �ڱ� ���� offset */
char time_chk[30];
FILE *tfp;  /* time_chk���� ���� ȭ�� ��Ʈ�� */
struct idst pf, you_pf;   /* pf�� ��ü���� ���� �ڱ����� */
struct menustruct *menu,*nmenu,*mtop,*mtmp;  /* �޴������� ���� ����ü�̴�
						������ ���� �ʿ� */
struct cmenustruct *cmenu,*cnmenu,*cmtop,*cmtmp; /* ��ȣȸ �޴�����*/

struct dis_list l_title;   /* �Խ��� fileheader���̴�(��ǰ� ����� ��) */ 
struct dis_list f_title;   /* ȭ�ϰԽ��� fileheader���̴�(��ǰ� ����� ��) */
struct nowwork work;       /* ���� �������� ����� */
struct lestfile lest;      /* �α׾ƿ��� ���Ǵ� ����ü */
struct fcfg cfg;           /* ��񿡽� �⺻ config ȭ�� */
struct mail letter;        /* ���� fileheader���̴� */ 
struct mail_dis mail_chk;  /* ���� üũ�� fileheader�̴�. */
struct termio systerm, mbuf;   
struct shmid_ds ss;        /* ä�ÿ� �����޸�, ����ǥ�ñ� */

int mycellnum;  /* ��������: ä�ù濡�� �� �ڱⰡ �� ���̸� */
		/* ������ ���� �� �̰��� �ִ� �ڽ��� �����
			���� ������ �˸���.*/
int chatcheck;   /* ���� ä�ù�(���� ����)�� �־��ٸ� 1�� ���� */
char chatalias[10]; /* ä�ù濡�� ���� ���� */
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
/* ���� �߿��� Ư¡�̴�. ��ںҿ����� ���� �����ؼ� ����ϰ� �ִ�. */
struct config{              /* �޴�ȭ�� ���� */
    char form;              /* �޴� ���� */
    int level;              /* ���� ������ */
    int del;                /* ���� ���� �÷��� */
    char ccode[10];          /* ���� �ڵ� */
    int w;                  /* ����,�� ���� */
    int r;                  /* �б�,�ٿ� ���� */
    char list[40];          /* �޴� ���� */
    int deep;               /* �Խù� ���� */
    struct config *downp;   /* �����޴� ��ġ */
    struct config *upp;     /* �����޴� ��ġ */
    struct config *aeqp;    /* �����޴� ��ġ */
    struct config *neqp;    /* �����޴� ��ġ */
    char file[30];          /* ȭ����ġ ���丮 */
};
/* ���ο� menu struct����ü */
struct menustruct {
    char choice[5];     /*�޴��� ������ �� ���� ��ȣ�� ���� */
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

/* ��ȣȸ �޴����� */
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
	char manager[10];	/* �Խù�,ȭ�� ������ */
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
        char manager[10];       /* �Խù�,ȭ�� ������ */
};

/* ȸ���� �����̴�. */
struct idst {               /* ȸ�� ���� */
    int num;                /* ȸ�� �Ϸ� ��ȣ */
    char id[9];             /* ���̵� */
    char passwd[9];         /* ��й�ȣ */
    char name[9];           /* ���� */
    int flag;               /* ȸ������ ���� �÷��� */
    char sex;               /* ���� (0x01) ȸ������ */
    char birthday[12];      /* ������� (0x02) */
    time_t inday;           /* �������� (0x04) */
    time_t logout;          /* ������ ������ (0x08) */
    char post[8];           /* �����ȣ (0x10) */
    char home_addr[80];     /* ���ּ� (0x20) */
    char home_tel[15];      /* ����ȭ��ȣ (0x40) */
    char office_name[80];   /* ����� (0x80) */
    char office_tel[15];    /* ������ȭ��ȣ (0x100) */
    char id_no[15];         /* �ֹε�Ϲ�ȣ (0x200) */
    int circle;             /* ���Ƹ� ���� ���� */
    int level;              /* ���� (0x400) */
    char expflag;           /* Ȯ�� �÷��� */
    int logcount;           /* ����Ƚ�� */
    time_t totaltime;       /* �ѻ��ð���� */
    time_t lasttime;        /* ��ȿ ����� */
};
/* �Խù� ���� ȭ����� */
struct dis_list {           /* �Խù� ���� */
    int num;                /* �Ϸ� ��ȣ */
    char id[10];            /* �ۼ��� ���̵� */
    char name[10];          /* �ۼ��� �̸� */
    time_t date;            /* �ۼ� ���� */
    int line;               /* ���� ����(�ټ�) */
    int look;               /* ��Ƚ�� */
    int feel;               /* ����Ƚ��(���߻���� ���� reserved) */
    char title[40];         /* ���� */
    long int file_size;     /* ȭ���ϰ�� ������,�Խù��� ���� */
    int down;               /* ȭ���ϰ�� �ٿ��,�Խù��� ���� */
    int position;           /* ���� ������(������� �Ǿ���ȴ�) */
    int size;               /* ���� ũ�� */
    char bfilename[30];	    /* ������ ����� ȭ���̸�. */
    char filename[30];      /* �̰��� ���߿� ����� */
    int thread;		    /* thread�Խ��� ��������� reserved */
};
/* ���� ����ü ������� �ʽ��ϴ�.*/
struct mail {               /* ���� */
    time_t tf;              /* ��� �ð� */
    int num;                /* �Ϸ� ��ȣ */
    char T;                 /* �б� Ȯ�� */
    char id[10];             /* ������� ���̵� */
    int id_num;             /* ������� �Ϸù�ȣ�ʿ���� �Ǿ��� */
    char name[10];           /* ������� �̸� */
    char title[40];         /* ���� */
    int line;               /* ���� �ټ� */
    long int point;         /* ���� ������ �ʿ���� �Ǿ��� */
    long int size;          /* ���� ũ�� */
    char bfilename[30];	    /* ���� �Խù� ȭ�ϳ��� */
    char filename[30];      /* ���� �ڷṰ ȭ�ϳ��� */
};

struct mail_dis {           /* ���� Ȯ�� */
    time_t tf;              /* ��� �ð� */
    char id[10];             /* �޴� ��� ���̵� */
    char name[10];           /* �޴� ��� �̸� */
    time_t date;            /* ���� ��¥ */
    char title[40];         /* ���� ���� */
};
struct mailist {
    char group[20];
    char towhom[1000];
};

/* fcfgȭ���� ��� ���Ű��� */
struct fcfg {               /* �ʱⰪ */
    char guide[10];          /* �������� */
    int del;                /* �������� */
    int sh;                 /* ����뷹�� */
    int chat;               /* ����� ��ȭ���� */
    int seroom;             /* �������ȭ�ǰ��� */
    int inc;                /* ���ӷ��������� */
    char secret[10];        /* ��� ��й�ȣ */
    int flag;               /* ���� �÷���(����, ��ȿ��) */
    int room;               /* ��ȭ�� �ο� */
    int genmaxlevel;        /* �Ϲ� ������� ������ ����(�ڵ������������)*/ 
    
};
/* ���� �������� ����� �����ش�. */
struct nowwork {            /* ������ ��� */
    char chose;             /* �ʴ���� */
    char id[10];             /* ���̵� */
    char name[10];           /* �̸� */
    char ccode[10];         /* �ִ°� */
    char tty[15];           /* ���� ��� */
};
/* ���� ��� ȭ���Դϴ�. */
struct lestfile {           /* ���� ��� ȭ�� */
    char id[10];             /* ���̵� */
    char name[10];           /* �̸� */
    time_t intime;          /* ���� �ð� */
    time_t outtime;         /* ���� �ð� */
    char tty[15];           /* ���� ��� */
    char flag;              /* Ȯ���÷��� �� */
};

struct roomname {   /* ��ȭ�ǻ��� */
    int flag;
    int howmany;
    char passwd[10];
    char name[60];
} roominfo;

struct line {    /* �����Ϳ��� ���� ����ü(���� ��������) */
    struct line *left;   /* �� �� */
    struct line *right;  /* ���� �� */
    int linenum;         /* ���γѹ� (reserved) */
    char text[80];       /* �� ���� */
} *t_start, *t_now, *t_end, *t_tmp;

struct chatin{  /* ä�� �濡�� ���� ����ü */
    char id[10];      /* ���̵� */
    char chatid[10]; /*ä�ù濡���� ä�ú��� */
    char name[10];    /* �̸� */
    char tty[15];    /* ���� ��Ʈ */
};

struct commands {  /* �̰��� ��ɾ� �����̴�. */
        char *cmdname;        	/* �ڸ�� �̸� */
        int (*cmdfunc)(); 	/* �ڸ�� ��� */
        int level;         	/* ��밡�ɷ��� */
};


struct report {   	/* �̰��� ������� �� �� ���� ����ü */
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
 	char title[40]; /* ������ */
	char writer[10]; /* ���ڸ� */
	char translator[10]; /* ���ڸ� */
	char publisher[10];  /* ���ǻ� */
	time_t publishdate;  /*  ���ǿ��� */
	int controlnumber;   /* �����ȣ */
};


extern struct commands contentcmd[];
extern struct commands fcontentcmd[];
extern struct commands tcontentcmd[];
/* ä�ù� ���� ��ɾ� �϶� */
extern struct commands chatloungecmd[];
extern struct commands chattingcmd[];
/* ��ȣȸ ���Խ�û �϶� */
extern struct commands applycmd[];
extern struct commands menucmd[];
extern struct commands boardcmd[];
extern struct commands filecmd[];
/* ��ȣȸ ���� ��ɾ� �϶� */
extern struct commands ccontentcmd[];
extern struct commands cfcontentcmd[];
extern struct commands cmenucmd[];
extern struct commands cboardcmd[];
extern struct commands cfilecmd[];
/* ���� ���� ��ɾ� �϶� */
extern struct commands mailcmd[];

extern char *FTITLELINE,*TITLELINE,*HELPLINE,*CHATHELP;
extern struct pread {
	long int fbp;
	long int fos;
	int ie;
	struct pread *next;
} *fbp1,*fbp2;


