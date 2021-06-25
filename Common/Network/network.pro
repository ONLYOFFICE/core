TARGET = network
TEMPLATE = lib

CONFIG -= qt
QT -= core gui
CONFIG += plugin

CORE_ROOT_DIR = $$PWD/../../../core
PWD_ROOT_DIR = $$PWD
include(../base.pri)

DEFINES += KERNEL_USE_DYNAMIC_LIBRARY_BUILDING

# WEBSOCKET
include(./WebSocket/websocket.pri)

# DOWNLOAD/UPLOAD
include(./FileTransporter/filetransporter.pri)

ADD_DEPENDENCY(kernel)
