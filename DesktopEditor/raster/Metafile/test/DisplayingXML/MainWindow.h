#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextCharFormat>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

#include "../../../../xml/include/xmlutils.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_ChangeButton_clicked();

    void on_expandButton_clicked();

    void on_ModButton_clicked();

private:
    Ui::MainWindow      *ui;
    std::wstring        m_wsPathToFile;
    std::wstring        m_wsPathToXmlFile;

    bool ReadFile();
    void ConvertToRaster();
protected:
    virtual void resizeEvent(QResizeEvent *);

};
#endif // MAINWINDOW_H
