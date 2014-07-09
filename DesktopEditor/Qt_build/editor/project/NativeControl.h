#ifndef NATIVECONTROL_H
#define NATIVECONTROL_H

#include <QWidget>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPaintEvent>
#include <QGLWidget>

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

private:
    CEditorCtrlWrapper* m_pWrapper;
};

#endif // NATIVECONTROL_H
