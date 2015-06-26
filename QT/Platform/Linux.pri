!contains(CONFIG, staticlib) {
        isEmpty(PREFIX) {
                PREFIX = /usr
        }

        # Executable
        LIBS += -ldl -lrt $$P/Honey/libHoney.a
}
