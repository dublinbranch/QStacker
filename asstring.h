#ifndef ASSTRING_H
#define ASSTRING_H

#include "minMysql/min_mysql.h"
#include <QDebug>
#include <QList>
#include <QString>

template <class T>
QString asString(QList<T> t) {
	QString cry;
	QDebug(&cry) << t;
	return cry;
}

QString asString(sqlRow& row);

#endif // ASSTRING_H
