#-------------------------------------------------
#
# Project created by QtCreator 2015-04-05T13:38:45
#
#-------------------------------------------------

QT       += core gui
UI_DIR=./UI
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = hotel
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dialogclientin.cpp \
    dialoginquiry.cpp \
    dialogroomshow.cpp \
    dialogcheck_out.cpp \
    dialoginqury_client_result.cpp \
    dialoginqury_room_result.cpp \
    dialogmanage.cpp \
    dialogloading.cpp \
    log_manage.cpp \
    dialogreserve.cpp \
    dialogbackup.cpp \
    dialoggoods_amount.cpp \
    dialogshow_history_message.cpp \
    dialogstatistics.cpp \
    dialogtimeline.cpp

HEADERS  += mainwindow.h \
    dialogclientin.h \
    dialoginquiry.h \
    hotel.h \
    programcontrol.h \
    dialogroomshow.h \
    dialogcheck_out.h \
    dialogclientin.h \
    dialoginqury_client_result.h \
    dialoginqury_room_result.h \
    dialogmanage.h \
    dialogloading.h \
    dialogreserve.h \
    dialogbackup.h \
    dialoggoods_amount.h \
    log_manage.h \
    dialogshow_history_message.h \
    dialogstatistics.h \
    dialogtimeline.h

FORMS    += \
    dialogclientin.ui \
    dialoginquiry.ui \
    dialogroomshow.ui \
    dialogcheck_out.ui \
    dialoginqury_client_result.ui \
    dialoginqury_room_result.ui \
    dialogmanage.ui \
    dialogloading.ui \
    dialogreserve.ui \
    dialogbackup.ui \
    dialoggoods_amount.ui \
    dialogshow_history_message.ui \
    mainwindow.ui \
    dialogstatistics.ui \
    dialogtimeline.ui

RESOURCES += \
    hotel.qrc
