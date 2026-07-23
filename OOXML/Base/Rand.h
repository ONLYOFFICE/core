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

#ifndef SECURE_RANDOM_BYTES_H
#define SECURE_RANDOM_BYTES_H

#include <random>
#include <mutex>
#include <cstdio>
#include <cstring>
#include <cstddef>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>

#if defined(WIN_XP_OR_VISTA)

// RtlGenRandom (a.k.a. SystemFunction036 in advapi32.dll).
// Available since Windows XP, cryptographically secure,
// does not open key containers — stable under load.
extern "C" BOOLEAN NTAPI SystemFunction036(PVOID RandomBuffer, ULONG RandomBufferLength);
#pragma comment(lib, "advapi32.lib")

#else

#include <bcrypt.h>
#pragma comment(lib, "bcrypt.lib")
#endif

#elif defined(_MAC)

#include <Security/Security.h>

#elif defined(_LINUX)

#include <unistd.h>
#include <errno.h>
#include <sys/syscall.h>

// Fallback syscall numbers for systems where SYS_getrandom is not in headers.
// getrandom() syscall added in Linux 3.17 (October 2014).

#ifndef SYS_getrandom

#if defined(__x86_64__)
#define SYS_getrandom 318
#elif defined(__i386__)
#define SYS_getrandom 355
#elif defined(__aarch64__)
#define SYS_getrandom 278
#elif defined(__arm__)
#define SYS_getrandom 384
#elif defined(__powerpc64__)
#define SYS_getrandom 359
#elif defined(__riscv)
#define SYS_getrandom 278
#endif

#endif

#endif


// Fills `buf` with `len` cryptographically secure random bytes.
// Returns true on success, false if every available source failed.
static bool SecureRandomBytes(unsigned char* buf, size_t len)
{
	if (len == 0)
		return true;
	if (buf == nullptr)
		return false;

	size_t filled = 0;

#if defined(_WIN32) || defined(_WIN64)

#if defined(WIN_XP_OR_VISTA)
	while (filled < len)
	{
		ULONG chunk = (len - filled > 0xFFFFFF00u) ? 0xFFFFFF00u : static_cast<ULONG>(len - filled);
		if (!::SystemFunction036(buf + filled, chunk))
			break;
		filled += chunk;
	}
	if (filled == len)
		return true;
#else
	while (filled < len)
	{
		ULONG chunk = (len - filled > 0xFFFFFF00u) ? 0xFFFFFF00u : static_cast<ULONG>(len - filled);
		NTSTATUS status = ::BCryptGenRandom(nullptr, buf + filled, chunk, BCRYPT_USE_SYSTEM_PREFERRED_RNG);
		if (status != 0) // STATUS_SUCCESS
			break;
		filled += chunk;
	}
	if (filled == len)
		return true;
#endif

#elif defined(_MAC)
	if (::SecRandomCopyBytes(kSecRandomDefault, len, buf) == errSecSuccess)
		return true;

#elif defined(__ANDROID__) || defined(__linux__)
	// ===== Linux / Android =====
	// Direct syscall — does not depend on glibc version or headers.
	// On kernels older than 3.17 syscall returns ENOSYS and we fall through.
#ifdef SYS_getrandom
	while (filled < len) {
		size_t chunk = (len - filled > 256) ? 256 : (len - filled);
		long ret = ::syscall(SYS_getrandom, buf + filled, chunk, 0);
		if (ret < 0) {
			if (errno == EINTR) continue;
			break;  // ENOSYS or other error — fall through to /dev/urandom
		}
		filled += static_cast<size_t>(ret);
	}
	if (filled == len) return true;
#endif
#endif

	// POSIX fallback: /dev/urandom
#if !defined(_WIN32) && !defined(_WIN64)
	{
		FILE* f = std::fopen("/dev/urandom", "rb");
		if (f) {
			while (filled < len) {
				size_t n = std::fread(buf + filled, 1, len - filled, f);
				if (n == 0) break;
				filled += n;
			}
			std::fclose(f);
			if (filled == len) return true;
		}
	}
#endif

	// Final fallback: std::random_device
	try
	{
		static std::mutex m;
		static std::random_device rd;
		std::lock_guard<std::mutex> lock(m);
		while (filled < len)
		{
			unsigned int r = rd();
			size_t to_copy = (len - filled < sizeof(r)) ? (len - filled) : sizeof(r);
			std::memcpy(buf + filled, &r, to_copy);
			filled += to_copy;
		}
		return true;
	}
	catch (...)	{
		return false;
	}

	return false;
}

// Returns a cryptographically secure random unsigned int.
static unsigned int RandUInt()
{
	unsigned int result = 0;
	SecureRandomBytes(reinterpret_cast<unsigned char*>(&result), sizeof(result));
	return result;
}

#endif // SECURE_RANDOM_BYTES_H
