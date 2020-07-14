#!/usr/bin/env python

import sys
sys.path.append('./../../../build_tools/scripts')
import base

def make():
    base.configure_common_apps("../../../build_tools/scripts/base.py")

    # Извлекаем файлы
    base.extract('template.docx', 'tmp/')
    # Читаем содержимое
    content_types = base.readFile('tmp/[Content_Types].xml')
    rels = base.readFile('tmp/_rels/.rels')
    fontTable = base.readFile('tmp/word/fontTable.xml')
    settings = base.readFile('tmp/word/settings.xml')
    styles = base.readFile('tmp/word/styles.xml')
    webSettings = base.readFile('tmp/word/webSettings.xml')
    theme = base.readFile('tmp/word/theme/theme1.xml')
    footnotes = base.readFile('tmp/word/_rels/footnotes.xml.rels')
    # Удаляем директорию tmp
    base.delete_dir('tmp')
    # Заменяем " на \"
    content_types = content_types.replace("\"", '\\\"')
    rels = rels.replace("\"", '\\\"')
    fontTable = fontTable.replace("\"", '\\\"')
    settings = settings.replace("\"", '\\\"')
    
    webSettings = webSettings.replace("\"", '\\\"')
    theme = theme.replace("\"", '\\\"')
    footnotes = footnotes.replace("\"", '\\\"')

    # style слишком огромный. Делим на строки
    STYLE = ""
    max_length = 2500
    range_style = range(len(styles) // max_length + 1)
    
    for i in range_style:
        STYLE += 'std::wstring style' + str(i) + ' = L\"' + styles[i * max_length : (i + 1) * max_length].replace("\"", '\\\"') + '\";\n'

    styleWriter = ""
    for i in range_style:
        styleWriter += 'oWriter.WriteStringUTF8(style' + str(i) + ');\n'

    # Пишем tmp в template.h
    base.replaceInFile('template.h', '// [START]', 
                       'std::wstring content_types = L"' + content_types + '";\n'
                       + 'std::wstring rels = L"' + rels + '";\n'
                       + 'std::wstring fontTable = L"' + fontTable + '";\n'
                       + 'std::wstring settings = L"' + settings + '";\n'
                       + STYLE
                       + 'std::wstring webSettings = L"' + webSettings + '";\n'
                       + 'std::wstring theme = L"' + theme + '";\n'
                       + 'std::wstring footnotes = L"' + footnotes + '";\n')

    # Пишем строки в файлы
    base.replaceInFile('template.h', '// [END]', 'NSDirectory::CreateDirectory(sDirectory);\n'
                       + 'NSFile::CFileBinary oWriter;\n'
                       + 'oWriter.CreateFileW(sDirectory + L\"/[Content_Types].xml\");\n'
                       + 'oWriter.WriteStringUTF8(content_types);\n'
                       + 'oWriter.CloseFile();\n'
                       + 'NSDirectory::CreateDirectory(sDirectory + L\"/_rels\");\n'
                       + 'oWriter.CreateFileW(sDirectory + L\"/_rels/.rels\");\n'
                       + 'oWriter.WriteStringUTF8(rels);\n'
                       + 'oWriter.CloseFile();\n'
                       + 'NSDirectory::CreateDirectory(sDirectory + L\"/word\");\n'
                       + 'oWriter.CreateFileW(sDirectory + L\"/word/fontTable.xml\");\n'
                       + 'oWriter.WriteStringUTF8(fontTable);\n'
                       + 'oWriter.CloseFile();\n'
                       + 'oWriter.CreateFileW(sDirectory + L\"/word/settings.xml\");\n'
                       + 'oWriter.WriteStringUTF8(settings);\n'
                       + 'oWriter.CloseFile();\n'
                       + 'oWriter.CreateFileW(sDirectory + L\"/word/styles.xml\");\n'
                       + styleWriter
                       + 'oWriter.CloseFile();\n'
                       + 'oWriter.CreateFileW(sDirectory + L\"/word/webSettings.xml\");\n'
                       + 'oWriter.WriteStringUTF8(webSettings);\n'
                       + 'oWriter.CloseFile();\n'
                       + 'NSDirectory::CreateDirectory(sDirectory + L\"/word/theme\");\n'
                       + 'oWriter.CreateFileW(sDirectory + L\"/word/theme/theme1.xml\");\n'
                       + 'oWriter.WriteStringUTF8(theme);\n'
                       + 'oWriter.CloseFile();\n'
                       + 'NSDirectory::CreateDirectory(sDirectory + L\"/word/_rels\");\n'
                       + 'oWriter.CreateFileW(sDirectory + L\"/word/_rels/footnotes.xml.rels\");\n'
                       + 'oWriter.WriteStringUTF8(footnotes);\n'
                       + 'oWriter.CloseFile();\n'
                       )

    print('HI')
    return

make()
