QT += widgets

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/core/dict_parser.cpp \
    src/core/wrpaths.cpp \
    src/gui/debug_window.cpp \
    src/gui/wrmainwindow.cpp \
    src/gui/wrsettings.cpp \
    src/gui/wrsettingswidget.cpp \
    src/main.cpp

HEADERS += \
    src/core/dict_parser.h \
    src/core/wrdefined.h \
    src/core/wrpaths.h \
    src/gui/debug_window.h \
    src/gui/wrmainwindow.h \
    src/gui/wrsettings.h \
    src/gui/wrsettingswidget.h

TRANSLATIONS += \
    trs/translations.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

######## additional actions
DESTDIR = $$OUT_PWD

win32 {
    # Copy the whole 'dictionaries' folder recursively
    QMAKE_POST_LINK += xcopy /Y /I /E $$shell_path($$PWD/dictionaries) $$shell_path($$DESTDIR\\dictionaries)
}
unix {
    QMAKE_POST_LINK += cp -rf $$PWD/assets $$DESTDIR/
}
