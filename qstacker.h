#ifndef QSTACKER_H
#define QSTACKER_H

#include <QByteArray>

Q_REQUIRED_RESULT std::string stacker(uint skip=3);
Q_REQUIRED_RESULT QByteArray QStacker(uint skip=4);
Q_REQUIRED_RESULT QString QStacker16(uint skip=4);

#endif // QSTACKER_H
