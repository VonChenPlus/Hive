!contains(CONFIG, staticlib) {
        isEmpty(PREFIX) {
                PREFIX = /usr
        }

        # Executable
        LIBS += -ldl
        mac:CONFIG(release, debug|release): LIBS += $$P/Honey/release/Honey.lib
        else:mac:CONFIG(debug, debug|release): LIBS += $$P/Honey/debug/Honey.lib
}
