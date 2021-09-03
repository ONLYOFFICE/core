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
        //EMR_POLYBEZIER, EMR_POLYGON, EMR_POLYLINE, EMR_POLYBEZIERTO, EMR_POLYLINETO
        void CreatePointWidgets(QString qsName);
        //EMR_POLYPOLYLINE
        void CreatePolyWidgets(QString qsName);
        //EMR_SELECTOBJECT
        void Create_Widgets_EMR_SELECTOBJECT();

        void CreateBoundsWidget();
        void CreatePointsWidget();
        void CreatePolyWidget();

        void ConvertWidgets(QString qsName);

        void ClearData();
        void Close();

        Ui::CRecordCreator *ui;
        MainWindow *m_pMainWindow;
        QStandardItem *m_pNewItem;

        int m_nSelectedRecord;

        std::vector<CSharedWidget*> m_arWidgets;
};

#endif // CRECORDCREATOR_H
