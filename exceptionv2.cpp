#include "exceptionv2.h"
#include "qstacker.h"
#include <QString>

ExceptionV2::ExceptionV2(const QString& _msg, uint skip) {
	msg = _msg.toUtf8() + QStacker(skip);
}

ExceptionV2::ExceptionV2(const char* _msg, uint skip) {
	msg = _msg + QStacker(skip);
}

ExceptionV2::ExceptionV2(const std::string& _msg, uint skip) {
	msg = QByteArray::fromStdString(_msg) + QStacker(skip);
}

const char* ExceptionV2::what() const noexcept {
	return msg.constData();
}
