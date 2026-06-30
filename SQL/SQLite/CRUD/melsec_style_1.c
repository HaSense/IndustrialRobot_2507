#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

/*
    MELSEC 입력 예시
    X0 : 시작 버튼
    X1 : 정지 버튼
    X2 : 제품 감지 센서
    X3 : 불량 감지 센서

    MELSEC 출력 예시
    Y0 : 컨베이어 모터
    Y1 : 배출 실린더
    Y2 : 정상 램프
    Y3 : 불량 램프

    내부 릴레이 예시
    M0 : 운전 상태
    M1 : 제품 감지 상태
    M2 : 불량 처리 상태
*/

typedef struct
{
    bool X0;
    bool X1;
    bool X2;
    bool X3;

    bool Y0;
    bool Y1;
    bool Y2;
    bool Y3;

    bool M0;
    bool M1;
    bool M2;

    int timer;
} PLC;

void scanPLC(PLC *plc)
{
    // 자기유지 회로
    // X0가 ON이면 M0 운전 시작
    // X1이 ON이면 M0 운전 정지
    if (plc->X0)
    {
        plc->M0 = true;
    }

    if (plc->X1)
    {
        plc->M0 = false;
    }

    // 운전 중이면 컨베이어 ON
    if (plc->M0)
    {
        plc->Y0 = true;
    }
    else
    {
        plc->Y0 = false;
    }

    // 제품 감지
    if (plc->X2 && plc->M0)
    {
        plc->M1 = true;
    }

    // 불량 감지
    if (plc->M1 && plc->X3)
    {
        plc->M2 = true;
        plc->timer = 3;
    }

    // 불량 배출 처리
    if (plc->M2)
    {
        plc->Y1 = true;
        plc->Y3 = true;
        plc->Y2 = false;

        plc->timer--;

        if (plc->timer <= 0)
        {
            plc->M2 = false;
            plc->M1 = false;
            plc->Y1 = false;
            plc->Y3 = false;
        }
    }
    else
    {
        // 불량이 아니면 정상 램프
        if (plc->M1 && !plc->X3)
        {
            plc->Y2 = true;
            plc->M1 = false;
        }
    }
}

void printPLC(PLC plc)
{
    printf("M0 운전상태     : %d\n", plc.M0);
    printf("Y0 컨베이어     : %d\n", plc.Y0);
    printf("Y1 배출실린더   : %d\n", plc.Y1);
    printf("Y2 정상램프     : %d\n", plc.Y2);
    printf("Y3 불량램프     : %d\n", plc.Y3);
    printf("-------------------------\n");
}

int main()
{
    PLC plc = {0};

    // 시작 버튼 입력
    plc.X0 = true;
    scanPLC(&plc);
    plc.X0 = false;

    printPLC(plc);

    // 제품 감지
    plc.X2 = true;
    scanPLC(&plc);
    plc.X2 = false;

    printPLC(plc);

    // 불량 감지
    plc.X3 = true;

    for (int i = 0; i < 5; i++)
    {
        scanPLC(&plc);
        printPLC(plc);
        sleep(1);
    }

    return 0;
}
