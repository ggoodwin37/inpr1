// alg.cpp: demo of various STL algorithms. Copied verbatim from "Professional C++", then tweaked.
#include <algorithm>
#include <functional>
#include <map>
#include <list>
#include <iostream>
#include <utility>
#include <string>
using namespace std;



class NameInList : public unary_function<string, bool>
{
public:
	NameInList(const list<string>& names) : mNames(names) { }
	bool operator() (const string& val);
protected:
	const list<string>& mNames;
};




// declare functor used above.
// TBD: understand this subclassing of unary_function a bit more! why is second type param void, eg?
// answer: given type params unary_function(T1, T2), then operator() has declaratin T2 C::operator() (T1(&))
class RemoveNames: public unary_function<pair<const string, list<string> >, void>
{
public:
	RemoveNames(const list<string>& names) : mNames(names) { }
	void operator() (pair<const string, list<string> >& val);
	
protected:
	const list<string>& mNames;
};



list<string> getDuplicates(const map<string, list<string> >& voters)
{
	list<string> allNames, duplicates;
	
	map<string, list<string> >::const_iterator it;
	for(it = voters.begin(); it != voters.end(); ++it) {
		// what is it->second referring to here? I think *it is a pair<T>? i.e. map returns iterated values
		// as pairs?
		allNames.insert(allNames.end(), it->second.begin(), it->second.end());
	}
	
	allNames.sort();
	
	list<string>::iterator lit;
	for(lit = allNames.begin(); lit != allNames.end(); ++lit) {
		lit = adjacent_find(lit, allNames.end());
		if(lit == allNames.end()) break;
		
		duplicates.push_back(*lit);
	}
	
	duplicates.sort();
	duplicates.unique();
	
	return duplicates;
}



void auditVoterRolls(map<string, list<string> >& votersByCounty, const list<string>& convictedFelons)
{

	list<string> duplicates = getDuplicates(votersByCounty);
	
	duplicates.insert(duplicates.end(), convictedFelons.begin(), convictedFelons.end());
	
	duplicates.sort();
	duplicates.unique();
	
	for_each(votersByCounty.begin(), votersByCounty.end(), RemoveNames(duplicates));
	
}




void RemoveNames::operator() (pair<const string, list<string> >& val)
{
	list<string>::iterator it = remove_if(val.second.begin(), val.second.end(), NameInList(mNames));
	val.second.erase(it, val.second.end());
}



bool NameInList::operator() (const string& val) {
	return (find(mNames.begin(), mNames.end(), val) != mNames.end());
}


void printString(const string& str)
{
	cout << " {" << str << "}";
}

void printCounty(const pair<const string, list<string> >& county)
{
	cout << county.first << ":";
	for_each(county.second.begin(), county.second.end(), &printString);
	cout << endl;
}

int main()
{

	map<string, list<string> > voters;
	list<string> nameList, felons;

	nameList.push_back("Bob");
	nameList.push_back("Frank");
	nameList.push_back("Ed");
	nameList.push_back("Alan");
	nameList.push_back("Jim");
	voters.insert(make_pair("Fresno", nameList));
	
	nameList.clear();
	nameList.push_back("Fred");
	nameList.push_back("Bill");
	nameList.push_back("Jim");
	nameList.push_back("Ed");
	nameList.push_back("Xander");
	voters.insert(make_pair("Sacramento", nameList));
	
	nameList.clear();
	nameList.push_back("Mitch");
	nameList.push_back("Frank");
	nameList.push_back("Clement");
	nameList.push_back("Hal");
	nameList.push_back("Jim");
	voters.insert(make_pair("Los Angeles", nameList));
	
	felons.push_back("Xander");
	felons.push_back("Hal");
	felons.push_back("Phil");
	
	for_each(voters.begin(), voters.end(), &printCounty);
	cout << endl;
	auditVoterRolls(voters, felons);
	for_each(voters.begin(), voters.end(), &printCounty);
	
	return 0;
}
	
