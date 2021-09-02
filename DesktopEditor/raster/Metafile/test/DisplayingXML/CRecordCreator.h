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
        //EMR_POLYBEZIER
        void Create_Widgets_EMR_POLYBEZIER();
        void Convert_Widgets_EMR_POLYBEZIER();
        //EMR_POLYGON
        void Create_Widgets_EMR_POLYGON();
        void Convert_Widgets_EMR_POLYGON();
        //EMR_POLYLINE
        void Create_Widgets_EMR_POLYLINE();
        void Convert_Widgets_EMR_POLYLINE();
        //EMR_POLYBEZIERTO
        void Create_Widgets_EMR_POLYBEZIERTO();
        void Convert_Widgets_EMR_POLYBEZIERTO();
        //EMR_POLYLINETO
        void Create_Widgets_EMR_POLYLINETO();
        void Convert_Widgets_EMR_POLYLINETO();
        //EMR_POLYPOLYLINE
        void Create_Widgets_EMR_POLYPOLYLINE();
        void Convert_Widgets_EMR_POLYPOLYLINE();
        //EMR_SELECTOBJECT
        void Create_Widgets_EMR_SELECTOBJECT();
        void Convert_Widgets_EMR_SELECTOBJECT();

        void CreateBoundsWidget();
        void CreatePointsWidget();
        void CreatePolyWidget();

        void ConvertWidgets();

        void ClearData();
        void Close();

        Ui::CRecordCreator *ui;
        MainWindow *m_pMainWindow;
        QStandardItem *m_pNewItem;

        int m_nSelectedRecord;

        std::vector<CSharedWidget*> m_arWidgets;
};

#endif // CRECORDCREATOR_H
