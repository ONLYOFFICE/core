#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QResizeEvent>

class CInternalWorker;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public:
    virtual void resizeEvent(QResizeEvent *event);

    void send_onDirectoryChecked();
    void send_onFileConverted(int);

signals:
    void signal_onDirectoryChecked();
    void signal_onFileConverted(int);

public slots:
    void pushButtonDirectoryClicked();
    void pushButtonConvertClicked();
    void outputIndexChanged(int value);

    void stateChanged_DOCX(bool);
    void stateChanged_DOC(bool);
    void stateChanged_ODT(bool);
    void stateChanged_RTF(bool);
    void stateChanged_TXT(bool);
    void stateChanged_HTML(bool);
    void stateChanged_DOC_ALL(bool);

    void stateChanged_PPTX(bool);
    void stateChanged_PPT(bool);
    void stateChanged_ODP(bool);
    void stateChanged_PPT_ALL(bool);

    void stateChanged_XLSX(bool);
    void stateChanged_XLS(bool);
    void stateChanged_ODS(bool);
    void stateChanged_CSV(bool);
    void stateChanged_XLS_ALL(bool);

    void stateChanged_PDF(bool);
    void stateChanged_XPS(bool);
    void stateChanged_DJVU(bool);
    void stateChanged_DJVU_ALL(bool);

    void slot_onDirectoryChecked();
    void slot_onFileConverted(int);


private:
    Ui::MainWindow *ui;

    CInternalWorker* m_pWorker;

private:
    void CorrectCheckBoxes();
};

#endif // MAINWINDOW_H
