	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 11, 0	sdk_version 26, 0
	.private_extern	__ZNSt3__18functionIFffEED1Ev ; -- Begin function _ZNSt3__18functionIFffEED1Ev
	.globl	__ZNSt3__18functionIFffEED1Ev
	.weak_def_can_be_hidden	__ZNSt3__18functionIFffEED1Ev
	.p2align	2
__ZNSt3__18functionIFffEED1Ev:          ; @_ZNSt3__18functionIFffEED1Ev
	.cfi_startproc
; %bb.0:
	stp	x20, x19, [sp, #-32]!           ; 16-byte Folded Spill
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	mov	x19, x0
	ldr	x0, [x0, #24]
	cmp	x0, x19
	b.eq	LBB0_3
; %bb.1:
	cbz	x0, LBB0_5
; %bb.2:
	mov	w8, #40                         ; =0x28
	b	LBB0_4
LBB0_3:
	mov	w8, #32                         ; =0x20
LBB0_4:
	ldr	x9, [x0]
	ldr	x8, [x9, x8]
	blr	x8
LBB0_5:
	mov	x0, x19
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp], #32             ; 16-byte Folded Reload
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	__Z13test_function12AlignedArrayIfLm64EE ; -- Begin function _Z13test_function12AlignedArrayIfLm64EE
	.p2align	2
__Z13test_function12AlignedArrayIfLm64EE: ; @_Z13test_function12AlignedArrayIfLm64EE
	.cfi_startproc
; %bb.0:
	mov	w9, #1285                       ; =0x505
	movk	w9, #16565, lsl #16
	dup.4s	v1, w9
	mov	w9, #1285                       ; =0x505
	movk	w9, #49077, lsl #16
	dup.4s	v0, w9
	mov	w9, #1285                       ; =0x505
	movk	w9, #16309, lsl #16
	dup.4s	v2, w9
	mov.16b	v4, v0
	ldp	q3, q6, [x0]
	fmla.4s	v4, v3, v1
	fcmge.4s	v5, v2, v4
Lloh0:
	adrp	x9, __MergedGlobals@PAGE+1248
Lloh1:
	ldr	q3, [x9, __MergedGlobals@PAGEOFF+1248]
	fsub.4s	v7, v3, v4
	bsl.16b	v5, v4, v7
	mov	w9, #50102                      ; =0xc3b6
	movk	w9, #16263, lsl #16
	dup.4s	v4, w9
	mov.16b	v7, v0
	fmla.4s	v7, v6, v1
	fcmge.4s	v6, v2, v7
	fsub.4s	v16, v3, v7
	bsl.16b	v6, v7, v16
	mov.16b	v7, v0
	ldp	q16, q17, [x0, #32]
	fmla.4s	v7, v16, v1
	fcmge.4s	v16, v2, v7
	fsub.4s	v18, v3, v7
	bif.16b	v7, v18, v16
	mov.16b	v16, v0
	fmla.4s	v16, v17, v1
	fcmge.4s	v17, v2, v16
	fsub.4s	v18, v3, v16
	bif.16b	v16, v18, v17
	mov.16b	v17, v0
	ldp	q18, q19, [x0, #64]
	fmla.4s	v17, v18, v1
	fcmge.4s	v18, v2, v17
	fsub.4s	v20, v3, v17
	bif.16b	v17, v20, v18
	mov.16b	v18, v0
	fmla.4s	v18, v19, v1
	fcmge.4s	v19, v2, v18
	fsub.4s	v20, v3, v18
	bif.16b	v18, v20, v19
	mov.16b	v19, v0
	ldp	q20, q21, [x0, #96]
	fmla.4s	v19, v20, v1
	fcmge.4s	v20, v2, v19
	fsub.4s	v22, v3, v19
	bif.16b	v19, v22, v20
	mov.16b	v20, v0
	fmla.4s	v20, v21, v1
	fcmge.4s	v21, v2, v20
	fsub.4s	v22, v3, v20
	bif.16b	v20, v22, v21
	mov.16b	v21, v0
	ldp	q22, q23, [x0, #128]
	fmla.4s	v21, v22, v1
	fcmge.4s	v22, v2, v21
	fsub.4s	v24, v3, v21
	bif.16b	v21, v24, v22
	mov.16b	v22, v0
	fmla.4s	v22, v23, v1
	fcmge.4s	v23, v2, v22
	fsub.4s	v24, v3, v22
	bif.16b	v22, v24, v23
	mov.16b	v23, v0
	ldp	q24, q25, [x0, #160]
	fmla.4s	v23, v24, v1
	fcmge.4s	v24, v2, v23
	fsub.4s	v26, v3, v23
	bif.16b	v23, v26, v24
	mov.16b	v24, v0
	fmla.4s	v24, v25, v1
	fcmge.4s	v25, v2, v24
	fsub.4s	v26, v3, v24
	bif.16b	v24, v26, v25
	mov.16b	v25, v0
	ldp	q26, q27, [x0, #192]
	fmla.4s	v25, v26, v1
	fcmge.4s	v26, v2, v25
	fsub.4s	v28, v3, v25
	bif.16b	v25, v28, v26
	mov.16b	v26, v0
	fmla.4s	v26, v27, v1
	fcmge.4s	v27, v2, v26
	fsub.4s	v28, v3, v26
	bif.16b	v26, v28, v27
	mov.16b	v28, v0
	ldp	q29, q27, [x0, #224]
	fmla.4s	v28, v29, v1
	fcmge.4s	v29, v2, v28
	fsub.4s	v30, v3, v28
	bif.16b	v28, v30, v29
	fmul.4s	v29, v5, v4
	fmul.4s	v30, v5, v5
	mov	w9, #43691                      ; =0xaaab
	movk	w9, #15914, lsl #16
	dup.4s	v5, w9
	fmul.4s	v30, v30, v5
	fmls.4s	v29, v29, v30
	fmul.4s	v30, v6, v4
	fmul.4s	v6, v6, v6
	fmul.4s	v6, v6, v5
	fmls.4s	v30, v30, v6
	stp	q29, q30, [x8]
	fmul.4s	v6, v7, v4
	fmul.4s	v7, v7, v7
	fmul.4s	v7, v7, v5
	fmls.4s	v6, v6, v7
	fmul.4s	v7, v16, v4
	fmul.4s	v16, v16, v16
	fmul.4s	v16, v16, v5
	fmls.4s	v7, v7, v16
	stp	q6, q7, [x8, #32]
	fmul.4s	v6, v17, v4
	fmul.4s	v7, v17, v17
	fmul.4s	v7, v7, v5
	fmls.4s	v6, v6, v7
	fmul.4s	v7, v18, v4
	fmul.4s	v16, v18, v18
	fmul.4s	v16, v16, v5
	fmls.4s	v7, v7, v16
	stp	q6, q7, [x8, #64]
	fmul.4s	v6, v19, v4
	fmul.4s	v7, v19, v19
	fmul.4s	v7, v7, v5
	fmls.4s	v6, v6, v7
	fmul.4s	v7, v20, v4
	fmul.4s	v16, v20, v20
	fmul.4s	v16, v16, v5
	fmls.4s	v7, v7, v16
	stp	q6, q7, [x8, #96]
	fmul.4s	v6, v21, v4
	fmul.4s	v7, v21, v21
	fmul.4s	v7, v7, v5
	fmls.4s	v6, v6, v7
	fmul.4s	v7, v22, v4
	fmul.4s	v16, v22, v22
	fmul.4s	v16, v16, v5
	fmls.4s	v7, v7, v16
	stp	q6, q7, [x8, #128]
	fmul.4s	v6, v23, v4
	fmul.4s	v7, v23, v23
	fmul.4s	v7, v7, v5
	fmls.4s	v6, v6, v7
	fmul.4s	v7, v24, v4
	fmul.4s	v16, v24, v24
	fmul.4s	v16, v16, v5
	fmls.4s	v7, v7, v16
	stp	q6, q7, [x8, #160]
	fmul.4s	v6, v25, v4
	fmul.4s	v7, v25, v25
	fmul.4s	v7, v7, v5
	fmls.4s	v6, v6, v7
	fmul.4s	v7, v26, v4
	fmul.4s	v16, v26, v26
	fmul.4s	v16, v16, v5
	fmls.4s	v7, v7, v16
	stp	q6, q7, [x8, #192]
	fmul.4s	v6, v28, v4
	fmul.4s	v7, v28, v28
	fmul.4s	v7, v7, v5
	fmls.4s	v6, v6, v7
	fmla.4s	v0, v27, v1
	fcmge.4s	v1, v2, v0
	fsub.4s	v2, v3, v0
	bif.16b	v0, v2, v1
	fmul.4s	v1, v0, v4
	fmul.4s	v0, v0, v0
	fmul.4s	v0, v0, v5
	fmls.4s	v1, v1, v0
	stp	q6, q1, [x8, #224]
	ret
	.loh AdrpLdr	Lloh0, Lloh1
	.cfi_endproc
                                        ; -- End function
	.globl	_main                           ; -- Begin function main
	.p2align	2
_main:                                  ; @main
	.cfi_startproc
; %bb.0:
	stp	x28, x27, [sp, #-32]!           ; 16-byte Folded Spill
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	sub	sp, sp, #512
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w27, -24
	.cfi_offset w28, -32
	movi.2d	v0, #0000000000000000
	stp	q0, q0, [sp, #224]
	stp	q0, q0, [sp, #192]
	stp	q0, q0, [sp, #160]
	stp	q0, q0, [sp, #128]
	stp	q0, q0, [sp, #96]
	stp	q0, q0, [sp, #64]
	stp	q0, q0, [sp, #32]
	stp	q0, q0, [sp]
	add	x8, sp, #256
	mov	x0, sp
	bl	__Z13test_function12AlignedArrayIfLm64EE
	add	x0, sp, #256
	bl	_force_use
	mov	w0, #0                          ; =0x0
	add	sp, sp, #512
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	ldp	x28, x27, [sp], #32             ; 16-byte Folded Reload
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections3$_0ENS_9allocatorIS4_EEFffEED1Ev
__ZNSt3__110__function6__funcIN2ml11projections3$_0ENS_9allocatorIS4_EEFffEED1Ev: ; @"_ZNSt3__110__function6__funcIN2ml11projections3$_0ENS_9allocatorIS4_EEFffEED1Ev"
	.cfi_startproc
; %bb.0:
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections3$_0ENS_9allocatorIS4_EEFffEED0Ev
__ZNSt3__110__function6__funcIN2ml11projections3$_0ENS_9allocatorIS4_EEFffEED0Ev: ; @"_ZNSt3__110__function6__funcIN2ml11projections3$_0ENS_9allocatorIS4_EEFffEED0Ev"
	.cfi_startproc
; %bb.0:
	b	__ZdlPv
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections3$_0ENS_9allocatorIS4_EEFffEE7__cloneEv
__ZNKSt3__110__function6__funcIN2ml11projections3$_0ENS_9allocatorIS4_EEFffEE7__cloneEv: ; @"_ZNKSt3__110__function6__funcIN2ml11projections3$_0ENS_9allocatorIS4_EEFffEE7__cloneEv"
	.cfi_startproc
; %bb.0:
	stp	x29, x30, [sp, #-16]!           ; 16-byte Folded Spill
	mov	x29, sp
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	mov	w0, #16                         ; =0x10
	bl	__Znwm
Lloh2:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml11projections3$_0ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh3:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml11projections3$_0ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x0]
	ldp	x29, x30, [sp], #16             ; 16-byte Folded Reload
	ret
	.loh AdrpAdd	Lloh2, Lloh3
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections3$_0ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE
__ZNKSt3__110__function6__funcIN2ml11projections3$_0ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE: ; @"_ZNKSt3__110__function6__funcIN2ml11projections3$_0ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE"
	.cfi_startproc
; %bb.0:
Lloh4:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml11projections3$_0ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh5:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml11projections3$_0ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x1]
	ret
	.loh AdrpAdd	Lloh4, Lloh5
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections3$_0ENS_9allocatorIS4_EEFffEE7destroyEv
__ZNSt3__110__function6__funcIN2ml11projections3$_0ENS_9allocatorIS4_EEFffEE7destroyEv: ; @"_ZNSt3__110__function6__funcIN2ml11projections3$_0ENS_9allocatorIS4_EEFffEE7destroyEv"
	.cfi_startproc
; %bb.0:
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections3$_0ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv
__ZNSt3__110__function6__funcIN2ml11projections3$_0ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv: ; @"_ZNSt3__110__function6__funcIN2ml11projections3$_0ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv"
	.cfi_startproc
; %bb.0:
	b	__ZdlPv
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections3$_0ENS_9allocatorIS4_EEFffEEclEOf
__ZNSt3__110__function6__funcIN2ml11projections3$_0ENS_9allocatorIS4_EEFffEEclEOf: ; @"_ZNSt3__110__function6__funcIN2ml11projections3$_0ENS_9allocatorIS4_EEFffEEclEOf"
	.cfi_startproc
; %bb.0:
	movi	d0, #0000000000000000
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections3$_0ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info
__ZNKSt3__110__function6__funcIN2ml11projections3$_0ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info: ; @"_ZNKSt3__110__function6__funcIN2ml11projections3$_0ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info"
	.cfi_startproc
; %bb.0:
	ldr	x8, [x1, #8]
Lloh6:
	adrp	x9, __ZTSN2ml11projections3$_0E@PAGE
Lloh7:
	add	x9, x9, __ZTSN2ml11projections3$_0E@PAGEOFF
	cmp	x8, x9
	b.ne	LBB10_2
LBB10_1:
	add	x0, x0, #8
	ret
LBB10_2:
	tst	x8, x9
	b.lt	LBB10_4
; %bb.3:
	mov	x0, #0                          ; =0x0
	ret
LBB10_4:
	stp	x20, x19, [sp, #-32]!           ; 16-byte Folded Spill
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	mov	x19, x0
	and	x0, x8, #0x7fffffffffffffff
	and	x1, x9, #0x7fffffffffffffff
	bl	_strcmp
	mov	x8, x0
	mov	x0, x19
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp], #32             ; 16-byte Folded Reload
	cbz	w8, LBB10_1
; %bb.5:
	mov	x0, #0                          ; =0x0
	ret
	.loh AdrpAdd	Lloh6, Lloh7
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections3$_0ENS_9allocatorIS4_EEFffEE11target_typeEv
__ZNKSt3__110__function6__funcIN2ml11projections3$_0ENS_9allocatorIS4_EEFffEE11target_typeEv: ; @"_ZNKSt3__110__function6__funcIN2ml11projections3$_0ENS_9allocatorIS4_EEFffEE11target_typeEv"
	.cfi_startproc
; %bb.0:
Lloh8:
	adrp	x0, __ZTIN2ml11projections3$_0E@PAGE
Lloh9:
	add	x0, x0, __ZTIN2ml11projections3$_0E@PAGEOFF
	ret
	.loh AdrpAdd	Lloh8, Lloh9
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections3$_1ENS_9allocatorIS4_EEFffEED1Ev
__ZNSt3__110__function6__funcIN2ml11projections3$_1ENS_9allocatorIS4_EEFffEED1Ev: ; @"_ZNSt3__110__function6__funcIN2ml11projections3$_1ENS_9allocatorIS4_EEFffEED1Ev"
	.cfi_startproc
; %bb.0:
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections3$_1ENS_9allocatorIS4_EEFffEED0Ev
__ZNSt3__110__function6__funcIN2ml11projections3$_1ENS_9allocatorIS4_EEFffEED0Ev: ; @"_ZNSt3__110__function6__funcIN2ml11projections3$_1ENS_9allocatorIS4_EEFffEED0Ev"
	.cfi_startproc
; %bb.0:
	b	__ZdlPv
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections3$_1ENS_9allocatorIS4_EEFffEE7__cloneEv
__ZNKSt3__110__function6__funcIN2ml11projections3$_1ENS_9allocatorIS4_EEFffEE7__cloneEv: ; @"_ZNKSt3__110__function6__funcIN2ml11projections3$_1ENS_9allocatorIS4_EEFffEE7__cloneEv"
	.cfi_startproc
; %bb.0:
	stp	x29, x30, [sp, #-16]!           ; 16-byte Folded Spill
	mov	x29, sp
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	mov	w0, #16                         ; =0x10
	bl	__Znwm
Lloh10:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml11projections3$_1ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh11:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml11projections3$_1ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x0]
	ldp	x29, x30, [sp], #16             ; 16-byte Folded Reload
	ret
	.loh AdrpAdd	Lloh10, Lloh11
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections3$_1ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE
__ZNKSt3__110__function6__funcIN2ml11projections3$_1ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE: ; @"_ZNKSt3__110__function6__funcIN2ml11projections3$_1ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE"
	.cfi_startproc
; %bb.0:
Lloh12:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml11projections3$_1ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh13:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml11projections3$_1ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x1]
	ret
	.loh AdrpAdd	Lloh12, Lloh13
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections3$_1ENS_9allocatorIS4_EEFffEE7destroyEv
__ZNSt3__110__function6__funcIN2ml11projections3$_1ENS_9allocatorIS4_EEFffEE7destroyEv: ; @"_ZNSt3__110__function6__funcIN2ml11projections3$_1ENS_9allocatorIS4_EEFffEE7destroyEv"
	.cfi_startproc
; %bb.0:
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections3$_1ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv
__ZNSt3__110__function6__funcIN2ml11projections3$_1ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv: ; @"_ZNSt3__110__function6__funcIN2ml11projections3$_1ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv"
	.cfi_startproc
; %bb.0:
	b	__ZdlPv
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections3$_1ENS_9allocatorIS4_EEFffEEclEOf
__ZNSt3__110__function6__funcIN2ml11projections3$_1ENS_9allocatorIS4_EEFffEEclEOf: ; @"_ZNSt3__110__function6__funcIN2ml11projections3$_1ENS_9allocatorIS4_EEFffEEclEOf"
	.cfi_startproc
; %bb.0:
	ldr	s0, [x1]
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections3$_1ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info
__ZNKSt3__110__function6__funcIN2ml11projections3$_1ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info: ; @"_ZNKSt3__110__function6__funcIN2ml11projections3$_1ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info"
	.cfi_startproc
; %bb.0:
	ldr	x8, [x1, #8]
Lloh14:
	adrp	x9, __ZTSN2ml11projections3$_1E@PAGE
Lloh15:
	add	x9, x9, __ZTSN2ml11projections3$_1E@PAGEOFF
	cmp	x8, x9
	b.ne	LBB19_2
LBB19_1:
	add	x0, x0, #8
	ret
LBB19_2:
	tst	x8, x9
	b.lt	LBB19_4
; %bb.3:
	mov	x0, #0                          ; =0x0
	ret
LBB19_4:
	stp	x20, x19, [sp, #-32]!           ; 16-byte Folded Spill
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	mov	x19, x0
	and	x0, x8, #0x7fffffffffffffff
	and	x1, x9, #0x7fffffffffffffff
	bl	_strcmp
	mov	x8, x0
	mov	x0, x19
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp], #32             ; 16-byte Folded Reload
	cbz	w8, LBB19_1
; %bb.5:
	mov	x0, #0                          ; =0x0
	ret
	.loh AdrpAdd	Lloh14, Lloh15
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections3$_1ENS_9allocatorIS4_EEFffEE11target_typeEv
__ZNKSt3__110__function6__funcIN2ml11projections3$_1ENS_9allocatorIS4_EEFffEE11target_typeEv: ; @"_ZNKSt3__110__function6__funcIN2ml11projections3$_1ENS_9allocatorIS4_EEFffEE11target_typeEv"
	.cfi_startproc
; %bb.0:
Lloh16:
	adrp	x0, __ZTIN2ml11projections3$_1E@PAGE
Lloh17:
	add	x0, x0, __ZTIN2ml11projections3$_1E@PAGEOFF
	ret
	.loh AdrpAdd	Lloh16, Lloh17
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections3$_2ENS_9allocatorIS4_EEFffEED1Ev
__ZNSt3__110__function6__funcIN2ml11projections3$_2ENS_9allocatorIS4_EEFffEED1Ev: ; @"_ZNSt3__110__function6__funcIN2ml11projections3$_2ENS_9allocatorIS4_EEFffEED1Ev"
	.cfi_startproc
; %bb.0:
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections3$_2ENS_9allocatorIS4_EEFffEED0Ev
__ZNSt3__110__function6__funcIN2ml11projections3$_2ENS_9allocatorIS4_EEFffEED0Ev: ; @"_ZNSt3__110__function6__funcIN2ml11projections3$_2ENS_9allocatorIS4_EEFffEED0Ev"
	.cfi_startproc
; %bb.0:
	b	__ZdlPv
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections3$_2ENS_9allocatorIS4_EEFffEE7__cloneEv
__ZNKSt3__110__function6__funcIN2ml11projections3$_2ENS_9allocatorIS4_EEFffEE7__cloneEv: ; @"_ZNKSt3__110__function6__funcIN2ml11projections3$_2ENS_9allocatorIS4_EEFffEE7__cloneEv"
	.cfi_startproc
; %bb.0:
	stp	x29, x30, [sp, #-16]!           ; 16-byte Folded Spill
	mov	x29, sp
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	mov	w0, #16                         ; =0x10
	bl	__Znwm
Lloh18:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml11projections3$_2ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh19:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml11projections3$_2ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x0]
	ldp	x29, x30, [sp], #16             ; 16-byte Folded Reload
	ret
	.loh AdrpAdd	Lloh18, Lloh19
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections3$_2ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE
__ZNKSt3__110__function6__funcIN2ml11projections3$_2ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE: ; @"_ZNKSt3__110__function6__funcIN2ml11projections3$_2ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE"
	.cfi_startproc
; %bb.0:
Lloh20:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml11projections3$_2ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh21:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml11projections3$_2ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x1]
	ret
	.loh AdrpAdd	Lloh20, Lloh21
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections3$_2ENS_9allocatorIS4_EEFffEE7destroyEv
__ZNSt3__110__function6__funcIN2ml11projections3$_2ENS_9allocatorIS4_EEFffEE7destroyEv: ; @"_ZNSt3__110__function6__funcIN2ml11projections3$_2ENS_9allocatorIS4_EEFffEE7destroyEv"
	.cfi_startproc
; %bb.0:
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections3$_2ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv
__ZNSt3__110__function6__funcIN2ml11projections3$_2ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv: ; @"_ZNSt3__110__function6__funcIN2ml11projections3$_2ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv"
	.cfi_startproc
; %bb.0:
	b	__ZdlPv
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections3$_2ENS_9allocatorIS4_EEFffEEclEOf
__ZNSt3__110__function6__funcIN2ml11projections3$_2ENS_9allocatorIS4_EEFffEEclEOf: ; @"_ZNSt3__110__function6__funcIN2ml11projections3$_2ENS_9allocatorIS4_EEFffEEclEOf"
	.cfi_startproc
; %bb.0:
	ldr	s0, [x1]
	fmul	s0, s0, s0
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections3$_2ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info
__ZNKSt3__110__function6__funcIN2ml11projections3$_2ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info: ; @"_ZNKSt3__110__function6__funcIN2ml11projections3$_2ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info"
	.cfi_startproc
; %bb.0:
	ldr	x8, [x1, #8]
Lloh22:
	adrp	x9, __ZTSN2ml11projections3$_2E@PAGE
Lloh23:
	add	x9, x9, __ZTSN2ml11projections3$_2E@PAGEOFF
	cmp	x8, x9
	b.ne	LBB28_2
LBB28_1:
	add	x0, x0, #8
	ret
LBB28_2:
	tst	x8, x9
	b.lt	LBB28_4
; %bb.3:
	mov	x0, #0                          ; =0x0
	ret
LBB28_4:
	stp	x20, x19, [sp, #-32]!           ; 16-byte Folded Spill
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	mov	x19, x0
	and	x0, x8, #0x7fffffffffffffff
	and	x1, x9, #0x7fffffffffffffff
	bl	_strcmp
	mov	x8, x0
	mov	x0, x19
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp], #32             ; 16-byte Folded Reload
	cbz	w8, LBB28_1
; %bb.5:
	mov	x0, #0                          ; =0x0
	ret
	.loh AdrpAdd	Lloh22, Lloh23
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections3$_2ENS_9allocatorIS4_EEFffEE11target_typeEv
__ZNKSt3__110__function6__funcIN2ml11projections3$_2ENS_9allocatorIS4_EEFffEE11target_typeEv: ; @"_ZNKSt3__110__function6__funcIN2ml11projections3$_2ENS_9allocatorIS4_EEFffEE11target_typeEv"
	.cfi_startproc
; %bb.0:
Lloh24:
	adrp	x0, __ZTIN2ml11projections3$_2E@PAGE
Lloh25:
	add	x0, x0, __ZTIN2ml11projections3$_2E@PAGEOFF
	ret
	.loh AdrpAdd	Lloh24, Lloh25
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections3$_3ENS_9allocatorIS4_EEFffEED1Ev
__ZNSt3__110__function6__funcIN2ml11projections3$_3ENS_9allocatorIS4_EEFffEED1Ev: ; @"_ZNSt3__110__function6__funcIN2ml11projections3$_3ENS_9allocatorIS4_EEFffEED1Ev"
	.cfi_startproc
; %bb.0:
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections3$_3ENS_9allocatorIS4_EEFffEED0Ev
__ZNSt3__110__function6__funcIN2ml11projections3$_3ENS_9allocatorIS4_EEFffEED0Ev: ; @"_ZNSt3__110__function6__funcIN2ml11projections3$_3ENS_9allocatorIS4_EEFffEED0Ev"
	.cfi_startproc
; %bb.0:
	b	__ZdlPv
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections3$_3ENS_9allocatorIS4_EEFffEE7__cloneEv
__ZNKSt3__110__function6__funcIN2ml11projections3$_3ENS_9allocatorIS4_EEFffEE7__cloneEv: ; @"_ZNKSt3__110__function6__funcIN2ml11projections3$_3ENS_9allocatorIS4_EEFffEE7__cloneEv"
	.cfi_startproc
; %bb.0:
	stp	x29, x30, [sp, #-16]!           ; 16-byte Folded Spill
	mov	x29, sp
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	mov	w0, #16                         ; =0x10
	bl	__Znwm
Lloh26:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml11projections3$_3ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh27:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml11projections3$_3ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x0]
	ldp	x29, x30, [sp], #16             ; 16-byte Folded Reload
	ret
	.loh AdrpAdd	Lloh26, Lloh27
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections3$_3ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE
__ZNKSt3__110__function6__funcIN2ml11projections3$_3ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE: ; @"_ZNKSt3__110__function6__funcIN2ml11projections3$_3ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE"
	.cfi_startproc
; %bb.0:
Lloh28:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml11projections3$_3ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh29:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml11projections3$_3ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x1]
	ret
	.loh AdrpAdd	Lloh28, Lloh29
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections3$_3ENS_9allocatorIS4_EEFffEE7destroyEv
__ZNSt3__110__function6__funcIN2ml11projections3$_3ENS_9allocatorIS4_EEFffEE7destroyEv: ; @"_ZNSt3__110__function6__funcIN2ml11projections3$_3ENS_9allocatorIS4_EEFffEE7destroyEv"
	.cfi_startproc
; %bb.0:
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections3$_3ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv
__ZNSt3__110__function6__funcIN2ml11projections3$_3ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv: ; @"_ZNSt3__110__function6__funcIN2ml11projections3$_3ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv"
	.cfi_startproc
; %bb.0:
	b	__ZdlPv
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections3$_3ENS_9allocatorIS4_EEFffEEclEOf
__ZNSt3__110__function6__funcIN2ml11projections3$_3ENS_9allocatorIS4_EEFffEEclEOf: ; @"_ZNSt3__110__function6__funcIN2ml11projections3$_3ENS_9allocatorIS4_EEFffEEclEOf"
	.cfi_startproc
; %bb.0:
	ldr	s0, [x1]
	fmov	s1, #1.00000000
	fsub	s0, s1, s0
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections3$_3ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info
__ZNKSt3__110__function6__funcIN2ml11projections3$_3ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info: ; @"_ZNKSt3__110__function6__funcIN2ml11projections3$_3ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info"
	.cfi_startproc
; %bb.0:
	ldr	x8, [x1, #8]
Lloh30:
	adrp	x9, __ZTSN2ml11projections3$_3E@PAGE
Lloh31:
	add	x9, x9, __ZTSN2ml11projections3$_3E@PAGEOFF
	cmp	x8, x9
	b.ne	LBB37_2
LBB37_1:
	add	x0, x0, #8
	ret
LBB37_2:
	tst	x8, x9
	b.lt	LBB37_4
; %bb.3:
	mov	x0, #0                          ; =0x0
	ret
LBB37_4:
	stp	x20, x19, [sp, #-32]!           ; 16-byte Folded Spill
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	mov	x19, x0
	and	x0, x8, #0x7fffffffffffffff
	and	x1, x9, #0x7fffffffffffffff
	bl	_strcmp
	mov	x8, x0
	mov	x0, x19
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp], #32             ; 16-byte Folded Reload
	cbz	w8, LBB37_1
; %bb.5:
	mov	x0, #0                          ; =0x0
	ret
	.loh AdrpAdd	Lloh30, Lloh31
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections3$_3ENS_9allocatorIS4_EEFffEE11target_typeEv
__ZNKSt3__110__function6__funcIN2ml11projections3$_3ENS_9allocatorIS4_EEFffEE11target_typeEv: ; @"_ZNKSt3__110__function6__funcIN2ml11projections3$_3ENS_9allocatorIS4_EEFffEE11target_typeEv"
	.cfi_startproc
; %bb.0:
Lloh32:
	adrp	x0, __ZTIN2ml11projections3$_3E@PAGE
Lloh33:
	add	x0, x0, __ZTIN2ml11projections3$_3E@PAGEOFF
	ret
	.loh AdrpAdd	Lloh32, Lloh33
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections3$_4ENS_9allocatorIS4_EEFffEED1Ev
__ZNSt3__110__function6__funcIN2ml11projections3$_4ENS_9allocatorIS4_EEFffEED1Ev: ; @"_ZNSt3__110__function6__funcIN2ml11projections3$_4ENS_9allocatorIS4_EEFffEED1Ev"
	.cfi_startproc
; %bb.0:
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections3$_4ENS_9allocatorIS4_EEFffEED0Ev
__ZNSt3__110__function6__funcIN2ml11projections3$_4ENS_9allocatorIS4_EEFffEED0Ev: ; @"_ZNSt3__110__function6__funcIN2ml11projections3$_4ENS_9allocatorIS4_EEFffEED0Ev"
	.cfi_startproc
; %bb.0:
	b	__ZdlPv
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections3$_4ENS_9allocatorIS4_EEFffEE7__cloneEv
__ZNKSt3__110__function6__funcIN2ml11projections3$_4ENS_9allocatorIS4_EEFffEE7__cloneEv: ; @"_ZNKSt3__110__function6__funcIN2ml11projections3$_4ENS_9allocatorIS4_EEFffEE7__cloneEv"
	.cfi_startproc
; %bb.0:
	stp	x29, x30, [sp, #-16]!           ; 16-byte Folded Spill
	mov	x29, sp
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	mov	w0, #16                         ; =0x10
	bl	__Znwm
Lloh34:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml11projections3$_4ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh35:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml11projections3$_4ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x0]
	ldp	x29, x30, [sp], #16             ; 16-byte Folded Reload
	ret
	.loh AdrpAdd	Lloh34, Lloh35
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections3$_4ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE
__ZNKSt3__110__function6__funcIN2ml11projections3$_4ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE: ; @"_ZNKSt3__110__function6__funcIN2ml11projections3$_4ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE"
	.cfi_startproc
; %bb.0:
Lloh36:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml11projections3$_4ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh37:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml11projections3$_4ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x1]
	ret
	.loh AdrpAdd	Lloh36, Lloh37
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections3$_4ENS_9allocatorIS4_EEFffEE7destroyEv
__ZNSt3__110__function6__funcIN2ml11projections3$_4ENS_9allocatorIS4_EEFffEE7destroyEv: ; @"_ZNSt3__110__function6__funcIN2ml11projections3$_4ENS_9allocatorIS4_EEFffEE7destroyEv"
	.cfi_startproc
; %bb.0:
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections3$_4ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv
__ZNSt3__110__function6__funcIN2ml11projections3$_4ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv: ; @"_ZNSt3__110__function6__funcIN2ml11projections3$_4ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv"
	.cfi_startproc
; %bb.0:
	b	__ZdlPv
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections3$_4ENS_9allocatorIS4_EEFffEEclEOf
__ZNSt3__110__function6__funcIN2ml11projections3$_4ENS_9allocatorIS4_EEFffEEclEOf: ; @"_ZNSt3__110__function6__funcIN2ml11projections3$_4ENS_9allocatorIS4_EEFffEEclEOf"
	.cfi_startproc
; %bb.0:
	ldr	s0, [x1]
	fmov	s1, #1.00000000
	fminnm	s0, s0, s1
	movi	d1, #0000000000000000
	fmaxnm	s0, s0, s1
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections3$_4ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info
__ZNKSt3__110__function6__funcIN2ml11projections3$_4ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info: ; @"_ZNKSt3__110__function6__funcIN2ml11projections3$_4ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info"
	.cfi_startproc
; %bb.0:
	ldr	x8, [x1, #8]
Lloh38:
	adrp	x9, __ZTSN2ml11projections3$_4E@PAGE
Lloh39:
	add	x9, x9, __ZTSN2ml11projections3$_4E@PAGEOFF
	cmp	x8, x9
	b.ne	LBB46_2
LBB46_1:
	add	x0, x0, #8
	ret
LBB46_2:
	tst	x8, x9
	b.lt	LBB46_4
; %bb.3:
	mov	x0, #0                          ; =0x0
	ret
LBB46_4:
	stp	x20, x19, [sp, #-32]!           ; 16-byte Folded Spill
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	mov	x19, x0
	and	x0, x8, #0x7fffffffffffffff
	and	x1, x9, #0x7fffffffffffffff
	bl	_strcmp
	mov	x8, x0
	mov	x0, x19
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp], #32             ; 16-byte Folded Reload
	cbz	w8, LBB46_1
; %bb.5:
	mov	x0, #0                          ; =0x0
	ret
	.loh AdrpAdd	Lloh38, Lloh39
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections3$_4ENS_9allocatorIS4_EEFffEE11target_typeEv
__ZNKSt3__110__function6__funcIN2ml11projections3$_4ENS_9allocatorIS4_EEFffEE11target_typeEv: ; @"_ZNKSt3__110__function6__funcIN2ml11projections3$_4ENS_9allocatorIS4_EEFffEE11target_typeEv"
	.cfi_startproc
; %bb.0:
Lloh40:
	adrp	x0, __ZTIN2ml11projections3$_4E@PAGE
Lloh41:
	add	x0, x0, __ZTIN2ml11projections3$_4E@PAGEOFF
	ret
	.loh AdrpAdd	Lloh40, Lloh41
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections3$_5ENS_9allocatorIS4_EEFffEED1Ev
__ZNSt3__110__function6__funcIN2ml11projections3$_5ENS_9allocatorIS4_EEFffEED1Ev: ; @"_ZNSt3__110__function6__funcIN2ml11projections3$_5ENS_9allocatorIS4_EEFffEED1Ev"
	.cfi_startproc
; %bb.0:
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections3$_5ENS_9allocatorIS4_EEFffEED0Ev
__ZNSt3__110__function6__funcIN2ml11projections3$_5ENS_9allocatorIS4_EEFffEED0Ev: ; @"_ZNSt3__110__function6__funcIN2ml11projections3$_5ENS_9allocatorIS4_EEFffEED0Ev"
	.cfi_startproc
; %bb.0:
	b	__ZdlPv
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections3$_5ENS_9allocatorIS4_EEFffEE7__cloneEv
__ZNKSt3__110__function6__funcIN2ml11projections3$_5ENS_9allocatorIS4_EEFffEE7__cloneEv: ; @"_ZNKSt3__110__function6__funcIN2ml11projections3$_5ENS_9allocatorIS4_EEFffEE7__cloneEv"
	.cfi_startproc
; %bb.0:
	stp	x29, x30, [sp, #-16]!           ; 16-byte Folded Spill
	mov	x29, sp
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	mov	w0, #16                         ; =0x10
	bl	__Znwm
Lloh42:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml11projections3$_5ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh43:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml11projections3$_5ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x0]
	ldp	x29, x30, [sp], #16             ; 16-byte Folded Reload
	ret
	.loh AdrpAdd	Lloh42, Lloh43
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections3$_5ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE
__ZNKSt3__110__function6__funcIN2ml11projections3$_5ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE: ; @"_ZNKSt3__110__function6__funcIN2ml11projections3$_5ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE"
	.cfi_startproc
; %bb.0:
Lloh44:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml11projections3$_5ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh45:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml11projections3$_5ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x1]
	ret
	.loh AdrpAdd	Lloh44, Lloh45
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections3$_5ENS_9allocatorIS4_EEFffEE7destroyEv
__ZNSt3__110__function6__funcIN2ml11projections3$_5ENS_9allocatorIS4_EEFffEE7destroyEv: ; @"_ZNSt3__110__function6__funcIN2ml11projections3$_5ENS_9allocatorIS4_EEFffEE7destroyEv"
	.cfi_startproc
; %bb.0:
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections3$_5ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv
__ZNSt3__110__function6__funcIN2ml11projections3$_5ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv: ; @"_ZNSt3__110__function6__funcIN2ml11projections3$_5ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv"
	.cfi_startproc
; %bb.0:
	b	__ZdlPv
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections3$_5ENS_9allocatorIS4_EEFffEEclEOf
__ZNSt3__110__function6__funcIN2ml11projections3$_5ENS_9allocatorIS4_EEFffEEclEOf: ; @"_ZNSt3__110__function6__funcIN2ml11projections3$_5ENS_9allocatorIS4_EEFffEEclEOf"
	.cfi_startproc
; %bb.0:
	ldr	s0, [x1]
	fmul	s1, s0, s0
	fadd	s0, s0, s0
	fmov	s2, #3.00000000
	fsub	s0, s2, s0
	fmul	s0, s1, s0
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections3$_5ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info
__ZNKSt3__110__function6__funcIN2ml11projections3$_5ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info: ; @"_ZNKSt3__110__function6__funcIN2ml11projections3$_5ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info"
	.cfi_startproc
; %bb.0:
	ldr	x8, [x1, #8]
Lloh46:
	adrp	x9, __ZTSN2ml11projections3$_5E@PAGE
Lloh47:
	add	x9, x9, __ZTSN2ml11projections3$_5E@PAGEOFF
	cmp	x8, x9
	b.ne	LBB55_2
LBB55_1:
	add	x0, x0, #8
	ret
LBB55_2:
	tst	x8, x9
	b.lt	LBB55_4
; %bb.3:
	mov	x0, #0                          ; =0x0
	ret
LBB55_4:
	stp	x20, x19, [sp, #-32]!           ; 16-byte Folded Spill
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	mov	x19, x0
	and	x0, x8, #0x7fffffffffffffff
	and	x1, x9, #0x7fffffffffffffff
	bl	_strcmp
	mov	x8, x0
	mov	x0, x19
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp], #32             ; 16-byte Folded Reload
	cbz	w8, LBB55_1
; %bb.5:
	mov	x0, #0                          ; =0x0
	ret
	.loh AdrpAdd	Lloh46, Lloh47
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections3$_5ENS_9allocatorIS4_EEFffEE11target_typeEv
__ZNKSt3__110__function6__funcIN2ml11projections3$_5ENS_9allocatorIS4_EEFffEE11target_typeEv: ; @"_ZNKSt3__110__function6__funcIN2ml11projections3$_5ENS_9allocatorIS4_EEFffEE11target_typeEv"
	.cfi_startproc
; %bb.0:
Lloh48:
	adrp	x0, __ZTIN2ml11projections3$_5E@PAGE
Lloh49:
	add	x0, x0, __ZTIN2ml11projections3$_5E@PAGEOFF
	ret
	.loh AdrpAdd	Lloh48, Lloh49
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections3$_6ENS_9allocatorIS4_EEFffEED1Ev
__ZNSt3__110__function6__funcIN2ml11projections3$_6ENS_9allocatorIS4_EEFffEED1Ev: ; @"_ZNSt3__110__function6__funcIN2ml11projections3$_6ENS_9allocatorIS4_EEFffEED1Ev"
	.cfi_startproc
; %bb.0:
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections3$_6ENS_9allocatorIS4_EEFffEED0Ev
__ZNSt3__110__function6__funcIN2ml11projections3$_6ENS_9allocatorIS4_EEFffEED0Ev: ; @"_ZNSt3__110__function6__funcIN2ml11projections3$_6ENS_9allocatorIS4_EEFffEED0Ev"
	.cfi_startproc
; %bb.0:
	b	__ZdlPv
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections3$_6ENS_9allocatorIS4_EEFffEE7__cloneEv
__ZNKSt3__110__function6__funcIN2ml11projections3$_6ENS_9allocatorIS4_EEFffEE7__cloneEv: ; @"_ZNKSt3__110__function6__funcIN2ml11projections3$_6ENS_9allocatorIS4_EEFffEE7__cloneEv"
	.cfi_startproc
; %bb.0:
	stp	x29, x30, [sp, #-16]!           ; 16-byte Folded Spill
	mov	x29, sp
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	mov	w0, #16                         ; =0x10
	bl	__Znwm
Lloh50:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml11projections3$_6ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh51:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml11projections3$_6ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x0]
	ldp	x29, x30, [sp], #16             ; 16-byte Folded Reload
	ret
	.loh AdrpAdd	Lloh50, Lloh51
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections3$_6ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE
__ZNKSt3__110__function6__funcIN2ml11projections3$_6ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE: ; @"_ZNKSt3__110__function6__funcIN2ml11projections3$_6ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE"
	.cfi_startproc
; %bb.0:
Lloh52:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml11projections3$_6ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh53:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml11projections3$_6ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x1]
	ret
	.loh AdrpAdd	Lloh52, Lloh53
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections3$_6ENS_9allocatorIS4_EEFffEE7destroyEv
__ZNSt3__110__function6__funcIN2ml11projections3$_6ENS_9allocatorIS4_EEFffEE7destroyEv: ; @"_ZNSt3__110__function6__funcIN2ml11projections3$_6ENS_9allocatorIS4_EEFffEE7destroyEv"
	.cfi_startproc
; %bb.0:
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections3$_6ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv
__ZNSt3__110__function6__funcIN2ml11projections3$_6ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv: ; @"_ZNSt3__110__function6__funcIN2ml11projections3$_6ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv"
	.cfi_startproc
; %bb.0:
	b	__ZdlPv
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections3$_6ENS_9allocatorIS4_EEFffEEclEOf
__ZNSt3__110__function6__funcIN2ml11projections3$_6ENS_9allocatorIS4_EEFffEEclEOf: ; @"_ZNSt3__110__function6__funcIN2ml11projections3$_6ENS_9allocatorIS4_EEFffEEclEOf"
	.cfi_startproc
; %bb.0:
	ldr	s0, [x1]
	fmov	s1, #-0.50000000
	fadd	s0, s0, s1
	fmul	s1, s0, s0
	fmov	s2, #4.00000000
	fmul	s0, s2, s0
	fmov	s2, #0.50000000
	fmadd	s0, s1, s0, s2
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections3$_6ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info
__ZNKSt3__110__function6__funcIN2ml11projections3$_6ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info: ; @"_ZNKSt3__110__function6__funcIN2ml11projections3$_6ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info"
	.cfi_startproc
; %bb.0:
	ldr	x8, [x1, #8]
Lloh54:
	adrp	x9, __ZTSN2ml11projections3$_6E@PAGE
Lloh55:
	add	x9, x9, __ZTSN2ml11projections3$_6E@PAGEOFF
	cmp	x8, x9
	b.ne	LBB64_2
LBB64_1:
	add	x0, x0, #8
	ret
LBB64_2:
	tst	x8, x9
	b.lt	LBB64_4
; %bb.3:
	mov	x0, #0                          ; =0x0
	ret
LBB64_4:
	stp	x20, x19, [sp, #-32]!           ; 16-byte Folded Spill
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	mov	x19, x0
	and	x0, x8, #0x7fffffffffffffff
	and	x1, x9, #0x7fffffffffffffff
	bl	_strcmp
	mov	x8, x0
	mov	x0, x19
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp], #32             ; 16-byte Folded Reload
	cbz	w8, LBB64_1
; %bb.5:
	mov	x0, #0                          ; =0x0
	ret
	.loh AdrpAdd	Lloh54, Lloh55
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections3$_6ENS_9allocatorIS4_EEFffEE11target_typeEv
__ZNKSt3__110__function6__funcIN2ml11projections3$_6ENS_9allocatorIS4_EEFffEE11target_typeEv: ; @"_ZNKSt3__110__function6__funcIN2ml11projections3$_6ENS_9allocatorIS4_EEFffEE11target_typeEv"
	.cfi_startproc
; %bb.0:
Lloh56:
	adrp	x0, __ZTIN2ml11projections3$_6E@PAGE
Lloh57:
	add	x0, x0, __ZTIN2ml11projections3$_6E@PAGEOFF
	ret
	.loh AdrpAdd	Lloh56, Lloh57
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections3$_7ENS_9allocatorIS4_EEFffEED1Ev
__ZNSt3__110__function6__funcIN2ml11projections3$_7ENS_9allocatorIS4_EEFffEED1Ev: ; @"_ZNSt3__110__function6__funcIN2ml11projections3$_7ENS_9allocatorIS4_EEFffEED1Ev"
	.cfi_startproc
; %bb.0:
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections3$_7ENS_9allocatorIS4_EEFffEED0Ev
__ZNSt3__110__function6__funcIN2ml11projections3$_7ENS_9allocatorIS4_EEFffEED0Ev: ; @"_ZNSt3__110__function6__funcIN2ml11projections3$_7ENS_9allocatorIS4_EEFffEED0Ev"
	.cfi_startproc
; %bb.0:
	b	__ZdlPv
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections3$_7ENS_9allocatorIS4_EEFffEE7__cloneEv
__ZNKSt3__110__function6__funcIN2ml11projections3$_7ENS_9allocatorIS4_EEFffEE7__cloneEv: ; @"_ZNKSt3__110__function6__funcIN2ml11projections3$_7ENS_9allocatorIS4_EEFffEE7__cloneEv"
	.cfi_startproc
; %bb.0:
	stp	x29, x30, [sp, #-16]!           ; 16-byte Folded Spill
	mov	x29, sp
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	mov	w0, #16                         ; =0x10
	bl	__Znwm
Lloh58:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml11projections3$_7ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh59:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml11projections3$_7ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x0]
	ldp	x29, x30, [sp], #16             ; 16-byte Folded Reload
	ret
	.loh AdrpAdd	Lloh58, Lloh59
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections3$_7ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE
__ZNKSt3__110__function6__funcIN2ml11projections3$_7ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE: ; @"_ZNKSt3__110__function6__funcIN2ml11projections3$_7ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE"
	.cfi_startproc
; %bb.0:
Lloh60:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml11projections3$_7ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh61:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml11projections3$_7ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x1]
	ret
	.loh AdrpAdd	Lloh60, Lloh61
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections3$_7ENS_9allocatorIS4_EEFffEE7destroyEv
__ZNSt3__110__function6__funcIN2ml11projections3$_7ENS_9allocatorIS4_EEFffEE7destroyEv: ; @"_ZNSt3__110__function6__funcIN2ml11projections3$_7ENS_9allocatorIS4_EEFffEE7destroyEv"
	.cfi_startproc
; %bb.0:
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections3$_7ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv
__ZNSt3__110__function6__funcIN2ml11projections3$_7ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv: ; @"_ZNSt3__110__function6__funcIN2ml11projections3$_7ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv"
	.cfi_startproc
; %bb.0:
	b	__ZdlPv
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections3$_7ENS_9allocatorIS4_EEFffEEclEOf
__ZNSt3__110__function6__funcIN2ml11projections3$_7ENS_9allocatorIS4_EEFffEEclEOf: ; @"_ZNSt3__110__function6__funcIN2ml11projections3$_7ENS_9allocatorIS4_EEFffEEclEOf"
	.cfi_startproc
; %bb.0:
	stp	x29, x30, [sp, #-16]!           ; 16-byte Folded Spill
	mov	x29, sp
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	ldr	s0, [x1]
	fadd	s0, s0, s0
	fmov	s1, #-1.00000000
	fadd	s1, s0, s1
	fnmsub	s0, s0, s1, s1
	fmov	s1, #-10.00000000
	fmul	s0, s0, s1
	bl	_exp2f
	ldp	x29, x30, [sp], #16             ; 16-byte Folded Reload
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections3$_7ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info
__ZNKSt3__110__function6__funcIN2ml11projections3$_7ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info: ; @"_ZNKSt3__110__function6__funcIN2ml11projections3$_7ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info"
	.cfi_startproc
; %bb.0:
	ldr	x8, [x1, #8]
Lloh62:
	adrp	x9, __ZTSN2ml11projections3$_7E@PAGE
Lloh63:
	add	x9, x9, __ZTSN2ml11projections3$_7E@PAGEOFF
	cmp	x8, x9
	b.ne	LBB73_2
LBB73_1:
	add	x0, x0, #8
	ret
LBB73_2:
	tst	x8, x9
	b.lt	LBB73_4
; %bb.3:
	mov	x0, #0                          ; =0x0
	ret
LBB73_4:
	stp	x20, x19, [sp, #-32]!           ; 16-byte Folded Spill
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	mov	x19, x0
	and	x0, x8, #0x7fffffffffffffff
	and	x1, x9, #0x7fffffffffffffff
	bl	_strcmp
	mov	x8, x0
	mov	x0, x19
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp], #32             ; 16-byte Folded Reload
	cbz	w8, LBB73_1
; %bb.5:
	mov	x0, #0                          ; =0x0
	ret
	.loh AdrpAdd	Lloh62, Lloh63
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections3$_7ENS_9allocatorIS4_EEFffEE11target_typeEv
__ZNKSt3__110__function6__funcIN2ml11projections3$_7ENS_9allocatorIS4_EEFffEE11target_typeEv: ; @"_ZNKSt3__110__function6__funcIN2ml11projections3$_7ENS_9allocatorIS4_EEFffEE11target_typeEv"
	.cfi_startproc
; %bb.0:
Lloh64:
	adrp	x0, __ZTIN2ml11projections3$_7E@PAGE
Lloh65:
	add	x0, x0, __ZTIN2ml11projections3$_7E@PAGEOFF
	ret
	.loh AdrpAdd	Lloh64, Lloh65
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections3$_8ENS_9allocatorIS4_EEFffEED1Ev
__ZNSt3__110__function6__funcIN2ml11projections3$_8ENS_9allocatorIS4_EEFffEED1Ev: ; @"_ZNSt3__110__function6__funcIN2ml11projections3$_8ENS_9allocatorIS4_EEFffEED1Ev"
	.cfi_startproc
; %bb.0:
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections3$_8ENS_9allocatorIS4_EEFffEED0Ev
__ZNSt3__110__function6__funcIN2ml11projections3$_8ENS_9allocatorIS4_EEFffEED0Ev: ; @"_ZNSt3__110__function6__funcIN2ml11projections3$_8ENS_9allocatorIS4_EEFffEED0Ev"
	.cfi_startproc
; %bb.0:
	b	__ZdlPv
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections3$_8ENS_9allocatorIS4_EEFffEE7__cloneEv
__ZNKSt3__110__function6__funcIN2ml11projections3$_8ENS_9allocatorIS4_EEFffEE7__cloneEv: ; @"_ZNKSt3__110__function6__funcIN2ml11projections3$_8ENS_9allocatorIS4_EEFffEE7__cloneEv"
	.cfi_startproc
; %bb.0:
	stp	x29, x30, [sp, #-16]!           ; 16-byte Folded Spill
	mov	x29, sp
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	mov	w0, #16                         ; =0x10
	bl	__Znwm
Lloh66:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml11projections3$_8ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh67:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml11projections3$_8ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x0]
	ldp	x29, x30, [sp], #16             ; 16-byte Folded Reload
	ret
	.loh AdrpAdd	Lloh66, Lloh67
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections3$_8ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE
__ZNKSt3__110__function6__funcIN2ml11projections3$_8ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE: ; @"_ZNKSt3__110__function6__funcIN2ml11projections3$_8ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE"
	.cfi_startproc
; %bb.0:
Lloh68:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml11projections3$_8ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh69:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml11projections3$_8ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x1]
	ret
	.loh AdrpAdd	Lloh68, Lloh69
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections3$_8ENS_9allocatorIS4_EEFffEE7destroyEv
__ZNSt3__110__function6__funcIN2ml11projections3$_8ENS_9allocatorIS4_EEFffEE7destroyEv: ; @"_ZNSt3__110__function6__funcIN2ml11projections3$_8ENS_9allocatorIS4_EEFffEE7destroyEv"
	.cfi_startproc
; %bb.0:
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections3$_8ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv
__ZNSt3__110__function6__funcIN2ml11projections3$_8ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv: ; @"_ZNSt3__110__function6__funcIN2ml11projections3$_8ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv"
	.cfi_startproc
; %bb.0:
	b	__ZdlPv
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections3$_8ENS_9allocatorIS4_EEFffEEclEOf
__ZNSt3__110__function6__funcIN2ml11projections3$_8ENS_9allocatorIS4_EEFffEEclEOf: ; @"_ZNSt3__110__function6__funcIN2ml11projections3$_8ENS_9allocatorIS4_EEFffEEclEOf"
	.cfi_startproc
; %bb.0:
	ldr	s0, [x1]
	fmov	s1, #-1.00000000
	fadd	s1, s0, s1
	fnmsub	s0, s0, s1, s1
	fmov	s1, #1.00000000
	fsub	s0, s1, s0
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections3$_8ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info
__ZNKSt3__110__function6__funcIN2ml11projections3$_8ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info: ; @"_ZNKSt3__110__function6__funcIN2ml11projections3$_8ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info"
	.cfi_startproc
; %bb.0:
	ldr	x8, [x1, #8]
Lloh70:
	adrp	x9, __ZTSN2ml11projections3$_8E@PAGE
Lloh71:
	add	x9, x9, __ZTSN2ml11projections3$_8E@PAGEOFF
	cmp	x8, x9
	b.ne	LBB82_2
LBB82_1:
	add	x0, x0, #8
	ret
LBB82_2:
	tst	x8, x9
	b.lt	LBB82_4
; %bb.3:
	mov	x0, #0                          ; =0x0
	ret
LBB82_4:
	stp	x20, x19, [sp, #-32]!           ; 16-byte Folded Spill
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	mov	x19, x0
	and	x0, x8, #0x7fffffffffffffff
	and	x1, x9, #0x7fffffffffffffff
	bl	_strcmp
	mov	x8, x0
	mov	x0, x19
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp], #32             ; 16-byte Folded Reload
	cbz	w8, LBB82_1
; %bb.5:
	mov	x0, #0                          ; =0x0
	ret
	.loh AdrpAdd	Lloh70, Lloh71
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections3$_8ENS_9allocatorIS4_EEFffEE11target_typeEv
__ZNKSt3__110__function6__funcIN2ml11projections3$_8ENS_9allocatorIS4_EEFffEE11target_typeEv: ; @"_ZNKSt3__110__function6__funcIN2ml11projections3$_8ENS_9allocatorIS4_EEFffEE11target_typeEv"
	.cfi_startproc
; %bb.0:
Lloh72:
	adrp	x0, __ZTIN2ml11projections3$_8E@PAGE
Lloh73:
	add	x0, x0, __ZTIN2ml11projections3$_8E@PAGEOFF
	ret
	.loh AdrpAdd	Lloh72, Lloh73
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections3$_9ENS_9allocatorIS4_EEFffEED1Ev
__ZNSt3__110__function6__funcIN2ml11projections3$_9ENS_9allocatorIS4_EEFffEED1Ev: ; @"_ZNSt3__110__function6__funcIN2ml11projections3$_9ENS_9allocatorIS4_EEFffEED1Ev"
	.cfi_startproc
; %bb.0:
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections3$_9ENS_9allocatorIS4_EEFffEED0Ev
__ZNSt3__110__function6__funcIN2ml11projections3$_9ENS_9allocatorIS4_EEFffEED0Ev: ; @"_ZNSt3__110__function6__funcIN2ml11projections3$_9ENS_9allocatorIS4_EEFffEED0Ev"
	.cfi_startproc
; %bb.0:
	b	__ZdlPv
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections3$_9ENS_9allocatorIS4_EEFffEE7__cloneEv
__ZNKSt3__110__function6__funcIN2ml11projections3$_9ENS_9allocatorIS4_EEFffEE7__cloneEv: ; @"_ZNKSt3__110__function6__funcIN2ml11projections3$_9ENS_9allocatorIS4_EEFffEE7__cloneEv"
	.cfi_startproc
; %bb.0:
	stp	x29, x30, [sp, #-16]!           ; 16-byte Folded Spill
	mov	x29, sp
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	mov	w0, #16                         ; =0x10
	bl	__Znwm
Lloh74:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml11projections3$_9ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh75:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml11projections3$_9ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x0]
	ldp	x29, x30, [sp], #16             ; 16-byte Folded Reload
	ret
	.loh AdrpAdd	Lloh74, Lloh75
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections3$_9ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE
__ZNKSt3__110__function6__funcIN2ml11projections3$_9ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE: ; @"_ZNKSt3__110__function6__funcIN2ml11projections3$_9ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE"
	.cfi_startproc
; %bb.0:
Lloh76:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml11projections3$_9ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh77:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml11projections3$_9ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x1]
	ret
	.loh AdrpAdd	Lloh76, Lloh77
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections3$_9ENS_9allocatorIS4_EEFffEE7destroyEv
__ZNSt3__110__function6__funcIN2ml11projections3$_9ENS_9allocatorIS4_EEFffEE7destroyEv: ; @"_ZNSt3__110__function6__funcIN2ml11projections3$_9ENS_9allocatorIS4_EEFffEE7destroyEv"
	.cfi_startproc
; %bb.0:
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections3$_9ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv
__ZNSt3__110__function6__funcIN2ml11projections3$_9ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv: ; @"_ZNSt3__110__function6__funcIN2ml11projections3$_9ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv"
	.cfi_startproc
; %bb.0:
	b	__ZdlPv
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections3$_9ENS_9allocatorIS4_EEFffEEclEOf
__ZNSt3__110__function6__funcIN2ml11projections3$_9ENS_9allocatorIS4_EEFffEEclEOf: ; @"_ZNSt3__110__function6__funcIN2ml11projections3$_9ENS_9allocatorIS4_EEFffEEclEOf"
	.cfi_startproc
; %bb.0:
	ldr	s0, [x1]
	fmul	s0, s0, s0
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections3$_9ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info
__ZNKSt3__110__function6__funcIN2ml11projections3$_9ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info: ; @"_ZNKSt3__110__function6__funcIN2ml11projections3$_9ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info"
	.cfi_startproc
; %bb.0:
	ldr	x8, [x1, #8]
Lloh78:
	adrp	x9, __ZTSN2ml11projections3$_9E@PAGE
Lloh79:
	add	x9, x9, __ZTSN2ml11projections3$_9E@PAGEOFF
	cmp	x8, x9
	b.ne	LBB91_2
LBB91_1:
	add	x0, x0, #8
	ret
LBB91_2:
	tst	x8, x9
	b.lt	LBB91_4
; %bb.3:
	mov	x0, #0                          ; =0x0
	ret
LBB91_4:
	stp	x20, x19, [sp, #-32]!           ; 16-byte Folded Spill
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	mov	x19, x0
	and	x0, x8, #0x7fffffffffffffff
	and	x1, x9, #0x7fffffffffffffff
	bl	_strcmp
	mov	x8, x0
	mov	x0, x19
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp], #32             ; 16-byte Folded Reload
	cbz	w8, LBB91_1
; %bb.5:
	mov	x0, #0                          ; =0x0
	ret
	.loh AdrpAdd	Lloh78, Lloh79
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections3$_9ENS_9allocatorIS4_EEFffEE11target_typeEv
__ZNKSt3__110__function6__funcIN2ml11projections3$_9ENS_9allocatorIS4_EEFffEE11target_typeEv: ; @"_ZNKSt3__110__function6__funcIN2ml11projections3$_9ENS_9allocatorIS4_EEFffEE11target_typeEv"
	.cfi_startproc
; %bb.0:
Lloh80:
	adrp	x0, __ZTIN2ml11projections3$_9E@PAGE
Lloh81:
	add	x0, x0, __ZTIN2ml11projections3$_9E@PAGEOFF
	ret
	.loh AdrpAdd	Lloh80, Lloh81
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections4$_10ENS_9allocatorIS4_EEFffEED1Ev
__ZNSt3__110__function6__funcIN2ml11projections4$_10ENS_9allocatorIS4_EEFffEED1Ev: ; @"_ZNSt3__110__function6__funcIN2ml11projections4$_10ENS_9allocatorIS4_EEFffEED1Ev"
	.cfi_startproc
; %bb.0:
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections4$_10ENS_9allocatorIS4_EEFffEED0Ev
__ZNSt3__110__function6__funcIN2ml11projections4$_10ENS_9allocatorIS4_EEFffEED0Ev: ; @"_ZNSt3__110__function6__funcIN2ml11projections4$_10ENS_9allocatorIS4_EEFffEED0Ev"
	.cfi_startproc
; %bb.0:
	b	__ZdlPv
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections4$_10ENS_9allocatorIS4_EEFffEE7__cloneEv
__ZNKSt3__110__function6__funcIN2ml11projections4$_10ENS_9allocatorIS4_EEFffEE7__cloneEv: ; @"_ZNKSt3__110__function6__funcIN2ml11projections4$_10ENS_9allocatorIS4_EEFffEE7__cloneEv"
	.cfi_startproc
; %bb.0:
	stp	x29, x30, [sp, #-16]!           ; 16-byte Folded Spill
	mov	x29, sp
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	mov	w0, #16                         ; =0x10
	bl	__Znwm
Lloh82:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml11projections4$_10ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh83:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml11projections4$_10ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x0]
	ldp	x29, x30, [sp], #16             ; 16-byte Folded Reload
	ret
	.loh AdrpAdd	Lloh82, Lloh83
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections4$_10ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE
__ZNKSt3__110__function6__funcIN2ml11projections4$_10ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE: ; @"_ZNKSt3__110__function6__funcIN2ml11projections4$_10ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE"
	.cfi_startproc
; %bb.0:
Lloh84:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml11projections4$_10ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh85:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml11projections4$_10ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x1]
	ret
	.loh AdrpAdd	Lloh84, Lloh85
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections4$_10ENS_9allocatorIS4_EEFffEE7destroyEv
__ZNSt3__110__function6__funcIN2ml11projections4$_10ENS_9allocatorIS4_EEFffEE7destroyEv: ; @"_ZNSt3__110__function6__funcIN2ml11projections4$_10ENS_9allocatorIS4_EEFffEE7destroyEv"
	.cfi_startproc
; %bb.0:
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections4$_10ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv
__ZNSt3__110__function6__funcIN2ml11projections4$_10ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv: ; @"_ZNSt3__110__function6__funcIN2ml11projections4$_10ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv"
	.cfi_startproc
; %bb.0:
	b	__ZdlPv
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections4$_10ENS_9allocatorIS4_EEFffEEclEOf
__ZNSt3__110__function6__funcIN2ml11projections4$_10ENS_9allocatorIS4_EEFffEEclEOf: ; @"_ZNSt3__110__function6__funcIN2ml11projections4$_10ENS_9allocatorIS4_EEFffEEclEOf"
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #32
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	ldr	s0, [x1]
	fmov	s1, #0.50000000
	fcmp	s0, s1
	b.ge	LBB99_3
; %bb.1:
	fadd	s0, s0, s0
	str	s0, [sp, #8]
Lloh86:
	adrp	x8, __MergedGlobals@PAGE+1624
Lloh87:
	ldr	x0, [x8, __MergedGlobals@PAGEOFF+1624]
	cbz	x0, LBB99_5
; %bb.2:
	ldr	x8, [x0]
	ldr	x8, [x8, #48]
	add	x1, sp, #8
	blr	x8
	fmov	s1, #0.50000000
	fmul	s0, s0, s1
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	add	sp, sp, #32
	ret
LBB99_3:
	fadd	s0, s0, s0
	fmov	s1, #-1.00000000
	fadd	s0, s0, s1
	stur	s0, [x29, #-4]
Lloh88:
	adrp	x8, __MergedGlobals@PAGE+1592
Lloh89:
	ldr	x0, [x8, __MergedGlobals@PAGEOFF+1592]
	cbz	x0, LBB99_5
; %bb.4:
	ldr	x8, [x0]
	ldr	x8, [x8, #48]
	sub	x1, x29, #4
	blr	x8
	fmov	s1, #0.50000000
	fmadd	s0, s0, s1, s1
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	add	sp, sp, #32
	ret
LBB99_5:
	bl	__ZNSt3__125__throw_bad_function_callB8ne200100Ev
	.loh AdrpLdr	Lloh86, Lloh87
	.loh AdrpLdr	Lloh88, Lloh89
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections4$_10ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info
__ZNKSt3__110__function6__funcIN2ml11projections4$_10ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info: ; @"_ZNKSt3__110__function6__funcIN2ml11projections4$_10ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info"
	.cfi_startproc
; %bb.0:
	ldr	x8, [x1, #8]
Lloh90:
	adrp	x9, __ZTSN2ml11projections4$_10E@PAGE
Lloh91:
	add	x9, x9, __ZTSN2ml11projections4$_10E@PAGEOFF
	cmp	x8, x9
	b.ne	LBB100_2
LBB100_1:
	add	x0, x0, #8
	ret
LBB100_2:
	tst	x8, x9
	b.lt	LBB100_4
; %bb.3:
	mov	x0, #0                          ; =0x0
	ret
LBB100_4:
	stp	x20, x19, [sp, #-32]!           ; 16-byte Folded Spill
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	mov	x19, x0
	and	x0, x8, #0x7fffffffffffffff
	and	x1, x9, #0x7fffffffffffffff
	bl	_strcmp
	mov	x8, x0
	mov	x0, x19
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp], #32             ; 16-byte Folded Reload
	cbz	w8, LBB100_1
; %bb.5:
	mov	x0, #0                          ; =0x0
	ret
	.loh AdrpAdd	Lloh90, Lloh91
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections4$_10ENS_9allocatorIS4_EEFffEE11target_typeEv
__ZNKSt3__110__function6__funcIN2ml11projections4$_10ENS_9allocatorIS4_EEFffEE11target_typeEv: ; @"_ZNKSt3__110__function6__funcIN2ml11projections4$_10ENS_9allocatorIS4_EEFffEE11target_typeEv"
	.cfi_startproc
; %bb.0:
Lloh92:
	adrp	x0, __ZTIN2ml11projections4$_10E@PAGE
Lloh93:
	add	x0, x0, __ZTIN2ml11projections4$_10E@PAGEOFF
	ret
	.loh AdrpAdd	Lloh92, Lloh93
	.cfi_endproc
                                        ; -- End function
	.private_extern	__ZNSt3__125__throw_bad_function_callB8ne200100Ev ; -- Begin function _ZNSt3__125__throw_bad_function_callB8ne200100Ev
	.globl	__ZNSt3__125__throw_bad_function_callB8ne200100Ev
	.weak_def_can_be_hidden	__ZNSt3__125__throw_bad_function_callB8ne200100Ev
	.p2align	2
__ZNSt3__125__throw_bad_function_callB8ne200100Ev: ; @_ZNSt3__125__throw_bad_function_callB8ne200100Ev
	.cfi_startproc
; %bb.0:
	stp	x29, x30, [sp, #-16]!           ; 16-byte Folded Spill
	mov	x29, sp
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	mov	w0, #8                          ; =0x8
	bl	___cxa_allocate_exception
Lloh94:
	adrp	x8, __ZTVNSt3__117bad_function_callE@GOTPAGE
Lloh95:
	ldr	x8, [x8, __ZTVNSt3__117bad_function_callE@GOTPAGEOFF]
	add	x8, x8, #16
	str	x8, [x0]
Lloh96:
	adrp	x1, __ZTINSt3__117bad_function_callE@PAGE
Lloh97:
	add	x1, x1, __ZTINSt3__117bad_function_callE@PAGEOFF
Lloh98:
	adrp	x2, __ZNSt3__117bad_function_callD1Ev@PAGE
Lloh99:
	add	x2, x2, __ZNSt3__117bad_function_callD1Ev@PAGEOFF
	bl	___cxa_throw
	.loh AdrpAdd	Lloh98, Lloh99
	.loh AdrpAdd	Lloh96, Lloh97
	.loh AdrpLdrGot	Lloh94, Lloh95
	.cfi_endproc
                                        ; -- End function
	.private_extern	__ZNSt3__117bad_function_callD1Ev ; -- Begin function _ZNSt3__117bad_function_callD1Ev
	.globl	__ZNSt3__117bad_function_callD1Ev
	.weak_def_can_be_hidden	__ZNSt3__117bad_function_callD1Ev
	.p2align	2
__ZNSt3__117bad_function_callD1Ev:      ; @_ZNSt3__117bad_function_callD1Ev
	.cfi_startproc
; %bb.0:
	b	__ZNSt9exceptionD2Ev
	.cfi_endproc
                                        ; -- End function
	.private_extern	__ZNSt3__117bad_function_callD0Ev ; -- Begin function _ZNSt3__117bad_function_callD0Ev
	.globl	__ZNSt3__117bad_function_callD0Ev
	.weak_def_can_be_hidden	__ZNSt3__117bad_function_callD0Ev
	.p2align	2
__ZNSt3__117bad_function_callD0Ev:      ; @_ZNSt3__117bad_function_callD0Ev
	.cfi_startproc
; %bb.0:
	stp	x29, x30, [sp, #-16]!           ; 16-byte Folded Spill
	mov	x29, sp
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	bl	__ZNSt9exceptionD2Ev
	ldp	x29, x30, [sp], #16             ; 16-byte Folded Reload
	b	__ZdlPv
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections4$_11ENS_9allocatorIS4_EEFffEED1Ev
__ZNSt3__110__function6__funcIN2ml11projections4$_11ENS_9allocatorIS4_EEFffEED1Ev: ; @"_ZNSt3__110__function6__funcIN2ml11projections4$_11ENS_9allocatorIS4_EEFffEED1Ev"
	.cfi_startproc
; %bb.0:
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections4$_11ENS_9allocatorIS4_EEFffEED0Ev
__ZNSt3__110__function6__funcIN2ml11projections4$_11ENS_9allocatorIS4_EEFffEED0Ev: ; @"_ZNSt3__110__function6__funcIN2ml11projections4$_11ENS_9allocatorIS4_EEFffEED0Ev"
	.cfi_startproc
; %bb.0:
	b	__ZdlPv
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections4$_11ENS_9allocatorIS4_EEFffEE7__cloneEv
__ZNKSt3__110__function6__funcIN2ml11projections4$_11ENS_9allocatorIS4_EEFffEE7__cloneEv: ; @"_ZNKSt3__110__function6__funcIN2ml11projections4$_11ENS_9allocatorIS4_EEFffEE7__cloneEv"
	.cfi_startproc
; %bb.0:
	stp	x29, x30, [sp, #-16]!           ; 16-byte Folded Spill
	mov	x29, sp
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	mov	w0, #16                         ; =0x10
	bl	__Znwm
Lloh100:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml11projections4$_11ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh101:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml11projections4$_11ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x0]
	ldp	x29, x30, [sp], #16             ; 16-byte Folded Reload
	ret
	.loh AdrpAdd	Lloh100, Lloh101
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections4$_11ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE
__ZNKSt3__110__function6__funcIN2ml11projections4$_11ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE: ; @"_ZNKSt3__110__function6__funcIN2ml11projections4$_11ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE"
	.cfi_startproc
; %bb.0:
Lloh102:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml11projections4$_11ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh103:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml11projections4$_11ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x1]
	ret
	.loh AdrpAdd	Lloh102, Lloh103
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections4$_11ENS_9allocatorIS4_EEFffEE7destroyEv
__ZNSt3__110__function6__funcIN2ml11projections4$_11ENS_9allocatorIS4_EEFffEE7destroyEv: ; @"_ZNSt3__110__function6__funcIN2ml11projections4$_11ENS_9allocatorIS4_EEFffEE7destroyEv"
	.cfi_startproc
; %bb.0:
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections4$_11ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv
__ZNSt3__110__function6__funcIN2ml11projections4$_11ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv: ; @"_ZNSt3__110__function6__funcIN2ml11projections4$_11ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv"
	.cfi_startproc
; %bb.0:
	b	__ZdlPv
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections4$_11ENS_9allocatorIS4_EEFffEEclEOf
__ZNSt3__110__function6__funcIN2ml11projections4$_11ENS_9allocatorIS4_EEFffEEclEOf: ; @"_ZNSt3__110__function6__funcIN2ml11projections4$_11ENS_9allocatorIS4_EEFffEEclEOf"
	.cfi_startproc
; %bb.0:
	ldr	s0, [x1]
	fmov	s1, #1.00000000
	fsub	s2, s1, s0
	fmsub	s2, s0, s2, s2
	fmsub	s0, s0, s2, s2
	fsub	s0, s1, s0
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections4$_11ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info
__ZNKSt3__110__function6__funcIN2ml11projections4$_11ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info: ; @"_ZNKSt3__110__function6__funcIN2ml11projections4$_11ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info"
	.cfi_startproc
; %bb.0:
	ldr	x8, [x1, #8]
Lloh104:
	adrp	x9, __ZTSN2ml11projections4$_11E@PAGE
Lloh105:
	add	x9, x9, __ZTSN2ml11projections4$_11E@PAGEOFF
	cmp	x8, x9
	b.ne	LBB112_2
LBB112_1:
	add	x0, x0, #8
	ret
LBB112_2:
	tst	x8, x9
	b.lt	LBB112_4
; %bb.3:
	mov	x0, #0                          ; =0x0
	ret
LBB112_4:
	stp	x20, x19, [sp, #-32]!           ; 16-byte Folded Spill
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	mov	x19, x0
	and	x0, x8, #0x7fffffffffffffff
	and	x1, x9, #0x7fffffffffffffff
	bl	_strcmp
	mov	x8, x0
	mov	x0, x19
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp], #32             ; 16-byte Folded Reload
	cbz	w8, LBB112_1
; %bb.5:
	mov	x0, #0                          ; =0x0
	ret
	.loh AdrpAdd	Lloh104, Lloh105
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections4$_11ENS_9allocatorIS4_EEFffEE11target_typeEv
__ZNKSt3__110__function6__funcIN2ml11projections4$_11ENS_9allocatorIS4_EEFffEE11target_typeEv: ; @"_ZNKSt3__110__function6__funcIN2ml11projections4$_11ENS_9allocatorIS4_EEFffEE11target_typeEv"
	.cfi_startproc
; %bb.0:
Lloh106:
	adrp	x0, __ZTIN2ml11projections4$_11E@PAGE
Lloh107:
	add	x0, x0, __ZTIN2ml11projections4$_11E@PAGEOFF
	ret
	.loh AdrpAdd	Lloh106, Lloh107
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections4$_12ENS_9allocatorIS4_EEFffEED1Ev
__ZNSt3__110__function6__funcIN2ml11projections4$_12ENS_9allocatorIS4_EEFffEED1Ev: ; @"_ZNSt3__110__function6__funcIN2ml11projections4$_12ENS_9allocatorIS4_EEFffEED1Ev"
	.cfi_startproc
; %bb.0:
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections4$_12ENS_9allocatorIS4_EEFffEED0Ev
__ZNSt3__110__function6__funcIN2ml11projections4$_12ENS_9allocatorIS4_EEFffEED0Ev: ; @"_ZNSt3__110__function6__funcIN2ml11projections4$_12ENS_9allocatorIS4_EEFffEED0Ev"
	.cfi_startproc
; %bb.0:
	b	__ZdlPv
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections4$_12ENS_9allocatorIS4_EEFffEE7__cloneEv
__ZNKSt3__110__function6__funcIN2ml11projections4$_12ENS_9allocatorIS4_EEFffEE7__cloneEv: ; @"_ZNKSt3__110__function6__funcIN2ml11projections4$_12ENS_9allocatorIS4_EEFffEE7__cloneEv"
	.cfi_startproc
; %bb.0:
	stp	x29, x30, [sp, #-16]!           ; 16-byte Folded Spill
	mov	x29, sp
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	mov	w0, #16                         ; =0x10
	bl	__Znwm
Lloh108:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml11projections4$_12ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh109:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml11projections4$_12ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x0]
	ldp	x29, x30, [sp], #16             ; 16-byte Folded Reload
	ret
	.loh AdrpAdd	Lloh108, Lloh109
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections4$_12ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE
__ZNKSt3__110__function6__funcIN2ml11projections4$_12ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE: ; @"_ZNKSt3__110__function6__funcIN2ml11projections4$_12ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE"
	.cfi_startproc
; %bb.0:
Lloh110:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml11projections4$_12ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh111:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml11projections4$_12ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x1]
	ret
	.loh AdrpAdd	Lloh110, Lloh111
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections4$_12ENS_9allocatorIS4_EEFffEE7destroyEv
__ZNSt3__110__function6__funcIN2ml11projections4$_12ENS_9allocatorIS4_EEFffEE7destroyEv: ; @"_ZNSt3__110__function6__funcIN2ml11projections4$_12ENS_9allocatorIS4_EEFffEE7destroyEv"
	.cfi_startproc
; %bb.0:
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections4$_12ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv
__ZNSt3__110__function6__funcIN2ml11projections4$_12ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv: ; @"_ZNSt3__110__function6__funcIN2ml11projections4$_12ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv"
	.cfi_startproc
; %bb.0:
	b	__ZdlPv
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections4$_12ENS_9allocatorIS4_EEFffEEclEOf
__ZNSt3__110__function6__funcIN2ml11projections4$_12ENS_9allocatorIS4_EEFffEEclEOf: ; @"_ZNSt3__110__function6__funcIN2ml11projections4$_12ENS_9allocatorIS4_EEFffEEclEOf"
	.cfi_startproc
; %bb.0:
	ldr	s0, [x1]
	fmul	s1, s0, s0
	fmul	s0, s1, s0
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections4$_12ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info
__ZNKSt3__110__function6__funcIN2ml11projections4$_12ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info: ; @"_ZNKSt3__110__function6__funcIN2ml11projections4$_12ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info"
	.cfi_startproc
; %bb.0:
	ldr	x8, [x1, #8]
Lloh112:
	adrp	x9, __ZTSN2ml11projections4$_12E@PAGE
Lloh113:
	add	x9, x9, __ZTSN2ml11projections4$_12E@PAGEOFF
	cmp	x8, x9
	b.ne	LBB121_2
LBB121_1:
	add	x0, x0, #8
	ret
LBB121_2:
	tst	x8, x9
	b.lt	LBB121_4
; %bb.3:
	mov	x0, #0                          ; =0x0
	ret
LBB121_4:
	stp	x20, x19, [sp, #-32]!           ; 16-byte Folded Spill
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	mov	x19, x0
	and	x0, x8, #0x7fffffffffffffff
	and	x1, x9, #0x7fffffffffffffff
	bl	_strcmp
	mov	x8, x0
	mov	x0, x19
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp], #32             ; 16-byte Folded Reload
	cbz	w8, LBB121_1
; %bb.5:
	mov	x0, #0                          ; =0x0
	ret
	.loh AdrpAdd	Lloh112, Lloh113
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections4$_12ENS_9allocatorIS4_EEFffEE11target_typeEv
__ZNKSt3__110__function6__funcIN2ml11projections4$_12ENS_9allocatorIS4_EEFffEE11target_typeEv: ; @"_ZNKSt3__110__function6__funcIN2ml11projections4$_12ENS_9allocatorIS4_EEFffEE11target_typeEv"
	.cfi_startproc
; %bb.0:
Lloh114:
	adrp	x0, __ZTIN2ml11projections4$_12E@PAGE
Lloh115:
	add	x0, x0, __ZTIN2ml11projections4$_12E@PAGEOFF
	ret
	.loh AdrpAdd	Lloh114, Lloh115
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections4$_13ENS_9allocatorIS4_EEFffEED1Ev
__ZNSt3__110__function6__funcIN2ml11projections4$_13ENS_9allocatorIS4_EEFffEED1Ev: ; @"_ZNSt3__110__function6__funcIN2ml11projections4$_13ENS_9allocatorIS4_EEFffEED1Ev"
	.cfi_startproc
; %bb.0:
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections4$_13ENS_9allocatorIS4_EEFffEED0Ev
__ZNSt3__110__function6__funcIN2ml11projections4$_13ENS_9allocatorIS4_EEFffEED0Ev: ; @"_ZNSt3__110__function6__funcIN2ml11projections4$_13ENS_9allocatorIS4_EEFffEED0Ev"
	.cfi_startproc
; %bb.0:
	b	__ZdlPv
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections4$_13ENS_9allocatorIS4_EEFffEE7__cloneEv
__ZNKSt3__110__function6__funcIN2ml11projections4$_13ENS_9allocatorIS4_EEFffEE7__cloneEv: ; @"_ZNKSt3__110__function6__funcIN2ml11projections4$_13ENS_9allocatorIS4_EEFffEE7__cloneEv"
	.cfi_startproc
; %bb.0:
	stp	x29, x30, [sp, #-16]!           ; 16-byte Folded Spill
	mov	x29, sp
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	mov	w0, #16                         ; =0x10
	bl	__Znwm
Lloh116:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml11projections4$_13ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh117:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml11projections4$_13ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x0]
	ldp	x29, x30, [sp], #16             ; 16-byte Folded Reload
	ret
	.loh AdrpAdd	Lloh116, Lloh117
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections4$_13ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE
__ZNKSt3__110__function6__funcIN2ml11projections4$_13ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE: ; @"_ZNKSt3__110__function6__funcIN2ml11projections4$_13ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE"
	.cfi_startproc
; %bb.0:
Lloh118:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml11projections4$_13ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh119:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml11projections4$_13ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x1]
	ret
	.loh AdrpAdd	Lloh118, Lloh119
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections4$_13ENS_9allocatorIS4_EEFffEE7destroyEv
__ZNSt3__110__function6__funcIN2ml11projections4$_13ENS_9allocatorIS4_EEFffEE7destroyEv: ; @"_ZNSt3__110__function6__funcIN2ml11projections4$_13ENS_9allocatorIS4_EEFffEE7destroyEv"
	.cfi_startproc
; %bb.0:
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections4$_13ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv
__ZNSt3__110__function6__funcIN2ml11projections4$_13ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv: ; @"_ZNSt3__110__function6__funcIN2ml11projections4$_13ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv"
	.cfi_startproc
; %bb.0:
	b	__ZdlPv
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections4$_13ENS_9allocatorIS4_EEFffEEclEOf
__ZNSt3__110__function6__funcIN2ml11projections4$_13ENS_9allocatorIS4_EEFffEEclEOf: ; @"_ZNSt3__110__function6__funcIN2ml11projections4$_13ENS_9allocatorIS4_EEFffEEclEOf"
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #32
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	ldr	s0, [x1]
	fmov	s1, #0.50000000
	fcmp	s0, s1
	b.ge	LBB129_3
; %bb.1:
	fadd	s0, s0, s0
	str	s0, [sp, #8]
Lloh120:
	adrp	x8, __MergedGlobals@PAGE+1720
Lloh121:
	ldr	x0, [x8, __MergedGlobals@PAGEOFF+1720]
	cbz	x0, LBB129_5
; %bb.2:
	ldr	x8, [x0]
	ldr	x8, [x8, #48]
	add	x1, sp, #8
	blr	x8
	fmov	s1, #0.50000000
	fmul	s0, s0, s1
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	add	sp, sp, #32
	ret
LBB129_3:
	fadd	s0, s0, s0
	fmov	s1, #-1.00000000
	fadd	s0, s0, s1
	stur	s0, [x29, #-4]
Lloh122:
	adrp	x8, __MergedGlobals@PAGE+1688
Lloh123:
	ldr	x0, [x8, __MergedGlobals@PAGEOFF+1688]
	cbz	x0, LBB129_5
; %bb.4:
	ldr	x8, [x0]
	ldr	x8, [x8, #48]
	sub	x1, x29, #4
	blr	x8
	fmov	s1, #0.50000000
	fmadd	s0, s0, s1, s1
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	add	sp, sp, #32
	ret
LBB129_5:
	bl	__ZNSt3__125__throw_bad_function_callB8ne200100Ev
	.loh AdrpLdr	Lloh120, Lloh121
	.loh AdrpLdr	Lloh122, Lloh123
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections4$_13ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info
__ZNKSt3__110__function6__funcIN2ml11projections4$_13ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info: ; @"_ZNKSt3__110__function6__funcIN2ml11projections4$_13ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info"
	.cfi_startproc
; %bb.0:
	ldr	x8, [x1, #8]
Lloh124:
	adrp	x9, __ZTSN2ml11projections4$_13E@PAGE
Lloh125:
	add	x9, x9, __ZTSN2ml11projections4$_13E@PAGEOFF
	cmp	x8, x9
	b.ne	LBB130_2
LBB130_1:
	add	x0, x0, #8
	ret
LBB130_2:
	tst	x8, x9
	b.lt	LBB130_4
; %bb.3:
	mov	x0, #0                          ; =0x0
	ret
LBB130_4:
	stp	x20, x19, [sp, #-32]!           ; 16-byte Folded Spill
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	mov	x19, x0
	and	x0, x8, #0x7fffffffffffffff
	and	x1, x9, #0x7fffffffffffffff
	bl	_strcmp
	mov	x8, x0
	mov	x0, x19
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp], #32             ; 16-byte Folded Reload
	cbz	w8, LBB130_1
; %bb.5:
	mov	x0, #0                          ; =0x0
	ret
	.loh AdrpAdd	Lloh124, Lloh125
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections4$_13ENS_9allocatorIS4_EEFffEE11target_typeEv
__ZNKSt3__110__function6__funcIN2ml11projections4$_13ENS_9allocatorIS4_EEFffEE11target_typeEv: ; @"_ZNKSt3__110__function6__funcIN2ml11projections4$_13ENS_9allocatorIS4_EEFffEE11target_typeEv"
	.cfi_startproc
; %bb.0:
Lloh126:
	adrp	x0, __ZTIN2ml11projections4$_13E@PAGE
Lloh127:
	add	x0, x0, __ZTIN2ml11projections4$_13E@PAGEOFF
	ret
	.loh AdrpAdd	Lloh126, Lloh127
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections4$_14ENS_9allocatorIS4_EEFffEED1Ev
__ZNSt3__110__function6__funcIN2ml11projections4$_14ENS_9allocatorIS4_EEFffEED1Ev: ; @"_ZNSt3__110__function6__funcIN2ml11projections4$_14ENS_9allocatorIS4_EEFffEED1Ev"
	.cfi_startproc
; %bb.0:
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections4$_14ENS_9allocatorIS4_EEFffEED0Ev
__ZNSt3__110__function6__funcIN2ml11projections4$_14ENS_9allocatorIS4_EEFffEED0Ev: ; @"_ZNSt3__110__function6__funcIN2ml11projections4$_14ENS_9allocatorIS4_EEFffEED0Ev"
	.cfi_startproc
; %bb.0:
	b	__ZdlPv
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections4$_14ENS_9allocatorIS4_EEFffEE7__cloneEv
__ZNKSt3__110__function6__funcIN2ml11projections4$_14ENS_9allocatorIS4_EEFffEE7__cloneEv: ; @"_ZNKSt3__110__function6__funcIN2ml11projections4$_14ENS_9allocatorIS4_EEFffEE7__cloneEv"
	.cfi_startproc
; %bb.0:
	stp	x29, x30, [sp, #-16]!           ; 16-byte Folded Spill
	mov	x29, sp
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	mov	w0, #16                         ; =0x10
	bl	__Znwm
Lloh128:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml11projections4$_14ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh129:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml11projections4$_14ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x0]
	ldp	x29, x30, [sp], #16             ; 16-byte Folded Reload
	ret
	.loh AdrpAdd	Lloh128, Lloh129
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections4$_14ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE
__ZNKSt3__110__function6__funcIN2ml11projections4$_14ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE: ; @"_ZNKSt3__110__function6__funcIN2ml11projections4$_14ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE"
	.cfi_startproc
; %bb.0:
Lloh130:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml11projections4$_14ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh131:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml11projections4$_14ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x1]
	ret
	.loh AdrpAdd	Lloh130, Lloh131
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections4$_14ENS_9allocatorIS4_EEFffEE7destroyEv
__ZNSt3__110__function6__funcIN2ml11projections4$_14ENS_9allocatorIS4_EEFffEE7destroyEv: ; @"_ZNSt3__110__function6__funcIN2ml11projections4$_14ENS_9allocatorIS4_EEFffEE7destroyEv"
	.cfi_startproc
; %bb.0:
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections4$_14ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv
__ZNSt3__110__function6__funcIN2ml11projections4$_14ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv: ; @"_ZNSt3__110__function6__funcIN2ml11projections4$_14ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv"
	.cfi_startproc
; %bb.0:
	b	__ZdlPv
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections4$_14ENS_9allocatorIS4_EEFffEEclEOf
__ZNSt3__110__function6__funcIN2ml11projections4$_14ENS_9allocatorIS4_EEFffEEclEOf: ; @"_ZNSt3__110__function6__funcIN2ml11projections4$_14ENS_9allocatorIS4_EEFffEEclEOf"
	.cfi_startproc
; %bb.0:
	ldr	s0, [x1]
	fmov	s1, #-1.00000000
	fadd	s1, s0, s1
	fnmsub	s0, s0, s1, s1
	fmov	s1, #1.00000000
	fmsub	s0, s0, s0, s1
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections4$_14ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info
__ZNKSt3__110__function6__funcIN2ml11projections4$_14ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info: ; @"_ZNKSt3__110__function6__funcIN2ml11projections4$_14ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info"
	.cfi_startproc
; %bb.0:
	ldr	x8, [x1, #8]
Lloh132:
	adrp	x9, __ZTSN2ml11projections4$_14E@PAGE
Lloh133:
	add	x9, x9, __ZTSN2ml11projections4$_14E@PAGEOFF
	cmp	x8, x9
	b.ne	LBB139_2
LBB139_1:
	add	x0, x0, #8
	ret
LBB139_2:
	tst	x8, x9
	b.lt	LBB139_4
; %bb.3:
	mov	x0, #0                          ; =0x0
	ret
LBB139_4:
	stp	x20, x19, [sp, #-32]!           ; 16-byte Folded Spill
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	mov	x19, x0
	and	x0, x8, #0x7fffffffffffffff
	and	x1, x9, #0x7fffffffffffffff
	bl	_strcmp
	mov	x8, x0
	mov	x0, x19
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp], #32             ; 16-byte Folded Reload
	cbz	w8, LBB139_1
; %bb.5:
	mov	x0, #0                          ; =0x0
	ret
	.loh AdrpAdd	Lloh132, Lloh133
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections4$_14ENS_9allocatorIS4_EEFffEE11target_typeEv
__ZNKSt3__110__function6__funcIN2ml11projections4$_14ENS_9allocatorIS4_EEFffEE11target_typeEv: ; @"_ZNKSt3__110__function6__funcIN2ml11projections4$_14ENS_9allocatorIS4_EEFffEE11target_typeEv"
	.cfi_startproc
; %bb.0:
Lloh134:
	adrp	x0, __ZTIN2ml11projections4$_14E@PAGE
Lloh135:
	add	x0, x0, __ZTIN2ml11projections4$_14E@PAGEOFF
	ret
	.loh AdrpAdd	Lloh134, Lloh135
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections4$_15ENS_9allocatorIS4_EEFffEED1Ev
__ZNSt3__110__function6__funcIN2ml11projections4$_15ENS_9allocatorIS4_EEFffEED1Ev: ; @"_ZNSt3__110__function6__funcIN2ml11projections4$_15ENS_9allocatorIS4_EEFffEED1Ev"
	.cfi_startproc
; %bb.0:
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections4$_15ENS_9allocatorIS4_EEFffEED0Ev
__ZNSt3__110__function6__funcIN2ml11projections4$_15ENS_9allocatorIS4_EEFffEED0Ev: ; @"_ZNSt3__110__function6__funcIN2ml11projections4$_15ENS_9allocatorIS4_EEFffEED0Ev"
	.cfi_startproc
; %bb.0:
	b	__ZdlPv
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections4$_15ENS_9allocatorIS4_EEFffEE7__cloneEv
__ZNKSt3__110__function6__funcIN2ml11projections4$_15ENS_9allocatorIS4_EEFffEE7__cloneEv: ; @"_ZNKSt3__110__function6__funcIN2ml11projections4$_15ENS_9allocatorIS4_EEFffEE7__cloneEv"
	.cfi_startproc
; %bb.0:
	stp	x29, x30, [sp, #-16]!           ; 16-byte Folded Spill
	mov	x29, sp
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	mov	w0, #16                         ; =0x10
	bl	__Znwm
Lloh136:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml11projections4$_15ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh137:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml11projections4$_15ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x0]
	ldp	x29, x30, [sp], #16             ; 16-byte Folded Reload
	ret
	.loh AdrpAdd	Lloh136, Lloh137
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections4$_15ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE
__ZNKSt3__110__function6__funcIN2ml11projections4$_15ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE: ; @"_ZNKSt3__110__function6__funcIN2ml11projections4$_15ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE"
	.cfi_startproc
; %bb.0:
Lloh138:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml11projections4$_15ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh139:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml11projections4$_15ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x1]
	ret
	.loh AdrpAdd	Lloh138, Lloh139
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections4$_15ENS_9allocatorIS4_EEFffEE7destroyEv
__ZNSt3__110__function6__funcIN2ml11projections4$_15ENS_9allocatorIS4_EEFffEE7destroyEv: ; @"_ZNSt3__110__function6__funcIN2ml11projections4$_15ENS_9allocatorIS4_EEFffEE7destroyEv"
	.cfi_startproc
; %bb.0:
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections4$_15ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv
__ZNSt3__110__function6__funcIN2ml11projections4$_15ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv: ; @"_ZNSt3__110__function6__funcIN2ml11projections4$_15ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv"
	.cfi_startproc
; %bb.0:
	b	__ZdlPv
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections4$_15ENS_9allocatorIS4_EEFffEEclEOf
__ZNSt3__110__function6__funcIN2ml11projections4$_15ENS_9allocatorIS4_EEFffEEclEOf: ; @"_ZNSt3__110__function6__funcIN2ml11projections4$_15ENS_9allocatorIS4_EEFffEEclEOf"
	.cfi_startproc
; %bb.0:
	ldr	s0, [x1]
	fmul	s0, s0, s0
	fmul	s0, s0, s0
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections4$_15ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info
__ZNKSt3__110__function6__funcIN2ml11projections4$_15ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info: ; @"_ZNKSt3__110__function6__funcIN2ml11projections4$_15ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info"
	.cfi_startproc
; %bb.0:
	ldr	x8, [x1, #8]
Lloh140:
	adrp	x9, __ZTSN2ml11projections4$_15E@PAGE
Lloh141:
	add	x9, x9, __ZTSN2ml11projections4$_15E@PAGEOFF
	cmp	x8, x9
	b.ne	LBB148_2
LBB148_1:
	add	x0, x0, #8
	ret
LBB148_2:
	tst	x8, x9
	b.lt	LBB148_4
; %bb.3:
	mov	x0, #0                          ; =0x0
	ret
LBB148_4:
	stp	x20, x19, [sp, #-32]!           ; 16-byte Folded Spill
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	mov	x19, x0
	and	x0, x8, #0x7fffffffffffffff
	and	x1, x9, #0x7fffffffffffffff
	bl	_strcmp
	mov	x8, x0
	mov	x0, x19
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp], #32             ; 16-byte Folded Reload
	cbz	w8, LBB148_1
; %bb.5:
	mov	x0, #0                          ; =0x0
	ret
	.loh AdrpAdd	Lloh140, Lloh141
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections4$_15ENS_9allocatorIS4_EEFffEE11target_typeEv
__ZNKSt3__110__function6__funcIN2ml11projections4$_15ENS_9allocatorIS4_EEFffEE11target_typeEv: ; @"_ZNKSt3__110__function6__funcIN2ml11projections4$_15ENS_9allocatorIS4_EEFffEE11target_typeEv"
	.cfi_startproc
; %bb.0:
Lloh142:
	adrp	x0, __ZTIN2ml11projections4$_15E@PAGE
Lloh143:
	add	x0, x0, __ZTIN2ml11projections4$_15E@PAGEOFF
	ret
	.loh AdrpAdd	Lloh142, Lloh143
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections4$_16ENS_9allocatorIS4_EEFffEED1Ev
__ZNSt3__110__function6__funcIN2ml11projections4$_16ENS_9allocatorIS4_EEFffEED1Ev: ; @"_ZNSt3__110__function6__funcIN2ml11projections4$_16ENS_9allocatorIS4_EEFffEED1Ev"
	.cfi_startproc
; %bb.0:
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections4$_16ENS_9allocatorIS4_EEFffEED0Ev
__ZNSt3__110__function6__funcIN2ml11projections4$_16ENS_9allocatorIS4_EEFffEED0Ev: ; @"_ZNSt3__110__function6__funcIN2ml11projections4$_16ENS_9allocatorIS4_EEFffEED0Ev"
	.cfi_startproc
; %bb.0:
	b	__ZdlPv
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections4$_16ENS_9allocatorIS4_EEFffEE7__cloneEv
__ZNKSt3__110__function6__funcIN2ml11projections4$_16ENS_9allocatorIS4_EEFffEE7__cloneEv: ; @"_ZNKSt3__110__function6__funcIN2ml11projections4$_16ENS_9allocatorIS4_EEFffEE7__cloneEv"
	.cfi_startproc
; %bb.0:
	stp	x29, x30, [sp, #-16]!           ; 16-byte Folded Spill
	mov	x29, sp
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	mov	w0, #16                         ; =0x10
	bl	__Znwm
Lloh144:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml11projections4$_16ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh145:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml11projections4$_16ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x0]
	ldp	x29, x30, [sp], #16             ; 16-byte Folded Reload
	ret
	.loh AdrpAdd	Lloh144, Lloh145
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections4$_16ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE
__ZNKSt3__110__function6__funcIN2ml11projections4$_16ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE: ; @"_ZNKSt3__110__function6__funcIN2ml11projections4$_16ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE"
	.cfi_startproc
; %bb.0:
Lloh146:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml11projections4$_16ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh147:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml11projections4$_16ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x1]
	ret
	.loh AdrpAdd	Lloh146, Lloh147
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections4$_16ENS_9allocatorIS4_EEFffEE7destroyEv
__ZNSt3__110__function6__funcIN2ml11projections4$_16ENS_9allocatorIS4_EEFffEE7destroyEv: ; @"_ZNSt3__110__function6__funcIN2ml11projections4$_16ENS_9allocatorIS4_EEFffEE7destroyEv"
	.cfi_startproc
; %bb.0:
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections4$_16ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv
__ZNSt3__110__function6__funcIN2ml11projections4$_16ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv: ; @"_ZNSt3__110__function6__funcIN2ml11projections4$_16ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv"
	.cfi_startproc
; %bb.0:
	b	__ZdlPv
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections4$_16ENS_9allocatorIS4_EEFffEEclEOf
__ZNSt3__110__function6__funcIN2ml11projections4$_16ENS_9allocatorIS4_EEFffEEclEOf: ; @"_ZNSt3__110__function6__funcIN2ml11projections4$_16ENS_9allocatorIS4_EEFffEEclEOf"
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #32
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	ldr	s0, [x1]
	fmov	s1, #0.50000000
	fcmp	s0, s1
	b.ge	LBB156_3
; %bb.1:
	fadd	s0, s0, s0
	str	s0, [sp, #8]
Lloh148:
	adrp	x8, __MergedGlobals@PAGE+1816
Lloh149:
	ldr	x0, [x8, __MergedGlobals@PAGEOFF+1816]
	cbz	x0, LBB156_5
; %bb.2:
	ldr	x8, [x0]
	ldr	x8, [x8, #48]
	add	x1, sp, #8
	blr	x8
	fmov	s1, #0.50000000
	fmul	s0, s0, s1
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	add	sp, sp, #32
	ret
LBB156_3:
	fadd	s0, s0, s0
	fmov	s1, #-1.00000000
	fadd	s0, s0, s1
	stur	s0, [x29, #-4]
Lloh150:
	adrp	x8, __MergedGlobals@PAGE+1784
Lloh151:
	ldr	x0, [x8, __MergedGlobals@PAGEOFF+1784]
	cbz	x0, LBB156_5
; %bb.4:
	ldr	x8, [x0]
	ldr	x8, [x8, #48]
	sub	x1, x29, #4
	blr	x8
	fmov	s1, #0.50000000
	fmadd	s0, s0, s1, s1
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	add	sp, sp, #32
	ret
LBB156_5:
	bl	__ZNSt3__125__throw_bad_function_callB8ne200100Ev
	.loh AdrpLdr	Lloh148, Lloh149
	.loh AdrpLdr	Lloh150, Lloh151
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections4$_16ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info
__ZNKSt3__110__function6__funcIN2ml11projections4$_16ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info: ; @"_ZNKSt3__110__function6__funcIN2ml11projections4$_16ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info"
	.cfi_startproc
; %bb.0:
	ldr	x8, [x1, #8]
Lloh152:
	adrp	x9, __ZTSN2ml11projections4$_16E@PAGE
Lloh153:
	add	x9, x9, __ZTSN2ml11projections4$_16E@PAGEOFF
	cmp	x8, x9
	b.ne	LBB157_2
LBB157_1:
	add	x0, x0, #8
	ret
LBB157_2:
	tst	x8, x9
	b.lt	LBB157_4
; %bb.3:
	mov	x0, #0                          ; =0x0
	ret
LBB157_4:
	stp	x20, x19, [sp, #-32]!           ; 16-byte Folded Spill
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	mov	x19, x0
	and	x0, x8, #0x7fffffffffffffff
	and	x1, x9, #0x7fffffffffffffff
	bl	_strcmp
	mov	x8, x0
	mov	x0, x19
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp], #32             ; 16-byte Folded Reload
	cbz	w8, LBB157_1
; %bb.5:
	mov	x0, #0                          ; =0x0
	ret
	.loh AdrpAdd	Lloh152, Lloh153
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections4$_16ENS_9allocatorIS4_EEFffEE11target_typeEv
__ZNKSt3__110__function6__funcIN2ml11projections4$_16ENS_9allocatorIS4_EEFffEE11target_typeEv: ; @"_ZNKSt3__110__function6__funcIN2ml11projections4$_16ENS_9allocatorIS4_EEFffEE11target_typeEv"
	.cfi_startproc
; %bb.0:
Lloh154:
	adrp	x0, __ZTIN2ml11projections4$_16E@PAGE
Lloh155:
	add	x0, x0, __ZTIN2ml11projections4$_16E@PAGEOFF
	ret
	.loh AdrpAdd	Lloh154, Lloh155
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections4$_17ENS_9allocatorIS4_EEFffEED1Ev
__ZNSt3__110__function6__funcIN2ml11projections4$_17ENS_9allocatorIS4_EEFffEED1Ev: ; @"_ZNSt3__110__function6__funcIN2ml11projections4$_17ENS_9allocatorIS4_EEFffEED1Ev"
	.cfi_startproc
; %bb.0:
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections4$_17ENS_9allocatorIS4_EEFffEED0Ev
__ZNSt3__110__function6__funcIN2ml11projections4$_17ENS_9allocatorIS4_EEFffEED0Ev: ; @"_ZNSt3__110__function6__funcIN2ml11projections4$_17ENS_9allocatorIS4_EEFffEED0Ev"
	.cfi_startproc
; %bb.0:
	b	__ZdlPv
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections4$_17ENS_9allocatorIS4_EEFffEE7__cloneEv
__ZNKSt3__110__function6__funcIN2ml11projections4$_17ENS_9allocatorIS4_EEFffEE7__cloneEv: ; @"_ZNKSt3__110__function6__funcIN2ml11projections4$_17ENS_9allocatorIS4_EEFffEE7__cloneEv"
	.cfi_startproc
; %bb.0:
	stp	x29, x30, [sp, #-16]!           ; 16-byte Folded Spill
	mov	x29, sp
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	mov	w0, #16                         ; =0x10
	bl	__Znwm
Lloh156:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml11projections4$_17ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh157:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml11projections4$_17ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x0]
	ldp	x29, x30, [sp], #16             ; 16-byte Folded Reload
	ret
	.loh AdrpAdd	Lloh156, Lloh157
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections4$_17ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE
__ZNKSt3__110__function6__funcIN2ml11projections4$_17ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE: ; @"_ZNKSt3__110__function6__funcIN2ml11projections4$_17ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE"
	.cfi_startproc
; %bb.0:
Lloh158:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml11projections4$_17ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh159:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml11projections4$_17ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x1]
	ret
	.loh AdrpAdd	Lloh158, Lloh159
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections4$_17ENS_9allocatorIS4_EEFffEE7destroyEv
__ZNSt3__110__function6__funcIN2ml11projections4$_17ENS_9allocatorIS4_EEFffEE7destroyEv: ; @"_ZNSt3__110__function6__funcIN2ml11projections4$_17ENS_9allocatorIS4_EEFffEE7destroyEv"
	.cfi_startproc
; %bb.0:
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections4$_17ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv
__ZNSt3__110__function6__funcIN2ml11projections4$_17ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv: ; @"_ZNSt3__110__function6__funcIN2ml11projections4$_17ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv"
	.cfi_startproc
; %bb.0:
	b	__ZdlPv
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections4$_17ENS_9allocatorIS4_EEFffEEclEOf
__ZNSt3__110__function6__funcIN2ml11projections4$_17ENS_9allocatorIS4_EEFffEEclEOf: ; @"_ZNSt3__110__function6__funcIN2ml11projections4$_17ENS_9allocatorIS4_EEFffEEclEOf"
	.cfi_startproc
; %bb.0:
	ldr	s0, [x1]
	fmov	s1, #3.00000000
	fmul	s2, s0, s0
	fadd	s2, s2, s2
	fnmsub	s0, s0, s1, s2
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections4$_17ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info
__ZNKSt3__110__function6__funcIN2ml11projections4$_17ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info: ; @"_ZNKSt3__110__function6__funcIN2ml11projections4$_17ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info"
	.cfi_startproc
; %bb.0:
	ldr	x8, [x1, #8]
Lloh160:
	adrp	x9, __ZTSN2ml11projections4$_17E@PAGE
Lloh161:
	add	x9, x9, __ZTSN2ml11projections4$_17E@PAGEOFF
	cmp	x8, x9
	b.ne	LBB166_2
LBB166_1:
	add	x0, x0, #8
	ret
LBB166_2:
	tst	x8, x9
	b.lt	LBB166_4
; %bb.3:
	mov	x0, #0                          ; =0x0
	ret
LBB166_4:
	stp	x20, x19, [sp, #-32]!           ; 16-byte Folded Spill
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	mov	x19, x0
	and	x0, x8, #0x7fffffffffffffff
	and	x1, x9, #0x7fffffffffffffff
	bl	_strcmp
	mov	x8, x0
	mov	x0, x19
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp], #32             ; 16-byte Folded Reload
	cbz	w8, LBB166_1
; %bb.5:
	mov	x0, #0                          ; =0x0
	ret
	.loh AdrpAdd	Lloh160, Lloh161
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections4$_17ENS_9allocatorIS4_EEFffEE11target_typeEv
__ZNKSt3__110__function6__funcIN2ml11projections4$_17ENS_9allocatorIS4_EEFffEE11target_typeEv: ; @"_ZNKSt3__110__function6__funcIN2ml11projections4$_17ENS_9allocatorIS4_EEFffEE11target_typeEv"
	.cfi_startproc
; %bb.0:
Lloh162:
	adrp	x0, __ZTIN2ml11projections4$_17E@PAGE
Lloh163:
	add	x0, x0, __ZTIN2ml11projections4$_17E@PAGEOFF
	ret
	.loh AdrpAdd	Lloh162, Lloh163
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections4$_18ENS_9allocatorIS4_EEFffEED1Ev
__ZNSt3__110__function6__funcIN2ml11projections4$_18ENS_9allocatorIS4_EEFffEED1Ev: ; @"_ZNSt3__110__function6__funcIN2ml11projections4$_18ENS_9allocatorIS4_EEFffEED1Ev"
	.cfi_startproc
; %bb.0:
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections4$_18ENS_9allocatorIS4_EEFffEED0Ev
__ZNSt3__110__function6__funcIN2ml11projections4$_18ENS_9allocatorIS4_EEFffEED0Ev: ; @"_ZNSt3__110__function6__funcIN2ml11projections4$_18ENS_9allocatorIS4_EEFffEED0Ev"
	.cfi_startproc
; %bb.0:
	b	__ZdlPv
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections4$_18ENS_9allocatorIS4_EEFffEE7__cloneEv
__ZNKSt3__110__function6__funcIN2ml11projections4$_18ENS_9allocatorIS4_EEFffEE7__cloneEv: ; @"_ZNKSt3__110__function6__funcIN2ml11projections4$_18ENS_9allocatorIS4_EEFffEE7__cloneEv"
	.cfi_startproc
; %bb.0:
	stp	x29, x30, [sp, #-16]!           ; 16-byte Folded Spill
	mov	x29, sp
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	mov	w0, #16                         ; =0x10
	bl	__Znwm
Lloh164:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml11projections4$_18ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh165:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml11projections4$_18ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x0]
	ldp	x29, x30, [sp], #16             ; 16-byte Folded Reload
	ret
	.loh AdrpAdd	Lloh164, Lloh165
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections4$_18ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE
__ZNKSt3__110__function6__funcIN2ml11projections4$_18ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE: ; @"_ZNKSt3__110__function6__funcIN2ml11projections4$_18ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE"
	.cfi_startproc
; %bb.0:
Lloh166:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml11projections4$_18ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh167:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml11projections4$_18ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x1]
	ret
	.loh AdrpAdd	Lloh166, Lloh167
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections4$_18ENS_9allocatorIS4_EEFffEE7destroyEv
__ZNSt3__110__function6__funcIN2ml11projections4$_18ENS_9allocatorIS4_EEFffEE7destroyEv: ; @"_ZNSt3__110__function6__funcIN2ml11projections4$_18ENS_9allocatorIS4_EEFffEE7destroyEv"
	.cfi_startproc
; %bb.0:
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections4$_18ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv
__ZNSt3__110__function6__funcIN2ml11projections4$_18ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv: ; @"_ZNSt3__110__function6__funcIN2ml11projections4$_18ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv"
	.cfi_startproc
; %bb.0:
	b	__ZdlPv
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections4$_18ENS_9allocatorIS4_EEFffEEclEOf
__ZNSt3__110__function6__funcIN2ml11projections4$_18ENS_9allocatorIS4_EEFffEEclEOf: ; @"_ZNSt3__110__function6__funcIN2ml11projections4$_18ENS_9allocatorIS4_EEFffEEclEOf"
	.cfi_startproc
; %bb.0:
	ldr	s0, [x1]
	fabs	s1, s0
	fmul	s0, s1, s0
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections4$_18ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info
__ZNKSt3__110__function6__funcIN2ml11projections4$_18ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info: ; @"_ZNKSt3__110__function6__funcIN2ml11projections4$_18ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info"
	.cfi_startproc
; %bb.0:
	ldr	x8, [x1, #8]
Lloh168:
	adrp	x9, __ZTSN2ml11projections4$_18E@PAGE
Lloh169:
	add	x9, x9, __ZTSN2ml11projections4$_18E@PAGEOFF
	cmp	x8, x9
	b.ne	LBB175_2
LBB175_1:
	add	x0, x0, #8
	ret
LBB175_2:
	tst	x8, x9
	b.lt	LBB175_4
; %bb.3:
	mov	x0, #0                          ; =0x0
	ret
LBB175_4:
	stp	x20, x19, [sp, #-32]!           ; 16-byte Folded Spill
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	mov	x19, x0
	and	x0, x8, #0x7fffffffffffffff
	and	x1, x9, #0x7fffffffffffffff
	bl	_strcmp
	mov	x8, x0
	mov	x0, x19
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp], #32             ; 16-byte Folded Reload
	cbz	w8, LBB175_1
; %bb.5:
	mov	x0, #0                          ; =0x0
	ret
	.loh AdrpAdd	Lloh168, Lloh169
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections4$_18ENS_9allocatorIS4_EEFffEE11target_typeEv
__ZNKSt3__110__function6__funcIN2ml11projections4$_18ENS_9allocatorIS4_EEFffEE11target_typeEv: ; @"_ZNKSt3__110__function6__funcIN2ml11projections4$_18ENS_9allocatorIS4_EEFffEE11target_typeEv"
	.cfi_startproc
; %bb.0:
Lloh170:
	adrp	x0, __ZTIN2ml11projections4$_18E@PAGE
Lloh171:
	add	x0, x0, __ZTIN2ml11projections4$_18E@PAGEOFF
	ret
	.loh AdrpAdd	Lloh170, Lloh171
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections4$_19ENS_9allocatorIS4_EEFffEED1Ev
__ZNSt3__110__function6__funcIN2ml11projections4$_19ENS_9allocatorIS4_EEFffEED1Ev: ; @"_ZNSt3__110__function6__funcIN2ml11projections4$_19ENS_9allocatorIS4_EEFffEED1Ev"
	.cfi_startproc
; %bb.0:
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections4$_19ENS_9allocatorIS4_EEFffEED0Ev
__ZNSt3__110__function6__funcIN2ml11projections4$_19ENS_9allocatorIS4_EEFffEED0Ev: ; @"_ZNSt3__110__function6__funcIN2ml11projections4$_19ENS_9allocatorIS4_EEFffEED0Ev"
	.cfi_startproc
; %bb.0:
	b	__ZdlPv
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections4$_19ENS_9allocatorIS4_EEFffEE7__cloneEv
__ZNKSt3__110__function6__funcIN2ml11projections4$_19ENS_9allocatorIS4_EEFffEE7__cloneEv: ; @"_ZNKSt3__110__function6__funcIN2ml11projections4$_19ENS_9allocatorIS4_EEFffEE7__cloneEv"
	.cfi_startproc
; %bb.0:
	stp	x29, x30, [sp, #-16]!           ; 16-byte Folded Spill
	mov	x29, sp
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	mov	w0, #16                         ; =0x10
	bl	__Znwm
Lloh172:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml11projections4$_19ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh173:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml11projections4$_19ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x0]
	ldp	x29, x30, [sp], #16             ; 16-byte Folded Reload
	ret
	.loh AdrpAdd	Lloh172, Lloh173
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections4$_19ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE
__ZNKSt3__110__function6__funcIN2ml11projections4$_19ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE: ; @"_ZNKSt3__110__function6__funcIN2ml11projections4$_19ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE"
	.cfi_startproc
; %bb.0:
Lloh174:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml11projections4$_19ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh175:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml11projections4$_19ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x1]
	ret
	.loh AdrpAdd	Lloh174, Lloh175
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections4$_19ENS_9allocatorIS4_EEFffEE7destroyEv
__ZNSt3__110__function6__funcIN2ml11projections4$_19ENS_9allocatorIS4_EEFffEE7destroyEv: ; @"_ZNSt3__110__function6__funcIN2ml11projections4$_19ENS_9allocatorIS4_EEFffEE7destroyEv"
	.cfi_startproc
; %bb.0:
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections4$_19ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv
__ZNSt3__110__function6__funcIN2ml11projections4$_19ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv: ; @"_ZNSt3__110__function6__funcIN2ml11projections4$_19ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv"
	.cfi_startproc
; %bb.0:
	b	__ZdlPv
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml11projections4$_19ENS_9allocatorIS4_EEFffEEclEOf
__ZNSt3__110__function6__funcIN2ml11projections4$_19ENS_9allocatorIS4_EEFffEEclEOf: ; @"_ZNSt3__110__function6__funcIN2ml11projections4$_19ENS_9allocatorIS4_EEFffEEclEOf"
	.cfi_startproc
; %bb.0:
	ldr	s0, [x1]
	fabs	s1, s0
	fsqrt	s1, s1
	fcmp	s0, #0.0
	fmov	s2, #-1.00000000
	fmov	s3, #1.00000000
	fcsel	s2, s3, s2, gt
	fcsel	s0, s0, s2, eq
	fmul	s0, s0, s1
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections4$_19ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info
__ZNKSt3__110__function6__funcIN2ml11projections4$_19ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info: ; @"_ZNKSt3__110__function6__funcIN2ml11projections4$_19ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info"
	.cfi_startproc
; %bb.0:
	ldr	x8, [x1, #8]
Lloh176:
	adrp	x9, __ZTSN2ml11projections4$_19E@PAGE
Lloh177:
	add	x9, x9, __ZTSN2ml11projections4$_19E@PAGEOFF
	cmp	x8, x9
	b.ne	LBB184_2
LBB184_1:
	add	x0, x0, #8
	ret
LBB184_2:
	tst	x8, x9
	b.lt	LBB184_4
; %bb.3:
	mov	x0, #0                          ; =0x0
	ret
LBB184_4:
	stp	x20, x19, [sp, #-32]!           ; 16-byte Folded Spill
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	mov	x19, x0
	and	x0, x8, #0x7fffffffffffffff
	and	x1, x9, #0x7fffffffffffffff
	bl	_strcmp
	mov	x8, x0
	mov	x0, x19
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp], #32             ; 16-byte Folded Reload
	cbz	w8, LBB184_1
; %bb.5:
	mov	x0, #0                          ; =0x0
	ret
	.loh AdrpAdd	Lloh176, Lloh177
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml11projections4$_19ENS_9allocatorIS4_EEFffEE11target_typeEv
__ZNKSt3__110__function6__funcIN2ml11projections4$_19ENS_9allocatorIS4_EEFffEE11target_typeEv: ; @"_ZNKSt3__110__function6__funcIN2ml11projections4$_19ENS_9allocatorIS4_EEFffEE11target_typeEv"
	.cfi_startproc
; %bb.0:
Lloh178:
	adrp	x0, __ZTIN2ml11projections4$_19E@PAGE
Lloh179:
	add	x0, x0, __ZTIN2ml11projections4$_19E@PAGEOFF
	ret
	.loh AdrpAdd	Lloh178, Lloh179
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml10dspwindows4$_20ENS_9allocatorIS4_EEFffEED1Ev
__ZNSt3__110__function6__funcIN2ml10dspwindows4$_20ENS_9allocatorIS4_EEFffEED1Ev: ; @"_ZNSt3__110__function6__funcIN2ml10dspwindows4$_20ENS_9allocatorIS4_EEFffEED1Ev"
	.cfi_startproc
; %bb.0:
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml10dspwindows4$_20ENS_9allocatorIS4_EEFffEED0Ev
__ZNSt3__110__function6__funcIN2ml10dspwindows4$_20ENS_9allocatorIS4_EEFffEED0Ev: ; @"_ZNSt3__110__function6__funcIN2ml10dspwindows4$_20ENS_9allocatorIS4_EEFffEED0Ev"
	.cfi_startproc
; %bb.0:
	b	__ZdlPv
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml10dspwindows4$_20ENS_9allocatorIS4_EEFffEE7__cloneEv
__ZNKSt3__110__function6__funcIN2ml10dspwindows4$_20ENS_9allocatorIS4_EEFffEE7__cloneEv: ; @"_ZNKSt3__110__function6__funcIN2ml10dspwindows4$_20ENS_9allocatorIS4_EEFffEE7__cloneEv"
	.cfi_startproc
; %bb.0:
	stp	x29, x30, [sp, #-16]!           ; 16-byte Folded Spill
	mov	x29, sp
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	mov	w0, #16                         ; =0x10
	bl	__Znwm
Lloh180:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml10dspwindows4$_20ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh181:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml10dspwindows4$_20ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x0]
	ldp	x29, x30, [sp], #16             ; 16-byte Folded Reload
	ret
	.loh AdrpAdd	Lloh180, Lloh181
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml10dspwindows4$_20ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE
__ZNKSt3__110__function6__funcIN2ml10dspwindows4$_20ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE: ; @"_ZNKSt3__110__function6__funcIN2ml10dspwindows4$_20ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE"
	.cfi_startproc
; %bb.0:
Lloh182:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml10dspwindows4$_20ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh183:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml10dspwindows4$_20ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x1]
	ret
	.loh AdrpAdd	Lloh182, Lloh183
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml10dspwindows4$_20ENS_9allocatorIS4_EEFffEE7destroyEv
__ZNSt3__110__function6__funcIN2ml10dspwindows4$_20ENS_9allocatorIS4_EEFffEE7destroyEv: ; @"_ZNSt3__110__function6__funcIN2ml10dspwindows4$_20ENS_9allocatorIS4_EEFffEE7destroyEv"
	.cfi_startproc
; %bb.0:
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml10dspwindows4$_20ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv
__ZNSt3__110__function6__funcIN2ml10dspwindows4$_20ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv: ; @"_ZNSt3__110__function6__funcIN2ml10dspwindows4$_20ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv"
	.cfi_startproc
; %bb.0:
	b	__ZdlPv
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml10dspwindows4$_20ENS_9allocatorIS4_EEFffEEclEOf
__ZNSt3__110__function6__funcIN2ml10dspwindows4$_20ENS_9allocatorIS4_EEFffEEclEOf: ; @"_ZNSt3__110__function6__funcIN2ml10dspwindows4$_20ENS_9allocatorIS4_EEFffEEclEOf"
	.cfi_startproc
; %bb.0:
	ldr	s0, [x1]
	fmov	s1, #0.75000000
	fcmp	s0, s1
	cset	w8, gt
	fmov	s1, #0.25000000
	fcmp	s0, s1
	cset	w9, lt
	orr	w8, w8, w9
	tst	w8, #0x1
	fmov	s0, #1.00000000
	movi	d1, #0000000000000000
	fcsel	s0, s1, s0, ne
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml10dspwindows4$_20ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info
__ZNKSt3__110__function6__funcIN2ml10dspwindows4$_20ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info: ; @"_ZNKSt3__110__function6__funcIN2ml10dspwindows4$_20ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info"
	.cfi_startproc
; %bb.0:
	ldr	x8, [x1, #8]
Lloh184:
	adrp	x9, __ZTSN2ml10dspwindows4$_20E@PAGE
Lloh185:
	add	x9, x9, __ZTSN2ml10dspwindows4$_20E@PAGEOFF
	cmp	x8, x9
	b.ne	LBB193_2
LBB193_1:
	add	x0, x0, #8
	ret
LBB193_2:
	tst	x8, x9
	b.lt	LBB193_4
; %bb.3:
	mov	x0, #0                          ; =0x0
	ret
LBB193_4:
	stp	x20, x19, [sp, #-32]!           ; 16-byte Folded Spill
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	mov	x19, x0
	and	x0, x8, #0x7fffffffffffffff
	and	x1, x9, #0x7fffffffffffffff
	bl	_strcmp
	mov	x8, x0
	mov	x0, x19
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp], #32             ; 16-byte Folded Reload
	cbz	w8, LBB193_1
; %bb.5:
	mov	x0, #0                          ; =0x0
	ret
	.loh AdrpAdd	Lloh184, Lloh185
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml10dspwindows4$_20ENS_9allocatorIS4_EEFffEE11target_typeEv
__ZNKSt3__110__function6__funcIN2ml10dspwindows4$_20ENS_9allocatorIS4_EEFffEE11target_typeEv: ; @"_ZNKSt3__110__function6__funcIN2ml10dspwindows4$_20ENS_9allocatorIS4_EEFffEE11target_typeEv"
	.cfi_startproc
; %bb.0:
Lloh186:
	adrp	x0, __ZTIN2ml10dspwindows4$_20E@PAGE
Lloh187:
	add	x0, x0, __ZTIN2ml10dspwindows4$_20E@PAGEOFF
	ret
	.loh AdrpAdd	Lloh186, Lloh187
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml10dspwindows4$_21ENS_9allocatorIS4_EEFffEED1Ev
__ZNSt3__110__function6__funcIN2ml10dspwindows4$_21ENS_9allocatorIS4_EEFffEED1Ev: ; @"_ZNSt3__110__function6__funcIN2ml10dspwindows4$_21ENS_9allocatorIS4_EEFffEED1Ev"
	.cfi_startproc
; %bb.0:
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml10dspwindows4$_21ENS_9allocatorIS4_EEFffEED0Ev
__ZNSt3__110__function6__funcIN2ml10dspwindows4$_21ENS_9allocatorIS4_EEFffEED0Ev: ; @"_ZNSt3__110__function6__funcIN2ml10dspwindows4$_21ENS_9allocatorIS4_EEFffEED0Ev"
	.cfi_startproc
; %bb.0:
	b	__ZdlPv
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml10dspwindows4$_21ENS_9allocatorIS4_EEFffEE7__cloneEv
__ZNKSt3__110__function6__funcIN2ml10dspwindows4$_21ENS_9allocatorIS4_EEFffEE7__cloneEv: ; @"_ZNKSt3__110__function6__funcIN2ml10dspwindows4$_21ENS_9allocatorIS4_EEFffEE7__cloneEv"
	.cfi_startproc
; %bb.0:
	stp	x29, x30, [sp, #-16]!           ; 16-byte Folded Spill
	mov	x29, sp
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	mov	w0, #16                         ; =0x10
	bl	__Znwm
Lloh188:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml10dspwindows4$_21ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh189:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml10dspwindows4$_21ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x0]
	ldp	x29, x30, [sp], #16             ; 16-byte Folded Reload
	ret
	.loh AdrpAdd	Lloh188, Lloh189
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml10dspwindows4$_21ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE
__ZNKSt3__110__function6__funcIN2ml10dspwindows4$_21ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE: ; @"_ZNKSt3__110__function6__funcIN2ml10dspwindows4$_21ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE"
	.cfi_startproc
; %bb.0:
Lloh190:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml10dspwindows4$_21ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh191:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml10dspwindows4$_21ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x1]
	ret
	.loh AdrpAdd	Lloh190, Lloh191
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml10dspwindows4$_21ENS_9allocatorIS4_EEFffEE7destroyEv
__ZNSt3__110__function6__funcIN2ml10dspwindows4$_21ENS_9allocatorIS4_EEFffEE7destroyEv: ; @"_ZNSt3__110__function6__funcIN2ml10dspwindows4$_21ENS_9allocatorIS4_EEFffEE7destroyEv"
	.cfi_startproc
; %bb.0:
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml10dspwindows4$_21ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv
__ZNSt3__110__function6__funcIN2ml10dspwindows4$_21ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv: ; @"_ZNSt3__110__function6__funcIN2ml10dspwindows4$_21ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv"
	.cfi_startproc
; %bb.0:
	b	__ZdlPv
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml10dspwindows4$_21ENS_9allocatorIS4_EEFffEEclEOf
__ZNSt3__110__function6__funcIN2ml10dspwindows4$_21ENS_9allocatorIS4_EEFffEEclEOf: ; @"_ZNSt3__110__function6__funcIN2ml10dspwindows4$_21ENS_9allocatorIS4_EEFffEEclEOf"
	.cfi_startproc
; %bb.0:
	ldr	s0, [x1]
	fadd	s1, s0, s0
	fmov	s2, #2.00000000
	fsub	s2, s2, s1
	fmov	s3, #0.50000000
	fcmp	s0, s3
	fcsel	s0, s2, s1, gt
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml10dspwindows4$_21ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info
__ZNKSt3__110__function6__funcIN2ml10dspwindows4$_21ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info: ; @"_ZNKSt3__110__function6__funcIN2ml10dspwindows4$_21ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info"
	.cfi_startproc
; %bb.0:
	ldr	x8, [x1, #8]
Lloh192:
	adrp	x9, __ZTSN2ml10dspwindows4$_21E@PAGE
Lloh193:
	add	x9, x9, __ZTSN2ml10dspwindows4$_21E@PAGEOFF
	cmp	x8, x9
	b.ne	LBB202_2
LBB202_1:
	add	x0, x0, #8
	ret
LBB202_2:
	tst	x8, x9
	b.lt	LBB202_4
; %bb.3:
	mov	x0, #0                          ; =0x0
	ret
LBB202_4:
	stp	x20, x19, [sp, #-32]!           ; 16-byte Folded Spill
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	mov	x19, x0
	and	x0, x8, #0x7fffffffffffffff
	and	x1, x9, #0x7fffffffffffffff
	bl	_strcmp
	mov	x8, x0
	mov	x0, x19
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp], #32             ; 16-byte Folded Reload
	cbz	w8, LBB202_1
; %bb.5:
	mov	x0, #0                          ; =0x0
	ret
	.loh AdrpAdd	Lloh192, Lloh193
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml10dspwindows4$_21ENS_9allocatorIS4_EEFffEE11target_typeEv
__ZNKSt3__110__function6__funcIN2ml10dspwindows4$_21ENS_9allocatorIS4_EEFffEE11target_typeEv: ; @"_ZNKSt3__110__function6__funcIN2ml10dspwindows4$_21ENS_9allocatorIS4_EEFffEE11target_typeEv"
	.cfi_startproc
; %bb.0:
Lloh194:
	adrp	x0, __ZTIN2ml10dspwindows4$_21E@PAGE
Lloh195:
	add	x0, x0, __ZTIN2ml10dspwindows4$_21E@PAGEOFF
	ret
	.loh AdrpAdd	Lloh194, Lloh195
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml10dspwindows4$_22ENS_9allocatorIS4_EEFffEED1Ev
__ZNSt3__110__function6__funcIN2ml10dspwindows4$_22ENS_9allocatorIS4_EEFffEED1Ev: ; @"_ZNSt3__110__function6__funcIN2ml10dspwindows4$_22ENS_9allocatorIS4_EEFffEED1Ev"
	.cfi_startproc
; %bb.0:
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml10dspwindows4$_22ENS_9allocatorIS4_EEFffEED0Ev
__ZNSt3__110__function6__funcIN2ml10dspwindows4$_22ENS_9allocatorIS4_EEFffEED0Ev: ; @"_ZNSt3__110__function6__funcIN2ml10dspwindows4$_22ENS_9allocatorIS4_EEFffEED0Ev"
	.cfi_startproc
; %bb.0:
	b	__ZdlPv
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml10dspwindows4$_22ENS_9allocatorIS4_EEFffEE7__cloneEv
__ZNKSt3__110__function6__funcIN2ml10dspwindows4$_22ENS_9allocatorIS4_EEFffEE7__cloneEv: ; @"_ZNKSt3__110__function6__funcIN2ml10dspwindows4$_22ENS_9allocatorIS4_EEFffEE7__cloneEv"
	.cfi_startproc
; %bb.0:
	stp	x29, x30, [sp, #-16]!           ; 16-byte Folded Spill
	mov	x29, sp
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	mov	w0, #16                         ; =0x10
	bl	__Znwm
Lloh196:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml10dspwindows4$_22ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh197:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml10dspwindows4$_22ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x0]
	ldp	x29, x30, [sp], #16             ; 16-byte Folded Reload
	ret
	.loh AdrpAdd	Lloh196, Lloh197
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml10dspwindows4$_22ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE
__ZNKSt3__110__function6__funcIN2ml10dspwindows4$_22ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE: ; @"_ZNKSt3__110__function6__funcIN2ml10dspwindows4$_22ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE"
	.cfi_startproc
; %bb.0:
Lloh198:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml10dspwindows4$_22ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh199:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml10dspwindows4$_22ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x1]
	ret
	.loh AdrpAdd	Lloh198, Lloh199
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml10dspwindows4$_22ENS_9allocatorIS4_EEFffEE7destroyEv
__ZNSt3__110__function6__funcIN2ml10dspwindows4$_22ENS_9allocatorIS4_EEFffEE7destroyEv: ; @"_ZNSt3__110__function6__funcIN2ml10dspwindows4$_22ENS_9allocatorIS4_EEFffEE7destroyEv"
	.cfi_startproc
; %bb.0:
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml10dspwindows4$_22ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv
__ZNSt3__110__function6__funcIN2ml10dspwindows4$_22ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv: ; @"_ZNSt3__110__function6__funcIN2ml10dspwindows4$_22ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv"
	.cfi_startproc
; %bb.0:
	b	__ZdlPv
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml10dspwindows4$_22ENS_9allocatorIS4_EEFffEEclEOf
__ZNSt3__110__function6__funcIN2ml10dspwindows4$_22ENS_9allocatorIS4_EEFffEEclEOf: ; @"_ZNSt3__110__function6__funcIN2ml10dspwindows4$_22ENS_9allocatorIS4_EEFffEEclEOf"
	.cfi_startproc
; %bb.0:
	stp	x29, x30, [sp, #-16]!           ; 16-byte Folded Spill
	mov	x29, sp
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	ldr	s0, [x1]
	mov	w8, #4059                       ; =0xfdb
	movk	w8, #16585, lsl #16
	fmov	s1, w8
	fmul	s0, s0, s1
	bl	_cosf
	fmov	s1, #0.50000000
	fmsub	s0, s0, s1, s1
	ldp	x29, x30, [sp], #16             ; 16-byte Folded Reload
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml10dspwindows4$_22ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info
__ZNKSt3__110__function6__funcIN2ml10dspwindows4$_22ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info: ; @"_ZNKSt3__110__function6__funcIN2ml10dspwindows4$_22ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info"
	.cfi_startproc
; %bb.0:
	ldr	x8, [x1, #8]
Lloh200:
	adrp	x9, __ZTSN2ml10dspwindows4$_22E@PAGE
Lloh201:
	add	x9, x9, __ZTSN2ml10dspwindows4$_22E@PAGEOFF
	cmp	x8, x9
	b.ne	LBB211_2
LBB211_1:
	add	x0, x0, #8
	ret
LBB211_2:
	tst	x8, x9
	b.lt	LBB211_4
; %bb.3:
	mov	x0, #0                          ; =0x0
	ret
LBB211_4:
	stp	x20, x19, [sp, #-32]!           ; 16-byte Folded Spill
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	mov	x19, x0
	and	x0, x8, #0x7fffffffffffffff
	and	x1, x9, #0x7fffffffffffffff
	bl	_strcmp
	mov	x8, x0
	mov	x0, x19
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp], #32             ; 16-byte Folded Reload
	cbz	w8, LBB211_1
; %bb.5:
	mov	x0, #0                          ; =0x0
	ret
	.loh AdrpAdd	Lloh200, Lloh201
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml10dspwindows4$_22ENS_9allocatorIS4_EEFffEE11target_typeEv
__ZNKSt3__110__function6__funcIN2ml10dspwindows4$_22ENS_9allocatorIS4_EEFffEE11target_typeEv: ; @"_ZNKSt3__110__function6__funcIN2ml10dspwindows4$_22ENS_9allocatorIS4_EEFffEE11target_typeEv"
	.cfi_startproc
; %bb.0:
Lloh202:
	adrp	x0, __ZTIN2ml10dspwindows4$_22E@PAGE
Lloh203:
	add	x0, x0, __ZTIN2ml10dspwindows4$_22E@PAGEOFF
	ret
	.loh AdrpAdd	Lloh202, Lloh203
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml10dspwindows4$_23ENS_9allocatorIS4_EEFffEED1Ev
__ZNSt3__110__function6__funcIN2ml10dspwindows4$_23ENS_9allocatorIS4_EEFffEED1Ev: ; @"_ZNSt3__110__function6__funcIN2ml10dspwindows4$_23ENS_9allocatorIS4_EEFffEED1Ev"
	.cfi_startproc
; %bb.0:
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml10dspwindows4$_23ENS_9allocatorIS4_EEFffEED0Ev
__ZNSt3__110__function6__funcIN2ml10dspwindows4$_23ENS_9allocatorIS4_EEFffEED0Ev: ; @"_ZNSt3__110__function6__funcIN2ml10dspwindows4$_23ENS_9allocatorIS4_EEFffEED0Ev"
	.cfi_startproc
; %bb.0:
	b	__ZdlPv
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml10dspwindows4$_23ENS_9allocatorIS4_EEFffEE7__cloneEv
__ZNKSt3__110__function6__funcIN2ml10dspwindows4$_23ENS_9allocatorIS4_EEFffEE7__cloneEv: ; @"_ZNKSt3__110__function6__funcIN2ml10dspwindows4$_23ENS_9allocatorIS4_EEFffEE7__cloneEv"
	.cfi_startproc
; %bb.0:
	stp	x29, x30, [sp, #-16]!           ; 16-byte Folded Spill
	mov	x29, sp
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	mov	w0, #16                         ; =0x10
	bl	__Znwm
Lloh204:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml10dspwindows4$_23ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh205:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml10dspwindows4$_23ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x0]
	ldp	x29, x30, [sp], #16             ; 16-byte Folded Reload
	ret
	.loh AdrpAdd	Lloh204, Lloh205
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml10dspwindows4$_23ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE
__ZNKSt3__110__function6__funcIN2ml10dspwindows4$_23ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE: ; @"_ZNKSt3__110__function6__funcIN2ml10dspwindows4$_23ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE"
	.cfi_startproc
; %bb.0:
Lloh206:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml10dspwindows4$_23ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh207:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml10dspwindows4$_23ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x1]
	ret
	.loh AdrpAdd	Lloh206, Lloh207
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml10dspwindows4$_23ENS_9allocatorIS4_EEFffEE7destroyEv
__ZNSt3__110__function6__funcIN2ml10dspwindows4$_23ENS_9allocatorIS4_EEFffEE7destroyEv: ; @"_ZNSt3__110__function6__funcIN2ml10dspwindows4$_23ENS_9allocatorIS4_EEFffEE7destroyEv"
	.cfi_startproc
; %bb.0:
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml10dspwindows4$_23ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv
__ZNSt3__110__function6__funcIN2ml10dspwindows4$_23ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv: ; @"_ZNSt3__110__function6__funcIN2ml10dspwindows4$_23ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv"
	.cfi_startproc
; %bb.0:
	b	__ZdlPv
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml10dspwindows4$_23ENS_9allocatorIS4_EEFffEEclEOf
__ZNSt3__110__function6__funcIN2ml10dspwindows4$_23ENS_9allocatorIS4_EEFffEEclEOf: ; @"_ZNSt3__110__function6__funcIN2ml10dspwindows4$_23ENS_9allocatorIS4_EEFffEEclEOf"
	.cfi_startproc
; %bb.0:
	stp	x29, x30, [sp, #-16]!           ; 16-byte Folded Spill
	mov	x29, sp
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	ldr	s0, [x1]
	mov	w8, #4059                       ; =0xfdb
	movk	w8, #16585, lsl #16
	fmov	s1, w8
	fmul	s0, s0, s1
	bl	_cosf
	mov	w8, #34079                      ; =0x851f
	movk	w8, #48875, lsl #16
	fmov	s1, w8
	mov	w8, #15729                      ; =0x3d71
	movk	w8, #16138, lsl #16
	fmov	s2, w8
	fmadd	s0, s0, s1, s2
	ldp	x29, x30, [sp], #16             ; 16-byte Folded Reload
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml10dspwindows4$_23ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info
__ZNKSt3__110__function6__funcIN2ml10dspwindows4$_23ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info: ; @"_ZNKSt3__110__function6__funcIN2ml10dspwindows4$_23ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info"
	.cfi_startproc
; %bb.0:
	ldr	x8, [x1, #8]
Lloh208:
	adrp	x9, __ZTSN2ml10dspwindows4$_23E@PAGE
Lloh209:
	add	x9, x9, __ZTSN2ml10dspwindows4$_23E@PAGEOFF
	cmp	x8, x9
	b.ne	LBB220_2
LBB220_1:
	add	x0, x0, #8
	ret
LBB220_2:
	tst	x8, x9
	b.lt	LBB220_4
; %bb.3:
	mov	x0, #0                          ; =0x0
	ret
LBB220_4:
	stp	x20, x19, [sp, #-32]!           ; 16-byte Folded Spill
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	mov	x19, x0
	and	x0, x8, #0x7fffffffffffffff
	and	x1, x9, #0x7fffffffffffffff
	bl	_strcmp
	mov	x8, x0
	mov	x0, x19
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp], #32             ; 16-byte Folded Reload
	cbz	w8, LBB220_1
; %bb.5:
	mov	x0, #0                          ; =0x0
	ret
	.loh AdrpAdd	Lloh208, Lloh209
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml10dspwindows4$_23ENS_9allocatorIS4_EEFffEE11target_typeEv
__ZNKSt3__110__function6__funcIN2ml10dspwindows4$_23ENS_9allocatorIS4_EEFffEE11target_typeEv: ; @"_ZNKSt3__110__function6__funcIN2ml10dspwindows4$_23ENS_9allocatorIS4_EEFffEE11target_typeEv"
	.cfi_startproc
; %bb.0:
Lloh210:
	adrp	x0, __ZTIN2ml10dspwindows4$_23E@PAGE
Lloh211:
	add	x0, x0, __ZTIN2ml10dspwindows4$_23E@PAGEOFF
	ret
	.loh AdrpAdd	Lloh210, Lloh211
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml10dspwindows4$_24ENS_9allocatorIS4_EEFffEED1Ev
__ZNSt3__110__function6__funcIN2ml10dspwindows4$_24ENS_9allocatorIS4_EEFffEED1Ev: ; @"_ZNSt3__110__function6__funcIN2ml10dspwindows4$_24ENS_9allocatorIS4_EEFffEED1Ev"
	.cfi_startproc
; %bb.0:
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml10dspwindows4$_24ENS_9allocatorIS4_EEFffEED0Ev
__ZNSt3__110__function6__funcIN2ml10dspwindows4$_24ENS_9allocatorIS4_EEFffEED0Ev: ; @"_ZNSt3__110__function6__funcIN2ml10dspwindows4$_24ENS_9allocatorIS4_EEFffEED0Ev"
	.cfi_startproc
; %bb.0:
	b	__ZdlPv
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml10dspwindows4$_24ENS_9allocatorIS4_EEFffEE7__cloneEv
__ZNKSt3__110__function6__funcIN2ml10dspwindows4$_24ENS_9allocatorIS4_EEFffEE7__cloneEv: ; @"_ZNKSt3__110__function6__funcIN2ml10dspwindows4$_24ENS_9allocatorIS4_EEFffEE7__cloneEv"
	.cfi_startproc
; %bb.0:
	stp	x29, x30, [sp, #-16]!           ; 16-byte Folded Spill
	mov	x29, sp
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	mov	w0, #16                         ; =0x10
	bl	__Znwm
Lloh212:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml10dspwindows4$_24ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh213:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml10dspwindows4$_24ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x0]
	ldp	x29, x30, [sp], #16             ; 16-byte Folded Reload
	ret
	.loh AdrpAdd	Lloh212, Lloh213
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml10dspwindows4$_24ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE
__ZNKSt3__110__function6__funcIN2ml10dspwindows4$_24ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE: ; @"_ZNKSt3__110__function6__funcIN2ml10dspwindows4$_24ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE"
	.cfi_startproc
; %bb.0:
Lloh214:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml10dspwindows4$_24ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh215:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml10dspwindows4$_24ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x1]
	ret
	.loh AdrpAdd	Lloh214, Lloh215
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml10dspwindows4$_24ENS_9allocatorIS4_EEFffEE7destroyEv
__ZNSt3__110__function6__funcIN2ml10dspwindows4$_24ENS_9allocatorIS4_EEFffEE7destroyEv: ; @"_ZNSt3__110__function6__funcIN2ml10dspwindows4$_24ENS_9allocatorIS4_EEFffEE7destroyEv"
	.cfi_startproc
; %bb.0:
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml10dspwindows4$_24ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv
__ZNSt3__110__function6__funcIN2ml10dspwindows4$_24ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv: ; @"_ZNSt3__110__function6__funcIN2ml10dspwindows4$_24ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv"
	.cfi_startproc
; %bb.0:
	b	__ZdlPv
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml10dspwindows4$_24ENS_9allocatorIS4_EEFffEEclEOf
__ZNSt3__110__function6__funcIN2ml10dspwindows4$_24ENS_9allocatorIS4_EEFffEEclEOf: ; @"_ZNSt3__110__function6__funcIN2ml10dspwindows4$_24ENS_9allocatorIS4_EEFffEEclEOf"
	.cfi_startproc
; %bb.0:
	stp	d9, d8, [sp, #-32]!             ; 16-byte Folded Spill
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset b8, -24
	.cfi_offset b9, -32
	ldr	s8, [x1]
	mov	w8, #4059                       ; =0xfdb
	movk	w8, #16585, lsl #16
	fmov	s0, w8
	fmul	s0, s8, s0
	bl	_cosf
	fmov	s1, #-0.50000000
	mov	w8, #2621                       ; =0xa3d
	movk	w8, #16087, lsl #16
	fmov	s2, w8
	fmadd	s9, s0, s1, s2
	mov	w8, #4059                       ; =0xfdb
	movk	w8, #16713, lsl #16
	fmov	s0, w8
	fmul	s0, s8, s0
	bl	_cosf
	mov	w8, #55050                      ; =0xd70a
	movk	w8, #15779, lsl #16
	fmov	s1, w8
	fmadd	s0, s0, s1, s9
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	ldp	d9, d8, [sp], #32               ; 16-byte Folded Reload
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml10dspwindows4$_24ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info
__ZNKSt3__110__function6__funcIN2ml10dspwindows4$_24ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info: ; @"_ZNKSt3__110__function6__funcIN2ml10dspwindows4$_24ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info"
	.cfi_startproc
; %bb.0:
	ldr	x8, [x1, #8]
Lloh216:
	adrp	x9, __ZTSN2ml10dspwindows4$_24E@PAGE
Lloh217:
	add	x9, x9, __ZTSN2ml10dspwindows4$_24E@PAGEOFF
	cmp	x8, x9
	b.ne	LBB229_2
LBB229_1:
	add	x0, x0, #8
	ret
LBB229_2:
	tst	x8, x9
	b.lt	LBB229_4
; %bb.3:
	mov	x0, #0                          ; =0x0
	ret
LBB229_4:
	stp	x20, x19, [sp, #-32]!           ; 16-byte Folded Spill
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	mov	x19, x0
	and	x0, x8, #0x7fffffffffffffff
	and	x1, x9, #0x7fffffffffffffff
	bl	_strcmp
	mov	x8, x0
	mov	x0, x19
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp], #32             ; 16-byte Folded Reload
	cbz	w8, LBB229_1
; %bb.5:
	mov	x0, #0                          ; =0x0
	ret
	.loh AdrpAdd	Lloh216, Lloh217
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml10dspwindows4$_24ENS_9allocatorIS4_EEFffEE11target_typeEv
__ZNKSt3__110__function6__funcIN2ml10dspwindows4$_24ENS_9allocatorIS4_EEFffEE11target_typeEv: ; @"_ZNKSt3__110__function6__funcIN2ml10dspwindows4$_24ENS_9allocatorIS4_EEFffEE11target_typeEv"
	.cfi_startproc
; %bb.0:
Lloh218:
	adrp	x0, __ZTIN2ml10dspwindows4$_24E@PAGE
Lloh219:
	add	x0, x0, __ZTIN2ml10dspwindows4$_24E@PAGEOFF
	ret
	.loh AdrpAdd	Lloh218, Lloh219
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml10dspwindows4$_25ENS_9allocatorIS4_EEFffEED1Ev
__ZNSt3__110__function6__funcIN2ml10dspwindows4$_25ENS_9allocatorIS4_EEFffEED1Ev: ; @"_ZNSt3__110__function6__funcIN2ml10dspwindows4$_25ENS_9allocatorIS4_EEFffEED1Ev"
	.cfi_startproc
; %bb.0:
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml10dspwindows4$_25ENS_9allocatorIS4_EEFffEED0Ev
__ZNSt3__110__function6__funcIN2ml10dspwindows4$_25ENS_9allocatorIS4_EEFffEED0Ev: ; @"_ZNSt3__110__function6__funcIN2ml10dspwindows4$_25ENS_9allocatorIS4_EEFffEED0Ev"
	.cfi_startproc
; %bb.0:
	b	__ZdlPv
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml10dspwindows4$_25ENS_9allocatorIS4_EEFffEE7__cloneEv
__ZNKSt3__110__function6__funcIN2ml10dspwindows4$_25ENS_9allocatorIS4_EEFffEE7__cloneEv: ; @"_ZNKSt3__110__function6__funcIN2ml10dspwindows4$_25ENS_9allocatorIS4_EEFffEE7__cloneEv"
	.cfi_startproc
; %bb.0:
	stp	x29, x30, [sp, #-16]!           ; 16-byte Folded Spill
	mov	x29, sp
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	mov	w0, #16                         ; =0x10
	bl	__Znwm
Lloh220:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml10dspwindows4$_25ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh221:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml10dspwindows4$_25ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x0]
	ldp	x29, x30, [sp], #16             ; 16-byte Folded Reload
	ret
	.loh AdrpAdd	Lloh220, Lloh221
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml10dspwindows4$_25ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE
__ZNKSt3__110__function6__funcIN2ml10dspwindows4$_25ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE: ; @"_ZNKSt3__110__function6__funcIN2ml10dspwindows4$_25ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE"
	.cfi_startproc
; %bb.0:
Lloh222:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml10dspwindows4$_25ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh223:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml10dspwindows4$_25ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x1]
	ret
	.loh AdrpAdd	Lloh222, Lloh223
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml10dspwindows4$_25ENS_9allocatorIS4_EEFffEE7destroyEv
__ZNSt3__110__function6__funcIN2ml10dspwindows4$_25ENS_9allocatorIS4_EEFffEE7destroyEv: ; @"_ZNSt3__110__function6__funcIN2ml10dspwindows4$_25ENS_9allocatorIS4_EEFffEE7destroyEv"
	.cfi_startproc
; %bb.0:
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml10dspwindows4$_25ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv
__ZNSt3__110__function6__funcIN2ml10dspwindows4$_25ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv: ; @"_ZNSt3__110__function6__funcIN2ml10dspwindows4$_25ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv"
	.cfi_startproc
; %bb.0:
	b	__ZdlPv
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNSt3__110__function6__funcIN2ml10dspwindows4$_25ENS_9allocatorIS4_EEFffEEclEOf
__ZNSt3__110__function6__funcIN2ml10dspwindows4$_25ENS_9allocatorIS4_EEFffEEclEOf: ; @"_ZNSt3__110__function6__funcIN2ml10dspwindows4$_25ENS_9allocatorIS4_EEFffEEclEOf"
	.cfi_startproc
; %bb.0:
	stp	d15, d14, [sp, #-80]!           ; 16-byte Folded Spill
	stp	d13, d12, [sp, #16]             ; 16-byte Folded Spill
	stp	d11, d10, [sp, #32]             ; 16-byte Folded Spill
	stp	d9, d8, [sp, #48]               ; 16-byte Folded Spill
	stp	x29, x30, [sp, #64]             ; 16-byte Folded Spill
	add	x29, sp, #64
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset b8, -24
	.cfi_offset b9, -32
	.cfi_offset b10, -40
	.cfi_offset b11, -48
	.cfi_offset b12, -56
	.cfi_offset b13, -64
	.cfi_offset b14, -72
	.cfi_offset b15, -80
	ldr	s11, [x1]
	mov	w8, #4059                       ; =0xfdb
	movk	w8, #16585, lsl #16
	fmov	s0, w8
	fmul	s0, s11, s0
	bl	_cosf
	fmov	s8, s0
	mov	w8, #4059                       ; =0xfdb
	movk	w8, #16713, lsl #16
	fmov	s0, w8
	fmul	s0, s11, s0
	bl	_cosf
	fmov	s9, s0
	mov	w8, #62832                      ; =0xf570
	movk	w8, #16013, lsl #16
	fmov	s12, w8
	mov	w8, #52196                      ; =0xcbe4
	movk	w8, #16790, lsl #16
	fmov	s0, w8
	fmul	s0, s11, s0
	bl	_cosf
	fmov	s10, s0
	mov	w8, #20668                      ; =0x50bc
	movk	w8, #48853, lsl #16
	fmov	s13, w8
	mov	w8, #11120                      ; =0x2b70
	movk	w8, #48555, lsl #16
	fmov	s14, w8
	mov	w8, #4059                       ; =0xfdb
	movk	w8, #16841, lsl #16
	fmov	s0, w8
	fmul	s0, s11, s0
	bl	_cosf
	mov	w8, #42687                      ; =0xa6bf
	movk	w8, #15331, lsl #16
	fmov	s1, w8
	mov	w8, #49338                      ; =0xc0ba
	movk	w8, #15964, lsl #16
	fmov	s2, w8
	fmadd	s2, s9, s12, s2
	fmadd	s2, s10, s14, s2
	fmadd	s2, s8, s13, s2
	fmadd	s0, s0, s1, s2
	ldp	x29, x30, [sp, #64]             ; 16-byte Folded Reload
	ldp	d9, d8, [sp, #48]               ; 16-byte Folded Reload
	ldp	d11, d10, [sp, #32]             ; 16-byte Folded Reload
	ldp	d13, d12, [sp, #16]             ; 16-byte Folded Reload
	ldp	d15, d14, [sp], #80             ; 16-byte Folded Reload
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml10dspwindows4$_25ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info
__ZNKSt3__110__function6__funcIN2ml10dspwindows4$_25ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info: ; @"_ZNKSt3__110__function6__funcIN2ml10dspwindows4$_25ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info"
	.cfi_startproc
; %bb.0:
	ldr	x8, [x1, #8]
Lloh224:
	adrp	x9, __ZTSN2ml10dspwindows4$_25E@PAGE
Lloh225:
	add	x9, x9, __ZTSN2ml10dspwindows4$_25E@PAGEOFF
	cmp	x8, x9
	b.ne	LBB238_2
LBB238_1:
	add	x0, x0, #8
	ret
LBB238_2:
	tst	x8, x9
	b.lt	LBB238_4
; %bb.3:
	mov	x0, #0                          ; =0x0
	ret
LBB238_4:
	stp	x20, x19, [sp, #-32]!           ; 16-byte Folded Spill
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	mov	x19, x0
	and	x0, x8, #0x7fffffffffffffff
	and	x1, x9, #0x7fffffffffffffff
	bl	_strcmp
	mov	x8, x0
	mov	x0, x19
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp], #32             ; 16-byte Folded Reload
	cbz	w8, LBB238_1
; %bb.5:
	mov	x0, #0                          ; =0x0
	ret
	.loh AdrpAdd	Lloh224, Lloh225
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function _ZNKSt3__110__function6__funcIN2ml10dspwindows4$_25ENS_9allocatorIS4_EEFffEE11target_typeEv
__ZNKSt3__110__function6__funcIN2ml10dspwindows4$_25ENS_9allocatorIS4_EEFffEE11target_typeEv: ; @"_ZNKSt3__110__function6__funcIN2ml10dspwindows4$_25ENS_9allocatorIS4_EEFffEE11target_typeEv"
	.cfi_startproc
; %bb.0:
Lloh226:
	adrp	x0, __ZTIN2ml10dspwindows4$_25E@PAGE
Lloh227:
	add	x0, x0, __ZTIN2ml10dspwindows4$_25E@PAGEOFF
	ret
	.loh AdrpAdd	Lloh226, Lloh227
	.cfi_endproc
                                        ; -- End function
	.section	__TEXT,__StaticInit,regular,pure_instructions
	.p2align	2                               ; -- Begin function _GLOBAL__sub_I_test_phasor.cpp
__GLOBAL__sub_I_test_phasor.cpp:        ; @_GLOBAL__sub_I_test_phasor.cpp
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #64
	stp	x22, x21, [sp, #16]             ; 16-byte Folded Spill
	stp	x20, x19, [sp, #32]             ; 16-byte Folded Spill
	stp	x29, x30, [sp, #48]             ; 16-byte Folded Spill
	add	x29, sp, #48
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	.cfi_offset w21, -40
	.cfi_offset w22, -48
Lloh228:
	adrp	x21, __MergedGlobals@PAGE
Lloh229:
	add	x21, x21, __MergedGlobals@PAGEOFF
Lloh230:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml11projections3$_0ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh231:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml11projections3$_0ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x21, #1312]
	add	x1, x21, #1312
	str	x1, [x21, #1336]
Lloh232:
	adrp	x20, __ZNSt3__18functionIFffEED1Ev@PAGE
Lloh233:
	add	x20, x20, __ZNSt3__18functionIFffEED1Ev@PAGEOFF
Lloh234:
	adrp	x19, ___dso_handle@PAGE
Lloh235:
	add	x19, x19, ___dso_handle@PAGEOFF
	mov	x0, x20
	mov	x2, x19
	bl	___cxa_atexit
Lloh236:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml11projections3$_1ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh237:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml11projections3$_1ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x21, #1344]
	add	x1, x21, #1344
	str	x1, [x21, #1368]
	mov	x0, x20
	mov	x2, x19
	bl	___cxa_atexit
Lloh238:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml11projections3$_2ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh239:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml11projections3$_2ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x21, #1376]
	add	x1, x21, #1376
	str	x1, [x21, #1400]
	mov	x0, x20
	mov	x2, x19
	bl	___cxa_atexit
Lloh240:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml11projections3$_3ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh241:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml11projections3$_3ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x21, #1408]
	add	x1, x21, #1408
	str	x1, [x21, #1432]
	mov	x0, x20
	mov	x2, x19
	bl	___cxa_atexit
Lloh242:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml11projections3$_4ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh243:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml11projections3$_4ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x21, #1440]
	add	x1, x21, #1440
	str	x1, [x21, #1464]
	mov	x0, x20
	mov	x2, x19
	bl	___cxa_atexit
Lloh244:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml11projections3$_5ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh245:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml11projections3$_5ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x21, #1472]
	add	x1, x21, #1472
	str	x1, [x21, #1496]
	mov	x0, x20
	mov	x2, x19
	bl	___cxa_atexit
Lloh246:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml11projections3$_6ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh247:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml11projections3$_6ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x21, #1504]
	add	x1, x21, #1504
	str	x1, [x21, #1528]
	mov	x0, x20
	mov	x2, x19
	bl	___cxa_atexit
Lloh248:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml11projections3$_7ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh249:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml11projections3$_7ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x21, #1536]
	add	x1, x21, #1536
	str	x1, [x21, #1560]
	mov	x0, x20
	mov	x2, x19
	bl	___cxa_atexit
Lloh250:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml11projections3$_8ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh251:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml11projections3$_8ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x21, #1568]
	add	x1, x21, #1568
	str	x1, [x21, #1592]
	mov	x0, x20
	mov	x2, x19
	bl	___cxa_atexit
Lloh252:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml11projections3$_9ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh253:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml11projections3$_9ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x21, #1600]
	add	x1, x21, #1600
	str	x1, [x21, #1624]
	mov	x0, x20
	mov	x2, x19
	bl	___cxa_atexit
Lloh254:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml11projections4$_10ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh255:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml11projections4$_10ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x21, #1632]
	add	x1, x21, #1632
	str	x1, [x21, #1656]
	mov	x0, x20
	mov	x2, x19
	bl	___cxa_atexit
Lloh256:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml11projections4$_11ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh257:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml11projections4$_11ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x21, #1664]
	add	x1, x21, #1664
	str	x1, [x21, #1688]
	mov	x0, x20
	mov	x2, x19
	bl	___cxa_atexit
Lloh258:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml11projections4$_12ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh259:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml11projections4$_12ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x21, #1696]
	add	x1, x21, #1696
	str	x1, [x21, #1720]
	mov	x0, x20
	mov	x2, x19
	bl	___cxa_atexit
Lloh260:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml11projections4$_13ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh261:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml11projections4$_13ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x21, #1728]
	add	x1, x21, #1728
	str	x1, [x21, #1752]
	mov	x0, x20
	mov	x2, x19
	bl	___cxa_atexit
Lloh262:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml11projections4$_14ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh263:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml11projections4$_14ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x21, #1760]
	add	x1, x21, #1760
	str	x1, [x21, #1784]
	mov	x0, x20
	mov	x2, x19
	bl	___cxa_atexit
Lloh264:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml11projections4$_15ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh265:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml11projections4$_15ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x21, #1792]
	add	x1, x21, #1792
	str	x1, [x21, #1816]
	mov	x0, x20
	mov	x2, x19
	bl	___cxa_atexit
Lloh266:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml11projections4$_16ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh267:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml11projections4$_16ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x21, #1824]
	add	x1, x21, #1824
	str	x1, [x21, #1848]
	mov	x0, x20
	mov	x2, x19
	bl	___cxa_atexit
Lloh268:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml11projections4$_17ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh269:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml11projections4$_17ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x21, #1856]
	add	x1, x21, #1856
	str	x1, [x21, #1880]
	mov	x0, x20
	mov	x2, x19
	bl	___cxa_atexit
Lloh270:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml11projections4$_18ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh271:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml11projections4$_18ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x21, #1888]
	add	x1, x21, #1888
	str	x1, [x21, #1912]
	mov	x0, x20
	mov	x2, x19
	bl	___cxa_atexit
Lloh272:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml11projections4$_19ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh273:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml11projections4$_19ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x21, #1920]
	add	x1, x21, #1920
	str	x1, [x21, #1944]
	mov	x0, x20
	mov	x2, x19
	bl	___cxa_atexit
	fmov.4s	v0, #1.00000000
	str	q0, [sp]                        ; 16-byte Folded Spill
	movi.4s	v1, #63, lsl #24
	stp	q0, q1, [x21]
	mov	w8, #1267                       ; =0x4f3
	movk	w8, #16181, lsl #16
	dup.4s	v0, w8
	mov	w8, #8635                       ; =0x21bb
	movk	w8, #15760, lsl #16
	dup.4s	v2, w8
	mov	w8, #53688                      ; =0xd1b8
	movk	w8, #48619, lsl #16
	dup.4s	v3, w8
	stp	q0, q2, [x21, #32]
	mov	w8, #9498                       ; =0x251a
	movk	w8, #15855, lsl #16
	dup.4s	v0, w8
	stp	q3, q0, [x21, #64]
	mov	w8, #23887                      ; =0x5d4f
	movk	w8, #48638, lsl #16
	dup.4s	v0, w8
	mov	w8, #59839                      ; =0xe9bf
	movk	w8, #15889, lsl #16
	dup.4s	v2, w8
	stp	q0, q2, [x21, #96]
	mov	w8, #44624                      ; =0xae50
	movk	w8, #48682, lsl #16
	dup.4s	v0, w8
	mov	w8, #52908                      ; =0xceac
	movk	w8, #15948, lsl #16
	dup.4s	v2, w8
	mov	w8, #65532                      ; =0xfffc
	movk	w8, #48767, lsl #16
	dup.4s	v3, w8
	stp	q0, q2, [x21, #128]
	mov	w8, #43690                      ; =0xaaaa
	movk	w8, #16042, lsl #16
	dup.4s	v0, w8
	stp	q3, q0, [x21, #160]
	mov	w8, #32899                      ; =0x8083
	movk	w8, #47454, lsl #16
	dup.4s	v2, w8
	mov	w8, #32768                      ; =0x8000
	movk	w8, #16177, lsl #16
	dup.4s	v3, w8
	stp	q2, q3, [x21, #192]
	mov	w8, #49317                      ; =0xc0a5
	movk	w8, #17072, lsl #16
	dup.4s	v4, w8
	mov	w8, #49317                      ; =0xc0a5
	movk	w8, #49840, lsl #16
	dup.4s	v5, w8
	mov	w8, #43579                      ; =0xaa3b
	movk	w8, #16312, lsl #16
	dup.4s	v0, w8
	stp	q4, q5, [x21, #224]
	stp	q0, q3, [x21, #256]
	mov	w8, #26983                      ; =0x6967
	movk	w8, #14672, lsl #16
	dup.4s	v3, w8
	stp	q2, q3, [x21, #288]
	mov	w8, #17358                      ; =0x43ce
	movk	w8, #15031, lsl #16
	dup.4s	v2, w8
	mov	w8, #35080                      ; =0x8908
	movk	w8, #15368, lsl #16
	dup.4s	v3, w8
	mov	w8, #43457                      ; =0xa9c1
	movk	w8, #15658, lsl #16
	dup.4s	v4, w8
	stp	q2, q3, [x21, #320]
	mov	w8, #43690                      ; =0xaaaa
	movk	w8, #15914, lsl #16
	dup.4s	v2, w8
	stp	q4, q2, [x21, #352]
	mov	w8, #-1085734912                ; =0xbf490000
	dup.4s	v2, w8
	stp	q1, q2, [x21, #384]
	mov	w8, #40960                      ; =0xa000
	movk	w8, #47485, lsl #16
	dup.4s	v1, w8
	mov	w8, #8553                       ; =0x2169
	movk	w8, #45858, lsl #16
	dup.4s	v2, w8
	mov	w8, #41465                      ; =0xa1f9
	movk	w8, #47436, lsl #16
	dup.4s	v3, w8
	mov	w8, #33694                      ; =0x839e
	movk	w8, #15368, lsl #16
	dup.4s	v4, w8
	stp	q1, q2, [x21, #416]
	stp	q3, q4, [x21, #448]
	mov	w8, #43683                      ; =0xaaa3
	movk	w8, #48682, lsl #16
	dup.4s	v1, w8
	mov	w8, #62926                      ; =0xf5ce
	movk	w8, #14284, lsl #16
	dup.4s	v2, w8
	stp	q1, q2, [x21, #480]
	mov	w8, #1562                       ; =0x61a
	movk	w8, #47798, lsl #16
	dup.4s	v1, w8
	mov	w8, #43685                      ; =0xaaa5
	movk	w8, #15658, lsl #16
	dup.4s	v2, w8
	stp	q1, q2, [x21, #512]
	mov	w8, #63875                      ; =0xf983
	movk	w8, #16290, lsl #16
	dup.4s	v1, w8
	movi.4s	v2, #128, lsl #16
	stp	q1, q2, [x21, #544]
	mvni.4s	v1, #127, msl #16
	fneg.4s	v1, v1
	movi.4s	v2, #127, msl #16
	fneg.4s	v2, v2
	stp	q1, q2, [x21, #576]
	movi.4s	v1, #128, lsl #24
	mvni.4s	v2, #128, lsl #24
	stp	q1, q2, [x21, #608]
	movi.4s	v1, #1
	mvni.4s	v2, #1
	stp	q1, q2, [x21, #640]
	movi.4s	v1, #2
	movi.4s	v2, #4
	stp	q1, q2, [x21, #672]
	movi.4s	v1, #127
	mov	w8, #65190                      ; =0xfea6
	movk	w8, #16255, lsl #16
	dup.4s	v2, w8
	mov	w8, #40852                      ; =0x9f94
	movk	w8, #48682, lsl #16
	dup.4s	v3, w8
	mov	w8, #8802                       ; =0x2262
	movk	w8, #15368, lsl #16
	dup.4s	v4, w8
	stp	q1, q2, [x21, #704]
	stp	q3, q4, [x21, #736]
	mov	w8, #479                        ; =0x1df
	movk	w8, #47434, lsl #16
	dup.4s	v1, w8
	mov	w8, #9117                       ; =0x239d
	movk	w8, #13840, lsl #16
	dup.4s	v2, w8
	stp	q1, q2, [x21, #768]
	mov	w8, #64861                      ; =0xfd5d
	movk	w8, #16255, lsl #16
	dup.4s	v1, w8
	mov	w8, #58593                      ; =0xe4e1
	movk	w8, #48895, lsl #16
	dup.4s	v2, w8
	mov	w8, #63414                      ; =0xf7b6
	movk	w8, #15657, lsl #16
	dup.4s	v3, w8
	mov	w8, #35382                      ; =0x8a36
	movk	w8, #47791, lsl #16
	dup.4s	v4, w8
	stp	q1, q2, [x21, #800]
	stp	q3, q4, [x21, #832]
	mov	w8, #41699                      ; =0xa2e3
	movk	w8, #14237, lsl #16
	dup.4s	v1, w8
	mov	w8, #1325334528                 ; =0x4eff0000
	dup.4s	v2, w8
	stp	q1, q2, [x21, #864]
	mov	w8, #43579                      ; =0xaa3b
	movk	w8, #19256, lsl #16
	dup.4s	v1, w8
	mov	w8, #1316880384                 ; =0x4e7e0000
	dup.4s	v2, w8
	mov	w8, #43367                      ; =0xa967
	movk	w8, #16130, lsl #16
	dup.4s	v3, w8
	mov	w8, #4161                       ; =0x1041
	movk	w8, #16031, lsl #16
	dup.4s	v4, w8
	stp	q1, q2, [x21, #896]
	stp	q3, q4, [x21, #928]
	mov	w8, #11723                      ; =0x2dcb
	movk	w8, #15916, lsl #16
	dup.4s	v1, w8
	mov	w8, #52757                      ; =0xce15
	movk	w8, #47932, lsl #16
	dup.4s	v2, w8
	stp	q1, q2, [x21, #960]
	mov	w8, #64622                      ; =0xfc6e
	movk	w8, #15455, lsl #16
	dup.4s	v1, w8
	mov	w8, #61703                      ; =0xf107
	movk	w8, #49843, lsl #16
	dup.4s	v2, w8
	stp	q1, q2, [x21, #992]
	mov	w8, #57378                      ; =0xe022
	movk	w8, #16481, lsl #16
	dup.4s	v1, w8
	str	q1, [x21, #1024]
	mov	w8, #33962                      ; =0x84aa
	movk	w8, #49181, lsl #16
	dup.4s	v1, w8
	str	q1, [x21, #1040]
	mov	w8, #47196                      ; =0xb85c
	movk	w8, #16272, lsl #16
	dup.4s	v1, w8
	str	q1, [x21, #1056]
	mov	w8, #54713                      ; =0xd5b9
	movk	w8, #48787, lsl #16
	dup.4s	v1, w8
	str	q1, [x21, #1072]
	mov	w8, #52698                      ; =0xcdda
	movk	w8, #15614, lsl #16
	dup.4s	v1, w8
	str	q1, [x21, #1088]
	mov	w8, #29208                      ; =0x7218
	movk	w8, #16177, lsl #16
	dup.4s	v1, w8
	str	q1, [x21, #1104]
	fmov.4s	v2, #9.00000000
	str	q2, [x21, #1120]
	fmov.4s	v2, #27.00000000
	str	q2, [x21, #1136]
	str	q1, [x21, #1152]
	str	q0, [x21, #1168]
Lloh274:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml10dspwindows4$_20ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh275:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml10dspwindows4$_20ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x21, #1952]
	add	x1, x21, #1952
	str	x1, [x21, #1976]
	mov	x0, x20
	mov	x2, x19
	bl	___cxa_atexit
Lloh276:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml10dspwindows4$_21ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh277:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml10dspwindows4$_21ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x21, #1984]
	add	x1, x21, #1984
	str	x1, [x21, #2008]
	mov	x0, x20
	mov	x2, x19
	bl	___cxa_atexit
Lloh278:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml10dspwindows4$_22ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh279:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml10dspwindows4$_22ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x21, #2016]
	add	x1, x21, #2016
	str	x1, [x21, #2040]
	mov	x0, x20
	mov	x2, x19
	bl	___cxa_atexit
Lloh280:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml10dspwindows4$_23ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh281:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml10dspwindows4$_23ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x21, #2048]
	add	x1, x21, #2048
	str	x1, [x21, #2072]
	mov	x0, x20
	mov	x2, x19
	bl	___cxa_atexit
Lloh282:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml10dspwindows4$_24ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh283:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml10dspwindows4$_24ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x21, #2080]
	add	x1, x21, #2080
	str	x1, [x21, #2104]
	mov	x0, x20
	mov	x2, x19
	bl	___cxa_atexit
Lloh284:
	adrp	x8, __ZTVNSt3__110__function6__funcIN2ml10dspwindows4$_25ENS_9allocatorIS4_EEFffEEE@PAGE+16
Lloh285:
	add	x8, x8, __ZTVNSt3__110__function6__funcIN2ml10dspwindows4$_25ENS_9allocatorIS4_EEFffEEE@PAGEOFF+16
	str	x8, [x21, #2112]
	add	x1, x21, #2112
	str	x1, [x21, #2136]
	mov	x0, x20
	mov	x2, x19
	bl	___cxa_atexit
Lloh286:
	adrp	x1, l_.str@PAGE
Lloh287:
	add	x1, x1, l_.str@PAGEOFF
	add	x0, x21, #2144
	bl	__ZN2ml12TextFragmentC1EPKc
Lloh288:
	adrp	x0, __ZN2ml12TextFragmentD1Ev@GOTPAGE
Lloh289:
	ldr	x0, [x0, __ZN2ml12TextFragmentD1Ev@GOTPAGEOFF]
	add	x1, x21, #2144
	mov	x2, x19
	bl	___cxa_atexit
	mov	w8, #1285                       ; =0x505
	movk	w8, #16565, lsl #16
	dup.4s	v0, w8
	str	q0, [x21, #1184]
	mov	w8, #1285                       ; =0x505
	movk	w8, #49077, lsl #16
	dup.4s	v0, w8
	str	q0, [x21, #1200]
	mov	w8, #1285                       ; =0x505
	movk	w8, #16309, lsl #16
	dup.4s	v0, w8
	str	q0, [x21, #1216]
	mov	w8, #50102                      ; =0xc3b6
	movk	w8, #16263, lsl #16
	dup.4s	v0, w8
	str	q0, [x21, #1232]
	mov	w8, #1285                       ; =0x505
	movk	w8, #16437, lsl #16
	dup.4s	v0, w8
	str	q0, [x21, #1248]
	movi.2d	v0, #0000000000000000
	str	q0, [x21, #1264]
	ldr	q0, [sp]                        ; 16-byte Folded Reload
	str	q0, [x21, #1280]
	mov	w8, #43691                      ; =0xaaab
	movk	w8, #15914, lsl #16
	dup.4s	v0, w8
	str	q0, [x21, #1296]
	ldp	x29, x30, [sp, #48]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp, #32]             ; 16-byte Folded Reload
	ldp	x22, x21, [sp, #16]             ; 16-byte Folded Reload
	add	sp, sp, #64
	ret
	.loh AdrpLdrGot	Lloh288, Lloh289
	.loh AdrpAdd	Lloh286, Lloh287
	.loh AdrpAdd	Lloh284, Lloh285
	.loh AdrpAdd	Lloh282, Lloh283
	.loh AdrpAdd	Lloh280, Lloh281
	.loh AdrpAdd	Lloh278, Lloh279
	.loh AdrpAdd	Lloh276, Lloh277
	.loh AdrpAdd	Lloh274, Lloh275
	.loh AdrpAdd	Lloh272, Lloh273
	.loh AdrpAdd	Lloh270, Lloh271
	.loh AdrpAdd	Lloh268, Lloh269
	.loh AdrpAdd	Lloh266, Lloh267
	.loh AdrpAdd	Lloh264, Lloh265
	.loh AdrpAdd	Lloh262, Lloh263
	.loh AdrpAdd	Lloh260, Lloh261
	.loh AdrpAdd	Lloh258, Lloh259
	.loh AdrpAdd	Lloh256, Lloh257
	.loh AdrpAdd	Lloh254, Lloh255
	.loh AdrpAdd	Lloh252, Lloh253
	.loh AdrpAdd	Lloh250, Lloh251
	.loh AdrpAdd	Lloh248, Lloh249
	.loh AdrpAdd	Lloh246, Lloh247
	.loh AdrpAdd	Lloh244, Lloh245
	.loh AdrpAdd	Lloh242, Lloh243
	.loh AdrpAdd	Lloh240, Lloh241
	.loh AdrpAdd	Lloh238, Lloh239
	.loh AdrpAdd	Lloh236, Lloh237
	.loh AdrpAdd	Lloh234, Lloh235
	.loh AdrpAdd	Lloh232, Lloh233
	.loh AdrpAdd	Lloh230, Lloh231
	.loh AdrpAdd	Lloh228, Lloh229
	.cfi_endproc
                                        ; -- End function
	.section	__TEXT,__cstring,cstring_literals
l_.str:                                 ; @.str
	.asciz	"!BLOB!"

	.section	__DATA,__const
	.p2align	3, 0x0                          ; @"_ZTVNSt3__110__function6__funcIN2ml11projections3$_0ENS_9allocatorIS4_EEFffEEE"
__ZTVNSt3__110__function6__funcIN2ml11projections3$_0ENS_9allocatorIS4_EEFffEEE:
	.quad	0
	.quad	__ZTINSt3__110__function6__funcIN2ml11projections3$_0ENS_9allocatorIS4_EEFffEEE
	.quad	__ZNSt3__110__function6__funcIN2ml11projections3$_0ENS_9allocatorIS4_EEFffEED1Ev
	.quad	__ZNSt3__110__function6__funcIN2ml11projections3$_0ENS_9allocatorIS4_EEFffEED0Ev
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections3$_0ENS_9allocatorIS4_EEFffEE7__cloneEv
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections3$_0ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE
	.quad	__ZNSt3__110__function6__funcIN2ml11projections3$_0ENS_9allocatorIS4_EEFffEE7destroyEv
	.quad	__ZNSt3__110__function6__funcIN2ml11projections3$_0ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv
	.quad	__ZNSt3__110__function6__funcIN2ml11projections3$_0ENS_9allocatorIS4_EEFffEEclEOf
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections3$_0ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections3$_0ENS_9allocatorIS4_EEFffEE11target_typeEv

	.section	__TEXT,__const
__ZTSNSt3__110__function6__funcIN2ml11projections3$_0ENS_9allocatorIS4_EEFffEEE: ; @"_ZTSNSt3__110__function6__funcIN2ml11projections3$_0ENS_9allocatorIS4_EEFffEEE"
	.asciz	"NSt3__110__function6__funcIN2ml11projections3$_0ENS_9allocatorIS4_EEFffEEE"

	.private_extern	__ZTSNSt3__110__function6__baseIFffEEE ; @_ZTSNSt3__110__function6__baseIFffEEE
	.globl	__ZTSNSt3__110__function6__baseIFffEEE
	.weak_definition	__ZTSNSt3__110__function6__baseIFffEEE
__ZTSNSt3__110__function6__baseIFffEEE:
	.asciz	"NSt3__110__function6__baseIFffEEE"

	.private_extern	__ZTINSt3__110__function6__baseIFffEEE ; @_ZTINSt3__110__function6__baseIFffEEE
	.section	__DATA,__const
	.globl	__ZTINSt3__110__function6__baseIFffEEE
	.weak_definition	__ZTINSt3__110__function6__baseIFffEEE
	.p2align	3, 0x0
__ZTINSt3__110__function6__baseIFffEEE:
	.quad	__ZTVN10__cxxabiv117__class_type_infoE+16
	.quad	__ZTSNSt3__110__function6__baseIFffEEE-9223372036854775808

	.p2align	3, 0x0                          ; @"_ZTINSt3__110__function6__funcIN2ml11projections3$_0ENS_9allocatorIS4_EEFffEEE"
__ZTINSt3__110__function6__funcIN2ml11projections3$_0ENS_9allocatorIS4_EEFffEEE:
	.quad	__ZTVN10__cxxabiv120__si_class_type_infoE+16
	.quad	__ZTSNSt3__110__function6__funcIN2ml11projections3$_0ENS_9allocatorIS4_EEFffEEE
	.quad	__ZTINSt3__110__function6__baseIFffEEE

	.section	__TEXT,__const
__ZTSN2ml11projections3$_0E:            ; @"_ZTSN2ml11projections3$_0E"
	.asciz	"N2ml11projections3$_0E"

	.section	__DATA,__const
	.p2align	3, 0x0                          ; @"_ZTIN2ml11projections3$_0E"
__ZTIN2ml11projections3$_0E:
	.quad	__ZTVN10__cxxabiv117__class_type_infoE+16
	.quad	__ZTSN2ml11projections3$_0E

	.p2align	3, 0x0                          ; @"_ZTVNSt3__110__function6__funcIN2ml11projections3$_1ENS_9allocatorIS4_EEFffEEE"
__ZTVNSt3__110__function6__funcIN2ml11projections3$_1ENS_9allocatorIS4_EEFffEEE:
	.quad	0
	.quad	__ZTINSt3__110__function6__funcIN2ml11projections3$_1ENS_9allocatorIS4_EEFffEEE
	.quad	__ZNSt3__110__function6__funcIN2ml11projections3$_1ENS_9allocatorIS4_EEFffEED1Ev
	.quad	__ZNSt3__110__function6__funcIN2ml11projections3$_1ENS_9allocatorIS4_EEFffEED0Ev
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections3$_1ENS_9allocatorIS4_EEFffEE7__cloneEv
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections3$_1ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE
	.quad	__ZNSt3__110__function6__funcIN2ml11projections3$_1ENS_9allocatorIS4_EEFffEE7destroyEv
	.quad	__ZNSt3__110__function6__funcIN2ml11projections3$_1ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv
	.quad	__ZNSt3__110__function6__funcIN2ml11projections3$_1ENS_9allocatorIS4_EEFffEEclEOf
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections3$_1ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections3$_1ENS_9allocatorIS4_EEFffEE11target_typeEv

	.section	__TEXT,__const
__ZTSNSt3__110__function6__funcIN2ml11projections3$_1ENS_9allocatorIS4_EEFffEEE: ; @"_ZTSNSt3__110__function6__funcIN2ml11projections3$_1ENS_9allocatorIS4_EEFffEEE"
	.asciz	"NSt3__110__function6__funcIN2ml11projections3$_1ENS_9allocatorIS4_EEFffEEE"

	.section	__DATA,__const
	.p2align	3, 0x0                          ; @"_ZTINSt3__110__function6__funcIN2ml11projections3$_1ENS_9allocatorIS4_EEFffEEE"
__ZTINSt3__110__function6__funcIN2ml11projections3$_1ENS_9allocatorIS4_EEFffEEE:
	.quad	__ZTVN10__cxxabiv120__si_class_type_infoE+16
	.quad	__ZTSNSt3__110__function6__funcIN2ml11projections3$_1ENS_9allocatorIS4_EEFffEEE
	.quad	__ZTINSt3__110__function6__baseIFffEEE

	.section	__TEXT,__const
__ZTSN2ml11projections3$_1E:            ; @"_ZTSN2ml11projections3$_1E"
	.asciz	"N2ml11projections3$_1E"

	.section	__DATA,__const
	.p2align	3, 0x0                          ; @"_ZTIN2ml11projections3$_1E"
__ZTIN2ml11projections3$_1E:
	.quad	__ZTVN10__cxxabiv117__class_type_infoE+16
	.quad	__ZTSN2ml11projections3$_1E

	.p2align	3, 0x0                          ; @"_ZTVNSt3__110__function6__funcIN2ml11projections3$_2ENS_9allocatorIS4_EEFffEEE"
__ZTVNSt3__110__function6__funcIN2ml11projections3$_2ENS_9allocatorIS4_EEFffEEE:
	.quad	0
	.quad	__ZTINSt3__110__function6__funcIN2ml11projections3$_2ENS_9allocatorIS4_EEFffEEE
	.quad	__ZNSt3__110__function6__funcIN2ml11projections3$_2ENS_9allocatorIS4_EEFffEED1Ev
	.quad	__ZNSt3__110__function6__funcIN2ml11projections3$_2ENS_9allocatorIS4_EEFffEED0Ev
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections3$_2ENS_9allocatorIS4_EEFffEE7__cloneEv
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections3$_2ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE
	.quad	__ZNSt3__110__function6__funcIN2ml11projections3$_2ENS_9allocatorIS4_EEFffEE7destroyEv
	.quad	__ZNSt3__110__function6__funcIN2ml11projections3$_2ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv
	.quad	__ZNSt3__110__function6__funcIN2ml11projections3$_2ENS_9allocatorIS4_EEFffEEclEOf
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections3$_2ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections3$_2ENS_9allocatorIS4_EEFffEE11target_typeEv

	.section	__TEXT,__const
__ZTSNSt3__110__function6__funcIN2ml11projections3$_2ENS_9allocatorIS4_EEFffEEE: ; @"_ZTSNSt3__110__function6__funcIN2ml11projections3$_2ENS_9allocatorIS4_EEFffEEE"
	.asciz	"NSt3__110__function6__funcIN2ml11projections3$_2ENS_9allocatorIS4_EEFffEEE"

	.section	__DATA,__const
	.p2align	3, 0x0                          ; @"_ZTINSt3__110__function6__funcIN2ml11projections3$_2ENS_9allocatorIS4_EEFffEEE"
__ZTINSt3__110__function6__funcIN2ml11projections3$_2ENS_9allocatorIS4_EEFffEEE:
	.quad	__ZTVN10__cxxabiv120__si_class_type_infoE+16
	.quad	__ZTSNSt3__110__function6__funcIN2ml11projections3$_2ENS_9allocatorIS4_EEFffEEE
	.quad	__ZTINSt3__110__function6__baseIFffEEE

	.section	__TEXT,__const
__ZTSN2ml11projections3$_2E:            ; @"_ZTSN2ml11projections3$_2E"
	.asciz	"N2ml11projections3$_2E"

	.section	__DATA,__const
	.p2align	3, 0x0                          ; @"_ZTIN2ml11projections3$_2E"
__ZTIN2ml11projections3$_2E:
	.quad	__ZTVN10__cxxabiv117__class_type_infoE+16
	.quad	__ZTSN2ml11projections3$_2E

	.p2align	3, 0x0                          ; @"_ZTVNSt3__110__function6__funcIN2ml11projections3$_3ENS_9allocatorIS4_EEFffEEE"
__ZTVNSt3__110__function6__funcIN2ml11projections3$_3ENS_9allocatorIS4_EEFffEEE:
	.quad	0
	.quad	__ZTINSt3__110__function6__funcIN2ml11projections3$_3ENS_9allocatorIS4_EEFffEEE
	.quad	__ZNSt3__110__function6__funcIN2ml11projections3$_3ENS_9allocatorIS4_EEFffEED1Ev
	.quad	__ZNSt3__110__function6__funcIN2ml11projections3$_3ENS_9allocatorIS4_EEFffEED0Ev
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections3$_3ENS_9allocatorIS4_EEFffEE7__cloneEv
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections3$_3ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE
	.quad	__ZNSt3__110__function6__funcIN2ml11projections3$_3ENS_9allocatorIS4_EEFffEE7destroyEv
	.quad	__ZNSt3__110__function6__funcIN2ml11projections3$_3ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv
	.quad	__ZNSt3__110__function6__funcIN2ml11projections3$_3ENS_9allocatorIS4_EEFffEEclEOf
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections3$_3ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections3$_3ENS_9allocatorIS4_EEFffEE11target_typeEv

	.section	__TEXT,__const
__ZTSNSt3__110__function6__funcIN2ml11projections3$_3ENS_9allocatorIS4_EEFffEEE: ; @"_ZTSNSt3__110__function6__funcIN2ml11projections3$_3ENS_9allocatorIS4_EEFffEEE"
	.asciz	"NSt3__110__function6__funcIN2ml11projections3$_3ENS_9allocatorIS4_EEFffEEE"

	.section	__DATA,__const
	.p2align	3, 0x0                          ; @"_ZTINSt3__110__function6__funcIN2ml11projections3$_3ENS_9allocatorIS4_EEFffEEE"
__ZTINSt3__110__function6__funcIN2ml11projections3$_3ENS_9allocatorIS4_EEFffEEE:
	.quad	__ZTVN10__cxxabiv120__si_class_type_infoE+16
	.quad	__ZTSNSt3__110__function6__funcIN2ml11projections3$_3ENS_9allocatorIS4_EEFffEEE
	.quad	__ZTINSt3__110__function6__baseIFffEEE

	.section	__TEXT,__const
__ZTSN2ml11projections3$_3E:            ; @"_ZTSN2ml11projections3$_3E"
	.asciz	"N2ml11projections3$_3E"

	.section	__DATA,__const
	.p2align	3, 0x0                          ; @"_ZTIN2ml11projections3$_3E"
__ZTIN2ml11projections3$_3E:
	.quad	__ZTVN10__cxxabiv117__class_type_infoE+16
	.quad	__ZTSN2ml11projections3$_3E

	.p2align	3, 0x0                          ; @"_ZTVNSt3__110__function6__funcIN2ml11projections3$_4ENS_9allocatorIS4_EEFffEEE"
__ZTVNSt3__110__function6__funcIN2ml11projections3$_4ENS_9allocatorIS4_EEFffEEE:
	.quad	0
	.quad	__ZTINSt3__110__function6__funcIN2ml11projections3$_4ENS_9allocatorIS4_EEFffEEE
	.quad	__ZNSt3__110__function6__funcIN2ml11projections3$_4ENS_9allocatorIS4_EEFffEED1Ev
	.quad	__ZNSt3__110__function6__funcIN2ml11projections3$_4ENS_9allocatorIS4_EEFffEED0Ev
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections3$_4ENS_9allocatorIS4_EEFffEE7__cloneEv
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections3$_4ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE
	.quad	__ZNSt3__110__function6__funcIN2ml11projections3$_4ENS_9allocatorIS4_EEFffEE7destroyEv
	.quad	__ZNSt3__110__function6__funcIN2ml11projections3$_4ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv
	.quad	__ZNSt3__110__function6__funcIN2ml11projections3$_4ENS_9allocatorIS4_EEFffEEclEOf
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections3$_4ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections3$_4ENS_9allocatorIS4_EEFffEE11target_typeEv

	.section	__TEXT,__const
__ZTSNSt3__110__function6__funcIN2ml11projections3$_4ENS_9allocatorIS4_EEFffEEE: ; @"_ZTSNSt3__110__function6__funcIN2ml11projections3$_4ENS_9allocatorIS4_EEFffEEE"
	.asciz	"NSt3__110__function6__funcIN2ml11projections3$_4ENS_9allocatorIS4_EEFffEEE"

	.section	__DATA,__const
	.p2align	3, 0x0                          ; @"_ZTINSt3__110__function6__funcIN2ml11projections3$_4ENS_9allocatorIS4_EEFffEEE"
__ZTINSt3__110__function6__funcIN2ml11projections3$_4ENS_9allocatorIS4_EEFffEEE:
	.quad	__ZTVN10__cxxabiv120__si_class_type_infoE+16
	.quad	__ZTSNSt3__110__function6__funcIN2ml11projections3$_4ENS_9allocatorIS4_EEFffEEE
	.quad	__ZTINSt3__110__function6__baseIFffEEE

	.section	__TEXT,__const
__ZTSN2ml11projections3$_4E:            ; @"_ZTSN2ml11projections3$_4E"
	.asciz	"N2ml11projections3$_4E"

	.section	__DATA,__const
	.p2align	3, 0x0                          ; @"_ZTIN2ml11projections3$_4E"
__ZTIN2ml11projections3$_4E:
	.quad	__ZTVN10__cxxabiv117__class_type_infoE+16
	.quad	__ZTSN2ml11projections3$_4E

	.p2align	3, 0x0                          ; @"_ZTVNSt3__110__function6__funcIN2ml11projections3$_5ENS_9allocatorIS4_EEFffEEE"
__ZTVNSt3__110__function6__funcIN2ml11projections3$_5ENS_9allocatorIS4_EEFffEEE:
	.quad	0
	.quad	__ZTINSt3__110__function6__funcIN2ml11projections3$_5ENS_9allocatorIS4_EEFffEEE
	.quad	__ZNSt3__110__function6__funcIN2ml11projections3$_5ENS_9allocatorIS4_EEFffEED1Ev
	.quad	__ZNSt3__110__function6__funcIN2ml11projections3$_5ENS_9allocatorIS4_EEFffEED0Ev
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections3$_5ENS_9allocatorIS4_EEFffEE7__cloneEv
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections3$_5ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE
	.quad	__ZNSt3__110__function6__funcIN2ml11projections3$_5ENS_9allocatorIS4_EEFffEE7destroyEv
	.quad	__ZNSt3__110__function6__funcIN2ml11projections3$_5ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv
	.quad	__ZNSt3__110__function6__funcIN2ml11projections3$_5ENS_9allocatorIS4_EEFffEEclEOf
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections3$_5ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections3$_5ENS_9allocatorIS4_EEFffEE11target_typeEv

	.section	__TEXT,__const
__ZTSNSt3__110__function6__funcIN2ml11projections3$_5ENS_9allocatorIS4_EEFffEEE: ; @"_ZTSNSt3__110__function6__funcIN2ml11projections3$_5ENS_9allocatorIS4_EEFffEEE"
	.asciz	"NSt3__110__function6__funcIN2ml11projections3$_5ENS_9allocatorIS4_EEFffEEE"

	.section	__DATA,__const
	.p2align	3, 0x0                          ; @"_ZTINSt3__110__function6__funcIN2ml11projections3$_5ENS_9allocatorIS4_EEFffEEE"
__ZTINSt3__110__function6__funcIN2ml11projections3$_5ENS_9allocatorIS4_EEFffEEE:
	.quad	__ZTVN10__cxxabiv120__si_class_type_infoE+16
	.quad	__ZTSNSt3__110__function6__funcIN2ml11projections3$_5ENS_9allocatorIS4_EEFffEEE
	.quad	__ZTINSt3__110__function6__baseIFffEEE

	.section	__TEXT,__const
__ZTSN2ml11projections3$_5E:            ; @"_ZTSN2ml11projections3$_5E"
	.asciz	"N2ml11projections3$_5E"

	.section	__DATA,__const
	.p2align	3, 0x0                          ; @"_ZTIN2ml11projections3$_5E"
__ZTIN2ml11projections3$_5E:
	.quad	__ZTVN10__cxxabiv117__class_type_infoE+16
	.quad	__ZTSN2ml11projections3$_5E

	.p2align	3, 0x0                          ; @"_ZTVNSt3__110__function6__funcIN2ml11projections3$_6ENS_9allocatorIS4_EEFffEEE"
__ZTVNSt3__110__function6__funcIN2ml11projections3$_6ENS_9allocatorIS4_EEFffEEE:
	.quad	0
	.quad	__ZTINSt3__110__function6__funcIN2ml11projections3$_6ENS_9allocatorIS4_EEFffEEE
	.quad	__ZNSt3__110__function6__funcIN2ml11projections3$_6ENS_9allocatorIS4_EEFffEED1Ev
	.quad	__ZNSt3__110__function6__funcIN2ml11projections3$_6ENS_9allocatorIS4_EEFffEED0Ev
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections3$_6ENS_9allocatorIS4_EEFffEE7__cloneEv
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections3$_6ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE
	.quad	__ZNSt3__110__function6__funcIN2ml11projections3$_6ENS_9allocatorIS4_EEFffEE7destroyEv
	.quad	__ZNSt3__110__function6__funcIN2ml11projections3$_6ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv
	.quad	__ZNSt3__110__function6__funcIN2ml11projections3$_6ENS_9allocatorIS4_EEFffEEclEOf
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections3$_6ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections3$_6ENS_9allocatorIS4_EEFffEE11target_typeEv

	.section	__TEXT,__const
__ZTSNSt3__110__function6__funcIN2ml11projections3$_6ENS_9allocatorIS4_EEFffEEE: ; @"_ZTSNSt3__110__function6__funcIN2ml11projections3$_6ENS_9allocatorIS4_EEFffEEE"
	.asciz	"NSt3__110__function6__funcIN2ml11projections3$_6ENS_9allocatorIS4_EEFffEEE"

	.section	__DATA,__const
	.p2align	3, 0x0                          ; @"_ZTINSt3__110__function6__funcIN2ml11projections3$_6ENS_9allocatorIS4_EEFffEEE"
__ZTINSt3__110__function6__funcIN2ml11projections3$_6ENS_9allocatorIS4_EEFffEEE:
	.quad	__ZTVN10__cxxabiv120__si_class_type_infoE+16
	.quad	__ZTSNSt3__110__function6__funcIN2ml11projections3$_6ENS_9allocatorIS4_EEFffEEE
	.quad	__ZTINSt3__110__function6__baseIFffEEE

	.section	__TEXT,__const
__ZTSN2ml11projections3$_6E:            ; @"_ZTSN2ml11projections3$_6E"
	.asciz	"N2ml11projections3$_6E"

	.section	__DATA,__const
	.p2align	3, 0x0                          ; @"_ZTIN2ml11projections3$_6E"
__ZTIN2ml11projections3$_6E:
	.quad	__ZTVN10__cxxabiv117__class_type_infoE+16
	.quad	__ZTSN2ml11projections3$_6E

	.p2align	3, 0x0                          ; @"_ZTVNSt3__110__function6__funcIN2ml11projections3$_7ENS_9allocatorIS4_EEFffEEE"
__ZTVNSt3__110__function6__funcIN2ml11projections3$_7ENS_9allocatorIS4_EEFffEEE:
	.quad	0
	.quad	__ZTINSt3__110__function6__funcIN2ml11projections3$_7ENS_9allocatorIS4_EEFffEEE
	.quad	__ZNSt3__110__function6__funcIN2ml11projections3$_7ENS_9allocatorIS4_EEFffEED1Ev
	.quad	__ZNSt3__110__function6__funcIN2ml11projections3$_7ENS_9allocatorIS4_EEFffEED0Ev
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections3$_7ENS_9allocatorIS4_EEFffEE7__cloneEv
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections3$_7ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE
	.quad	__ZNSt3__110__function6__funcIN2ml11projections3$_7ENS_9allocatorIS4_EEFffEE7destroyEv
	.quad	__ZNSt3__110__function6__funcIN2ml11projections3$_7ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv
	.quad	__ZNSt3__110__function6__funcIN2ml11projections3$_7ENS_9allocatorIS4_EEFffEEclEOf
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections3$_7ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections3$_7ENS_9allocatorIS4_EEFffEE11target_typeEv

	.section	__TEXT,__const
__ZTSNSt3__110__function6__funcIN2ml11projections3$_7ENS_9allocatorIS4_EEFffEEE: ; @"_ZTSNSt3__110__function6__funcIN2ml11projections3$_7ENS_9allocatorIS4_EEFffEEE"
	.asciz	"NSt3__110__function6__funcIN2ml11projections3$_7ENS_9allocatorIS4_EEFffEEE"

	.section	__DATA,__const
	.p2align	3, 0x0                          ; @"_ZTINSt3__110__function6__funcIN2ml11projections3$_7ENS_9allocatorIS4_EEFffEEE"
__ZTINSt3__110__function6__funcIN2ml11projections3$_7ENS_9allocatorIS4_EEFffEEE:
	.quad	__ZTVN10__cxxabiv120__si_class_type_infoE+16
	.quad	__ZTSNSt3__110__function6__funcIN2ml11projections3$_7ENS_9allocatorIS4_EEFffEEE
	.quad	__ZTINSt3__110__function6__baseIFffEEE

	.section	__TEXT,__const
__ZTSN2ml11projections3$_7E:            ; @"_ZTSN2ml11projections3$_7E"
	.asciz	"N2ml11projections3$_7E"

	.section	__DATA,__const
	.p2align	3, 0x0                          ; @"_ZTIN2ml11projections3$_7E"
__ZTIN2ml11projections3$_7E:
	.quad	__ZTVN10__cxxabiv117__class_type_infoE+16
	.quad	__ZTSN2ml11projections3$_7E

	.p2align	3, 0x0                          ; @"_ZTVNSt3__110__function6__funcIN2ml11projections3$_8ENS_9allocatorIS4_EEFffEEE"
__ZTVNSt3__110__function6__funcIN2ml11projections3$_8ENS_9allocatorIS4_EEFffEEE:
	.quad	0
	.quad	__ZTINSt3__110__function6__funcIN2ml11projections3$_8ENS_9allocatorIS4_EEFffEEE
	.quad	__ZNSt3__110__function6__funcIN2ml11projections3$_8ENS_9allocatorIS4_EEFffEED1Ev
	.quad	__ZNSt3__110__function6__funcIN2ml11projections3$_8ENS_9allocatorIS4_EEFffEED0Ev
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections3$_8ENS_9allocatorIS4_EEFffEE7__cloneEv
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections3$_8ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE
	.quad	__ZNSt3__110__function6__funcIN2ml11projections3$_8ENS_9allocatorIS4_EEFffEE7destroyEv
	.quad	__ZNSt3__110__function6__funcIN2ml11projections3$_8ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv
	.quad	__ZNSt3__110__function6__funcIN2ml11projections3$_8ENS_9allocatorIS4_EEFffEEclEOf
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections3$_8ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections3$_8ENS_9allocatorIS4_EEFffEE11target_typeEv

	.section	__TEXT,__const
__ZTSNSt3__110__function6__funcIN2ml11projections3$_8ENS_9allocatorIS4_EEFffEEE: ; @"_ZTSNSt3__110__function6__funcIN2ml11projections3$_8ENS_9allocatorIS4_EEFffEEE"
	.asciz	"NSt3__110__function6__funcIN2ml11projections3$_8ENS_9allocatorIS4_EEFffEEE"

	.section	__DATA,__const
	.p2align	3, 0x0                          ; @"_ZTINSt3__110__function6__funcIN2ml11projections3$_8ENS_9allocatorIS4_EEFffEEE"
__ZTINSt3__110__function6__funcIN2ml11projections3$_8ENS_9allocatorIS4_EEFffEEE:
	.quad	__ZTVN10__cxxabiv120__si_class_type_infoE+16
	.quad	__ZTSNSt3__110__function6__funcIN2ml11projections3$_8ENS_9allocatorIS4_EEFffEEE
	.quad	__ZTINSt3__110__function6__baseIFffEEE

	.section	__TEXT,__const
__ZTSN2ml11projections3$_8E:            ; @"_ZTSN2ml11projections3$_8E"
	.asciz	"N2ml11projections3$_8E"

	.section	__DATA,__const
	.p2align	3, 0x0                          ; @"_ZTIN2ml11projections3$_8E"
__ZTIN2ml11projections3$_8E:
	.quad	__ZTVN10__cxxabiv117__class_type_infoE+16
	.quad	__ZTSN2ml11projections3$_8E

	.p2align	3, 0x0                          ; @"_ZTVNSt3__110__function6__funcIN2ml11projections3$_9ENS_9allocatorIS4_EEFffEEE"
__ZTVNSt3__110__function6__funcIN2ml11projections3$_9ENS_9allocatorIS4_EEFffEEE:
	.quad	0
	.quad	__ZTINSt3__110__function6__funcIN2ml11projections3$_9ENS_9allocatorIS4_EEFffEEE
	.quad	__ZNSt3__110__function6__funcIN2ml11projections3$_9ENS_9allocatorIS4_EEFffEED1Ev
	.quad	__ZNSt3__110__function6__funcIN2ml11projections3$_9ENS_9allocatorIS4_EEFffEED0Ev
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections3$_9ENS_9allocatorIS4_EEFffEE7__cloneEv
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections3$_9ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE
	.quad	__ZNSt3__110__function6__funcIN2ml11projections3$_9ENS_9allocatorIS4_EEFffEE7destroyEv
	.quad	__ZNSt3__110__function6__funcIN2ml11projections3$_9ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv
	.quad	__ZNSt3__110__function6__funcIN2ml11projections3$_9ENS_9allocatorIS4_EEFffEEclEOf
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections3$_9ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections3$_9ENS_9allocatorIS4_EEFffEE11target_typeEv

	.section	__TEXT,__const
__ZTSNSt3__110__function6__funcIN2ml11projections3$_9ENS_9allocatorIS4_EEFffEEE: ; @"_ZTSNSt3__110__function6__funcIN2ml11projections3$_9ENS_9allocatorIS4_EEFffEEE"
	.asciz	"NSt3__110__function6__funcIN2ml11projections3$_9ENS_9allocatorIS4_EEFffEEE"

	.section	__DATA,__const
	.p2align	3, 0x0                          ; @"_ZTINSt3__110__function6__funcIN2ml11projections3$_9ENS_9allocatorIS4_EEFffEEE"
__ZTINSt3__110__function6__funcIN2ml11projections3$_9ENS_9allocatorIS4_EEFffEEE:
	.quad	__ZTVN10__cxxabiv120__si_class_type_infoE+16
	.quad	__ZTSNSt3__110__function6__funcIN2ml11projections3$_9ENS_9allocatorIS4_EEFffEEE
	.quad	__ZTINSt3__110__function6__baseIFffEEE

	.section	__TEXT,__const
__ZTSN2ml11projections3$_9E:            ; @"_ZTSN2ml11projections3$_9E"
	.asciz	"N2ml11projections3$_9E"

	.section	__DATA,__const
	.p2align	3, 0x0                          ; @"_ZTIN2ml11projections3$_9E"
__ZTIN2ml11projections3$_9E:
	.quad	__ZTVN10__cxxabiv117__class_type_infoE+16
	.quad	__ZTSN2ml11projections3$_9E

	.p2align	3, 0x0                          ; @"_ZTVNSt3__110__function6__funcIN2ml11projections4$_10ENS_9allocatorIS4_EEFffEEE"
__ZTVNSt3__110__function6__funcIN2ml11projections4$_10ENS_9allocatorIS4_EEFffEEE:
	.quad	0
	.quad	__ZTINSt3__110__function6__funcIN2ml11projections4$_10ENS_9allocatorIS4_EEFffEEE
	.quad	__ZNSt3__110__function6__funcIN2ml11projections4$_10ENS_9allocatorIS4_EEFffEED1Ev
	.quad	__ZNSt3__110__function6__funcIN2ml11projections4$_10ENS_9allocatorIS4_EEFffEED0Ev
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections4$_10ENS_9allocatorIS4_EEFffEE7__cloneEv
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections4$_10ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE
	.quad	__ZNSt3__110__function6__funcIN2ml11projections4$_10ENS_9allocatorIS4_EEFffEE7destroyEv
	.quad	__ZNSt3__110__function6__funcIN2ml11projections4$_10ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv
	.quad	__ZNSt3__110__function6__funcIN2ml11projections4$_10ENS_9allocatorIS4_EEFffEEclEOf
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections4$_10ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections4$_10ENS_9allocatorIS4_EEFffEE11target_typeEv

	.section	__TEXT,__const
__ZTSNSt3__110__function6__funcIN2ml11projections4$_10ENS_9allocatorIS4_EEFffEEE: ; @"_ZTSNSt3__110__function6__funcIN2ml11projections4$_10ENS_9allocatorIS4_EEFffEEE"
	.asciz	"NSt3__110__function6__funcIN2ml11projections4$_10ENS_9allocatorIS4_EEFffEEE"

	.section	__DATA,__const
	.p2align	3, 0x0                          ; @"_ZTINSt3__110__function6__funcIN2ml11projections4$_10ENS_9allocatorIS4_EEFffEEE"
__ZTINSt3__110__function6__funcIN2ml11projections4$_10ENS_9allocatorIS4_EEFffEEE:
	.quad	__ZTVN10__cxxabiv120__si_class_type_infoE+16
	.quad	__ZTSNSt3__110__function6__funcIN2ml11projections4$_10ENS_9allocatorIS4_EEFffEEE
	.quad	__ZTINSt3__110__function6__baseIFffEEE

	.private_extern	__ZTSNSt3__117bad_function_callE ; @_ZTSNSt3__117bad_function_callE
	.section	__TEXT,__const
	.globl	__ZTSNSt3__117bad_function_callE
	.weak_definition	__ZTSNSt3__117bad_function_callE
__ZTSNSt3__117bad_function_callE:
	.asciz	"NSt3__117bad_function_callE"

	.private_extern	__ZTINSt3__117bad_function_callE ; @_ZTINSt3__117bad_function_callE
	.section	__DATA,__const
	.globl	__ZTINSt3__117bad_function_callE
	.weak_definition	__ZTINSt3__117bad_function_callE
	.p2align	3, 0x0
__ZTINSt3__117bad_function_callE:
	.quad	__ZTVN10__cxxabiv120__si_class_type_infoE+16
	.quad	__ZTSNSt3__117bad_function_callE-9223372036854775808
	.quad	__ZTISt9exception

	.globl	__ZTVNSt3__117bad_function_callE ; @_ZTVNSt3__117bad_function_callE
	.weak_def_can_be_hidden	__ZTVNSt3__117bad_function_callE
	.p2align	3, 0x0
__ZTVNSt3__117bad_function_callE:
	.quad	0
	.quad	__ZTINSt3__117bad_function_callE
	.quad	__ZNSt3__117bad_function_callD1Ev
	.quad	__ZNSt3__117bad_function_callD0Ev
	.quad	__ZNKSt9exception4whatEv

	.section	__TEXT,__const
__ZTSN2ml11projections4$_10E:           ; @"_ZTSN2ml11projections4$_10E"
	.asciz	"N2ml11projections4$_10E"

	.section	__DATA,__const
	.p2align	3, 0x0                          ; @"_ZTIN2ml11projections4$_10E"
__ZTIN2ml11projections4$_10E:
	.quad	__ZTVN10__cxxabiv117__class_type_infoE+16
	.quad	__ZTSN2ml11projections4$_10E

	.p2align	3, 0x0                          ; @"_ZTVNSt3__110__function6__funcIN2ml11projections4$_11ENS_9allocatorIS4_EEFffEEE"
__ZTVNSt3__110__function6__funcIN2ml11projections4$_11ENS_9allocatorIS4_EEFffEEE:
	.quad	0
	.quad	__ZTINSt3__110__function6__funcIN2ml11projections4$_11ENS_9allocatorIS4_EEFffEEE
	.quad	__ZNSt3__110__function6__funcIN2ml11projections4$_11ENS_9allocatorIS4_EEFffEED1Ev
	.quad	__ZNSt3__110__function6__funcIN2ml11projections4$_11ENS_9allocatorIS4_EEFffEED0Ev
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections4$_11ENS_9allocatorIS4_EEFffEE7__cloneEv
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections4$_11ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE
	.quad	__ZNSt3__110__function6__funcIN2ml11projections4$_11ENS_9allocatorIS4_EEFffEE7destroyEv
	.quad	__ZNSt3__110__function6__funcIN2ml11projections4$_11ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv
	.quad	__ZNSt3__110__function6__funcIN2ml11projections4$_11ENS_9allocatorIS4_EEFffEEclEOf
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections4$_11ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections4$_11ENS_9allocatorIS4_EEFffEE11target_typeEv

	.section	__TEXT,__const
__ZTSNSt3__110__function6__funcIN2ml11projections4$_11ENS_9allocatorIS4_EEFffEEE: ; @"_ZTSNSt3__110__function6__funcIN2ml11projections4$_11ENS_9allocatorIS4_EEFffEEE"
	.asciz	"NSt3__110__function6__funcIN2ml11projections4$_11ENS_9allocatorIS4_EEFffEEE"

	.section	__DATA,__const
	.p2align	3, 0x0                          ; @"_ZTINSt3__110__function6__funcIN2ml11projections4$_11ENS_9allocatorIS4_EEFffEEE"
__ZTINSt3__110__function6__funcIN2ml11projections4$_11ENS_9allocatorIS4_EEFffEEE:
	.quad	__ZTVN10__cxxabiv120__si_class_type_infoE+16
	.quad	__ZTSNSt3__110__function6__funcIN2ml11projections4$_11ENS_9allocatorIS4_EEFffEEE
	.quad	__ZTINSt3__110__function6__baseIFffEEE

	.section	__TEXT,__const
__ZTSN2ml11projections4$_11E:           ; @"_ZTSN2ml11projections4$_11E"
	.asciz	"N2ml11projections4$_11E"

	.section	__DATA,__const
	.p2align	3, 0x0                          ; @"_ZTIN2ml11projections4$_11E"
__ZTIN2ml11projections4$_11E:
	.quad	__ZTVN10__cxxabiv117__class_type_infoE+16
	.quad	__ZTSN2ml11projections4$_11E

	.p2align	3, 0x0                          ; @"_ZTVNSt3__110__function6__funcIN2ml11projections4$_12ENS_9allocatorIS4_EEFffEEE"
__ZTVNSt3__110__function6__funcIN2ml11projections4$_12ENS_9allocatorIS4_EEFffEEE:
	.quad	0
	.quad	__ZTINSt3__110__function6__funcIN2ml11projections4$_12ENS_9allocatorIS4_EEFffEEE
	.quad	__ZNSt3__110__function6__funcIN2ml11projections4$_12ENS_9allocatorIS4_EEFffEED1Ev
	.quad	__ZNSt3__110__function6__funcIN2ml11projections4$_12ENS_9allocatorIS4_EEFffEED0Ev
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections4$_12ENS_9allocatorIS4_EEFffEE7__cloneEv
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections4$_12ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE
	.quad	__ZNSt3__110__function6__funcIN2ml11projections4$_12ENS_9allocatorIS4_EEFffEE7destroyEv
	.quad	__ZNSt3__110__function6__funcIN2ml11projections4$_12ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv
	.quad	__ZNSt3__110__function6__funcIN2ml11projections4$_12ENS_9allocatorIS4_EEFffEEclEOf
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections4$_12ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections4$_12ENS_9allocatorIS4_EEFffEE11target_typeEv

	.section	__TEXT,__const
__ZTSNSt3__110__function6__funcIN2ml11projections4$_12ENS_9allocatorIS4_EEFffEEE: ; @"_ZTSNSt3__110__function6__funcIN2ml11projections4$_12ENS_9allocatorIS4_EEFffEEE"
	.asciz	"NSt3__110__function6__funcIN2ml11projections4$_12ENS_9allocatorIS4_EEFffEEE"

	.section	__DATA,__const
	.p2align	3, 0x0                          ; @"_ZTINSt3__110__function6__funcIN2ml11projections4$_12ENS_9allocatorIS4_EEFffEEE"
__ZTINSt3__110__function6__funcIN2ml11projections4$_12ENS_9allocatorIS4_EEFffEEE:
	.quad	__ZTVN10__cxxabiv120__si_class_type_infoE+16
	.quad	__ZTSNSt3__110__function6__funcIN2ml11projections4$_12ENS_9allocatorIS4_EEFffEEE
	.quad	__ZTINSt3__110__function6__baseIFffEEE

	.section	__TEXT,__const
__ZTSN2ml11projections4$_12E:           ; @"_ZTSN2ml11projections4$_12E"
	.asciz	"N2ml11projections4$_12E"

	.section	__DATA,__const
	.p2align	3, 0x0                          ; @"_ZTIN2ml11projections4$_12E"
__ZTIN2ml11projections4$_12E:
	.quad	__ZTVN10__cxxabiv117__class_type_infoE+16
	.quad	__ZTSN2ml11projections4$_12E

	.p2align	3, 0x0                          ; @"_ZTVNSt3__110__function6__funcIN2ml11projections4$_13ENS_9allocatorIS4_EEFffEEE"
__ZTVNSt3__110__function6__funcIN2ml11projections4$_13ENS_9allocatorIS4_EEFffEEE:
	.quad	0
	.quad	__ZTINSt3__110__function6__funcIN2ml11projections4$_13ENS_9allocatorIS4_EEFffEEE
	.quad	__ZNSt3__110__function6__funcIN2ml11projections4$_13ENS_9allocatorIS4_EEFffEED1Ev
	.quad	__ZNSt3__110__function6__funcIN2ml11projections4$_13ENS_9allocatorIS4_EEFffEED0Ev
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections4$_13ENS_9allocatorIS4_EEFffEE7__cloneEv
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections4$_13ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE
	.quad	__ZNSt3__110__function6__funcIN2ml11projections4$_13ENS_9allocatorIS4_EEFffEE7destroyEv
	.quad	__ZNSt3__110__function6__funcIN2ml11projections4$_13ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv
	.quad	__ZNSt3__110__function6__funcIN2ml11projections4$_13ENS_9allocatorIS4_EEFffEEclEOf
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections4$_13ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections4$_13ENS_9allocatorIS4_EEFffEE11target_typeEv

	.section	__TEXT,__const
__ZTSNSt3__110__function6__funcIN2ml11projections4$_13ENS_9allocatorIS4_EEFffEEE: ; @"_ZTSNSt3__110__function6__funcIN2ml11projections4$_13ENS_9allocatorIS4_EEFffEEE"
	.asciz	"NSt3__110__function6__funcIN2ml11projections4$_13ENS_9allocatorIS4_EEFffEEE"

	.section	__DATA,__const
	.p2align	3, 0x0                          ; @"_ZTINSt3__110__function6__funcIN2ml11projections4$_13ENS_9allocatorIS4_EEFffEEE"
__ZTINSt3__110__function6__funcIN2ml11projections4$_13ENS_9allocatorIS4_EEFffEEE:
	.quad	__ZTVN10__cxxabiv120__si_class_type_infoE+16
	.quad	__ZTSNSt3__110__function6__funcIN2ml11projections4$_13ENS_9allocatorIS4_EEFffEEE
	.quad	__ZTINSt3__110__function6__baseIFffEEE

	.section	__TEXT,__const
__ZTSN2ml11projections4$_13E:           ; @"_ZTSN2ml11projections4$_13E"
	.asciz	"N2ml11projections4$_13E"

	.section	__DATA,__const
	.p2align	3, 0x0                          ; @"_ZTIN2ml11projections4$_13E"
__ZTIN2ml11projections4$_13E:
	.quad	__ZTVN10__cxxabiv117__class_type_infoE+16
	.quad	__ZTSN2ml11projections4$_13E

	.p2align	3, 0x0                          ; @"_ZTVNSt3__110__function6__funcIN2ml11projections4$_14ENS_9allocatorIS4_EEFffEEE"
__ZTVNSt3__110__function6__funcIN2ml11projections4$_14ENS_9allocatorIS4_EEFffEEE:
	.quad	0
	.quad	__ZTINSt3__110__function6__funcIN2ml11projections4$_14ENS_9allocatorIS4_EEFffEEE
	.quad	__ZNSt3__110__function6__funcIN2ml11projections4$_14ENS_9allocatorIS4_EEFffEED1Ev
	.quad	__ZNSt3__110__function6__funcIN2ml11projections4$_14ENS_9allocatorIS4_EEFffEED0Ev
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections4$_14ENS_9allocatorIS4_EEFffEE7__cloneEv
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections4$_14ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE
	.quad	__ZNSt3__110__function6__funcIN2ml11projections4$_14ENS_9allocatorIS4_EEFffEE7destroyEv
	.quad	__ZNSt3__110__function6__funcIN2ml11projections4$_14ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv
	.quad	__ZNSt3__110__function6__funcIN2ml11projections4$_14ENS_9allocatorIS4_EEFffEEclEOf
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections4$_14ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections4$_14ENS_9allocatorIS4_EEFffEE11target_typeEv

	.section	__TEXT,__const
__ZTSNSt3__110__function6__funcIN2ml11projections4$_14ENS_9allocatorIS4_EEFffEEE: ; @"_ZTSNSt3__110__function6__funcIN2ml11projections4$_14ENS_9allocatorIS4_EEFffEEE"
	.asciz	"NSt3__110__function6__funcIN2ml11projections4$_14ENS_9allocatorIS4_EEFffEEE"

	.section	__DATA,__const
	.p2align	3, 0x0                          ; @"_ZTINSt3__110__function6__funcIN2ml11projections4$_14ENS_9allocatorIS4_EEFffEEE"
__ZTINSt3__110__function6__funcIN2ml11projections4$_14ENS_9allocatorIS4_EEFffEEE:
	.quad	__ZTVN10__cxxabiv120__si_class_type_infoE+16
	.quad	__ZTSNSt3__110__function6__funcIN2ml11projections4$_14ENS_9allocatorIS4_EEFffEEE
	.quad	__ZTINSt3__110__function6__baseIFffEEE

	.section	__TEXT,__const
__ZTSN2ml11projections4$_14E:           ; @"_ZTSN2ml11projections4$_14E"
	.asciz	"N2ml11projections4$_14E"

	.section	__DATA,__const
	.p2align	3, 0x0                          ; @"_ZTIN2ml11projections4$_14E"
__ZTIN2ml11projections4$_14E:
	.quad	__ZTVN10__cxxabiv117__class_type_infoE+16
	.quad	__ZTSN2ml11projections4$_14E

	.p2align	3, 0x0                          ; @"_ZTVNSt3__110__function6__funcIN2ml11projections4$_15ENS_9allocatorIS4_EEFffEEE"
__ZTVNSt3__110__function6__funcIN2ml11projections4$_15ENS_9allocatorIS4_EEFffEEE:
	.quad	0
	.quad	__ZTINSt3__110__function6__funcIN2ml11projections4$_15ENS_9allocatorIS4_EEFffEEE
	.quad	__ZNSt3__110__function6__funcIN2ml11projections4$_15ENS_9allocatorIS4_EEFffEED1Ev
	.quad	__ZNSt3__110__function6__funcIN2ml11projections4$_15ENS_9allocatorIS4_EEFffEED0Ev
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections4$_15ENS_9allocatorIS4_EEFffEE7__cloneEv
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections4$_15ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE
	.quad	__ZNSt3__110__function6__funcIN2ml11projections4$_15ENS_9allocatorIS4_EEFffEE7destroyEv
	.quad	__ZNSt3__110__function6__funcIN2ml11projections4$_15ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv
	.quad	__ZNSt3__110__function6__funcIN2ml11projections4$_15ENS_9allocatorIS4_EEFffEEclEOf
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections4$_15ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections4$_15ENS_9allocatorIS4_EEFffEE11target_typeEv

	.section	__TEXT,__const
__ZTSNSt3__110__function6__funcIN2ml11projections4$_15ENS_9allocatorIS4_EEFffEEE: ; @"_ZTSNSt3__110__function6__funcIN2ml11projections4$_15ENS_9allocatorIS4_EEFffEEE"
	.asciz	"NSt3__110__function6__funcIN2ml11projections4$_15ENS_9allocatorIS4_EEFffEEE"

	.section	__DATA,__const
	.p2align	3, 0x0                          ; @"_ZTINSt3__110__function6__funcIN2ml11projections4$_15ENS_9allocatorIS4_EEFffEEE"
__ZTINSt3__110__function6__funcIN2ml11projections4$_15ENS_9allocatorIS4_EEFffEEE:
	.quad	__ZTVN10__cxxabiv120__si_class_type_infoE+16
	.quad	__ZTSNSt3__110__function6__funcIN2ml11projections4$_15ENS_9allocatorIS4_EEFffEEE
	.quad	__ZTINSt3__110__function6__baseIFffEEE

	.section	__TEXT,__const
__ZTSN2ml11projections4$_15E:           ; @"_ZTSN2ml11projections4$_15E"
	.asciz	"N2ml11projections4$_15E"

	.section	__DATA,__const
	.p2align	3, 0x0                          ; @"_ZTIN2ml11projections4$_15E"
__ZTIN2ml11projections4$_15E:
	.quad	__ZTVN10__cxxabiv117__class_type_infoE+16
	.quad	__ZTSN2ml11projections4$_15E

	.p2align	3, 0x0                          ; @"_ZTVNSt3__110__function6__funcIN2ml11projections4$_16ENS_9allocatorIS4_EEFffEEE"
__ZTVNSt3__110__function6__funcIN2ml11projections4$_16ENS_9allocatorIS4_EEFffEEE:
	.quad	0
	.quad	__ZTINSt3__110__function6__funcIN2ml11projections4$_16ENS_9allocatorIS4_EEFffEEE
	.quad	__ZNSt3__110__function6__funcIN2ml11projections4$_16ENS_9allocatorIS4_EEFffEED1Ev
	.quad	__ZNSt3__110__function6__funcIN2ml11projections4$_16ENS_9allocatorIS4_EEFffEED0Ev
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections4$_16ENS_9allocatorIS4_EEFffEE7__cloneEv
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections4$_16ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE
	.quad	__ZNSt3__110__function6__funcIN2ml11projections4$_16ENS_9allocatorIS4_EEFffEE7destroyEv
	.quad	__ZNSt3__110__function6__funcIN2ml11projections4$_16ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv
	.quad	__ZNSt3__110__function6__funcIN2ml11projections4$_16ENS_9allocatorIS4_EEFffEEclEOf
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections4$_16ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections4$_16ENS_9allocatorIS4_EEFffEE11target_typeEv

	.section	__TEXT,__const
__ZTSNSt3__110__function6__funcIN2ml11projections4$_16ENS_9allocatorIS4_EEFffEEE: ; @"_ZTSNSt3__110__function6__funcIN2ml11projections4$_16ENS_9allocatorIS4_EEFffEEE"
	.asciz	"NSt3__110__function6__funcIN2ml11projections4$_16ENS_9allocatorIS4_EEFffEEE"

	.section	__DATA,__const
	.p2align	3, 0x0                          ; @"_ZTINSt3__110__function6__funcIN2ml11projections4$_16ENS_9allocatorIS4_EEFffEEE"
__ZTINSt3__110__function6__funcIN2ml11projections4$_16ENS_9allocatorIS4_EEFffEEE:
	.quad	__ZTVN10__cxxabiv120__si_class_type_infoE+16
	.quad	__ZTSNSt3__110__function6__funcIN2ml11projections4$_16ENS_9allocatorIS4_EEFffEEE
	.quad	__ZTINSt3__110__function6__baseIFffEEE

	.section	__TEXT,__const
__ZTSN2ml11projections4$_16E:           ; @"_ZTSN2ml11projections4$_16E"
	.asciz	"N2ml11projections4$_16E"

	.section	__DATA,__const
	.p2align	3, 0x0                          ; @"_ZTIN2ml11projections4$_16E"
__ZTIN2ml11projections4$_16E:
	.quad	__ZTVN10__cxxabiv117__class_type_infoE+16
	.quad	__ZTSN2ml11projections4$_16E

	.p2align	3, 0x0                          ; @"_ZTVNSt3__110__function6__funcIN2ml11projections4$_17ENS_9allocatorIS4_EEFffEEE"
__ZTVNSt3__110__function6__funcIN2ml11projections4$_17ENS_9allocatorIS4_EEFffEEE:
	.quad	0
	.quad	__ZTINSt3__110__function6__funcIN2ml11projections4$_17ENS_9allocatorIS4_EEFffEEE
	.quad	__ZNSt3__110__function6__funcIN2ml11projections4$_17ENS_9allocatorIS4_EEFffEED1Ev
	.quad	__ZNSt3__110__function6__funcIN2ml11projections4$_17ENS_9allocatorIS4_EEFffEED0Ev
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections4$_17ENS_9allocatorIS4_EEFffEE7__cloneEv
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections4$_17ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE
	.quad	__ZNSt3__110__function6__funcIN2ml11projections4$_17ENS_9allocatorIS4_EEFffEE7destroyEv
	.quad	__ZNSt3__110__function6__funcIN2ml11projections4$_17ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv
	.quad	__ZNSt3__110__function6__funcIN2ml11projections4$_17ENS_9allocatorIS4_EEFffEEclEOf
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections4$_17ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections4$_17ENS_9allocatorIS4_EEFffEE11target_typeEv

	.section	__TEXT,__const
__ZTSNSt3__110__function6__funcIN2ml11projections4$_17ENS_9allocatorIS4_EEFffEEE: ; @"_ZTSNSt3__110__function6__funcIN2ml11projections4$_17ENS_9allocatorIS4_EEFffEEE"
	.asciz	"NSt3__110__function6__funcIN2ml11projections4$_17ENS_9allocatorIS4_EEFffEEE"

	.section	__DATA,__const
	.p2align	3, 0x0                          ; @"_ZTINSt3__110__function6__funcIN2ml11projections4$_17ENS_9allocatorIS4_EEFffEEE"
__ZTINSt3__110__function6__funcIN2ml11projections4$_17ENS_9allocatorIS4_EEFffEEE:
	.quad	__ZTVN10__cxxabiv120__si_class_type_infoE+16
	.quad	__ZTSNSt3__110__function6__funcIN2ml11projections4$_17ENS_9allocatorIS4_EEFffEEE
	.quad	__ZTINSt3__110__function6__baseIFffEEE

	.section	__TEXT,__const
__ZTSN2ml11projections4$_17E:           ; @"_ZTSN2ml11projections4$_17E"
	.asciz	"N2ml11projections4$_17E"

	.section	__DATA,__const
	.p2align	3, 0x0                          ; @"_ZTIN2ml11projections4$_17E"
__ZTIN2ml11projections4$_17E:
	.quad	__ZTVN10__cxxabiv117__class_type_infoE+16
	.quad	__ZTSN2ml11projections4$_17E

	.p2align	3, 0x0                          ; @"_ZTVNSt3__110__function6__funcIN2ml11projections4$_18ENS_9allocatorIS4_EEFffEEE"
__ZTVNSt3__110__function6__funcIN2ml11projections4$_18ENS_9allocatorIS4_EEFffEEE:
	.quad	0
	.quad	__ZTINSt3__110__function6__funcIN2ml11projections4$_18ENS_9allocatorIS4_EEFffEEE
	.quad	__ZNSt3__110__function6__funcIN2ml11projections4$_18ENS_9allocatorIS4_EEFffEED1Ev
	.quad	__ZNSt3__110__function6__funcIN2ml11projections4$_18ENS_9allocatorIS4_EEFffEED0Ev
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections4$_18ENS_9allocatorIS4_EEFffEE7__cloneEv
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections4$_18ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE
	.quad	__ZNSt3__110__function6__funcIN2ml11projections4$_18ENS_9allocatorIS4_EEFffEE7destroyEv
	.quad	__ZNSt3__110__function6__funcIN2ml11projections4$_18ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv
	.quad	__ZNSt3__110__function6__funcIN2ml11projections4$_18ENS_9allocatorIS4_EEFffEEclEOf
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections4$_18ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections4$_18ENS_9allocatorIS4_EEFffEE11target_typeEv

	.section	__TEXT,__const
__ZTSNSt3__110__function6__funcIN2ml11projections4$_18ENS_9allocatorIS4_EEFffEEE: ; @"_ZTSNSt3__110__function6__funcIN2ml11projections4$_18ENS_9allocatorIS4_EEFffEEE"
	.asciz	"NSt3__110__function6__funcIN2ml11projections4$_18ENS_9allocatorIS4_EEFffEEE"

	.section	__DATA,__const
	.p2align	3, 0x0                          ; @"_ZTINSt3__110__function6__funcIN2ml11projections4$_18ENS_9allocatorIS4_EEFffEEE"
__ZTINSt3__110__function6__funcIN2ml11projections4$_18ENS_9allocatorIS4_EEFffEEE:
	.quad	__ZTVN10__cxxabiv120__si_class_type_infoE+16
	.quad	__ZTSNSt3__110__function6__funcIN2ml11projections4$_18ENS_9allocatorIS4_EEFffEEE
	.quad	__ZTINSt3__110__function6__baseIFffEEE

	.section	__TEXT,__const
__ZTSN2ml11projections4$_18E:           ; @"_ZTSN2ml11projections4$_18E"
	.asciz	"N2ml11projections4$_18E"

	.section	__DATA,__const
	.p2align	3, 0x0                          ; @"_ZTIN2ml11projections4$_18E"
__ZTIN2ml11projections4$_18E:
	.quad	__ZTVN10__cxxabiv117__class_type_infoE+16
	.quad	__ZTSN2ml11projections4$_18E

	.p2align	3, 0x0                          ; @"_ZTVNSt3__110__function6__funcIN2ml11projections4$_19ENS_9allocatorIS4_EEFffEEE"
__ZTVNSt3__110__function6__funcIN2ml11projections4$_19ENS_9allocatorIS4_EEFffEEE:
	.quad	0
	.quad	__ZTINSt3__110__function6__funcIN2ml11projections4$_19ENS_9allocatorIS4_EEFffEEE
	.quad	__ZNSt3__110__function6__funcIN2ml11projections4$_19ENS_9allocatorIS4_EEFffEED1Ev
	.quad	__ZNSt3__110__function6__funcIN2ml11projections4$_19ENS_9allocatorIS4_EEFffEED0Ev
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections4$_19ENS_9allocatorIS4_EEFffEE7__cloneEv
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections4$_19ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE
	.quad	__ZNSt3__110__function6__funcIN2ml11projections4$_19ENS_9allocatorIS4_EEFffEE7destroyEv
	.quad	__ZNSt3__110__function6__funcIN2ml11projections4$_19ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv
	.quad	__ZNSt3__110__function6__funcIN2ml11projections4$_19ENS_9allocatorIS4_EEFffEEclEOf
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections4$_19ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info
	.quad	__ZNKSt3__110__function6__funcIN2ml11projections4$_19ENS_9allocatorIS4_EEFffEE11target_typeEv

	.section	__TEXT,__const
__ZTSNSt3__110__function6__funcIN2ml11projections4$_19ENS_9allocatorIS4_EEFffEEE: ; @"_ZTSNSt3__110__function6__funcIN2ml11projections4$_19ENS_9allocatorIS4_EEFffEEE"
	.asciz	"NSt3__110__function6__funcIN2ml11projections4$_19ENS_9allocatorIS4_EEFffEEE"

	.section	__DATA,__const
	.p2align	3, 0x0                          ; @"_ZTINSt3__110__function6__funcIN2ml11projections4$_19ENS_9allocatorIS4_EEFffEEE"
__ZTINSt3__110__function6__funcIN2ml11projections4$_19ENS_9allocatorIS4_EEFffEEE:
	.quad	__ZTVN10__cxxabiv120__si_class_type_infoE+16
	.quad	__ZTSNSt3__110__function6__funcIN2ml11projections4$_19ENS_9allocatorIS4_EEFffEEE
	.quad	__ZTINSt3__110__function6__baseIFffEEE

	.section	__TEXT,__const
__ZTSN2ml11projections4$_19E:           ; @"_ZTSN2ml11projections4$_19E"
	.asciz	"N2ml11projections4$_19E"

	.section	__DATA,__const
	.p2align	3, 0x0                          ; @"_ZTIN2ml11projections4$_19E"
__ZTIN2ml11projections4$_19E:
	.quad	__ZTVN10__cxxabiv117__class_type_infoE+16
	.quad	__ZTSN2ml11projections4$_19E

	.p2align	3, 0x0                          ; @"_ZTVNSt3__110__function6__funcIN2ml10dspwindows4$_20ENS_9allocatorIS4_EEFffEEE"
__ZTVNSt3__110__function6__funcIN2ml10dspwindows4$_20ENS_9allocatorIS4_EEFffEEE:
	.quad	0
	.quad	__ZTINSt3__110__function6__funcIN2ml10dspwindows4$_20ENS_9allocatorIS4_EEFffEEE
	.quad	__ZNSt3__110__function6__funcIN2ml10dspwindows4$_20ENS_9allocatorIS4_EEFffEED1Ev
	.quad	__ZNSt3__110__function6__funcIN2ml10dspwindows4$_20ENS_9allocatorIS4_EEFffEED0Ev
	.quad	__ZNKSt3__110__function6__funcIN2ml10dspwindows4$_20ENS_9allocatorIS4_EEFffEE7__cloneEv
	.quad	__ZNKSt3__110__function6__funcIN2ml10dspwindows4$_20ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE
	.quad	__ZNSt3__110__function6__funcIN2ml10dspwindows4$_20ENS_9allocatorIS4_EEFffEE7destroyEv
	.quad	__ZNSt3__110__function6__funcIN2ml10dspwindows4$_20ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv
	.quad	__ZNSt3__110__function6__funcIN2ml10dspwindows4$_20ENS_9allocatorIS4_EEFffEEclEOf
	.quad	__ZNKSt3__110__function6__funcIN2ml10dspwindows4$_20ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info
	.quad	__ZNKSt3__110__function6__funcIN2ml10dspwindows4$_20ENS_9allocatorIS4_EEFffEE11target_typeEv

	.section	__TEXT,__const
__ZTSNSt3__110__function6__funcIN2ml10dspwindows4$_20ENS_9allocatorIS4_EEFffEEE: ; @"_ZTSNSt3__110__function6__funcIN2ml10dspwindows4$_20ENS_9allocatorIS4_EEFffEEE"
	.asciz	"NSt3__110__function6__funcIN2ml10dspwindows4$_20ENS_9allocatorIS4_EEFffEEE"

	.section	__DATA,__const
	.p2align	3, 0x0                          ; @"_ZTINSt3__110__function6__funcIN2ml10dspwindows4$_20ENS_9allocatorIS4_EEFffEEE"
__ZTINSt3__110__function6__funcIN2ml10dspwindows4$_20ENS_9allocatorIS4_EEFffEEE:
	.quad	__ZTVN10__cxxabiv120__si_class_type_infoE+16
	.quad	__ZTSNSt3__110__function6__funcIN2ml10dspwindows4$_20ENS_9allocatorIS4_EEFffEEE
	.quad	__ZTINSt3__110__function6__baseIFffEEE

	.section	__TEXT,__const
__ZTSN2ml10dspwindows4$_20E:            ; @"_ZTSN2ml10dspwindows4$_20E"
	.asciz	"N2ml10dspwindows4$_20E"

	.section	__DATA,__const
	.p2align	3, 0x0                          ; @"_ZTIN2ml10dspwindows4$_20E"
__ZTIN2ml10dspwindows4$_20E:
	.quad	__ZTVN10__cxxabiv117__class_type_infoE+16
	.quad	__ZTSN2ml10dspwindows4$_20E

	.p2align	3, 0x0                          ; @"_ZTVNSt3__110__function6__funcIN2ml10dspwindows4$_21ENS_9allocatorIS4_EEFffEEE"
__ZTVNSt3__110__function6__funcIN2ml10dspwindows4$_21ENS_9allocatorIS4_EEFffEEE:
	.quad	0
	.quad	__ZTINSt3__110__function6__funcIN2ml10dspwindows4$_21ENS_9allocatorIS4_EEFffEEE
	.quad	__ZNSt3__110__function6__funcIN2ml10dspwindows4$_21ENS_9allocatorIS4_EEFffEED1Ev
	.quad	__ZNSt3__110__function6__funcIN2ml10dspwindows4$_21ENS_9allocatorIS4_EEFffEED0Ev
	.quad	__ZNKSt3__110__function6__funcIN2ml10dspwindows4$_21ENS_9allocatorIS4_EEFffEE7__cloneEv
	.quad	__ZNKSt3__110__function6__funcIN2ml10dspwindows4$_21ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE
	.quad	__ZNSt3__110__function6__funcIN2ml10dspwindows4$_21ENS_9allocatorIS4_EEFffEE7destroyEv
	.quad	__ZNSt3__110__function6__funcIN2ml10dspwindows4$_21ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv
	.quad	__ZNSt3__110__function6__funcIN2ml10dspwindows4$_21ENS_9allocatorIS4_EEFffEEclEOf
	.quad	__ZNKSt3__110__function6__funcIN2ml10dspwindows4$_21ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info
	.quad	__ZNKSt3__110__function6__funcIN2ml10dspwindows4$_21ENS_9allocatorIS4_EEFffEE11target_typeEv

	.section	__TEXT,__const
__ZTSNSt3__110__function6__funcIN2ml10dspwindows4$_21ENS_9allocatorIS4_EEFffEEE: ; @"_ZTSNSt3__110__function6__funcIN2ml10dspwindows4$_21ENS_9allocatorIS4_EEFffEEE"
	.asciz	"NSt3__110__function6__funcIN2ml10dspwindows4$_21ENS_9allocatorIS4_EEFffEEE"

	.section	__DATA,__const
	.p2align	3, 0x0                          ; @"_ZTINSt3__110__function6__funcIN2ml10dspwindows4$_21ENS_9allocatorIS4_EEFffEEE"
__ZTINSt3__110__function6__funcIN2ml10dspwindows4$_21ENS_9allocatorIS4_EEFffEEE:
	.quad	__ZTVN10__cxxabiv120__si_class_type_infoE+16
	.quad	__ZTSNSt3__110__function6__funcIN2ml10dspwindows4$_21ENS_9allocatorIS4_EEFffEEE
	.quad	__ZTINSt3__110__function6__baseIFffEEE

	.section	__TEXT,__const
__ZTSN2ml10dspwindows4$_21E:            ; @"_ZTSN2ml10dspwindows4$_21E"
	.asciz	"N2ml10dspwindows4$_21E"

	.section	__DATA,__const
	.p2align	3, 0x0                          ; @"_ZTIN2ml10dspwindows4$_21E"
__ZTIN2ml10dspwindows4$_21E:
	.quad	__ZTVN10__cxxabiv117__class_type_infoE+16
	.quad	__ZTSN2ml10dspwindows4$_21E

	.p2align	3, 0x0                          ; @"_ZTVNSt3__110__function6__funcIN2ml10dspwindows4$_22ENS_9allocatorIS4_EEFffEEE"
__ZTVNSt3__110__function6__funcIN2ml10dspwindows4$_22ENS_9allocatorIS4_EEFffEEE:
	.quad	0
	.quad	__ZTINSt3__110__function6__funcIN2ml10dspwindows4$_22ENS_9allocatorIS4_EEFffEEE
	.quad	__ZNSt3__110__function6__funcIN2ml10dspwindows4$_22ENS_9allocatorIS4_EEFffEED1Ev
	.quad	__ZNSt3__110__function6__funcIN2ml10dspwindows4$_22ENS_9allocatorIS4_EEFffEED0Ev
	.quad	__ZNKSt3__110__function6__funcIN2ml10dspwindows4$_22ENS_9allocatorIS4_EEFffEE7__cloneEv
	.quad	__ZNKSt3__110__function6__funcIN2ml10dspwindows4$_22ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE
	.quad	__ZNSt3__110__function6__funcIN2ml10dspwindows4$_22ENS_9allocatorIS4_EEFffEE7destroyEv
	.quad	__ZNSt3__110__function6__funcIN2ml10dspwindows4$_22ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv
	.quad	__ZNSt3__110__function6__funcIN2ml10dspwindows4$_22ENS_9allocatorIS4_EEFffEEclEOf
	.quad	__ZNKSt3__110__function6__funcIN2ml10dspwindows4$_22ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info
	.quad	__ZNKSt3__110__function6__funcIN2ml10dspwindows4$_22ENS_9allocatorIS4_EEFffEE11target_typeEv

	.section	__TEXT,__const
__ZTSNSt3__110__function6__funcIN2ml10dspwindows4$_22ENS_9allocatorIS4_EEFffEEE: ; @"_ZTSNSt3__110__function6__funcIN2ml10dspwindows4$_22ENS_9allocatorIS4_EEFffEEE"
	.asciz	"NSt3__110__function6__funcIN2ml10dspwindows4$_22ENS_9allocatorIS4_EEFffEEE"

	.section	__DATA,__const
	.p2align	3, 0x0                          ; @"_ZTINSt3__110__function6__funcIN2ml10dspwindows4$_22ENS_9allocatorIS4_EEFffEEE"
__ZTINSt3__110__function6__funcIN2ml10dspwindows4$_22ENS_9allocatorIS4_EEFffEEE:
	.quad	__ZTVN10__cxxabiv120__si_class_type_infoE+16
	.quad	__ZTSNSt3__110__function6__funcIN2ml10dspwindows4$_22ENS_9allocatorIS4_EEFffEEE
	.quad	__ZTINSt3__110__function6__baseIFffEEE

	.section	__TEXT,__const
__ZTSN2ml10dspwindows4$_22E:            ; @"_ZTSN2ml10dspwindows4$_22E"
	.asciz	"N2ml10dspwindows4$_22E"

	.section	__DATA,__const
	.p2align	3, 0x0                          ; @"_ZTIN2ml10dspwindows4$_22E"
__ZTIN2ml10dspwindows4$_22E:
	.quad	__ZTVN10__cxxabiv117__class_type_infoE+16
	.quad	__ZTSN2ml10dspwindows4$_22E

	.p2align	3, 0x0                          ; @"_ZTVNSt3__110__function6__funcIN2ml10dspwindows4$_23ENS_9allocatorIS4_EEFffEEE"
__ZTVNSt3__110__function6__funcIN2ml10dspwindows4$_23ENS_9allocatorIS4_EEFffEEE:
	.quad	0
	.quad	__ZTINSt3__110__function6__funcIN2ml10dspwindows4$_23ENS_9allocatorIS4_EEFffEEE
	.quad	__ZNSt3__110__function6__funcIN2ml10dspwindows4$_23ENS_9allocatorIS4_EEFffEED1Ev
	.quad	__ZNSt3__110__function6__funcIN2ml10dspwindows4$_23ENS_9allocatorIS4_EEFffEED0Ev
	.quad	__ZNKSt3__110__function6__funcIN2ml10dspwindows4$_23ENS_9allocatorIS4_EEFffEE7__cloneEv
	.quad	__ZNKSt3__110__function6__funcIN2ml10dspwindows4$_23ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE
	.quad	__ZNSt3__110__function6__funcIN2ml10dspwindows4$_23ENS_9allocatorIS4_EEFffEE7destroyEv
	.quad	__ZNSt3__110__function6__funcIN2ml10dspwindows4$_23ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv
	.quad	__ZNSt3__110__function6__funcIN2ml10dspwindows4$_23ENS_9allocatorIS4_EEFffEEclEOf
	.quad	__ZNKSt3__110__function6__funcIN2ml10dspwindows4$_23ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info
	.quad	__ZNKSt3__110__function6__funcIN2ml10dspwindows4$_23ENS_9allocatorIS4_EEFffEE11target_typeEv

	.section	__TEXT,__const
__ZTSNSt3__110__function6__funcIN2ml10dspwindows4$_23ENS_9allocatorIS4_EEFffEEE: ; @"_ZTSNSt3__110__function6__funcIN2ml10dspwindows4$_23ENS_9allocatorIS4_EEFffEEE"
	.asciz	"NSt3__110__function6__funcIN2ml10dspwindows4$_23ENS_9allocatorIS4_EEFffEEE"

	.section	__DATA,__const
	.p2align	3, 0x0                          ; @"_ZTINSt3__110__function6__funcIN2ml10dspwindows4$_23ENS_9allocatorIS4_EEFffEEE"
__ZTINSt3__110__function6__funcIN2ml10dspwindows4$_23ENS_9allocatorIS4_EEFffEEE:
	.quad	__ZTVN10__cxxabiv120__si_class_type_infoE+16
	.quad	__ZTSNSt3__110__function6__funcIN2ml10dspwindows4$_23ENS_9allocatorIS4_EEFffEEE
	.quad	__ZTINSt3__110__function6__baseIFffEEE

	.section	__TEXT,__const
__ZTSN2ml10dspwindows4$_23E:            ; @"_ZTSN2ml10dspwindows4$_23E"
	.asciz	"N2ml10dspwindows4$_23E"

	.section	__DATA,__const
	.p2align	3, 0x0                          ; @"_ZTIN2ml10dspwindows4$_23E"
__ZTIN2ml10dspwindows4$_23E:
	.quad	__ZTVN10__cxxabiv117__class_type_infoE+16
	.quad	__ZTSN2ml10dspwindows4$_23E

	.p2align	3, 0x0                          ; @"_ZTVNSt3__110__function6__funcIN2ml10dspwindows4$_24ENS_9allocatorIS4_EEFffEEE"
__ZTVNSt3__110__function6__funcIN2ml10dspwindows4$_24ENS_9allocatorIS4_EEFffEEE:
	.quad	0
	.quad	__ZTINSt3__110__function6__funcIN2ml10dspwindows4$_24ENS_9allocatorIS4_EEFffEEE
	.quad	__ZNSt3__110__function6__funcIN2ml10dspwindows4$_24ENS_9allocatorIS4_EEFffEED1Ev
	.quad	__ZNSt3__110__function6__funcIN2ml10dspwindows4$_24ENS_9allocatorIS4_EEFffEED0Ev
	.quad	__ZNKSt3__110__function6__funcIN2ml10dspwindows4$_24ENS_9allocatorIS4_EEFffEE7__cloneEv
	.quad	__ZNKSt3__110__function6__funcIN2ml10dspwindows4$_24ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE
	.quad	__ZNSt3__110__function6__funcIN2ml10dspwindows4$_24ENS_9allocatorIS4_EEFffEE7destroyEv
	.quad	__ZNSt3__110__function6__funcIN2ml10dspwindows4$_24ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv
	.quad	__ZNSt3__110__function6__funcIN2ml10dspwindows4$_24ENS_9allocatorIS4_EEFffEEclEOf
	.quad	__ZNKSt3__110__function6__funcIN2ml10dspwindows4$_24ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info
	.quad	__ZNKSt3__110__function6__funcIN2ml10dspwindows4$_24ENS_9allocatorIS4_EEFffEE11target_typeEv

	.section	__TEXT,__const
__ZTSNSt3__110__function6__funcIN2ml10dspwindows4$_24ENS_9allocatorIS4_EEFffEEE: ; @"_ZTSNSt3__110__function6__funcIN2ml10dspwindows4$_24ENS_9allocatorIS4_EEFffEEE"
	.asciz	"NSt3__110__function6__funcIN2ml10dspwindows4$_24ENS_9allocatorIS4_EEFffEEE"

	.section	__DATA,__const
	.p2align	3, 0x0                          ; @"_ZTINSt3__110__function6__funcIN2ml10dspwindows4$_24ENS_9allocatorIS4_EEFffEEE"
__ZTINSt3__110__function6__funcIN2ml10dspwindows4$_24ENS_9allocatorIS4_EEFffEEE:
	.quad	__ZTVN10__cxxabiv120__si_class_type_infoE+16
	.quad	__ZTSNSt3__110__function6__funcIN2ml10dspwindows4$_24ENS_9allocatorIS4_EEFffEEE
	.quad	__ZTINSt3__110__function6__baseIFffEEE

	.section	__TEXT,__const
__ZTSN2ml10dspwindows4$_24E:            ; @"_ZTSN2ml10dspwindows4$_24E"
	.asciz	"N2ml10dspwindows4$_24E"

	.section	__DATA,__const
	.p2align	3, 0x0                          ; @"_ZTIN2ml10dspwindows4$_24E"
__ZTIN2ml10dspwindows4$_24E:
	.quad	__ZTVN10__cxxabiv117__class_type_infoE+16
	.quad	__ZTSN2ml10dspwindows4$_24E

	.p2align	3, 0x0                          ; @"_ZTVNSt3__110__function6__funcIN2ml10dspwindows4$_25ENS_9allocatorIS4_EEFffEEE"
__ZTVNSt3__110__function6__funcIN2ml10dspwindows4$_25ENS_9allocatorIS4_EEFffEEE:
	.quad	0
	.quad	__ZTINSt3__110__function6__funcIN2ml10dspwindows4$_25ENS_9allocatorIS4_EEFffEEE
	.quad	__ZNSt3__110__function6__funcIN2ml10dspwindows4$_25ENS_9allocatorIS4_EEFffEED1Ev
	.quad	__ZNSt3__110__function6__funcIN2ml10dspwindows4$_25ENS_9allocatorIS4_EEFffEED0Ev
	.quad	__ZNKSt3__110__function6__funcIN2ml10dspwindows4$_25ENS_9allocatorIS4_EEFffEE7__cloneEv
	.quad	__ZNKSt3__110__function6__funcIN2ml10dspwindows4$_25ENS_9allocatorIS4_EEFffEE7__cloneEPNS0_6__baseIS7_EE
	.quad	__ZNSt3__110__function6__funcIN2ml10dspwindows4$_25ENS_9allocatorIS4_EEFffEE7destroyEv
	.quad	__ZNSt3__110__function6__funcIN2ml10dspwindows4$_25ENS_9allocatorIS4_EEFffEE18destroy_deallocateEv
	.quad	__ZNSt3__110__function6__funcIN2ml10dspwindows4$_25ENS_9allocatorIS4_EEFffEEclEOf
	.quad	__ZNKSt3__110__function6__funcIN2ml10dspwindows4$_25ENS_9allocatorIS4_EEFffEE6targetERKSt9type_info
	.quad	__ZNKSt3__110__function6__funcIN2ml10dspwindows4$_25ENS_9allocatorIS4_EEFffEE11target_typeEv

	.section	__TEXT,__const
__ZTSNSt3__110__function6__funcIN2ml10dspwindows4$_25ENS_9allocatorIS4_EEFffEEE: ; @"_ZTSNSt3__110__function6__funcIN2ml10dspwindows4$_25ENS_9allocatorIS4_EEFffEEE"
	.asciz	"NSt3__110__function6__funcIN2ml10dspwindows4$_25ENS_9allocatorIS4_EEFffEEE"

	.section	__DATA,__const
	.p2align	3, 0x0                          ; @"_ZTINSt3__110__function6__funcIN2ml10dspwindows4$_25ENS_9allocatorIS4_EEFffEEE"
__ZTINSt3__110__function6__funcIN2ml10dspwindows4$_25ENS_9allocatorIS4_EEFffEEE:
	.quad	__ZTVN10__cxxabiv120__si_class_type_infoE+16
	.quad	__ZTSNSt3__110__function6__funcIN2ml10dspwindows4$_25ENS_9allocatorIS4_EEFffEEE
	.quad	__ZTINSt3__110__function6__baseIFffEEE

	.section	__TEXT,__const
__ZTSN2ml10dspwindows4$_25E:            ; @"_ZTSN2ml10dspwindows4$_25E"
	.asciz	"N2ml10dspwindows4$_25E"

	.section	__DATA,__const
	.p2align	3, 0x0                          ; @"_ZTIN2ml10dspwindows4$_25E"
__ZTIN2ml10dspwindows4$_25E:
	.quad	__ZTVN10__cxxabiv117__class_type_infoE+16
	.quad	__ZTSN2ml10dspwindows4$_25E

	.section	__DATA,__mod_init_func,mod_init_funcs
	.p2align	3, 0x0
	.quad	__GLOBAL__sub_I_test_phasor.cpp
.zerofill __DATA,__bss,__MergedGlobals,2224,4 ; @_MergedGlobals
.subsections_via_symbols
