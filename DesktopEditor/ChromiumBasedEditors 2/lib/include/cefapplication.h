#ifndef APPLICATION_CEF_H
#define APPLICATION_CEF_H

#include "base.h"

class CApplicationCEF_Private;
class CAscApplicationManager;
class Q_DECL_EXPORT CApplicationCEF
{
protected:
    CApplicationCEF_Private* m_pInternal;

public:

    CApplicationCEF();

    void Init_CEF(CAscApplicationManager* , int argc = 0, char* argv[] = NULL);
    virtual ~CApplicationCEF();
    void Close();
};

#endif // APPLICATION_CEF_H
