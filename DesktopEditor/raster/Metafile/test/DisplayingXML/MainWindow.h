#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextCharFormat>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


#include <QStandardItem>
#include "../../../../xml/include/xmlutils.h"

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

        bool ConvertToRaster(const std::wstring& wsPathToFile, bool bWithXmlFile = true);
        bool ConvertToEmf(const std::wstring& wsPathToXmlFile, const std::wstring& wsPathToNewEmfFile = L"");

        bool SaveInXmlFile(const std::wstring& wsPathToFile);

        void DisplayingFile(const std::wstring& wsPathToFile, bool bWithXmlFile = true);

    private slots:
        void on_expandButton_clicked();
        void on_ModButton_clicked();
        void on_actionExit_triggered();
        void on_actionChange_File_triggered();
        void on_actionStatistics_triggered();
        void on_actionSave_XML_as_triggered();

        void on_actionSave_EMF_as_triggered();

private:
        Ui::MainWindow      *ui;

        bool ReadXmlFile(const std::wstring& wsPathToXmlFile);
        void Clear();
    protected:
        virtual void resizeEvent(QResizeEvent *);

};
#endif // MAINWINDOW_H
