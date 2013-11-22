;------------------------------------------------------------------------------
;
; MAKE_COLORSPACE(NAME,STACK, BYTES,PIXELS,ROWS, FUNC, ARG1)
;
; This macro provides a assembler width/height scroll loop
; NAME		function name
; STACK		additional stack bytes required by FUNC
; BYTES		bytes-per-pixel for the given colorspace
; PIXELS	pixels (columns) operated on per FUNC call
; VPIXELS	vpixels (rows) operated on per FUNC call
; FUNC		conversion macro name; we expect to find FUNC_INIT and FUNC macros
; ARG1		argument passed to FUNC
; 
; throughout the FUNC the registers mean:
; eax		y_stride
; ebx		u_ptr
; ecx		v_ptr
; edx		x_stride
; esi		y_ptr
; edi		x_ptr
; ebp		width
;
;------------------------------------------------------------------------------
%macro		MAKE_COLORSPACE			8
%define NAME		%1
%define STACK		%2
%define BYTES		%3
%define PIXELS		%4
%define VPIXELS		%5
%define FUNC		%6
%define ARG1		%7
%define ARG2		%8
	; --- define function global/symbol
ALIGN 16
cglobal NAME
NAME:
	; --- init stack ---

%define pushsize	16
%define localsize	20 + STACK

%define vflip           esp + localsize + pushsize + 40
%define height          esp + localsize + pushsize + 36
%define width           esp + localsize + pushsize + 32
%define uv_stride       esp + localsize + pushsize + 28
%define y_stride        esp + localsize + pushsize + 24
%define v_ptr           esp + localsize + pushsize + 20
%define u_ptr           esp + localsize + pushsize + 16
%define y_ptr           esp + localsize + pushsize + 12
%define x_stride        esp + localsize + pushsize + 8
%define x_ptr           esp + localsize + pushsize + 4
%define _ip             esp + localsize + pushsize + 0

  push ebx    ;   esp + localsize + 16
  push esi    ;   esp + localsize + 8
  push edi    ;   esp + localsize + 4
  push ebp    ;   esp + localsize + 0

%define x_dif           esp + localsize - 4
%define y_dif           esp + localsize - 8
%define uv_dif          esp + localsize - 12
%define fixed_width     esp + localsize - 16
%define tmp_height      esp + localsize - 20

    sub esp, localsize

    ; --- init varibles ---
    
  mov eax, [width]          ; fixed width
  add eax, 15               ;
  and eax, ~15              ;
  mov [fixed_width],eax     ;

  mov ebx, [x_stride]       ;
%rep BYTES
  sub ebx, eax              ;
%endrep
  mov [x_dif], ebx          ; x_dif = x_stride - BYTES*fixed_width

  mov ebx, [y_stride]       ;
  sub ebx, eax              ;
  mov [y_dif], ebx          ; y_dif = y_stride - fixed_width

  mov ebx, [uv_stride]      ;
  mov ecx, eax              ;
  shr ecx, 1                ;
  sub ebx, ecx              ;
  mov [uv_dif], ebx         ; uv_dif = uv_stride - fixed_width/2

  mov esi, [y_ptr]          ; $esi$ = y_ptr
  mov edi, [x_ptr]          ; $edi$ = x_ptr
  mov edx, [x_stride]       ; $edx$ = x_stride
  mov ebp, [height]         ; $ebp$ = height


  mov ebx, [vflip]
  or ebx, ebx
  jz .dont_flip

    ; --- do flipping ---

  xor ebx,ebx
%rep BYTES
  sub ebx, eax
%endrep
  sub ebx, edx
  mov [x_dif], ebx          ; x_dif = -BYTES*fixed_width - x_stride

  mov eax, ebp
  sub eax, 1
  push edx                
  mul edx
  pop edx
  add edi, eax              ; $edi$ += (height-1) * x_stride

  neg edx                   ; x_stride = -x_stride

.dont_flip

    ; --- begin loop ---

  mov eax, [y_stride]       ; $eax$ = y_stride
  mov ebx, [u_ptr]          ; $ebx$ = u_ptr
  mov ecx, [v_ptr]          ; $ecx$ = v_ptr

  FUNC %+ _INIT ARG1, ARG2  ; call FUNC_INIT

.y_loop
  mov [tmp_height], ebp
  mov ebp, [fixed_width]

.x_loop
  FUNC ARG1, ARG2           ; call FUNC

  add edi, BYTES*PIXELS     ; x_ptr += BYTES*PIXELS
  add esi, PIXELS           ; y_ptr += PIXELS
  add ebx, PIXELS/2         ; u_ptr += PIXELS/2
  add ecx, PIXELS/2         ; v_ptr += PIXELS/2
        
  sub ebp, PIXELS           ; $ebp$ -= PIXELS
  jg .x_loop                ; if ($ebp$ > 0) goto .x_loop

  mov ebp, [tmp_height]
  add edi, [x_dif]          ; x_ptr += x_dif + (VPIXELS-1)*x_stride
  add esi, [y_dif]          ; y_ptr += y_dif + (VPIXELS-1)*y_stride
%rep VPIXELS-1
  add edi, edx            
  add esi, eax            
%endrep

  add ebx, [uv_dif]         ; u_ptr += uv_dif + ((VPIXELS/2)-1)*uv_stride
  add ecx, [uv_dif]         ; v_ptr += uv_dif + ((VPIXELS/2)-1)*uv_stride
%rep (VPIXELS/2)-1
  add ebx, [uv_stride]
  add ecx, [uv_stride]
%endrep

  sub ebp, VPIXELS          ; $ebp$ -= VPIXELS
  jg .y_loop                ; if ($ebp$ > 0) goto .y_loop

  ; cleanup stack & undef everything

  add esp, localsize
  pop ebp
  pop edi
  pop esi
  pop ebx

%undef vflip
%undef height
%undef width
%undef uv_stride
%undef y_stride
%undef v_ptr
%undef u_ptr
%undef y_ptr
%undef x_stride
%undef x_ptr
%undef _ip
%undef x_dif
%undef y_dif
%undef uv_dif
%undef fixed_width
%undef tmp_height
        ret
%undef NAME
%undef STACK
%undef BYTES
%undef PIXELS
%undef VPIXELS
%undef FUNC
%undef ARG1
%endmacro
;------------------------------------------------------------------------------
