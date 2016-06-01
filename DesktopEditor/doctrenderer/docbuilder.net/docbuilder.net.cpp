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

	ref class CWrapper_Private
	{
	public:
		NSDoctRenderer::CDocBuilder* m_pBuilder;

		CWrapper_Private(bool bIsCheckSystemFonts)
		{
			m_pBuilder = new NSDoctRenderer::CDocBuilder(bIsCheckSystemFonts);
		}
		~CWrapper_Private()
		{
			delete m_pBuilder;
		}
	};


	CWrapper::CWrapper(bool bIsCheckSystemFonts)
	{
		m_pInternal = gcnew CWrapper_Private(bIsCheckSystemFonts);
	}
	CWrapper::~CWrapper()
	{
		delete m_pInternal;
	}

	bool CWrapper::OpenFile(String^ path, String^ params)
	{		
		return m_pInternal->m_pBuilder->OpenFile(StringToStdString(path), StringToStdString(params));
	}
	bool CWrapper::CreateFile(int type)
	{
		return m_pInternal->m_pBuilder->CreateFile(type);
	}
	void CWrapper::SetTmpFolder(String^ folder)
	{
		m_pInternal->m_pBuilder->SetTmpFolder(StringToStdString(folder));
	}
	bool CWrapper::SaveFile(int type, String^ path)
	{
		return m_pInternal->m_pBuilder->SaveFile(type, StringToStdString(path));
	}
	void CWrapper::CloseFile()
	{
		m_pInternal->m_pBuilder->CloseFile();
	}
	bool CWrapper::ExecuteCommand(String^ command)
	{
		return m_pInternal->m_pBuilder->ExecuteCommand(StringToStdString(command));
	}

	bool CWrapper::Run(String^ path)
	{
		return m_pInternal->m_pBuilder->Run(StringToStdString(path));
	}

	bool CWrapper::RunText(String^ text)
	{
		return m_pInternal->m_pBuilder->RunTextW(StringToStdString(text));
	}

	void CWrapper::Initialize()
	{
		NSDoctRenderer::CDocBuilder::Initialize();
	}
	void CWrapper::Destroy()
	{
		NSDoctRenderer::CDocBuilder::Dispose();
	}

}

