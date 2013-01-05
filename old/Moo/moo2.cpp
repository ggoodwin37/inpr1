// moo2.cpp: investigate some curious behavior relating to return-by-value that I noticed while playing with a globalfriend operator+
#include <string>
#include <iostream>
using std::string;
using std::cout;
using std::endl;

class Test
{
		string		member_;
	
	public:
		Test(const string& s) : member_(s)  { }
		Test(const Test& copy) : member_(copy.member_) {
			cout << "copy" << endl;
		}
		
		void out() { cout << member_ << endl; }
		
};

Test test_by_value(const Test& in)
{
	Test temp(in);
	return temp;
}

Test test_by_value_2() {

	Test temp("stack_var?");
	return temp;					// is this copied across stack frames? -apparently not
}

int main()
{
	//Test t("hello");
	//Test v = test_by_value(t);
	//v.out();

	Test u = test_by_value_2();
	u.out();
	
	return 0;
}


/*
expected output:
copy
copy
hello


actual output:
copy
hello

how is the copied value being returned from test_by_value() without a second copy (since temp is a stack var)?
*/
