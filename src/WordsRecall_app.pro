QT += widgets

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    core/dict_parser.cpp \
    core/wrgamedata.cpp \
    core/wrgamelogic.cpp \
    core/wrpaths.cpp \
    core/wrsettings.cpp \
    gui/debug_window.cpp \
    gui/wrgamewidget.cpp \
    gui/wrmainwindow.cpp \
    gui/wrsettingswidget.cpp \
    main.cpp

HEADERS += \
    core/dict_parser.h \
    core/wrdefined.h \
    core/wrgamedata.h \
    core/wrgamelogic.h \
    core/wrpaths.h \
    core/wrsettings.h \
    gui/debug_window.h \
    gui/wrgamewidget.h \
    gui/wrmainwindow.h \
    gui/wrsettingswidget.h

TRANSLATIONS += \
    trs/translations.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

######## additional actions
DESTDIR = $$PWD/../build_exec

win32 {
    # Copy the whole 'dictionaries' folder recursively
    QMAKE_POST_LINK += xcopy /Y /I /E $$shell_path($$PWD/../dictionaries) $$shell_path($$DESTDIR\\dictionaries)
}
unix {
    QMAKE_POST_LINK += cp -rf $$PWD/assets $$DESTDIR/
}


# for debugging qmake
#message("_PWD: $$PWD")
#message("_DESTDIR: $$DESTDIR")
