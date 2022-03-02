#include "exceptionv2.h"
#include "fileFunction/sourcelocation.h"
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

ExceptionV2 ExceptionV2::raw(const std::string& _msg) {
	ExceptionV2 e;
	e.setMsg(_msg);
}

ExceptionV2 ExceptionV2::location(const std::string& _msg, const std::source_location location) {
	ExceptionV2 e;
	e.setMsg(_msg + " in " + locationFull(location));
}

ExceptionV2 ExceptionV2::location(const QString& _msg, const std::source_location location) {
	ExceptionV2 e;
	e.setMsg(_msg.toStdString() + " in " + locationFull(location));
}

const char* ExceptionV2::what() const noexcept {
	return msg.constData();
}

void ExceptionV2::setMsg(const QByteArray& newMsg) {
	msg = newMsg;
}

void ExceptionV2::setMsg(const std::string& newMsg) {
	msg = QByteArray::fromStdString(newMsg);
}
