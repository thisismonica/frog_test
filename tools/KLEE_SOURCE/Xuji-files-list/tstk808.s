
    .include "esal/inc/esal_ar_stk_defs.inc"
    .include "esal/inc/esal_ar_isr_defs.inc"


    .section   .init
    .align      4


#±£´æ¸¡µã¼Ä´æÆ÷
#VOID  FSTK_Save_Float_Register(register VOID *stack_ptr)

    .globl FSTK_Save_Float_Register
FSTK_Save_Float_Register:

    addi    r3,r3,264
    
    stfdu   f31,-8(r3)
    stfdu   f30,-8(r3)
    stfdu   f29,-8(r3)
    stfdu   f28,-8(r3)
    stfdu   f27,-8(r3)
    stfdu   f26,-8(r3)
    stfdu   f25,-8(r3)
    stfdu   f24,-8(r3)
    stfdu   f23,-8(r3)
    stfdu   f22,-8(r3)
    stfdu   f21,-8(r3)
    stfdu   f20,-8(r3)
    stfdu   f19,-8(r3)
    stfdu   f18,-8(r3)
    stfdu   f17,-8(r3)
    stfdu   f16,-8(r3)
    stfdu   f15,-8(r3)
    stfdu   f14,-8(r3)
    stfdu   f13,-8(r3)
    stfdu   f12,-8(r3)
    stfdu   f11,-8(r3)
    stfdu   f10,-8(r3)
    stfdu   f9,-8(r3)
    stfdu   f8,-8(r3)
    stfdu   f7,-8(r3)
    stfdu   f6,-8(r3)
    stfdu   f5,-8(r3)
    stfdu   f4,-8(r3)
    stfdu   f3,-8(r3)
    stfdu   f2,-8(r3)
    stfdu   f1,-8(r3)
    stfdu   f0,-8(r3)
    mffs    f0
    stfdu   f0,-8(r3)
	blr


#»Ö¸´¸¡µã¼Ä´æÆ÷
#VOID  FSTK_Restore_Float_Register(register VOID *stack_ptr)

    .globl FSTK_Restore_Float_Register
FSTK_Restore_Float_Register:

    lfdu    f0,0(r3) 
    mtfsf   ESAL_AR_STK_FPSCR_FM,f0
    
    lfdu    f0,8(r3)
    lfdu    f1,8(r3)
    lfdu    f2,8(r3)
    lfdu    f3,8(r3)
    lfdu    f4,8(r3)
    lfdu    f5,8(r3)
    lfdu    f6,8(r3)
    lfdu    f7,8(r3)
    lfdu    f8,8(r3)
    lfdu    f9,8(r3)
    lfdu    f10,8(r3)
    lfdu    f11,8(r3)
    lfdu    f12,8(r3)
    lfdu    f13,8(r3)
    lfdu    f14,8(r3)
    lfdu    f15,8(r3)
    lfdu    f16,8(r3)
    lfdu    f17,8(r3)
    lfdu    f18,8(r3)
    lfdu    f19,8(r3)
    lfdu    f20,8(r3)
    lfdu    f21,8(r3)
    lfdu    f22,8(r3)
    lfdu    f23,8(r3)
    lfdu    f24,8(r3)
    lfdu    f25,8(r3)
    lfdu    f26,8(r3)
    lfdu    f27,8(r3)
    lfdu    f28,8(r3)
    lfdu    f29,8(r3)
    lfdu    f30,8(r3)
    lfdu    f31,8(r3)
    blr



