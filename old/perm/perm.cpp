// perm.cpp: a permutation algorithm in STL.
//  algorithm based on one from Alexander Bogomolyn
#include <vector>
#include <list>
#include <iostream>
#include <string>
#include <algorithm>

using std::list;
using std::vector;
using std::cout;
using std::endl;
using std::string;

class StringPermutator
{
		string				str_;
		mutable int			col;
		static const int	col_wrap = 6;
	
	public:
  		
  		StringPermutator(const string& s = "def"): str_(s), col(0) { }
  		
  		void operator() (const vector<int>& v) const;
};


void StringPermutator::operator() (const vector<int>& v) const
{
	for(vector<int>::const_iterator it = v.begin(); it != v.end(); it++) {
		cout << str_[*it - 1];	// since perm algo gives results on [1,N], not [0,N-1]
	}
	if(++col % col_wrap) cout << "\t";
	else cout << endl;
}


void visit(vector<int>& v, const int k, list<vector<int> >& results)
{
	static int level = -1;
	level++;
	v[k] = level;
	
	if(level == v.size()) {
		results.push_back(v);		// copy this configuration of v to results list
	} else {
		for(int i = 0; i < v.size(); i++) {
			if(v[i] == 0) {
				visit(v, i, results);
			}
		}
	}
	level--;
	v[k] = 0;

}

int main()
{
	string str_test = "abcdef";				// length defines N
	vector<int> work_vector;				// the work vector used in the recursion
	list<vector<int> > results;				// a list of results
	
	for(int i = 0; i < str_test.size(); ++i) work_vector.push_back(0);
	
	visit(work_vector, 0, results);

	// now results is a list of all permutations
	cout << "N=" << str_test.size() << ". Total permutations: " << results.size() << endl;
	StringPermutator fu(str_test);
  std::for_each(results.begin(), results.end(), fu);
	
	return 0;
}
