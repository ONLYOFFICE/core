TARGET = kernel_network
TEMPLATE = lib

CONFIG -= qt
QT -= core gui

CONFIG += shared
CONFIG += plugin

CORE_ROOT_DIR = $$PWD/../..
PWD_ROOT_DIR = $$PWD
include(../base.pri)

DEFINES += KERNEL_USE_DYNAMIC_LIBRARY_BUILDING

support_web_socket {
    # WEBSOCKET
    include(./WebSocket/websocket.pri)
}

# DOWNLOAD/UPLOAD
include(./FileTransporter/filetransporter.pri)

ADD_DEPENDENCY(kernel)
