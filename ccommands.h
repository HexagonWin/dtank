/* ���⿡ ��ɾ��� ������ �ֽ��ϴ�. ���� �����ϱ� ������. */
int cwriteboard();
int edittext(char*),listtitle(char*),edittext(char*);
int nexttitle(),pretitle(),gototop(),byebye(),help();
int premenu(),nextmenu();
int gotomenu(char*),feeltext(char*);
int downfile(char*),upfile();
int delftext(char*);
int supersysop(),gotoshell();
int pfsomeone(char*);
int redisplay();
int message(char*),sendmemo(char*);
int editfileheader(char*);
int whoisnow();
int hideme();
/* ��ȣȸ �κ��Դϴ�. */
int cuppermenu();
int cpremenu();
int cnextmenu();
int cdownfile(char*);
int cupfile();
int cgotomenu(char*);
int cboardsort();
int delcboard(char*);
int cmakemenu();
/* �̰��� commands.h���� ����� �Ͱ� ���� �͵��̴� */
int quitboard(),quitfboard(),quitmail();
int boardli(char*);
int boardlt(char*);
int boardld(char*);
int boardln(char*);
int cmoveboard(char *);

struct commands cboardcmd[] = {
  	"sortboard",	cboardsort,	100,
        "memo", sendmemo,       0,
	"mv",	cmoveboard,	0,
        "us",   whoisnow,       0,
        "pf",   pfsomeone,      0,
        "go",   cgotomenu,       0,
        "dd",   delcboard,	0,
	"li",	boardli,	0,
	"lt",	boardlt,	0,
        "ls",   listtitle,      0,
        "to",   message,        0,
        "w",    cwriteboard,     0,
        "f",    nexttitle,      0,
        "b",    pretitle,       0,
        "t",    gototop,        0,
        "x",    byebye,         0,
        "p",    quitboard,      0,
        "h",    help,           0,
        "a",    cpremenu,        0,
        "n",    cnextmenu,       0,
        "z",    redisplay,      0,
        NULL,   NULL,           0,      
};
int delcfboard(char*);
int cfilesort();

struct commands cfilecmd[] = {
	"sortfiles",	cfilesort,	100,
        "header",       editfileheader, 0,
        "edit", edittext,       0,
        "us",   whoisnow,       0,
        "pf",   pfsomeone,      0,
	"li",	boardli,	0,
	"lt",	boardlt,	0,
	"ln",	boardln,	0,
        "ls",   listtitle,      0,
        "go",   cgotomenu,       0,
        "dd",   delcfboard,        0,
        "dn",   cdownfile,      0,
        "up",   cupfile, 	0,
        "to",   message,        0,
/*        "w",    writeboard,     0,   ���� ������ ���� */
        "f",    nexttitle,      0,
        "b",    pretitle,       0,
        "t",    gototop,        0,
        "x",    byebye,         0,
        "p",    quitfboard,	0,
        "h",    help,           0,
        "a",    cpremenu,        0,
        "n",    cnextmenu,       0,
        "z",    redisplay,      0,
        NULL,   NULL,           0,      
};

struct commands ccontentcmd[] = {
        "edit", edittext,       0,
        "header",       editfileheader, 0,
        "us",   whoisnow,       0,
        "pf",   pfsomeone,      0,
        "feel", feeltext,       0,
/*      "w",    cwriteboard,     0,   �ȿ� �θ� �ȵȴ�. ���� ��ģ��.���� */
        "t",    gototop,        0,
        "h",    help,           0,
        "x",    byebye,         0,
        NULL,   NULL,           0,
};

struct commands cfcontentcmd[] = {
        "header",       editfileheader, 0,
        "edit", edittext,       0,
        "us",   whoisnow,       0,
        "pf",   pfsomeone,      0,
        "feel", feeltext,       0,
/*        "dn",   cdownfile,       0, */
        "w",    cwriteboard,     0,
        "t",    gototop,        0,
        "h",    help,           0,
        "x",    byebye,         0,
        NULL,   NULL,           0,
};
int removeclogo();
int rzclogo();

struct commands cmenucmd[] = {
	"makemenu",	cmakemenu,	1000,
        "removelogo",   removeclogo,     0,
        "receivelogo",  rzclogo,         0,
        "us",   whoisnow,       0,
        "pf",   pfsomeone,      0,
        "go",   gotomenu,       0,
        "to",   message,        0,
        "f",    nexttitle,      0,
        "b",    pretitle,       0,
        "t",    gototop,        0,
        "x",    byebye,         0,
        "p",    cuppermenu,	0,
        "h",    help,           0,
        "a",    cpremenu,        0,
        "n",    cnextmenu,       0,
        "z",    redisplay,      0,
        NULL,   NULL,           0,      
};
 
