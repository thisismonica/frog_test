#pragma section code_type ".init"

#ifdef __cplusplus
extern "C" {
#endif

asm void __reset(void);
asm void usr_init();
extern void __start();
extern unsigned long gInterruptVectorTable;
extern unsigned long gInterruptVectorTableEnd;
extern void mmu_cache_init();
#ifdef __cplusplus
}
#endif


asm void __reset(void)
{
	//
	//   Enable machine check exceptions & floating point
	//
	lis		r3, 0x0000
	ori		r3, r3, 0x3000
	mtmsr	r3
		
	b		__start
}

asm void usr_init()
{
	nofralloc
	
#ifdef ROM_VERSION


	// change internal MMR base to 0xe0000000
	lis		r5, 0xe000
	ori		r5, r5, 0x0000
	lis		r4, 0xff400000@ha
	stw		r5, 0xff400000@l(r4)


	// Local Access Windows 
	/////////////////////////

	/*/ Local Bus Local Access Windows /*/

	// WINDOW 0 - NOR FLASH
	lis		r5, 0xfe00
	ori		r5, r5, 0x0000
	lis		r4, 0xe0000020@ha
	stw		r5, 0xe0000020@l(r4)
	
	lis		r5, 0x8000
	ori		r5, r5, 0x0018
	lis		r4, 0xe0000024@ha
	stw		r5, 0xe0000024@l(r4)
	// WINDOW 1 - NAND FLASH
	lis		r5, 0xf800
	ori		r5, r5, 0x0000
	lis		r4, 0xe0000028@ha
	stw		r5, 0xe0000028@l(r4)
	
	lis		r5, 0x8000
	ori		r5, r5, 0x0018
	lis		r4, 0xe000002c@ha
	stw		r5, 0xe000002c@l(r4)
	// WINDOW 2 - VSC7385
	lis		r5, 0xf000
	ori		r5, r5, 0x0000
	lis		r4, 0xe0000030@ha
	stw		r5, 0xe0000030@l(r4)
	
	lis		r5, 0x8000
	ori		r5, r5, 0x0010
	lis		r4, 0xe0000034@ha
	stw		r5, 0xe0000034@l(r4)

	// WINDOW 3 - Read Write Buffer
	lis		r5, 0xfa00
	ori		r5, r5, 0x0000
	lis		r4, 0xe0000038@ha
	stw		r5, 0xe0000038@l(r4)
	
	lis		r5, 0x8000
	ori		r5, r5, 0x000e
	lis		r4, 0xe000003c@ha
	stw		r5, 0xe000003c@l(r4)


	/*/   PCI Local Access Windows   /*/
	
	// WINDOW 0
	lis		r5, 0x8000
	ori		r5, r5, 0x0000
	lis		r4, 0xe0000060@ha
	stw		r5, 0xe0000060@l(r4)
	
	lis		r5, 0x8000
	ori		r5, r5, 0x001c
	lis		r4, 0xe0000064@ha
	stw		r5, 0xe0000064@l(r4)
	// WINDOW 1
	lis		r5, 0xa000
	ori		r5, r5, 0x0000
	lis		r4, 0xe0000068@ha
	stw		r5, 0xe0000068@l(r4)
	
	lis		r5, 0x8000
	ori		r5, r5, 0x001c
	lis		r4, 0xe000006c@ha
	stw		r5, 0xe000006c@l(r4)

	
	/*/   DDR Local Access Windows   /*/
		
	// WINDOW 0 - 1st DDR SODIMM
	lis		r5, 0x0000
	ori		r5, r5, 0x0000
	lis		r4, 0xe00000a0@ha
	stw		r5, 0xe00000a0@l(r4)
	
	lis		r5, 0x8000
	ori		r5, r5, 0x001a
	lis		r4, 0xe00000a4@ha
	stw		r5, 0xe00000a4@l(r4)

	
	// DDR Controller Initialization
	////////////////////////////////
	
	// DDRCDR
	lis		r5, 0x7300
	ori		r5, r5, 0x0002
	lis		r4, 0xe0000128@ha
	stw		r5, 0xe0000128@l(r4)

	// CLK_CNTL
	lis		r5, 0x0200
	ori		r5, r5, 0x0000
	lis		r4, 0xe0002130@ha
	stw		r5, 0xe0002130@l(r4)

	// CS0_BNDS
	lis		r5, 0x0000
	ori		r5, r5, 0x0007
	lis		r4, 0xe0002000@ha
	stw		r5, 0xe0002000@l(r4)
	
	// CS0_CONFIG
	lis		r5, 0x8084
	ori		r5, r5, 0x0102
	lis		r4, 0xe0002080@ha
	stw		r5, 0xe0002080@l(r4)

	// TIMING_CONFIG_0
	lis		r5, 0x0022
	ori		r5, r5, 0x0802
	lis		r4, 0xe0002104@ha
	stw		r5, 0xe0002104@l(r4)

	// TIMING_CONFIG_1
	lis		r5, 0x2625
	ori		r5, r5, 0x6222
	lis		r4, 0xe0002108@ha
	stw		r5, 0xe0002108@l(r4)

	// TIMING_CONFIG_2
	lis		r5, 0x0f90
	ori		r5, r5, 0x28c7
	lis		r4, 0xe000210C@ha
	stw		r5, 0xe000210C@l(r4)
	
	// TIMING_CONFIG_3
	lis		r5, 0x0000
	ori		r5, r5, 0x0000
	lis		r4, 0xe0002100@ha
	stw		r5, 0xe0002100@l(r4)

	// DDR_SDRAM_CFG
	lis		r5, 0x4308
	ori		r5, r5, 0x0000
	lis		r4, 0xe0002110@ha
	stw		r5, 0xe0002110@l(r4)

	// DDR_SDRAM_CFG2
	lis		r5, 0x0040
	ori		r5, r5, 0x1000
	lis		r4, 0xe0002114@ha
	stw		r5, 0xe0002114@l(r4)

	// DDR_SDRAM_MODE
	lis		r5, 0x4440
	ori		r5, r5, 0x0232
	lis		r4, 0xe0002118@ha
	stw		r5, 0xe0002118@l(r4)

	// DDR_SDRAM_MODE2
	lis		r5, 0x8000
	ori		r5, r5, 0xc000
	lis		r4, 0xe000211c@ha
	stw		r5, 0xe000211c@l(r4)
	
	// DDR_SDRAM_INTERVAL
	lis		r5, 0x0320
	ori		r5, r5, 0x0064
	lis		r4, 0xe0002124@ha
	stw		r5, 0xe0002124@l(r4)
	
	isync

	// wait aprox. 200us
	lis r5, 0
	ori  r5,r5,0xf000
	mtspr CTR,r5
wait_loop:
    bc    16,0,wait_loop  // Decrement CTR, then branch if CTR is not 0      

	//enable DDR memory controller
	// DDR_SDRAM_CFG
	lis		r5, 0xc308
	ori		r5, r5, 0x0000
	lis		r4, 0xe0002110@ha
	stw		r5, 0xe0002110@l(r4)
	

	// Configure local bus memory controller
	////////////////////////////////////////

	// CS0 - NOR FLASH
	lis		r5, 0xfe00
	ori		r5, r5, 0x1001
	lis		r4, 0xe0005000@ha
	stw		r5, 0xe0005000@l(r4)
	
	lis		r5, 0xfe00
	ori		r5, r5, 0x6ff7
	lis		r4, 0xe0005004@ha
	stw		r5, 0xe0005004@l(r4)
	
	 //  CS1 - NAND FLASH
	lis		r5, 0xf800
	ori		r5, r5, 0x0C21
	lis		r4, 0xe0005008@ha
	stw		r5, 0xe0005008@l(r4)
	
	lis		r5, 0xffff
	ori		r5, r5, 0x93CC
	lis		r4, 0xe000500c@ha
	stw		r5, 0xe000500c@l(r4)

	 //  CS2 - VSC7385
	lis		r5, 0xFFFE
	ori		r5, r5, 0x09FF
	lis		r4, 0xe0005014@ha
	stw		r5, 0xe0005014@l(r4)
	
	lis		r5, 0xf000
	ori		r5, r5, 0x0801
	lis		r4, 0xe0005010c@ha
	stw		r5, 0xe0005010@l(r4)
	 
	 //  CS2 - 	Read Write Buffer
	lis		r5, 0xFFFF
	ori		r5, r5, 0x8FF7
	lis		r4, 0xe000501c@ha
	stw		r5, 0xe000501c@l(r4)
	
	lis		r5, 0xfa00
	ori		r5, r5, 0x0801
	lis		r4, 0xe0005018c@ha
	stw		r5, 0xe0005018@l(r4)
	
	// LBCR
	lis		r5, 0x0000
	ori		r5, r5, 0x0000
	lis		r4, 0xe00050d0c@ha
	stw		r5, 0xe00050d0@l(r4)

	// LCRR 
	lis		r5, 0x0003
	ori		r5, r5, 0x0002
	lis		r4, 0xe00050d4c@ha
	stw		r5, 0xe00050d4@l(r4)

	// ACR
	lis		r5, 0x0000
	ori		r5, r5, 0x0000
	lis		r4, 0xe0000800@ha
	stw		r5, 0xe0000800@l(r4)

	// MRTPR
	lis		r5, 0x2000
	ori		r5, r5, 0x0000
	lis		r4, 0xe0005084@ha
	stw		r5, 0xe0005084@l(r4)


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Copy the exception vectors from ROM to RAM
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	lis			r3, gInterruptVectorTable@h
	ori			r3, r3, gInterruptVectorTable@l
	subi		r3,r3,0x0004
	
	lis			r4, gInterruptVectorTableEnd@h
	ori			r4, r4, gInterruptVectorTableEnd@l
	
	lis			r5, 0xFFFF
	ori			r5,r5,0xFFFC
	
loop:
	lwzu		r6, 4(r3)
	stwu		r6, 4(r5)
	
	cmpw		r3,r4
	blt			loop
  
   blr 
   
#endif //ROM_version
	
#ifdef CACHE_VERSION
	mfspr    r6,LR        // Save the Link Register value. The link register's
                          // value will be restored so that this function 
                          // can return to the calling address.
	bl		mmu_cache_init
	mtspr    LR,r6      // restore original Link Register value 
#endif

blr

}
