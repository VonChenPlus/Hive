!contains(CONFIG, staticlib) {
        isEmpty(PREFIX) {
                PREFIX = /usr
        }

        # Executable
        LIBS += -ldl
        mac:CONFIG(release, debug|release): LIBS += $$P/Native/release/Native.lib
        else:mac:CONFIG(debug, debug|release): LIBS += $$P/Native/debug/Native.lib
}
