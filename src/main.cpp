#include "main.h"

int main(void)
{
	std::vector<std::string> vec_image_path ;
	
	//get image file path
	boost::system::error_code c;
	try
	{
		if(boost::filesystem::is_directory(IMAGE_PATH,c)) 
		{
	        std::cout << IMAGE_PATH << " is a directory containing:\n";

			for (boost::filesystem::directory_iterator end, dir(IMAGE_PATH); dir != end; dir++) 
			{
				const boost::filesystem::path &this_path = dir->path();
				if (this_path.extension() == ".png" || this_path.extension() == ".PNG" ||
					this_path.extension() == ".jpg" || this_path.extension() == ".JPG" ||
					this_path.extension() == ".bmp" || this_path.extension() == ".BMP" ) 
				{
					//std::cout << this_path << "\n";
					//function1(this_path); // Nothing to free

					vec_image_path.push_back(this_path.string()) ;
				} 
			}
	    }
	}
	catch (const boost::exception& ex) 
	{
	    // error handling
	    std::cerr << boost::diagnostic_information(ex);
	} 
		
	return 0 ;
}
