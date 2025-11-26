#include <iostream>
#include <string>
using namespace std;

class Triangle {
private:
    int width;
    int height;
public:
    double getArea(string width, string heidht){
        this->width = stoi(width);
        this->height = stoi(heidht);
        return this->width * this->height / 2.0;
    }
};

int main()
{
    string width, height;
    getline(cin, width);
    getline(cin, height);

    Triangle tri;
    cout << "삼각형의 넓이는 : " << tri.getArea(width, height) << endl;
    
    return 0;
}
