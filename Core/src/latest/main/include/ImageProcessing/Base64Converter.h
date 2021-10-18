#ifndef BASE64CONVERTER_H
#define BASE64CONVERTER_H

#include "common_headers.h"

class Base64Converter {
 public:
	Base64Converter();

	cv::Mat str2Mat(const std::string &imageBase64);
	std::string mat2Str(const cv::Mat &img);

	virtual ~Base64Converter();
 private:
	std::string base64_encode(uchar const* bytesToEncode, unsigned int inLen);
	std::string base64_decode(std::string const& encodedString);
};

#endif