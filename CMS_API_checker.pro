QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    apicomparison.cpp \
    cmsmanagement.cpp \
    dbmanager.cpp \
    main.cpp \
    mainwindow.cpp \
    setupfrombackup.cpp \
    setupfrombackupddev.cpp \
    setupfromgitlab.cpp \
    todo.cpp

HEADERS += \
    apicomparison.h \
    cmsmanagement.h \
    dbmanager.h \
    mainwindow.h \
    setupfrombackup.h \
    setupfrombackupddev.h \
    setupfromgitlab.h

FORMS += \
    apicomparison.ui \
    cmsmanagement.ui \
    mainwindow.ui \
    setupfrombackup.ui \
    setupfrombackupddev.ui \
    setupfromgitlab.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    cms.qrc
