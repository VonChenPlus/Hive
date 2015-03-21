!contains(CONFIG, staticlib) {
	isEmpty(PREFIX) {
		PREFIX = /usr
	}

	# Executable
	LIBS += -ldl -lrt $$CONFIG_DIR/libNative.a
}