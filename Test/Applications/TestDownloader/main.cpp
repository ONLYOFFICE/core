#include "../../../Common/Network/FileTransporter/include/FileTransporter.h"
#define CHECK_URL L"http://download.onlyoffice.com/install/desktop/editors/windows/onlyoffice/updates/editors_update_x64.exe"
typedef NSNetwork::NSFileTransport::CFileDownloader Downloader;
#include <vector>
int wmain(int argc, wchar_t *argv[])
{
    auto check_url = CHECK_URL;
    auto downloader = new Downloader(check_url, false);
    std::vector<int> vec;
    downloader->SetEvent_OnProgress([&](int percent) {
            vec.push_back(percent);
        });
    downloader->SetFilePath(L"C:\\Users\\Vladimir.Gorshenkov\\Downloads\\tmp.exe");
    downloader->DownloadSync();
}
