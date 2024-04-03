#pragma once 
#include <iostream>
#include <assert.h>
using namespace std;

//声明日期类Date
class Date
{

public:
	//全缺省构造函数，为程序提供默认的缺省值
	Date(int year = 1, int month = 1, int day = 1);

	//运算符重载函数，更好的反应出函数的所提供的服务
	//日期大小的逻辑判断
	bool operator>(const Date& d);//返回值为bool型，因为我们只需要知道到底是大于还是小于即可
	bool operator<(const Date& d);//采用传引用传参,传入的是右操作数的别名，且加了const修饰后该别名只有读权限，对别名的修改不会影响全局
	bool operator==(const Date& d);
	bool operator>=(const Date& d);
	bool operator<=(const Date& d);
	bool operator!=(const Date& d);


	//日期增加和减少的运算
	Date& operator+=(int d);//返回值类型是Date和Date&均可但是是Date类型时会发生数据拷贝，造成资源浪费
	Date operator+(int d);//+=采用Date&做返回值类型是为了效率，+采用Date做返回值类型是因为防止野引用
	Date operator-(int d);
	Date& operator-=(int d);

	//日期++和--的运算（需要做特殊处理，区分前置和后置++、--）,特殊处理都是为了解决语法逻辑不自洽，Date&是因为前置++在函数中进行+1后直接返回即可（相当于走个过场）用引用做返回值类型可以减少拷贝，Date是因为后置++返回的是原对象的拷贝虽然原对象也进行了++操作但是不能让它作为返回值，如果也是Date&由于tmp在函数结束时就会销毁会造成野引用的情况
	Date& operator++();//++d1
	Date operator++(int);//d1++，为了跟前置++区分，需要强行增加一个int形参，从而构成operator++()和operator++(int)之间的函数重载
	Date& operator--();//--d1
	Date operator--(int);//d1--

	//日期减日期（运算符重载函数同时也可以构成函数重载，这里的int operator-和Date operator-构成函数重载）
	int operator-(const Date& d);//返回的是两个日期之间的差值，同时不能修改传入的日期


	void Print()
	{
		cout << _year << "-" << _month << "-" << _day << endl;
	}

	//根据月份获取本月天数(本质是inline)
	//即使没有显式使用 inline 关键字，类中定义的函数也可能被视为内联函数。
	//在C++中，将函数定义放在类声明内部（通常是在类定义头文件中）的成员函数会被隐式视为内联函数。
	//当你把一个成员函数的定义直接放在类声明里时，编译器会自动将其视为内联函数。
	// 这种情况下，编译器会尝试优化这些成员方法以减少调用开销，并且不需要额外的 inline 关键字来指示。
	int GetMonthDay(int year, int month)	//省去了if...else的判断，直接将月份写在数组中，同时为了能够使输入的月份能与数组中该月份的天数对应，创建了一个13个元素大小的数组，数组首元素为0，当传入月份为1时monthDays[1]刚好是31天
	{
		assert(month > 0 && month < 13);//月份的大小要在合理的范围内
		static int monthDays[13] = { 0,31,28,31,30,31,30,31,31,30,31,30,31 };//该函数会被频繁调用，所以为了避免每次调用函数的时候都会创建数组，应该在静态区开辟数组（生命周期延长至全局）
		if (month == 2 && ((year % 4 == 0) && year % 100 != 0) || (year % 400 == 0))//只有month==2时才需要进行模运算，逻辑短路
		{
			return 29;
		}
		return monthDays[month];//返回当前月对应的天数
	}

	//检查流提取时的无效日期：流提取时输入2024 2 30可是二月根本没有三十天,但是如果没有检查函数的话编译器不认为这是错的，所以需要在人为初始化对象的时候（全缺省参数）以及流提取时进行无效日期的判断
	bool CheckInvalid();//看起来没参数，但是类中的成员函数有一个隐藏的this指针参数

	//友元声明(朋友声明)
	friend ostream& operator<<(ostream& out, const Date& d);
	friend istream& operator>>(istream& in, Date& d);
	//友元声明允许一个函数或类访问另一个类的私有成员。
	//通过使用友元声明,可以授权指定的外部函数或类来访问另一个类的私有成员，而无需将这些外部函数或类作为该类的成员函数
	//友元声明通常在被授权访问私有成员的外部实体（如全局函数、其他类）中进行。下面是一些关于友元声明使用方式和语法示例：
	//注意事项：
	//友元关系不具备传递性。
	//类之间互为友好时，它们各自可见对方所有非私密信息。
	//应慎用友好机制以避免破坏封装性原则。

private:
	int _year;
	int _month;
	int _day;
};

//流插入运算重载，目的是为了让除了内置类型的对象能用流插入运算符，自定义类型的对象也可以用流插入
//流插入运算符属于iostraem类中的ostream对象
//将该运算符重载函数放在类外是因为<<是双目运算符，在类中该函数的形式operator<<(ostream& out),而类中的成员函数有一个默认的this指针作为第一个形参，所以实际上的形式是operator<<(this，ostream& out),而cout<<d1，很明显调用时运算符左右两侧的类型是相反的，那么就需要写成d1 << cout 的形式，但是这就表示控制台流入到日期类中（不会报错，但是倒反天罡）
//根本原因：operator<<作为成员函数重载，this指针必须占据第一个参数，Date必须是左操作数
//因此为了让ostream作为第一个参数必须让该函数是全局函数
//但是此时就无法直接访问private的成员变量了，
ostream& operator<<(ostream& out, const Date& d);//全局函数不能放在.h中否则容易出现重定义（两个.cpp文件都会包含该函数.h）
//ostream&是为了满足多次流插入的需求写的，如果只进行一次流插入可以写成void



//道理基本相同，流提取运算符属于iostream类中的istraem对象
istream& operator>>(istream& in, Date& d);//这里不加const的原因是,流提取的值要放在日期类中这个日期类碧血得是可以被读写的，加了const就不能更改日期类

