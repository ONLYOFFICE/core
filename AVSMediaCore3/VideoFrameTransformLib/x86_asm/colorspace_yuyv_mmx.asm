;/****************************************************************************
; *
; *  XVID MPEG-4 VIDEO CODEC
; *  - MMX and XMM YUYV<->YV12 conversion -
; *
; *  Copyright(C) 2002 Peter Ross <pross@xvid.org>
; *
; *  This program is free software; you can redistribute it and/or modify it
; *  under the terms of the GNU General Public License as published by
; *  the Free Software Foundation; either version 2 of the License, or
; *  (at your option) any later version.
; *
; *  This program is distributed in the hope that it will be useful,
; *  but WITHOUT ANY WARRANTY; without even the implied warranty of
; *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; *  GNU General Public License for more details.
; *
; *  You should have received a copy of the GNU General Public License
; *  along with this program; if not, write to the Free Software
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
; Read only data
;=============================================================================

%ifdef FORMAT_COFF
SECTION .rodata
%else
SECTION .rodata align=16
%endif

;-----------------------------------------------------------------------------
; yuyv/uyvy mask for extracting yuv components
;-----------------------------------------------------------------------------
;				y     u     y     v     y     u     y     v

ALIGN 16
yuyv_mask:	db 0xff,  0,  0xff,   0,   0xff,  0,   0xff,  0
mmx_one:    dw 1, 1, 1, 1

;=============================================================================
; helper macros used with colorspace_mmx.inc
;=============================================================================

;-----------------------------------------------------------------------------
; YUYV_TO_YV12( TYPE, PAVG )
;
; TYPE	0=yuyv, 1=uyvy
; PAVG  0=mmx, pavgusb=3dnow, pavgb=xmm
;
; bytes=2, pixels = 8, vpixels=2
;-----------------------------------------------------------------------------

%macro YUYV_TO_YV12_INIT		2
  movq mm7, [yuyv_mask]
%endmacro


%macro YUYV_TO_YV12             2
  movq mm0, [edi]               ; x_ptr[0]
  movq mm1, [edi + 8]           ; x_ptr[8]
  movq mm2, [edi + edx]         ; x_ptr[x_stride + 0]
  movq mm3, [edi + edx + 8]     ; x_ptr[x_stride + 8]

    ; average uv-components
;---[ plain mmx ]----------------------------------------------------
%ifidn %2,0     ; if (%2 eq "0")
  movq mm4, mm0
  movq mm5, mm2
%if %1 == 0         ; yuyv
  psrlw mm4, 8
  psrlw mm5, 8
%endif
  pand mm4, mm7
  pand mm5, mm7
  paddw mm4, mm5

  movq mm5, mm1
  movq mm6, mm3
%if %1 == 0         ; yuyv
  psrlw mm5, 8
  psrlw mm6, 8
%endif
  pand mm5, mm7
  pand mm6, mm7
  paddw mm5, mm6
  paddw mm4, [mmx_one]      ; +1 rounding
  paddw mm5, [mmx_one]      ;
  psrlw mm4, 1
  psrlw mm5, 1
;---[ 3dnow/xmm ]----------------------------------------------------
%else
  movq mm4, mm0
  movq mm5, mm1
  %2 mm4, mm2           ;pavgb/pavgusb mm4, mm2
  %2 mm5, mm3           ;pavgb/pavgusb mm5, mm3

  ;;movq mm6, mm0       ; 0 rounding
  ;;pxor mm6, mm2       ;
  ;;psubb mm4, mm6      ;
  ;;movq mm6, mm1       ;
  ;;pxor mm6, mm3       ;
  ;;psubb mm5, mm5      ;

%if %1 == 0             ; yuyv
  psrlw mm4, 8
  psrlw mm5, 8
%endif
  pand mm4, mm7
  pand mm5, mm7
%endif
;--------------------------------------------------------------------

    ; write y-component
%if %1 == 1         ; uyvy
  psrlw mm0, 8
  psrlw mm1, 8
  psrlw mm2, 8
  psrlw mm3, 8
%endif
  pand mm0, mm7
  pand mm1, mm7
  pand mm2, mm7
  pand mm3, mm7
  packuswb mm0, mm1
  packuswb mm2, mm3

%ifidn %2,pavgb         ; xmm
  movntq [esi], mm0
  movntq [esi+eax], mm2
%else                   ; plain mmx,3dnow
  movq [esi], mm0
  movq [esi+eax], mm2
%endif

    ; write uv-components
  packuswb mm4, mm5
  movq mm5, mm4
  psrlq mm4, 8
  pand mm5, mm7
  pand mm4, mm7
  packuswb mm5,mm5
  packuswb mm4,mm4
  movd [ebx],mm5
  movd [ecx],mm4
%endmacro

;-----------------------------------------------------------------------------
; YV12_TO_YUYV( TYPE )
;
; TYPE  0=yuyv, 1=uyvy
;
; bytes=2, pixels = 8, vpixels=2
;-----------------------------------------------------------------------------

%macro YV12_TO_YUYV_INIT        2
%endmacro


%macro YV12_TO_YUYV             2
  movd mm4, [ebx]               ; [    |uuuu]
  movd mm5, [ecx]               ; [    |vvvv]
  movq mm0, [esi]               ; [yyyy|yyyy] ; y row 0
  movq mm1, [esi+eax]           ; [yyyy|yyyy] ; y row 1
  punpcklbw mm4, mm5            ; [vuvu|vuvu] ; uv row 0

%if %1 == 0     ; YUYV
  movq mm2, mm0
  movq mm3, mm1
  punpcklbw mm0, mm4            ; [vyuy|vyuy] ; y row 0 + 0
  punpckhbw mm2, mm4            ; [vyuy|vyuy] ; y row 0 + 8
  punpcklbw mm1, mm4            ; [vyuy|vyuy] ; y row 1 + 0
  punpckhbw mm3, mm4            ; [vyuy|vyuy] ; y row 1 + 8
  movq [edi], mm0
  movq [edi+8], mm2
  movq [edi+edx], mm1
  movq [edi+edx+8], mm3
%else           ; UYVY
  movq mm5, mm4
  movq mm6, mm4
  movq mm7, mm4
  punpcklbw mm4, mm0            ; [yvyu|yvyu]   ; y row 0 + 0
  punpckhbw mm5, mm0            ; [yvyu|yvyu]   ; y row 0 + 8
  punpcklbw mm6, mm1            ; [yvyu|yvyu]   ; y row 1 + 0
  punpckhbw mm7, mm1            ; [yvyu|yvyu]   ; y row 1 + 8
  movq [edi], mm4
  movq [edi+8], mm5
  movq [edi+edx], mm6
  movq [edi+edx+8], mm7
%endif
%endmacro

;------------------------------------------------------------------------------
; YV12_TO_YUYVI( TYPE )
;
; TYPE  0=yuyv, 1=uyvy
;
; bytes=2, pixels = 8, vpixels=4
;------------------------------------------------------------------------------

%macro YV12_TO_YUYVI_INIT       2
%endmacro

%macro YV12_TO_YUYVI                2
  xchg ebp, [uv_stride]
  movd mm0, [ebx]               ; [    |uuuu]
  movd mm1, [ebx+ebp]           ; [    |uuuu]
  punpcklbw mm0, [ecx]          ; [vuvu|vuvu] ; uv row 0
  punpcklbw mm1, [ecx+ebp]      ; [vuvu|vuvu] ; uv row 1
  xchg ebp, [uv_stride]

%if %1 == 0     ; YUYV
  movq mm4, [esi]               ; [yyyy|yyyy] ; y row 0
  movq mm6, [esi+eax]           ; [yyyy|yyyy] ; y row 1
  movq mm5, mm4
  movq mm7, mm6
  punpcklbw mm4, mm0            ; [yuyv|yuyv] ; y row 0 + 0
  punpckhbw mm5, mm0            ; [yuyv|yuyv] ; y row 0 + 8
  punpcklbw mm6, mm1            ; [yuyv|yuyv] ; y row 1 + 0
  punpckhbw mm7, mm1            ; [yuyv|yuyv] ; y row 1 + 8
  movq [edi], mm4
  movq [edi+8], mm5
  movq [edi+edx], mm6
  movq [edi+edx+8], mm7

  push esi
  push edi
  add esi, eax
  add edi, edx
  movq mm4, [esi+eax]           ; [yyyy|yyyy] ; y row 2
  movq mm6, [esi+2*eax]         ; [yyyy|yyyy] ; y row 3
  movq mm5, mm4
  movq mm7, mm6
  punpcklbw mm4, mm0            ; [yuyv|yuyv] ; y row 2 + 0
  punpckhbw mm5, mm0            ; [yuyv|yuyv] ; y row 2 + 8
  punpcklbw mm6, mm1            ; [yuyv|yuyv] ; y row 3 + 0
  punpckhbw mm7, mm1            ; [yuyv|yuyv] ; y row 3 + 8
  movq [edi+edx], mm4
  movq [edi+edx+8], mm5
  movq [edi+2*edx], mm6
  movq [edi+2*edx+8], mm7
  pop edi
  pop esi
%else           ; UYVY
  movq mm2, [esi]               ; [yyyy|yyyy] ; y row 0
  movq mm3, [esi+eax]           ; [yyyy|yyyy] ; y row 1
  movq mm4, mm0
  movq mm5, mm0
  movq mm6, mm1
  movq mm7, mm1
  punpcklbw mm4, mm2            ; [uyvy|uyvy] ; y row 0 + 0
  punpckhbw mm5, mm2            ; [uyvy|uyvy] ; y row 0 + 8
  punpcklbw mm6, mm3            ; [uyvy|uyvy] ; y row 1 + 0
  punpckhbw mm7, mm3            ; [uyvy|uyvy] ; y row 1 + 8
  movq [edi], mm4
  movq [edi+8], mm5
  movq [edi+edx], mm6
  movq [edi+edx+8], mm7

  push esi
  push edi
  add esi, eax
  add edi, edx
  movq mm2, [esi+eax]           ; [yyyy|yyyy] ; y row 2
  movq mm3, [esi+2*eax]         ; [yyyy|yyyy] ; y row 3
  movq mm4, mm0
  movq mm5, mm0
  movq mm6, mm1
  movq mm7, mm1
  punpcklbw mm4, mm2            ; [uyvy|uyvy] ; y row 2 + 0
  punpckhbw mm5, mm2            ; [uyvy|uyvy] ; y row 2 + 8
  punpcklbw mm6, mm3            ; [uyvy|uyvy] ; y row 3 + 0
  punpckhbw mm7, mm3            ; [uyvy|uyvy] ; y row 3 + 8
  movq [edi+edx], mm4
  movq [edi+edx+8], mm5
  movq [edi+2*edx], mm6
  movq [edi+2*edx+8], mm7
  pop edi
  pop esi
%endif
%endmacro

;=============================================================================
; Code
;=============================================================================

SECTION .text

%include ".\x86_asm\colorspace_mmx.inc"

; input

MAKE_COLORSPACE	 fastconvert_yuyv_to_yv12_mmx,0,    2,8,2,  YUYV_TO_YV12, 0, 0
MAKE_COLORSPACE	 fastconvert_yuyv_to_yv12_3dn,0,    2,8,2,  YUYV_TO_YV12, 0, pavgusb
MAKE_COLORSPACE	 fastconvert_yuyv_to_yv12_xmm,0,    2,8,2,  YUYV_TO_YV12, 0, pavgb

MAKE_COLORSPACE  fastconvert_uyvy_to_yv12_mmx,0,    2,8,2,  YUYV_TO_YV12, 1, 0
MAKE_COLORSPACE  fastconvert_uyvy_to_yv12_3dn,0,    2,8,2,  YUYV_TO_YV12, 1, pavgusb
MAKE_COLORSPACE  fastconvert_uyvy_to_yv12_xmm,0,    2,8,2,  YUYV_TO_YV12, 1, pavgb

; output

MAKE_COLORSPACE  fastconvert_yv12_to_yuyv_mmx,0,    2,8,2,  YV12_TO_YUYV, 0, -1
MAKE_COLORSPACE  fastconvert_yv12_to_uyvy_mmx,0,    2,8,2,  YV12_TO_YUYV, 1, -1

MAKE_COLORSPACE  fastconvert_yv12_to_yuyvi_mmx,0,   2,8,4,  YV12_TO_YUYVI, 0, -1
MAKE_COLORSPACE  fastconvert_yv12_to_uyvyi_mmx,0,   2,8,4,  YV12_TO_YUYVI, 1, -1
