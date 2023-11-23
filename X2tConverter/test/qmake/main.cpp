#include "../../../DesktopEditor/common/Directory.h"
#include "../../../DesktopEditor/common/StringBuilder.h"
#include "../../../DesktopEditor/fontengine/ApplicationFontsWorker.h"
#include "../../../Common/OfficeFileFormatChecker.h"
#include "../../src/dylib/x2t.h"
#include "common.h"

#include <gtest/gtest.h>

int main(int argc, char** argv)
{ 
	::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
