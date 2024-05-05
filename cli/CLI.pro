QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.c \
        srcs/cli.c \
        srcs/controller.c \
        srcs/gas_cam_cli.c \
        srcs/message.c

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    headers/Queue.h \
    headers/capture.h \
    headers/cli.h \
    headers/controller.h \
    headers/encode.h \
    headers/functions_tools.h \
    headers/gas_cam_cli.h \
    headers/gas_cammera.h \
    headers/message.h \
    headers/rgb_converter.h \
    headers/save_snapshot.h \
    headers/stage_lib.h \
    headers/write_record.h \
    headers/yuv_converter.h

DISTFILES += \
    CLI.pro.user
