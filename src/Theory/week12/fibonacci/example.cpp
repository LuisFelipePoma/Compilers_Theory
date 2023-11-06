#include <string>
#include <memory>
#include <iostream>

class A
{
	int x;

public:
	A(int x) : x(x) {}
	int &gitX1() { return x; }
	const int &gitX2() const { return x; }
};

int main()
{
	std::unique_ptr<A> obj = std::unique_ptr<A>(new A(29));
	int &y = obj->gitX1();
	y = 30;
	std::cout << y << '\n';
	std::cout << obj->gitX1() << '\n';

	// const int &z = obj->gitX2();
	// z = 10;
	// std::cout << obj->gitX1() << '\n';
	return 0;
}