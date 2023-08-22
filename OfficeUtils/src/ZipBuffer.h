/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
#ifndef _ZIPBUFFER_H_
#define _ZIPBUFFER_H_

#include <string>
#include <vector>

#include "../../Common/kernel_config.h"
#include "../../DesktopEditor/common/Types.h"

// Работает с архивом в памяти
class KERNEL_DECL CZipBuffer
{
private:
	// Архивированные данные
	BYTE* m_zipFile;
	// Длина архивированных данных
	DWORD m_sizeZip;
	// Файл
	struct CFile
	{
		// Относительный путь файла в архиве
		std::string m_sPath;
		// Данные файла
		BYTE* m_pData;
		// Длина файла
		DWORD m_nLength;
		CFile(const std::string& sPath, BYTE* pData, DWORD nLength) : m_sPath(sPath), m_pData(pData), m_nLength(nLength) {}
	};
	// Вектор файлов в архиве
	std::vector<CFile> m_arrFiles;

	friend class CZipFolderMemory;
public:
	// Создает архив в памяти
	CZipBuffer()
	{
		create();
	}
	// Открывает архив в памяти, переданные данные необходимо освободить после использования класса
	CZipBuffer(BYTE* buffer, DWORD size)
	{
		open(buffer, size);
	}
	// Закрывает архив и очищает память
	~CZipBuffer()
	{
		close();
	}

	// Создает архив в памяти
	void create();
	// Открывает архив в памяти, переданные данные необходимо освободить после использования класса
	void open(BYTE* buffer, DWORD size);
	// Закрывает архив и очищает память
	void close();
	// Перемещает файл в архиве
	void move(const std::string& sSrc, const std::string& sDst);

	// Возвращает вектор путей в архиве
	std::vector<std::string> getPaths();
	// Сохраняет архив в переданную память, полученные данные необходимо освободить
	void save(BYTE*& data, DWORD& length);
	// По относительно пути в архиве возвращает файл, полученные данные будут освобождены после использования класса
	void getFile(const std::string& sPath, BYTE*& data, DWORD& length);
	// По относительно пути в архиве добавляет файл,  переданные данные будут освобождены после использования класса
	void addFile   (const std::string& sPath, BYTE* data, DWORD length);
	// По относительно пути в архиве удаляет файл
	bool removeFile(const std::string& sPath);
};

#endif // _ZIPBUFFER_H_
