#ifndef CRECORDCREATOR_H
#define CRECORDCREATOR_H

#include <QDialog>
#include <QStandardItem>

#include "MainWindow.h"
#include "Common/CSharedWidget.h"

namespace Ui {
        class CRecordCreator;
}

class CRecordCreator : public QDialog
{
        Q_OBJECT

public:
        explicit CRecordCreator(QWidget *parent = nullptr);
        virtual ~CRecordCreator();

        void SetMainWindow(MainWindow *pMainWindow);
        QStandardItem* CreateRecord();
private slots:
        void on_selectButton_clicked();
private:
        void CreatePointWidgets(const QString& qsName);
        void CreatePolyWidgets(const QString& qsName);
        void CreatePolyDrawWidgets(const QString& qsName);
        void CreateEXTEX(const QString& qsName);
        void CreateORGEX(const QString& qsName);
        void CreateFormWidget(const QString& qsName, const QString& qsFormName);
        void CreateColorWidgets(const QString& qsName, bool bBounds = false);
        void CreateEmptyWidgets(const QString& qsName);
        void CreateRectangleWidgets(const QString& qsName, const QString& qsReactangleName);
        void CreateScaleWidgets(const QString& qsName);
        void CreateArcWidgets(const QString& qsName);
        void CreateNotSupportedWidgets(const QString &qsName);
        void CreateExtTextOutWidget(const QString& qsName);
        void CreatePolyTextOutWidget(const QString& qsName);
        void CreateSetICMPRrofileWidget(const QString& qsName);

        void Create_Widgets_EMR_SETCOLORADJUSTMENT();
        void Create_Widgets_EMR_SETWORLDTRANSFORM();
        void Create_Widgets_EMR_MODIFYWORLDTRANSFORM();
        void Create_Widgets_EMR_CREATEPEN();
        void Create_Widgets_EMR_CREATEBRUSHINDIRECT();
        void Create_Widgets_EMR_ANGLEARC();
        void Create_Widgets_EMR_ROUNDRECT();
        void Create_Widgets_EMR_CREATEPALETTE();
        void Create_Widgets_EMR_SETPALETTEENTRIES();
        void Create_Widgets_EMR_RESIZEPALETTE();
        void Create_Widgets_EMR_EXTFLOODFILL();
        void Create_Widgets_EMR_LINETO();
        void Create_Widgets_EMR_FILLRGN();
        void Create_Widgets_EMR_FRAMERGN();
        void Create_Widgets_EMR_INVERTRGN();
        void Create_Widgets_EMR_PAINTRGN();
        void Create_Widgets_EMR_EXTSELECTCLIPRGN();
        void Create_Widgets_EMR_EXTCREATEFONTINDIRECTW();
        void Create_Widgets_EMR_CREATECOLORSPACE();
        void Create_Widgets_EMR_GLSBOUNDEDRECORD();
        void Create_Widgets_EMR_PIXELFORMAT();
        void Create_Widgets_EMR_SMALLTEXTOUT();
        void Create_Widgets_EMR_FORCEUFIMAPPING();
        void Create_Widgets_EMR_NAMEDESCAPE();
        void Create_Widgets_EMR_COLORCORRECTPALETTE();
        void Create_Widgets_EMR_GRADIENTFILL();
        void Create_Widgets_EMR_SETLINKEDUFIS();
        void Create_Widgets_EMR_SETTEXTJUSTIFICATION();
        void Create_Widgets_EMR_COLORMATCHTOTARGETW();
        void Create_Widgets_EMR_CREATECOLORSPACEW();

        void ConvertWidgets(const QString& qsName);
        void AddSharedWidget(CSharedWidget* pWidget);
        void AddSharedWidgets(std::vector<CSharedWidget*> arWidgets);

        void ClearData();
        void Close();

        Ui::CRecordCreator *ui;
        MainWindow *m_pMainWindow;
        QStandardItem *m_pNewItem;

        int m_nSelectedRecord;

        std::vector<CSharedWidget*> m_arWidgets;
};

#endif // CRECORDCREATOR_H
