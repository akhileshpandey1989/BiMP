#include<iostream>
#include<fstream>
#include<string>
#include<cstdlib>
#include<cstring>
#include "utility_functions.h"
#include "BMP_Header.h"
#include "exception.h"

using namespace std;

BMP_Header::BMP_Header()
{
}

BMP_Header::BMP_Header(const char* filename)
{
	int temp_file_size;
	try
	{
		ifstream myfile;
		myfile.open(filename, ios::in | ios::binary);
		if(!myfile.is_open())
		{
			throw new Exception(filename, "File open failed.");
		}
		myfile.seekg(0, ios::end);
		if((temp_file_size=myfile.tellg())<14)
		{
			throw new Exception(filename, "File size less than 14 bytes. File corrupt");
		}
		myfile.seekg(0, ios::beg);
		myfile.read(header, sizeof(header));
		myfile.close();
		if(memcmp(header, "BM", 2)!=0)
		{
			throw new Exception(filename, "File not a BMP File. BMP Header doesn't begin with BM.");
		}
		file_size=get_value_reverse(header, 2, 4);
		if(file_size!=temp_file_size)
		{
			throw new Exception(filename, "Filesize on disk and filesize reported in BMP Header doesn't match.");
		}
		pixel_array_offset=get_value_reverse(header, 10, 4);
	}
	catch(Exception *e)
	{
		cout<<e->get_message();
		exit(0);
	}
}

int BMP_Header::get_file_size()
{
	return file_size;
}

void BMP_Header::set_file_size(int fs)
{
	file_size=fs;
	set_value_reverse(header, 2, 4, fs);
}

int BMP_Header::get_pixel_array_offset()
{
	return pixel_array_offset;
}

void BMP_Header::set_pixel_array_offset(int pao)
{
	pixel_array_offset=pao;
	set_value_reverse(header, 10, 4, pao);
}

void BMP_Header::copy_BMP_Header(char *destination)
{
	memcpy(destination, header, sizeof(header));
}

void BMP_Header::display_BMP_Header()
{
	cout<<"BMP Image file size="<<file_size<<" bytes"<<endl;
	cout<<"Pixel Array begins at "<<pixel_array_offset<<" byte"<<endl;
}

