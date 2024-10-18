import os
import sys
sys.path.append('C:/Program Files/ONLYOFFICE/DocumentBuilder')
import docbuilder

# Helper functions
def setPictureFormProperties(pictureForm, key, tip, required, placeholder, scaleFlag, lockAspectRatio, respectBorders, shiftX, shiftY, imageUrl):
    pictureForm.Call('SetFormKey', key)
    pictureForm.Call('SetTipText', tip)
    pictureForm.Call('SetRequired', required)
    pictureForm.Call('SetPlaceholderText', placeholder)
    pictureForm.Call('SetScaleFlag', scaleFlag)
    pictureForm.Call('SetLockAspectRatio', lockAspectRatio)
    pictureForm.Call('SetRespectBorders', respectBorders)
    pictureForm.Call('SetPicturePosition', shiftX, shiftY)
    pictureForm.Call('SetImage', imageUrl)

def setTextFormProperties(textForm, key, tip, required, placeholder, comb, maxCharacters, cellWidth, multiLine, autoFit):
    textForm.Call('SetFormKey', key)
    textForm.Call('SetTipText', tip)
    textForm.Call('SetRequired', required)
    textForm.Call('SetPlaceholderText', placeholder)
    textForm.Call('SetComb', comb)
    textForm.Call('SetCharactersLimit', maxCharacters)
    textForm.Call('SetCellWidth', cellWidth)
    textForm.Call('SetCellWidth', multiLine)
    textForm.Call('SetMultiline', autoFit)

if __name__ == '__main__':
    builder = docbuilder.CDocBuilder()
    builder.CreateFile(docbuilder.FileTypes.Document.DOCX)

    context = builder.GetContext()
    globalObj = context.GetGlobal()
    api = globalObj['Api']

    document = api.Call('GetDocument')
    paragraph = document.Call('GetElement', 0)
    headingStyle = document.Call('GetStyle', 'Heading 3')

    paragraph.Call('AddText', 'Employee pass card')
    paragraph.Call('SetStyle', headingStyle)
    document.Call('Push', paragraph)

    pictureForm = api.Call('CreatePictureForm')
    setPictureFormProperties(pictureForm, 'Photo', 'Upload your photo', False, 'Photo', 'tooBig', True, False, 50, 50, 'https://api.onlyoffice.com/content/img/docbuilder/examples/user-profile.png')
    paragraph = api.Call('CreateParagraph')
    paragraph.Call('AddElement', pictureForm)
    document.Call('Push', paragraph)

    textForm = api.Call('CreateTextForm')
    setTextFormProperties(textForm, 'First name', 'Enter your first name', False, 'First name', True, 13, 3, False, False)
    paragraph = api.Call('CreateParagraph')
    paragraph.Call('AddElement', textForm)
    document.Call('Push', paragraph)

    # Save and close
    resultPath = os.getcwd() + '/result.docx'
    builder.SaveFile(docbuilder.FileTypes.Document.DOCX, resultPath)
    builder.CloseFile()
