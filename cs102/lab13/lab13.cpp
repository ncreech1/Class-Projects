#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <stdexcept>

using namespace std;

/* Nicholas Creech
 * 11/14/18
 * Lab13: Create a "vector" class manually by using pointers and dynamic memory
 */ 

class doublevector {
     static constexpr double DOUBLE_DEFAULT_VALUE = -5.55;
     double *mValues;
     int mNumValues;
public:
     doublevector();
     ~doublevector();
     void resize(int new_size, double initial_value=DOUBLE_DEFAULT_VALUE);
     void push_back(double value);
     double &at(int index);
     const double &at(int index) const;
     unsigned long size() const;
};
void print_all(const doublevector &v)
{
     if (v.size() == 0) {
           cout << "Vector is empty.\n";
     }
     else {
          unsigned long i;
          for (i = 0;i < v.size();i++) {
                cout << "[" << setfill('0') << right << setw(3) << i
                     << "] = " << fixed << setprecision(4) << v.at(i)
                     << '\n';
          }
     }
}
int main()
{
      doublevector v;
      do {
           string command;
           cout << "Enter a command ('quit' to quit): ";
           if (!(cin >> command) || command == "quit") {
                break;
           }
           else if (command == "push_back") {
                double value;
                cin >> value;
                v.push_back(value);
		cout << "Pushed back "
		     << fixed << setprecision(4)
		     << value << '\n';
           }
           else if (command == "resize") {
                string line;
                int new_size;
                double initial;
                cin >> new_size;
                getline(cin, line);
                istringstream sin(line);

                if (sin >> initial)
                     v.resize(new_size, initial);
                else
                     v.resize(new_size);
           }
           else if (command == "size") {
                cout << v.size() << '\n';
           }
           else if (command == "print") {
                print_all(v);
           }
           else if (command == "get") {
                int index;
                cin >> index;
                try {
                     cout << "Value at " << index << " = "
		          << setprecision(4) << fixed
                          << v.at(index) << '\n';;
                }
                catch(out_of_range &err) {
                     cout << err.what() << '\n';
                }
           }
           else if (command == "set") {
		   double d;
		   int index;
                   cin >> index >> d;
                   try {
			v.at(index) = d;
                        cout << "SET: " << index << " = "
			     << setprecision(4) << fixed
                             << v.at(index) << '\n';;
                   }
                   catch(out_of_range &err) {
                        cout << err.what() << '\n';
                   }
           }
           else if (command == "clear") {
                v.resize(0);
           }
           else {
                cout << "Invalid command '" << command << "'\n";
           }
      } while(true);
      cout << endl;
      return 0;
}

doublevector::doublevector()
{
	mValues = nullptr;
	mNumValues = 0;
}

doublevector::~doublevector()
{
	if(mValues != nullptr)
		delete[] mValues;
}

void doublevector::resize(int new_size, double initial_value)
{
	if(new_size == 0) //Resize to zero; free memory
	{
		if(mValues != nullptr)
			delete[] mValues;
		mNumValues = 0;
	}

	else if(new_size > mNumValues) //Greater than current size; expand
	{
		double* newValues = new double[new_size];
		for(int x = 0; x < new_size; x++)
		{
			if(x < mNumValues) //Copy values from old array to new one
				newValues[x] = mValues[x];
			else
				newValues[x] = initial_value; //Set new values to initial_value
		}

		if(mValues != nullptr)
			delete[] mValues;
		mValues = newValues;
		mNumValues = new_size;
	}

	else if(new_size < mNumValues && new_size > 0) //Smaller than current size; shrink
	{
		double* newValues = new double[new_size];
		for(int x = 0; x < new_size; x++)
			newValues[x] = mValues[x]; //Copy values from old array to new one

		if(mValues != nullptr)
			delete[] mValues;
		mValues = newValues;
		mNumValues = new_size;
	}
}

void doublevector::push_back(double value)
{
	resize(mNumValues + 1, value);
}

double& doublevector::at(int index)
{
	if(index >= mNumValues)
	{
		ostringstream sout;
		sout << "Invalid index #" << index;
		throw out_of_range(sout.str());
	}
	
	return mValues[index];
}

const double& doublevector::at(int index) const
{
	if(index >= mNumValues)
	{
		ostringstream sout;
		sout << "Invalid index #" << index;
		throw out_of_range(sout.str());
	}
	
	return mValues[index];
}

unsigned long doublevector::size() const
{
	return mNumValues;
}
