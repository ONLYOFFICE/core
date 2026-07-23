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

#ifndef JNI_EXCEPTIONS_UTILS_H
#define JNI_EXCEPTIONS_UTILS_H

#include <iostream>
#include <jni.h>
#include <JniLogUtils.h>

using namespace std;

#define THROW(ENV, CLASS, MESSAGE)                                    \
    jclass jc = ENV->FindClass(CLASS);                                \
    if (jc != NULL) {                                                 \
        ENV->ThrowNew(jc, MESSAGE);                                   \
    }

#define TRY_JNI(ENV, STUFF)                                           \
    try {                                                             \
        STUFF;                                                        \
    } catch (const std::bad_alloc& e){                                \
        THROW(ENV, "java/lang/OutOfMemoryError", e.what())            \
    } catch (const std::ios_base::failure& e) {                       \
        THROW(ENV, "java/io/IOException", e.what())                   \
    } catch (const std::exception& e) {                               \
        THROW(ENV, "java/lang/Exception", e.what())                   \
    } catch (...) {                                                   \
        THROW(ENV, "java/lang/Error", "Unidentified exception")       \
    }

#endif
