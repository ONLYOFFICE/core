using static System.Console;
using System.Runtime.InteropServices;

using onlyofficedocbuilder;

using CDocBuilder = onlyofficedocbuilder.CONLYOFFICEDocBuilder;
using CValue = onlyofficedocbuilder.CONLYOFFICEDocBuilderValue;
using CContext = onlyofficedocbuilder.CONLYOFFICEDocBuilderContext;
using CContextScope = onlyofficedocbuilder.CONLYOFFICEDocBuilderContextScope;

namespace Test
{
	public class Program
	{
		public static void Main(string[] args)
		{
			string resultPath = "result.docx";
			Test(resultPath);
		}
		public static void Test(string resultPath)
		{
			var doctype = ".docx";
			CDocBuilder oBuilder = new CDocBuilder();

			oBuilder.Initialize();
			
			oBuilder.CreateFile(doctype);

			CContext oContext = oBuilder.GetContext();
			CContextScope oScope = oContext.CreateScope();
		  
			CValue oGlobal = oContext.GetGlobal();

			CValue oApi = oGlobal.GetProperty("Api");
			CValue oDocument = oApi.Call("GetDocument");
			CValue oParagraph = oApi.Call("CreateParagraph");
			CValue oContent = oContext.CreateArray(1);

			oParagraph.Call("SetSpacingAfter", 1000, false);
			oParagraph.Call("AddText", "Hello from COM -> .net!");
			oContent.Set(0, oParagraph);
			oDocument.Call("InsertContent", oContent);

			oBuilder.SaveFile(doctype, resultPath);
			oBuilder.CloseFile();

			oBuilder.Dispose();
		}
	}
   
}
