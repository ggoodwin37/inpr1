#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Foo
{
	public:
		Foo(string sName="Default");
		Foo(const Foo& copy);
		const Foo& operator=(const Foo& rhs);
		virtual ~Foo();

		const string GetName() const;
		void SetName(const string& name);
		
	private:
		string mName;
};

Foo::Foo(string sName): mName(sName) { }

Foo::Foo(const Foo& copy): mName(copy.GetName()) { }

Foo::~Foo() { }

const Foo& Foo::operator=(const Foo& rhs)
{
	SetName(rhs.GetName());
	return (*this);
}

const string Foo::GetName() const
{
	return mName;
}

void Foo::SetName(const string& name)
{
	mName = name;
}


void PerformInput(void)
throw(exception)
{
	int nInts;
	
	wcout << L"Enter the number of values: ";
	wcin >> nInts;
	wcout << L"The value you entered was " << nInts << endl;
	
	vector<int> v(nInts);
	
	for(int i = 0; i < nInts; i++)
	{
		wcout << L"Enter value " << i << L": ";
		wcin >> v[i];
	}

	long int lProduct = 1;
	for(int i = 0; i < nInts; i++)
	{
		lProduct *= v[i];
	}
	wcout << L"Product of all values is: " << lProduct << endl;


	// TBD: come up with some scenario where we can test out further stream methods. check for valid, etc. throw an exception if not valid. test stack unwinding.

}



int main(int argc, char **argv)
{

	PerformInput();

}
