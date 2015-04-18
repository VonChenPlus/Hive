!contains(CONFIG, staticlib) {
	isEmpty(PREFIX) {
		PREFIX = /usr
	}

	# Executable
	LIBS += -ldl -lrt $$P/Native/libNative.a
}