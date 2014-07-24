#ifndef NATIVECONTROL_H
#define NATIVECONTROL_H

#include <QWidget>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPaintEvent>
#include <QGLWidget>

#define _USE_WEB_MENU_

#ifdef _USE_WEB_MENU_
#include <QGraphicsView>
#include <QtWebKitWidgets/QWebView>
#include <QDialog>
#include <QLayout>
#include <QDir>
#include <QUuid>
#include <QtWebKitWidgets/QWebFrame>
#endif

class CEditorCtrlWrapper;
class CNativeCtrl : public QGLWidget
{
    Q_OBJECT

signals:
    void signal_threadRepaint();

protected slots:
    void slot_threadRepaint();

public:
    CNativeCtrl(QWidget *parent = 0, const char *name = NULL);
    virtual ~CNativeCtrl();

public:
    virtual void initializeGL();

    virtual void paintGL();
    virtual void resizeGL(int width, int height);

    virtual void closeEvent(QCloseEvent* e);

    virtual void mousePressEvent(QMouseEvent* e);
    virtual void mouseMoveEvent(QMouseEvent* e);
    virtual void mouseReleaseEvent(QMouseEvent* e);

    virtual void keyPressEvent(QKeyEvent* e);
    virtual void keyReleaseEvent(QKeyEvent* e);

    virtual void InvalidateRectNative(int x, int y, int w, int h);

public:
    void InitSDK(const std::wstring& sFontsPath, const std::wstring& sSdkPath);
    void OpenFile(const std::wstring& sFilePath);

private:
    CEditorCtrlWrapper* m_pWrapper;
};

#ifdef _USE_WEB_MENU_

class CEditorCtrlWrapper2;
class COpenGLSceneCtrl : public QGraphicsScene
{
    Q_OBJECT

private:
    QWebView*           m_pWebView;
    CEditorCtrlWrapper2* m_pWrapper;
    QGraphicsView*      m_pView;
    QWidget*            m_pMenu;

    int m_nEditorX;
    int m_nEditorY;
    int m_nEditorW;
    int m_nEditorH;

public:

    std::wstring m_sFontsPath;
    std::wstring m_sSdkPath;
    std::wstring m_sFilePath;

signals:
    void native_OnResize(int X, int Y, int W, int H);
    void native_OnMouse(int type, int x, int y, int flags);
    void native_OnKeyboard(int type, int keycode, int charcode, int flags);

    void signal_threadRepaint();

public slots:
    void _native_OnResize(int X, int Y, int W, int H);
    void _native_OnMouse(int type, int x, int y, int flags);
    void _native_OnKeyboard(int type, int keycode, int charcode, int flags);

    void slot_threadRepaint();

    void onWebViewFinishLoading(bool)
    {
        if (m_pWebView)
        {
            QList<QWebFrame *> listFrames = m_pWebView->page()->mainFrame()->childFrames();
            if (!listFrames.isEmpty())
            {
                QList<QWebFrame *>::iterator frameIterator = listFrames.begin();
                QWebFrame *pChildFrame = *frameIterator;
                if (NULL != pChildFrame)
                {
                    pChildFrame->addToJavaScriptWindowObject(QString("NativeEditorObj"), this);
                }
            }
        }
    }

public:
    COpenGLSceneCtrl(QGraphicsView* pView) : QGraphicsScene()
    {
        m_pWebView = NULL;
        m_pWrapper = NULL;
        m_pView    = pView;

        connect(this, SIGNAL(signal_threadRepaint()), SLOT(slot_threadRepaint()), Qt::QueuedConnection);
    }

    void InitSDK(const std::wstring& sFontsPath, const std::wstring& sSdkPath);
    void OpenFile(const std::wstring& sFilePath);

    void Init()
    {
        m_pMenu = createDialog();
        m_pMenu->setAttribute(Qt::WA_TranslucentBackground);

        m_pWebView = new QWebView(m_pView);

        QPalette pal = m_pWebView->palette();
        pal.setBrush(QPalette::Base, Qt::transparent);
        m_pWebView->page()->setPalette(pal);

        m_pWebView->setStyleSheet("background:transparent;");
        m_pWebView->setAttribute(Qt::WA_TranslucentBackground);

        QWebSettings::globalSettings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
        QWebSettings* settings = m_pWebView->settings();

        QString m_sLocalStoragePath = QDir::tempPath() + "/" + QUuid::createUuid().toString();
        settings->setAttribute(QWebSettings::LocalContentCanAccessRemoteUrls,true);
        settings->setAttribute(QWebSettings::LocalContentCanAccessFileUrls,true);
        settings->setAttribute(QWebSettings::OfflineStorageDatabaseEnabled,true);
        settings->setAttribute(QWebSettings::OfflineWebApplicationCacheEnabled,true);
        settings->setAttribute(QWebSettings::LocalStorageEnabled, true);
        settings->setAttribute(QWebSettings::AcceleratedCompositingEnabled, true);

        settings->setOfflineStoragePath(m_sLocalStoragePath);
        settings->setOfflineWebApplicationCachePath(m_sLocalStoragePath);
        settings->setLocalStoragePath(m_sLocalStoragePath);
        settings->setOfflineStorageDefaultQuota(5*1024*1024);
        settings->setOfflineWebApplicationCacheQuota(5*1024*1024);

        QString sMenu = "file:///D:/Office/Application/apps/api/documents/index.html";
        QString sFile = "?url=file:///D:/Office/Application/sdk/Word/document/";
        QString sFullUrl = sMenu + sFile;
        QUrl url = sFullUrl;

        connect(m_pWebView, SIGNAL(loadFinished(bool)), SLOT(onWebViewFinishLoading(bool)));
        connect(this, SIGNAL (native_OnResize(int, int, int, int)), SLOT (_native_OnResize(int, int, int, int)));
        connect(this, SIGNAL (native_OnMouse(int, int, int, int)), SLOT (_native_OnMouse(int, int, int, int)));
        connect(this, SIGNAL (native_OnKeyboard(int, int, int, int)), SLOT (_native_OnKeyboard(int, int, int, int)));

        m_pWebView->load(url);

        m_pMenu->setGeometry(0, 0, m_pView->width(), m_pView->height());
        m_pWebView->setGeometry(0, 0, m_pView->width(), m_pView->height());
        m_pMenu->layout()->addWidget(m_pWebView);

        addWidget(m_pMenu);
    }

    void UpdateSize(int w, int h)
    {
        setSceneRect(0, 0, w, h);

        if (m_pMenu)
        {
            m_pMenu->setGeometry(0, 0, m_pView->width(), m_pView->height());
            m_pWebView->setGeometry(0, 0, m_pView->width(), m_pView->height());
        }
    }

    virtual  void drawBackground(QPainter *painter, const QRectF &rect);

    virtual void InvalidateRectNative(int x, int y, int w, int h);

protected:
    QDialog* createDialog()
    {
        QDialog *dialog = new QDialog(0, Qt::FramelessWindowHint);

        dialog->setWindowTitle("");
        dialog->setLayout(new QVBoxLayout());
        dialog->layout()->setSpacing(0);
        dialog->layout()->setMargin(0);

        return dialog;
    }
};

class CNativeCtrlWebMenu : public QGraphicsView
{
private:
    COpenGLSceneCtrl* m_pScene;
    QGLWidget* m_pWidget;
public:
    CNativeCtrlWebMenu(QWidget *parent = 0) : QGraphicsView(parent)
    {
        m_pWidget = new QGLWidget(QGLFormat(QGL::SampleBuffers));
        setViewport(m_pWidget);
        setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

        setStyleSheet("border-width: 0px; border-style: none;");
        setContentsMargins(0, 0, 0, 0);

        m_pScene = new COpenGLSceneCtrl(this);
        setScene(m_pScene);
    }

    void Init(std::wstring sFilePath)
    {
        m_pScene->m_sFilePath = sFilePath;
        m_pScene->Init();
    }

    void InitSDK(const std::wstring& sFontsPath, const std::wstring& sSdkPath)
    {
        m_pScene = new COpenGLSceneCtrl(this);
        setScene(m_pScene);

        m_pScene->m_sFontsPath = sFontsPath;
        m_pScene->m_sSdkPath = sSdkPath;
    }

protected:
    void resizeEvent(QResizeEvent *event)
    {
        QGraphicsView::resizeEvent(event);

        if (m_pScene)
            m_pScene->UpdateSize(width(), height());
    }

public:
    virtual void closeEvent(QCloseEvent *e)
    {
        QGraphicsView::closeEvent(e);
    }

    virtual void drawBackground(QPainter *painter, const QRectF &rect)
    {
        /*
        m_pWidget->makeCurrent();
        QGraphicsView::drawBackground(painter, rect);
        m_pWidget->doneCurrent();
        */
        m_pWidget->makeCurrent();
        if (NULL != m_pScene)
        {
            m_pScene->drawBackground(painter, rect);
        }
        m_pWidget->doneCurrent();
    }
};

#endif

#endif // NATIVECONTROL_H
