INCLUDEPATH += $$P

!contains(CONFIG, staticlib) {
        # Executable
	# Use a fixed base-address under windows
	QMAKE_LFLAGS += /FIXED /BASE:"0x00400000" /DYNAMICBASE:NO
	LIBS += -lwinmm -lws2_32 -lShell32 -lAdvapi32 -lopengl32
        win32:CONFIG(release, debug|release): LIBS += $$P/Honey/release/Honey.lib
        else:win32:CONFIG(debug, debug|release): LIBS += $$P/Honey/debug/Honey.lib
}
