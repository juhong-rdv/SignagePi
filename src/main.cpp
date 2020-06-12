#include "main.h"

void CheckSourceData(std::vector<Source>& vec_source, const std::string path, const int rotate, const int width, const int height)
{
	vec_source.clear() ;
	
	//get image file path
	boost::system::error_code c;
	try
	{
		if(boost::filesystem::is_directory(path,c)) 
		{
	        std::cout << IMAGE_PATH << " is a directory containing:\n";

			for (boost::filesystem::directory_iterator end, dir(path); dir != end; dir++) 
			{
				const boost::filesystem::path &this_path = dir->path();
				if (this_path.extension() == ".png" || this_path.extension() == ".PNG" ||
					this_path.extension() == ".jpg" || this_path.extension() == ".JPG" ||
					this_path.extension() == ".bmp" || this_path.extension() == ".BMP" ) 
				{
					//std::cout << this_path << "\n";
					//function1(this_path); // Nothing to free

					Source source ;
					source.str_path = this_path.string() ;
					source.image = cv::imread(source.str_path) ;

					vec_source.push_back(source) ;
				} 
			}
	    }
	}
	catch (const boost::exception& ex) 
	{
	    // error handling
	    std::cerr << boost::diagnostic_information(ex);
	} 

	const int size_image_files = vec_source.size() ;
	
	std::vector<cv::Mat> vec_image(size_image_files) ;
	for( int i=0 ; i<size_image_files ; i++ )
	{
		vec_source[i].image.copyTo(vec_image[i]) ;

		//rotate
		double angle = (double)rotate;

	    // get rotation matrix for rotating the image around its center in pixel coordinates
	    cv::Point2f center((vec_image[i].cols-1)/2.0, (vec_image[i].rows-1)/2.0);
	    cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1.0);
	    // determine bounding rectangle, center not relevant
	    cv::Rect2f bbox = cv::RotatedRect(cv::Point2f(), vec_image[i].size(), angle).boundingRect2f();
	    // adjust transformation matrix
	    rot.at<double>(0,2) += bbox.width/2.0 - vec_image[i].cols/2.0;
	    rot.at<double>(1,2) += bbox.height/2.0 - vec_image[i].rows/2.0;

	    cv::Mat dst;
	    cv::warpAffine(vec_image[i], dst, rot, bbox.size());
		
		//Resize to fit monitor resolution.
		float resize_width_rate = 1.0 ;
		float resize_height_rate = 1.0 ;
		
		resize_width_rate = (float)width / (float)dst.cols ;
		resize_height_rate = (float)height / (float)dst.rows ;

		float resize_rate = std::fmin(resize_width_rate, resize_height_rate) ;

		if( resize_rate < 1.0 )
		{
			cv::resize(dst, vec_image[i], cv::Size(), resize_rate, resize_rate) ;
		}
	}
}

int main(int argc, char** argv)
{
	int i_user_delay = 1000 ; 
	int i_rotate = 90 ;
	std::string str_user_path = IMAGE_PATH ;
		
	//------------------------------------------------------------------------------------
	//Command parser
	if(argc > 0)
    {
        namespace po = boost::program_options;
        po::options_description desc("Options");
        desc.add_options()("delay", po::value<int>()->default_value(0));
		desc.add_options()("rotate", po::value<int>()->default_value(90));
		desc.add_options()("path", po::value<std::string>()->default_value(""));

        po::variables_map vm;
        po::store(po::parse_command_line(argc,argv,desc),vm);
        po::notify(vm);

        std::cout << "delay: '" << boost::any_cast<int>(vm["delay"].value()) << "'\n";
		std::cout << "rotate: '" << boost::any_cast<int>(vm["rotate"].value()) << "'\n";	
		std::cout << "path: '" << boost::any_cast<std::string>(vm["path"].value()) << "'\n";

		//port 
		i_user_delay = boost::any_cast<int>(vm["delay"].value()) ;
		//rotate
		i_rotate = boost::any_cast<int>(vm["rotate"].value()) ;
		//path
		str_user_path = boost::any_cast<std::string>(vm["path"].value()) ;
    }	
	//Command parser
	//------------------------------------------------------------------------------------

	//모니터 해상도
	Display* disp = XOpenDisplay(NULL);
    Screen*  scrn = DefaultScreenOfDisplay(disp);
    const int monitor_width  = scrn->width;
    const int monitor_height = scrn->height;

	printf("Monitor size : %d, %d\n", monitor_width, monitor_height) ;

	cv::Mat display = cv::Mat::zeros(cv::Size(monitor_width, monitor_height), CV_8UC3) ;
	cv::Mat next_image = cv::Mat::zeros(cv::Size(monitor_width, monitor_height), CV_8UC3) ;
	cv::Mat cur_image = cv::Mat::zeros(cv::Size(monitor_width, monitor_height), CV_8UC3) ;
	
	std::vector<Source> vec_source ;

	char key = 0 ;
	int index = 0 ;
	const int size_images = vec_source.size() ;

	cv::namedWindow("image", cv::WND_PROP_FULLSCREEN) ;
	cv::setWindowProperty("image", cv::WND_PROP_FULLSCREEN, cv::WINDOW_FULLSCREEN) ;

	while(1)
	{
		CheckSourceData(vec_source, str_user_path, i_rotate, monitor_width, monitor_height) ;
		
		if( index < size_images )
		{
			//copy
			cv::Rect roi ;
			roi.x = (display.cols - vec_source[index].image.cols)/2 ;
			roi.y = (display.rows - vec_source[index].image.rows)/2 ;
			roi.width = vec_source[index].image.cols ;
			roi.height = vec_source[index].image.rows ;

			display = 0 ;
			vec_source[index].image.copyTo(display(roi)) ;
			
			cv::imshow("image", display);
			
		}
		
		for( int i=0 ; i<i_user_delay ; i++ )
		{
			key = cv::waitKey(1) ;

			if( key == 'q' || key == 'Q' )
			{
				goto exit ;
			}
		}

		int next_index = index + 1 ;
		if( next_index >= size_images )	next_index = 0 ;

		if( index != next_index )
		{
			cv::Rect roi ;
			roi.x = (next_image.cols - vec_source[next_index].image.cols)/2 ;
			roi.y = (next_image.rows - vec_source[next_index].image.rows)/2 ;
			roi.width = vec_source[next_index].image.cols ;
			roi.height = vec_source[next_index].image.rows ;

			next_image = 0 ;
			vec_source[next_index].image.copyTo(next_image(roi)) ;

			roi.x = (cur_image.cols - vec_source[index].image.cols)/2 ;
			roi.y = (cur_image.rows - vec_source[index].image.rows)/2 ;
			roi.width = vec_source[index].image.cols ;
			roi.height = vec_source[index].image.rows ;

			cur_image = 0 ;
			vec_source[index].image.copyTo(cur_image(roi)) ;
			
			for (double alpha = 0; alpha < 1; alpha += 0.05) 
			{
		        cv::Mat out;
		        cv::addWeighted(next_image, alpha, cur_image, 1-alpha, 0, out, -1);
		        imshow("image", out);
		        cv::waitKey(100);
		    }
		}

		index = next_index ;
		//
	}

exit:

	return 0 ;
}
