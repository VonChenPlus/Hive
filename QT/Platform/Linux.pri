!contains(CONFIG, staticlib) {
	LIBS += -ldl -lrt $$P/Native/libNative.a
}
