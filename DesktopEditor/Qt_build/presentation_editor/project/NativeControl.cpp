#include "NativeControl.h"
#include "../../../DesktopEditor/presentation_editor/Control.h"
#include "../../../Word_Api/Editor_Api.h"
#include "../../../fontengine/ApplicationFontsWorker.h"

#include <QtOpenGL/QGLContext>
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
    int GetCountThreadsUse()
    {
        return 3;
    }
};

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

void CNativeCtrl::InitSDK(const std::wstring& sFontsPath, const std::wstring& sSdkPath, CApplicationFontsWorker *pWorker)
{
    m_pWrapper->SetFontsDirectory(sFontsPath);

    unsigned char* pDataDst = NULL;
    unsigned int nLenDst = 0;

    std::vector<std::wstring> arrNames = pWorker->CheckApplication(true, NULL, 0, pDataDst, nLenDst);
    int nLenAllFonts = 0;
    memcpy(&nLenAllFonts, pDataDst, sizeof(int));
    m_pWrapper->SetAllFontsJSData((char*)(pDataDst + 4), nLenAllFonts);
    std::wstring sScriptPath = sSdkPath;
    m_pWrapper->m_oWorkJS.m_oThreadJS.StartFromScript(sScriptPath);
}

void CNativeCtrl::OpenFile(const std::wstring& sFilePath)
{
    m_pWrapper->InternalOpenFile(sFilePath);
}


void CNativeCtrl::SetZoom(double dZoom)
{
    m_pWrapper->SetZoom(dZoom);
}

void CNativeCtrl::initializeGL()
{
    m_pWrapper->m_bIsRetina = false;
    m_pWrapper->m_oDevicePainter.m_oFrameControls.m_oFrame.Init(m_pWrapper->GetCountThreadsUse());
    m_pWrapper->InternalInit();
    m_pWrapper->Resize(600, 600);
    m_pWrapper->m_oWorkJS.m_oThreadJS.SetMainCtrl((CEditorCtrl*)m_pWrapper);
}

void CNativeCtrl::paintGL()
{


    CVideoFrameControls* pVRAM_Worker = &m_pWrapper->m_oDevicePainter.m_oFrameControls;

    pVRAM_Worker->m_oFrame.SetCurrentCtx();

    DWORD dwTime1 = NSTimers::GetTickCount();


    CRgbColor3* pBackgroundColor = &(m_pWrapper->m_pSkin->BackgroundColor);
    glClearColor(pBackgroundColor->R / 255.0,
                 pBackgroundColor->G / 255.0,
                 pBackgroundColor->B / 255.0,
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

    pVRAM_Worker->m_oFrame.UnSetCurrentCtx();

    pVRAM_Worker->m_oFrame.SetSizes((int)_width, (int)_height);

    m_pWrapper->m_oSlideDrawer.DrawGL();



    m_pWrapper->m_oDevicePainter.LockPaint();

       // buttons

       pVRAM_Worker->m_oFrame.DrawTexture(pVRAM_Worker->m_pButtonNextPage,
                                          m_pWrapper->m_oNext.X,
                                          m_pWrapper->m_oNext.Y, true);

       pVRAM_Worker->m_oFrame.DrawTexture(pVRAM_Worker->m_pButtonPrevPage,
                                          m_pWrapper->m_oPrev.X,
                                          m_pWrapper->m_oPrev.Y, true);

       // scrolls
       pVRAM_Worker->m_oFrame.DrawTexture(pVRAM_Worker->m_pVerScroll,
                                          m_pWrapper->m_oVerScroll.X,
                                          m_pWrapper->m_oVerScroll.Y, true);

       if (m_pWrapper->m_oHorScroll.IsDisplay)
       {
           pVRAM_Worker->m_oFrame.DrawTexture(pVRAM_Worker->m_pHorScroll,
                                              m_pWrapper->m_oHorScroll.X,
                                              m_pWrapper->m_oHorScroll.Y, true);
       }

       pVRAM_Worker->m_oFrame.SetCurrentCtx();

       glDisable(GL_BLEND);

       glPopMatrix();

       glFlush();

       pVRAM_Worker->m_oFrame.UnSetCurrentCtx();

       m_pWrapper->m_oDevicePainter.LeavePaint();
}

void CNativeCtrl::resizeGL(int width, int height)
{
    int nDevicePixelRatio = this->devicePixelRatio();
    m_pWrapper->Resize(this->width() * nDevicePixelRatio, this->height() * nDevicePixelRatio);
}

void CNativeCtrl::closeEvent(QCloseEvent* e)
{
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
void CNativeCtrl::wheelEvent(QWheelEvent* event)
{
}

void CNativeCtrl::keyPressEvent(QKeyEvent* e)
{

    if (e->key()==Qt::Key_Left)
    {
        if(m_pWrapper->m_lCurSlide > 0)
        {
            m_pWrapper->GoToPage(m_pWrapper->m_lCurSlide - 1);
        }
    }
    else if(e->key()==Qt::Key_Right)
    {
        if(m_pWrapper->m_lCurSlide < m_pWrapper->m_lSlidesCount - 1)
        {
            m_pWrapper->GoToPage(m_pWrapper->m_lCurSlide + 1);
        }
    }
}

void CNativeCtrl::keyReleaseEvent(QKeyEvent* e)
{
}

void CNativeCtrl::InvalidateRectNative(int x, int y, int w, int h)
{
    emit signal_threadRepaint();
}
