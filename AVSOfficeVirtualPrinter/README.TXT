Описание:

Основной проект: AVSVirtualPrinter
PDFTest - тестовый пример для VirtualPrinter на C#

В папке drivers лежат необходимые драйвера для принтера, сначала нужно их скопировать в соответствующие папки (см. drivers/readme.txt )

После регистрации библиотеки AVSVirtualPrinter.dll, в ту же папку нужно перенести и AVSPostScript.dll, этот файл лежит в папке X:\AVS\Sources\AVSOfficeStudio\AVSVirtualPrinter\AVSPostScript\AVSPostScript\AVSPostScript\bin

Это надо сделать, потому что последняя dll не является ActiveX и ищется в папке, в которой лежит зарегистрированная AVSVirtualPrinter.dll

Кроме этого нужно зарегистрировать Spool.exe командой Spool.exe /regserver это ActiveX exe.   (лежит в папке X:\AVS\Sources\AVSOfficeStudio\AVSVirtualPrinter\Spool\Spool\Release)

Вообще предполагается, что три файла:

AVSVirtualPrinter.dll
AVSPostScript.dll
Spool.exe 

будут лежать вместе в Redist.
