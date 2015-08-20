#ifndef QASCAPPLICATIONMANAGER
#define QASCAPPLICATIONMANAGER

#include "../../lib/include/cefapplication.h"
#include "../../lib/include/applicationmanager.h"

#include <shlwapi.h>
#include <shlobj.h>
#include <QFileDialog>
#include "qmainpanel.h"

class QAscApplicationManager : public CAscApplicationManager
{
public:
    QMainPanel* m_pPanel;

public:
    QAscApplicationManager()
    {
        m_pPanel = NULL;
    }

public:
    virtual void StartSaveDialog(const std::wstring& sName)
    {
        // сделал через QMainPanel - чтобы использовать сигналы-слоты.
        // если сделать QAscApplicationManager : public QObject, то он будет прокидывать
        // слоты родителю. Т.е. классу CAscApplicationManager.
        // А в либе я не буду затачиваться на QT

        ((QMainPanel*)m_pPanel)->sendDialogSave(sName);
    }
};

#endif // QASCAPPLICATIONMANAGER

