#include "Date.h"

int main()
{
	Date d1(2024, 2, 30);
	Date d2(2024, 8, 1);
	cout << d1 - d2 << endl;
	Date d3;
	operator<<(cout, d1);
	cout << d1;
	cout << d1 << d2;//<<����������ϣ������Ƚ��е���cout<<d1��Ȼ��cout<<d1������ķ���ֵ��Ϊ��һ����������������������ֵ << d2��
	cin >> d3;
	cout << d3;

	return 0;
}