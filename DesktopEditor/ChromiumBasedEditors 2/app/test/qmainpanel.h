#ifndef QMAINPANELW_H
#define QMAINPANELW_H

#include "qwinwidget.h"
#include "./src/QAscMainPanel.h"

class QMainPanel : public QWinWidget, public CMainButtonsController
{
    Q_OBJECT

public:
    QMainPanel( HWND hWnd, CAscApplicationManager* pManager );
    bool nativeEvent(const QByteArray &, void *msg, long *result);

    void mousePressEvent( QMouseEvent *event );
    void resizeEvent(QResizeEvent* event);

    virtual void pushButtonMinimizeClicked();
    virtual void pushButtonMaximizeClicked();
    virtual void pushButtonCloseClicked();

private:
    HWND windowHandle;

public:
    QAscMainPanel* m_pPanel;
};

#endif // QMAINPANELW_H
