#include <iostream>
#include <cstdlib>
#include <memory>

#include "paladin.hpp"
#include "sorcerer.hpp"

using namespace std;

int main()
{
    //1) 성기사 생성 패턴 - Builder 패턴 사용
    Paladin hammerdin = Paladin::Builder("해머딘")
        .setLeftWeapon("스피릿 크리스탈 소드")   // 왼손: 스피릿
        .setRightWeapon("스피릿 성기사 방패")     // 오른손: 스피릿 방패
        .setLeftRing("패캐 링")                  // 왼손 반지
        .setRightRing("레번 프로스트")           // 오른손 반지
        .setArmor("수수께끼 아머")               // 갑옷
        .setAmulet("마라의 만화경")              // 목걸이
        .build();

    hammerdin.print();
    cout<<endl;

    Sorcerer blizzSorcerer = Sorcerer::Builder("블리츠 소서리스")
        .setLeftWeapon("에테리얼 오브")          // 왼손: 오브
        .setRightWeapon("에테리얼 스피릿 타워 쉴드") // 오른손: 타워 쉴드
        .setLeftRing("매직 파인드 링")          // 왼손 반지
        .setRightRing("매직 파인드 링")         // 오른손 반지
        .setArmor("수수께끼 로브")               // 갑옷
        .setAmulet("마라의 만화경")              // 목걸이
        .build();

    blizzSorcerer.print();

    //default 소서러
    Sorcerer ds = Sorcerer::Builder("원소술사")
    .setLeftWeapon("기본 지팡이")
    .build();
             
    cout << endl;
    ds.print();

    return 0;
}
