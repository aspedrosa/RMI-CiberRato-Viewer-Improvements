TEMPLATE = app
CONFIG   += qt warn_off release thread
#CONFIG   += qt warn_on debug thread
DEPENDPATH += Lab

#DEFINES += DEBUG

win32 {
    DEFINES += MicWindows
    LIBS += -lws2_32
}

# Input
HEADERS += \
    crmainparameters.h \
    crqscene.h \
    crqparamhandler.h \
    Comm/crqsimulatorcomm.h \
    Comm/crqrobotcomm.h \
    Comm/crqcommhandler.h \
    Comm/crqdrawhandler.h \
    Lab/crshape.h \
    crqrobotinfo.h \
    crqlabview.h \
    crqdataview.h
SOURCES += \
    crmainparameters.cpp \
    crqparamhandler.cpp \
    main.cpp \
    Lab/crbeacon.cpp \
    Lab/crgrid.cpp \
    Lab/crlab.cpp \
    Lab/crrobot.cpp \
    Lab/crtarget.cpp \
    Lab/crvertice.cpp \
    Lab/crwall.cpp \
    Lab/crshape.cpp \
    Comm/crqsimulatorcomm.cpp \
    Comm/crqrobotcomm.cpp \
    Comm/crqcommhandler.cpp \
    Comm/crqdrawhandler.cpp \
    Comm/crqreplyhandler.cpp \
    crqrobotinfo.cpp \
    crqlabview.cpp \
    crqdataview.cpp \
    crqscene.cpp

QT += network xml

FORMS += \
    crqrobotinfo.ui \
    crqlabview.ui \
    crqdataview.ui

RESOURCES += \
    default.qrc
