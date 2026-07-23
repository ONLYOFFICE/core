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
#include "Biff12RecordFactory.h"
namespace XLSB {

    CBiff12RecordFactory* CBiff12RecordFactory::Instance()
    {
        static CBiff12RecordFactory factory;
        return &factory;
    }

    std::shared_ptr<CBiff12RecordBase> CBiff12RecordFactory::Create(int typeRecord) {
        CBiff12RecordBase* instance = nullptr;

        // find name in the registry and call factory method.
        auto it = factoryFunctionRegistry.find(typeRecord);
        if (it != factoryFunctionRegistry.end())
        {
            instance = it->second();
        }

        // wrap instance in a shared ptr and return
        if (instance != nullptr)
        {
            return std::shared_ptr<CBiff12RecordBase>(instance);
        }
        else
        {
            return nullptr;
        }
    }

    void CBiff12RecordFactory::RegisterFactoryFunction(int typeRecord, std::function<CBiff12RecordBase*(void)> classFactoryFunction)
    {
        // register the class factory function
        factoryFunctionRegistry.insert(std::make_pair(typeRecord, classFactoryFunction));
    }
}
