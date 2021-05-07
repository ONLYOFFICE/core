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
    void on_SaveButton_clicked();
    void on_ChangeButton_clicked();

private:
    Ui::MainWindow      *ui;
    std::wstring        m_wsPathToFile;
    std::wstring        m_wsPathToXmlFile;

    QTextCharFormat     m_oRootTextFormat;
    QTextCharFormat     m_oRecordTextFormat;
    QTextCharFormat     m_oObjectTextFormat;
    QTextCharFormat     m_oArgumentTextFormat;
    QTextCharFormat     m_oArgumentValueTextFormat;
    QTextCharFormat     m_oStandardTextFormat;

    bool ReadFile();
    void ReadXmlNode(XmlUtils::CXmlNode& oXmlNode, unsigned int unLevel = 0);
    void AddIndent(unsigned int unLevel);
    void WriteFile();

    void ConvertToRaster();
protected:
    virtual void resizeEvent(QResizeEvent *);

};
#endif // MAINWINDOW_H
