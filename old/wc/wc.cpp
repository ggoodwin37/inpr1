// wc.cpp: demo of C++ widechar stuff
#include <iostream>
#include <fstream>
using std::wstring;
using std::wcout;
using std::wcin;
using std::endl;
using std::wofstream;

int main()
{
	
	wcout << L"Welcome to widechar demo." << endl;
	wchar_t test = L'e';
	wcout << L"Test wchar_t: '" << test << "'." << endl;
	
	wstring str = L"Test wide string.";
	wofstream wof("wt.txt");
	if(wof.is_open()) {
		wof << str << endl;
		wof.close();
	}

}