#include "Hero.hpp"

int main() {
    HeroCreationDirector director;

    PaladinBuilder   paladinBuilder;
    SorceressBuilder sorcBuilder;
    BarbarianBuilder barbBuilder;

    auto hammerdin = director.createHammerdin(paladinBuilder);
    auto blizzSorc = director.createBlizzardSorceress(sorcBuilder);
    auto wwBarb    = director.createWhirlwindBarbarian(barbBuilder);

    hammerdin->print();
    blizzSorc->print();
    wwBarb->print();

    return 0;
}
