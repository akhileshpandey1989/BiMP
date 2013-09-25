#include<string>

using namespace std;

class Exception
{
	string message;
	public: Exception(const char *f, const char *m);
		string get_message();
};
