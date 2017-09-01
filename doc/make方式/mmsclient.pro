
QT -=gui widget
QT+=network
TEMPLATE = app
TARGET = mmsclientapp

include (../common.pri)

CONFIG += console debug

DESTDIR = ../../bin
OBJECTS_DIR	= ../../obj/debug/mmsclient

INCLUDEPATH += \
	../include\
	../include/iec61850client/mmslite_v60050/inc

LIBS += \
	-L../../lib

LIBS += \
	-liec61850client\
	-lgeneralobject\
	-lmemshare\
	-lipcsocket

unix {
LIBS += \
	-lpthread
}

SOURCES	+=	\
	main.cpp\
	mmsclient.cpp\
	mmsmgr.cpp\
	commobject.cpp\
	mmsdataprocessthread.cpp\
	commobjectmgr.cpp\
	mmsdatahandler.cpp\
	commstateprocessthread.cpp\
	associateobject.cpp\
	associatehost.cpp\
	modelmgr.cpp\
	appcontrolthread.cpp

HEADERS	+= \
	mmsclient.h\
	mmsmgr.h\
	commobject.h\
	mmsdataprocessthread.h\
	commobjectmgr.h\
	commdef.h\
	mmsdatahandler.h\
	commstateprocessthread.h\
	associateobject.h\
	associatehost.h\
	modelmgr.h\
	appcontrolthread.h

#message($$QT)
#error( "This is a message" )
