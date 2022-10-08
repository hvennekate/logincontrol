QT -= gui
QT += dbus sql

CONFIG += c++20 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        dbutils.cpp \
        main.cpp \
        session.cpp \
        sessionmanager.cpp \
        sessiontimebudget.cpp \
        sessiontimelogger.cpp \
        timeslotsfortoday.cpp \
        userlist.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    dbutils.h \
    session.h \
    sessionmanager.h \
    sessiontimebudget.h \
    sessiontimelogger.h \
    timeslotsfortoday.h \
    userlist.h

DBUS_ADAPTORS += ../org.hendrik.logintimer.xml
