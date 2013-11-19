REM 1. 	Собираем bjam

REM 2. 	Теперь копируем bjam.exe из "%BOOST_ROOT%\tools\jam\stage\boost-jam-3.1.17-1-ntx86? в "%SystemRoot%\system32" 
REM 	или добавляем директорию "%BOOST_ROOT%\tools\jam\stage\boost-jam-3.1.17-1-ntx86"
REM 	в системную переменную PATH. Что бы проверить, что все идет по плану - запускаем консоль и выполняем команду: bjam.exe

REM 3.  Настроить Boost.Build. Добавить переменные: BOOST_ROOT - указывает на корневую директорию, куда мы изначально распаковали boost архив (см. статью Development Environment Settings)
REM		BOOST_BUILD_PATH - указывает на %BOOST_ROOT%\tools\build\v2 Важно! Не ставте слэши в конце! Напомню, 
REM		что установить системные переменные можно в Панель управления -> Система -> Дополнительно -> Переменные среды -> Системные переменные 

@echo off
bjam ^
   -j2 ^
   --toolset=msvc-8.0 ^
   release debug ^
   --without-math ^
   --without-graph ^
   --without-python ^
   threading=multi link=static runtime-link=shared  ^
   stage --stagedir="lib"
   pause