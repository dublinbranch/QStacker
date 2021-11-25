#ifndef HTTPEXCEPTION_H
#define HTTPEXCEPTION_H

#include "exceptionv2.h"
#include <QString>

class HttpException : public ExceptionV2 {
	  public:
	HttpException(QString e);
	HttpException(std::string e);
	HttpException(const char* e);
};

#endif // HTTPEXCEPTION_H
