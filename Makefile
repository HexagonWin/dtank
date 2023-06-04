# ����ũ ���� 1.1.0 
# ����ũ ȭ���Դϴ�. 
# 
# �̰��� ��񿡽��� ��ġ�� ���丮�Դϴ�.***�ν����� �� �̰��� �� �����մϴ�.
# �̰� �ܿ��� �մ��� �ʴ� ���� �����ϴ�.
BBSDIR= /home/hexagonwin/dtank

SHELL=  /bin/sh
CC=     gcc48 -g

CFLAGS= -O0
LFLAGS= 

LIBS=  
LLIBS=  

# what are we making
SRCS    =files.c  io.c maintain.c boardisplay.c circle.c contentdisplay.c\
	editor.c guest.c profile.c chatting.c etc.c idcheck.c main.c\
	sysop.c commands.c ccommands.c newprofile.c maildisplay.c\
	office.c mail.c texteditor.c database.c menustruct.c convertmenu.c\
	kongzidisplay.c hitelemul.c

OBJS    =files.c  io.o maintain.o boardisplay.o circle.o contentdisplay.o\
	editor.o guest.o profile.o chatting.o etc.o idcheck.o main.o\
	sysop.o commands.o ccommands.o newprofile.o maildisplay.o\
	office.o mail.o texteditor.o database.o menustruct.o convertmenu.o\
	kongzidisplay.o hitelemul.o
CKSRC   = checktime.c
CKOBJ   = checktime.o

CHATSRC = chatt.c
CHATOBJ = chatt.o

DIRSTRUCTURE = bin boards fboards ftp letter logfile logo outbin sysopfile \
               tmp
# rules

all:    wooky  checktime chatt

wooky:  $(OBJS)
	$(CC) $(LFLAGS) -o $@ $(OBJS) $(LIBS)  

checktime: $(CKOBJ)
	$(CC) $(LFLAGS) -o $@ $(CKOBJ) $(LIBS) 

chatt: $(CHATOBJ)
	$(CC) $(LFLAGS) -o $@ $(CHATOBJ) $(LIBS)

# �̰��� �ν����� ��Ű�� �κ��̴�. ���丮������ ��� �ν��� �غ� ��ģ��.

install: 
	-sh install.sh $(BBSDIR)

update: all
	mv wooky $(BBSDIR)/bin
	mv checktime $(BBSDIR)/bin
	mv chatt $(BBSDIR)/bin

versionup: all
	mkdir $(BBSDIR)/etc

clean:
	rm -f *.o core *.out *~ .depend Make.Log Makefile.old

clobber: clean
	rm -f getty uugetty

realclean: clobber


