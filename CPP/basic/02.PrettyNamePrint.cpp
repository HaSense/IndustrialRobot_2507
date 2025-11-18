#include <iostream>
#include <string>

int main()
{
	std::cout << "첫번째 이름을 입력해 주세요 : ";
	std::string name;
	std::cin >> name;

	
	//출력하려는 메시지를 구성
	std::string const greeting = "Hello, " + name + "!";

	//인사말의 두 번째 행과 네 번째 행
	std::string const spaces(greeting.size(), ' ');
	std::string const second = "* " + spaces + " *"; //concat

	//인사말의 첫 번째 행과 다섯 번째 행
	std::string const first(second.size(), '*');

	std::cout << std::endl;
	std::cout << first << std::endl;
	std::cout << second << std::endl;
	std::cout << "* " << greeting << " *" << std::endl;
	std::cout << second << std::endl;
	std::cout << first << std::endl;
}
