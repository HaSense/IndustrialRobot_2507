#include <iostream>

int main()
{
	//1.변수 선언 및 입력
	int score, div;
	std::cout << "점수를 입력하세요 : ";
	std::cin >> score;

	//1.5 입력 예외처리
	if ( (score > 100) || (score < 0) ) {
		std::cout << "잘못된 점수 입니다.";
		return 0;
	}
	//2.로직 , 출력
	div = score / 10;
	switch (div) {
	case 10:
	case 9:
		std::cout << "A" << std::endl;
		break;
	case 8:
		std::cout << "B" << std::endl;
		break;
	case 7:
		std::cout << "C" << std::endl;
		break;
	case 6:
		std::cout << "D" << std::endl;
		break;
	default:
		std::cout << "F" << std::endl;
		break;
	}

}

