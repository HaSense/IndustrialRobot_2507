#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>

using namespace std;

int main()
{
    vector<int> numbers;

    numbers.push_back(30);
    numbers.push_back(20);
    numbers.push_back(70);
    numbers.push_back(40);
    numbers.push_back(50);
    numbers.push_back(10);

   /* for (int i = 0; i < numbers.size(); i++) {
        cout << numbers[i] << endl;
    }*/
    
    sort(numbers.begin(), numbers.end());

    for (int i : numbers) {
        cout << i << endl;
    }

    return 0;
}
