#ifndef CEFCLIENT_QCEFWEBVIEW_H
#define CEFCLIENT_QCEFWEBVIEW_H

#include "./../include/cefview.h"
#include "./../include/applicationmanager.h"

#include <QWidget>
#include <QStyleOption>

class QCefView : public QWidget, public CCefViewWidgetImpl
{
    Q_OBJECT

signals:
    void _loaded();

public:
    QCefView(QWidget* parent);
    virtual ~QCefView();

    virtual void paintEvent(QPaintEvent *);
    void SetBackgroundCefColor(unsigned char r, unsigned char g, unsigned char b);

    virtual void focusInEvent(QFocusEvent* e);
    virtual void resizeEvent(QResizeEvent* e);
    virtual void moveEvent(QMoveEvent* e);

    virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result);

#if 0
    virtual void dragEnterEvent(QDragEnterEvent *e);
    virtual void dragMoveEvent(QDragMoveEvent *e);
    virtual void dragLeaveEvent(QDragLeaveEvent *e);
    virtual void dropEvent(QDropEvent *e);
#endif

    CCefView* GetCefView();
    void Create(CAscApplicationManager* pManager, CefViewWrapperType eType);

protected:
    CCefView* m_pCefView;
    QWidget* m_pLoader;

public:
    // CCefViewWidgetImpl
    virtual int parent_x();
    virtual int parent_y();
    virtual int parent_width();
    virtual int parent_height();
    virtual WindowHandleId parent_wid();
    virtual bool parent_window_is_empty();
    virtual void child_loaded();

protected slots:

    void _loadedSlot();
};

#endif  // CEFCLIENT_QCEFWEBVIEW_H
