#ifndef _MAIN_H_
#define _MAIN_H_

#include <string>
//#include <curl/curl.h>

#define IMAGE_PATH "~/usbdata"

#include "opencv2/opencv.hpp"

//boost 
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/regex.hpp>

#include <boost/program_options.hpp>
#include <boost/program_options/options_description.hpp>

#include <boost/range/iterator_range.hpp>
#include <boost/system/error_code.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/iostreams/categories.hpp>  // sink_tag

#include <X11/Xlib.h>

typedef struct _Source
{
	std::string str_path ;
	cv::Mat image ;
} Source;

#endif

