CONFIG += object_parallel_to_source

SOURCES += \
    $$PWD/asstring.cpp \
	$$PWD/exceptionv2.cpp \
	$$PWD/httpexception.cpp \
    $$PWD/qstacker.cpp 
    
HEADERS += \
    $$PWD/asstring.h \
    $$PWD/backward.hpp \
    $$PWD/exceptionv2.h \
    $$PWD/httpexception.h \
    $$PWD/qstacker.h 

DISTFILES += \
	$$PWD/README.md 
	
LIBS += -ldw
LIBS += -ldl
