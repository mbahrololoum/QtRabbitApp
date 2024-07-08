#-------------------------------------------------
#
# Project created by QtCreator 2017-11-15T04:24:56
#
#-------------------------------------------------

QT       += core gui svg
#QTPLUGIN += qandroid

#CONFIG += c++17

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QTRabbitMQApp
#TEMPLATE = app


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

message("------------- Qt ARCHITECTER = " + $$QT_ARCH)
message("------------- Ot MAJ VERSION = " + $$QT_MAJOR_VERSION)
message("------------- Ot VERSION     = " + $$QT_VERSION)
message("------------- OS             = " + $$QMAKE_HOST.os)
message("------------- CPU ARCH       = " + $$QMAKE_HOST.arch)
message("------------- CPU COUNT      = " + $$QMAKE_HOST.cpu_count)
message("------------- HOST NAME      = " + $$QMAKE_HOST.name)
message("------------- VERSION        = " + $$QMAKE_HOST.version)
message("------------- OS VERSION DET = " + $$QMAKE_HOST.version_string)
message("--------------------------------------------------------------------")

win32 {
    contains(QT_MAJOR_VERSION, 5): {
        message("------------- Qt 5 WIN OS")
        *-g++* {
            message("------------- MINGW")
            LIBS += -L$$PWD/lib/qt5/win/mingw/ -lrabbitmq
        }

        *-msvc* {
            message("------------- MSVC")
            LIBS += -L$$PWD/lib/qt5/win/msvc/ -lrabbitmq
        }
    }
    contains(QT_MAJOR_VERSION, 6): {
        message("------------- Qt 6 WIN OS")
        *-g++* {
            message("------------- MINGW")
            LIBS += -L$$PWD/lib/qt6/win/mingw/ -lrabbitmq
        }

        *-msvc* {
            message("------------- MSVC")
            LIBS += -L$$PWD/lib/qt6/win/msvc/ -lrabbitmq
        }
    }
}

mac {
    contains(QT_MAJOR_VERSION, 5): {
        # message("------------- Qt 5 MAC OS")
         LIBS += -L$$PWD/lib/qt5/mac/ -lrabbitmq
    }

    contains(QT_MAJOR_VERSION, 6): {
        # message("------------- Qt 6 MAC OS")
         LIBS += -L$$PWD/lib/qt6/mac/ -lrabbitmq
    }
}

linux-g++{
    message("------------- LINUX OS")

    contains(QT_ARCH, x86_64): {
        message("---OK---")
        LIBS += -L$$PWD/lib/qt5/linux/ -lrabbitmq
    }
}

android{

    contains(QT_MAJOR_VERSION, 5): {
        contains(QT_ARCH, armeabi-v7a ): {
            message("------------- Qt 5 ANDROID OS v7")
            LIBS += -L$$PWD/lib/qt5/android/armv7/ -lrabbitmq
        }

        contains(QT_ARCH, arm64-v8a): {
            message("------------- Qt 5 ANDROID OS v8")
            LIBS += -L$$PWD/lib/qt5/android/armv8/ -lrabbitmq
        }
    }

    contains(QT_MAJOR_VERSION, 6): {

        contains(QT_ARCH, armeabi-v7a ): {
            message("------------- Qt 6 ANDROID OS v7")
            LIBS += -L$$PWD/lib/qt6/android/armv7/ -lrabbitmq
        }

        contains(QT_ARCH, arm64-v8a): {
            message("------------- Qt 6 ANDROID OS v8")
            LIBS += -L$$PWD/lib/qt6/android/armv8/ -lrabbitmq
        }
    }
}

ios{
    contains(QT_MAJOR_VERSION, 5): {
        message("------------- Qt 5 IOS")
        LIBS += -L$$PWD/lib/qt5/ios/ -lrabbitmq
    }

    contains(QT_MAJOR_VERSION, 6): {
        message("------------- Qt 6 IOS")
        LIBS += -L$$PWD/lib/qt6/ios/ -lrabbitmq
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
android: include(/Users/masoudbahrololoum/Library/Android/sdk/android_openssl/openssl.pri)
