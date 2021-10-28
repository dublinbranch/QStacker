#include "httpexception.h"

HttpException::HttpException(QString e, bool _die) {
	msg = e.toUtf8();
	die = _die;
}

HttpException::HttpException(std::string e, bool _die) {
	msg = QByteArray::fromStdString(e);
	die = _die;
}
