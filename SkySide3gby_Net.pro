#-------------------------------------------------
#
# Project created by QtCreator 2017-12-19T14:49:11
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += widgets

wasm:CONFIG+=c++11

android:{
QT       += androidextras
}

TARGET = SkySide3gby_Net
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
#QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    pstring.c \
    pconst.c \
    citycmd.c \
    citycmdb.c \
    citycmdc.c \
    citycmdd.c \
    citycmde.c \
    cityedit.c \
    datman.c \
    debug.c \
    FgtCount.c \
    FgtPkAi.c \
    Fight.c \
    FightSub.c \
    fsys.c \
    gamEng.c \
    infdeal.c \
    paccount.c \
    PublicFun.c \
    showface.c \
    strcnt.c \
    tactic.c \
    varma.c \
    main1.c \
    comIn.c \
    comOut.c \
    GameModel.cpp \
    gameview.cpp \
    global.cpp \
    PicConverter.cpp \
    windowinidialog.cpp \
    aboutdialog.cpp \
    keyinidialog.cpp \
    mythread.cpp \
    savechange.cpp \
    tcpclient.cpp \
    tcpdatastruct.c  \
    logindialog.cpp

ios:{
    SOURCES += ios-sources/src/viewRoute.mm
}

HEADERS += \
    mainwindow.h \
    attribute.h \
    comm.h \
    compa.h \
    consdef.h \
    datman.h \
    enghead.h \
    errcode.h \
    extern.h \
    fight.h \
    fsys.h \
    fundec.h \
    fundef.h \
    graph.h \
    order.h \
    paccount.h \
    picture.h \
    resource.h \
    sconst.h \
    sharefun.h \
    sources.h \
    stdsys.h \
    pconst.h \
    pstring.h \
    GameModel.h \
    gameview.h \
    global.h \
    PicConverter.h \
    windowinidialog.h \
    aboutdialog.h \
    keyinidialog.h \
    mythread.h \
    savechange.h \
    tcpclient.h \
    tcpdatastruct.h \
    logindialog.h

ios:{
    HEADERS += ios-sources/src/viewRoute.h
    gamlib.path = /
    gamlib.files += ios-sources/assets/dat.lib
    gamlib.files += ios-sources/assets/res.rlb
    gamlib.files += ios-sources/assets/font.bin
    gamlib.files += ios-sources/assets/startup.ini
    gamlib.files += ios-sources/assets/version.ini
    gamlib.files += ios-sources/assets/whatsnew.txt
    QMAKE_BUNDLE_DATA += gamlib
}

wasm:{
LIBS += --preload-file /home/wstjb/src/SkySide3gby_Net/res@/
#QMAKE_WASM_PTHREAD_POOL_SIZE = 4
#wasm:QMAKE_LFLAGS += -s \"TOTAL_MEMORY=20381696\"
}

FORMS += \
    windowinidialog.ui \
    aboutdialog.ui \
    keyinidialog.ui \
    logindialog.ui

RESOURCES += \
    buttonres.qrc

DISTFILES += \
    android-sources/AndroidManifest.xml \
    android-sources/assets/dat.lib \
    android-sources/assets/font.bin \
    android-sources/assets/res.rlb \
    android-sources/build.gradle \
    android-sources/gradle.properties \
    android-sources/gradle/wrapper/gradle-wrapper.jar \
    android-sources/gradle/wrapper/gradle-wrapper.properties \
    android-sources/gradlew \
    android-sources/gradlew.bat \
    android-sources/libs/commons-net-3.0.1.jar \
    android-sources/libs/support-v4-23.0.0.aar \
    android-sources/res/values/libs.xml \
    android-sources/res/values/styles.xml \
    android-sources/res/menu/activity_main.xml \
    android-sources/res/values/strings.xml \
    android-sources/src/com/skysidestudio/skyside3gby/ConfigurationFile.java \
    android-sources/src/com/skysidestudio/skyside3gby/ContextUtil.java \
    android-sources/src/com/skysidestudio/skyside3gby/CrashHandler.java \
    android-sources/src/com/skysidestudio/skyside3gby/MainActivity.java \
    android-sources/src/com/skysidestudio/skyside3gby/ParseXmlService.java \
    android-sources/src/com/skysidestudio/skyside3gby/Public.java \
    android-sources/src/com/skysidestudio/skyside3gby/UpdateManager.java \
    android-sources/res/layout/softupdate_progress.xml \
    ios-sources/assets/dat.lib \
    ios-sources/assets/font.bin \
    ios-sources/assets/res.rlb \
    ios-sources/assets/startup.ini \
    ios-sources/assets/version.ini \
    ios-sources/assets/whatsnew.txt \
    ios-sources/res/icon-1024.png \
    ios-sources/res/icon-120.png \
    ios-sources/res/icon-152.png \
    ios-sources/res/icon-167.png \
    ios-sources/res/icon-180.png \
    ios-sources/res/icon-20.png \
    ios-sources/res/icon-29.png \
    ios-sources/res/icon-40.png \
    ios-sources/res/icon-58.png \
    ios-sources/res/icon-60.png \
    ios-sources/res/icon-76.png \
    ios-sources/res/icon-80.png \
    ios-sources/res/icon-87.png \
    skyico.rc \
    android-sources/assets/startup.ini \
    android-sources/assets/version.ini \
    android-sources/assets/whatsnew.txt

#LIBS += -lpthread

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android-sources

win32:{
RC_FILE     += skyico.rc
}

ios:{
    LIBS += -framework Foundation -framework UIKit

    OBJECTIVE_SOURCES += \
        ios-sources/src/viewRoute.mm
    QMAKE_LFLAGS    += -framework OpenGLES
    QMAKE_LFLAGS    += -framework GLKit
    QMAKE_LFLAGS    += -framework QuartzCore
    QMAKE_LFLAGS    += -framework CoreImage
    QMAKE_LFLAGS    += -framework CoreGraphics
    QMAKE_LFLAGS    += -framework UIKit
}
