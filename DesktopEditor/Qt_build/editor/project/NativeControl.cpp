#include "NativeControl.h"
#include "../../../DesktopEditor/editor/Control.h"

#include <QGLContext>
#include <QThread>

class CEditorCtrlWrapper : public CEditorCtrl
{
public:
    CNativeCtrl* m_pCtrl;

public:
    CEditorCtrlWrapper() : CEditorCtrl()
    {
        m_pCtrl = NULL;
    }
    virtual ~CEditorCtrlWrapper()
    {
        m_pCtrl = NULL;
    }

    virtual void InvalidateRectNative(int x, int y, int w, int h)
    {
        if (m_pCtrl != NULL)
            m_pCtrl->InvalidateRectNative(x, y, w, h);
    }
};

#if !defined(WIN32) && !defined(_MAC)
void CVideoMemory::Init()
{
    CTemporaryCS oCS(&m_oCS);

    QGLContext* ctx = const_cast<QGLContext *>(QGLContext::currentContext());

    QGLWidget* pShare = static_cast<QGLWidget *>(ctx->device());

    m_main_ctx = (void*)QGLContext::currentContext();

    m_arThreads.Add((void*)QThread::currentThread());
    m_arContext.Add((void*)pShare);

    ctx->makeCurrent();
}

void CVideoMemory::SetCurrentCtx()
{
    QThread* pThread = QThread::currentThread();

    int nCount = m_arThreads.GetCount();
    for (int i = 0; i <  nCount; ++i)
    {
        if (pThread == m_arThreads[i])
        {
            QGLWidget* pCurrent = ((QGLWidget*)m_arContext[i]);
            pCurrent->makeCurrent();
            return;
        }
    }

    QGLWidget *share = static_cast<QGLWidget *>(m_arContext[0]);

    QGLWidget* pWorker = new QGLWidget(0, share);
    pWorker->resize(8, 8);

    m_arThreads.Add((void*)pThread);
    m_arContext.Add((void*)pWorker);

    pWorker->makeCurrent();
}

void CVideoMemory::UnSetCurrentCtx()
{
}

#endif

// Native control
void CNativeCtrl::slot_threadRepaint()
{
    repaint();
}

CNativeCtrl::CNativeCtrl(QWidget *parent, const char *name) : QGLWidget(parent)
{
    connect(this, SIGNAL(signal_threadRepaint()), SLOT(slot_threadRepaint()), Qt::QueuedConnection);
    setAttribute(Qt::WA_NoBackground);
    setAttribute(Qt::WA_NoSystemBackground);

    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setMouseTracking(true);
    grabKeyboard();

    m_pWrapper = new CEditorCtrlWrapper();
    m_pWrapper->m_pCtrl = this;

    m_pWrapper->m_strFontsDirectory = L"";
}
CNativeCtrl::~CNativeCtrl()
{
}

void CNativeCtrl::InitSDK(const std::wstring& sFontsPath, const std::wstring& sSdkPath)
{
    m_pWrapper->m_strFontsDirectory = sFontsPath;
    std::wstring sScriptPath = sSdkPath;
    m_pWrapper->m_oWorkJS.m_oThreadJS.StartFromScript(sScriptPath);
}

void CNativeCtrl::OpenFile(const std::wstring& sFilePath)
{
    m_pWrapper->InternalOpenFile(sFilePath);
    m_pWrapper->InternalCalculateFile();
}

void CNativeCtrl::initializeGL()
{
    m_pWrapper->m_oDevicePainter.m_oFrameControls.m_oFrame.Init();
    m_pWrapper->Resize(100, 100);
    m_pWrapper->InternalInit();

    m_pWrapper->m_oWorkJS.m_oThreadJS.SetMainCtrl((CEditorCtrl*)m_pWrapper);
}

void CNativeCtrl::paintGL()
{
    CEditorCtrl* m_pCPlusPlusWrapper = (CEditorCtrl*)m_pWrapper;

    CTemporaryCS* pCS = NULL;
    if (!m_pCPlusPlusWrapper || !m_pCPlusPlusWrapper->m_pSkin)
        return;

    if (m_pCPlusPlusWrapper->m_bIsNeedCheckCSOnNativeBlitting)
        pCS = new CTemporaryCS(&m_pCPlusPlusWrapper->m_oCS);

    CVideoFrameControls* pVRAM_Worker = &m_pCPlusPlusWrapper->m_oDevicePainter.m_oFrameControls;

    NSCriticalSection::CRITICAL_SECTION* pCS_GL = pVRAM_Worker->m_oFrame.GetLocker();

    pCS_GL->Enter();

	pVRAM_Worker->m_oFrame.SetCurrentCtx();

    DWORD dwTime1 = NSTimers::GetTickCount();

    glClearColor(m_pCPlusPlusWrapper->m_pSkin->BackgroundColor.R / 255.0,
                 m_pCPlusPlusWrapper->m_pSkin->BackgroundColor.G / 255.0,
                 m_pCPlusPlusWrapper->m_pSkin->BackgroundColor.B / 255.0,
                 1);
    glClear(GL_COLOR_BUFFER_BIT);

    GLfloat _width = this->width() * this->devicePixelRatio();
    GLfloat _height = this->height() * this->devicePixelRatio();

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0, _width, 0.0, _height, -1.0, 1.0);

    glEnable(GL_BLEND);

    glColor3f(1,1,1);

    glViewport(0, 0, _width, _height);

    pVRAM_Worker->m_oFrame.SetSizes((int)_width, (int)_height);

    // pages
    Aggplus::Rect oClipRect(m_pCPlusPlusWrapper->m_oViewer.X,
                            m_pCPlusPlusWrapper->m_oViewer.Y,
                            m_pCPlusPlusWrapper->m_oViewer.Width,
                            m_pCPlusPlusWrapper->m_oViewer.Height);

    if (m_pCPlusPlusWrapper->m_oDrawingDocument.m_lStartBlock >= 0)
    {
        for (int i = (int)m_pCPlusPlusWrapper->m_oDrawingDocument.m_lStartBlock;
             i <= m_pCPlusPlusWrapper->m_oDrawingDocument.m_lEndBlock; ++i)
        {
            NSDrawingDocument::CBlockPages* pBlock = &m_pCPlusPlusWrapper->m_oDrawingDocument.m_arBlocks[i];
            for (int j = 0; j < pBlock->m_arPages.GetCount(); ++j)
            {
                Aggplus::Rect oRect = pBlock->m_arPages[j]->m_oGlobalBounds;
                oRect.Offset(m_pCPlusPlusWrapper->m_oViewer.X, m_pCPlusPlusWrapper->m_oViewer.Y);

                {
                    pVRAM_Worker->m_oFrame.UnSetCurrentCtx();
                    pCS_GL->Leave();
                    m_pCPlusPlusWrapper->m_oCacheDocument.DrawGL(&m_pCPlusPlusWrapper->m_oDevicePainter,
                                          oClipRect,
                                          oRect,
                                          pBlock->m_arPages[j]->m_lPageIndex);
                    pCS_GL->Enter();
                    pVRAM_Worker->m_oFrame.SetCurrentCtx();
                }
            }
        }
    }

    // overlay
    if (m_pCPlusPlusWrapper->m_oOverlay.m_oBounds.max_x >= m_pCPlusPlusWrapper->m_oOverlay.m_oBounds.min_x &&
        m_pCPlusPlusWrapper->m_oOverlay.m_oBounds.max_y >= m_pCPlusPlusWrapper->m_oOverlay.m_oBounds.min_y &&
        NULL != pVRAM_Worker->m_pOverlay)
    {
        //glEnable(GL_BLEND);
        glBlendFunc(GL_ONE,GL_ONE_MINUS_SRC_ALPHA);

        pVRAM_Worker->m_oFrame.DrawTexture(pVRAM_Worker->m_pOverlay,
                                           m_pCPlusPlusWrapper->m_oViewer.X,
                                           m_pCPlusPlusWrapper->m_oViewer.Y,
                                           true);

        //glDisable(GL_BLEND);
    }

    // buttons
    pVRAM_Worker->m_oFrame.DrawTexture(pVRAM_Worker->m_pButtonRulers,
                                       m_pCPlusPlusWrapper->m_oButtonRulers.X,
                                       m_pCPlusPlusWrapper->m_oButtonRulers.Y, true);

    pVRAM_Worker->m_oFrame.DrawTexture(pVRAM_Worker->m_pButtonNextPage,
                                       m_pCPlusPlusWrapper->m_oNext.X,
                                       m_pCPlusPlusWrapper->m_oNext.Y, true);

    pVRAM_Worker->m_oFrame.DrawTexture(pVRAM_Worker->m_pButtonPrevPage,
                                       m_pCPlusPlusWrapper->m_oPrev.X,
                                       m_pCPlusPlusWrapper->m_oPrev.Y, true);

    // scrolls
    pVRAM_Worker->m_oFrame.DrawTexture(pVRAM_Worker->m_pVerScroll,
                                       m_pCPlusPlusWrapper->m_oVerScroll.X,
                                       m_pCPlusPlusWrapper->m_oVerScroll.Y, true);

    if (m_pCPlusPlusWrapper->m_bIsHorScrollVisible)
    {
        pVRAM_Worker->m_oFrame.DrawTexture(pVRAM_Worker->m_pHorScroll,
                                           m_pCPlusPlusWrapper->m_oHorScroll.X,
                                           m_pCPlusPlusWrapper->m_oHorScroll.Y, true);
    }

    glDisable(GL_BLEND);

    glPopMatrix();

    glFlush();

    pVRAM_Worker->m_oFrame.UnSetCurrentCtx();

    pCS_GL->Leave();

    RELEASEOBJECT(pCS);

    DWORD dwTime2 = NSTimers::GetTickCount();

    /*
    FILE* file = fopen("/home/oleg/activex/AVS/Sources/TeamlabOffice/trunk/ServerComponents/DesktopEditor/log.txt", "a+");
    fprintf(file, "time: %d\n", (int)(dwTime2 - dwTime1));
    fclose(file);
}

void CNativeCtrl::resizeGL(int width, int height)
{
    int nDevicePixelRatio = this->devicePixelRatio();
    m_pWrapper->Resize(this->width() * nDevicePixelRatio, this->height() * nDevicePixelRatio);
}

void CNativeCtrl::closeEvent(QCloseEvent* e)
{
    m_pWrapper->Stop();
    m_pWrapper->m_oTarget.Stop();
}

void CNativeCtrl::mousePressEvent(QMouseEvent* e)
{
    m_pWrapper->m_oMouseController.X = e->pos().x() * this->devicePixelRatio();
    m_pWrapper->m_oMouseController.Y = e->pos().y() * this->devicePixelRatio();

    Qt::MouseButton button = e->button();
    if (button == Qt::LeftButton)
        m_pWrapper->m_oMouseController.Button = CMouseController::mbtLeft;
    else if (button == Qt::RightButton)
        m_pWrapper->m_oMouseController.Button = CMouseController::mbtRight;
    else
        m_pWrapper->m_oMouseController.Button = CMouseController::mbtCenter;

    Qt::KeyboardModifiers mods = e->modifiers();

    m_pWrapper->m_oKeyboardController.ShiftKey = (0 != (mods & Qt::ShiftModifier)) ? true : false;
    m_pWrapper->m_oKeyboardController.CtrlKey = (0 != (mods & Qt::ControlModifier)) ? true : false;
    m_pWrapper->m_oKeyboardController.AltKey = (0 != (mods & Qt::AltModifier)) ? true : false;

    m_pWrapper->m_oMouseController.CheckDownCallback(true);
    m_pWrapper->InternalOnMouseDown();
}
void CNativeCtrl::mouseMoveEvent(QMouseEvent* e)
{
    m_pWrapper->m_oMouseController.X = e->pos().x() * this->devicePixelRatio();
    m_pWrapper->m_oMouseController.Y = e->pos().y() * this->devicePixelRatio();

    Qt::KeyboardModifiers mods = e->modifiers();

    m_pWrapper->m_oKeyboardController.ShiftKey = (0 != (mods & Qt::ShiftModifier)) ? true : false;
    m_pWrapper->m_oKeyboardController.CtrlKey = (0 != (mods & Qt::ControlModifier)) ? true : false;
    m_pWrapper->m_oKeyboardController.AltKey = (0 != (mods & Qt::AltModifier)) ? true : false;

    m_pWrapper->m_oMouseController.CheckMoveCallback();
    m_pWrapper->InternalOnMouseMove();
}

void CNativeCtrl::mouseReleaseEvent(QMouseEvent* e)
{
    m_pWrapper->m_oMouseController.X = e->pos().x() * this->devicePixelRatio();
    m_pWrapper->m_oMouseController.Y = e->pos().y() * this->devicePixelRatio();

    Qt::MouseButton button = e->button();
    if (button == Qt::LeftButton)
        m_pWrapper->m_oMouseController.Button = CMouseController::mbtLeft;
    else if (button == Qt::RightButton)
        m_pWrapper->m_oMouseController.Button = CMouseController::mbtRight;
    else
        m_pWrapper->m_oMouseController.Button = CMouseController::mbtCenter;

    Qt::KeyboardModifiers mods = e->modifiers();

    m_pWrapper->m_oKeyboardController.ShiftKey = (0 != (mods & Qt::ShiftModifier)) ? true : false;
    m_pWrapper->m_oKeyboardController.CtrlKey = (0 != (mods & Qt::ControlModifier)) ? true : false;
    m_pWrapper->m_oKeyboardController.AltKey = (0 != (mods & Qt::AltModifier)) ? true : false;

    m_pWrapper->InternalOnMouseUp();
}

void CNativeCtrl::keyPressEvent(QKeyEvent* e)
{
    std::wstring s = e->text().toStdWString();

    m_pWrapper->m_oKeyboardController.KeyCode   = e->key();
    m_pWrapper->m_oKeyboardController.CharCode  = e->key();

    Qt::KeyboardModifiers mods = e->modifiers();

    m_pWrapper->m_oKeyboardController.ShiftKey = (0 != (mods & Qt::ShiftModifier)) ? true : false;
    m_pWrapper->m_oKeyboardController.CtrlKey = (0 != (mods & Qt::ControlModifier)) ? true : false;
    m_pWrapper->m_oKeyboardController.AltKey = (0 != (mods & Qt::AltModifier)) ? true : false;

    if (s.length() == 0)
    {
        m_pWrapper->InternalOnKeyDown();
    }
    else
    {
        m_pWrapper->m_oKeyboardController.KeyCode  = (int)s.c_str()[0];
        m_pWrapper->m_oKeyboardController.CharCode  = (int)s.c_str()[0];
        m_pWrapper->InternalOnKeyDown();
        m_pWrapper->InternalOnKeyPress();
    }
}

void CNativeCtrl::keyReleaseEvent(QKeyEvent* e)
{
    Qt::KeyboardModifiers mods = e->modifiers();

    m_pWrapper->m_oKeyboardController.ShiftKey = (0 != (mods & Qt::ShiftModifier)) ? true : false;
    m_pWrapper->m_oKeyboardController.CtrlKey = (0 != (mods & Qt::ControlModifier)) ? true : false;
    m_pWrapper->m_oKeyboardController.AltKey = (0 != (mods & Qt::AltModifier)) ? true : false;

    m_pWrapper->InternalOnKeyUp();
}

void CNativeCtrl::InvalidateRectNative(int x, int y, int w, int h)
{
    emit signal_threadRepaint();
}
