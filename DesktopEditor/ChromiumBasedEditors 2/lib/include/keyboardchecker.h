#ifndef APPLICATION_KEYBOARD_CHECKER_H
#define APPLICATION_KEYBOARD_CHECKER_H

#include "base.h"

class CAscApplicationManager;
class Q_DECL_EXPORT CAscKeyboardChecker
{
public:
    CAscKeyboardChecker();
    ~CAscKeyboardChecker();

public:
    void Check(CAscApplicationManager* pManager, int nLangInput = -1);
    void Send(CAscApplicationManager* pManager);

protected:
    int m_nLanguage;
};

#endif // APPLICATION_KEYBOARD_CHECKER_H
