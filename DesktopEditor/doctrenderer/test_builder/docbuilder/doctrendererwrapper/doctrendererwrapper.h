// doctrendererwrapper.h

#pragma once

using namespace System;

namespace doctrendererwrapper {

	ref class CWrapper_Private;
	public ref class CWrapper
	{
	public:
		CWrapper(bool bIsCheckSystemFonts);
		~CWrapper();

		bool OpenFile(String^ path, String^ params);
		bool CreateFile(int type);
		void SetTmpFolder(String^ folder);
		bool SaveFile(int type, String^ path);
		void CloseFile();
		bool ExecuteCommand(String^ command);

		bool Run(String^ path);
		bool RunText(String^ text_commands);

		static void Initialize();
		static void Destroy();
		
	private:
		CWrapper_Private^ m_pInternal;
	};
}
