/* 여기에 명령어의 집합이 있습니다. 아주 수정하기 좋지요. */
int writeboard(),uppermenu(),nothing();
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
int boardli(char*);
int boardlt(char*);
int boardld(char*);
int boardln(char*);

int boardsort();
int quitboard();

char *TITLELINE=\
"\r\n 번호 성  명   아이디   날 짜조회라인      제      목\
\r\n-------------------------------------------------------------------------------";
 
char *HELPLINE=\
"\r\n--------------------------------------------------------------------------------\
\r\n(도움말은H):h,p,a,n,t,x,go,dd ##[Return]";
char *FTITLELINE=\
"\r\n 번호 아이디   화일명           크 기       제     목\
\r\n-------------------------------------------------------------------------------";
char *CHATHELP = "\r\n 별명(nick),방들어가기(J),도움(H)>> ";
int delboard(char*);
int moveboard(char*);
struct commands boardcmd[] = {
	"sortboard",	boardsort,	1000,
	"hide",	hideme,		1000,
	"edit",	edittext,	0,
  	"memo",	sendmemo,	0,
	"mv",	moveboard,	1000,
	"li",	boardli,	0,
	"lt",	boardlt,	0,
	"ld",	boardld,	0,
	"us",	whoisnow,	0,
	"pf",	pfsomeone,	0,
        "go",   gotomenu,       0,
        "dd",   delboard,        0,
	"ls",	listtitle,	0,
	"to",	message,	0,
        "w",    writeboard,     0,
        "f",    nexttitle,      0,
        "b",    pretitle,       0,
        "t",    gototop,        0,
        "x",    byebye,         0,
        "p",    quitboard,	0, 
        "h",    help,           0,
        "a",    premenu,        0,
        "n",    nextmenu,       0,
	"z",	redisplay,	0,
        NULL,   NULL,           0,      
};
int delmail(char*);
int mailsort();
int quitmail();

struct commands mailcmd[] = {
	"sortmails",	mailsort,	0,
        "hide", hideme,         1000,
        "edit", edittext,       0,
        "memo", sendmemo,       0,
        "li",   boardli,        0,
        "us",   whoisnow,       0,
        "pf",   pfsomeone,      0,
        "go",   gotomenu,       0,
        "dd",   delmail,        0,
        "ls",   listtitle,      0,
        "to",   message,        0,
        "w",    writeboard,     0,
        "f",    nexttitle,      0,
        "b",    pretitle,       0,
        "t",    gototop,        0,
        "x",    byebye,         0,
        "p",    quitmail,      0, 
        "h",    help,           0,
        "a",    premenu,        0,
        "n",    nextmenu,       0,
        "z",    redisplay,      0,
        NULL,   NULL,           0,      
};

struct commands contentcmd[] = {
	"edit",	edittext,	0,
	"header",	editfileheader,	0,
        "us",   whoisnow,       0,
	"pf",	pfsomeone,	0,
	"go",	gotomenu,	0,
	"feel",	feeltext,	0,
/*	"w",	writeboard,	0,   안에 두면 안된다. 글이 겹친다.버그 */
	"t",	gototop,	0,
	"h",	help,		0,
	"x",	byebye,		0,
	NULL,	NULL,		0,
};
int filesort();
int delfboard(char*);
int quitfboard();

struct commands filecmd[] = {
	"sortfiles",	filesort,	1000,	
	"header",	editfileheader,	0,
        "edit", edittext,       0,
	"li",	boardli,	0,
	"lt",	boardlt,	0,
	"ld",	boardld,	0,
	"ln",	boardln,	0,
	"us",	whoisnow,	0,
	"pf",	pfsomeone,	0,
        "ls",   listtitle,      0,
        "go",   gotomenu,       0,
        "dd",   delfboard,	0,
	"dn", 	downfile,	0,
	"up",	upfile,	0,
	"to",	message,	0,
/*        "w",    writeboard,     0,   위의 이유와 같음 */
        "f",    nexttitle,      0,
        "b",    pretitle,       0,
        "t",    gototop,        0,
        "x",    byebye,         0,
        "p",    quitfboard,	0,
        "h",    help,           0,
        "a",    premenu,        0,
        "n",    nextmenu,       0,
	"z",	redisplay,	0,
	NULL,   NULL,           0,      
};

struct commands tcontentcmd[] = {
	"header",	editfileheader,	0,
        "edit", edittext,       0,
        "us",   whoisnow,       0,
	"pf",	pfsomeone,	0,
        "go",   gotomenu,       0,
        "feel", feeltext,       0,
	"dn",	downfile,	0,
	"t",	gototop,	0,
	"h",	help,		0,
	"x",	byebye,		0,
        NULL,   NULL,           0,
};

struct commands fcontentcmd[] = {
        "header",       editfileheader, 0,
        "edit", edittext,       0,
        "us",   whoisnow,       0,
        "pf",   pfsomeone,      0,
        "go",   gotomenu,       0,
        "feel", feeltext,       0,
        "dn",   downfile,       0,
        "w",    writeboard,     0,
        "t",    gototop,        0,
	"h",	help,		0,
        "x",    byebye,         0,
        NULL,   NULL,           0,
};
int supermanager();
/* 서브메뉴가 있는 M에서의 명령어 */
int rzlogo();
int removelogo();

struct commands menucmd[] = {
	"removelogo",	removelogo,	0,
	"receivelogo",	rzlogo,		0,
	"supersysop",	supersysop,	0,	
	"supermanager",	supermanager,	0,
	"shell",	gotoshell,	0, 
        "us",   whoisnow,       0,
	"pf",	pfsomeone,	0,
        "go",   gotomenu,       0,
	"to",	message,	0,
        "w",    writeboard,     0,
        "f",    nexttitle,      0,
        "b",    pretitle,       0,
        "t",    gototop,        0,
        "x",    byebye,         0,
        "p",    uppermenu,      0,
        "h",    help,           0,
        "a",    premenu,        0,
        "n",    nextmenu,       0,
	"z",	redisplay,	0,
        NULL,   NULL,           0,      
};
int joincell(char*);
int invitechange();
int showROOM();
int helpchat();
int gotop();
int invite(char*),invitee(char*);
int exitchat(), exitchatting();
int nickname(char*);
int chatwhoisnow();

struct commands chatloungecmd[] = {
	"nick",	nickname,	0,
	"me",	invitechange,	0,
	"us",	chatwhoisnow,	0,
	"st",	showROOM,	0,
	"in",	invite,		0,
	"pf", 	pfsomeone,	0,
	"to",	message,	0,
	"go",	gotomenu,	0,
	"j",	joincell,	0,
	"h",	helpchat,	0,
	"p",	exitchat,	0,
	"t",	gotop,		0, 
	"x",	byebye,		0,
	NULL,	NULL,		0,
};

struct commands chattingcmd[] = {
	"nick",	nickname,	0,
	"q",	exitchatting,	0,
	"h",	helpchat,	0,
	"us",	chatwhoisnow,	0,
	"me",	invitechange,	0,
	"st",	showROOM,	0,	
	"pf",	pfsomeone,	0,
	"to",	message,	0,
/*	"wh",			0,
	"fi",			0,
*/	"in",	invite,		0,
	NULL,	NULL,		0,
};
/* 동호회 가입신청란에서 쓰일 명령어*/
struct commands applycmd[] = {
        "hide", hideme,         1000,
        "us",   whoisnow,       0,
        "pf",   pfsomeone,      0,
        "go",   gotomenu,       0,
        "to",   message,        0,
        "t",    gototop,        0,
        "x",    byebye,         0,
        "p",    uppermenu,      0,
        "h",    help,           0,
	"a", 	premenu,	0,
        "n",    nextmenu,       0,
        "z",    redisplay,      0,
        NULL,   NULL,           0,    
};
 
