#include <iostream>
#include <cstdlib>

using namespace std;

class IAttackStrategy
{
public:
	virtual void attack() = 0;
	virtual ~IAttackStrategy() = default;
};

class FireAttack : public IAttackStrategy
{
public:
	void attack() override
	{
		cout << "화공으로 공격합니다." << endl;
	}
};

class WaterAttack : public IAttackStrategy
{
public:
	void attack() override
	{
		cout << "수공으로 공격합니다." << endl;
	}
};

class PhysicalAttack : public IAttackStrategy
{
public:
	void attack() override
	{
		cout << "물리력으로 공격합니다." << endl;
	}
};

class Character
{
private:
	string name;
	IAttackStrategy* strategy = nullptr; // 소유 X
public:
	Character(string name) : name(name){}
	//참조로 받아 내부에 포인터로 저장
	void setStrategy(IAttackStrategy& s) 
	{ 
		strategy = &s;
	}
	
	void Attack() {
		if (!strategy)
		{ 
			cout << "(경고) 전략 없음\n";  
		}
		return strategy->attack();
	}
};

int main()
{
	Character hero("Lee Sun Sin");

	FireAttack fire;
	WaterAttack water;
	PhysicalAttack phys;

	hero.setStrategy(fire);
	hero.Attack();
	hero.setStrategy(water);
	hero.Attack();
	hero.setStrategy(phys);
	hero.Attack();
   
}
