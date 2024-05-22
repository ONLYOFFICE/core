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
paragraph1 = api.Call('CreateParagraph')
paragraph1.Call('SetSpacingAfter', 1000, False)
paragraph1.Call('AddText', 'Hello from Python!')

paragraph2 = api.Call('CreateParagraph')
paragraph2.Call('AddText', 'Goodbye!')

content = [paragraph1, paragraph2]
document.Call('InsertContent', content)

dstPath = os.getcwd() + '/result.docx'
builder.SaveFile(docbuilder.FileTypes.Document.DOCX, dstPath)
builder.CloseFile()
