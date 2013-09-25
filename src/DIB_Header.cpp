#include<iostream>
#include<cstdlib>
#include<fstream>
#include<cstring>
#include "utility_functions.h"
#include "DIB_Header.h"
#include "exception.h"

using namespace std;

DIB_Header::DIB_Header()
{
}

DIB_Header::DIB_Header(const char *filename)
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
		temp_file_size=myfile.tellg();
		if(temp_file_size<18)
		{
			throw new Exception(filename, "DIB Header incomplete. File corrupt.");
		}
		myfile.seekg(14, ios::beg);
		myfile.read(header, 4);
		header_size=get_value_reverse(header, 0, 4);
		if(temp_file_size<header_size+14)
		{
			throw new Exception(filename, "DIB Header incomplete. File corrupt.");
		}
		myfile.read(header+4, header_size-4);
		myfile.close();
		image_width=get_value_reverse(header, 4, 4);
		image_height=get_value_reverse(header, 8, 4);
		number_of_planes=get_value_reverse(header, 12, 2);
		bits_per_pixel=get_value_reverse(header, 14, 2);
		if(bits_per_pixel<24)
		{
			throw new Exception(filename, "Bits per pixel less than 24. Not supported.");
		}
		compression_method=get_value_reverse(header, 16, 4);
		pixel_array_size=get_value_reverse(header, 20, 4);
		if(temp_file_size<14+header_size+pixel_array_size)
		{
			throw new Exception(filename, "File corrupt.");
		}
		pixel_per_meter_h=get_value_reverse(header, 24, 4);
		pixel_per_meter_v=get_value_reverse(header, 28, 4);
		number_of_colors=get_value_reverse(header, 32, 4);
		number_of_imp_colors=get_value_reverse(header, 36, 4);
	}
	catch(Exception *e)
	{
		cout<<e->get_message();
		exit(0);
	}
}

int DIB_Header::get_header_size()
{
	return header_size;
}

void DIB_Header::set_header_size(int hs)
{
	header_size=hs;
	set_value_reverse(header, 0, 4, hs);
}

int DIB_Header::get_image_width()
{
	return image_width;
}

void DIB_Header::set_image_width(int iw)
{
	image_width=iw;
	set_value_reverse(header, 4, 4, iw);
}

int DIB_Header::get_image_height()
{
	return image_height;
}

void DIB_Header::set_image_height(int ih)
{
	image_height=ih;
	set_value_reverse(header, 8, 4, ih);
}

int DIB_Header::get_number_of_planes()
{
	return number_of_planes;
}

void DIB_Header::set_number_of_planes(int nop)
{
	number_of_planes=nop;
}

int DIB_Header::get_bits_per_pixel()
{
	return bits_per_pixel;
}

void DIB_Header::set_bits_per_pixel(int bpp)
{
	bits_per_pixel=bpp;
}

int DIB_Header::get_compression_method()
{
	return compression_method;
}

void DIB_Header::set_compression_method(int cm)
{
	compression_method=cm;
}

int DIB_Header::get_pixel_array_size()
{
	return pixel_array_size;
}

void DIB_Header::set_pixel_array_size(int pas)
{
	pixel_array_size=pas;
	set_value_reverse(header, 20, 4, pas);
}

int DIB_Header::get_pixel_per_meter_h()
{
	return pixel_per_meter_h;
}

void DIB_Header::set_pixel_per_meter_h(int ppmh)
{
	pixel_per_meter_h=ppmh;
}

int DIB_Header::get_pixel_per_meter_v()
{
	return pixel_per_meter_v;
}

void DIB_Header::set_pixel_per_meter_v(int ppmv)
{
	pixel_per_meter_v=ppmv;
}

int DIB_Header::get_number_of_colors()
{
	return number_of_colors;
}

void DIB_Header::set_number_of_colors(int noc)
{
	number_of_colors=noc;
}

int DIB_Header::get_number_of_imp_colors()
{
	return number_of_imp_colors;
}

void DIB_Header::set_number_of_imp_colors(int noic)
{
	number_of_imp_colors=noic;
}

void DIB_Header::copy_DIB_Header(char *destination)
{
	memcpy(destination, header, header_size);
}

void DIB_Header::display_DIB_Header()
{
	cout<<"DIB Header size="<<header_size<<" bytes"<<endl;
	cout<<"Image width="<<image_width<<" pixels"<<endl;
	cout<<"Image height="<<image_height<<" pixels"<<endl;
	cout<<"Number of color planes="<<number_of_planes<<endl;
	cout<<"Bits per pixel="<<bits_per_pixel<<endl;
	cout<<"Compression Method="<<compression_method<<endl;
	cout<<"Pixel array size="<<pixel_array_size<<" bytes"<<endl;
}

