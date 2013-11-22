;/****************************************************************************
; *
; *  XVID MPEG-4 VIDEO CODEC
; *  - CPUID check processors capabilities -
; *
; *  Copyright (C) 2001 Michael Militzer <isibaar@xvid.org>
; *
; *  This program is free software ; you can redistribute it and/or modify
; *  it under the terms of the GNU General Public License as published by
; *  the Free Software Foundation ; either version 2 of the License, or
; *  (at your option) any later version.
; *
; *  This program is distributed in the hope that it will be useful,
; *  but WITHOUT ANY WARRANTY ; without even the implied warranty of
; *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; *  GNU General Public License for more details.
; *
; *  You should have received a copy of the GNU General Public License
; *  along with this program ; if not, write to the Free Software
; *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
; *
; * $Id$
; *
; ***************************************************************************/

BITS 32

%macro cglobal 1
	%ifdef PREFIX
		global _%1
		%define %1 _%1
	%else
		global %1
	%endif
%endmacro

;=============================================================================
; Constants
;=============================================================================

%define CPUID_TSC               0x00000010
%define CPUID_MMX               0x00800000
%define CPUID_SSE               0x02000000
%define CPUID_SSE2              0x04000000

%define EXT_CPUID_3DNOW         0x80000000
%define EXT_CPUID_AMD_3DNOWEXT  0x40000000
%define EXT_CPUID_AMD_MMXEXT    0x00400000

;;; NB: Make sure these defines match the ones defined in xvid.h
%define XVID_CPU_MMX      (1<< 0)
%define XVID_CPU_MMXEXT   (1<< 1)
%define XVID_CPU_SSE      (1<< 2)
%define XVID_CPU_SSE2     (1<< 3)
%define XVID_CPU_3DNOW    (1<< 4)
%define XVID_CPU_3DNOWEXT (1<< 5)
%define XVID_CPU_TSC      (1<< 6)

;=============================================================================
; Read only data
;=============================================================================

ALIGN 32
%ifdef FORMAT_COFF
SECTION .rodata
%else
SECTION .rodata align=16
%endif

vendorAMD:
		db "AuthenticAMD"

;=============================================================================
; Macros
;=============================================================================

%macro  CHECK_FEATURE         3
  mov ecx, %1
  and ecx, edx
  neg ecx
  sbb ecx, ecx
  and ecx, %2
  or %3, ecx
%endmacro

;=============================================================================
; Code
;=============================================================================

SECTION .text

; int check_cpu_feature(void)

cglobal fastconvert_check_cpu_features
fastconvert_check_cpu_features:

  push ebx
  push esi
  push edi
  push ebp

  sub esp, 12             ; Stack space for vendor name
  
  xor ebp, ebp

	; CPUID command ?
  pushfd
  pop eax
  mov ecx, eax
  xor eax, 0x200000
  push eax
  popfd
  pushfd
  pop eax
  cmp eax, ecx

  jz near .cpu_quit		; no CPUID command -> exit


	; get vendor string, used later
  xor eax, eax
  cpuid
  mov [esp], ebx       ; vendor string
  mov [esp+4], edx
  mov [esp+8], ecx
  test eax, eax

  jz near .cpu_quit

  mov eax, 1
  cpuid

 ; RDTSC command ?
  CHECK_FEATURE CPUID_TSC, XVID_CPU_TSC, ebp

  ; MMX support ?
  CHECK_FEATURE CPUID_MMX, XVID_CPU_MMX, ebp

  ; SSE support ?
  CHECK_FEATURE CPUID_SSE, (XVID_CPU_MMXEXT|XVID_CPU_SSE), ebp

  ; SSE2 support?
  CHECK_FEATURE CPUID_SSE2, XVID_CPU_SSE2, ebp

  ; extended functions?
  mov eax, 0x80000000
  cpuid
  cmp eax, 0x80000000
  jbe near .cpu_quit

  mov eax, 0x80000001
  cpuid

 ; AMD cpu ?
  lea esi, [vendorAMD]
  lea edi, [esp]
  mov ecx, 12
  cld
  repe cmpsb
  jnz .cpu_quit

  ; 3DNow! support ?
  CHECK_FEATURE EXT_CPUID_3DNOW, XVID_CPU_3DNOW, ebp

  ; 3DNOW extended ?
  CHECK_FEATURE EXT_CPUID_AMD_3DNOWEXT, XVID_CPU_3DNOWEXT, ebp

  ; extended MMX ?
  CHECK_FEATURE EXT_CPUID_AMD_MMXEXT, XVID_CPU_MMXEXT, ebp

.cpu_quit:

  mov eax, ebp

  add esp, 12

  pop ebp
  pop edi
  pop esi
  pop ebx

  ret

; sse/sse2 operating support detection routines
; these will trigger an invalid instruction signal if not supported.
ALIGN 16
cglobal fastconvert_sse_os_trigger
fastconvert_sse_os_trigger:
  xorps xmm0, xmm0
  ret


ALIGN 16
cglobal fastconvert_sse2_os_trigger
fastconvert_sse2_os_trigger:
  xorpd xmm0, xmm0
  ret


; enter/exit mmx state
ALIGN 16
cglobal fastconvert_emms_mmx
fastconvert_emms_mmx:
  emms
  ret

; faster enter/exit mmx state
ALIGN 16
cglobal fastconvert_emms_3dn
fastconvert_emms_3dn:
  femms
  ret
