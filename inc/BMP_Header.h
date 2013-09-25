class BMP_Header
{
	char header[14];
	int file_size;
	int pixel_array_offset;
	public: BMP_Header();
		BMP_Header(const char* inp_file);
		int get_file_size();
		void set_file_size(int fs);
		int get_pixel_array_offset();
		void set_pixel_array_offset(int pao);
		void copy_BMP_Header(char *destination);
		void display_BMP_Header();
};
