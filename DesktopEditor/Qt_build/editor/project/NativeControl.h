/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#ifndef NATIVECONTROL_H
#define NATIVECONTROL_H

#include <QWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QPaintEvent>
#include <QtOpenGL/QGLWidget>

//#define _USE_WEB_MENU_

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
    virtual void wheelEvent(QWheelEvent* event);

    virtual void keyPressEvent(QKeyEvent* e);
    virtual void keyReleaseEvent(QKeyEvent* e);

    virtual void InvalidateRectNative(int x, int y, int w, int h);

public:
    void InitSDK(const std::wstring& sFontsPath, const std::wstring& sSdkPath);
    void OpenFile(const std::wstring& sFilePath);

    void SetZoom(double dZoom);
    void ChangeCountPagesInBlock();

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

    void LockGL();
    void UnlockGL();

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

        QString sMenu = "file:///E:/Office/Application/apps/api/documents/index.html";
        QString sFile = "?url=file:///E:/Office/Application/sdk/Word/document/";
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

#if 0
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
            m_pScene->LockGL();
            m_pScene->drawBackground(painter, rect);
            m_pScene->UnlockGL();
        }
        m_pWidget->makeCurrent();

        /*
        m_pWidget->makeCurrent();
        if (NULL != m_pScene)
        {
            m_pScene->drawBackground(painter, rect);
        }
        m_pWidget->doneCurrent();
        */
    }
#endif

#if 1

    virtual void paintEvent(QPaintEvent *event)
    {
        if (NULL == m_pScene)
        {
            QGraphicsView::paintEvent(event);
            return;
        }

        QGraphicsView::paintEvent(event);
    }

#endif
};

#endif

#endif // NATIVECONTROL_H
