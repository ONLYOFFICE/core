#include <iostream>
#include "../../../DesktopEditor/xml/include/xmlutils.h"
#include "../../../DesktopEditor/common/File.h"

int main()
{
    // sax reader
    XmlUtils::CXmlLiteReader oLightReader;
    /*
    Выводы:
    Возвращает успешность открытия файла
    Читает файл в поток m_pStream
    Длина потока в m_lStreamLen
    Формирует reader
    После чтения файл закрывается
    Мысли:
    xmlreader.h - функции работы с xml
    */
    bool res1 = oLightReader.FromFile(NSFile::GetProcessDirectory() + L"/../../../examples/test.xml");
    std::wcout << res1 << std::endl; // true

    /*
    Возвращает порядковый номер узла (кажется нет)
    Мысли:
    Возвращает XmlNodeType_None(0)
    */
    XmlUtils::XmlNodeType res2 = oLightReader.GetNodeType();
    std::wcout << res2<< " " << oLightReader.GetName() << std::endl; // 0 пусто, т.к. текущий узел отсутствует

    /*
    GetName() - возвращает имя текущего узла
    GetDepth() - возвращает глубину текущего узла
    */

    /*
    Возвращает успешность чтения узла указанного типа
    По ссылке сохраняет тип узла
    Мысли:
    Значение XmlNodeType_None(0) преобразуется в XmlNodeType_Element(1)
    */
    bool res3 = oLightReader.Read(res2); // Прочитает company
    std::wcout << res3 << " " << res2 << " " << oLightReader.GetName() << std::endl; // true 1

    /*
    Мысли:
    Возвращает успешность чтения следующего узла
    В reader изменяются глубина depth если узел вложенный
    И текущий узел node
    */
    bool res4 = oLightReader.ReadNextNode(); // Прочитает staff
    std::wcout << res4 << " " << oLightReader.GetName() << std::endl; // true
    bool res5 = oLightReader.ReadNextNode(); // Прочитает firstname
    std::wcout << res5 << " " << oLightReader.GetName() << std::endl; // true

    /*
    Возвращает успешность чтения следующего узла на той же глубине - 1
    Внешний тэг не в счет
    */
    bool res6 = oLightReader.ReadNextSiblingNode(1); // Прочитает lastname
    std::wcout << res6 << " " << oLightReader.GetName() << std::endl; // true
    //bool res7 = oLightReader.ReadNextSiblingNode(1); // Прочитает staff закрывающий
    //std::wcout << res7 << std::endl; // false, т.к. прочитанный узел на другой глубине

    /*
    Мысли:
    Принудительно читает узел на той-же глубине,
    Если узла нет, то создает узел text
    */
    bool res7 = oLightReader.ReadNextSiblingNode2(1); // Прочитает text (ЧТО ЭТО) родитель parent будет staff
    std::wcout << res7 << " " << oLightReader.GetName() << std::endl; // true

    return 0;
}
