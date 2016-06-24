/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
