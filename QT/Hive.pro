TARGET = Hive

# Main Qt modules
QT       += core gui opengl
CONFIG -= flat

include(Settings.pri)

# To support Sailfish which is stuck on GCC 4.6
linux-g++:system($$QMAKE_CXX --version | grep "4.6."): DEFINES+=override

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
SOURCES += $$P/QT/*.cpp \
    $$P/RFB/*.cpp

HEADERS += $$P/QT/*.h \
    $$P/RFB/*.h

INCLUDEPATH += $$P/Honey

RC_FILE += Hive.rc

DISTFILES += \
    Hive.rc
