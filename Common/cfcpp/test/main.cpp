#include "tst_streamrw.h"
#include "tst_header.h"
#include "tst_directoryentry.h"
#include "tst_compondfile.h"

using namespace CFCPP;
using namespace std;

int main(int argc, char *argv[])
{
#ifdef _LINUX
    system("pwd");
#endif
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
