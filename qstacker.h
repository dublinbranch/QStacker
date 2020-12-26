#ifndef QSTACKER_H
#define QSTACKER_H

#include <QByteArray>
struct QStackerOpt {
	bool snippet = true;
	bool object  = true;
	bool address = true;
	//prepend a newline
	bool prependReturn = true;
};
constexpr QStackerOpt QStackerOptLight = {false, false, false};
Q_REQUIRED_RESULT std::string stacker(uint skip = 3, QStackerOpt opt = QStackerOptLight);
Q_REQUIRED_RESULT QByteArray QStacker(uint skip = 4, QStackerOpt opt = QStackerOptLight);
Q_REQUIRED_RESULT QString QStacker16(uint skip = 4, QStackerOpt opt = QStackerOptLight);
Q_REQUIRED_RESULT QString QStacker16Light(uint skip = 5, QStackerOpt opt = QStackerOptLight);
/**
 * @brief StackerMinLevel is used to cut away the initial part of the stack trace
 * execially when running a thread there is a lot of stuff
 * It must be the name of the folder from which the code has been compiled
 */
inline std::string StackerMinLevel;
//Next throw will not append stack trace, reset after use
inline thread_local bool cxaNoStack = false;

enum class CxaLevel {
	//TODO in theory I should tweak ABORT, and always force print here the stacktrace generated inside __cxa
	//lot of food for thought
	none, //this will NOT forcefully print the stack on throw
	warn,
	debug,
	critical
};
//what level shall we use ? reset after use as critical
inline thread_local CxaLevel cxaLevel = CxaLevel::critical;

const ulong uukeyV2 = 0xBADBEEFBADBEEF02;
class ExceptionV2 : public std::exception {
      public:
	//This is an ugly hack to achieve a weird objective, but is a quite commont techique https://en.wikipedia.org/wiki/Hexspeak
	//We cast the obj and check if start with that to know is ours
	const ulong uukey = uukeyV2;

	ExceptionV2(const QString& _msg);
	ExceptionV2(const char* _msg);

	const char* what() const noexcept override;

      private:
	QByteArray msg;
};

#endif // QSTACKER_H
