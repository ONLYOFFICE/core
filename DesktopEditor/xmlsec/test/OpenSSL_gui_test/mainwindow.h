#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QPushButton>
#include <QResizeEvent>
#include <QPlainTextEdit>
#include "opensslwrapper.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    virtual void resizeEvent(QResizeEvent*);
    void CalculatePlaces();

public:

    QPlainTextEdit* m_pKeyFile;
    QPlainTextEdit* m_pKeyPassword;
    QPushButton* m_pKeyDialog;

    QPlainTextEdit* m_pCertFile;
    QPlainTextEdit* m_pCertPassword;
    QPushButton* m_pCertDialog;

    QPushButton* m_pOK;

public slots:
    void pushButtonKeyClicked();
    void pushButtonCertClicked();
    void pushButtonOKClicked();
};

#endif // MAINWINDOW_H
