# 돌탱크 버젼 1.1.0 
# 메이크 화일입니다. 
# 
# 이곳이 비비에스가 위치할 디렉토리입니다.***인스톨할 때 이곳을 꼭 수정합니다.
# 이곳 외에는 손대지 않는 것이 좋습니다.
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

# 이곳은 인스톨을 시키는 부분이다. 디렉토리구조와 모든 인스톨 준비를 마친다.

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


