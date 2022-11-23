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

#include <memory>
#include <stdexcept>
#include <vector>
#include <exception>


template <class T>
using SVectorBase = std::vector<std::shared_ptr<T>>;

template <class T>
using SVectorBasePtr = std::shared_ptr<SVectorBase<T>>;

template <class T>
class SVector : public SVectorBasePtr<T>
{
public:
    SVector(bool bInit = true, size_t res = 0)
    {
        if (bInit || res)
            init(res);
    }

    SVector(const SVectorBasePtr<T>& oth) : SVectorBasePtr<T>(oth)
    {}

    void init(size_t res = 0)
    {
            SVectorBasePtr<T>::reset(new SVectorBase<T>(res));
    }

    inline bool isInit() const
    {
        return SVectorBasePtr<T>::get() != nullptr;
    }

    void canUse() const
    {
        if (!isInit())
            throw std::bad_cast();
    }

    size_t size() const
    {
        canUse();
        return SVectorBasePtr<T>::get()->size();
    }

    bool empty() const
    {
        canUse();
        return SVectorBasePtr<T>::get()->empty();
    }

    std::shared_ptr<T>& operator[](size_t index)
    {
        canUse();
        return SVectorBasePtr<T>::get()->operator[](index);
    }

    const std::shared_ptr<T>& operator[](size_t index) const
    {
        canUse();
        return SVectorBasePtr<T>::get()->operator[](index);
    }

    void push_back(const std::shared_ptr<T> &el)
    {
        if (!isInit())
            init();

        SVectorBasePtr<T>::get()->push_back(el);
    }

    void clear()
    {
        canUse();
        for (auto& pEl : *(SVectorBasePtr<T>::get()))
            pEl.reset();

        SVectorBasePtr<T>::get()->clear();
    }
};
