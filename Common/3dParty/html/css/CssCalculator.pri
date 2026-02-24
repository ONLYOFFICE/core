INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

CORE_ROOT_DIR = $$PWD/../../../..

include($$CORE_ROOT_DIR/Common/3dParty/boost/boost.pri)

css_calculator_without_xhtml {
	HEADERS += \
		$$PWD/src/CCssCalculator_Private.h \
		$$PWD/src/CElement.h \
		$$PWD/src/CNode.h \
		$$PWD/src/ConstValues.h \
		$$PWD/src/StaticFunctions.h \
		$$PWD/src/StyleProperties.h \
		$$PWD/src/CUnitMeasureConverter.h

	SOURCES += \
		$$PWD/src/CCssCalculator_Private.cpp \
		$$PWD/src/CElement.cpp \
		$$PWD/src/CNode.cpp \
		$$PWD/src/ConstValues.cpp \
		$$PWD/src/StaticFunctions.cpp \
		$$PWD/src/StyleProperties.cpp \
		$$PWD/src/CUnitMeasureConverter.cpp
} else {
	HEADERS += $$files($$PWD/src/*.h, true)
	SOURCES += $$files($$PWD/src/*.cpp, true)

	DEFINES += CSS_CALCULATOR_WITH_XHTML
}
include($$CORE_ROOT_DIR/Common/3dParty/html/katana.pri)
