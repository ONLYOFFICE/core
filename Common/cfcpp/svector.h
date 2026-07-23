/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
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
