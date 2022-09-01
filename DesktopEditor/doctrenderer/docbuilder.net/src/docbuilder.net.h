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

// docbuilder.net.h

#include "../../docbuilder.h"

#pragma once

using namespace System;

namespace docbuilder_net
{
	ref class CDocBuilderValue;
	ref class CDocBuilder;
	ref class CDocBuilderContextScope;
	ref class CDocBuilderContext;
	
	/// <summary>
	/// Class for getting the results of called JS commands.
	/// It represents a wrapper for a JS object.
	/// </summary>
	public ref class CDocBuilderValue
	{
	public:
		CDocBuilderValue();
		CDocBuilderValue(const CDocBuilderValue% other);
		~CDocBuilderValue();

	protected:
		!CDocBuilderValue();

	public:
		/// <returns> True if this object is empty. </returns>
		bool IsEmpty();

		/// <summary> Clears the object. </summary>
		void Clear();

		/// <returns> True if this object is null. </returns>
		bool IsNull();

		/// <returns> True if this object is undefined. </returns>
		bool IsUndefined();

		/// <returns> True if this object is an integer. </returns>
		bool IsInt();

		/// <returns> True if this object is a double value. </returns>
		bool IsDouble();

		/// <returns> True if this object is a string. </returns>
		bool IsString();

		/// <returns> True if this object is a function. </returns>
		bool IsFunction();

		/// <returns> True if this object is an object. </returns>
		bool IsObject();

		/// <returns> True if this object is an array. </returns>
		bool IsArray();

		/// <returns> True if this object is a typed array. </returns>
		bool IsTypedArray();


		/// <returns> The length if this object is an array/typed array. Otherwise, returns 0. </returns>
		unsigned int GetLength();


		/// <summary> Converts this object to a boolean value. </summary>
		bool ToBool();

		/// <summary> Converts this object to an integer. </summary>
		int ToInt();

		/// <summary> Converts this object to a double value. </summary>
		double ToDouble();

		/// <summary> Converts this object to a string. </summary>
		String^ ToString() override;

		
		/// <summary> Returns a property of this object. </summary>
		/// <param name="name"> The name of the CDocBuilderValue object property. </param>
		CDocBuilderValue^ GetProperty(String^ name);

		/// <summary> Analogue of the GetProperty method. </summary>
		/// <param name="name"> The name of the CDocBuilderValue object property. </param>
		CDocBuilderValue^ Get(String^ name);

		/// <summary> Returns an array value by its index. </summary>
		/// <param name="index"> The index of the array value. </param>
		CDocBuilderValue^ Get(int index);

		
		/// <summary> Sets a property to this object. </summary>
		/// <param name="name"> The name of the CDocBuilderValue object property. </param>
		/// <param name="value"> The value of the CDocBuilderValue object property. </param>
		void SetProperty(String^ name, CDocBuilderValue^ value);

		/// <summary> Analogue of the SetProperty method. </summary>
		/// <param name="name"> The name of the CDocBuilderValue object property. </param>
		/// <param name="value"> The value of the CDocBuilderValue object property. </param>
		void Set(String^ name, CDocBuilderValue^ value);

		/// <summary> Sets an array value by its index. </summary>
		/// <param name="index"> The index of the array value. </param>
		/// <param name="value"> The array value to be set. </param>
		void Set(int index, CDocBuilderValue^ value);


		/// <summary> Returns/Sets an array value by its index. </summary>
		/// <param name="index"> The index of the array value. </param>
		/// <param name="value"> The array value to be set. </param>
		property CDocBuilderValue^ default[int]
		{
			CDocBuilderValue ^ get(int index) { return Get(index); }
			void set(int index, CDocBuilderValue ^ value) { Set(index, value); }
		}

		/// <summary> The indexer analogue of the GetProperty/SetProperty methods. </summary>
		/// <param name="name"> The name of the CDocBuilderValue object property. </param>
		/// <param name="value"> The value of the CDocBuilderValue object property. </param>
		property CDocBuilderValue^ default[String^]
		{
			CDocBuilderValue ^ get(String^ name) { return Get(name); }
			void set(String^ name, CDocBuilderValue ^ value) { Set(name, value); }
		}
		
	public:
		/// <summary> Creates an object from primitive types. </summary>
		CDocBuilderValue(bool value);
		CDocBuilderValue(int value);
		CDocBuilderValue(unsigned int value);
		CDocBuilderValue(double value);
		CDocBuilderValue(String^ value);

		static operator CDocBuilderValue^ (bool value);
		static operator CDocBuilderValue^ (int value);
		static operator CDocBuilderValue^ (unsigned int value);
		static operator CDocBuilderValue^ (double value);
		static operator CDocBuilderValue^ (String^ value);

		/// <summary>
		/// Please use CDocBuilderContext.CreateUndefined.
		/// Creates an undefined value. This method returns the current context and calls its CreateUndefined method.
		/// </summary>
		static CDocBuilderValue^ CreateUndefined();

		/// <summary>
		/// Please use CDocBuilderContext.CreateNull.
		/// Creates a null value. This method returns the current context and calls its CreateNull method.
		/// </summary>
		static CDocBuilderValue^ CreateNull();

	public:
		/// <summary> Calls the specified Document Builder method. </summary>
		/// <param name="name"> The name of the Document Builder method. </param>
		CDocBuilderValue^ Call(String^ name);

		/// <summary> Calls the specified Document Builder method. </summary>
		/// <param name="name"> The name of the Document Builder method. </param>
		/// <param name="p1"> The first parameter that the Document Builder method takes as an arguments. </param>
		CDocBuilderValue^ Call(String^ name, CDocBuilderValue^ p1);

		/// <summary> Calls the specified Document Builder method. </summary>
		/// <param name="name"> The name of the Document Builder method. </param>
		/// <param name="p1"> The first parameter that the Document Builder method takes as an arguments. </param>
		/// <param name="p2"> The second parameter that the Document Builder method takes as an arguments. </param>
		CDocBuilderValue^ Call(String^ name, CDocBuilderValue^ p1, CDocBuilderValue^ p2);

		/// <summary> Calls the specified Document Builder method. </summary>
		/// <param name="name"> The name of the Document Builder method. </param>
		/// <param name="p1"> The first parameter that the Document Builder method takes as an arguments. </param>
		/// <param name="p2"> The second parameter that the Document Builder method takes as an arguments. </param>
		/// <param name="p3"> The third parameter that the Document Builder method takes as an arguments. </param>
		CDocBuilderValue^ Call(String^ name, CDocBuilderValue^ p1, CDocBuilderValue^ p2, CDocBuilderValue^ p3);

		/// <summary> Calls the specified Document Builder method. </summary>
		/// <param name="name"> The name of the Document Builder method. </param>
		/// <param name="p1"> The first parameter that the Document Builder method takes as an arguments. </param>
		/// <param name="p2"> The second parameter that the Document Builder method takes as an arguments. </param>
		/// <param name="p3"> The third parameter that the Document Builder method takes as an arguments. </param>
		/// <param name="p4"> The fourth parameter that the Document Builder method takes as an arguments. </param>
		CDocBuilderValue^ Call(String^ name, CDocBuilderValue^ p1, CDocBuilderValue^ p2, CDocBuilderValue^ p3, CDocBuilderValue^ p4);

		/// <summary> Calls the specified Document Builder method. </summary>
		/// <param name="name"> The name of the Document Builder method. </param>
		/// <param name="p1"> The first parameter that the Document Builder method takes as an arguments. </param>
		/// <param name="p2"> The second parameter that the Document Builder method takes as an arguments. </param>
		/// <param name="p3"> The third parameter that the Document Builder method takes as an arguments. </param>
		/// <param name="p4"> The fourth parameter that the Document Builder method takes as an arguments. </param>
		/// <param name="p5"> The fifth parameter that the Document Builder method takes as an arguments. </param>
		CDocBuilderValue^ Call(String^ name, CDocBuilderValue^ p1, CDocBuilderValue^ p2, CDocBuilderValue^ p3, CDocBuilderValue^ p4, CDocBuilderValue^ p5);

		/// <summary> Calls the specified Document Builder method. </summary>
		/// <param name="name"> The name of the Document Builder method. </param>
		/// <param name="p1"> The first parameter that the Document Builder method takes as an arguments. </param>
		/// <param name="p2"> The second parameter that the Document Builder method takes as an arguments. </param>
		/// <param name="p3"> The third parameter that the Document Builder method takes as an arguments. </param>
		/// <param name="p4"> The fourth parameter that the Document Builder method takes as an arguments. </param>
		/// <param name="p5"> The fifth parameter that the Document Builder method takes as an arguments. </param>
		/// <param name="p6"> The sixth parameter that the Document Builder method takes as an arguments. </param>
		CDocBuilderValue^ Call(String^ name, CDocBuilderValue^ p1, CDocBuilderValue^ p2, CDocBuilderValue^ p3, CDocBuilderValue^ p4, CDocBuilderValue^ p5, CDocBuilderValue^ p6);

	internal:
		NSDoctRenderer::CDocBuilderValue* m_internal;
	};

	public ref class CDocBuilder
	{
	public:
		CDocBuilder();
		~CDocBuilder();

	protected:
		!CDocBuilder();

	public:
		/// <summary>
		/// Opens the document file which will be edited and saved afterwards.
		/// </summary>
		/// <param name="path"> The path to the file to be opened together with its name and extension. </param>
		/// <param name="params"> 
		/// The parameters needed for the correct file opening (most commonly, the encoding is used for the txt and csv file types or the delimiter for the csv files,
		/// for other file types this is just an empty string). The parameters are added in the form of XML tags, where m_nCsvTxtEncoding is used for the text encoding
		/// and m_nCsvDelimiter is used for the csv delimiter. The supported values for the csv delimiters include: 0 - no delimiter; 1 - tab; 2 - semicolon; 3 - colon; 4 - comma; 5 - space.
		/// </param>
		/// <returns> Process x2t return code. </returns>
		int OpenFile(String^ path, String^ params);

		/// <summary>
		/// Creates a new file. The type of the file which will be created needs to be set.
		/// </summary>
		/// <param name="type"> 
		/// The type of the file to be created set as a hexadecimal
		/// integer for the .Net code or docx, xlsx or pptx for the .docbuilder script file (see AVS_OFFICESTUDIO_FILE_XXX values). 
		/// </param>
		/// <returns> True if the operation is successful. </returns>
		bool CreateFile(int type);

		/// <summary>
		/// Sets the path to the folder where the program will temporarily save files needed for the program correct work.
		/// After the successful document file creation, all the files will be deleted from the folder.If no temporary folder is set, the system one will be used.
		/// </summary>
		/// <param name="folder"> The path to the folder where the temporary files will be saved. </param>
		void SetTmpFolder(String^ folder);


		/// <summary>
		/// Saves the file after all the changes are made. The type of the file which will be saved needs to be set.
		/// </summary>
		/// <param name="type">
		/// The type of the file to be saved set as a hexadecimal integer for the .Net code; for the .docbuilder script file the following values are possible:
		/// docx, odt, rtf, txt, pptx, xlsx, ods, csv, pdf (see AVS_OFFICESTUDIO_FILE_XXX values).
		/// </param>
		/// <param name="path"> The path to the file to be saved together with its name and extension. </param>
		/// <returns> Process x2t return code. </returns>
		bool SaveFile(int type, String^ path);

		/// <summary>
		/// Saves the file after all the changes are made. The type of the file which will be saved needs to be set.
		/// </summary>
		/// <param name="type">
		/// The type of the file to be saved set as a hexadecimal integer for the .Net code; for the .docbuilder script file the following values are possible:
		/// docx, odt, rtf, txt, pptx, xlsx, ods, csv, pdf (see AVS_OFFICESTUDIO_FILE_XXX values).
		/// </param>
		/// <param name="path"> The path to the file to be saved together with its name and extension. </param>
		/// <param name="params">
		/// The parameters needed for the correct file opening (most commonly, the encoding is used for the txt and csv file types or the delimiter for the csv files,
		/// for other file types this is just an empty string). The parameters are added in the form of XML tags, where m_nCsvTxtEncoding is used for the text encoding
		/// and m_nCsvDelimiter is used for the csv delimiter. The supported values for the csv delimiters include : 0 - no delimiter; 1 - tab; 2 - semicolon; 3 - colon; 4 - comma; 5 - space.
		/// When saving into an image file (png or jpg) for creating thumbnails, the additional parameters in the form of XML tags are used:
		/// m_oThumbnail - the core tag showing that the inner nodes will be used to create a thumbnail out of the document file;
		/// format - the image file format used to create a thumbnail (can be of the following values: 3 - for a JPG file, 4 - for a PNG file);
		/// aspect - the image aspect when creating a thumbnail from the document file (can be of the following values: 1 - will keep the original aspect, 0 - will stretch the image to fit the width and the height set below;
		/// first - whether only the first page or all the pages should be converted into a thumbnail (can be of the following values: true - only the first page will be converted, false - all the document pages will be used to create thumbnails, in this case the file will be saved as an archive of images, one for each page);
		/// width - the image width in pixels;
		/// height - the image height in pixels.
		/// For example: oBuilder.SaveFile(OFFICESTUDIO_FILE_IMAGE, L"thumbnail.png", "<m_oThumbnail><format>4</format><aspect>1</aspect><first>false</first><width>1000</width><height>1000</height></m_oThumbnail>");
		/// </param>
		/// <returns> Process x2t return code. </returns>
		bool SaveFile(int type, String^ path, String^ params);
		

		/// <summary>
		/// Closes the file to stop working with it. You can use a single ONLYOFFICE Document Builder instance
		/// to work with all your files, but you need to close the previous file before you can
		/// start working with the next one in this case.
		/// 
		/// Attention!!!
		/// There is always a chance that you will need to exchange the data among the documents.
		/// For example, you might need to open some document, copy style from it, or some data values,
		/// close the document, create a new one and paste the copied data to it or use it to form some report.
		/// But once you close the file (using the CloseFile method (or new OpenFile), all the variable data you worked
		/// with will be void and cleared, and you will not be able to use it with any other file.
		/// To exchange the data between the files GlobalVariable is introduced.
		/// What you need, is to create a variable in the first opened or created file using the GlobalVariable method, e.g.:
		/// builder.OpenFile("file1");
		/// var tmpVariable = Api.GetDocument().GetCommentsReport();
		/// GlobalVariable["CommentsReport"] = tmpVariable;
		/// builder.CloseFile();
		/// builder.OpenFile("file2");
		/// tmpVariable now undefined
		/// GlobalVariable["CommentsReport"] - valid object
		/// </summary>
		void CloseFile();

		/// <summary>
		/// Executes the command which will be used to create the document file (text document, spreadsheet, presentation, form document, PDF).
		/// </summary>
		/// <param name="command"> 
		/// The command in the form of JavaScript code which will be used to create the document file
		/// (in .Net, the escape character must be used when the command contains quotation symbols).
		/// </param>
		/// <returns> True if the operation is successful. </returns>
		bool ExecuteCommand(String^ command);

		/// <summary>
		/// Executes the command which will be used to create the document file (text document, spreadsheet, presentation, form document, PDF).
		/// </summary>
		/// <param name="command"> 
		/// The command in the form of JavaScript code which will be used to create the document file
		/// (in .Net, the escape character must be used when the command contains quotation symbols).
		/// </param>
		/// <param name="retValue"> The command return value. </param>
		/// <returns> True if the operation is successful. </returns>
		bool ExecuteCommand(String^ command, [Runtime::InteropServices::Out] CDocBuilderValue^% retValue);


		/// <summary>
		/// Runs the ONLYOFFICE Document Builder executable. If you do not want to write a .Net application,
		/// you can simply use the docbuilder.exe executable file and run it with the.docbuilder file as an argument,
		/// where all the code for the document file creation will be written. For .Net, create the CDocBuilder object
		/// and call the Run method with the path to the executable file from the sPath parameter.
		/// </summary>
		/// <param name="path"> The path to the ONLYOFFICE Document Builder executable. </param>
		/// <returns> True if the operation is successful. </returns>
		bool Run(String^ path);

		/// <summary>
		/// Runs all the commands for the document creation using a single command.
		/// Compared to CDocBuilder.ExecuteCommand where only one command at a time is allowed, CDocBuilder.RunText
		/// makes it possible to enter all the commands for the document creation at once.
		/// </summary>
		/// <param name="commands">
		/// The commands which will be used to create the document file.
		/// All the commands containing builder. are line separated, i.e. you cannot write them in one line, each command must start with its own line.
		/// </param>
		/// <returns> True if the operation is successful. </returns>
		bool RunText(String^ commands);

		/// <summary>
		/// Sets an argument to the builder class which can be trasferred to the program outside the CDocBuilder.ExecuteCommand method,
		/// i.e. either as an additional property when running ONLYOFFICE Document Builder executable file or as a part of program code, but not included into the document file script.
		/// </summary>
		/// <remarks>
		/// Supported properties:
		/// <para>
		/// --use-doctrenderer-scheme - Specifies if the doctrenderer mode is used when building a document or getting content from the editor when saving a file (true / false).
		/// Default: false
		/// </para>
		/// <para>
		/// --check-fonts - Specifies if the system fonts are cached for faster work (true / false).
		/// Default: true
		/// </para>
		/// <para>
		/// --work-directory - The path to the temporary directory.
		/// Default: ""
		/// </para>
		/// <para>
		/// --cache-scripts - Specifies if the sdkjs scripts are cached (true / false).
		/// Default: true
		/// </para>
		/// <para>
		/// --save-use-only-names - Specifies if the destination paths are used (for server work) (true / false).
		/// <example>
		/// For example: /home/user/1.txt = > /tmp/1.txt
		/// </example>
		/// Default: false
		/// </para>
		/// <para>
		/// --all-fonts-path - The path to the AllFonts.js script.
		/// --argument - The JSON argument which is sent to the global parameters of all the opened JS context.
		/// --fonts-system - Specifies if the system fonts are used (true / false).
		/// Default: true
		/// </para>
		/// --fonts-dir - The path to the additional fonts directory (may be many records).
		/// </remarks>
		/// <param name="param"> The parameter name, the value is always --argument. </param>
		/// <param name="value"> The parameter value which will be used in the document. </param>
		void SetProperty(String^ param, String^ value);


		/// <summary>
		/// Writes data to the log file. It is used for logs in JS code.
		/// <param name="path"> The path to the file where all the logs will be written. </param>
		/// <param name="value"> The data which will be written to the log file. </param>
		/// <param name="append"> Specifies if the new data will be appended to the already existing log file or a new file will be created. </param>
		/// </summary>
		void WriteData(String^ path, String^ value, bool append);

		/// <summary>
		/// Specifies if the doctrenderer mode is used when building 
		/// a document or getting content from the editor when saving a file.
		/// </summary>
		/// <returns> True if the doctrenderer mode is used on saving. See the --use-doctrenderer-scheme property. </returns>
		bool IsSaveWithDoctrendererMode();

		/// <summary> Returns the ONLYOFFICE Document Builder engine version. </summary>
		String^ GetVersion();

		/// <summary> Returns the current JS context. </summary>
		CDocBuilderContext^ GetContext();

	public:
		/// <summary>
		/// Initializes the ONLYOFFICE Document Builder as a library for the application to be able to work with it.
		/// This method just sets the directory to the main Document Builder resources (icu files, etc). If this method is not called,
		/// the Document Builder will find resources from the current process directory.
		/// </summary>
		/// <param name="directory"> The path to the main Document Builder resources. </param>
		static void Initialize(String^ directory);

		/// <summary>
		/// Initializes the ONLYOFFICE Document Builder as a library for the application to be able to work with it.
		/// This method just sets the directory to the main Document Builder resources (icu files, etc). If this method is not called,
		/// the Document Builder will find resources from the current process directory.
		/// </summary>
		static void Initialize();

		/// <summary>
		/// Unloads the ONLYOFFICE Document Builder from the application memory when it is no longer needed.
		/// Generally, there is no need to dispose JS before exiting
		/// the process, it should happen automatically. It should only be used if the process needs the resources taken up by JS.
		/// </summary>
		static void Destroy();
		
	internal:
		NSDoctRenderer::CDocBuilder* m_internal;
	};

	/// <summary>
	/// The stack-allocated class which sets the execution context for all operations executed within a local scope.
	/// All opened scopes will be closed automatically when the builder CloseFile method is called.
	/// </summary>
	public ref class CDocBuilderContextScope
	{
	public:
		CDocBuilderContextScope();
		CDocBuilderContextScope(const CDocBuilderContextScope% other);
		~CDocBuilderContextScope();

	protected:
		!CDocBuilderContextScope();

	public:
		/// <summary>
		/// Closes the current scope. 
		/// This method will be called automatically when the destructor is executed.
		/// </summary>
		void Close();

	internal:
		NSDoctRenderer::CDocBuilderContextScope* m_internal;
	};

	/// <summary>
	/// Class for getting JS context for working.
	/// </summary>
	public ref class CDocBuilderContext
	{
	public:
		CDocBuilderContext();
		CDocBuilderContext(const CDocBuilderContext% other);
		~CDocBuilderContext();

	protected:
		!CDocBuilderContext();

	public:
		/// <summary>
		/// Creates an undefined value, an analogue of undefined in JS.
		/// </summary>
		CDocBuilderValue^ CreateUndefined();

		/// <summary>
		/// Creates a null value, an analogue of null in JS.
		/// </summary>
		CDocBuilderValue^ CreateNull();

		/// <summary>
		/// Creates an empty object, an analogue of {} in JS.
		/// </summary>
		CDocBuilderValue^ CreateObject();

		/// <summary>
		/// Creates an array value, an analogue of new Array (length) in JS.
		/// </summary>
		/// <param name="length"> The array length. </param>
		CDocBuilderValue^ CreateArray(int length);

		/// <summary>
		/// Creates a Uint8Array value, an analogue of Uint8Array in JS.
		/// </summary>
		/// <param name="buffer"> The array buffer. </param>
		CDocBuilderValue^ CreateTypedArray(array<Byte>^ buffer);

		/// <summary>
		/// Returns the global object for the current context.
		/// </summary>
		CDocBuilderValue^ GetGlobal();

		/// <summary>
		/// Creates a context scope which sets the execution context for all operations executed within a local scope.
		/// </summary>
		CDocBuilderContextScope^ CreateScope();

		/// <summary>
		/// Checks for errors in JS. 
		/// The error message and call stack will be written to std::cerr.
		/// </summary>
		bool IsError();

	internal:
		NSDoctRenderer::CDocBuilderContext* m_internal;
	};
}
