#include<iostream>
#include<new>
#include<cstring>
#include<fstream>
#include<cstdlib>
#include<cmath>
#include "BMP_File.h"
#include "utility_functions.h"
#include "exception.h"

using namespace std;

BMP_File::BMP_File():BMP_Header(), DIB_Header()
{
	cout<<"Useless constructor. Use BMP_File(\"filename.bmp\")"<<endl;
}

BMP_File::BMP_File(const char *filename) :BMP_Header(filename), DIB_Header(filename)
{
	try
	{
		int temp_pixel_array_size=get_pixel_array_size();
		int temp_pixel_array_offset=get_pixel_array_offset();

		pixel_array=new char[temp_pixel_array_size];

		ifstream myfile;
		myfile.open(filename, ios::in | ios::binary);
		if(!myfile.is_open())
		{
			throw new Exception(filename, "File open failed.");
		}
		myfile.seekg(temp_pixel_array_offset, ios::beg);
		myfile.read(pixel_array, temp_pixel_array_size); 
		myfile.close();
	}
	catch(Exception *e)
	{
		cout<<e->get_message()<<endl;
		exit(0);
	}
	catch(std::bad_alloc)
	{
		cout<<"new failed to get memory to store pixel array."<<endl;
		exit(1);
	}
}

void BMP_File::grayscale()
{
	int i, j;
	int temp_image_width=get_image_width();
	int temp_image_height=get_image_height();
	int bytes_per_pixel=get_bits_per_pixel()/8;
	int bytes_read=0;
	for(i=0; i<temp_image_height; i++)
	{
		for(j=0; j<temp_image_width; j++)
		{
			pixel_array[bytes_read]=pixel_array[bytes_read+1];
			pixel_array[bytes_read+2]=pixel_array[bytes_read+1];
			bytes_read=bytes_read+bytes_per_pixel;
		}
		if(bytes_read%4!=0)
		{
			bytes_read=bytes_read+(4-(bytes_read)%4);
		}
	}
}

void BMP_File::sepia()
{
	int i, j;
	int temp_image_width=get_image_width();
	int temp_image_height=get_image_height();
	int bytes_per_pixel=get_bits_per_pixel()/8;
	int bytes_read=0;
	float intensity;
	for(i=0; i<temp_image_height; i++)
	{
		for(j=0; j<temp_image_width; j++)
		{
			intensity=pixel_array[bytes_read]*0.11+pixel_array[bytes_read+1]*0.59+pixel_array[bytes_read+2]*0.30;
			pixel_array[bytes_read]=char(intensity*0.41);
			pixel_array[bytes_read+1]=char(intensity*0.71);
			pixel_array[bytes_read+2]=char(intensity);
			bytes_read=bytes_read+bytes_per_pixel;
		}
		if(bytes_read%4!=0)
		{
			bytes_read=bytes_read+(4-(bytes_read)%4);
		}

	}
}

void BMP_File::rotate(int rotation_type)
{
	char *new_pixel_array;
	int bytes_per_pixel=get_bits_per_pixel()/8;
	int original_image_height=get_image_height();
	int original_image_width=get_image_width();
	int height_bytes=original_image_height*bytes_per_pixel;
	int width_bytes=original_image_width*bytes_per_pixel;
	int new_pixel_array_size;
	int new_image_height;
	int new_image_width;
	int required_padding;
	int removed_padding;
	int i;
	int j;
	if(height_bytes%4==0)
	{
		height_bytes=0;
	}
	else
	{
		height_bytes=4-(height_bytes)%4;
	}
	required_padding=height_bytes;
	height_bytes=height_bytes*original_image_width;
	if(width_bytes%4==0)
	{
		width_bytes=0;
	}
	else
	{
		width_bytes=4-(width_bytes)%4;
	}
	removed_padding=width_bytes;

	new_pixel_array_size=original_image_height*original_image_width*bytes_per_pixel+height_bytes;
	new_image_height=original_image_width;
	new_image_width=original_image_height;

	new_pixel_array=new char[new_pixel_array_size];

	set_image_height(new_image_height);
	set_image_width(new_image_width);
	set_pixel_array_size(new_pixel_array_size);
	set_file_size(get_header_size()+new_pixel_array_size+14);

	for(i=0; i<original_image_height; i++)
	{
		for(j=0; j<original_image_width; j++)
		{
			memcpy(new_pixel_array+i*bytes_per_pixel+(new_image_width*bytes_per_pixel+required_padding)*(original_image_width-j-1), pixel_array+(original_image_width*bytes_per_pixel+removed_padding)*i+j*bytes_per_pixel, bytes_per_pixel); 

		}
	}
	delete [] pixel_array;
	pixel_array=new_pixel_array;
}

void BMP_File::mirror(int mirror_type)
{
	int i, j;
	int temp_image_height=get_image_height();
	int temp_image_width=get_image_width();
	int bytes_per_pixel=get_bits_per_pixel()/8;
	int bytes_read=0;
	int offset;
	int padding_per_row;
	char *buffer=NULL;
	if(mirror_type==0)
	{
		for(i=0; i<temp_image_height; i++)
		{
			offset=bytes_read;
			reverse(pixel_array+offset, temp_image_width*bytes_per_pixel);
			bytes_read=bytes_read+temp_image_width*bytes_per_pixel;
			for(j=0; j<temp_image_width; j++) 
			{
				reverse(pixel_array+offset+j*bytes_per_pixel, bytes_per_pixel);
			}
			if(bytes_read%4!=0)
			{
				bytes_read=bytes_read+(4-(bytes_read)%4);
			}
		}
	}
	else
	{
		padding_per_row=temp_image_width*bytes_per_pixel;
		buffer=new char[bytes_per_pixel];
		if(padding_per_row%4==0)
		{
			padding_per_row=0;
		}
		else
		{
			padding_per_row=padding_per_row+(4-(padding_per_row)%4);
		}
		for(i=0; i<temp_image_width; i++)
		{
			for(j=0; j<temp_image_height/2; j++)
			{
				memcpy(buffer, pixel_array+i*bytes_per_pixel+j*(temp_image_width*bytes_per_pixel+padding_per_row), bytes_per_pixel);
				memcpy(pixel_array+i*bytes_per_pixel+j*(temp_image_width*bytes_per_pixel+padding_per_row), pixel_array+i*bytes_per_pixel+(temp_image_height-j-1)*(temp_image_width*bytes_per_pixel+padding_per_row), bytes_per_pixel);
				memcpy(pixel_array+i*bytes_per_pixel+(temp_image_height-j-1)*(temp_image_width*bytes_per_pixel+padding_per_row), buffer, bytes_per_pixel);
			}
		}
	}
}

void BMP_File::border(int border_size, int red, int green, int blue)
{
	try
	{
		if(border_size<=0)
			return;
		char *new_pixel_array=NULL;
		int padding_required_per_row=0;
		int original_image_width=get_image_width();
		int original_image_height=get_image_height();
		int new_image_width=original_image_width+2*border_size;
		int new_image_height=original_image_height+2*border_size;
		int bytes_per_pixel=get_bits_per_pixel()/8;
		int bytes_written=0;
		int bytes_read=0;
		int i, j, k;
		if((new_image_width*bytes_per_pixel)%4!=0)
		{
			padding_required_per_row=4-(new_image_width*bytes_per_pixel)%4;
		}
		int new_pixel_array_size=(new_image_width*bytes_per_pixel+padding_required_per_row)*new_image_height;

		new_pixel_array=new char[new_pixel_array_size];

		set_image_width(new_image_width);
		set_image_height(new_image_height);
		set_file_size(get_header_size()+new_pixel_array_size+14);
		set_pixel_array_size(new_pixel_array_size);

		for(k=0; k<border_size; k++)
		{
			for(j=0; j<new_image_width; j++)
			{
				new_pixel_array[bytes_written]=blue;
				new_pixel_array[bytes_written+1]=green;
				new_pixel_array[bytes_written+2]=red;
				bytes_written=bytes_written+bytes_per_pixel;
			}
			if(bytes_written%4!=0)
			{
				bytes_written=bytes_written+(4-bytes_written%4);
			}
		}

		for(i=0; i<original_image_height; i++)
		{
			for(k=0; k<border_size; k++)
			{
				new_pixel_array[bytes_written]=blue;
				new_pixel_array[bytes_written+1]=green;
				new_pixel_array[bytes_written+2]=red;
				bytes_written=bytes_written+bytes_per_pixel;
			}
			for(j=0; j<original_image_width; j++)
			{
				for(k=0; k<bytes_per_pixel; k++)
				{
					new_pixel_array[bytes_written+k]=pixel_array[bytes_read+k];
				}
				bytes_written=bytes_written+bytes_per_pixel;
				bytes_read=bytes_read+bytes_per_pixel;
			}
			for(k=0; k<border_size; k++)
			{
				new_pixel_array[bytes_written]=blue;
				new_pixel_array[bytes_written+1]=green;
				new_pixel_array[bytes_written+2]=red;
				bytes_written=bytes_written+bytes_per_pixel;
			}
			if(bytes_read%4!=0)
			{
				bytes_read=bytes_read+(4-bytes_read%4);
			}
			if(bytes_written%4!=0)
			{
				bytes_written=bytes_written+(4-bytes_written%4);
			}

		}
		for(k=0; k<border_size; k++)
		{
			for(j=0; j<new_image_width; j++)
			{
				new_pixel_array[bytes_written]=blue;
				new_pixel_array[bytes_written+1]=green;
				new_pixel_array[bytes_written+2]=red;
				bytes_written=bytes_written+bytes_per_pixel;
			}
		}

		delete [] pixel_array;

		pixel_array=new_pixel_array;
	}
	catch(std::bad_alloc)
	{
		cout<<"new failed to get memory."<<endl;
		exit(1);
	}
}

void BMP_File::border(int border_size)
{
	try
	{
		if(border_size<=0)
			return;
		char *new_pixel_array=NULL;
		int i, j, k;
		int new_pixel_array_size;
		int original_image_height=get_image_height();
		int original_image_width=get_image_width();
		int bytes_per_pixel=get_bits_per_pixel()/8;
		if(border_size>=min(original_image_height, original_image_width)/2)
		{
			throw new Exception("Border", "Trying to remove this amount of border will leave the image with impossible dimensions.");
		}
		int new_image_height=original_image_height-2*border_size;
		int new_image_width=original_image_width-2*border_size;
		int bytes_read=0;
		int bytes_written=0;
		int padding_required_per_row=0;
		int original_padding_per_row=0;
		if((new_image_width*bytes_per_pixel)%4!=0)
		{
			padding_required_per_row=4-(new_image_width*bytes_per_pixel)%4;
		}
		if((original_image_width*bytes_per_pixel)%4!=0)
		{
			original_padding_per_row=4-(original_image_width*bytes_per_pixel)%4;
		}

		new_pixel_array_size=(new_image_width*bytes_per_pixel+padding_required_per_row)*new_image_height;

		new_pixel_array=new char[new_pixel_array_size];

		set_image_height(new_image_height);
		set_image_width(new_image_width);
		set_file_size(get_header_size()+new_pixel_array_size+14);
		set_pixel_array_size(new_pixel_array_size);

		bytes_read=border_size*(original_image_width*bytes_per_pixel+original_padding_per_row);

		for(i=0; i<new_image_height; i++)
		{
			bytes_read=bytes_read+border_size*bytes_per_pixel;
			for(j=0; j<new_image_width; j++)
			{
				for(k=0; k<bytes_per_pixel; k++)
				{
					new_pixel_array[bytes_written+k]=pixel_array[bytes_read+k];
				}
				bytes_written=bytes_written+bytes_per_pixel;
				bytes_read=bytes_read+bytes_per_pixel;
			}
			if(bytes_written%4!=0)
			{
				bytes_written=bytes_written+(4-bytes_written%4);
			}
			bytes_read=bytes_read+border_size*bytes_per_pixel;
			if(bytes_read%4!=0)
			{
				bytes_read=bytes_read+(4-bytes_read%4);
			}
		}

		delete [] pixel_array;

		pixel_array=new_pixel_array;
	}
	catch(Exception *e)
	{
		cout<<e->get_message();
		exit(1);
	}
	catch(std::bad_alloc)
	{
		cout<<"new failed to get memory."<<endl;
		exit(1);
	}
}

void BMP_File::negative()
{
	int maxrgb=255;
	int temp_image_height=get_image_height();
	int temp_image_width=get_image_width();
	int i, j;
	int bytes_read=0;
	int bytes_per_pixel=get_bits_per_pixel()/8;
	for(i=0; i<temp_image_height; i++)
	{
		for(j=0; j<temp_image_width; j++)
		{
			pixel_array[bytes_read]=maxrgb-pixel_array[bytes_read];
			pixel_array[bytes_read+1]=maxrgb-pixel_array[bytes_read+1];
			pixel_array[bytes_read+2]=maxrgb-pixel_array[bytes_read+2];
			bytes_read=bytes_read+bytes_per_pixel;
		}
		if(bytes_read%4!=0)
		{
			bytes_read=bytes_read+(4-bytes_read%4);
		}
	}
}

void BMP_File::contrast(int points)
{
	int temp_image_height=get_image_height();
	int temp_image_width=get_image_width();
	int bytes_per_pixel=get_bits_per_pixel()/8;
	int bytes_read=0;
	int i, j;
	float contrst=(100.0+points)/100.0;
	contrst=contrst*contrst;

	for(i=0; i<temp_image_height; i++)
	{
		for(j=0; j<temp_image_width; j++)
		{
			pixel_array[bytes_read]=(((pixel_array[bytes_read]/255.0-0.5)*contrst)+0.5)*255.0;
			if(pixel_array[bytes_read]<0)
			{
				pixel_array[bytes_read]=0;
			}
			else if(pixel_array[bytes_read]>255)
			{
				pixel_array[bytes_read]=255;
			}
			pixel_array[bytes_read+1]=(((pixel_array[bytes_read+1]/255.0-0.5)*contrst)+0.5)*255.0;
			if(pixel_array[bytes_read+1]<0)
			{
				pixel_array[bytes_read+1]=0;
			}
			else if(pixel_array[bytes_read+1]>255)
			{
				pixel_array[bytes_read+1]=255;
			}
			pixel_array[bytes_read+2]=(((pixel_array[bytes_read+2]/255.0-0.5)*contrst)+0.5)*255.0;
			if(pixel_array[bytes_read+2]<0)
			{
				pixel_array[bytes_read+2]=0;
			}
			else if(pixel_array[bytes_read+2]>255)
			{
				pixel_array[bytes_read+2]=255;
			}
			bytes_read=bytes_read+bytes_per_pixel;
		}
		if(bytes_read%4!=0)
		{
			bytes_read=bytes_read+(4-bytes_read%4);
		}
	}
}

void BMP_File::write_to_file(const char *filename)
{
	char header_storage[200];
	ofstream myfile;
	myfile.open(filename, ios::out | ios::binary);
	copy_BMP_Header(header_storage);
	myfile.write(header_storage, 14);
	copy_DIB_Header(header_storage);
	myfile.write(header_storage, get_header_size());
	myfile.write(pixel_array, get_pixel_array_size());
	myfile.close();
}
