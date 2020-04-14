#ifndef QSTACKER_H
#define QSTACKER_H

#include <QByteArray>
struct QStackerOpt {
	bool snippet = true;
	bool object  = true;
	bool address = true;
};
constexpr QStackerOpt QStackerOptLight = {false, false, false};
Q_REQUIRED_RESULT std::string stacker(uint skip = 3, QStackerOpt opt = QStackerOpt());
Q_REQUIRED_RESULT QByteArray  QStacker(uint skip = 4, QStackerOpt opt = QStackerOpt());
Q_REQUIRED_RESULT QString     QStacker16(uint skip = 4, QStackerOpt opt = QStackerOpt());
Q_REQUIRED_RESULT QString     QStacker16Light(uint skip = 4, QStackerOpt opt = QStackerOptLight);

#endif // QSTACKER_H
