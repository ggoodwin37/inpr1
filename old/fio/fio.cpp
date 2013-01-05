// fio.cpp: test filestream i/o, basic exception syntax
#include <iostream>
#include <string>
#include <functional>
#include <vector>
#include <fstream>
#include <algorithm>

// simple functor
template<typename T>
class MyStreamOutputter
{
	public:
		MyStreamOutputter(std::ostream& ostr = std::cout): ostr_(ostr) { }
		
		void operator()(const T& data) {
			ostr_ << data;
			ostr_ << std::endl;
		}
	private:
		std::ostream& ostr_;
};

class FIO_B
{

	public:
		// tbd: here I provide standard constructor and virtual destructor, but
		//  prohibit copy/assn by declaring them private. But the rule of the Big 3
		//  states that if you provide one (destructor), must provide all 3.
		//  What will go wrong by disabling copy/assn?

		explicit FIO_B(const std::string& f) : filename_(f)
		{
			//std::cout << "FIO_B ctor." << std::endl;
		}
		virtual ~FIO_B()
		{
			//std::cout << "FIO_B dtor." << std::endl;
		}
		
		// pattern: NVI (non-virtual interface)
		//  public interface is concrete (non-virtual) in the base class.
		//  the implementation of the public API relies on protected virtual calls
		//  to implement polymorphic functionality. Reason? Bypass the potential
		//  misuse of overridden functions, i.e. failing to enforce contracts
		//  specified in the base class. (clearly overkill for this example!)
		//  Destructor must still be virtual, however.
		
		void input_data();
		void write_data_to_file() const;
		void read_data_from_file();
		void display_data() const;
	
	protected:
		
		// now provide the (pure) virtual interface, only visible within this class and
		//  subclasses.
		// any functionality that subclasses have in common can be factored into these
		//  methods (making them non-pure virtual as needed).
		virtual void do_input_data_prep() = 0;
		virtual void do_input_data_main() = 0;
		virtual void save_data() const = 0;
		virtual void read_data() = 0;
		virtual void output_data_to_screen() const = 0;

		// data common to all descendants
		std::string filename_;
		
	private:
		// no copy/assignment
		FIO_B(const FIO_B& no_copy_constructor);
		FIO_B& operator=(const FIO_B& no_assignment_operator);
};

void FIO_B::input_data()
{
	do_input_data_prep();
	do_input_data_main();
}

void FIO_B::write_data_to_file() const
{
	save_data();
}

void FIO_B::read_data_from_file()
{
	read_data();
}

void FIO_B::display_data() const
{
	output_data_to_screen();
}



// now define a descendant which implements the protected virtual interface specified above.

class FIO_D1: public FIO_B
{
	public:
		explicit FIO_D1(const std::string& fname);
		virtual ~FIO_D1();
		
		// child class doesn't add anything to public API!
		
	protected:
		virtual void do_input_data_prep();
		virtual void do_input_data_main();
		virtual void save_data() const;
		virtual void read_data();
		virtual void output_data_to_screen() const;

	private:
		FIO_D1(const FIO_D1& nocopy);
		FIO_D1& operator=(const FIO_D1& noassn);
	
		std::vector<int>	data_;
};

FIO_D1::FIO_D1(const std::string& fname): FIO_B(fname), data_()
{
	//std::cout << "FIO_D1 ctor." << std::endl;
}

FIO_D1::~FIO_D1()
{
	//std::cout << "FIO_D1 dtor." << std::endl;
}

void FIO_D1::do_input_data_prep()
{
	// ...
}

void FIO_D1::do_input_data_main()
{
	int i;
	while(1) {
		std::cout << "Enter a number (0 to exit): ";
		std::cin >> i;
		if(0 == i) break;
		data_.push_back(i);
	}
	std::sort(data_.begin(), data_.end());
}

void FIO_D1::save_data() const
{
	std::cout << "Saving data to file: " << filename_ << std::endl;
	std::ofstream ofs(filename_.c_str());
	if(ofs.is_open()) {
		std::for_each(data_.begin(), data_.end(), MyStreamOutputter<int>(ofs));
	} else {
		// throw exception
	}
}

void FIO_D1::read_data()
{
	std::cout << "Reading data from disk." << std::endl;
	int i;
	
	std::ifstream ifs(filename_.c_str());
	if(ifs.is_open()) {
	
		while(1) {
			ifs >> i;
			if(ifs.eof()) break;
			data_.push_back(i);
		}
		ifs.close();		// could be left to destructor to clean up.
	} else {
		// throw exception: bad filename/couldn't open
	}
}

void FIO_D1::output_data_to_screen() const
{
	std::for_each(data_.begin(), data_.end(), MyStreamOutputter<int>());
	std::cout << std::endl;
}

int main()
{
	const std::string fname = "test";
	FIO_D1 foo(fname);
	foo.input_data();
	foo.write_data_to_file();
	//foo.display_data();
	
	FIO_D1 foo2(fname);
	foo2.read_data_from_file();
	foo2.display_data();

	return 0;
}


