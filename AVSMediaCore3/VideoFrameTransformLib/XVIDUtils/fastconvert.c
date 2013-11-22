
void __declspec(naked) fastconvert_asm_blend_row_clipped(void* dst, void* src, int w, int srcpitch)
{
	__asm
	{
		push	ebp
		push	edi
		push	esi
		push	ebx

		mov		edi,[esp+20]
		mov		esi,[esp+24]
		sub		edi,esi
		mov		ebp,[esp+28]
		mov		edx,[esp+32]

xloop:
		mov		ecx,[esi]
		mov		eax,0xfefefefe

		mov		ebx,[esi+edx]
		and		eax,ecx

		shr		eax,1
		and		ebx,0xfefefefe

		shr		ebx,1
		add		esi,4

		add		eax,ebx
		dec		ebp

		mov		[edi+esi-4],eax
		jnz		xloop

		pop		ebx
		pop		esi
		pop		edi
		pop		ebp
		ret
	};
}

void __declspec(naked) fastconvert_asm_blend_row(void* dst, void* src, int w, int srcpitch)
{
	__asm
	{
		push	ebp
		push	edi
		push	esi
		push	ebx

		mov		edi,[esp+20]
		mov		esi,[esp+24]
		sub		edi,esi
		mov		ebp,[esp+28]
		mov		edx,[esp+32]

xloop:
		mov		ecx,[esi]
		mov		eax,0xfcfcfcfc

		mov		ebx,[esi+edx]
		and		eax,ecx

		shr		ebx,1
		mov		ecx,[esi+edx*2]

		shr		ecx,2
		and		ebx,0x7f7f7f7f

		shr		eax,2
		and		ecx,0x3f3f3f3f

		add		eax,ebx
		add		esi,4

		add		eax,ecx
		dec		ebp

		mov		[edi+esi-4],eax
		jnz		xloop

		pop		ebx
		pop		esi
		pop		edi
		pop		ebp
		ret
	};
}

void __declspec(naked) fastconvert_asm_blend_row_MMX(void* dst, void* src, int w, int srcpitch)
{
	static const __int64 mask0 = 0xfcfcfcfcfcfcfcfci64;
	static const __int64 mask1 = 0x7f7f7f7f7f7f7f7fi64;
	static const __int64 mask2 = 0x3f3f3f3f3f3f3f3fi64;
	__asm 
	{
		push	ebp
		push	edi
		push	esi
		push	ebx

		mov		edi,[esp+20]
		mov		esi,[esp+24]
		sub		edi,esi
		mov		ebp,[esp+28]
		mov		edx,[esp+32]

		movq	mm5,mask0
		movq	mm6,mask1
		movq	mm7,mask2
		shr		ebp,1
		jz		oddpart

xloop:
		movq	mm2,[esi]
		movq	mm0,mm5

		movq	mm1,[esi+edx]
		pand	mm0,mm2

		psrlq	mm1,1
		movq	mm2,[esi+edx*2]

		psrlq	mm2,2
		pand	mm1,mm6

		psrlq	mm0,2
		pand	mm2,mm7

		paddb	mm0,mm1
		add		esi,8

		paddb	mm0,mm2
		dec		ebp

		movq	[edi+esi-8],mm0
		jne		xloop

oddpart:
		test	byte ptr [esp+28],1
		jz		nooddpart

		mov		ecx,[esi]
		mov		eax,0fcfcfcfch
		mov		ebx,[esi+edx]
		and		eax,ecx
		shr		ebx,1
		mov		ecx,[esi+edx*2]
		shr		ecx,2
		and		ebx,07f7f7f7fh
		shr		eax,2
		and		ecx,03f3f3f3fh
		add		eax,ebx
		add		eax,ecx
		mov		[edi+esi],eax

nooddpart:

		pop		ebx
		pop		esi
		pop		edi
		pop		ebp
		ret
	};
}

void __declspec(naked) fastconvert_asm_middle_row(void* dst, void* src, int w, int srcpitch)
{
	__asm
	{
		push	ebp
		push	edi
		push	esi
		push	ebx

		mov		edi,[esp+20]	//dst
		mov		esi,[esp+24]	//src
		sub		edi,esi
		mov		ebp,[esp+28]	//w
		mov		edx,[esp+32]	//stride

xloop:
		mov		ecx,[esi]
		mov		eax,0xfefefefe

		mov		ebx,[esi+edx*2]
		and		eax,ecx

		and		ebx,0xfefefefe
		shr		eax,1

		shr		ebx,1
		add		esi,4

		add		eax,ebx
		dec		ebp

		mov		[edi+esi-4],eax
		jnz		xloop

		pop		ebx
		pop		esi
		pop		edi
		pop		ebp
		ret
	};
}

void __declspec(naked) fastconvert_asm_middle_row_MMX(void* dst, void* src, int w, int srcpitch)
{
	static const __int64 mask0 = 0xfefefefefefefefei64;
	__asm 
	{
		push	ebp
		push	edi
		push	esi
		push	ebx

		mov		edi,[esp+20]	//dst
		mov		esi,[esp+24]	//src
		sub		edi,esi
		mov		ebp,[esp+28]	//w
		mov		edx,[esp+32]	//stride

		movq	mm3,mask0
		shr		ebp,1
		jz		oddpart

xloop:
		movq	mm0,[esi]
		movq	mm2,mm3

		movq	mm1,[esi+edx*2]
		pand	mm0,mm3

		pand	mm1,mm2
		psrlq	mm0,1

		psrlq	mm1,1
		add		esi,8

		paddb	mm0,mm1
		dec		ebp

		movq	[edi+esi-8],mm0
		jne		xloop

oddpart:
		test	byte ptr [esp+28],1
		jz		nooddpart

		mov		ecx,[esi]
		mov		eax,0xfefefefe
		mov		ebx,[esi+edx*2]
		and		eax,ecx
		and		ebx,0xfefefefe
		shr		eax,1
		shr		ebx,1
		add		eax,ebx
		mov		[edi+esi],eax
nooddpart:

		pop		ebx
		pop		esi
		pop		edi
		pop		ebp
		ret
	};
}
