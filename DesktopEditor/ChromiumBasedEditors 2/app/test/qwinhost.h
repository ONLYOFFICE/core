#ifndef QWINHOST_H
#define QWINHOST_H

#include <QtWidgets/QWidget>
#include <Windows.h>

class QWinHost : public QWidget
{
    Q_OBJECT
public:
    QWinHost(QWidget *parent = 0, Qt::WindowFlags f = 0);
    ~QWinHost();

    void setWindow(HWND);
    HWND window() const;

protected:
    virtual HWND createWindow(HWND parent, HINSTANCE instance);

    bool event(QEvent *e);
    void showEvent(QShowEvent *);
    void focusInEvent(QFocusEvent*);
    void resizeEvent(QResizeEvent*);

    bool nativeEvent(const QByteArray &eventType, void *message, long *result);

private:
    void fixParent();
    friend void* getWindowProc(QWinHost*);

    void* wndproc;
    bool own_hwnd;
    HWND hwnd;
};

#endif // QWINHOST_H
