1)собрать проекты MemoryConsumer и ParentProcess
2)запустить и убедиться, что выходит через 5 сек(MemoryConsumer с лимитом 5мб и выделением 1мб каждую секунду)
MemoryConsumer.exe 5 1
3)убедить что ParentProcess завершается через 10 секунд(ParentProcess с лимитом 10мб, 0 собственной памяти и дочерний MemoryConsumer с выделением 1мб каждую секунду)
ParentProcess.exe 10 0 "..\\..\\build-MemoryConsumer-Desktop_Qt_5_7_0_MSVC2015_64bit-Debug\\debug\\MemoryConsumer.exe 0 1"
3)выделяем в ParentProcess 5мб и убеждаемся что это никак не влияет на на время завершения
ParentProcess.exe 10 5 "..\\..\\build-MemoryConsumer-Desktop_Qt_5_7_0_MSVC2015_64bit-Debug\\debug\\MemoryConsumer.exe 0 1"

4)проделываем тоже самое на ubuntu
qmake MemoryConsumer.pro
make
./ParentProcess 15 5 "../MemoryConsumer/MemoryConsumer 0 1"
