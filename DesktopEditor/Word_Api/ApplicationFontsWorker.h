#ifndef _BUILD_APPLICATIONFONTSWORKER_H_
#define _BUILD_APPLICATIONFONTSWORKER_H_

#include <string>
#include <vector>

class CApplicationFontsWorker
{
public:
    std::vector<std::wstring> m_arAdditionalFolders;
    
public:
    CApplicationFontsWorker();
    ~CApplicationFontsWorker();
    
    std::vector<std::wstring> CheckApplication(bool bIsNeedSystemFonts,
                          unsigned char* pDataSrc, unsigned int nLenSrc,
                          unsigned char*& pDataDst, unsigned int& nLenDst);
};

#endif // _BUILD_APPLICATIONFONTSWORKER_H_
