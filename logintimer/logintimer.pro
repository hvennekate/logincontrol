QT -= gui
QT += dbus sql

CONFIG += c++20 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        logintimesconfig.cpp \
        main.cpp \
        session.cpp \
        sessionmanager.cpp \
        sessiontimelogger.cpp \
        timerange.cpp \
        weeklytimerange.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    logintimesconfig.h \
    session.h \
    sessionmanager.h \
    sessiontimelogger.h \
    timerange.h \
    weeklytimerange.h

DBUS_ADAPTORS += ../org.hendrik.logintimer.xml
