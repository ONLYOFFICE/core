import os
import sys
sys.path.append('path_to_docbuilder')
import docbuilder

builder = docbuilder.CDocBuilder()

builder.CreateFile(docbuilder.FileTypes.Document.DOCX)

context = builder.GetContext()
scope = context.CreateScope()

globalObj = context.GetGlobal()

api = globalObj['Api']
document = api.Call('GetDocument')
paragraph = api.Call('CreateParagraph')
paragraph.Call('SetSpacingAfter', 1000, False)
paragraph.Call('AddText', 'Hello, World!')
content = context.CreateArray(1)
content[0] = paragraph
document.Call('InsertContent', content)

dstPath = os.getcwd() + '/result.docx'
builder.SaveFile(docbuilder.FileTypes.Document.DOCX, dstPath)
builder.CloseFile()
