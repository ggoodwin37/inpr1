// moo.cpp: misc operator overloading tests.
#include <iostream>
#include <ostream>

using std::cout;
using std::endl;
using std::ostream;

class Moo
{
		int x_;

	public:
		Moo(int in=0) : x_(in) { }
		Moo(const Moo& copy);		
		Moo& operator=(const Moo& other);

		Moo& operator+=(const Moo& other);
		Moo& operator-=(const Moo& other);
		
		Moo& operator++();
		const Moo operator++(int);
		
		friend ostream& operator<< (ostream& os, const Moo& m);
		friend Moo operator+(const Moo& left, const Moo& right);
		friend const Moo operator-(const Moo& left, const Moo& right);
		
		friend bool operator<(const Moo& left, const Moo& right);
		friend bool operator>(const Moo& left, const Moo& right);
		friend bool operator<=(const Moo& left, const Moo& right);
		friend bool operator>=(const Moo& left, const Moo& right);
		friend bool operator==(const Moo& left, const Moo& right);
		
		
};

Moo::Moo(const Moo& copy) : x_(copy.x_)
{
	//cout << "copy" << endl;
}

Moo& Moo::operator= (const Moo& other)
{
	if(this != &other) {
		x_ = other.x_;
	}		
	return *this;
}

Moo& Moo::operator+=(const Moo& other) {
	x_ += other.x_;
	return *this;
}

Moo& Moo::operator-=(const Moo& other)
{
	x_ -= other.x_;
	return *this;
}

Moo& Moo::operator++()
{
	x_ += 1;
	return *this;
}

const Moo Moo::operator++(int)
{
	Moo ret(*this);
	x_ += 1;
	return ret;
}

//////////////////////////////////////////////////////////////////////
// global friend functions for Moo
ostream& operator<< (ostream& os, const Moo& m)
{
	os << m.x_;
	return os;
}

Moo operator+(const Moo& left, const Moo& right)
{
	Moo temp(left);
	temp += right;
	return temp;
}

bool operator<(const Moo& left, const Moo& right)
{
	return (left.x_ < right.x_);
}

bool operator>(const Moo& left, const Moo& right)
{
	return (left.x_ > right.x_);
}

bool operator<=(const Moo& left, const Moo& right)
{
	return !(left > right);
}

bool operator>=(const Moo& left, const Moo& right)
{
	return !(left < right);
}

bool operator==(const Moo& left, const Moo& right)
{
	return (left.x_ == right.x_);
}


int main()
{

	Moo m1(5), m2(6);
	
	cout << "m1 is " << m1 << endl;
	cout << "m2 is " << m2 << endl;
	
	cout << std::boolalpha;
	
	cout << "m1 < m2? " << (m1 < m2) << endl;
	cout << "m1 >= m2? " << (m1 >= m2) << endl;
	cout << "++m1 is " << (++m1) << endl;
	cout << "m1 == m2? " << (m1 == m2) << endl;

	return 0;
}

