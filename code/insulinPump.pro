QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += charts

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    alert.cpp \
    batterymanager.cpp \
    bloodstream.cpp \
    boluscalculator.cpp \
    cgmreader.cpp \
    controliqalgorithm.cpp \
    datalogger.cpp \
    device.cpp \
    history.cpp \
    home.cpp \
    insulinreserve.cpp \
    login.cpp \
    main.cpp \
    profile.cpp \
    pumpcontroller.cpp \
    settings.cpp \
    userinterface.cpp

HEADERS += \
    alert.h \
    batterymanager.h \
    bloodstream.h \
    boluscalculator.h \
    cgmreader.h \
    controliqalgorithm.h \
    datalogger.h \
    device.h \
    history.h \
    home.h \
    insulinreserve.h \
    login.h \
    profile.h \
    pumpcontroller.h \
    settings.h \
    userinterface.h

FORMS += \
    alert.ui \
    boluscalculator.ui \
    device.ui \
    history.ui \
    home.ui \
    login.ui \
    settings.ui \
    userinterface.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
