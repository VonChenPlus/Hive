!contains(CONFIG, staticlib) {
        isEmpty(PREFIX) {
                PREFIX = /usr
        }

        # Executable
        LIBS += -ldl -lrt $$P/Honey/libHoney.a
        LIBS += $$P/Honey/EXTERNALS/freetype2/library/linux/libfreetype.a
        LIBS += -lX11 $$P/Honey/EXTERNALS/glfw/library/linux/libglfw3.a -lX11 -lXinerama -lXxf86vm -lgtk-x11-2.0 -lXi -lXrandr -lXcursor -lrt
}
