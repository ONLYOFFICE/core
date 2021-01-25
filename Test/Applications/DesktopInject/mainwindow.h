#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QFileDialog>
#include <QProcess>
#include <QResizeEvent>
#include <QCloseEvent>

enum OpenFileType
{
    oftEdit = 1,
    oftView = 2,
    oftReview = 3
};

class CProcessEditor : public QProcess
{
    Q_OBJECT

public:
    QString m_sPath;
    QString m_sParams;
    OpenFileType m_eType;
    int m_nId;

public:
    CProcessEditor(const QString& sPath, const OpenFileType& type, const int& nId);
    void StartEditor();

signals:
    void signalFinish(int id);

public slots:
    void slotFinish(int nCode, QProcess::ExitStatus exitStatus);
};

class CSubProcesses : public QProcess
{
    Q_OBJECT

private:
    QMap<int, CProcessEditor*> m_mapProcesses;
    int m_nCounterId;

public:
    CSubProcesses();
    CProcessEditor* Create(const QString& sPath, const OpenFileType& type);
    void CloseAll();

public slots:
    void slotFinish(int nId);
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QPushButton* m_pButtonOpenEdit;
    QPushButton* m_pButtonOpenView;
    CSubProcesses m_oProcesses;

public slots:
    void pushButtonOpenEdit();
    void pushButtonOpenView();

public:
    void calculatePlaces();
    void resizeEvent(QResizeEvent*);
    void closeEvent(QCloseEvent*);
};

#endif // MAINWINDOW_H
