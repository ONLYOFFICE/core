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
document = api.GetDocument()
paragraph1 = api.CreateParagraph()
paragraph1.SetSpacingAfter(1000, False)
paragraph1.AddText('Hello from Python!')

paragraph2 = api.CreateParagraph()
paragraph2.AddText('Goodbye!')

content = [paragraph1, paragraph2]
document.InsertContent(content)

dstPath = os.getcwd() + '/result.docx'
builder.SaveFile(docbuilder.FileTypes.Document.DOCX, dstPath)
builder.CloseFile()
