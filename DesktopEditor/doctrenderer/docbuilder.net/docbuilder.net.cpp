// This is the main DLL file.

#include "stdafx.h"

#include "docbuilder.net.h"
#include "../docbuilder.h"

#ifdef _WIN64
#pragma comment(lib, "../../../build/lib/win_64/doctrenderer.lib")
#else
#pragma comment(lib, "../../../build/lib/win_32/doctrenderer.lib")
#endif

namespace docbuilder_net
{
	static wchar_t* StringToStdString(String^ param)
	{
		return (wchar_t*)System::Runtime::InteropServices::Marshal::StringToHGlobalUni(param).ToPointer();
	}

	ref class CDocBuilder_Private
	{
	public:
		NSDoctRenderer::CDocBuilder* m_pBuilder;

		CDocBuilder_Private(bool bIsCheckSystemFonts)
		{
			m_pBuilder = new NSDoctRenderer::CDocBuilder(bIsCheckSystemFonts);
		}
		~CDocBuilder_Private()
		{
			delete m_pBuilder;
		}
	};


	CDocBuilder::CDocBuilder(bool bIsCheckSystemFonts)
	{
		m_pInternal = gcnew CDocBuilder_Private(bIsCheckSystemFonts);
	}
	CDocBuilder::~CDocBuilder()
	{
		delete m_pInternal;
	}

	bool CDocBuilder::OpenFile(String^ path, String^ params)
	{		
		return m_pInternal->m_pBuilder->OpenFile(StringToStdString(path), StringToStdString(params));
	}
	bool CDocBuilder::CreateFile(int type)
	{
		return m_pInternal->m_pBuilder->CreateFile(type);
	}
	void CDocBuilder::SetTmpFolder(String^ folder)
	{
		m_pInternal->m_pBuilder->SetTmpFolder(StringToStdString(folder));
	}
	bool CDocBuilder::SaveFile(int type, String^ path)
	{
		return m_pInternal->m_pBuilder->SaveFile(type, StringToStdString(path));
	}
	void CDocBuilder::CloseFile()
	{
		m_pInternal->m_pBuilder->CloseFile();
	}
	bool CDocBuilder::ExecuteCommand(String^ command)
	{
		return m_pInternal->m_pBuilder->ExecuteCommand(StringToStdString(command));
	}

	bool CDocBuilder::Run(String^ path)
	{
		return m_pInternal->m_pBuilder->Run(StringToStdString(path));
	}

	bool CDocBuilder::RunText(String^ text)
	{
		return m_pInternal->m_pBuilder->RunTextW(StringToStdString(text));
	}

	void CDocBuilder::Initialize()
	{
		NSDoctRenderer::CDocBuilder::Initialize();
	}
	void CDocBuilder::Destroy()
	{
		NSDoctRenderer::CDocBuilder::Dispose();
	}

}

