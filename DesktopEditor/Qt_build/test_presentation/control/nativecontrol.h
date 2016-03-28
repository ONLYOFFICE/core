#ifndef NATIVECONTROL
#define NATIVECONTROL

#include "../../presentation_editor/project/NativeControl.h"
#include "../../../fontengine/ApplicationFontsWorker.h"

class CNativeControl : public CNativeCtrl
{
public:
    CNativeControl(QWidget *parent = 0, const char *name = NULL) : CNativeCtrl(parent, name)
    {
    }
    virtual ~CNativeControl()
    {
    }
public:
    virtual void initializeGL()
    {
       CNativeCtrl::initializeGL();
       QString qsBasePath = QApplication::applicationDirPath();
       QString qsFontsPath = qsBasePath + "/../../../../fonts/owner";
       QString qsSDKPath   = qsBasePath + "/../../../../test_mobile/script.bin";
       QString qsFilePath  = qsBasePath + "/../../../../test_mobile/Editor2.bin";

       CApplicationFontsWorker *pWorker = new CApplicationFontsWorker();
       QString sqFreeFontsPath = qsBasePath + "/../../../../fonts/free";
       QString sqOwnerFontsPath = qsBasePath + "/../../../../fonts/owner";
       pWorker->m_arAdditionalFolders.push_back(sqFreeFontsPath.toStdWString());
       pWorker->m_arAdditionalFolders.push_back(sqOwnerFontsPath.toStdWString());

       InitSDK(qsFontsPath.toStdWString(), qsSDKPath.toStdWString(), pWorker);
       OpenFile(qsFilePath.toStdWString());
    }
};

#endif // NATIVECONTROL

