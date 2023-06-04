#include "bbs.h"

/* 이곳은 하이텔 에뮬에 들어가는 통로입니다. */
hitelemul()
{

    cls();
    printf("\r\n 이곳은 하이텔 에뮬레이터로 들어가는 통로입니다.");
    printf("\r\n login에서 hitel을 쳐 주세요.");
    pressreturn();
    system("telnet -8 doltank");
}

