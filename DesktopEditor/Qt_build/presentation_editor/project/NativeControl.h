#ifndef NATIVECONTROLL_H
#define NATIVECONTROLL_H

#include <QWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QPaintEvent>
#include <QtOpenGL/QGLWidget>

class CEditorCtrlWrapper;
class CApplicationFontsWorker;
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
    void InitSDK(const std::wstring& sFontsPath, const std::wstring& sSdkPath, CApplicationFontsWorker* pWorker);
    void OpenFile(const std::wstring& sFilePath);

    void SetZoom(double dZoom);
private:
    CEditorCtrlWrapper* m_pWrapper;
};

#endif // NATIVECONTROLL_H
