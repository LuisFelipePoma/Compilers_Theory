#include <iostream>

using namespace std;

int main()
{
	int f = 1;
	for (int i = 2; i <= 5; i++)
	{
		f *= i;
		cout << f << endl;
	}
	return 0;
}