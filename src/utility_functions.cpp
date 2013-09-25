#include<iostream>
#include<cstring>
#include "utility_functions.h"

using namespace std;

int get_value_reverse(char s[], int start, int number_of_bytes)
{
	char temp[4];
	memset(temp, '\0', sizeof(temp));
	int *p=(int *)temp;
	memcpy(temp, s+start, number_of_bytes);
	if(!is_little_endian())
	{
		reverse(temp, sizeof(temp));
	}
	return *p;

}

void set_value_reverse(char s[], int start, int number_of_bytes, int value)
{
	char temp[4];
	memset(temp, '\0', sizeof(temp));
	memcpy(temp, &value, number_of_bytes);
	if(!is_little_endian())
	{
		reverse(temp, sizeof(temp));
	}
	memcpy(s+start, temp, sizeof(temp));
} 

void reverse(char s[], int len)
{
	char temp;
	for(int i=0; i<len/2; i++)
	{
		temp=s[i];
		s[i]=s[len-i-1];
		s[len-i-1]=temp;
	}
}

bool is_little_endian()
{
	long int a=1;
	bool *bptr=(bool *)&a;
	return *bptr;
}
