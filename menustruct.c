/* �޴� ������ ���� �Լ��̴� */
/* ���� �߿��� Ư¡�� �Լ����� ���ִ� */
/* 1.3.0���� ������(����ũ)�� �� �ʿ��� ������ �Լ����̴� */
#include "bbs.h"

Make_Menu(int start)
{
    int i, j = 0, key = 1;
    char ch, buf[80];
    if(start) { 
        if((mtmp=(struct menustruct *)malloc(sizeof(struct menustruct))) ==NULL) {
            printf("\r\n�޸𸮰� �����ϳ׿�.");
            exit(1);
        }
        menu=mtop=nmenu=mtmp;
        if((mtmp=(struct menustruct *)malloc(sizeof(struct menustruct))) ==NULL) {
            printf("\r\n�޸𸮰� �����ϳ׿�.");
            exit(1);
        }
        nmenu->neqp=nmenu->aeqp=NULL;
        nmenu->upp=nmenu;
        nmenu->downp=mtmp;
        nmenu->form='M'; nmenu->level=nmenu->del=nmenu->w=nmenu->r = nmenu->deep=0;
        strcpy(nmenu->ccode,"top");
        strcpy(nmenu->list,"����ũ");
        strcpy(nmenu->file,"");
        mtmp->neqp=mtmp->aeqp=mtmp->downp=NULL;
        mtmp->upp=mtop;
    }
    while(1) {
        if(key) {
	    cls();
            printf("\r\n  %s      [%s]\r\n",mtop->list,mtop->ccode);
            nmenu = mtop->downp;
            j = 0;
	    printf("\r\nnum ���� F Ÿ��Ʋ      �����ڵ�   �б�  ����    ����    ����");
            while(nmenu->neqp != NULL) {
                printf("\r\n  %2d %3s %c %10s [%-8s]",++j,nmenu->choice,nmenu->form,nmenu->list,nmenu->ccode);
                printf(" %5d %5d : %5x : %5d",nmenu->w,nmenu->r,nmenu->del,nmenu->level);
                if(nmenu->form == 'f' || nmenu->form == 'F') printf(" %s",nmenu->file);
                nmenu = nmenu->neqp;
            }
            printf("\r\n  %2d %3s %c %10s [%-8s]",++j,nmenu->choice,nmenu->form,nmenu->list,nmenu->ccode);
            printf(" %5d %5d : %5x : %5d",nmenu->w,nmenu->r,nmenu->del,nmenu->level            );
            if(nmenu->form == 'f' || nmenu->form == 'F') printf(" %s",nmenu->file);
            printf("\r\n\n\r��ȣ ����[P] ����[E] �߰�[A] ����[I] ����[D] ����[S] ����[X] >> ");
        }
        else {
            i = strlen(buf);
            for(j = 0; j < i; j++) {
                putchar('\b'); putchar(' '); putchar('\b');
            }
        }
        key = 1;
        lineinput(buf,10,YEA);
        i = atoi(buf);
        if(i) {
            nmenu = mtop->downp;
            for(j = 1; j < i; j++) {
                if(nmenu->neqp != NULL) nmenu = nmenu->neqp;
                else break;
            }
            if(nmenu->form == 'M') {
                mtop = nmenu;
            }
            else key = 0;
        }
        else if(buf[0] == 'p' || buf[0] == 'P') {
            mtop = mtop->upp;
        }
        else if(buf[0] == 'e' || buf[0] == 'E') {
            buf[0] = ' ';
            i = atoi(buf);
            if(i) {
                nmenu = mtop->downp;
                for(j = 1; j < i; j++) {
                    if(nmenu->neqp != NULL) {
                        nmenu = nmenu->neqp;
                    }
                    else {
                        break;
                    }
                }
            }
            else {
                nmenu = mtop;
            }
            printf("\r\n\n%<%s> [%s]",nmenu->list,nmenu->ccode);
            Edit_Line();
        }
        else if(buf[0] == 'a' || buf[0] == 'A') {
            nmenu = mtop->downp;
            while(nmenu->neqp != NULL) nmenu = nmenu->neqp;
            Append_Line();
        }
        else if(buf[0] == 'i' || buf[0] == 'I') {
            buf[0] = ' ';
            i = atoi(buf);
            if(i == 1) {
                if((mtmp = (struct menustruct *)malloc(sizeof(struct menustruct))) == NULL) {
                    printf("\r\n�޸𸮰� ���ڶ��ϴ�.");
                    exit(1);
                }
                mtmp->neqp = mtop->downp;
                mtmp->downp = NULL;
                mtop->downp = mtmp;
                mtmp->upp = mtop;
                nmenu = mtmp;
                Edit_Line();
            }
            else if(i) {
                nmenu = mtop->downp;
                for(j = 2; j < i; j++) {
                    if(nmenu->neqp != NULL) {
                        nmenu = nmenu->neqp;
                    }
                    else {
                        break;
                    }
                }
                if((mtmp = (struct menustruct *)malloc(sizeof(struct menustruct))) == NULL) {
                    printf("\r\n�޸𸮰� ���ڶ��ϴ�.");
                    exit(1);
                }
                mtmp->downp = NULL;
                mtmp->neqp = nmenu->neqp;
                mtmp->upp = nmenu->upp;
                nmenu->neqp = mtmp;
                nmenu = mtmp;
                Edit_Line();
            }
        }
        else if(buf[0] == 'd' || buf[0] == 'D') {  /* �޴� ���� */
            buf[0] = ' ';
            i = atoi(buf);
            if(i == 1) {
                nmenu = mtop->downp;
                mtop->downp = nmenu->neqp;
                free(nmenu);
                if(mtop->downp == NULL) mtop = mtop->upp;
            }
            else if(i > 1) {
                nmenu = mtop->downp;
                for(j = 1; j < i; j++) {
                    if(nmenu->neqp != NULL) {
                        mtmp = nmenu;
                        nmenu = nmenu->neqp;
                    }
                    else {
                        break;
                    }
                }
                mtmp->neqp = nmenu->neqp;
                free(nmenu);
            }
        }
        else if(buf[0] == 's' || buf[0] == 'S') {
            Save_Menu();
        }
        else if(buf[0] == 'x' || buf[0] == 'X') break;
        else key = 0;
    }
}

Edit_Line()
{
    char buf[35];
    printf("\r\n ���ù�ȣ : ");
    keyinput(buf,5);
    if(buf[0]!='\0') {
	sprintf(nmenu->choice,"%s",buf); 
    }
    printf("\r\n\r���� : ");
    keyinput(buf,1);
    if(buf[0]!='\0') {
        nmenu->form = buf[0];
    }
    printf("\r\n\r�����ڵ� : ");
    keyinput(buf,9);
    if(buf[0]!='\0') {
        sprintf(nmenu->ccode,"%s",buf);
    }
    printf("\r\n\r�޴��� : ");
    keyinput(buf,39);
    if(buf[0]!='\0') {
        sprintf(nmenu->list,"%s",buf);
    }

    if((nmenu->form == 'M') && (nmenu->downp == NULL)) {
        Sub_Creat();
    }
    else {
        printf("\r\n���ⷹ�� : ");
        keyinput(buf,8);
        nmenu->w = atoi(buf);
        printf("\r\n�бⷹ�� : ");
        keyinput(buf,8);
        nmenu->r = atoi(buf);
        if(nmenu->form == 'L' || nmenu->form == 'l' ||
          nmenu->form == 'V' || nmenu->form == 'v') {
            printf("\r\n����/�Խù� : ");
            keyinput(buf,2);
            nmenu->level = atoi(buf);
        }
        else if(nmenu->form == 'F' || nmenu->form == 'f') {
            printf("\r\n����/ȭ�� : ");
            keyinput(buf,8);
            nmenu->level = atoi(buf);
            if(nmenu->level == 0) {
                nmenu->level = 0;
            }
            printf("\r\n�������� �÷��� : ");
            keyinput(buf,3);
            nmenu->del = atoi(buf);
            printf("\r\n ȭ����ġ���丮(�������� ������[files]��): ");
            lineinput(buf,29,YEA);
            if(buf[0]=='\0') strcpy(nmenu->file,"files");
	    else strcpy(nmenu->file,buf);  
        }
    }
}
Append_Line()
{
    if((mtmp = (struct menustruct *) malloc(sizeof(struct menustruct))) == NULL) {
        printf("\r\n�޸𸮰� ���ڶ��ϴ�.");
        exit(1);
    }
    nmenu->neqp = mtmp;
    mtmp->upp = nmenu->upp;
    nmenu = mtmp;
    nmenu->downp = nmenu->neqp = NULL;
    Edit_Line();
}
Sub_Creat()
{
    if((mtmp = (struct menustruct *) malloc(sizeof(struct menustruct))) == NULL) {
        printf("\r\n�޸𸮰� ���ڶ��ϴ�.");
        exit(1);
    }
    nmenu->downp = mtmp;
    mtmp->upp = nmenu;
    nmenu = mtmp;
    nmenu->downp = nmenu->neqp = NULL;
    Edit_Line();
}

Save_Menu()
{
    int i = 0;
    FILE *fp1;
    char buf[80],buf2[80],buf3[80];
    sprintf(buf,"bin/BBSMENUS");
    sprintf(buf2,"bin/BBSMENUS.backup");
    sprintf(buf3,"mv %s %s",buf,buf2);
    system(buf3);

    mtop = nmenu = menu;
    if((fp1 = fopen("bin/BBSMENUS","w")) != NULL) {
        mtop->deep = 0;
        fwrite((char*)nmenu,sizeof(struct menustruct),1,fp1);
        printf("\r\n%8s %s",nmenu->ccode,nmenu->list);
        nmenu = nmenu->downp;
        ++i;
        while(nmenu != mtop) {
            nmenu->deep = i;
            fwrite((char*)nmenu,sizeof(struct menustruct),1,fp1);
            printf("\r\n%8s %s",nmenu->ccode,nmenu->list);
            if(nmenu->downp == NULL) {
                if(nmenu->neqp == NULL) {
                    while(1) {
                        nmenu = nmenu->upp;
                        --i;
                        if(nmenu->neqp != NULL || i == 0) break;
                    }
                    if(i) nmenu = nmenu->neqp;
                }
                else {
                    nmenu = nmenu->neqp;
                }
            }
            else {
                nmenu = nmenu->downp;
                ++i;
            }

        }
        fclose(fp1);
    }
    else printf("\r\nȭ���� ������ �ʽ��ϴ�.");
}

Get_Menu()
{
    int i, j;
    char ch[2];
    FILE *fp1,*fp2;
    char buf[5];

    if((fp1 = fopen("bin/BBSMENUS","r")) == NULL) {
        printf("\r\n�޴�ȭ���� ������ �ʽ��ϴ�.");
	if((fp2=fopen("bin/MENUS","r"))!=NULL) {
	    printf("\r\n ������ �޴�ȭ���� �ֽ��ϴ�.�������ϰڽ��ϱ�?(y/N)");
	    keyinput(buf,1);
	    if(buf[0]=='y'||buf[0]=='Y') {
		convertoldmenu();
	    }
	}
        else {
	    printf("\r\n�޴�ȭ���� ����ðڽ��ϱ�?(y/N)>> ");
            lineinput(buf,1,YEA);
            if(buf[0]=='y' || buf[0]=='Y') Make_Menu(1);
	}
        host_end();
    }
    else {
        if((mtmp = (struct menustruct *)malloc(sizeof(struct menustruct))) == NULL) {
            printf("\r\n�޸� ����");
            exit(1);
        }
        menu = mtop = nmenu = mtmp;
        fread((char*)mtmp,sizeof(struct menustruct),1,fp1);
        if((mtmp = (struct menustruct *)malloc(sizeof(struct menustruct))) == NULL) {
            printf("\r\n�޸𸮰� ����");
            exit(1);
        }
        nmenu->neqp = nmenu->aeqp = NULL;
        nmenu->upp = nmenu;
        nmenu->downp = mtmp;
        while(fread((char*)mtmp,sizeof(struct menustruct),1,fp1)) {
            if(nmenu->deep < mtmp->deep) {
                nmenu->downp = mtmp;
                mtmp->upp = nmenu;
                nmenu->neqp = mtmp->aeqp = NULL;
            }
            else if(nmenu->deep == mtmp->deep) {
                nmenu->downp = NULL;
                nmenu->neqp = mtmp;
                mtmp->upp = nmenu->upp;
                mtmp->aeqp = nmenu;
            }
            else if(nmenu->deep > mtmp->deep) {
                nmenu->downp = NULL;
                nmenu->neqp = NULL;
                j = nmenu->deep - mtmp->deep;
                for(i = 0; i < j; i++) {
                    nmenu = nmenu->upp;
                }
                nmenu->neqp = mtmp;
                mtmp->upp = nmenu->upp;
                mtmp->neqp = mtmp->downp = NULL;
                mtmp->aeqp = nmenu;
            }
            nmenu = mtmp;
            if((mtmp=(struct menustruct *)malloc(sizeof(struct menustruct))) == NULL) {
                printf("\r\n�޸� ����");
                exit(1);
            }

        }
        fclose(fp1);
        free(mtmp);
    }
}
Menu_Free()
{
    nmenu = menu->downp;
    while(nmenu != menu) {
        if(nmenu->neqp != NULL) {
            nmenu = nmenu->neqp;
        }
        else {
            if(nmenu->downp != NULL) {
                nmenu = nmenu->downp;
            }
            else {
                mtmp = nmenu;
                nmenu = nmenu->upp;
                if(nmenu->downp == mtmp) {
                    free(mtmp);
                    nmenu->downp = NULL;
                }
                else {
                    nmenu = nmenu->downp;
                    while(nmenu->neqp != mtmp) {
                        nmenu = nmenu->neqp;
                    }

                    free(mtmp);
                    nmenu->neqp = NULL;
                }
            }
        }
    }
    free(menu);
}

