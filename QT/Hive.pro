TARGET = Hive

# Main Qt modules
QT       += core gui opengl

include(Settings.pri)

# To support Sailfish which is stuck on GCC 4.6
linux-g++:system($$QMAKE_CXX --version | grep "4.6."): DEFINES+=override

lessThan(QT_MAJOR_VERSION, 5) {
	macx: error(PPSSPP requires Qt5 for OS X but $$[QT_VERSION] was detected.)
	else:lessThan(QT_MINOR_VERSION, 7): error(PPSSPP requires Qt 4.7 or newer but Qt $$[QT_VERSION] was detected.)
}

# Extra Qt modules
greaterThan(QT_MAJOR_VERSION,4) {
	QT += widgets
	# Most platforms don't have this pre-installed. Especially hard to get on OSX.
	exists($$[QT_INSTALL_HEADERS]/QtSystemInfo) {
		QT += systeminfo
		DEFINES += QT_HAS_SYSTEMINFO
	}
}

# External (platform-dependant) libs

# Main
SOURCES += $$P/QT/mainwindow.cpp \
    $$P/QT/QTMain.cpp \
    $$P/QT/MainUI.cpp \
    $$P/QT/QTTextDrawer.cpp

HEADERS += $$P/QT/mainwindow.h \
    $$P/QT/MainUI.h \
    $$P/QT/QTTextDrawer.h

RESOURCES += \
    $$P/QT/HiveRes.qrc

INCLUDEPATH += $$P/Native
