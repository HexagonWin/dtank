/* 메뉴 구조를 위한 함수이다 */
/* 아주 중요한 특징의 함수들이 모여있다 */
/* 1.3.0으로 업버젼(돌탱크)할 때 필요한 구조의 함수들이다 */
#include "bbs.h"

Make_Menu(int start)
{
    int i, j = 0, key = 1;
    char ch, buf[80];
    if(start) { 
        if((mtmp=(struct menustruct *)malloc(sizeof(struct menustruct))) ==NULL) {
            printf("\r\n메모리가 부족하네요.");
            exit(1);
        }
        menu=mtop=nmenu=mtmp;
        if((mtmp=(struct menustruct *)malloc(sizeof(struct menustruct))) ==NULL) {
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
	    cls();
            printf("\r\n  %s      [%s]\r\n",mtop->list,mtop->ccode);
            nmenu = mtop->downp;
            j = 0;
	    printf("\r\nnum 선택 F 타이틀      가기코드   읽기  쓰기    삭제    증가");
            while(nmenu->neqp != NULL) {
                printf("\r\n  %2d %3s %c %10s [%-8s]",++j,nmenu->choice,nmenu->form,nmenu->list,nmenu->ccode);
                printf(" %5d %5d : %5x : %5d",nmenu->w,nmenu->r,nmenu->del,nmenu->level);
                if(nmenu->form == 'f' || nmenu->form == 'F') printf(" %s",nmenu->file);
                nmenu = nmenu->neqp;
            }
            printf("\r\n  %2d %3s %c %10s [%-8s]",++j,nmenu->choice,nmenu->form,nmenu->list,nmenu->ccode);
            printf(" %5d %5d : %5x : %5d",nmenu->w,nmenu->r,nmenu->del,nmenu->level            );
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
                    printf("\r\n메모리가 모자랍니다.");
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
                    printf("\r\n메모리가 모자랍니다.");
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
            Save_Menu();
        }
        else if(buf[0] == 'x' || buf[0] == 'X') break;
        else key = 0;
    }
}

Edit_Line()
{
    char buf[35];
    printf("\r\n 선택번호 : ");
    keyinput(buf,5);
    if(buf[0]!='\0') {
	sprintf(nmenu->choice,"%s",buf); 
    }
    printf("\r\n\r형태 : ");
    keyinput(buf,1);
    if(buf[0]!='\0') {
        nmenu->form = buf[0];
    }
    printf("\r\n\r가기코드 : ");
    keyinput(buf,9);
    if(buf[0]!='\0') {
        sprintf(nmenu->ccode,"%s",buf);
    }
    printf("\r\n\r메뉴명 : ");
    keyinput(buf,39);
    if(buf[0]!='\0') {
        sprintf(nmenu->list,"%s",buf);
    }

    if((nmenu->form == 'M') && (nmenu->downp == NULL)) {
        Sub_Creat();
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
            printf("\r\n 화일위치디렉토리(지정하지 않으면[files]임): ");
            lineinput(buf,29,YEA);
            if(buf[0]=='\0') strcpy(nmenu->file,"files");
	    else strcpy(nmenu->file,buf);  
        }
    }
}
Append_Line()
{
    if((mtmp = (struct menustruct *) malloc(sizeof(struct menustruct))) == NULL) {
        printf("\r\n메모리가 모자랍니다.");
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
        printf("\r\n메모리가 모자랍니다.");
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
    else printf("\r\n화일이 열리지 않습니다.");
}

Get_Menu()
{
    int i, j;
    char ch[2];
    FILE *fp1,*fp2;
    char buf[5];

    if((fp1 = fopen("bin/BBSMENUS","r")) == NULL) {
        printf("\r\n메뉴화일이 열리지 않습니다.");
	if((fp2=fopen("bin/MENUS","r"))!=NULL) {
	    printf("\r\n 구버젼 메뉴화일이 있습니다.컨버팅하겠습니까?(y/N)");
	    keyinput(buf,1);
	    if(buf[0]=='y'||buf[0]=='Y') {
		convertoldmenu();
	    }
	}
        else {
	    printf("\r\n메뉴화일을 만드시겠습니까?(y/N)>> ");
            lineinput(buf,1,YEA);
            if(buf[0]=='y' || buf[0]=='Y') Make_Menu(1);
	}
        host_end();
    }
    else {
        if((mtmp = (struct menustruct *)malloc(sizeof(struct menustruct))) == NULL) {
            printf("\r\n메모리 부족");
            exit(1);
        }
        menu = mtop = nmenu = mtmp;
        fread((char*)mtmp,sizeof(struct menustruct),1,fp1);
        if((mtmp = (struct menustruct *)malloc(sizeof(struct menustruct))) == NULL) {
            printf("\r\n메모리가 부족");
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
                printf("\r\n메모리 부족");
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

