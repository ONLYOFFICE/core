core_windows {

    GLEW_PATH=$$PWD/glew-2.1.0
    INCLUDEPATH += $$GLEW_PATH/include

    core_win_32 {
        LIBS += -L$$GLEW_PATH/bin/Release/Win32
        LIBS += -L$$GLEW_PATH/lib/Release/Win32 -lglew32 -lglew32s
    }
    core_win_64 {
        LIBS += -L$$GLEW_PATH/bin/Release/x64
        LIBS += -L$$GLEW_PATH/lib/Release/x64 -lglew32 -lglew32s
    }

    LIBS += -lwinmm
    LIBS += -ladvapi32
    LIBS += -lglu32
    LIBS += -lopengl32
    LIBS += -lgdi32
    LIBS += -lUser32
}
