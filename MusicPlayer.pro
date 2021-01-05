QT       += core gui \
    quick
QT       += multimedia
QT += sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    database.cpp \
    down_playlist.cpp \
    down_playlistbutton.cpp \
    down_playprogressbar.cpp \
    down_playwidget.cpp \
    down_voicewidget.cpp \
    left_table.cpp \
    lyricloader.cpp \
    lyricwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    middle_musicshow.cpp \
    middle_searchresult.cpp \
    myslider.cpp \
    qssparser.cpp \
    top_searchwidget.cpp

HEADERS += \
    database.h \
    down_playlist.h \
    down_playlistbutton.h \
    down_playprogressbar.h \
    down_playwidget.h \
    down_voicewidget.h \
    left_table.h \
    lyricloader.h \
    lyricwindow.h \
    mainwindow.h \
    middle_musicshow.h \
    middle_searchresult.h \
    myslider.h \
    qssparser.h \
    top_searchwidget.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    images/IconNextSong.ico \
    images/IconNextSong.jpg \
    images/IconPrevSong.ico \
    images/IconPrevSong.jpg \
    images/before.png \
    images/before_normal.png \
    images/before_pressed.png \
    images/mute.jpg \
    images/next.png \
    images/next_normal.png \
    images/next_pressed.png \
    images/pause.jpg \
    images/pause.png \
    images/play.png \
    images/play_pressed.png \


RESOURCES += \
    resources.qrc
