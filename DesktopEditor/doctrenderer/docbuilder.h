/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#ifndef DOCBUILDER_H
#define DOCBUILDER_H

#include "./common.h"
#include <stddef.h>

namespace NSDoctRenderer
{
	/**
	 * The string class with the wchar_t* property.
	 */
	class CString_Private;
	class Q_DECL_EXPORT CString
	{
	public:
		CString();
		CString(const CString& src);
		CString& operator=(const CString& src);
		~CString();

		wchar_t* c_str() const;

	private:
		CString_Private* m_internal;
		friend class CDocBuilderValue;
	};

	/**
	 * Class for getting the results of called js commands.
	 * It represents a wrapper for a js object.
	 */
	class CDocBuilderValue_Private;
	class Q_DECL_EXPORT CDocBuilderValue
	{
	public:
		CDocBuilderValue();
		CDocBuilderValue(const CDocBuilderValue& src);
		CDocBuilderValue& operator=(const CDocBuilderValue& src);
		~CDocBuilderValue();
		void* private_get_internal();

	public:
		/**
		 * Returns true if this object is empty.
		 */
		bool IsEmpty();
		/**
		 * Clears the object.
		 */
		void Clear();

		/**
		 * Returns true if this object is null.
		 */
		bool IsNull();
		/**
		 * Returns true if this object is undefined.
		 */
		bool IsUndefined();
		/**
		 * Returns true if this object is a boolean value.
		 */
		bool IsBool();
		/**
		 * Returns true if this object is an integer.
		 */
		bool IsInt();
		/**
		 * Returns true if this object is a double value.
		 */
		bool IsDouble();
		/**
		 * Returns true if this object is a string.
		 */
		bool IsString();
		/**
		 * Returns true if this object is a function.
		 */
		bool IsFunction();
		/**
		 * Returns true if this object is an object.
		 */
		bool IsObject();

		/**
		 * Returns true if this object is an array.
		 */
		bool IsArray();
		/**
		 * Returns true if this object is a typed array.
		 */
		bool IsTypedArray();

		/**
		 * Returns the length if this object is an array/typed array. Otherwise, returns 0.
		 */
		unsigned int GetLength();

		/**
		 * Converts this object to a boolean value.
		 */
		bool ToBool();
		/**
		 * Converts this object to an integer.
		 */
		int ToInt();
		/**
		 * Converts this object to a double value.
		 */
		double ToDouble();
		/**
		 * Converts this object to a string.
		 */
		CString ToString();

		/**
		 * Returns a property of this object.
		 */
		CDocBuilderValue GetProperty(const wchar_t* name);

		/**
		 * Analogues of the GetProperty method.
		 */
		CDocBuilderValue Get(const char* name);
		CDocBuilderValue Get(const wchar_t* name);
		CDocBuilderValue operator[](const char* name);
		CDocBuilderValue operator[](const wchar_t* name);

		/**
		 * Returns an array value by its index.
		 */
		CDocBuilderValue Get(const int& index);
		CDocBuilderValue operator[](const int& index);

		/**
		 * Sets a property to this object.
		 */
		void SetProperty(const wchar_t* name, CDocBuilderValue value);
		void Set(const wchar_t* name, CDocBuilderValue value);
		void Set(const int& index, CDocBuilderValue value);

	public:
		// Creates an object from primitive types
		CDocBuilderValue(const bool& value);
		CDocBuilderValue(const int& value);
		CDocBuilderValue(const unsigned int& value);
		CDocBuilderValue(const double& value);
		CDocBuilderValue(const char* value);
		CDocBuilderValue(const wchar_t* value);

		/**
		 * Please use CDocBuilderContext::CreateUndefined
		 * This method returns the current context and calls its method.
		 */
		static CDocBuilderValue CreateUndefined();
		/**
		 * Please use CDocBuilderContext::CreateNull
		 * This method returns the current context and calls its method.
		 */
		static CDocBuilderValue CreateNull();

	public:
		/**
		 * The object methods for calling functions that make writing code easier.
		 */
		CDocBuilderValue Call(const char* name);
		CDocBuilderValue Call(const char* name, CDocBuilderValue p1);
		CDocBuilderValue Call(const char* name, CDocBuilderValue p1, CDocBuilderValue p2);
		CDocBuilderValue Call(const char* name, CDocBuilderValue p1, CDocBuilderValue p2, CDocBuilderValue p3);
		CDocBuilderValue Call(const char* name, CDocBuilderValue p1, CDocBuilderValue p2, CDocBuilderValue p3, CDocBuilderValue p4);
		CDocBuilderValue Call(const char* name, CDocBuilderValue p1, CDocBuilderValue p2, CDocBuilderValue p3, CDocBuilderValue p4, CDocBuilderValue p5);
		CDocBuilderValue Call(const char* name, CDocBuilderValue p1, CDocBuilderValue p2, CDocBuilderValue p3, CDocBuilderValue p4, CDocBuilderValue p5, CDocBuilderValue p6);

		CDocBuilderValue Call(const wchar_t* name);
		CDocBuilderValue Call(const wchar_t* name, CDocBuilderValue p1);
		CDocBuilderValue Call(const wchar_t* name, CDocBuilderValue p1, CDocBuilderValue p2);
		CDocBuilderValue Call(const wchar_t* name, CDocBuilderValue p1, CDocBuilderValue p2, CDocBuilderValue p3);
		CDocBuilderValue Call(const wchar_t* name, CDocBuilderValue p1, CDocBuilderValue p2, CDocBuilderValue p3, CDocBuilderValue p4);
		CDocBuilderValue Call(const wchar_t* name, CDocBuilderValue p1, CDocBuilderValue p2, CDocBuilderValue p3, CDocBuilderValue p4, CDocBuilderValue p5);
		CDocBuilderValue Call(const wchar_t* name, CDocBuilderValue p1, CDocBuilderValue p2, CDocBuilderValue p3, CDocBuilderValue p4, CDocBuilderValue p5, CDocBuilderValue p6);

	private:
		CDocBuilderValue_Private* m_internal;

		friend class CDocBuilderContext;
	};

	/**
	 * Stack-allocated class which sets the execution context for all operations executed within a local scope.
	 * All opened scopes will be closed automatically when the builder CloseFile method is called.
	 */
	class CDocBuilderContextScope_Private;
	class Q_DECL_EXPORT CDocBuilderContextScope
	{
	public:
		CDocBuilderContextScope();
		CDocBuilderContextScope(const CDocBuilderContextScope& src);
		CDocBuilderContextScope& operator=(const CDocBuilderContextScope& src);
		~CDocBuilderContextScope();

		/**
		 * Closes the current scope. This method will be called automatically when the destructor is executed.
		 */
		void Close();

	private:
		CDocBuilderContextScope_Private* m_internal;

		friend class CDocBuilderContext;
	};

	/**
	 * Class for getting js context for working.
	 */
	class CDocBuilderContext_Private;
	class Q_DECL_EXPORT CDocBuilderContext
	{
	public:
		CDocBuilderContext();
		CDocBuilderContext(const CDocBuilderContext& src);
		CDocBuilderContext& operator=(const CDocBuilderContext& src);
		~CDocBuilderContext();

		/*
		 * Creates an undefined value, an analogue of undefined in js.
		 */
		CDocBuilderValue CreateUndefined();
		/*
		 * Creates a null value, an analogue of null in js.
		 */
		CDocBuilderValue CreateNull();
		/*
		 * Creates an empty object, an analogue of {} in js.
		 */
		CDocBuilderValue CreateObject();
		/*
		 * Creates an array value, an analogue of new Array(length) in js.
		 */
		CDocBuilderValue CreateArray(const int& length);
		/*
		 * Creates a Uint8Array value, an analogue of Uint8Array in js. If externalize == true, the application needs to release the memory after freeing the Uint8Array.
		 * Otherwise, the memory will be released automatically. In this case, the buffer must be created with the AllocMemoryTypedArray method.
		 */
		CDocBuilderValue CreateTypedArray(unsigned char* buffer, const int& length, const bool& externalize);

		/*
		 * Method to allocate the memory for a typed array.
		 */
		static unsigned char* AllocMemoryTypedArray(const size_t& size);
		/*
		 * Method to free the memory for a typed array.
		 */
		static void FreeMemoryTypedArray(unsigned char* data, const size_t& size);

		/*
		 * Returns the global object for the context.
		 */
		CDocBuilderValue GetGlobal();

		/*
		 * Creates a context scope.
		 */
		CDocBuilderContextScope CreateScope();

		/*
		 * Checks for errors in js. The error message and call stack will be written to std::cerr.
		 */
		bool IsError();

	private:
		CDocBuilderContext_Private* m_internal;

		friend class CDocBuilder_Private;
		friend class CDocBuilder;
	};

	/**
	 * Base class used by ONLYOFFICE Document Builder for the document file (text document, spreadsheet, PDF) to be generated.
	 */
	class CDocBuilder_Private;
	class Q_DECL_EXPORT CDocBuilder
	{
	public:
		CDocBuilder();
		~CDocBuilder();

	public:
		/**
		 * Opens the document file which will be edited and saved afterwards.
		 * @param path The path to the file to be opened together with its name and extension.
		 * @param params The parameters needed for the correct file opening (most commonly the encoding is used for the txt and csv file types or the delimiter for the csv files,
		 * for other file types this is just an empty string). The parameters are added in the form of XML tags, where m_nCsvTxtEncoding is used for the text encoding
		 * and m_nCsvDelimiter is used for the csv delimiter. The supported values for the csv delimiters include: 0 - no delimiter; 1 - tab; 2 - semicolon; 3 - colon; 4 - comma; 5 - space.
		 * @return Process x2t return code
		 */
		int OpenFile(const wchar_t* path, const wchar_t* params);
		/**
		 * Creates a new file. The type of the file which will be created needs to be set.
		 * @param type The type of the file to be created is specified as a hexadecimal integer for the C++ code or docx, xlsx or pptx for the .docbuilder script file (see AVS_OFFICESTUDIO_FILE_XXX values).
		 * @return True if the operation is successful
		 */
		bool CreateFile(const int& type);
		/**
		 * Sets the path to the folder where the program will temporarily save files needed for the program correct work.
		 * After the successful document file creation, all the files will be deleted from the folder. If no temporary folder is set, the system one will be used.
		 * @param folder The path to the folder where the temporary files will be saved.
		 */
		void SetTmpFolder(const wchar_t* folder);
		/**
		 * Saves the file after all the changes are made. The type of the file which will be saved needs to be set.
		 * @param type The type of the file to be saved is specified as a hexadecimal integer for the C++ code; for the .docbuilder script file the following values are possible:
		 * docx, odt, rtf, txt, pptx, xlsx, ods, csv, pdf (see AVS_OFFICESTUDIO_FILE_XXX values).
		 * @param path The path to the file to be saved together with its name and extension.
		 * @param params The parameters needed for the correct file opening (most commonly the encoding is used for the txt and csv file types or the delimiter for the csv files,
		 * for other file types this is just an empty string). The parameters are added in the form of XML tags, where m_nCsvTxtEncoding is used for the text encoding
		 * and m_nCsvDelimiter is used for the csv delimiter. The supported values for the csv delimiters include: 0 - no delimiter; 1 - tab; 2 - semicolon; 3 - colon; 4 - comma; 5 - space.
		 * When saving into an image file (png or jpg) for creating thumbnails, the additional parameters are used.
		 * @return Process x2t return code
		 */
		int SaveFile(const int& type, const wchar_t* path, const wchar_t* params = 0);
		/**
		 * Closes the file to stop working with it. You can use a single ONLYOFFICE Document Builder instance
		 * to work with all your files, but you need to close the previous file before you can
		 * start working with the next one in this case.
		 *
		 * Attention!!!
		 * There is always a chance that you will need to exchange the data among the documents.
		 * For example, you might need to open some document, copy style from it, or some data values,
		 * close the document, create a new one and paste the copied data to it or use it to form some report.
		 * But once you close the file (using the CloseFile method (or new OpenFile)), all the variable data you worked
		 * with will be void and cleared, and you will not be able to use it with any other file.
		 * To exchange the data between the files, GlobalVariable is introduced.
		 * What you need is to create a variable in the first opened or created file using the GlobalVariable method, e.g.:
		 * builder.OpenFile("file1");
		 * var tmpVariable = Api.GetDocument().GetCommentsReport();
		 * GlobalVariable["CommentsReport"] = tmpVariable;
		 * builder.CloseFile();
		 * builder.OpenFile("file2");
		 * tmpVariable now undefined
		 * GlobalVariable["CommentsReport"] - valid object
		 */
		void CloseFile();
		/**
		 * Executes the command which will be used to create the document file (text document, spreadsheet, PDF, etc.).
		 * @param command The command in the form of JavaScript code which will be used to create the document file (in C++, the escape character must be used when the command contains quotation symbols).
		 * @param retValue The command return value.
		 * @return True if the operation is successful
		 */
		bool ExecuteCommand(const wchar_t* command, CDocBuilderValue* retValue = 0);

		/**
		 * Runs ONLYOFFICE Document Builder executable. If you do not want to write a C++ application,
		 * you can simply use the docbuilder.exe executable file and run it with the .docbuilder file as an argument,
		 * where all the code for the document file creation will be written. For C++, use the path to the
		 * executable file from the sPath parameter, create the CDocBuilder object and call the Run method.
		 * @param path The path to the ONLYOFFICE Document Builder executable.
		 * @return True if the operation is successful
		 */
		bool Run(const wchar_t* path);

		/**
		 * Runs all the commands for the document creation using a single command.
		 * Compared to ExecuteCommand where only one command at a time is allowed, RunText(W)/(A)
		 * makes it possible to enter all the commands for the document creation at once.
		 * @param commands The commands in Unicode format which will be used to create the document file
		 * (in C++, the escape character must be used when the command contains quotation symbols).
		 * All the commands containing builder. are line separated, i.e. you cannot write them in one line, each command must start with its own line.
		 * @return True if the operation is successful
		 */
		bool RunTextW(const wchar_t* commands);
		bool RunTextA(const char* commands);

		/**
		 * Sets an argument to the builder class which can be trasferred to the program outside the CDocBuilder.ExecuteCommand method,
		 * i.e. either as an additional property when running ONLYOFFICE Document Builder executable file or as a part of program code, but not included into the document file script.
		 * Supported properties:
		 * --use-doctrenderer-scheme - Specifies if the doctrenderer mode is used when building a document or getting content from the editor when saving a file (true/false).
		 * Default: false
		 * --check-fonts - Specifies if the system fonts are cached for faster work (true/false).
		 * Default: true
		 * --work-directory - The path to the temporary directory.
		 * Default: ""
		 * --cache-scripts - Specifies if the sdkjs scripts are cached (true/false).
		 * Default: true
		 * --save-use-only-names - Specifies if the destination paths are used (for server work) (true/false). For example: /home/user/1.txt => /tmp/1.txt
		 * Default: false
		 * --all-fonts-path - The path to the AllFonts.js script.
		 * --argument - The JSON argument which is sent to the global parameters of all the opened js context.
		 * --fonts-system - Specifies if the system fonts are used (true/false).
		 * Default: true
		 * --fonts-dir - The path to the additional fonts directory (may be many records).
		 * @param param The parameter name in UTF8 format, the value is always --argument.
		 * @param value The parameter value in UTF8 format, which will be used in the document.
		 */
		void SetProperty(const char* param, const wchar_t* value);
		/**
		 * The Unicode version of the SetProperty method.
		 */
		void SetPropertyW(const wchar_t* param, const wchar_t* value);

		/**
		 * Writes data to the file. Used for logs in js code.
		 */
		void WriteData(const wchar_t* path, const wchar_t* value, const bool& append);

		/**
		 * Specifies if the doctrenderer mode is used when building a document or getting content from the editor when saving a file.
		 * @return True if the doctrenderer mode is used on saving. See the --use-doctrenderer-scheme property.
		 */
		bool IsSaveWithDoctrendererMode();

		/**
		 * Returns the document builder engine version.
		 * @return The document builder engine version
		 */
		char* GetVersion();

		/**
		 * Returns the current js context.
		 * @return The current js context
		 */
		CDocBuilderContext GetContext();

	public:
		/**
		 * Initializes the ONLYOFFICE Document Builder as a library for the application to be able to work with it.
		 * This method just sets the directory to the main builder resources (icu files and etc). If this method is not called,
		 * the builder will find resources from the current process directory.
		 */
		static void Initialize(const wchar_t* directory = 0);
		/**
		 * Unloads the ONLYOFFICE Document Builder from the application memory when it is no longer needed.
		 * Generally, there is no need to dispose js before exiting
		 * the process, it should happen automatically. It should only be used if the process needs the resources taken up by js.
		 */
		static void Dispose();

		/*
		 * Do not use this method. This is for internal needs.
		 */
		CDocBuilder_Private* GetPrivate() { return m_pInternal; }

	private:
		CDocBuilder_Private* m_pInternal;

		friend class CBuilderDocumentEmbed;
	};

	/**
	 * Attention!!!
	 * If you use the builder as both an application and a script, then you need to know some limitations.
	 *
	 * 1) docbuilder.exe "path-to-script.docbuilder"
	 * This is the same as
	 * NSDoctRenderer::CDocBuilder::Initialize();
	 * CDocBuilder builder;
	 * builder.Run("path-to-script.docbuilder");
	 * NSDoctRenderer::CDocBuilder::Dispose();
	 *
	 * 2) argument
	 * docbuilder.exe "--argument={\"company\":\"ONLYOFFICE\",\"product\":\"ONLYOFFICE Document Builder\"}" "path-to-script.docbuilder"
	 * This sets the --argument property to CDocBuilder & Run (see point 1)
	 * Using JavaScript:
	 * var sCompany   = Argument["company"];
	 * var sProduct = Argument["product"];
	 *
	 * 3) Methods of the "builder" object cannot be called with js variables.
	 * If necessary, wrap it in the "jsValue" instruction, e.g.:
	 * var jsVar = "123.docx";
	 * builder.SaveFile("docx", jsVar); // ERROR
	 * builder.SaveFile("docx", jsValue(jsVar)); // GOOD
	 *
	 * 4) For convenience, format types have been replaced with strings:
	 * builder.CreateFile("docx");
	 * This is the same as CDocBuilder.CreateFile(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX);
	 * builder.SaveFile("docx", file_path);
	 * This is the same as CDocBuilder.SaveFile("docx", file_path);
	 *
	 * All names:
	 * "docx": AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX;
	 * "doc": AVS_OFFICESTUDIO_FILE_DOCUMENT_DOC;
	 * "odt": AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT;
	 * "rtf": AVS_OFFICESTUDIO_FILE_DOCUMENT_RTF;
	 * "txt": AVS_OFFICESTUDIO_FILE_DOCUMENT_TXT;
	 * "pptx": AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX;
	 * "odp": AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP;
	 * "xlsx": AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX;
	 * "xls": AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLS;
	 * "ods": AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS;
	 * "csv": AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV;
	 * "pdf": AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF;
	 * "image": AVS_OFFICESTUDIO_FILE_IMAGE;
	 * "jpg": AVS_OFFICESTUDIO_FILE_IMAGE;
	 * "png": AVS_OFFICESTUDIO_FILE_IMAGE;
	 *
	 * 5) The entire JavaScript context is a wrapper over its native CDocBuilder object: builderJS.
	 * You need to work with it as with an ordinary JavaScript object. The jsValue instruction is not needed:
	 *
	 * builderJS methods:
	 * builderJS.OpenFile("path_or_url", "x2t_additons_as_xml");
	 * builderJS.CreateFile(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX); or builderJS.CreateFile("docx");
	 * builderJS.SetTmpFolder("folder");
	 * builderJS.SaveFile(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX, "path", "x2t_additons_as_xml"); or builderJS.SaveFile("docx", "path", "x2t_additons_as_xml");
	 * builderJS.CloseFile();
	 *
	 * Attention: The fourth rule does not work for the builderJS object.
	 *
	 * 6) The builder always works with one file.
	 * But there are times when you need to be able to open another file not for adding to the content,
	 * but for some manipulations (comparing, mailmerge). For such cases, the builder provides the OpenTmpFile method:
	 *
	 * var tmpFile = builderJS.OpenTmpFile("path_or_url");
	 *
	 * The tmpFile interface:
	 * IsValid() : returns true if the temporary file is valid
	 * GetBinary() : returns Uint8Array with the doct/pptt/xlst binary content
	 * GetFolder() : returns the string path to the temporary folder with the temporary file contents
	 * Close() : closes the file (removes the temporary folder contents)
	 * GetImageMap() : returns a dictionary object with the imageId -> imagePath key-value (for inserting into the document)
	 *
	 */

	// typedefs for shorter code
	typedef CDocBuilderValue CValue;
	typedef CDocBuilderContext CContext;
	typedef CDocBuilderContextScope CContextScope;
}

#endif // DOCBUILDER_H
