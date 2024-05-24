#include "docbuilder.python/src/docbuilder_functions.h"

#include <string>
#include <iostream>

#include "app_builder_lib/utils.cpp"

#define DECLARE_RAII_DOCBUILDER_FUNC_CLASS( CLASS_NAME )							\
class CLASS_NAME ## F                                                               \
{                                                                                   \
public:                                                                             \
	CLASS_NAME ## F(CLASS_NAME* internal) { m_internal = internal; }				\
	~CLASS_NAME ## F() { CLASS_NAME ## _Destroy(m_internal); }                      \
																					\
	CLASS_NAME* get() { return m_internal; }                                        \
																					\
private:                                                                            \
	CLASS_NAME* m_internal;                                                         \
};

DECLARE_RAII_DOCBUILDER_FUNC_CLASS(CDocBuilder)
DECLARE_RAII_DOCBUILDER_FUNC_CLASS(CDocBuilderValue)
DECLARE_RAII_DOCBUILDER_FUNC_CLASS(CDocBuilderContext)
DECLARE_RAII_DOCBUILDER_FUNC_CLASS(CDocBuilderContextScope)

int main()
{
	std::wstring sWorkDirectory = NSUtils::GetBuilderDirectory();

#if 0
	// Simple test that shows builder version if everything is correct
	CDocBuilder_InitializeWithDirectory(sWorkDirectory.c_str());
	CDocBuilder* pBuilder = CDocBuilder_Create();

	char* sVersion = CDocBuilder_GetVersion(pBuilder);
	std::cout << sVersion << std::endl;
	DeleteCharP(sVersion);

	CDocBuilder_Dispose();
	CDocBuilder_Destroy(pBuilder);
#else
	// Test is identical to app_builder_lib.pro
	// The test uses RAII wrappers - classes with 'F' postfix, which are destroyed automatically
	CDocBuilder_InitializeWithDirectory(sWorkDirectory.c_str());

	CDocBuilderF oBuilder = CDocBuilder_Create();
	CDocBuilder_SetProperty(oBuilder.get(), L"--work-directory", sWorkDirectory.c_str());

	CDocBuilder_CreateFileByExtension(oBuilder.get(), L"docx");

	CDocBuilderContextF oContext = CDocBuilder_GetContext(oBuilder.get());
	CDocBuilderContextScopeF oScope = CDocBuilderContext_CreateScope(oContext.get());

	CDocBuilderValueF oGlobal = CDocBuilderContext_GetGlobal(oContext.get());

	CDocBuilderValueF oApi = CDocBuilderValue_GetProperty(oGlobal.get(), L"Api");
	CDocBuilderValueF oDocument = CDocBuilderValue_Call0(oApi.get(), L"GetDocument");
	CDocBuilderValueF oParagraph = CDocBuilderValue_Call0(oApi.get(), L"CreateParagraph");
	CDocBuilderValue_Call2(oParagraph.get(), L"SetSpacingAfter", CDocBuilderValueF(CDocBuilderValue_CreateWithInt(1000)).get(), CDocBuilderValueF(CDocBuilderValue_CreateWithBool(false)).get());
	CDocBuilderValue_Call1(oParagraph.get(), L"AddText", CDocBuilderValueF(CDocBuilderValue_CreateWithString(L"Hello, world!")).get());
	CDocBuilderValueF oContent = CDocBuilderContext_CreateArray(oContext.get(), 1);
	CDocBuilderValue_SetByIndex(oContent.get(), 0, oParagraph.get());
	CDocBuilderValue_Call1(oDocument.get(), L"InsertContent", oContent.get());

	std::wstring sProcessDirectory = NSUtils::GetProcessDirectory();
	std::wstring sDstPath = sProcessDirectory + L"/result.docx";
	CDocBuilder_SaveFileByExtension(oBuilder.get(), L"docx", sDstPath.c_str());
	CDocBuilder_CloseFile(oBuilder.get());

	CDocBuilder_Dispose();
#endif

	return 0;
}
