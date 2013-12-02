#if defined(__NetBSD__) || defined(__FreeBSD__)
#define  RN_OLDMASK		1
/* Register numbers */
#define  RN_ESP			2
#define  RN_EBP			3
#define  RN_ESP_AT_SIGNAL	4
#define  RN_EIP			5
#define  RN_EFLAGS		6
#define  RN_ES			7
#define  RN_DS			8
#define  RN_CS			9
#define  RN_SS			10
#define  RN_EDI  		11
#define  RN_ESI			12
#define  RN_EBX			13
#define  RN_EDX			14
#define  RN_ECX			15
#define  RN_EAX			16
/* NetBSD doesn't context switch gs or fs */
#define  SC_GS			0x27
#define  SC_FS			0x27
#else
/* Register numbers */
#define  RN_GS			0
#define  RN_FS			1
#define  RN_ES			2
#define  RN_DS			3
#define  RN_EDI  		4
#define  RN_ESI			5
#define  RN_EBP			6
#define  RN_ESP			7
#define  RN_EBX			8
#define  RN_EDX			9
#define  RN_ECX			10
#define  RN_EAX			11
#define  RN_TRAPNO		12
#define  RN_ERR			13
#define  RN_EIP			14
#define  RN_CS			15
#define  RN_EFLAGS		16
#define  RN_ESP_AT_SIGNAL	17
#define  RN_SS			18
#define  RN_I387		19
#define  RN_OLDMASK		20
#define  RN_CR2			21
#define  SC_GS			regval[RN_GS]
#define  SC_FS			regval[RN_FS]
#define  I387			regval[RN_I387]
#endif

#define  SC_ES			regval[RN_ES]
#define  SC_DS			regval[RN_DS]
#define  SC_EDI(dbg_mask)  	(regval[RN_EDI] & dbg_mask)
#define  SC_ESI(dbg_mask)	(regval[RN_ESI] & dbg_mask)
#define  SC_EBP(dbg_mask)	(regval[RN_EBP] & dbg_mask)
#define  SC_ESP(dbg_mask)	(regval[RN_ESP] & dbg_mask)
#define  SC_EBX(dbg_mask)	(regval[RN_EBX] & dbg_mask)
#define  SC_EDX(dbg_mask)	(regval[RN_EDX] & dbg_mask)
#define  SC_ECX(dbg_mask)	(regval[RN_ECX] & dbg_mask)
#define  SC_EAX(dbg_mask)	(regval[RN_EAX] & dbg_mask)
#define  SC_DI                  (regval[RN_EDI] & 0xffff)
#define  SC_SI                  (regval[RN_ESI] & 0xffff)
#define  SC_BP                  (regval[RN_EBP] & 0xffff)
#define  SC_SP                  (regval[RN_ESP] & 0xffff)
#define  SC_BX                  (regval[RN_EBX] & 0xffff)
#define  SC_DX                  (regval[RN_EDX] & 0xffff)
#define  SC_CX                  (regval[RN_ECX] & 0xffff)
#define  SC_AX                  (regval[RN_EAX] & 0xffff)
#define  SC_TRAPNO		regval[RN_TRAPNO]
#define  SC_ERR			regval[RN_ERR]
#define  SC_EIP(dbg_mask)	(regval[RN_EIP] & dbg_mask)
#define  SC_CS			regval[RN_CS]
#define  SC_EFLAGS		regval[RN_EFLAGS]
#define  ESP_AT_SIGNAL		regval[RN_ESP_AT_SIGNAL]
#define  SC_SS			regval[RN_SS]
#define  OLDMASK		regval[RN_OLDMASK]
