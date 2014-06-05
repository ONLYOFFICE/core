//#include <QCoreApplication>

#include "../../src/ui/base/clipboard/clipboard.h"
#include <iostream>

int main(int argc, char *argv[])
{
    //QCoreApplication a(argc, argv);

    //return a.exec();

    ui::Clipboard *pClipboard = ui::Clipboard::GetForCurrentThread();
        bool bIsHtmlAvailable = pClipboard->IsFormatAvailable(ui::Clipboard::GetHtmlFormatType(), ui::Clipboard::BUFFER_STANDARD);
        if (bIsHtmlAvailable)
        {
            base::string16 markup;
            std::string src_url;
            uint32 fragment_start;
            uint32 fragment_end;

            pClipboard->ReadHTML(ui::Clipboard::BUFFER_STANDARD, &markup, &src_url, &fragment_start, &fragment_end);
            std::cout << "SRC Url: " << src_url << std::endl;

        }
        ui::Clipboard::DestroyClipboardForCurrentThread (pClipboard);

        return 0;
}
