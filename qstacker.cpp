#include "qstacker.h"
#include "backward.hpp"
#include <mutex>
std::string stacker(uint skip) {
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
	p.snippet = true;
	p.object  = true;
	p.address = true;

	std::ostringstream stream;
	p.print(st, stream);
	std::string str = stream.str();

	return str;
}

QByteArray QStacker(uint skip) {
	return QByteArray::fromStdString(stacker(skip));
}

///** ***************/
///** POWER SUPREME */
///** ***************/
//#include <dlfcn.h>
//#include <execinfo.h>
////define the functor
//typedef void (*cxa_throw_type)(void*, std::type_info*, void (*)(void*));
////now take the address of the REAL __cxa_throw
//static cxa_throw_type orig_cxa_throw = (cxa_throw_type)dlsym(RTLD_NEXT, "__cxa_throw");
//extern "C" {
////And NOW override it
//void __cxa_throw(void*           thrown_exception,
//                 std::type_info* pvtinfo,
//                 void (*dest)(void*)) {

//    static const QString x;
//    auto                 v1 = pvtinfo->hash_code();
//    auto                 v2 = typeid(x).hash_code();
//    if (v1 == v2) { //IF QString has been thrown is by us, and usually handled too
//        QString* th = static_cast<QString*>(thrown_exception);
//        qDebug().noquote() << *th << QStacker(5);
//    } else {
//        qCritical().noquote() << QStacker();
//    }

//    //this will pass tru the exception to the original handler so the program will not catch fire after an exception is thrown
//    orig_cxa_throw(thrown_exception, pvtinfo, dest);
//}
//}
