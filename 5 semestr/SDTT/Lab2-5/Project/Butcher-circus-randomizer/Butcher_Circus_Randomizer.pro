QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17



SOURCES += \
    main.cpp \
    mainwindow.cpp \
    sec_func.cpp

HEADERS += \
    Random.h \
    mainwindow.h \
    sec_func.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    Butcher_Circus_Randomizer_ru_RU.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
