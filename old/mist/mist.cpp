// mist.cpp: miscellaneous STL practice.
// cover vector, list, and map

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <iterator>
#include <map>
#include <utility>

using namespace std;


struct string_output: public unary_function<string, void>
{
	void operator() (const string& st) { cout << st << endl; }
};

struct string_pair_output: public unary_function<pair<string, string>, void>
{
	void operator() (const pair<string, string>& p) { cout << p.first << "->" << p.second << endl; }
};

struct string_addsomecrap: public unary_function<string, void>
{
	void operator() (string& st) { st += " plus some crap."; }
};

struct find_relative: public unary_function<pair<string, string>, bool>
{
	bool operator() (const pair<string, string>& p);
};

bool find_relative::operator() (const pair<string, string>& p)
{
	if(p.second == "Goodwin" || p.second == "goodwin") return true;
	return false;
}

template <typename T>
void populate_seq_container(T& seq_container)
{
	cout << "populate_seq_container." << endl;
	
	string st;
	
	while(1) {
		cout << "enter str #" << (seq_container.size()+1) << " ('done' to stop): " << flush;
		cin >> st;
		
		if(st == "done") break;
		else seq_container.push_back(st);
	}
	cout << endl;
	
}

void populate_map(map<string, string>& ma)
{

	cout << "populate_map." << endl;
	
	string st1, st2;
	
	while(1) {
		cout << "enter key #" << (ma.size()+1) << " ('done' to stop): " << flush;
		cin >> st1;
		
		if(st1 == "done") break;
		else {
			cout << "enter value #" << (ma.size()+1) << ": " << flush;
			cin >> st2;
			ma[st1] = st2;
		}
	}
	cout << endl;
}

int main()
{
/*	vector<string> vec;
	
	populate_seq_container(vec);

	cout << " -- input --" << endl;
	string_output func;
	for_each(vec.begin(), vec.end(), func);

	cout << " -- sorted --" << endl;
	sort(vec.begin(), vec.end());
	for_each(vec.begin(), vec.end(), func);

	cout << " -- add some crap --" << endl;
	string_addsomecrap func2;
	for_each(vec.begin(), vec.end(), func2);
	for_each(vec.begin(), vec.end(), func);
	
	list<string> li;
	populate_seq_container(li);
	
	cout << " -- the list --" << endl;
	copy(li.begin(), li.end(), ostream_iterator<string>(cout, "\n"));
	
	int mid = vec.size() / 2;
	vec.insert(vec.begin() + mid, li.begin(), li.end());
	
	cout << " -- insert the list into the middle of the vec --" << endl;
	for_each(vec.begin(), vec.end(), func);
*/

	map<string, string> ma;
	string_pair_output spo;
	populate_map(ma);
	
	cout << " -- the map --" << endl;
	for_each(ma.begin(), ma.end(), spo);
	
	//find_relative predicate;
	//map<string, string>::iterator it = find_if(ma.begin(), ma.end(), predicate);
	map<string, string>::iterator it = ma.find("Gideon");
	if(it != ma.end()) {
		cout << "Found your firstname, your last name is: " << (*it).second << endl;
	} else {
		cout << "Didn't find your first name." << endl;
	}
	
	return 0;
}
