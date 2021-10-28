#ifndef HTTPEXCEPTION_H
#define HTTPEXCEPTION_H

#include "exceptionv2.h"
#include <QString>

class HttpException : public ExceptionV2 {
	  public:
	// to simulate php die()
	bool die = false;

	HttpException(QString e, bool _die);
	HttpException(std::string e, bool _die);
};

#endif // HTTPEXCEPTION_H
