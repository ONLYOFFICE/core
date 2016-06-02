// docbuilder.net.h

#pragma once

using namespace System;

namespace docbuilder_net
{
	ref class CDocBuilder_Private;
	public ref class CDocBuilder
	{
	public:
		CDocBuilder();
		~CDocBuilder();

		bool OpenFile(String^ path, String^ params);
		bool CreateFile(int type);
		void SetTmpFolder(String^ folder);
		bool SaveFile(int type, String^ path);
		void CloseFile();
		bool ExecuteCommand(String^ command);
		void SetProperty(String^ key, String^ value);

		bool Run(String^ path);
		bool RunText(String^ text_commands);

		static void Initialize();
		static void Destroy();
		
	private:
		CDocBuilder_Private^ m_pInternal;
	};
}
