#-------------------------------------------------
#
# Project created by QtCreator 2017-11-15T04:24:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QTRabbitMQApp
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    rabbitmqutils.c \
    rabbitmqserver.cpp \
    rabbitmqclient.cpp

HEADERS  += mainwindow.h \
    rabbitmqutils.h \
    rabbitmqserver.h \
    rabbitmqclient.h

FORMS    += mainwindow.ui

INCLUDEPATH += $$PWD/inc/
message("------------- OS ARCHITECTER = " + $$QT_ARCH)
message("------------- OS ARCHITECTER = " + $$QMAKE_HOST.arch)
message("------------- OS ARCHITECTER = " + $$QMAKE_HOST.os)
message("------------- OS ARCHITECTER = " + $$QMAKE_HOST.cpu_count)
message("------------- OS ARCHITECTER = " + $$QMAKE_HOST.name)
message("------------- OS ARCHITECTER = " + $$QMAKE_HOST.version)
message("------------- OS ARCHITECTER = " + $$QMAKE_HOST.version_string)

mac{
    message("------------- MAC OS")

    contains(QT_ARCH, x86_64): {
        message("---OK---")
        LIBS += -L$$PWD/lib/mac/ -lrabbitmq
    }
}

win32 {
    message("------------- WIN OS")

    contains(QT_ARCH, x86_64): {
        message("---OK---")
        LIBS += -L$$PWD/lib/win/ -lrabbitmq
    }
}

linux-g++{
    message("------------- LINUX OS")

    contains(QT_ARCH, x86_64): {
        message("---OK---")
        LIBS += -L$$PWD/lib/linux/ -lrabbitmq
    }
}

android{
    message("------------- ANDROID OS = ")

    contains(QT_ARCH, x86): {
        message("---OK---")
        LIBS += -L$$PWD/lib/x86/ -lrabbitmq
    }
}


DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
