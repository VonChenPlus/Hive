INCLUDEPATH += $$P

!contains(CONFIG, staticlib) {
	# Executable
	# Use a fixed base-address under windows
	QMAKE_LFLAGS += /FIXED /BASE:"0x00400000" /DYNAMICBASE:NO
	LIBS += -lwinmm -lws2_32 -lShell32 -lAdvapi32
}
