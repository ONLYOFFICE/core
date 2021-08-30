#ifndef CRECORDCREATOR_H
#define CRECORDCREATOR_H

#include <QDialog>
#include <QStandardItem>

#include "MainWindow.h"

#include <QTextEdit>

namespace Ui {
        class CRecordCreator;
}

class CRecordCreator : public QDialog
{
        Q_OBJECT

public:
        explicit CRecordCreator(QWidget *parent = nullptr);
        ~CRecordCreator();

        void SetMainWindow(MainWindow *pMainWindow);
        QStandardItem* CreateRecord();
private slots:
        void on_selectButton_clicked();

private:
        void Create_EMR_SELECTOBJECT_boxs();
        void Convert_EMR_SELECTOBJECT();

        void ClearData();

        void AlignTextEdit(QTextEdit* pTextEdit);

        Ui::CRecordCreator *ui;
        MainWindow *m_pMainWindow;
        QStandardItem *m_pNewItem;

        int m_nSelectedRecord;

        QMap<QString, QTextEdit*> m_mData;
};

#endif // CRECORDCREATOR_H
