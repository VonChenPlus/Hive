!contains(CONFIG, staticlib) {
        LIBS += -ldl
        mac:CONFIG(release, debug|release): LIBS += $$P/Native/release/Native.lib
        else:mac:CONFIG(debug, debug|release): LIBS += $$P/Native/debug/Native.lib
}
