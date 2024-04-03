#include "Date.h"

//���µ����к�����λ�������У���͵��������������붨�����ʱ��.cpp�ļ���ֱ��д���������ɣ�
// ����λ���ض������еĺ���ֻ����ָ���������ڵ�����ʱ����ʹ�ã����ں�����ǰ�������������޶���

//ȫȱʡ��Ĭ�Ϲ��캯����ȷ��Ĭ��ֵ�����������Ͷ������ʱȱʡֵδ��������ֻ�ܳ���һ�Σ�������������и��룩
Date::Date(int year, int month, int day)//ȫȱʡ��Ĭ�Ϲ��캯������Ҫ����ֵ
{
	_year = year;
	_month = month;
	_day = day;

	if (!CheckInvalid())//��Ϊ��ʼ����ʱ�������Ͷ���d2(2024,2,30)��Ҳ�ü��
	{
		cout << "�������ڷǷ�" << endl;
	}
}

//С������
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

//С�ڵ���
bool Date::operator<=(const Date& y)
{
	return *this < y || *this == y;//this��ָ����Ƕ���d1�ռ��ָ�룬��this�Ľ����ÿ��Ի��d1�����ֵ
}
//�漰�����жϡ��Ӽ��������������ʱ����ȥ���������Զ������������غ���
//����ж�d1<=d2���ڽ������������غ���ʱ�����ȵ���С����������غ������������Ȼ������ֻ��һ��<��ʵ����ȴ��(*this).operator<(y),����д��һ���򵥵�<����Ϊ���������������˴���
//�ж�*this�Ƿ�С��y�����С�ھͷ��������Ϊ�٣������ܵ�Ŀ�����ж�d1�Ƿ�С�ڵ���d2����������С�ڻ����һ���������ɷ�����

//����
bool Date::operator>(const Date& y)
{
	return !(*this <= y);//*this<=y�Ľ�����Ϊ�棬��d1<=d2Ϊ�棬�ټ���һ���߼�ȡ�������!���Ϳ������߼��ϱ���d1�ǲ�����d2�ģ�����������߼�ȡ�����������ʡ�Ժܶ����
}//�ж�С�ڵ��ھͻ����operator<=����������operator<��operator==

//���ڵ���
bool Date::operator>=(const Date& y)
{
	return !(*this < y);//ԭ��ͬ��
}

//����
bool Date::operator==(const Date& y)
{
	return _year == y._year
		&& _month == y._month
		&& _day == y._day;
}

//������
bool Date::operator!=(const Date& y)
{
	return !(*this == y);//ԭ��ͬ��
}

//���ڼ�������������ú�����Ϊoperator+����ô�ú����ͻᵼ��d2 = d1 + 20�õ�d2ʱ��d1��ֵҲ�ᷢ���ı䣬�Ͼ���this->_day = day������Ϊ�˱����d1���޸�Ӧ�ô���һ��d1�Ŀ�������d1�������㣩
Date& Date::operator+=(int day)
{//��ʱ�ú��������thisָ����tmp����ĵ�ַ
	_day += day;//_day = 29+20 = 49
	//cout<<"��û����" << endl;
	//t����ڵ�ǰ��Ӧ�õ�������������Ӧ�ü�ȥ��ǰ�¶�Ӧ��������ͬʱ������++��������һ���µ��ж�
	while (_day > GetMonthDay(_year, _month))//��ʱ����GetMonthDay������49>(�ú����ķ���ֵ��)
	{
		//cout<<"������" << endl;
		_day -= GetMonthDay(_year, _month);
		++_month;
		if (_month == 13)//��������󵽰�12���·ݵ��������������꣬��ʱ��_month��ֵӦ�þ���13
		{
			++_year;//_month=13֤��_day����365��366���󣬴�ʱ���+1
			_month = 1;//���ֽ��·���Ϊ1���ٴο�ʼѭ���ж�
		}
	}
	return *this;//��������Ϊtmp����ı�����������tmp�����ֵ����������һ�����󣨶��������ã�ʱ������ÿ������캯��������һ���µĶ��󣬲�������Ϊ�����ķ���ֵ������ζ�Żᷢ��һ�����ݸ���
}//������������ֱ�Ӳ���ԭʼ������������ڴ��д����������������Ա�����⿪�������ݸ���

//���ڼ�����
Date Date::operator+(int day)//����ķ���ֵ������Date������������tmp�����٣����ʹ��Date&��Ϊ����ֵ�����ظ�d2����tmp�ı���������tmp���ڿռ��Ѿ������������ֵҲ����֤��Ч�ˣ���Date�Ļ��Ϳ��Խ�tmp��ֵ������ʱ��������󽻸�d2
{
	Date tmp = *this;//�������죬�ȼ���Date tmp(*this)
	tmp += day;//tmp = tmp + day,��ʱӦ�õ���operator+=���Ҵ�ʱ�����dayҲ��ԭ�������ṩ������
	return tmp;//Ϊ�˷�ֹd1+20��d1����Ҳ������Ӱ����������Ҫ��һ��Date���Ͷ�������d1����Ŀ�������tmp���޸��޷�Ӱ��d1
}


//���ڼ�����
Date Date::operator-(int day)//Date��ֹҰ����
{
	Date tmp = *this;
	tmp -= day;
	return tmp;
}


//���ڼ�����
Date& Date::operator-=(int day)//Date&����Ч��
{
	_day -= day;//��this->_day��ȥ���ݵ�����
	while (_day <= 0)//�����������_day��_day�Ľ�����Ǹ�������ô�ͽ��·���ǰ�ƶ�һ�£��·ݼ�һ���������С��_day������ѭ��ֱ�ӽ���ȥ����������ڷ��ؼ���
	{
		--_month;
		if (_month == 0)//����·ݼ�һ��Ϊ����֤���������������ˣ���Ҫ�������ǰ�ƶ�һ�꣬��ݼ�һ��ͬʱ���·ݱ�Ϊ12�·�
		{
			--_year;
			_month = 12;
		}
		_day += GetMonthDay(_year, _month);//_day��ԭ�е�Ϊ�����Ļ����ϼ��ϵ�ǰ�굱ǰ�·ݵ����������������ڵ�������ô֤�����ڼ������Ľ����ȷ�Ǹ������
	}

	return *this;
}

//ǰ��++,�������ڵ���ʱ���Զ�ʶ��++d1->d.operator++()
Date& Date::operator++()
{
	*this += 1;//����operator+=��1��ʾ������++������϶��Ǵ������Ͽ�ʼ�ӵģ�
	return *this;//ǰ��++����++��ʹ�ã�ֱ�ӷ����Լ���һ�Ľ������
}

//����++���������ڵ���ʱ���Զ�ʶ��++d1->d.operator++(0)��������Ŀ�����0��1�����ͳ���ֻ���𵽱�ʶ����
Date Date::operator++(int)
{
	Date tmp = *this;//�������죬tmp��d1�Ŀ���
	*this += 1;//����operator+=��1��ʾ������++������϶��Ǵ������Ͽ�ʼ�ӵģ�
	return tmp;//����++����ʹ�ú�++��Ҫ�ȷ���ʹ��ǰ�Ľ����Ȼ����++��tmp�б���ľ���++ǰ�Ľ��
}

//ǰ��--
Date& Date::operator--()
{
	*this -= 1;
	return *this;

}

//����--
Date Date::operator--(int)
{
	Date tmp = *this;
	*this -= 1;
	return tmp;
}

//���ڼ����ڣ����跨��
int Date::operator-(const Date& d)//�����d�Ƕ���d2�������Ǿ��������
{
	int flag = 1;//��־������d1>d2��ͬʱҲ����������ʾ������������
	Date max = *this;//�������죬��d1������max����
	Date min = d;//�������콫d2����d2������min������Ϊ����Ĭ�ϴ��������������Ҳ�����С������ֱ�ӽ����˿���

	//�����п������ǽ���С�㷴�ˣ����Ի���Ҫ�����ж�������
	if (*this < d)//���d1<d2��ô�ͽ�flag����Ϊ-1��ͬʱ���������ֵ
	{
		int flag = -1;
		max = d;
		min = *this;
	}//��������һ��max����϶��Ǵ����ڶ���Ŀ�����min�϶���С���ڶ���Ŀ���

	int n = 0;
	while (min != max)//operator!=��Ч�ʻ��operator<��Ч�ʸߣ���������ѭ����ֹ������min==max����ε��ø��鷳��operator<�ﵽ==������ѡ���ε��ü򵥵�operator!=�ﵽ==
	{
		++min;//ֱ��min�����ڼӵ���max���������ʱ�Ż�����ѭ��
		++n;//min������ÿ++һ�Σ�nҲ����++
	}
	return n * flag;//���flag���Ϊ������d1 - d2ʱ��d1>d2������϶�ҲΪ����������flagΪ������d1 - d2ʱ��d1<d2������϶�Ϊ��
}


ostream& operator<<(ostream& out, const Date& d)
{
	out << d._year << "��" << d._month << "��" << d._day << "��" << endl;//�Զ������͵ı��ʻ����������ͣ����Կ�����operator<<���ض�ȡ�Զ��������е���������
	return out;//��󷵻�out�ı�������
}

istream& operator>>(istream& in, Date& d)
{
	while (1)
	{
		cout << "�������������գ�>";
		in >> d._year >> d._month >> d._day;

		if (!d.CheckInvalid())//������������Ƿ���Ч
		{
			cout << "����������Ч" << endl;
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
		|| this->_day < 1 || this->_day > GetMonthDay(this->_year, this->_month))//��ֹ����thisָ�������ֱ�Ӽ�����
	{
		return false;
	}

	else
	{
		true;
	}
}

