#-------------------------------------------------
#
# Project created by QtCreator 2015-03-08T11:36:56
#
#-------------------------------------------------

TARGET = Hive

QT       += core gui opengl

symbian: LIBS += -lNative.lib
else: LIBS += -lNative

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
	mobile_platform: QT += sensors
} else:!maemo5:mobile_platform {
	CONFIG += mobility
	MOBILITY += sensors
	symbian: MOBILITY += systeminfo feedback
}

# External (platform-dependant) libs

macx|equals(PLATFORM_NAME, "linux") {
	PRE_TARGETDEPS += $$CONFIG_DIR/libCommon.a $$CONFIG_DIR/libCore.a $$CONFIG_DIR/libGPU.a $$CONFIG_DIR/libNative.a
	CONFIG += link_pkgconfig
	packagesExist(sdl2) {
		DEFINES += SDL
		SOURCES += $$P/SDL/SDLJoystick.cpp
		HEADERS += $$P/SDL/SDLJoystick.h
		PKGCONFIG += sdl2
		macx {
			LIBS += -F/Library/Frameworks -framework SDL
			INCLUDEPATH += /Library/Frameworks/SDL.framework/Versions/A/Headers
		}
	}
}

unix:contains(QT_CONFIG, system-zlib) {
	LIBS += -lz
}

# Qt Multimedia (if SDL is not found)
!contains(DEFINES, SDL) {
	lessThan(QT_MAJOR_VERSION,5):!exists($$[QT_INSTALL_HEADERS]/QtMultimedia) {
		# Fallback to mobility audio
		CONFIG += mobility
		MOBILITY += multimedia
	}
	else: QT += multimedia
}

INCLUDEPATH += $$P $$P/native

SOURCES +=\
        mainwindow.cpp \
    QTMain.cpp

HEADERS  += mainwindow.h

FORMS    +=
