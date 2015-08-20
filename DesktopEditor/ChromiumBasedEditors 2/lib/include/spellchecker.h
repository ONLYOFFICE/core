#ifndef APPLICATION_SPELL_CHECKER_H
#define APPLICATION_SPELL_CHECKER_H

#include <string>
#include "base.h"

class CAscApplicationManager;
class CAscSpellChecker_Private;
class Q_DECL_EXPORT CAscSpellChecker
{
protected:
    CAscSpellChecker_Private* m_pInternal;

public:
    CAscSpellChecker();
    ~CAscSpellChecker();

public:
    void SetApplicationManager(CAscApplicationManager* );
    void AddTask(const int& nEditorId, const std::string& sTask, int nId); // utf8

    void Init(std::wstring sDirectory);

    void Start();
    void End();
};

#endif // APPLICATION_SPELL_CHECKER_H
