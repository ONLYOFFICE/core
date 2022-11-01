BASE_DIR = $$PWD/../..

HEADERS += \
    $$BASE_DIR/Base/Base.h \
	$$BASE_DIR/Base/codecvt.h \
	$$BASE_DIR/Base/DateTime.h \
	$$BASE_DIR/Base/SmartPrt.h \ # TODO: std::shared_ptr
	$$BASE_DIR/Base/Nullable.h \
	$$BASE_DIR/Base/strings_hack_printf.h \
	$$BASE_DIR/Base/unicode_util.h \
	$$BASE_DIR/Base/Unit.h \
	$$BASE_DIR/Base/WinColor.h \
	\
	$$BASE_DIR/XML/XmlSimple.h \
	\
	$$BASE_DIR/SystemUtility/File.h \
	$$BASE_DIR/SystemUtility/SystemUtility.h

SOURCES += \
    $$BASE_DIR/Base/codecvt.cpp \
	$$BASE_DIR/Base/DateTime.cpp \
	$$BASE_DIR/Base/unicode_util.cpp \
	$$BASE_DIR/Base/Unit.cpp \
	$$BASE_DIR/Base/WinColor.cpp \
	\
	$$BASE_DIR/XML/XmlSimple.cpp \
	\
	$$BASE_DIR/SystemUtility/File.cpp \
	$$BASE_DIR/SystemUtility/SystemUtility.cpp

SOURCES += \
    $$BASE_DIR/../../3dParty/pole/pole.cpp

HEADERS += \
    $$BASE_DIR/Common/ComplexTypes.h \
	$$BASE_DIR/Common/SimpleTypes_Base.h \
	$$BASE_DIR/Common/SimpleTypes_OMath.h \
	$$BASE_DIR/Common/SimpleTypes_Drawing.h \
	$$BASE_DIR/Common/SimpleTypes_Vml.h \
	$$BASE_DIR/Common/SimpleTypes_Shared.h \
	$$BASE_DIR/Common/SimpleTypes_Word.h

SOURCES += \
    $$BASE_DIR/Common/SimpleTypes_Base.cpp \
	$$BASE_DIR/Common/SimpleTypes_OMath.cpp \
	$$BASE_DIR/Common/SimpleTypes_Drawing.cpp \
	$$BASE_DIR/Common/SimpleTypes_Vml.cpp \
	$$BASE_DIR/Common/SimpleTypes_Shared.cpp \
	$$BASE_DIR/Common/SimpleTypes_Word.cpp

