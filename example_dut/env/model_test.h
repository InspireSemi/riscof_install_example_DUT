#ifndef _COMPLIANCE_MODEL_H
#define _COMPLIANCE_MODEL_H

/* Opencore Uart */
//#define UART0_CTRL_ADDR 0x2F208000
/* Fast Uart SIM only */
#define UART0_CTRL_ADDR 0x2F208700

#define  UART_BASE UART0_CTRL_ADDR
#define UART0_CTRL_SIZE 0x10

/* Register offsets */
#define UART_RBR	0x0000
#define UART_THR	0x0000
#define UART_IER	0x0001
#define UART_IIR	0x0002
#define UART_FCR	0x0002
#define UART_LCR	0x0003
#define UART_MCR	0x0004
#define UART_LSR	0x0005
#define UART_MSR	0x0006
#define UART_DLL	0x0000
#define UART_DLM	0x0001

#define UART_8N1	0x03
#define UART_7E2	0x1E
#define UART_DR		0x01
#define UART_THRE	0x20

	/* divisor latch values for buad rates at freq = 33.333Mhz
	2400: 868 = 0x365
	4800: 434 = 0x1B2
	9600: 217 = 0xD9
	19200: 108 = 0x6C 
	38400: 54 = 0x36
	57600: 36 = 0x24
	115200: 18 = 0x12 
	1041000: 2 = 0x02 Sim Only
	3125000: 1 = 0x01 FastUart Sim Only
	*/

// Base address of GPIO 0-31
#define GPIO_ADDR 0x2f200000
#define GPIO_WR_OFFSET 0
#define GPIO_RD_OFFSET 4
#define GPIO_RXEN_OFFSET 44
#define GPIO_TXEN_OFFSET 48
#define GPIO13_SET 0x2000
#define GPIO13_CLR 0xFFFFDFFF

#define GPIO_INIT \
  li t0, GPIO_ADDR; \
  li t1, GPIO13_SET; \
  lw t2, GPIO_TXEN_OFFSET(t0); \
  or t1, t1, t2; \
  sw t1, GPIO_TXEN_OFFSET(t0); \
  li t1, GPIO13_CLR; \
  lw t2, GPIO_RXEN_OFFSET(t0); \
  and t1, t1, t2; \
  sw t1, GPIO_RXEN_OFFSET(t0);

#define GPIO_SET_13 \
  li t0, GPIO_ADDR; \
  li t1, GPIO13_SET; \
  lw t2, GPIO_WR_OFFSET(t0); \
  or t1, t1, t2; \
  sw t1, GPIO_WR_OFFSET(t0);


#define RV64UARTINIT \
	li	a0,UART_BASE; \
	sb	zero,UART_IER(a0);	/*  disable interrupts */ \
	li	t1,0x80; \
	sb	t1,UART_LCR(a0);		/*  set DLAB (divisor latch access bit) to hi */ \
	li	t1,0x01;			/*  Change based on the crystal/baud rate. &&&&&*/ \
	and	t1,t1,0xff;		/*  first drop the upper byte of the half word */ \
	sb	t1,UART_DLL(a0);	/*  store divisor LSB in DLL */ \
	li	t1,0x02;				/*  Change based on the crystal/baud rate. &&&&&*/ \
	srl	t1,t1,8;				/*  shift right 8 bits to move upper byte into lower byte */ \
	sb	t1,UART_DLM(a0);		/*  store divisor MSB in DLM */ \
	li	t1,UART_8N1;			/*  load t1 with 8 bits, no parity, 1 stop bit */ \
	sb	t1,UART_LCR(a0);		/*  write into LCR */ \
							/* This will also set the UART back to normal operations */ \
	li	t1,0x07;		/*  enable fifos and clear them */ \
	sb	t1,UART_FCR(a0); \
	li	t1, 0x00;	/* Clear MCR bit 5 to disable Auto-RTS & Auto-CTS */ \
	sb	t1, UART_MCR(a0)

#define RV64UARTOUT(_R) \
	li	t2,UART_BASE;	/* load uart register base address */ \
1:	lb	t1,UART_LSR(t2);	/* get that status one more time */ \
	nop;			/* again - dont be in a rush */	 \
	and	t1,t1,UART_THRE;	/* see if the transmitter is ready to */ \
				/* accept another char, i.e THRE indicator set*/ \
	beq	t1,zero,1b;	/* branch if not ready */ \
	nop; \
	sb	_R,UART_THR(t2);	/* send the char to xmit buf, THR. */



#define RVMODEL_DATA_SECTION \
        .pushsection .tohost,"aw",@progbits;                            \
        .align 8; .global tohost; tohost: .dword 0;                     \
        .align 8; .global fromhost; fromhost: .dword 0;                 \
        .popsection;                                                    \
        .align 8; .global begin_regstate; begin_regstate:               \
        .word 128;                                                      \
        .align 8; .global end_regstate; end_regstate:                   \
        .word 4;

//RV_COMPLIANCE_HALT
#define RVMODEL_HALT                                              \
        GPIO_SET_13; \
        RVMODEL_IO_WRITE_STR(t6, "GPIO13\n"); \
9999: \
        j 9999b;


/* 
        la s10, begin_signature; \
        la s11, end_signature; \
        RVMODEL_IO_WRITE_STR(t6, "Signature Begin\n"); \
1: \
        ld a0, 0(s10); \
        LOCAL_IO_PUSH(t6) \
        jal FN_WriteNmbr; \
        RVMODEL_IO_WRITE_STR2("\n") \
        LOCAL_IO_POP(t6) \
        add s10, s10, 8; \
        blt s10, s11, 1b; \
        RVMODEL_IO_WRITE_STR(t6, "Sigature End\n"); \
*/


//RV_COMPLIANCE_DATA_BEGIN
#define RVMODEL_DATA_BEGIN                                              \
  RVMODEL_DATA_SECTION                                                        \
  .align 4; .global begin_signature; begin_signature:

//RV_COMPLIANCE_DATA_END
#define RVMODEL_DATA_END                                                      \
  .global end_signature; end_signature:  

//RVMODEL_IO_INIT
#define RVMODEL_IO_INIT \
    RV64UARTINIT; \
    GPIO_INIT;

//RVMODEL_BOOT
#define RVMODEL_BOOT \
.section .text.init; \
  csrr a0, mhartid	;\
  li   a1, 0x3f		;\
  beq  a1, a0, cont	;\
55: \
  wfi ;\
  j 55b ;\
cont: ;\
  RVMODEL_IO_INIT;

/*
  la t0, _data_strings; \
  la t1, _fstext; \
  la t2, _estext; \
1: \
  lw t3, 0(t0); \
  sw t3, 0(t1); \
  addi t0, t0, 4; \
  addi t1, t1, 4; \
  bltu t1, t2, 1b; \
  la t0, _data_lma; \
  la t1, _data; \
  la t2, _edata; \
1: \
  lw t3, 0(t0); \
  sw t3, 0(t1); \
  addi t0, t0, 4; \
  addi t1, t1, 4; \
  bltu t1, t2, 1b; \
*/


/* This will put all Harts other than Boot Hart in a Wait state. With a mtvec = local sram for each Hart. */
#define smp_wait(reg1, reg2)  \
  csrr reg1, mhartid				;\
  li   reg2, NONSMP_HART		;\
  beq  reg1, reg2, cont			;\
  slli reg2, reg1, 15       ;\
  csrw mtvec, reg2           ;\
  wfi    					;\
  mret            ;\
cont: 

//RVTEST_IO_WRITE_STR
// _SP = (volatile register)
#define LOCAL_IO_WRITE_STR(_STR) RVMODEL_IO_WRITE_STR(x31, _STR)
#define RVMODEL_IO_WRITE_STR(_SP, _STR)                                  \
    LOCAL_IO_PUSH(_SP)                                                  \
    .section .data.string;                                              \
20001:                                                                  \
    .string _STR;                                                       \
    .section .text.init;                                                     \
    la a0, 20001b;                                                      \
    jal FN_WriteStr;                                                    \
    LOCAL_IO_POP(_SP)

//RVTEST_IO_CHECK
#define RVMODEL_IO_CHECK()

#define RVMODEL_IO_WRITE_STR2(_STR)                                  \
    .section .data.string;                                              \
20001:                                                                  \
    .string _STR;                                                       \
    .section .text.init;                                                     \
    la a0, 20001b;                                                      \
    jal FN_WriteStr; 


#define RSIZE 8
// _SP = (volatile register)
#define LOCAL_IO_PUSH(_SP)                                              \
    la      _SP,  begin_regstate;                                       \
    sd      ra,   (1*RSIZE)(_SP);                                       \
    sd      t0,   (2*RSIZE)(_SP);                                       \
    sd      t1,   (3*RSIZE)(_SP);                                       \
    sd      t2,   (4*RSIZE)(_SP);                                       \
    sd      t3,   (5*RSIZE)(_SP);                                       \   
    sd      t4,   (6*RSIZE)(_SP);                                      \
    sd      t5,   (7*RSIZE)(_SP);                                      \
    sd      s0,   (8*RSIZE)(_SP);                                      \
    sd      a0,   (9*RSIZE)(_SP);

// _SP = (volatile register)
#define LOCAL_IO_POP(_SP)                                               \
    la      _SP,   begin_regstate;                                      \
    ld      ra,   (1*RSIZE)(_SP);                                       \
    ld      t0,   (2*RSIZE)(_SP);                                       \
    ld      t1,   (3*RSIZE)(_SP);                                       \
    ld      t2,   (4*RSIZE)(_SP);                                       \
    ld      t3,   (5*RSIZE)(_SP);                                       \
    ld      t4,   (6*RSIZE)(_SP);                                       \    
    ld      t5,   (7*RSIZE)(_SP);                                       \       
    ld      s0,   (8*RSIZE)(_SP);                                       \
    ld      a0,   (9*RSIZE)(_SP);

.section .text
// FN_WriteStr: Uses a0, t0, t1, t2
// FN_WriteNmbr: Uses a0, t0, t1, t2, t3, t4, t6
FN_WriteStr: \
    mv          t0, a0; \
10000: \
    lbu         a0, (t0); \
    addi        t0, t0, 1; \
    beq         a0, zero, 10000f; \
    RV64UARTOUT(a0); \
    j           10000b; \
10000: \
    ret; \
FN_WriteNmbr: \
	mv 	t3,a0; \
   	li	t0,16; \
    li t6, 0x30; \
10: \
	srli a0,t3,60; \	
	add a0,a0,t6; \		
	li t4, 0x39; \
	sub	t4,t4,a0; \
	bgez	t4,20f; \	
	addi a0,a0,0x27; \	
20: \
	RV64UARTOUT(a0); \
	slli t3,t3,4; \
	li t2, 1; \
	sub	t0, t0, t2; \
	bgtz	t0, 10b; \
    mv a0, ra; \
    jalr x0, 0(a0); \
    ret;

//RVMODEL_IO_ASSERT_GPR_EQ
// _SP = (volatile register)
// _R = GPR
// _I = Immediate
#if 0

#define RVMODEL_IO_ASSERT_GPR_EQ(_SP, _R, _I)                                 \
    LOCAL_IO_PUSH(_SP)                                                  \
    RVMODEL_IO_WRITE_STR2("|");                                       \
    RVMODEL_IO_WRITE_STR2("\b=\b");                                       \    
    LOCAL_IO_POP(_SP)
#else

#define RVMODEL_IO_ASSERT_GPR_EQ(_SP, _R, _I)                                 \
    LOCAL_IO_PUSH(_SP)                                                  \
    mv          s0, _R;                                                 \
    li          t5, _I;                                                 \
    beq         s0, t5, 20002f;                                         \
    RVMODEL_IO_WRITE_STR2("Test Failed ");                               \
    RVMODEL_IO_WRITE_STR2(": ");                                         \
    RVMODEL_IO_WRITE_STR2(# _R);                                         \
        RVMODEL_IO_WRITE_STR2("( ");                                         \
    mv      a0, s0;                                                     \
    jal FN_WriteNmbr;                                                   \
    RVMODEL_IO_WRITE_STR2(" ) != ");                                     \
    mv      a0, t5;                                                     \
    jal FN_WriteNmbr;                                                   \  
    RVMODEL_IO_WRITE_STR2("\n");                                         \    
    j 20003f;                                                           \
20002:                                                                  \
    RVMODEL_IO_WRITE_STR2("|");                                       \
    RVMODEL_IO_WRITE_STR2("\b=\b");                                    \
20003:                                                                  \
    LOCAL_IO_POP(_SP)

#endif


//RVTEST_IO_ASSERT_SFPR_EQ
#define RVMODEL_IO_ASSERT_SFPR_EQ(_F, _R, _I)
//RVTEST_IO_ASSERT_DFPR_EQ
#define RVMODEL_IO_ASSERT_DFPR_EQ(_D, _R, _I)

#define RVMODEL_SET_MSW_INT       \
 li t1, 1;                         \
 li t2, 0x2000000;                 \
 sw t1, 0(t2);

#define RVMODEL_CLEAR_MSW_INT     \
 li t2, 0x2000000;                 \
 sw x0, 0(t2);

#define RVMODEL_CLEAR_MTIMER_INT

#define RVMODEL_CLEAR_MEXT_INT

#endif // _COMPLIANCE_MODEL_H

