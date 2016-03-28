#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
protected:
    virtual void resizeEvent(QResizeEvent* e);
    virtual void closeEvent(QCloseEvent* e);
    virtual void keyPressEvent(QKeyEvent *event);

    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);

/*private slots:
    void on_horizontalSlider_valueChanged(int value);
    void on_pushButton_clicked();*/
};

#endif // MAINWINDOW_H
