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
#ifndef BIFF12RECORDFACTORY_H
#define BIFF12RECORDFACTORY_H

#include <unordered_map>
#include <functional>
#include <memory>
#include "Biff12RecordBase.h"

namespace XLSB
{
    class CBiff12RecordFactory
    {
       std::unordered_map<int, std::function<CBiff12RecordBase*(void)>> factoryFunctionRegistry;

    public:
        static CBiff12RecordFactory* Instance();
        std::shared_ptr<CBiff12RecordBase> Create(int typeRecord);
        void RegisterFactoryFunction(int typeRecord, std::function<CBiff12RecordBase*(void)> classFactoryFunction);
    };

    template<class T> class Registrar
    {
    public:
        Registrar(int typeRecord)
        {
            // register the class factory function
            CBiff12RecordFactory::Instance()->RegisterFactoryFunction(typeRecord,
                    [](void) -> CBiff12RecordBase * { return new T();});
        }
    };
}
#endif // BIFF12RECORDFACTORY_H
