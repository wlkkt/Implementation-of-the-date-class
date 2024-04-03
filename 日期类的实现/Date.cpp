#include "Date.h"

//以下的所有函数都位于类域中，这就导致以往的声明与定义分离时在.cpp文件中直接写函数名即可，
// 但是位于特定类域中的函数只有在指明函数所在的类域时才能使用，故在函数名前加上了域作用限定符

//全缺省的默认构造函数（确定默认值，函数声明和定义分离时缺省值未避免歧义只能出现一次，最好是在声明中给与）
Date::Date(int year, int month, int day)//全缺省的默认构造函数不需要返回值
{
	_year = year;
	_month = month;
	_day = day;

	if (!CheckInvalid())//人为初始化的时候（上来就定义d2(2024,2,30)）也得检查
	{
		cout << "构造日期非法" << endl;
	}
}

//小于运算
bool Date::operator<(const Date& y)
{
	if (_year < y._year)
	{
		return true;
	}
	else if (_year == y._year)
	{
		if (_month < y._month)
		{
			return true;
		}
		else if (_month == y._month)
		{
			return _day < y._day;
		}
	}
	return false;
}

//小于等于
bool Date::operator<=(const Date& y)
{
	return *this < y || *this == y;//this是指向的是对象d1空间的指针，对this的解引用可以获得d1对象的值
}
//涉及对象判断、加减的运算符在运算时都会去调用我们自定义的运算符重载函数
//如果判断d1<=d2，在进入该运算符重载函数时，会先调用小于运算符重载函数的情况，虽然看起来只是一个<，实际上却是(*this).operator<(y),可以写成一个简单的<是因为编译器替我们做了处理
//判断*this是否小于y，如果小于就返回真否则为假，由于总的目的是判断d1是否小于等于d2，所以满足小于或等于一个条件即可返回真

//大于
bool Date::operator>(const Date& y)
{
	return !(*this <= y);//*this<=y的结果如果为真，即d1<=d2为真，再加上一个逻辑取反运算符!，就可以在逻辑上表明d1是不大于d2的，合理的运用逻辑取反运算符可以省略很多代码
}//判断小于等于就会调用operator<=，进而调用operator<和operator==

//大于等于
bool Date::operator>=(const Date& y)
{
	return !(*this < y);//原理同上
}

//等于
bool Date::operator==(const Date& y)
{
	return _year == y._year
		&& _month == y._month
		&& _day == y._day;
}

//不等于
bool Date::operator!=(const Date& y)
{
	return !(*this == y);//原理同上
}

//日期加天数（如果将该函数作为operator+，那么该函数就会导致d2 = d1 + 20得到d2时，d1的值也会发生改变，毕竟是this->_day = day，所以为了避免对d1的修改应该传入一个d1的拷贝代替d1进行运算）
Date& Date::operator+=(int day)
{//此时该函数传入的this指针是tmp对象的地址
	_day += day;//_day = 29+20 = 49
	//cout<<"还没进来" << endl;
	//t天大于当前月应该的天数，天数就应该减去当前月对应的天数，同时将月数++，进行下一个月的判断
	while (_day > GetMonthDay(_year, _month))//此时调用GetMonthDay函数，49>(该函数的返回值是)
	{
		//cout<<"进来了" << endl;
		_day -= GetMonthDay(_year, _month);
		++_month;
		if (_month == 13)//如果天数大到把12个月份的所有天数都减完，此时的_month的值应该就是13
		{
			++_year;//_month=13证明_day都比365或366都大，此时年份+1
			_month = 1;//重现将月份置为1，再次开始循环判断
		}
	}
	return *this;//返回类型为tmp对象的别名，而不是tmp对象的值当函数返回一个对象（而不是引用）时，会调用拷贝构造函数来创建一个新的对象，并将其作为函数的返回值。这意味着会发生一次数据复制
}//返回引用允许直接操作原始对象而不是在内存中创建副本。这样可以避免额外开销和数据复制

//日期加天数
Date Date::operator+(int day)//这里的返回值类型是Date，函数结束后tmp被销毁，如果使用Date&作为返回值，返回给d2的是tmp的别名，但是tmp所在空间已经被销毁里面的值也不保证有效了，而Date的话就可以将tmp的值放入临时变量中最后交给d2
{
	Date tmp = *this;//拷贝构造，等价于Date tmp(*this)
	tmp += day;//tmp = tmp + day,此时应该调用operator+=，且此时传入的day也是原来我们提供的数字
	return tmp;//为了防止d1+20对d1本身也产生了影响所以这里要用一个Date类型对象来做d1对象的拷贝，对tmp的修改无法影响d1
}


//日期减天数
Date Date::operator-(int day)//Date防止野引用
{
	Date tmp = *this;
	tmp -= day;
	return tmp;
}


//日期减天数
Date& Date::operator-=(int day)//Date&增加效率
{
	_day -= day;//用this->_day减去传递的天数
	while (_day <= 0)//如果天数大于_day，_day的结果就是负数，那么就将月份向前移动一月，月份减一，如果天数小于_day不进入循环直接将减去天数后的日期返回即可
	{
		--_month;
		if (_month == 0)//如果月份减一变为了零证明今年天数不够了，需要将年份向前移动一年，年份减一，同时将月份变为12月份
		{
			--_year;
			_month = 12;
		}
		_day += GetMonthDay(_year, _month);//_day在原有的为负数的基础上加上当前年当前月份的天数，如果结果大于等于零那么证明日期减天数的结果的确是该年该月
	}

	return *this;
}

//前置++,编译器在调用时会自动识别++d1->d.operator++()
Date& Date::operator++()
{
	*this += 1;//调用operator+=，1表示天数（++日期嘛肯定是从天数上开始加的）
	return *this;//前置++：先++后使用，直接返回自己加一的结果即可
}

//后置++，编译器在调用时会自动识别++d1->d.operator++(0)，括号里的可以是0、1等整型常量只是起到标识作用
Date Date::operator++(int)
{
	Date tmp = *this;//拷贝构造，tmp是d1的拷贝
	*this += 1;//调用operator+=，1表示天数（++日期嘛肯定是从天数上开始加的）
	return tmp;//后置++：先使用后++，要先返回使用前的结果，然后再++，tmp中保存的就是++前的结果
}

//前置--
Date& Date::operator--()
{
	*this -= 1;
	return *this;

}

//后置--
Date Date::operator--(int)
{
	Date tmp = *this;
	*this -= 1;
	return tmp;
}

//日期减日期（假设法）
int Date::operator-(const Date& d)//这里的d是对象d2，而不是具体的天数
{
	int flag = 1;//标志，假设d1>d2，同时也可以用来表示运算结果的正负
	Date max = *this;//拷贝构造，将d1拷贝给max对象
	Date min = d;//拷贝构造将d2，将d2拷贝给min对象，因为我们默认传入的左操作数比右操作数小，所以直接进行了拷贝

	//但是有可能我们将大小搞反了，所以还需要进行判断与修正
	if (*this < d)//如果d1<d2那么就将flag设置为-1，同时交换对象的值
	{
		int flag = -1;
		max = d;
		min = *this;
	}//反正到这一步max对象肯定是大日期对象的拷贝，min肯定是小日期对象的拷贝

	int n = 0;
	while (min != max)//operator!=的效率会比operator<的效率高，反正最后的循环终止条件是min==max，多次掉用更麻烦的operator<达到==还不如选择多次调用简单的operator!=达到==
	{
		++min;//直到min的日期加到和max的日期相等时才会跳出循环
		++n;//min的日期每++一次，n也跟着++
	}
	return n * flag;//如果flag最后为正，则d1 - d2时，d1>d2，结果肯定也为正，如果最后flag为负，则d1 - d2时，d1<d2，结果肯定为负
}


ostream& operator<<(ostream& out, const Date& d)
{
	out << d._year << "年" << d._month << "月" << d._day << "日" << endl;//自定义类型的本质还是内置类型，所以可以用operator<<重载读取自定义类型中的内置类型
	return out;//最后返回out的别名即可
}

istream& operator>>(istream& in, Date& d)
{
	while (1)
	{
		cout << "依次输入年月日：>";
		in >> d._year >> d._month >> d._day;

		if (!d.CheckInvalid())//检查输入日期是否有效
		{
			cout << "输入日期无效" << endl;
		}
		else
		{
			break;
		}
	}

	return in;
}

bool Date::CheckInvalid()
{
	if (this->_year <= 0
		|| this->_month < 1 || this->_month >12
		|| this->_day < 1 || this->_day > GetMonthDay(this->_year, this->_month))//防止遗忘this指针这里就直接加上了
	{
		return false;
	}

	else
	{
		true;
	}
}

