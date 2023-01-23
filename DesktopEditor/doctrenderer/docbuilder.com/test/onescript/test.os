doctype = ".docx";
resultPath = "result.docx";

oBuilder = New COMObject("ONLYOFFICE.Builder");

oBuilder.Initialize();
oBuilder.CreateFile(doctype);

oContext = oBuilder.GetContext();
oScope = oContext.CreateScope();

oGlobal = oContext.GetGlobal();

// optional is not working
oApi = oGlobal.GetProperty("Api");
oDocument = oApi.Call("GetDocument",,,,,,);
oParagraph = oApi.Call("CreateParagraph",,,,,,);
oContent = oContext.CreateArray(1);

oParagraph.Call("SetSpacingAfter",1000,false,,,,);
oParagraph.Call("AddText", "Hello from COM -> OS!",,,,,);
oContent.Set(0, oParagraph);
oDocument.Call("InsertContent", oContent,,,,,);

oBuilder.SaveFile(doctype, resultPath);
oBuilder.CloseFile();

oBuilder.Dispose();
