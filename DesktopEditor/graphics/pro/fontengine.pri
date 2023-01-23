include(freetype.pri)

FONT_ENGINE_PATH = $$PWD/../../fontengine

HEADERS += \
    $$PWD/Fonts.h \
	$$FONT_ENGINE_PATH/ApplicationFonts.h \
	$$FONT_ENGINE_PATH/FontFile.h \
	$$FONT_ENGINE_PATH/FontPath.h \
	$$FONT_ENGINE_PATH/GlyphString.h \
	$$FONT_ENGINE_PATH/FontManager.h \
	$$FONT_ENGINE_PATH/FontConverter.h

SOURCES += \
    $$FONT_ENGINE_PATH/ApplicationFonts.cpp \
	$$FONT_ENGINE_PATH/FontFile.cpp \
	$$FONT_ENGINE_PATH/FontManager.cpp \
	$$FONT_ENGINE_PATH/FontPath.cpp \
	$$FONT_ENGINE_PATH/GlyphString.cpp

# Application fonts worker
HEADERS += $$FONT_ENGINE_PATH/ApplicationFontsWorker.h
SOURCES += $$FONT_ENGINE_PATH/ApplicationFontsWorker.cpp

# deprecated!!! remove after 6.5 ------------------
HEADERS += $$FONT_ENGINE_PATH/FontsAssistant.h
SOURCES += $$FONT_ENGINE_PATH/FontsAssistant.cpp
# -------------------------------------------------

# ------------------- shaper ----------------------
HEADERS += $$FONT_ENGINE_PATH/TextShaper.h
SOURCES += $$FONT_ENGINE_PATH/TextShaper.cpp

include($$PWD/textshaper.pri)

enable_support_shaper {
    include($$PWD/../../../Common/3dParty/harfbuzz/harfbuzz.pri)
}
# -------------------------------------------------

core_ios {

    OBJECTIVE_SOURCES += \
	    $$FONT_ENGINE_PATH/ApplicationFonts_ios.mm \

    LIBS += -framework Foundation
}

SOURCES += $$PWD/pro_Fonts.cpp

support_font_converter {
SOURCES += \
    $$FONT_ENGINE_PATH/fontconverter/StringExt.cpp \
	$$FONT_ENGINE_PATH/fontconverter/Hash.cpp \
	$$FONT_ENGINE_PATH/fontconverter/FontConverter.cpp \
	$$FONT_ENGINE_PATH/fontconverter/FontFileEncodings.cpp \
	$$FONT_ENGINE_PATH/fontconverter/FontFileTrueType.cpp \
	$$FONT_ENGINE_PATH/fontconverter/FontFileType1.cpp \
	$$FONT_ENGINE_PATH/fontconverter/FontFileType1C.cpp
}
