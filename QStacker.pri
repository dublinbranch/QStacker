CONFIG += object_parallel_to_source

SOURCES += \
	$$PWD/exceptionv2.cpp \
    $$PWD/qstacker.cpp 
    
HEADERS += \
    $$PWD/backward.hpp \
    $$PWD/exceptionv2.h \
    $$PWD/qstacker.h 

DISTFILES += \
	$$PWD/README.md 
	
LIBS += -ldw
LIBS += -ldl
