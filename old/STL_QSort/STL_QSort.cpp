
/*
	My crappy version of the qsort using STL. -- ggoodwin 10/31/06
	
	Improvements/modifications:
		-use a list instead of a vector
		-use algorithms for the main loop in my_qsort
		-do an in-place version (vector)
*/

#include <iostream>
#include <vector>
#include <list>
#include <cstdio>
#include <cstdlib>

using std::list;
using std::vector;
using std::cout;
using std::endl;

void my_qsort_vector(vector<int>& v)
{
	if(v.size() <= 1) return;		// already sorted

	vector<int> v_less_equal;
	vector<int> v_more;

	vector<int>::iterator it_pivot = v.begin();
	
	// loop through the vector and add all values <= *itpivot to one list, all others to another list.
	// TBD: use an algorithm for this rather than manually looping!
	
	for(vector<int>::iterator it = v.begin() + 1; it < v.end(); ++it) {
		if(*it <= *it_pivot) {
			v_less_equal.push_back(*it);
		} else {
			v_more.push_back(*it);
		}
	}
	
	// sort the sublists
	my_qsort_vector(v_less_equal);
	my_qsort_vector(v_more);
	
	// rebuild v by adding less, pivot, greater
	// UGLY: do this better!
	int pivot_val = *it_pivot;
	v.clear();
	for(vector<int>::iterator it = v_less_equal.begin(); it < v_less_equal.end(); ++it) {
		v.push_back(*it);	
	}
	v.push_back(pivot_val);	
	for(vector<int>::iterator it = v_more.begin(); it < v_more.end(); ++it) {
		v.push_back(*it);	
	}
}

void my_qsort_list(list<int>& l)
{
	if(l.size() <= 1) return;		// already sorted

	list<int> l_less_equal;
	list<int> l_more;

	list<int>::iterator it_pivot = l.begin();
	
	// loop through the vector and add all values <= *itpivot to one list, all others to another list.
	// TBD: use an algorithm for this rather than manually looping!
	
	for(list<int>::iterator it = ++l.begin(); it != l.end(); ++it) {
		if(*it <= *it_pivot) {
			l_less_equal.push_back(*it);
		} else {
			l_more.push_back(*it);
		}
	}
	
	// sort the sublists
	my_qsort_list(l_less_equal);
	my_qsort_list(l_more);
	
	// splice lists together
	int pivot_val = *it_pivot;
	l.clear();
	l.splice(l.end(), l_less_equal);
	l.push_back(pivot_val);
	l.splice(l.end(), l_more);
}

template<typename T>
void show_collection(T& v)
{
	cout << "[ ";
	for(typename T::iterator it = v.begin(); it != v.end(); cout << *it << " ", ++it);
	cout << "]" << endl;
}

template<typename T>
void randomize_collection(T& v, const int num)
{
	int foo;
	for(int i = 0; i < num; i++) {
		v.push_back(rand()%50000);
	}
}

int main()
{
	const int my_size = 500000;
	vector<int> v;
	randomize_collection(v, my_size);
	time_t the_time;
	
	the_time = time(0);
	cout << "before sorting (vector). time: " << ctime(&the_time) << endl;

	//cout << " pre-qsort:" << endl;
	//show_collection(v);
	
	my_qsort_vector(v);

	the_time = time(0);
	cout << "done sorting (vector). time: " << ctime(&the_time) << endl;

	//cout << "post-qsort:" << endl;
	//show_collection(v);
	
	
	
	list<int> l;
	randomize_collection(l, my_size);
	
	the_time = time(0);
	cout << "before sorting (list). time: " << ctime(&the_time) << endl;

	//cout << " pre-qsort:" << endl;
	//show_collection(l);
	
	my_qsort_list(l);

	the_time = time(0);
	cout << "done sorting (list). time: " << ctime(&the_time) << endl;

	//cout << "post-qsort:" << endl;
	//show_collection(l);
	
	
	
	return 0;
}
