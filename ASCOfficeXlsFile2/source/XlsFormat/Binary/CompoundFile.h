/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#pragma once

#include "../../../Common/3dParty/pole/pole.h"

#include "BinSmartPointers.h"

#include <map>

namespace XLS
{


class CompoundFile
{
public:
	enum ReadWriteMode
	{
		cf_WriteMode,
		cf_ReadMode
	};

public:
	CompoundFile(const std::wstring & file_path, const ReadWriteMode mode);
	~CompoundFile();
	
	bool isError();

	CFStreamPtr getWorkbookStream();
	CFStreamPtr createWorkbookStream();

	CFStreamPtr getSummaryInformationStream();
	CFStreamPtr createSummaryInformationStream();
	void		closeSummaryInformationStream();

	CFStreamPtr getDocumentSummaryInformationStream();
	CFStreamPtr createDocumentSummaryInformationStream();
	void		closeDocumentSummaryInformationStream();

private:
	POLE::Stream* openStream		(const std::string & stream_name); // Opens a stream in the storage (shall be called not more than once per stream)
	POLE::Stream* createStream		(const std::string & stream_name); // Creates a new stream in the storage

	CFStreamPtr getNamedStream		(const std::string& name);
	CFStreamPtr createNamedStream	(const std::string& name);
	void		closeNamedStream	(const std::string& name);

private:
	std::map<std::string, CFStreamPtr>	streams;
	POLE::Storage						*storage_;
	ReadWriteMode						rwMode;
};

} // namespace XLS

