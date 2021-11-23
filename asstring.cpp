#include "asstring.h"

QString asString(sqlRow& row) {
	QString s;
	QDebug(&s) << row;
	return s;
}
