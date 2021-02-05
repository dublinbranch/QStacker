#include "qstacker.h"
#include "backward.hpp"
#include <QString>
#include <mutex>

#include <QDebug>
#include <QString>
#include <dlfcn.h>
#include <execinfo.h>

std::string stacker(uint skip, QStackerOpt opt) {
	/** For loading from an arbitrary position
	ucontext_t uctx;
	getcontext(&uctx);
	void* error_addr = reinterpret_cast<void*>(uctx.uc_mcontext.gregs[REG_RIP]);
	st.load_from(error_addr, 32);
	*/
	using namespace backward;
	static std::mutex            mu;
	std::unique_lock<std::mutex> lock(mu, std::try_to_lock);
	if (!lock.owns_lock()) {
		//too much concurret request to stacker ? something bad is happening!!!
		return "stacker already in use!";
	}

	StackTrace st;
	st.load_here(99);
	st.skip_n_firsts(skip); //skip internal lib stuff

	Printer p;
	p.snippet = opt.snippet;
	p.object  = opt.object;
	p.address = opt.address;

	std::ostringstream stream;
	p.print(st, stream);
	std::string str = "\n" + stream.str();

	//Remove all the stuff before our process (if set)
	if (!StackerMinLevel.empty()) {

		auto start = str.find(StackerMinLevel);
		if (start == std::string::npos) {
			return str;
		}
		//TODO search properly, the path can be different -.-
		//we are pre pended by        '#2    Source "../' = 17
		start = start - 17;
		if (opt.prependReturn) {
			str = "\n" + str.substr(start);
		} else {
			str = str.substr(start);
		}
	}
	return str;
}

QByteArray QStacker(uint skip, QStackerOpt opt) {
	return QByteArray::fromStdString(stacker(skip, opt));
}
QString QStacker16(uint skip, QStackerOpt opt) {
	return QString::fromStdString(stacker(skip, opt));
}

/**
 * @brief qt_assert_x is the internal handle used by qt to report error, sadly is missing the stack trace so 99% of the time 
 * you have ABSOLUTELY no clue where it happened, fixed!
 * @param where
 * @param what
 * @param file
 * @param line
 */
Q_CORE_EXPORT void qt_assert_x(const char* where, const char* what, const char* file, int line) Q_DECL_NOTHROW {
	(void)(where);
	(void)(what);
	(void)(file);
	(void)(line);
	qWarning().noquote() << QStacker16();
	//We can not throw! this function is forcefully declared Q_DECL_NOTHROW and can not be changed, but at least we now know why...
	abort();
}

///** ***************/
///** POWER SUPREME */
///** ***************/

//define the functor
using cxa_throw_type = void(void*, std::type_info*, void (*)(void*));
//now take the address of the REAL __cxa_throw
static cxa_throw_type* original_cxa_throw = (cxa_throw_type*)dlsym(RTLD_NEXT, "__cxa_throw");
extern "C" {
//And NOW override it
void __cxa_throw(void*           thrown_exception,
                 std::type_info* pvtinfo,
                 void (*dest)(void*)) {

	//New (as of 12/2020 way of managing excetion, with ExceptionV2
	//force a cast and look for our token
	const auto* v2 = static_cast<ExceptionV2*>(thrown_exception);
	if (v2->canaryKey == ExceptionV2::uukey) {
		/* Our exception ALWAYS carry the trowing point
		 * The exception point will be printed in case of missed catch
		 * In fact we have to do nothing to properly managed them!
		 */
		original_cxa_throw(thrown_exception, pvtinfo, dest);
	} else {
		if (cxaLevel == CxaLevel::none) {
			//reset after use
			cxaLevel = CxaLevel::critical;
			original_cxa_throw(thrown_exception, pvtinfo, dest);
		}
		static const QString x;
		static const auto    qstringCode = typeid(x).hash_code();

		auto exceptionTypeCode = pvtinfo->hash_code();

		QString msg;
		if (cxaNoStack) {
			cxaNoStack = false;
		} else {
			msg = QStacker16Light(5);
		}

		if (exceptionTypeCode == qstringCode) { //IF QString has been thrown is by us, and usually handled too
			auto th = static_cast<QString*>(thrown_exception);
			msg.prepend(*th);
		} else {

			auto th = static_cast<const char*>(thrown_exception);
			msg.prepend(*th);
		}

		switch (cxaLevel) {
		case CxaLevel::warn:
			qWarning().noquote() << msg;
			break;
		case CxaLevel::debug:
			qDebug().noquote() << msg;
			break;
		case CxaLevel::critical:
			qWarning().noquote() << msg;
			break;
		case CxaLevel::none:
			//none
			break;
		}

		//reset after use
		cxaLevel = CxaLevel::critical;
		//this will pass tru the exception to the original handler so the program will not catch fire after an exception is thrown
		original_cxa_throw(thrown_exception, pvtinfo, dest);
	}
}
}

QString QStacker16Light(uint skip, QStackerOpt opt) {
	return QStacker16(skip, opt);
}

void messanger(const QString& msg, CxaLevel level) {
	switch (level) {
	case CxaLevel::critical:
		qCritical() << msg;
		return;
	case CxaLevel::debug:
		qDebug() << msg;
		return;
	case CxaLevel::warn:
		qWarning() << msg;
		return;
	case CxaLevel::none:
		return;
	}
}
