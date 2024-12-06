QT       -= core gui

VERSION = 0.0.0.1
TARGET = HWPFile
TEMPLATE = lib

CONFIG += shared
CONFIG += plugin

CORE_ROOT_DIR = $$PWD/..
PWD_ROOT_DIR = $$PWD

include($$CORE_ROOT_DIR/Common/base.pri)

ADD_DEPENDENCY(kernel)

SOURCES += \
    HwpDoc/HWPDocInfo.cpp \
    HwpDoc/HWPElements/HWPRecord.cpp \
    HwpDoc/HWPElements/HWPRecordBinData.cpp \
    HwpDoc/HWPElements/HWPRecordBorderFill.cpp \
    HwpDoc/HWPElements/HWPRecordBullet.cpp \
    HwpDoc/HWPElements/HWPRecordCharShape.cpp \
    HwpDoc/HWPElements/HWPRecordCtrlData.cpp \
    HwpDoc/HWPElements/HWPRecordDocumentProperties.cpp \
    HwpDoc/HWPElements/HWPRecordFaceName.cpp \
    HwpDoc/HWPElements/HWPRecordFormObject.cpp \
    HwpDoc/HWPElements/HWPRecordIDMaping.cpp \
    HwpDoc/HWPElements/HWPRecordNumbering.cpp \
    HwpDoc/HWPFile.cpp \
    HwpDoc/HWPStream.cpp \
    HwpDoc/HwpFileHeader.cpp \
    HwpDoc/Paragraph/CapParagraph.cpp \
    HwpDoc/Paragraph/CellParagraph.cpp \
    HwpDoc/Paragraph/CharShape.cpp \
    HwpDoc/Paragraph/Ctrl.cpp \
    HwpDoc/Paragraph/CtrlAutoNumber.cpp \
    HwpDoc/Paragraph/CtrlCharacter.cpp \
    HwpDoc/Paragraph/CtrlClick.cpp \
    HwpDoc/Paragraph/CtrlColumnDef.cpp \
    HwpDoc/Paragraph/CtrlCommon.cpp \
    HwpDoc/Paragraph/CtrlContainer.cpp \
    HwpDoc/Paragraph/CtrlEqEdit.cpp \
    HwpDoc/Paragraph/CtrlForm.cpp \
    HwpDoc/Paragraph/CtrlGeneralShape.cpp \
    HwpDoc/Paragraph/CtrlHeadFoot.cpp \
    HwpDoc/Paragraph/CtrlObjElement.cpp \
    HwpDoc/Paragraph/CtrlPageNumPos.cpp \
    HwpDoc/Paragraph/CtrlShapeArc.cpp \
    HwpDoc/Paragraph/CtrlShapeConnectLine.cpp \
    HwpDoc/Paragraph/CtrlShapeCurve.cpp \
    HwpDoc/Paragraph/CtrlShapeEllipse.cpp \
    HwpDoc/Paragraph/CtrlShapeLine.cpp \
    HwpDoc/Paragraph/CtrlShapeOle.cpp \
    HwpDoc/Paragraph/CtrlShapePic.cpp \
    HwpDoc/Paragraph/CtrlShapePolygon.cpp \
    HwpDoc/Paragraph/CtrlShapeRect.cpp \
    HwpDoc/Paragraph/CtrlShapeTextArt.cpp \
    HwpDoc/Paragraph/CtrlShapeVideo.cpp \
    HwpDoc/Paragraph/CtrlTalbe.cpp \
    HwpDoc/Paragraph/HWPPargraph.cpp \
    HwpDoc/Paragraph/LineSeg.cpp \
    HwpDoc/Paragraph/TblCell.cpp

HEADERS += \
    HwpDoc/Errors.h \
    HwpDoc/HWPDocInfo.h \
    HwpDoc/HWPElements/HWPRecord.h \
    HwpDoc/HWPElements/HWPRecordBinData.h \
    HwpDoc/HWPElements/HWPRecordBorderFill.h \
    HwpDoc/HWPElements/HWPRecordBullet.h \
    HwpDoc/HWPElements/HWPRecordCharShape.h \
    HwpDoc/HWPElements/HWPRecordCtrlData.h \
    HwpDoc/HWPElements/HWPRecordDocumentProperties.h \
    HwpDoc/HWPElements/HWPRecordFaceName.h \
    HwpDoc/HWPElements/HWPRecordFormObject.h \
    HwpDoc/HWPElements/HWPRecordIDMaping.h \
    HwpDoc/HWPElements/HWPRecordNumbering.h \
    HwpDoc/HWPElements/HWPTag.h \
    HwpDoc/HWPElements/HwpRecordTypes.h \
    HwpDoc/HWPFile.h \
    HwpDoc/HWPStream.h \
    HwpDoc/HanType.h \
    HwpDoc/HwpFileHeader.h \
    HwpDoc/Paragraph/CapParagraph.h \
    HwpDoc/Paragraph/CellParagraph.h \
    HwpDoc/Paragraph/CharShape.h \
    HwpDoc/Paragraph/CommonObj.h \
    HwpDoc/Paragraph/Ctrl.h \
    HwpDoc/Paragraph/CtrlAutoNumber.h \
    HwpDoc/Paragraph/CtrlCharacter.h \
    HwpDoc/Paragraph/CtrlClick.h \
    HwpDoc/Paragraph/CtrlColumnDef.h \
    HwpDoc/Paragraph/CtrlCommon.h \
    HwpDoc/Paragraph/CtrlContainer.h \
    HwpDoc/Paragraph/CtrlEqEdit.h \
    HwpDoc/Paragraph/CtrlForm.h \
    HwpDoc/Paragraph/CtrlGeneralShape.h \
    HwpDoc/Paragraph/CtrlHeadFoot.h \
    HwpDoc/Paragraph/CtrlObjElement.h \
    HwpDoc/Paragraph/CtrlPageNumPos.h \
    HwpDoc/Paragraph/CtrlShapeArc.h \
    HwpDoc/Paragraph/CtrlShapeConnectLine.h \
    HwpDoc/Paragraph/CtrlShapeCurve.h \
    HwpDoc/Paragraph/CtrlShapeEllipse.h \
    HwpDoc/Paragraph/CtrlShapeLine.h \
    HwpDoc/Paragraph/CtrlShapeOle.h \
    HwpDoc/Paragraph/CtrlShapePic.h \
    HwpDoc/Paragraph/CtrlShapePolygon.h \
    HwpDoc/Paragraph/CtrlShapeRect.h \
    HwpDoc/Paragraph/CtrlShapeTextArt.h \
    HwpDoc/Paragraph/CtrlShapeVideo.h \
    HwpDoc/Paragraph/CtrlTalbe.h \
    HwpDoc/Paragraph/HWPPargraph.h \
    HwpDoc/Paragraph/LineSeg.h \
    HwpDoc/Paragraph/Point.h \
    HwpDoc/Paragraph/RangeTag.h \
    HwpDoc/Paragraph/TblCell.h

