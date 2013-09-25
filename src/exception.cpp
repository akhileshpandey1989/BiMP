#include<string>
#include<cstring>
#include "exception.h"

using namespace std;

Exception::Exception(const char *f, const char *s)
{
message.append(f);
message.append(":");
message.append(s);
}

string Exception::get_message()
{
return message;
}
