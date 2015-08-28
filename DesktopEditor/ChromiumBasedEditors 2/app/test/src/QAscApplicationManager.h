#ifndef QASCAPPLICATIONMANAGER
#define QASCAPPLICATIONMANAGER

#include "QAscMainPanel.h"
#include <QApplication>

class QAscApplicationManager : public CAscApplicationManager
{
public:
    QAscMainPanel* m_pPanel;

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

        m_pPanel->OpenDialogSave(sName);
    }

    virtual void OnNeedCheckKeyboard()
    {
        m_pPanel->CheckKeyboard();
    }

    virtual int GetPlatformKeyboardLayout()
    {
        if (this->IsPlatformKeyboardSupport())
            return CAscApplicationManager::GetPlatformKeyboardLayout();

        return -1;
    }
};

#endif // QASCAPPLICATIONMANAGER

