QT += testlib
QT += widgets

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    tst_tests.cpp \
    ../Butcher-circus-randomizer/sec_func.cpp

RESOURCES += \
    ../Butcher-circus-randomizer/resources.qrc
