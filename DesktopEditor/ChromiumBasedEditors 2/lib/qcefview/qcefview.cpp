#include "./qcefview.h"

#include <QPainter>

QCefView::QCefView(QWidget* parent) : QWidget(parent)
{
    m_pCefView = NULL;

    m_pLoader = new QWidget(this);
    m_pLoader->setGeometry(0, 0, width(), height());

    SetBackgroundCefColor(255, 255, 255);

    m_pLoader->setHidden(false);

    QObject::connect(this, SIGNAL( _loaded() ) , this, SLOT( _loadedSlot() ), Qt::QueuedConnection );
}

QCefView::~QCefView()
{
    // release from CApplicationManager
}

void QCefView::paintEvent(QPaintEvent *)
{
     QStyleOption opt;
     opt.init(this);
     QPainter p(this);
     style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void QCefView::SetBackgroundCefColor(unsigned char r, unsigned char g, unsigned char b)
{
    QString sColor = QString::number((int)r, 16) + QString::number((int)g, 16) + QString::number((int)b, 16);
    QString sStyle = "background-color:#" + sColor + ";";
    this->setStyleSheet(sStyle);
    m_pLoader->setStyleSheet(sStyle);
}

void QCefView::focusInEvent(QFocusEvent* e)
{
    if (NULL != m_pCefView)
        m_pCefView->focus();
}

void QCefView::resizeEvent(QResizeEvent* e)
{
    if (NULL != e)
        QWidget::resizeEvent(e);
    m_pLoader->setGeometry(0, 0, width(), height());
    if (NULL != m_pCefView)
        m_pCefView->resizeEvent();
}

void QCefView::moveEvent(QMoveEvent* e)
{
    if (NULL != e)
        QWidget::moveEvent(e);

    if (NULL != m_pCefView)
        m_pCefView->moveEvent();
}

bool QCefView::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    if (m_pCefView)
    {
        if (m_pCefView->nativeEvent(NULL, 0, message, result))
            return true;
    }

    return QWidget::nativeEvent(eventType, message, result);
}

#if 0
void QCefView::dragEnterEvent(QDragEnterEvent *e)
{
    m_pCefView->dragEnterEvent(e);
}
void QCefView::dragMoveEvent(QDragMoveEvent *e)
{
    m_pCefView->dragMoveEvent(e);
}
void QCefView::dragLeaveEvent(QDragLeaveEvent *e)
{
    m_pCefView->dragLeaveEvent(e);
}
void QCefView::dropEvent(QDropEvent *e)
{
    m_pCefView->dropEvent(e);
}
#endif

CCefView* QCefView::GetCefView()
{
    return m_pCefView;
}

void QCefView::Create(CAscApplicationManager* pManager, CefViewWrapperType eType)
{
    switch (eType)
    {
    case cvwtSimple:
        {
            m_pCefView = pManager->CreateCefView(this);
            break;
        }
    case cvwtEditor:
        {
            m_pCefView = pManager->CreateCefEditor(this);
            break;
        }
    default:
        break;
    }
}

// CCefViewWidgetImpl
int QCefView::parent_x() { return this->pos().x(); }
int QCefView::parent_y() { return this->pos().y(); }
int QCefView::parent_width() { return this->width(); }
int QCefView::parent_height() { return this->height(); }
WindowHandleId QCefView::parent_wid()
{
#if 1
    return (WindowHandleId)this->winId();
#else
    return (WindowHandleId)m_pLoader->winId();
#endif
}
bool QCefView::parent_window_is_empty() { return true; }
void QCefView::child_loaded()
{
    emit _loaded();
}

void QCefView::_loadedSlot()
{
    if (!m_pLoader->isHidden())
        m_pLoader->setHidden(true);
}
