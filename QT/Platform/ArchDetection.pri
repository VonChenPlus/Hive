PLATFORM_ARCH="generic"

# Override some bad defaults

contains(QT_ARCH, ".*86.*")|contains(QMAKE_TARGET.arch, ".*86.*") {
	!win32-msvc*: QMAKE_ALLFLAGS += -msse2

	CONFIG += i86

	contains(QT_ARCH, x86_64)|contains(QMAKE_TARGET.arch, x86_64) {
		PLATFORM_ARCH = x86_64
		DEFINES += _M_X64 _ARCH_64
	}
	else {
                QMAKE_ALLFLAGS += /arch:SSE2
		PLATFORM_ARCH = x86
		DEFINES += _M_IX86 _ARCH_32
	}
} else {
	# Generic
	warning("You are using an untested arch: $${QT_ARCH}. Only x86 and ARM CPUs are supported")
	DEFINES += GENERIC_ARCH
	CONFIG += generic
	PLATFORM_ARCH=$${QT_ARCH}
	DEFINES += _ARCH_32
}
