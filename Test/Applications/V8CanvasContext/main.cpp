#include <iostream>
#include <string>

#include "../../../DesktopEditor/common/Types.h"
#include "../../../DesktopEditor/common/File.h"


#include <V8CanvasContext.h>

int main()
{
    std::wstring sFile = NSFile::GetProcessDirectory() + L"/../../examples/test.js";
    CV8CanvasContext oTest;
    HRESULT oRes = oTest.Run(sFile);
    std::cout << (oRes == S_OK ? "Success" : "Failure") << std::endl;
    return 0;
}
