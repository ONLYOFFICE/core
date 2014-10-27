#-------------------------------------------------
#
# Project created by QtCreator 2014-10-27T18:11:24
#
#-------------------------------------------------

QT       -= core gui

TARGET = agglib
TEMPLATE = lib
CONFIG += staticlib

QMAKE_CXXFLAGS += -std=c++11 -Wall -Wno-ignored-qualifiers -g
DEFINES += UNICODE \
    _UNICODE \
    LINUX \
    _LINUX_QT

INCLUDEPATH += \
    ../include \
    ../include \

SOURCES += \
    ../src/agg_arc.cpp \
    ../src/agg_bezier_arc.cpp \
    ../src/agg_curves.cpp \
    ../src/agg_gsv_text.cpp \
    ../src/agg_image_filters.cpp \
    ../src/agg_line_aa_basics.cpp \
    ../src/agg_line_profile_aa.cpp \
    ../src/agg_rounded_rect.cpp \
    ../src/agg_sqrt_tables.cpp \
    ../src/agg_trans_affine.cpp \
    ../src/agg_trans_double_path.cpp \
    ../src/agg_trans_single_path.cpp \
    ../src/agg_trans_warp_magnifier.cpp \
    ../src/agg_vcgen_bspline.cpp \
    ../src/agg_vcgen_contour.cpp \
    ../src/agg_vcgen_dash.cpp \
    ../src/agg_vcgen_markers_term.cpp \
    ../src/agg_vcgen_smooth_poly1.cpp \
    ../src/agg_vcgen_stroke.cpp \
    ../src/agg_vpgen_clip_polygon.cpp \
    ../src/agg_vpgen_clip_polyline.cpp \
    ../src/agg_vpgen_segmentator.cpp

HEADERS += \
    ../include/agg_vcgen_bspline.h \
    ../include/agg_vcgen_contour.h \
    ../include/agg_vcgen_dash.h \
    ../include/agg_vcgen_markers_term.h \
    ../include/agg_vcgen_smooth_poly1.h \
    ../include/agg_vcgen_stroke.h \
    ../include/agg_vcgen_vertex_sequence.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
