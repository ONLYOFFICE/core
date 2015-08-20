#ifndef QWINWIDGET_H
#define QWINWIDGET_H

#include <QtWidgets/QWidget>

class QWinWidget : public QWidget
{
    Q_OBJECT
public:
    QWinWidget( HWND hParentWnd, QObject *parent = 0, Qt::WindowFlags f = 0 );
    ~QWinWidget();

    void show();
    void center();
    void showCentered();

    HWND parentWindow() const;

protected:
    void childEvent( QChildEvent *e );
    bool eventFilter( QObject *o, QEvent *e );

    bool focusNextPrevChild(bool next);
    void focusInEvent(QFocusEvent *e);
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);

private:
    void init();

    void saveFocus();
    void resetFocus();

    HWND hParent;
    HWND prevFocus;
    bool reenable_parent;
};

#endif // QWINWIDGET_H
