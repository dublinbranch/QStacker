#include "httpexception.h"

HttpException::HttpException(QString e) {
	msg = e.toUtf8();
}

HttpException::HttpException(std::string e) {
	msg = QByteArray::fromStdString(e);
}
