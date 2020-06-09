#include "main.h"

int main(int argc, char** argv)
{
	int i_user_delay = 1000 ; 
	std::string str_user_path = IMAGE_PATH ;
		
	//------------------------------------------------------------------------------------
	//Command parser
	if(argc > 0)
    {
        namespace po = boost::program_options;
        po::options_description desc("Options");
        desc.add_options()("delay", po::value<int>()->default_value(0));
		desc.add_options()("path", po::value<std::string>()->default_value(""));

        po::variables_map vm;
        po::store(po::parse_command_line(argc,argv,desc),vm);
        po::notify(vm);

        std::cout << "delay: '" << boost::any_cast<int>(vm["delay"].value()) << "'\n";
		std::cout << "path: '" << boost::any_cast<std::string>(vm["path"].value()) << "'\n";

		//port 
		i_user_delay = boost::any_cast<int>(vm["delay"].value()) ;
		//path
		str_user_path = boost::any_cast<std::string>(vm["path"].value()) ;
    }	
	//Command parser
	//------------------------------------------------------------------------------------
	
	std::vector<std::string> vec_image_path ;
	
	//get image file path
	boost::system::error_code c;
	try
	{
		if(boost::filesystem::is_directory(str_user_path,c)) 
		{
	        std::cout << IMAGE_PATH << " is a directory containing:\n";

			for (boost::filesystem::directory_iterator end, dir(str_user_path); dir != end; dir++) 
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

	const int size_image_files = vec_image_path.size() ;
	
	std::vector<cv::Mat> vec_image(size_image_files) ;
	for( int i=0 ; i<size_image_files ; i++ )
	{
		vec_image[i] = cv::imread(vec_image_path[i]) ;
	}

	char key = 0 ;
	int index = 0 ;
	const int size_images = vec_image.size() ;
	
	while(1)
	{
		if( index < size_images )
		{
			cv::imshow("image", vec_image[index])		 ;
			
			key = cv::waitKey(i_user_delay) ;

			if( key == 'q' || key == 'Q' )
			{
				break ;
			}
		}
		
		index++ ;
		if( index >= size_images )	index = 0 ;
	}
		
	return 0 ;
}
