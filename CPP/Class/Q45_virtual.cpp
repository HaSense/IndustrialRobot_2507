#include <iostream>
#include <memory>
#include <vector>
using namespace std;

class Hero{
public:
    virtual void attack(){
        cout << "Hero Attack!" << endl;
    }
};
class Knight : public Hero{
public:
    void attack() override {
        cout << "Knight Attack!" << endl;
    }
};
class Archer : public Hero{
public:
    void attack() override {
        cout << "Archer Attack!" << endl;
    }
};
class Wizard : public Hero{
public: 
    void attack() override {
        cout << "Wizard Attack!" << endl;
    }
};  

int main()
{
    vector<Hero*> list;
    unique_ptr<Hero> k = make_unique<Knight>();
    unique_ptr<Hero> a = make_unique<Archer>();
    unique_ptr<Hero> w = make_unique<Wizard>();
    
    list.push_back(k.get());
    list.push_back(a.get());
    list.push_back(w.get());
    
    for(auto hero : list){
        hero->attack();
    }
    return 0;
}
