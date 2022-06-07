#include "exceptionv2.h"
#include "fileFunction/sourcelocation.h"
#include "qstacker.h"
#include <QString>
#include <cxxabi.h>

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
	return e;
}

ExceptionV2 ExceptionV2::location(const std::string& _msg, const std::source_location location) {
	ExceptionV2 e;
	e.setMsg(_msg + " in " + locationFull(location));
	return e;
}

ExceptionV2 ExceptionV2::location(const QString& _msg, const std::source_location location) {
	ExceptionV2 e;
	e.setMsg(_msg.toStdString() + " in " + locationFull(location));
	return e;
}

/// We must mark this function to be skipped by lib asan as we do this unsafe operation
/// const auto* v2 = static_cast<ExceptionV2*>(thrown_exception);
/// if (v2->canaryKey == ExceptionV2::uukey) {
/// which is in case of a non ExceptionV2 and out of bound access!
/// https://clang.llvm.org/docs/AddressSanitizer.html#issue-suppression
__attribute__((no_sanitize("address"))) bool ExceptionV2::isExceptionV2Derived(void* ptr) {
	const auto* v2 = static_cast<ExceptionV2*>(ptr);
	return v2->canaryKey == ExceptionV2::uukey;
}

const QString& ExceptionV2::getLogFile() const noexcept {
	return logFile;
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

const char* currentExceptionTypeName() {
	int status;
	return abi::__cxa_demangle(abi::__cxa_current_exception_type()->name(), 0, 0, &status);
}
