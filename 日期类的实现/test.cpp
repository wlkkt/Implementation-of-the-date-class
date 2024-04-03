#include "Date.h"

int main()
{
	Date d1(2024, 2, 30);
	Date d2(2024, 8, 1);
	cout << d1 - d2 << endl;
	Date d3;
	operator<<(cout, d1);
	cout << d1;
	cout << d1 << d2;//<<运算符是左结合，所以先进行的是cout<<d1，然后cout<<d1结束后的返回值作为下一次流插入的左操作数“返回值 << d2”
	cin >> d3;
	cout << d3;

	return 0;
}