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
