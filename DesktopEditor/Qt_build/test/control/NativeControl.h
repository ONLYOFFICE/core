#ifndef TEST_NATIVECONTROL_H
#define TEST_NATIVECONTROL_H
#include <QApplication>

#include "../../editor/project/NativeControl.h"
class NativeControl : public CNativeCtrl
{
public:
    NativeControl(QWidget *parent = 0, const char *name = NULL) : CNativeCtrl(parent, name)
    {
    }
    virtual ~NativeControl()
    {
    }
public:
    virtual void initializeGL()
    {
        CNativeCtrl::initializeGL();

        QString sBasePath = QApplication::applicationDirPath();
        QString sDebug = "Debug";
        QString sRelease = "Release";

        int n1 = sBasePath.lastIndexOf(sDebug);
        int n2 = sBasePath.lastIndexOf(sRelease);
        if (n1 < 0 && n2 < 0)
            return;

        int nCount = 0;
        if (n1 > n2)
        {
            nCount = n1 + sDebug.length();
        }
        else
        {
            nCount = n2 + sRelease.length();
        }

        if (nCount != sBasePath.length())
        {
            sBasePath = sBasePath.mid(0, nCount);
        }

        QString sFontsPath = sBasePath + "/../../../../../OfficeWeb/Fonts/native";
        QString sSdkPath = sBasePath + "/../../test_resources/sdk-all.js";
        QString sDocumentPath = sBasePath + "/../../test_resources/document/Editor.bin";

        std::wstring s1 = sFontsPath.toStdWString();
        std::wstring s2 = sSdkPath.toStdWString();
        std::wstring s3 = sDocumentPath.toStdWString();

        InitSDK(s1, s2);
        OpenFile(s3);
    }
};

#endif // TEST_NATIVECONTROL_H
