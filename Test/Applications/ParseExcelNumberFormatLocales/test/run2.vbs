Set objExcel = CreateObject("Excel.Application")
objExcel.Application.Run "'" & WScript.Arguments(0) & "'!Module1.numformatlocale", WScript.Arguments(1)
objExcel.DisplayAlerts = False

objExcel.Application.Quit
Set objExcel = Nothing