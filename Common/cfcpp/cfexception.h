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
#pragma once

#include <exception>
#include <string>

namespace CFCPP
{
class CFException : virtual public std::exception
{
public:
    CFException() {}
    CFException(std::string message) : errorMessage(message) {}
    CFException(std::string message, std::exception& ex) : std::exception(ex), errorMessage(message) {}
    CFException(std::wstring message) : werrorMessage(message) {}
    CFException(std::wstring message, std::exception& ex) : std::exception(ex), werrorMessage(message) {}

    virtual ~CFException() throw () {}

    virtual const char* what() const throw () {
        return errorMessage.c_str();
    }
    virtual const wchar_t* what_w() const throw () {
        return werrorMessage.c_str();
    }

protected:
    std::string errorMessage;
    std::wstring werrorMessage;
};


class CFFileFormatException : public CFException
{
public:
    CFFileFormatException() {}
    CFFileFormatException(std::string message) :
        CFException(message)
    {}
    virtual ~CFFileFormatException() throw () {}
};

class CFDisposedException : public CFException
{
public:
    CFDisposedException() {}
    CFDisposedException(std::string message) :
        CFException(message)
    {}
    virtual ~CFDisposedException() throw () {}
};

class CFInvalidOperation : public CFException
{
public:
    CFInvalidOperation() {}
    CFInvalidOperation(std::string message) :
        CFException(message)
    {}
    virtual ~CFInvalidOperation() throw () {}
};

class CFCorruptedFileException : public CFException
{
public:
    CFCorruptedFileException() {}
    CFCorruptedFileException(std::string message) :
        CFException(message)
    {}
    virtual ~CFCorruptedFileException() throw () {}
};

class CFDuplicatedItemException : public CFException
{
public:
    CFDuplicatedItemException() {}
    CFDuplicatedItemException(std::wstring message) :
        CFException(message)
    {}
    virtual ~CFDuplicatedItemException() throw () {}
};
class CFItemNotFound : public CFException
{
public:
    CFItemNotFound() {}
    CFItemNotFound(std::wstring message) :
        CFException(message)
    {}
    virtual ~CFItemNotFound() throw () {}
};


}
