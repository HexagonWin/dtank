
BBSDIR=$1

# ��񿡽� ��� �ʿ��� ��� ���丮�� �����.
echo ��񿡽��� �ʿ��� ���丮�� ����ϴ�.
mkdir $BBSDIR
chmod a+rw $BBSDIR
mkdir $BBSDIR/bin
mkdir $BBSDIR/boards
mkdir $BBSDIR/fboards
mkdir $BBSDIR/ftp
mkdir $BBSDIR/letter
mkdir $BBSDIR/logfile
mkdir $BBSDIR/logo
mkdir $BBSDIR/outbin
mkdir $BBSDIR/sysopfile
mkdir $BBSDIR/tmp

mkdir $BBSDIR/circle
mkdir $BBSDIR/eletter
mkdir $BBSDIR/oletter
mkdir $BBSDIR/munhun
mkdir $BBSDIR/page
mkdir $BBSDIR/qletter
mkdir $BBSDIR/user
mkdir $BBSDIR/ftp/files
mkdir $BBSDIR/ftp/fail
mkdir $BBSDIR/ftp/tmp

# ��񿡽� ���丮�� �㰡���� �����.
chmod 777 $BBSDIR/bin
chmod 777 $BBSDIR/boards
chmod 777 $BBSDIR/fboards
chmod 777 $BBSDIR/ftp
chmod 777 $BBSDIR/letter
chmod 777 $BBSDIR/logfile
chmod 777 $BBSDIR/logo
chmod 777 $BBSDIR/outbin
chmod 777 $BBSDIR/sysopfile
chmod 777 $BBSDIR/tmp
chmod 777 $BBSDIR/circle
chmod 777 $BBSDIR/eletter
chmod 777 $BBSDIR/oletter
chmod 777 $BBSDIR/munhun
chmod 777 $BBSDIR/page
chmod 777 $BBSDIR/qletter
chmod 777 $BBSDIR/user
chmod 777 $BBSDIR/ftp/files
chmod 777 $BBSDIR/ftp/fail
chmod 777 $BBSDIR/ftp/tmp

# ��񿡽� ��� �ʿ��� �⺻���� ȭ���� ī���Ѵ�
echo ��񿡽� ��� �ʿ��� �⺻���� ȭ���� ī���մϴ�.
cp wooky checktime chatt $BBSDIR/bin
cp MENUS DOLTANK.CFG PASSWDS ADVERTISING $BBSDIR/bin
chmod a+rw $BBSDIR/bin/BBSMENUS
chmod a+rw $BBSDIR/bin/DOLTANK.CFG
chmod a+rw $BBSDIR/bin/PASSWDS

# �ٳ�����.
echo install.sh�� ���� �˸��ϴ�.

