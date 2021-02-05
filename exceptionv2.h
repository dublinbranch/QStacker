#ifndef EXCEPTIONV2_H
#define EXCEPTIONV2_H

#include <QByteArray>
#include <exception>

class ExceptionV2 : public std::exception {
      public:
	//This is an ugly hack to achieve a weird objective, but is a quite commont techique https://en.wikipedia.org/wiki/Hexspeak
	//We cast the obj and check if start with that to know is ours
	static constexpr ulong uukey     = 0xBADBEEFBADBEEF02;
	const ulong            canaryKey = uukey;

	ExceptionV2() = default;
	ExceptionV2(const QString& _msg, uint skip = 4);
	ExceptionV2(const char* _msg, uint skip = 4);
	ExceptionV2(const std::string& _msg, uint skip = 4);

	const char* what() const noexcept override;

      private:
	QByteArray msg;
};
#endif // EXCEPTIONV2_H
