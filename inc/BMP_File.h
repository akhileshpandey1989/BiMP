#include "BMP_Header.h"
#include "DIB_Header.h"

class BMP_File:public BMP_Header, public DIB_Header
{
	char *pixel_array;
	public: BMP_File();
		BMP_File(const char *filename);
		void grayscale();
		void rotate(int rotation_type=0);
		void sepia();
		void write_to_file(const char *filename);
		void mirror(int mirror_type=0);
		void border(int border_size);
		void border(int border_size, int red, int green, int blue);
		void negative();
		void contrast(int points);
};
