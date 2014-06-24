//#include <QCoreApplication>

#include "../src/ui/base/clipboard/clipboard.h"
#include <iostream>

int main(int argc, char *argv[])
{
    //QCoreApplication a(argc, argv);

    //return a.exec();
    ui::Clipboard *pClipboard = ui::Clipboard::GetForCurrentThread();

    bool bIsHtmlAvailable = pClipboard->IsFormatAvailable(ui::Clipboard::GetHtmlFormatType(), ui::Clipboard::BUFFER_STANDARD);
    bool bIsTextAvailable = pClipboard->IsFormatAvailable(ui::Clipboard::GetPlainTextFormatType(), ui::Clipboard::BUFFER_STANDARD);

    if (bIsHtmlAvailable)
    {
        base::string16 markup;
        std::string src_url;
        uint32_t fragment_start, fragment_end;

        pClipboard->ReadDoct(ui::Clipboard::BUFFER_STANDARD, &markup);

        std::cout << "SRC Url:" << src_url << std::endl;
        std::cout << "Markup:" << markup << std::endl;
    }

    ui::Clipboard::DestroyClipboardForCurrentThread(pClipboard);
    return 0;
}
