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
        void CreateEXTEX(const QString& qsName);
        void CreateORGEX(const QString& qsName);
        void CreateFormWidget(const QString& qsName, const QString& qsFormName);
        void CreateColorWidgets(const QString& qsName, bool bBounds = false);
        void CreateEmptyWidgets(const QString& qsName);
        void CreateRectangleWidgets(const QString& qsName, const QString& qsReactangleName);
        void CreateScaleWidgets(const QString& qsName);

        void Create_Widgets_EMR_SETCOLORADJUSTMENT();
        void Create_Widgets_EMR_SETWORLDTRANSFORM();
        void Create_Widgets_EMR_MODIFYWORLDTRANSFORM();
        void Create_Widgets_EMR_CREATEPEN();
        void Create_Widgets_EMR_CREATEBRUSHINDIRECT();

        void CreateRectangleWidget(const QString& qsName);
        void CreatePointsWidget();
        void CreateColorWidget();
        void CreatePolyWidget();
        void CreateXForm();

        void ConvertWidgets(const QString& qsName);

        void ClearData();
        void Close();

        Ui::CRecordCreator *ui;
        MainWindow *m_pMainWindow;
        QStandardItem *m_pNewItem;

        int m_nSelectedRecord;

        std::vector<CSharedWidget*> m_arWidgets;
};

#endif // CRECORDCREATOR_H
