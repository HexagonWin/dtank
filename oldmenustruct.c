
#include "bbs.h"

make_menu(int start)
{
    int i, j = 0, key = 1;
    char ch, buf[80];
    if(start) { 
	if((mtmp=(struct config *)malloc(sizeof(struct config))) ==NULL) {
	    printf("\r\n메모리가 부족하네요.");
	    exit(1);
	}
	menu=mtop=nmenu=mtmp;
        if((mtmp=(struct config *)malloc(sizeof(struct config))) ==NULL) {
            printf("\r\n메모리가 부족하네요.");
            exit(1);
        }
	nmenu->neqp=nmenu->aeqp=NULL;
	nmenu->upp=nmenu;
	nmenu->downp=mtmp;
	nmenu->form='M'; nmenu->level=nmenu->del=nmenu->w=nmenu->r = nmenu->deep=0;
	strcpy(nmenu->ccode,"top");
	strcpy(nmenu->list,"돌탱크");
	strcpy(nmenu->file,"");
        mtmp->neqp=mtmp->aeqp=mtmp->downp=NULL;
	mtmp->upp=mtop;
    }
    while(1) {
        if(key) {
            printf("\r\n  %s      [%s]\r\n",mtop->list,mtop->ccode);
            nmenu = mtop->downp;
            j = 0;
            while(nmenu->neqp != NULL) {
                printf("\r\n  %2d %c %s [%-8s]",++j,nmenu->form,nmenu->list,nmenu->ccode);
                printf(" %5d %5d : %x : %d",nmenu->w,nmenu->r,nmenu->del,nmenu->level);
                if(nmenu->form == 'f' || nmenu->form == 'F') printf(" %s",nmenu->file);
                nmenu = nmenu->neqp;
            }
            printf("\r\n  %2d %c %s [%-8s]",++j,nmenu->form,nmenu->list,nmenu->ccode);
            printf(" %5d %5d : %x : %d",nmenu->w,nmenu->r,nmenu->del,nmenu->level            );
            if(nmenu->form == 'f' || nmenu->form == 'F') printf(" %s",nmenu->file);
            printf("\r\n\n\r번호 상위[P] 수정[E] 추가[A] 삽입[I] 삭제[D] 저장[S] 종료[X] >> ");
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
            edit_line();
        }
        else if(buf[0] == 'a' || buf[0] == 'A') {
            nmenu = mtop->downp;
            while(nmenu->neqp != NULL) nmenu = nmenu->neqp;
            append_line();
        }
        else if(buf[0] == 'i' || buf[0] == 'I') {
            buf[0] = ' ';
            i = atoi(buf);
            if(i == 1) {
                if((mtmp = (struct config *)malloc(sizeof(struct config))) == NULL) {
                    printf("\r\n메모리가 모자랍니다.");
                    exit(1);
                }
                mtmp->neqp = mtop->downp;
                mtmp->downp = NULL;
                mtop->downp = mtmp;
                mtmp->upp = mtop;
                nmenu = mtmp;
                edit_line();
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
                if((mtmp = (struct config *)malloc(sizeof(struct config))) == NULL) {
                    printf("\r\n메모리가 모자랍니다.");
                    exit(1);
                }
                mtmp->downp = NULL;
                mtmp->neqp = nmenu->neqp;
                mtmp->upp = nmenu->upp;
                nmenu->neqp = mtmp;
                nmenu = mtmp;
                edit_line();
            }
        }
        else if(buf[0] == 'd' || buf[0] == 'D') {  /* 메뉴 삭제 */
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
            save_menu();
        }
        else if(buf[0] == 'x' || buf[0] == 'X') break;
        else key = 0;
    }
}



edit_line()
{
    char buf[35];
    printf("\r\n\r형태 : ");
    keyinput(buf,1);
    nmenu->form = buf[0];
    printf("\r\n\r가기코드 : ");
    keyinput(buf,9);
    sprintf(nmenu->ccode,"%s",buf);
    printf("\r\n\r메뉴명 : ");
    keyinput(buf,39);
    sprintf(nmenu->list,"%s",buf);
    if((nmenu->form == 'M') && (nmenu->downp == NULL)) {
        sub_creat();
    }
    else {
        printf("\r\n쓰기레벨 : ");
        keyinput(buf,8);
        nmenu->w = atoi(buf);
        printf("\r\n읽기레벨 : ");
        keyinput(buf,8);
        nmenu->r = atoi(buf);
        if(nmenu->form == 'L' || nmenu->form == 'l' ||
          nmenu->form == 'V' || nmenu->form == 'v') {
            printf("\r\n레벨/게시물 : ");
            keyinput(buf,2);
            nmenu->level = atoi(buf);
        }
        else if(nmenu->form == 'F' || nmenu->form == 'f') {
            printf("\r\n레벨/화일 : ");
            keyinput(buf,8);
            nmenu->level = atoi(buf);
            if(nmenu->level == 0) {
                nmenu->level = 0;
            }
            printf("\r\n삭제권한 플래그 : ");
            keyinput(buf,3);
            nmenu->del = atoi(buf);
	    printf("\r\n 화일위치디렉토리: ");
	    lineinput(buf,29,YEA);
	    strcpy(nmenu->file,buf);
        }
    }
}

void append_line()
{
    if((mtmp = (struct config *) malloc(sizeof(struct config))) == NULL) {
        printf("\r\n메모리가 모자랍니다.");
        exit(1);
    }
    nmenu->neqp = mtmp;
    mtmp->upp = nmenu->upp;
    nmenu = mtmp;
    nmenu->downp = nmenu->neqp = NULL;
    edit_line();
}

sub_creat()
{
    if((mtmp = (struct config *) malloc(sizeof(struct config))) == NULL) {
        printf("\r\n메모리가 모자랍니다.");
        exit(1);
    }
    nmenu->downp = mtmp;
    mtmp->upp = nmenu;
    nmenu = mtmp;
    nmenu->downp = nmenu->neqp = NULL;
    edit_line();
}


save_menu()
{
    int i = 0;
    FILE *fp1;
    char buf[80],buf2[80],buf3[80];
    sprintf(buf,"bin/MENUS");
    sprintf(buf2,"bin/MENUS.backup");
    sprintf(buf3,"mv %s %s",buf,buf2);
    system(buf3);

    mtop = nmenu = menu;
    if((fp1 = fopen("bin/MENUS","w")) != NULL) {
        mtop->deep = 0;
        fwrite((char*)nmenu,sizeof(struct config),1,fp1);
        printf("\r\n%8s %s",nmenu->ccode,nmenu->list);
        nmenu = nmenu->downp;
        ++i;
        while(nmenu != mtop) {
            nmenu->deep = i;
            fwrite((char*)nmenu,sizeof(struct config),1,fp1);
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
    else printf("\r\n화일이 열리지 않습니다.");
}


menu_free()
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
get_menu()
{
    int i, j;
    char ch[2];
    FILE *fp1;
    char buf[5];

    if((fp1 = fopen("bin/MENUS","r")) == NULL) {
        printf("\r\n메뉴화일이 열리지 않습니다.");
        printf("\r\n메뉴화일을 만드시겠습니까?(y/N)>> ");
        lineinput(buf,1,YEA);
        if(buf[0]=='y' || buf[0]=='Y') make_menu(1);
        host_end();
    }
    else {
        if((mtmp = (struct config *)malloc(sizeof(struct config))) == NULL) {
            printf("\r\n메모리 부족");
            exit(1);
        }
        menu = mtop = nmenu = mtmp;
        fread((char*)mtmp,sizeof(struct config),1,fp1);
        if((mtmp = (struct config *)malloc(sizeof(struct config))) == NULL) {
            printf("\r\n메모리가 부족");
            exit(1);
        }
        nmenu->neqp = nmenu->aeqp = NULL;
        nmenu->upp = nmenu;
        nmenu->downp = mtmp;
        while(fread((char*)mtmp,sizeof(struct config),1,fp1)) {
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
            if((mtmp=(struct config *)malloc(sizeof(struct config))) == NULL) {
                printf("\r\n메모리 부족");
                exit(1);
            }

        }
        fclose(fp1);
        free(mtmp);
    }
}

