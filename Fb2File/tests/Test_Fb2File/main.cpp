#include <iostream>

#include "../../Fb2File.h"

int main()
{
    CFb2File oFile;
    oFile.SetImpDirectory(L"../../../media/");
    bool bCheck = oFile.IsFb2File(L"../../../../test_TheLastWish.fb2");
    if (!bCheck)
    {
        std::cout << "This isn't a fb2 file" << std::endl;
        return 1;
    }

    int nResConvert = oFile.Convert(L"path", L"../../../media/");
    return 0;
}
