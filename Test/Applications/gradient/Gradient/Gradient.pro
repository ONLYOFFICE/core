QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target



win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../build/lib/linux_64/release/ -lgraphics
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../build/lib/linux_64/debug/ -lgraphics
else:unix: LIBS += -L$$PWD/../../../../build/lib/linux_64/ -lgraphics

INCLUDEPATH += $$PWD/../../../../DesktopEditor/graphics/pro
DEPENDPATH += $$PWD/../../../../DesktopEditor/graphics/pro


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../build/lib/linux_64/release/ -lkernel
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../build/lib/linux_64/debug/ -lkernel
else:unix: LIBS += -L$$PWD/../../../../build/lib/linux_64/ -lkernel

INCLUDEPATH += $$PWD/../../../../build/lib/linux_64
DEPENDPATH += $$PWD/../../../../build/lib/linux_64

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../build/lib/linux_64/release/ -lUnicodeConverter
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../build/lib/linux_64/debug/ -lUnicodeConverter
else:unix: LIBS += -L$$PWD/../../../../build/lib/linux_64/ -lUnicodeConverter

INCLUDEPATH +=  $$PWD/../../../../UnicodeConverter
DEPENDPATH += $$PWD/../../../../UnicodeConverter

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/./release/ -licuuc
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/./debug/ -licuuc
else:unix: LIBS += -L$$PWD/./ -licuuc

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/./release/ -licudata
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/./debug/ -licudata
else:unix: LIBS += -L$$PWD/./ -licudata

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.
