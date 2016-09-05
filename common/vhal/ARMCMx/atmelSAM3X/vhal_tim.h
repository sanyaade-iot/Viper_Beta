#ifndef __VHAL_TIM__
#define __VHAL_TIM__

#include "vhal_common.h"


#define TIM_MODE_OUTPUT_PUSHPULL        6
#define TIM_MODE_INPUT_PULLUP           3


/*******************  Definition for TC_CCRx register (see doc page 880) ********************/
typedef union {
	struct {
		uint32_t CLKEN 		: 1;	/*!<Counter Clock Enable Command        */
		uint32_t CLKDIS 	: 1;	/*!<Counter Clock Disable Command       */
		uint32_t SWTRG 		: 1;	/*!<Software Trigger Command            */
	} fields;
	uint32_t	value;
} TC_CCR_TypeDef;

/*******************  Definition for TC_CMRx register (see doc page 881) ********************/
typedef union {
	union {
        /* TC_CMRx Fields in Capture Mode */
        struct {
            uint32_t TCCLKS     : 3;    /*!<Clock Selection                     */
            uint32_t CLKI       : 1;    /*!<Clock Invert                        */
            uint32_t BURST      : 2;    /*!<Burst Signal Selection              */
            uint32_t LDBSTOP    : 1;
            uint32_t LDBDIS     : 1;
            uint32_t ETRGEDG    : 2;
            uint32_t ABETRG     : 1;
            uint32_t RESERVED0  : 3;
            uint32_t CPCTRG     : 1;
            uint32_t WAVE       : 1;
            uint32_t LDRA       : 2;
            uint32_t LDRB       : 2;
        } capture;

        /* TC_CMRx Fields in Waveform Mode */
        struct {
            uint32_t TCCLKS     : 3;    /*!<Clock Selection                     */
            uint32_t CLKI       : 1;    /*!<Clock Invert                        */
            uint32_t BURST      : 2;    /*!<Burst Signal Selection              */
            uint32_t CPCSTOP    : 1;
            uint32_t CPCDIS     : 1;
            uint32_t EEVTEDG    : 2;
            uint32_t EEVT       : 2;
            uint32_t ENTRG      : 1;
            uint32_t WAVSEL     : 2;
            uint32_t WAVE       : 1;
            uint32_t ACPA       : 2;
            uint32_t ACPC       : 2;
            uint32_t AEEVT      : 2;
            uint32_t ASWTRG     : 2;
            uint32_t BCPB       : 2;
            uint32_t BCPC       : 2;
            uint32_t BEEVT      : 2;
            uint32_t BSWTRG     : 2;
        } waveform;
    } fields;

    /* Register Value */
	uint32_t	value;
} TC_CMR_TypeDef;

/* Possible values for TC_CMR_TCCLKS */
// MCK / 2
#define  TC_CMR_TIMER_CLOCK1                0
// MCK / 8
#define  TC_CMR_TIMER_CLOCK2                1
// MCK / 32
#define  TC_CMR_TIMER_CLOCK3                2
// MCK / 128
#define  TC_CMR_TIMER_CLOCK4                3
// SLCK (Slow Clock)
#define  TC_CMR_TIMER_CLOCK5                4
#define  TC_CMR_XC0                         5
#define  TC_CMR_XC1                         6
#define  TC_CMR_XC2                         7


/*******************  Definition for TC_SMMRx register (see doc page 887) ********************/
typedef union {
    struct {
        uint32_t GCEN       : 1;    /*!<Gray Count Enabled        */
        uint32_t DOWN       : 1;    /*!<Down Count      */
    } fields;
    uint32_t    value;
} TC_SMMR_TypeDef;


/*******************  Definition for TC_SRx register (see doc page 892) ********************/
typedef union {
    struct {
        uint32_t COVFS      : 1;    /*!<Counter Overflow        */
        uint32_t LOVRS      : 1;    /*!<Load Overrun Status      */
        uint32_t CPAS       : 1;    /*!<RA Compare Status      */
        uint32_t CPBS       : 1;
        uint32_t CPCS       : 1;
        uint32_t LDRAS      : 1;
        uint32_t LDRBS      : 1;
        uint32_t ETRGS      : 1;
        uint32_t RESERVED0  : 8;
        uint32_t CLKSTA     : 1;
        uint32_t MTIOA      : 1;
        uint32_t MTIOB      : 1;
    } fields;
    uint32_t    value;
} TC_SR_TypeDef;


/*******************  Definition for TC_IERx register (see doc page 894) ********************/
typedef union {
    struct {
        uint32_t COVFS      : 1;    /*!<Counter Overflow        */
        uint32_t LOVRS      : 1;    /*!<Load Overrun Status      */
        uint32_t CPAS       : 1;    /*!<RA Compare Status      */
        uint32_t CPBS       : 1;
        uint32_t CPCS       : 1;
        uint32_t LDRAS      : 1;
        uint32_t LDRBS      : 1;
        uint32_t ETRGS      : 1;
    } fields;
    uint32_t    value;
} TC_IER_TypeDef;


/*******************  Definition for TC_IDRx register (see doc page 894) ********************/
typedef union {
    struct {
        uint32_t COVFS      : 1;    /*!<Counter Overflow        */
        uint32_t LOVRS      : 1;    /*!<Load Overrun Status      */
        uint32_t CPAS       : 1;    /*!<RA Compare Status      */
        uint32_t CPBS       : 1;
        uint32_t CPCS       : 1;
        uint32_t LDRAS      : 1;
        uint32_t LDRBS      : 1;
        uint32_t ETRGS      : 1;
    } fields;
    uint32_t    value;
} TC_IDR_TypeDef;


/*******************  Definition for TC_IDRx register (see doc page 894) ********************/
typedef union {
    struct {
        uint32_t COVFS      : 1;    /*!<Counter Overflow        */
        uint32_t LOVRS      : 1;    /*!<Load Overrun Status      */
        uint32_t CPAS       : 1;    /*!<RA Compare Status      */
        uint32_t CPBS       : 1;
        uint32_t CPCS       : 1;
        uint32_t LDRAS      : 1;
        uint32_t LDRBS      : 1;
        uint32_t ETRGS      : 1;
    } fields;
    uint32_t    value;
} TC_IMR_TypeDef;


typedef struct {
    __O         TC_CCR_TypeDef       TC_CCR;			/*!< TC Channel Control Register */
    __IO        TC_CMR_TypeDef       TC_CMR;			/*!< TC Channel Mode Register: Capture Mode */
    __IO        TC_SMMR_TypeDef      TC_SMMR;
                uint32_t             RESERVED0;
    __O         uint32_t             TC_CV;
    __IO        uint32_t             TC_RA;
    __IO        uint32_t             TC_RB;
    __IO        uint32_t             TC_RC;
    __O         TC_SR_TypeDef        TC_SR;
    __I         TC_IER_TypeDef       TC_IER;
    __I         TC_IDR_TypeDef       TC_IDR;
    __O         TC_IMR_TypeDef       TC_IMR;
                uint32_t             RESERVED1;
                uint32_t             RESERVED2;
                uint32_t             RESERVED3;
                uint32_t             RESERVED4;
} TIM_CHAN_TypeDef;


/*******************  Definition for TC_BCR register (see doc page 900) ********************/
typedef union {
    struct {
        uint32_t SYNC       : 1;    /*!<Synchro Command        */
    } fields;
    uint32_t    value;
} TC_BCR_TypeDef;


/*******************  Definition for TC_BMR register (see doc page 901) ********************/
typedef union {
    struct {
        uint32_t TC0XC0S    : 2;
        uint32_t TC1XC1S    : 2;
        uint32_t TC2XC2S    : 2;
        uint32_t RESERVED0  : 2;
        uint32_t QDEN       : 1;
        uint32_t POSEN      : 1;
        uint32_t SPEEDEN    : 1;
        uint32_t QDTRANS    : 1;
        uint32_t EDGPHA     : 1;
        uint32_t INVA       : 1;
        uint32_t INVB       : 1;
        uint32_t INVIDX     : 1;
        uint32_t SWAP       : 1;
        uint32_t IDXPHB     : 1;
        uint32_t RESERVED1  : 2;
        uint32_t MAXFILT    : 6;
    } fields;
    uint32_t    value;
} TC_BMR_TypeDef;


/*******************  Definition for TC_QIER register (see doc page 903) ********************/
typedef union {
    struct {
        uint32_t IDX        : 1;
        uint32_t DIRCHG     : 1;
        uint32_t QERR       : 1;
    } fields;
    uint32_t    value;
} TC_QIER_TypeDef;


/*******************  Definition for TC_QIER register (see doc page 903) ********************/
typedef union {
    struct {
        uint32_t IDX        : 1;
        uint32_t DIRCHG     : 1;
        uint32_t QERR       : 1;
    } fields;
    uint32_t    value;
} TC_QIDR_TypeDef;


/*******************  Definition for TC_QIMR register (see doc page 903) ********************/
typedef union {
    struct {
        uint32_t IDX        : 1;
        uint32_t DIRCHG     : 1;
        uint32_t QERR       : 1;
    } fields;
    uint32_t    value;
} TC_QIMR_TypeDef;


/*******************  Definition for TC_QIMR register (see doc page 903) ********************/
typedef union {
    struct {
        uint32_t IDX        : 1;
        uint32_t DIRCHG     : 1;
        uint32_t QERR       : 1;
    } fields;
    uint32_t    value;
} TC_QISR_TypeDef;


/*******************  Definition for TC_FMR register (see doc page 907) ********************/
typedef union {
    struct {
        uint32_t ENCF0      : 1;
        uint32_t ENCF1      : 1;
    } fields;
    uint32_t    value;
} TC_FMR_TypeDef;


/*******************  Definition for TC_WPMR register (see doc page 908) ********************/
typedef union {
    struct {
        uint32_t WPEN       : 1;
        uint32_t RESERVED0  : 7;
        uint32_t WPKEY      : 24;
    } fields;
    uint32_t    value;
} TC_WPMR_TypeDef;


typedef struct {
                TIM_CHAN_TypeDef	channels[3];
    __I	        TC_BCR_TypeDef          TC_BCR;
    __IO        TC_BMR_TypeDef          TC_BMR;
    __O         TC_QIER_TypeDef 	TC_QIER;
    __O         TC_QIDR_TypeDef 	TC_QIDR;
    __I         TC_QIMR_TypeDef 	TC_QIMR;
    __I         TC_QISR_TypeDef         TC_QISR;
    __IO        TC_FMR_TypeDef          TC_FMR;
    __IO        TC_WPMR_TypeDef         TC_WPMR;
                uint8_t                 RESERVED0[20];
} TIM_TypeDef;


/* The total number of Timer Counter on SAM3X8E */
#define		TIMNUM				3
/* The total number of Timer Channels per counter (see doc page 856) */
#define		CHANNUM				3

/* The actual timers of SAM3X8E */
#define  MODULE_TIM1                         ((TIM_TypeDef *) (0x40080000))
#define  MODULE_TIM2                         ((TIM_TypeDef *) (0x40084000))
#define  MODULE_TIM3                         ((TIM_TypeDef *) (0x40088000))

/* Each channel is mapped on a virtual timer */
#define TIM1                                 ((MODULE_TIM1)->channels[0])
#define TIM2                                 ((MODULE_TIM1)->channels[1])
#define TIM3                                 ((MODULE_TIM1)->channels[2])
#define TIM4                                 ((MODULE_TIM2)->channels[0])
#define TIM5                                 ((MODULE_TIM2)->channels[1])
#define TIM6                                 ((MODULE_TIM2)->channels[2])
#define TIM7                                 ((MODULE_TIM3)->channels[0])
#define TIM8                                 ((MODULE_TIM3)->channels[1])
#define TIM9                                 ((MODULE_TIM3)->channels[2])


/* PWM Clock Register */
typedef union {
    struct {
        uint32_t DIVA       : 8;
        uint32_t PREA       : 4;
        uint32_t RESERVED0  : 4;
        uint32_t DIVB       : 8;
        uint32_t PREB       : 4;
        uint32_t RESERVED1  : 4;
    } fields;
    uint32_t    value;
} PWM_CLK_TypeDef;

/* PWM Enable Register */
typedef union {
    struct {
        uint32_t CHID0      : 1;
        uint32_t CHID1      : 1;
        uint32_t CHID2      : 1;
        uint32_t CHID3      : 1;
        uint32_t CHID4      : 1;
        uint32_t CHID5      : 1;
        uint32_t CHID6      : 1;
        uint32_t CHID7      : 1;
        uint32_t RESERVED0  : 24;
    } fields;
    uint32_t    value;
} PWM_ENA_TypeDef;

/* PWM Write Protect Control Register */
typedef union {
    struct {
        uint32_t WPCMD      : 2;
        uint32_t WPRG0      : 1;
        uint32_t WPRG1      : 1;
        uint32_t WPRG2      : 1;
        uint32_t WPRG3      : 1;
        uint32_t WPRG4      : 1;
        uint32_t WPRG5      : 1;
        uint32_t WPKEY      : 24;
    } fields;
    uint32_t    value;
} PWM_WPCR_TypeDef;

/* PWM Channel Mode Register */
typedef union {
    struct {
        uint32_t CPRE       : 4;
        uint32_t RESERVED0  : 4;
        uint32_t CALG       : 1;
        uint32_t CPOL       : 1;
        uint32_t CES        : 1;
        uint32_t RESERVED1  : 5;
        uint32_t DTE        : 1;
        uint32_t DTHI       : 1;
        uint32_t DTLI       : 1;
        uint32_t RESERVED2  : 13;
    } fields;
    uint32_t    value;
} PWM_CMR_TypeDef;

/* PWM Channel Period Register */
typedef union {
    struct {
        uint32_t CPRD       : 24;
        uint32_t RESERVED0  : 8;
    } fields;
    uint32_t    value;
} PWM_CPRD_TypeDef;

/* PWM Duty Cycle Register */
typedef union {
    struct {
        uint32_t CDTY       : 24;
        uint32_t RESERVED0  : 8;
    } fields;
    uint32_t    value;
} PWM_CDTY_TypeDef;



#define  REGISTER_PWM_CLK                    ((PWM_CLK_TypeDef *) (0x40094000))
#define  REGISTER_PWM_ENA                    ((PWM_ENA_TypeDef *) (0x40094004))
#define  REGISTER_PWM_WPCR                   ((PWM_WPCR_TypeDef *) (0x400940E4))

// PWM CHANNEL MODE Registers
#define  REGISTER_PWM_CMR0                   ((PWM_CMR_TypeDef *) (0x40094200))
#define  REGISTER_PWM_CMR1                   ((PWM_CMR_TypeDef *) (0x40094220))
#define  REGISTER_PWM_CMR2                   ((PWM_CMR_TypeDef *) (0x40094240))
#define  REGISTER_PWM_CMR3                   ((PWM_CMR_TypeDef *) (0x40094260))
#define  REGISTER_PWM_CMR4                   ((PWM_CMR_TypeDef *) (0x40094280))
#define  REGISTER_PWM_CMR5                   ((PWM_CMR_TypeDef *) (0x400942A0))
#define  REGISTER_PWM_CMR6                   ((PWM_CMR_TypeDef *) (0x400942C0))
#define  REGISTER_PWM_CMR7                   ((PWM_CMR_TypeDef *) (0x400942E0))

// PWM Period Registers
#define  REGISTER_PWM_CPRD0                  ((PWM_CPRD_TypeDef *) (0x4009420C))
#define  REGISTER_PWM_CPRD1                  ((PWM_CPRD_TypeDef *) (0x4009422C))
#define  REGISTER_PWM_CPRD2                  ((PWM_CPRD_TypeDef *) (0x4009424C))
#define  REGISTER_PWM_CPRD3                  ((PWM_CPRD_TypeDef *) (0x4009426C))
#define  REGISTER_PWM_CPRD4                  ((PWM_CPRD_TypeDef *) (0x4009428C))
#define  REGISTER_PWM_CPRD5                  ((PWM_CPRD_TypeDef *) (0x400942AC))
#define  REGISTER_PWM_CPRD6                  ((PWM_CPRD_TypeDef *) (0x400942CC))
#define  REGISTER_PWM_CPRD7                  ((PWM_CPRD_TypeDef *) (0x400942EC))

// PWM Duty Cycle Registers
#define  REGISTER_PWM_CDTY0                  ((PWM_CDTY_TypeDef *) (0x40094204))
#define  REGISTER_PWM_CDTY1                  ((PWM_CDTY_TypeDef *) (0x40094224))
#define  REGISTER_PWM_CDTY2                  ((PWM_CDTY_TypeDef *) (0x40094244))
#define  REGISTER_PWM_CDTY3                  ((PWM_CDTY_TypeDef *) (0x40094264))
#define  REGISTER_PWM_CDTY4                  ((PWM_CDTY_TypeDef *) (0x40094284))
#define  REGISTER_PWM_CDTY5                  ((PWM_CDTY_TypeDef *) (0x400942A4))
#define  REGISTER_PWM_CDTY6                  ((PWM_CDTY_TypeDef *) (0x400942C4))
#define  REGISTER_PWM_CDTY7                  ((PWM_CDTY_TypeDef *) (0x400942E4))


#define PWM_CHANNEL_IS_ACTIVE(channel)       ((PWM->PWM_SR & (0x1 << channel)) != 0)


#if !defined(PWM)

/** \brief PwmCh_num hardware registers */
typedef struct {
  RwReg      PWM_CMR;       /**< \brief (PwmCh_num Offset: 0x0) PWM Channel Mode Register */
  RwReg      PWM_CDTY;      /**< \brief (PwmCh_num Offset: 0x4) PWM Channel Duty Cycle Register */
  RwReg      PWM_CDTYUPD;   /**< \brief (PwmCh_num Offset: 0x8) PWM Channel Duty Cycle Update Register */
  RwReg      PWM_CPRD;      /**< \brief (PwmCh_num Offset: 0xC) PWM Channel Period Register */
  RwReg      PWM_CPRDUPD;   /**< \brief (PwmCh_num Offset: 0x10) PWM Channel Period Update Register */
  RwReg      PWM_CCNT;      /**< \brief (PwmCh_num Offset: 0x14) PWM Channel Counter Register */
  RwReg      PWM_DT;        /**< \brief (PwmCh_num Offset: 0x18) PWM Channel Dead Time Register */
  RwReg      PWM_DTUPD;     /**< \brief (PwmCh_num Offset: 0x1C) PWM Channel Dead Time Update Register */
} PwmCh_num;
/** \brief PwmCmp hardware registers */
typedef struct {
  RwReg      PWM_CMPV;      /**< \brief (PwmCmp Offset: 0x0) PWM Comparison 0 Value Register */
  RwReg      PWM_CMPVUPD;   /**< \brief (PwmCmp Offset: 0x4) PWM Comparison 0 Value Update Register */
  RwReg      PWM_CMPM;      /**< \brief (PwmCmp Offset: 0x8) PWM Comparison 0 Mode Register */
  RwReg      PWM_CMPMUPD;   /**< \brief (PwmCmp Offset: 0xC) PWM Comparison 0 Mode Update Register */
} PwmCmp;
/** \brief Pwm hardware registers */
#define PWMCMP_NUMBER 8
#define PWMCH_NUM_NUMBER 8
typedef struct {
  RwReg      PWM_CLK;       /**< \brief (Pwm Offset: 0x00) PWM Clock Register */
  WoReg      PWM_ENA;       /**< \brief (Pwm Offset: 0x04) PWM Enable Register */
  WoReg      PWM_DIS;       /**< \brief (Pwm Offset: 0x08) PWM Disable Register */
  RoReg      PWM_SR;        /**< \brief (Pwm Offset: 0x0C) PWM Status Register */
  WoReg      PWM_IER1;      /**< \brief (Pwm Offset: 0x10) PWM Interrupt Enable Register 1 */
  WoReg      PWM_IDR1;      /**< \brief (Pwm Offset: 0x14) PWM Interrupt Disable Register 1 */
  RoReg      PWM_IMR1;      /**< \brief (Pwm Offset: 0x18) PWM Interrupt Mask Register 1 */
  RoReg      PWM_ISR1;      /**< \brief (Pwm Offset: 0x1C) PWM Interrupt Status Register 1 */
  RwReg      PWM_SCM;       /**< \brief (Pwm Offset: 0x20) PWM Sync Channels Mode Register */
  RoReg      Reserved1[1];
  RwReg      PWM_SCUC;      /**< \brief (Pwm Offset: 0x28) PWM Sync Channels Update Control Register */
  RwReg      PWM_SCUP;      /**< \brief (Pwm Offset: 0x2C) PWM Sync Channels Update Period Register */
  WoReg      PWM_SCUPUPD;   /**< \brief (Pwm Offset: 0x30) PWM Sync Channels Update Period Update Register */
  WoReg      PWM_IER2;      /**< \brief (Pwm Offset: 0x34) PWM Interrupt Enable Register 2 */
  WoReg      PWM_IDR2;      /**< \brief (Pwm Offset: 0x38) PWM Interrupt Disable Register 2 */
  RoReg      PWM_IMR2;      /**< \brief (Pwm Offset: 0x3C) PWM Interrupt Mask Register 2 */
  RoReg      PWM_ISR2;      /**< \brief (Pwm Offset: 0x40) PWM Interrupt Status Register 2 */
  RwReg      PWM_OOV;       /**< \brief (Pwm Offset: 0x44) PWM Output Override Value Register */
  RwReg      PWM_OS;        /**< \brief (Pwm Offset: 0x48) PWM Output Selection Register */
  WoReg      PWM_OSS;       /**< \brief (Pwm Offset: 0x4C) PWM Output Selection Set Register */
  WoReg      PWM_OSC;       /**< \brief (Pwm Offset: 0x50) PWM Output Selection Clear Register */
  WoReg      PWM_OSSUPD;    /**< \brief (Pwm Offset: 0x54) PWM Output Selection Set Update Register */
  WoReg      PWM_OSCUPD;    /**< \brief (Pwm Offset: 0x58) PWM Output Selection Clear Update Register */
  RwReg      PWM_FMR;       /**< \brief (Pwm Offset: 0x5C) PWM Fault Mode Register */
  RoReg      PWM_FSR;       /**< \brief (Pwm Offset: 0x60) PWM Fault Status Register */
  WoReg      PWM_FCR;       /**< \brief (Pwm Offset: 0x64) PWM Fault Clear Register */
  RwReg      PWM_FPV;       /**< \brief (Pwm Offset: 0x68) PWM Fault Protection Value Register */
  RwReg      PWM_FPE1;      /**< \brief (Pwm Offset: 0x6C) PWM Fault Protection Enable Register 1 */
  RwReg      PWM_FPE2;      /**< \brief (Pwm Offset: 0x70) PWM Fault Protection Enable Register 2 */
  RoReg      Reserved2[2];
  RwReg      PWM_ELMR[2];   /**< \brief (Pwm Offset: 0x7C) PWM Event Line 0 Mode Register */
  RoReg      Reserved3[11];
  RwReg      PWM_SMMR;      /**< \brief (Pwm Offset: 0xB0) PWM Stepper Motor Mode Register */
  RoReg      Reserved4[12];
  WoReg      PWM_WPCR;      /**< \brief (Pwm Offset: 0xE4) PWM Write Protect Control Register */
  RoReg      PWM_WPSR;      /**< \brief (Pwm Offset: 0xE8) PWM Write Protect Status Register */
  RoReg      Reserved5[7];
  RwReg      PWM_TPR;       /**< \brief (Pwm Offset: 0x108) Transmit Pointer Register */
  RwReg      PWM_TCR;       /**< \brief (Pwm Offset: 0x10C) Transmit Counter Register */
  RoReg      Reserved6[2];
  RwReg      PWM_TNPR;      /**< \brief (Pwm Offset: 0x118) Transmit Next Pointer Register */
  RwReg      PWM_TNCR;      /**< \brief (Pwm Offset: 0x11C) Transmit Next Counter Register */
  WoReg      PWM_PTCR;      /**< \brief (Pwm Offset: 0x120) Transfer Control Register */
  RoReg      PWM_PTSR;      /**< \brief (Pwm Offset: 0x124) Transfer Status Register */
  RoReg      Reserved7[2];
  PwmCmp     PWM_CMP[PWMCMP_NUMBER]; /**< \brief (Pwm Offset: 0x130) 0 .. 7 */
  RoReg      Reserved8[20];
  PwmCh_num  PWM_CH_NUM[PWMCH_NUM_NUMBER]; /**< \brief (Pwm Offset: 0x200) ch_num = 0 .. 7 */
} Pwm;
#define PWM        ((Pwm    *)0x40094000U) /**< \brief (PWM       ) Base Address */


/* -------- PWM_CLK : (PWM Offset: 0x00) PWM Clock Register -------- */
#define PWM_CLK_DIVA_Pos 0
#define PWM_CLK_DIVA_Msk (0xffu << PWM_CLK_DIVA_Pos) /**< \brief (PWM_CLK) CLKA, CLKB Divide Factor */
#define PWM_CLK_DIVA(value) ((PWM_CLK_DIVA_Msk & ((value) << PWM_CLK_DIVA_Pos)))
#define PWM_CLK_PREA_Pos 8
#define PWM_CLK_PREA_Msk (0xfu << PWM_CLK_PREA_Pos) /**< \brief (PWM_CLK) CLKA, CLKB Source Clock Selection */
#define PWM_CLK_PREA(value) ((PWM_CLK_PREA_Msk & ((value) << PWM_CLK_PREA_Pos)))
#define PWM_CLK_DIVB_Pos 16
#define PWM_CLK_DIVB_Msk (0xffu << PWM_CLK_DIVB_Pos) /**< \brief (PWM_CLK) CLKA, CLKB Divide Factor */
#define PWM_CLK_DIVB(value) ((PWM_CLK_DIVB_Msk & ((value) << PWM_CLK_DIVB_Pos)))
#define PWM_CLK_PREB_Pos 24
#define PWM_CLK_PREB_Msk (0xfu << PWM_CLK_PREB_Pos) /**< \brief (PWM_CLK) CLKA, CLKB Source Clock Selection */
#define PWM_CLK_PREB(value) ((PWM_CLK_PREB_Msk & ((value) << PWM_CLK_PREB_Pos)))
/* -------- PWM_ENA : (PWM Offset: 0x04) PWM Enable Register -------- */
#define PWM_ENA_CHID0 (0x1u << 0) /**< \brief (PWM_ENA) Channel ID */
#define PWM_ENA_CHID1 (0x1u << 1) /**< \brief (PWM_ENA) Channel ID */
#define PWM_ENA_CHID2 (0x1u << 2) /**< \brief (PWM_ENA) Channel ID */
#define PWM_ENA_CHID3 (0x1u << 3) /**< \brief (PWM_ENA) Channel ID */
#define PWM_ENA_CHID4 (0x1u << 4) /**< \brief (PWM_ENA) Channel ID */
#define PWM_ENA_CHID5 (0x1u << 5) /**< \brief (PWM_ENA) Channel ID */
#define PWM_ENA_CHID6 (0x1u << 6) /**< \brief (PWM_ENA) Channel ID */
#define PWM_ENA_CHID7 (0x1u << 7) /**< \brief (PWM_ENA) Channel ID */
/* -------- PWM_DIS : (PWM Offset: 0x08) PWM Disable Register -------- */
#define PWM_DIS_CHID0 (0x1u << 0) /**< \brief (PWM_DIS) Channel ID */
#define PWM_DIS_CHID1 (0x1u << 1) /**< \brief (PWM_DIS) Channel ID */
#define PWM_DIS_CHID2 (0x1u << 2) /**< \brief (PWM_DIS) Channel ID */
#define PWM_DIS_CHID3 (0x1u << 3) /**< \brief (PWM_DIS) Channel ID */
#define PWM_DIS_CHID4 (0x1u << 4) /**< \brief (PWM_DIS) Channel ID */
#define PWM_DIS_CHID5 (0x1u << 5) /**< \brief (PWM_DIS) Channel ID */
#define PWM_DIS_CHID6 (0x1u << 6) /**< \brief (PWM_DIS) Channel ID */
#define PWM_DIS_CHID7 (0x1u << 7) /**< \brief (PWM_DIS) Channel ID */
/* -------- PWM_SR : (PWM Offset: 0x0C) PWM Status Register -------- */
#define PWM_SR_CHID0 (0x1u << 0) /**< \brief (PWM_SR) Channel ID */
#define PWM_SR_CHID1 (0x1u << 1) /**< \brief (PWM_SR) Channel ID */
#define PWM_SR_CHID2 (0x1u << 2) /**< \brief (PWM_SR) Channel ID */
#define PWM_SR_CHID3 (0x1u << 3) /**< \brief (PWM_SR) Channel ID */
#define PWM_SR_CHID4 (0x1u << 4) /**< \brief (PWM_SR) Channel ID */
#define PWM_SR_CHID5 (0x1u << 5) /**< \brief (PWM_SR) Channel ID */
#define PWM_SR_CHID6 (0x1u << 6) /**< \brief (PWM_SR) Channel ID */
#define PWM_SR_CHID7 (0x1u << 7) /**< \brief (PWM_SR) Channel ID */
/* -------- PWM_IER1 : (PWM Offset: 0x10) PWM Interrupt Enable Register 1 -------- */
#define PWM_IER1_CHID0 (0x1u << 0) /**< \brief (PWM_IER1) Counter Event on Channel 0 Interrupt Enable */
#define PWM_IER1_CHID1 (0x1u << 1) /**< \brief (PWM_IER1) Counter Event on Channel 1 Interrupt Enable */
#define PWM_IER1_CHID2 (0x1u << 2) /**< \brief (PWM_IER1) Counter Event on Channel 2 Interrupt Enable */
#define PWM_IER1_CHID3 (0x1u << 3) /**< \brief (PWM_IER1) Counter Event on Channel 3 Interrupt Enable */
#define PWM_IER1_CHID4 (0x1u << 4) /**< \brief (PWM_IER1) Counter Event on Channel 4 Interrupt Enable */
#define PWM_IER1_CHID5 (0x1u << 5) /**< \brief (PWM_IER1) Counter Event on Channel 5 Interrupt Enable */
#define PWM_IER1_CHID6 (0x1u << 6) /**< \brief (PWM_IER1) Counter Event on Channel 6 Interrupt Enable */
#define PWM_IER1_CHID7 (0x1u << 7) /**< \brief (PWM_IER1) Counter Event on Channel 7 Interrupt Enable */
#define PWM_IER1_FCHID0 (0x1u << 16) /**< \brief (PWM_IER1) Fault Protection Trigger on Channel 0 Interrupt Enable */
#define PWM_IER1_FCHID1 (0x1u << 17) /**< \brief (PWM_IER1) Fault Protection Trigger on Channel 1 Interrupt Enable */
#define PWM_IER1_FCHID2 (0x1u << 18) /**< \brief (PWM_IER1) Fault Protection Trigger on Channel 2 Interrupt Enable */
#define PWM_IER1_FCHID3 (0x1u << 19) /**< \brief (PWM_IER1) Fault Protection Trigger on Channel 3 Interrupt Enable */
#define PWM_IER1_FCHID4 (0x1u << 20) /**< \brief (PWM_IER1) Fault Protection Trigger on Channel 4 Interrupt Enable */
#define PWM_IER1_FCHID5 (0x1u << 21) /**< \brief (PWM_IER1) Fault Protection Trigger on Channel 5 Interrupt Enable */
#define PWM_IER1_FCHID6 (0x1u << 22) /**< \brief (PWM_IER1) Fault Protection Trigger on Channel 6 Interrupt Enable */
#define PWM_IER1_FCHID7 (0x1u << 23) /**< \brief (PWM_IER1) Fault Protection Trigger on Channel 7 Interrupt Enable */
/* -------- PWM_IDR1 : (PWM Offset: 0x14) PWM Interrupt Disable Register 1 -------- */
#define PWM_IDR1_CHID0 (0x1u << 0) /**< \brief (PWM_IDR1) Counter Event on Channel 0 Interrupt Disable */
#define PWM_IDR1_CHID1 (0x1u << 1) /**< \brief (PWM_IDR1) Counter Event on Channel 1 Interrupt Disable */
#define PWM_IDR1_CHID2 (0x1u << 2) /**< \brief (PWM_IDR1) Counter Event on Channel 2 Interrupt Disable */
#define PWM_IDR1_CHID3 (0x1u << 3) /**< \brief (PWM_IDR1) Counter Event on Channel 3 Interrupt Disable */
#define PWM_IDR1_CHID4 (0x1u << 4) /**< \brief (PWM_IDR1) Counter Event on Channel 4 Interrupt Disable */
#define PWM_IDR1_CHID5 (0x1u << 5) /**< \brief (PWM_IDR1) Counter Event on Channel 5 Interrupt Disable */
#define PWM_IDR1_CHID6 (0x1u << 6) /**< \brief (PWM_IDR1) Counter Event on Channel 6 Interrupt Disable */
#define PWM_IDR1_CHID7 (0x1u << 7) /**< \brief (PWM_IDR1) Counter Event on Channel 7 Interrupt Disable */
#define PWM_IDR1_FCHID0 (0x1u << 16) /**< \brief (PWM_IDR1) Fault Protection Trigger on Channel 0 Interrupt Disable */
#define PWM_IDR1_FCHID1 (0x1u << 17) /**< \brief (PWM_IDR1) Fault Protection Trigger on Channel 1 Interrupt Disable */
#define PWM_IDR1_FCHID2 (0x1u << 18) /**< \brief (PWM_IDR1) Fault Protection Trigger on Channel 2 Interrupt Disable */
#define PWM_IDR1_FCHID3 (0x1u << 19) /**< \brief (PWM_IDR1) Fault Protection Trigger on Channel 3 Interrupt Disable */
#define PWM_IDR1_FCHID4 (0x1u << 20) /**< \brief (PWM_IDR1) Fault Protection Trigger on Channel 4 Interrupt Disable */
#define PWM_IDR1_FCHID5 (0x1u << 21) /**< \brief (PWM_IDR1) Fault Protection Trigger on Channel 5 Interrupt Disable */
#define PWM_IDR1_FCHID6 (0x1u << 22) /**< \brief (PWM_IDR1) Fault Protection Trigger on Channel 6 Interrupt Disable */
#define PWM_IDR1_FCHID7 (0x1u << 23) /**< \brief (PWM_IDR1) Fault Protection Trigger on Channel 7 Interrupt Disable */
/* -------- PWM_IMR1 : (PWM Offset: 0x18) PWM Interrupt Mask Register 1 -------- */
#define PWM_IMR1_CHID0 (0x1u << 0) /**< \brief (PWM_IMR1) Counter Event on Channel 0 Interrupt Mask */
#define PWM_IMR1_CHID1 (0x1u << 1) /**< \brief (PWM_IMR1) Counter Event on Channel 1 Interrupt Mask */
#define PWM_IMR1_CHID2 (0x1u << 2) /**< \brief (PWM_IMR1) Counter Event on Channel 2 Interrupt Mask */
#define PWM_IMR1_CHID3 (0x1u << 3) /**< \brief (PWM_IMR1) Counter Event on Channel 3 Interrupt Mask */
#define PWM_IMR1_CHID4 (0x1u << 4) /**< \brief (PWM_IMR1) Counter Event on Channel 4 Interrupt Mask */
#define PWM_IMR1_CHID5 (0x1u << 5) /**< \brief (PWM_IMR1) Counter Event on Channel 5 Interrupt Mask */
#define PWM_IMR1_CHID6 (0x1u << 6) /**< \brief (PWM_IMR1) Counter Event on Channel 6 Interrupt Mask */
#define PWM_IMR1_CHID7 (0x1u << 7) /**< \brief (PWM_IMR1) Counter Event on Channel 7 Interrupt Mask */
#define PWM_IMR1_FCHID0 (0x1u << 16) /**< \brief (PWM_IMR1) Fault Protection Trigger on Channel 0 Interrupt Mask */
#define PWM_IMR1_FCHID1 (0x1u << 17) /**< \brief (PWM_IMR1) Fault Protection Trigger on Channel 1 Interrupt Mask */
#define PWM_IMR1_FCHID2 (0x1u << 18) /**< \brief (PWM_IMR1) Fault Protection Trigger on Channel 2 Interrupt Mask */
#define PWM_IMR1_FCHID3 (0x1u << 19) /**< \brief (PWM_IMR1) Fault Protection Trigger on Channel 3 Interrupt Mask */
#define PWM_IMR1_FCHID4 (0x1u << 20) /**< \brief (PWM_IMR1) Fault Protection Trigger on Channel 4 Interrupt Mask */
#define PWM_IMR1_FCHID5 (0x1u << 21) /**< \brief (PWM_IMR1) Fault Protection Trigger on Channel 5 Interrupt Mask */
#define PWM_IMR1_FCHID6 (0x1u << 22) /**< \brief (PWM_IMR1) Fault Protection Trigger on Channel 6 Interrupt Mask */
#define PWM_IMR1_FCHID7 (0x1u << 23) /**< \brief (PWM_IMR1) Fault Protection Trigger on Channel 7 Interrupt Mask */
/* -------- PWM_ISR1 : (PWM Offset: 0x1C) PWM Interrupt Status Register 1 -------- */
#define PWM_ISR1_CHID0 (0x1u << 0) /**< \brief (PWM_ISR1) Counter Event on Channel 0 */
#define PWM_ISR1_CHID1 (0x1u << 1) /**< \brief (PWM_ISR1) Counter Event on Channel 1 */
#define PWM_ISR1_CHID2 (0x1u << 2) /**< \brief (PWM_ISR1) Counter Event on Channel 2 */
#define PWM_ISR1_CHID3 (0x1u << 3) /**< \brief (PWM_ISR1) Counter Event on Channel 3 */
#define PWM_ISR1_CHID4 (0x1u << 4) /**< \brief (PWM_ISR1) Counter Event on Channel 4 */
#define PWM_ISR1_CHID5 (0x1u << 5) /**< \brief (PWM_ISR1) Counter Event on Channel 5 */
#define PWM_ISR1_CHID6 (0x1u << 6) /**< \brief (PWM_ISR1) Counter Event on Channel 6 */
#define PWM_ISR1_CHID7 (0x1u << 7) /**< \brief (PWM_ISR1) Counter Event on Channel 7 */
#define PWM_ISR1_FCHID0 (0x1u << 16) /**< \brief (PWM_ISR1) Fault Protection Trigger on Channel 0 */
#define PWM_ISR1_FCHID1 (0x1u << 17) /**< \brief (PWM_ISR1) Fault Protection Trigger on Channel 1 */
#define PWM_ISR1_FCHID2 (0x1u << 18) /**< \brief (PWM_ISR1) Fault Protection Trigger on Channel 2 */
#define PWM_ISR1_FCHID3 (0x1u << 19) /**< \brief (PWM_ISR1) Fault Protection Trigger on Channel 3 */
#define PWM_ISR1_FCHID4 (0x1u << 20) /**< \brief (PWM_ISR1) Fault Protection Trigger on Channel 4 */
#define PWM_ISR1_FCHID5 (0x1u << 21) /**< \brief (PWM_ISR1) Fault Protection Trigger on Channel 5 */
#define PWM_ISR1_FCHID6 (0x1u << 22) /**< \brief (PWM_ISR1) Fault Protection Trigger on Channel 6 */
#define PWM_ISR1_FCHID7 (0x1u << 23) /**< \brief (PWM_ISR1) Fault Protection Trigger on Channel 7 */
/* -------- PWM_SCM : (PWM Offset: 0x20) PWM Sync Channels Mode Register -------- */
#define PWM_SCM_SYNC0 (0x1u << 0) /**< \brief (PWM_SCM) Synchronous Channel 0 */
#define PWM_SCM_SYNC1 (0x1u << 1) /**< \brief (PWM_SCM) Synchronous Channel 1 */
#define PWM_SCM_SYNC2 (0x1u << 2) /**< \brief (PWM_SCM) Synchronous Channel 2 */
#define PWM_SCM_SYNC3 (0x1u << 3) /**< \brief (PWM_SCM) Synchronous Channel 3 */
#define PWM_SCM_SYNC4 (0x1u << 4) /**< \brief (PWM_SCM) Synchronous Channel 4 */
#define PWM_SCM_SYNC5 (0x1u << 5) /**< \brief (PWM_SCM) Synchronous Channel 5 */
#define PWM_SCM_SYNC6 (0x1u << 6) /**< \brief (PWM_SCM) Synchronous Channel 6 */
#define PWM_SCM_SYNC7 (0x1u << 7) /**< \brief (PWM_SCM) Synchronous Channel 7 */
#define PWM_SCM_UPDM_Pos 16
#define PWM_SCM_UPDM_Msk (0x3u << PWM_SCM_UPDM_Pos) /**< \brief (PWM_SCM) Synchronous Channels Update Mode */
#define   PWM_SCM_UPDM_MODE0 (0x0u << 16) /**< \brief (PWM_SCM) Manual write of double buffer registers and manual update of synchronous channels */
#define   PWM_SCM_UPDM_MODE1 (0x1u << 16) /**< \brief (PWM_SCM) Manual write of double buffer registers and automatic update of synchronous channels */
#define   PWM_SCM_UPDM_MODE2 (0x2u << 16) /**< \brief (PWM_SCM) Automatic write of duty-cycle update registers by the PDC and automatic update of synchronous channels */
#define PWM_SCM_PTRM (0x1u << 20) /**< \brief (PWM_SCM) PDC Transfer Request Mode */
#define PWM_SCM_PTRCS_Pos 21
#define PWM_SCM_PTRCS_Msk (0x7u << PWM_SCM_PTRCS_Pos) /**< \brief (PWM_SCM) PDC Transfer Request Comparison Selection */
#define PWM_SCM_PTRCS(value) ((PWM_SCM_PTRCS_Msk & ((value) << PWM_SCM_PTRCS_Pos)))
/* -------- PWM_SCUC : (PWM Offset: 0x28) PWM Sync Channels Update Control Register -------- */
#define PWM_SCUC_UPDULOCK (0x1u << 0) /**< \brief (PWM_SCUC) Synchronous Channels Update Unlock */
/* -------- PWM_SCUP : (PWM Offset: 0x2C) PWM Sync Channels Update Period Register -------- */
#define PWM_SCUP_UPR_Pos 0
#define PWM_SCUP_UPR_Msk (0xfu << PWM_SCUP_UPR_Pos) /**< \brief (PWM_SCUP) Update Period */
#define PWM_SCUP_UPR(value) ((PWM_SCUP_UPR_Msk & ((value) << PWM_SCUP_UPR_Pos)))
#define PWM_SCUP_UPRCNT_Pos 4
#define PWM_SCUP_UPRCNT_Msk (0xfu << PWM_SCUP_UPRCNT_Pos) /**< \brief (PWM_SCUP) Update Period Counter */
#define PWM_SCUP_UPRCNT(value) ((PWM_SCUP_UPRCNT_Msk & ((value) << PWM_SCUP_UPRCNT_Pos)))
/* -------- PWM_SCUPUPD : (PWM Offset: 0x30) PWM Sync Channels Update Period Update Register -------- */
#define PWM_SCUPUPD_UPRUPD_Pos 0
#define PWM_SCUPUPD_UPRUPD_Msk (0xfu << PWM_SCUPUPD_UPRUPD_Pos) /**< \brief (PWM_SCUPUPD) Update Period Update */
#define PWM_SCUPUPD_UPRUPD(value) ((PWM_SCUPUPD_UPRUPD_Msk & ((value) << PWM_SCUPUPD_UPRUPD_Pos)))
/* -------- PWM_IER2 : (PWM Offset: 0x34) PWM Interrupt Enable Register 2 -------- */
#define PWM_IER2_WRDY (0x1u << 0) /**< \brief (PWM_IER2) Write Ready for Synchronous Channels Update Interrupt Enable */
#define PWM_IER2_ENDTX (0x1u << 1) /**< \brief (PWM_IER2) PDC End of TX Buffer Interrupt Enable */
#define PWM_IER2_TXBUFE (0x1u << 2) /**< \brief (PWM_IER2) PDC TX Buffer Empty Interrupt Enable */
#define PWM_IER2_UNRE (0x1u << 3) /**< \brief (PWM_IER2) Synchronous Channels Update Underrun Error Interrupt Enable */
#define PWM_IER2_CMPM0 (0x1u << 8) /**< \brief (PWM_IER2) Comparison 0 Match Interrupt Enable */
#define PWM_IER2_CMPM1 (0x1u << 9) /**< \brief (PWM_IER2) Comparison 1 Match Interrupt Enable */
#define PWM_IER2_CMPM2 (0x1u << 10) /**< \brief (PWM_IER2) Comparison 2 Match Interrupt Enable */
#define PWM_IER2_CMPM3 (0x1u << 11) /**< \brief (PWM_IER2) Comparison 3 Match Interrupt Enable */
#define PWM_IER2_CMPM4 (0x1u << 12) /**< \brief (PWM_IER2) Comparison 4 Match Interrupt Enable */
#define PWM_IER2_CMPM5 (0x1u << 13) /**< \brief (PWM_IER2) Comparison 5 Match Interrupt Enable */
#define PWM_IER2_CMPM6 (0x1u << 14) /**< \brief (PWM_IER2) Comparison 6 Match Interrupt Enable */
#define PWM_IER2_CMPM7 (0x1u << 15) /**< \brief (PWM_IER2) Comparison 7 Match Interrupt Enable */
#define PWM_IER2_CMPU0 (0x1u << 16) /**< \brief (PWM_IER2) Comparison 0 Update Interrupt Enable */
#define PWM_IER2_CMPU1 (0x1u << 17) /**< \brief (PWM_IER2) Comparison 1 Update Interrupt Enable */
#define PWM_IER2_CMPU2 (0x1u << 18) /**< \brief (PWM_IER2) Comparison 2 Update Interrupt Enable */
#define PWM_IER2_CMPU3 (0x1u << 19) /**< \brief (PWM_IER2) Comparison 3 Update Interrupt Enable */
#define PWM_IER2_CMPU4 (0x1u << 20) /**< \brief (PWM_IER2) Comparison 4 Update Interrupt Enable */
#define PWM_IER2_CMPU5 (0x1u << 21) /**< \brief (PWM_IER2) Comparison 5 Update Interrupt Enable */
#define PWM_IER2_CMPU6 (0x1u << 22) /**< \brief (PWM_IER2) Comparison 6 Update Interrupt Enable */
#define PWM_IER2_CMPU7 (0x1u << 23) /**< \brief (PWM_IER2) Comparison 7 Update Interrupt Enable */
/* -------- PWM_IDR2 : (PWM Offset: 0x38) PWM Interrupt Disable Register 2 -------- */
#define PWM_IDR2_WRDY (0x1u << 0) /**< \brief (PWM_IDR2) Write Ready for Synchronous Channels Update Interrupt Disable */
#define PWM_IDR2_ENDTX (0x1u << 1) /**< \brief (PWM_IDR2) PDC End of TX Buffer Interrupt Disable */
#define PWM_IDR2_TXBUFE (0x1u << 2) /**< \brief (PWM_IDR2) PDC TX Buffer Empty Interrupt Disable */
#define PWM_IDR2_UNRE (0x1u << 3) /**< \brief (PWM_IDR2) Synchronous Channels Update Underrun Error Interrupt Disable */
#define PWM_IDR2_CMPM0 (0x1u << 8) /**< \brief (PWM_IDR2) Comparison 0 Match Interrupt Disable */
#define PWM_IDR2_CMPM1 (0x1u << 9) /**< \brief (PWM_IDR2) Comparison 1 Match Interrupt Disable */
#define PWM_IDR2_CMPM2 (0x1u << 10) /**< \brief (PWM_IDR2) Comparison 2 Match Interrupt Disable */
#define PWM_IDR2_CMPM3 (0x1u << 11) /**< \brief (PWM_IDR2) Comparison 3 Match Interrupt Disable */
#define PWM_IDR2_CMPM4 (0x1u << 12) /**< \brief (PWM_IDR2) Comparison 4 Match Interrupt Disable */
#define PWM_IDR2_CMPM5 (0x1u << 13) /**< \brief (PWM_IDR2) Comparison 5 Match Interrupt Disable */
#define PWM_IDR2_CMPM6 (0x1u << 14) /**< \brief (PWM_IDR2) Comparison 6 Match Interrupt Disable */
#define PWM_IDR2_CMPM7 (0x1u << 15) /**< \brief (PWM_IDR2) Comparison 7 Match Interrupt Disable */
#define PWM_IDR2_CMPU0 (0x1u << 16) /**< \brief (PWM_IDR2) Comparison 0 Update Interrupt Disable */
#define PWM_IDR2_CMPU1 (0x1u << 17) /**< \brief (PWM_IDR2) Comparison 1 Update Interrupt Disable */
#define PWM_IDR2_CMPU2 (0x1u << 18) /**< \brief (PWM_IDR2) Comparison 2 Update Interrupt Disable */
#define PWM_IDR2_CMPU3 (0x1u << 19) /**< \brief (PWM_IDR2) Comparison 3 Update Interrupt Disable */
#define PWM_IDR2_CMPU4 (0x1u << 20) /**< \brief (PWM_IDR2) Comparison 4 Update Interrupt Disable */
#define PWM_IDR2_CMPU5 (0x1u << 21) /**< \brief (PWM_IDR2) Comparison 5 Update Interrupt Disable */
#define PWM_IDR2_CMPU6 (0x1u << 22) /**< \brief (PWM_IDR2) Comparison 6 Update Interrupt Disable */
#define PWM_IDR2_CMPU7 (0x1u << 23) /**< \brief (PWM_IDR2) Comparison 7 Update Interrupt Disable */
/* -------- PWM_IMR2 : (PWM Offset: 0x3C) PWM Interrupt Mask Register 2 -------- */
#define PWM_IMR2_WRDY (0x1u << 0) /**< \brief (PWM_IMR2) Write Ready for Synchronous Channels Update Interrupt Mask */
#define PWM_IMR2_ENDTX (0x1u << 1) /**< \brief (PWM_IMR2) PDC End of TX Buffer Interrupt Mask */
#define PWM_IMR2_TXBUFE (0x1u << 2) /**< \brief (PWM_IMR2) PDC TX Buffer Empty Interrupt Mask */
#define PWM_IMR2_UNRE (0x1u << 3) /**< \brief (PWM_IMR2) Synchronous Channels Update Underrun Error Interrupt Mask */
#define PWM_IMR2_CMPM0 (0x1u << 8) /**< \brief (PWM_IMR2) Comparison 0 Match Interrupt Mask */
#define PWM_IMR2_CMPM1 (0x1u << 9) /**< \brief (PWM_IMR2) Comparison 1 Match Interrupt Mask */
#define PWM_IMR2_CMPM2 (0x1u << 10) /**< \brief (PWM_IMR2) Comparison 2 Match Interrupt Mask */
#define PWM_IMR2_CMPM3 (0x1u << 11) /**< \brief (PWM_IMR2) Comparison 3 Match Interrupt Mask */
#define PWM_IMR2_CMPM4 (0x1u << 12) /**< \brief (PWM_IMR2) Comparison 4 Match Interrupt Mask */
#define PWM_IMR2_CMPM5 (0x1u << 13) /**< \brief (PWM_IMR2) Comparison 5 Match Interrupt Mask */
#define PWM_IMR2_CMPM6 (0x1u << 14) /**< \brief (PWM_IMR2) Comparison 6 Match Interrupt Mask */
#define PWM_IMR2_CMPM7 (0x1u << 15) /**< \brief (PWM_IMR2) Comparison 7 Match Interrupt Mask */
#define PWM_IMR2_CMPU0 (0x1u << 16) /**< \brief (PWM_IMR2) Comparison 0 Update Interrupt Mask */
#define PWM_IMR2_CMPU1 (0x1u << 17) /**< \brief (PWM_IMR2) Comparison 1 Update Interrupt Mask */
#define PWM_IMR2_CMPU2 (0x1u << 18) /**< \brief (PWM_IMR2) Comparison 2 Update Interrupt Mask */
#define PWM_IMR2_CMPU3 (0x1u << 19) /**< \brief (PWM_IMR2) Comparison 3 Update Interrupt Mask */
#define PWM_IMR2_CMPU4 (0x1u << 20) /**< \brief (PWM_IMR2) Comparison 4 Update Interrupt Mask */
#define PWM_IMR2_CMPU5 (0x1u << 21) /**< \brief (PWM_IMR2) Comparison 5 Update Interrupt Mask */
#define PWM_IMR2_CMPU6 (0x1u << 22) /**< \brief (PWM_IMR2) Comparison 6 Update Interrupt Mask */
#define PWM_IMR2_CMPU7 (0x1u << 23) /**< \brief (PWM_IMR2) Comparison 7 Update Interrupt Mask */
/* -------- PWM_ISR2 : (PWM Offset: 0x40) PWM Interrupt Status Register 2 -------- */
#define PWM_ISR2_WRDY (0x1u << 0) /**< \brief (PWM_ISR2) Write Ready for Synchronous Channels Update */
#define PWM_ISR2_ENDTX (0x1u << 1) /**< \brief (PWM_ISR2) PDC End of TX Buffer */
#define PWM_ISR2_TXBUFE (0x1u << 2) /**< \brief (PWM_ISR2) PDC TX Buffer Empty */
#define PWM_ISR2_UNRE (0x1u << 3) /**< \brief (PWM_ISR2) Synchronous Channels Update Underrun Error */
#define PWM_ISR2_CMPM0 (0x1u << 8) /**< \brief (PWM_ISR2) Comparison 0 Match */
#define PWM_ISR2_CMPM1 (0x1u << 9) /**< \brief (PWM_ISR2) Comparison 1 Match */
#define PWM_ISR2_CMPM2 (0x1u << 10) /**< \brief (PWM_ISR2) Comparison 2 Match */
#define PWM_ISR2_CMPM3 (0x1u << 11) /**< \brief (PWM_ISR2) Comparison 3 Match */
#define PWM_ISR2_CMPM4 (0x1u << 12) /**< \brief (PWM_ISR2) Comparison 4 Match */
#define PWM_ISR2_CMPM5 (0x1u << 13) /**< \brief (PWM_ISR2) Comparison 5 Match */
#define PWM_ISR2_CMPM6 (0x1u << 14) /**< \brief (PWM_ISR2) Comparison 6 Match */
#define PWM_ISR2_CMPM7 (0x1u << 15) /**< \brief (PWM_ISR2) Comparison 7 Match */
#define PWM_ISR2_CMPU0 (0x1u << 16) /**< \brief (PWM_ISR2) Comparison 0 Update */
#define PWM_ISR2_CMPU1 (0x1u << 17) /**< \brief (PWM_ISR2) Comparison 1 Update */
#define PWM_ISR2_CMPU2 (0x1u << 18) /**< \brief (PWM_ISR2) Comparison 2 Update */
#define PWM_ISR2_CMPU3 (0x1u << 19) /**< \brief (PWM_ISR2) Comparison 3 Update */
#define PWM_ISR2_CMPU4 (0x1u << 20) /**< \brief (PWM_ISR2) Comparison 4 Update */
#define PWM_ISR2_CMPU5 (0x1u << 21) /**< \brief (PWM_ISR2) Comparison 5 Update */
#define PWM_ISR2_CMPU6 (0x1u << 22) /**< \brief (PWM_ISR2) Comparison 6 Update */
#define PWM_ISR2_CMPU7 (0x1u << 23) /**< \brief (PWM_ISR2) Comparison 7 Update */
/* -------- PWM_OOV : (PWM Offset: 0x44) PWM Output Override Value Register -------- */
#define PWM_OOV_OOVH0 (0x1u << 0) /**< \brief (PWM_OOV) Output Override Value for PWMH output of the channel 0 */
#define PWM_OOV_OOVH1 (0x1u << 1) /**< \brief (PWM_OOV) Output Override Value for PWMH output of the channel 1 */
#define PWM_OOV_OOVH2 (0x1u << 2) /**< \brief (PWM_OOV) Output Override Value for PWMH output of the channel 2 */
#define PWM_OOV_OOVH3 (0x1u << 3) /**< \brief (PWM_OOV) Output Override Value for PWMH output of the channel 3 */
#define PWM_OOV_OOVH4 (0x1u << 4) /**< \brief (PWM_OOV) Output Override Value for PWMH output of the channel 4 */
#define PWM_OOV_OOVH5 (0x1u << 5) /**< \brief (PWM_OOV) Output Override Value for PWMH output of the channel 5 */
#define PWM_OOV_OOVH6 (0x1u << 6) /**< \brief (PWM_OOV) Output Override Value for PWMH output of the channel 6 */
#define PWM_OOV_OOVH7 (0x1u << 7) /**< \brief (PWM_OOV) Output Override Value for PWMH output of the channel 7 */
#define PWM_OOV_OOVL0 (0x1u << 16) /**< \brief (PWM_OOV) Output Override Value for PWML output of the channel 0 */
#define PWM_OOV_OOVL1 (0x1u << 17) /**< \brief (PWM_OOV) Output Override Value for PWML output of the channel 1 */
#define PWM_OOV_OOVL2 (0x1u << 18) /**< \brief (PWM_OOV) Output Override Value for PWML output of the channel 2 */
#define PWM_OOV_OOVL3 (0x1u << 19) /**< \brief (PWM_OOV) Output Override Value for PWML output of the channel 3 */
#define PWM_OOV_OOVL4 (0x1u << 20) /**< \brief (PWM_OOV) Output Override Value for PWML output of the channel 4 */
#define PWM_OOV_OOVL5 (0x1u << 21) /**< \brief (PWM_OOV) Output Override Value for PWML output of the channel 5 */
#define PWM_OOV_OOVL6 (0x1u << 22) /**< \brief (PWM_OOV) Output Override Value for PWML output of the channel 6 */
#define PWM_OOV_OOVL7 (0x1u << 23) /**< \brief (PWM_OOV) Output Override Value for PWML output of the channel 7 */
/* -------- PWM_OS : (PWM Offset: 0x48) PWM Output Selection Register -------- */
#define PWM_OS_OSH0 (0x1u << 0) /**< \brief (PWM_OS) Output Selection for PWMH output of the channel 0 */
#define PWM_OS_OSH1 (0x1u << 1) /**< \brief (PWM_OS) Output Selection for PWMH output of the channel 1 */
#define PWM_OS_OSH2 (0x1u << 2) /**< \brief (PWM_OS) Output Selection for PWMH output of the channel 2 */
#define PWM_OS_OSH3 (0x1u << 3) /**< \brief (PWM_OS) Output Selection for PWMH output of the channel 3 */
#define PWM_OS_OSH4 (0x1u << 4) /**< \brief (PWM_OS) Output Selection for PWMH output of the channel 4 */
#define PWM_OS_OSH5 (0x1u << 5) /**< \brief (PWM_OS) Output Selection for PWMH output of the channel 5 */
#define PWM_OS_OSH6 (0x1u << 6) /**< \brief (PWM_OS) Output Selection for PWMH output of the channel 6 */
#define PWM_OS_OSH7 (0x1u << 7) /**< \brief (PWM_OS) Output Selection for PWMH output of the channel 7 */
#define PWM_OS_OSL0 (0x1u << 16) /**< \brief (PWM_OS) Output Selection for PWML output of the channel 0 */
#define PWM_OS_OSL1 (0x1u << 17) /**< \brief (PWM_OS) Output Selection for PWML output of the channel 1 */
#define PWM_OS_OSL2 (0x1u << 18) /**< \brief (PWM_OS) Output Selection for PWML output of the channel 2 */
#define PWM_OS_OSL3 (0x1u << 19) /**< \brief (PWM_OS) Output Selection for PWML output of the channel 3 */
#define PWM_OS_OSL4 (0x1u << 20) /**< \brief (PWM_OS) Output Selection for PWML output of the channel 4 */
#define PWM_OS_OSL5 (0x1u << 21) /**< \brief (PWM_OS) Output Selection for PWML output of the channel 5 */
#define PWM_OS_OSL6 (0x1u << 22) /**< \brief (PWM_OS) Output Selection for PWML output of the channel 6 */
#define PWM_OS_OSL7 (0x1u << 23) /**< \brief (PWM_OS) Output Selection for PWML output of the channel 7 */
/* -------- PWM_OSS : (PWM Offset: 0x4C) PWM Output Selection Set Register -------- */
#define PWM_OSS_OSSH0 (0x1u << 0) /**< \brief (PWM_OSS) Output Selection Set for PWMH output of the channel 0 */
#define PWM_OSS_OSSH1 (0x1u << 1) /**< \brief (PWM_OSS) Output Selection Set for PWMH output of the channel 1 */
#define PWM_OSS_OSSH2 (0x1u << 2) /**< \brief (PWM_OSS) Output Selection Set for PWMH output of the channel 2 */
#define PWM_OSS_OSSH3 (0x1u << 3) /**< \brief (PWM_OSS) Output Selection Set for PWMH output of the channel 3 */
#define PWM_OSS_OSSH4 (0x1u << 4) /**< \brief (PWM_OSS) Output Selection Set for PWMH output of the channel 4 */
#define PWM_OSS_OSSH5 (0x1u << 5) /**< \brief (PWM_OSS) Output Selection Set for PWMH output of the channel 5 */
#define PWM_OSS_OSSH6 (0x1u << 6) /**< \brief (PWM_OSS) Output Selection Set for PWMH output of the channel 6 */
#define PWM_OSS_OSSH7 (0x1u << 7) /**< \brief (PWM_OSS) Output Selection Set for PWMH output of the channel 7 */
#define PWM_OSS_OSSL0 (0x1u << 16) /**< \brief (PWM_OSS) Output Selection Set for PWML output of the channel 0 */
#define PWM_OSS_OSSL1 (0x1u << 17) /**< \brief (PWM_OSS) Output Selection Set for PWML output of the channel 1 */
#define PWM_OSS_OSSL2 (0x1u << 18) /**< \brief (PWM_OSS) Output Selection Set for PWML output of the channel 2 */
#define PWM_OSS_OSSL3 (0x1u << 19) /**< \brief (PWM_OSS) Output Selection Set for PWML output of the channel 3 */
#define PWM_OSS_OSSL4 (0x1u << 20) /**< \brief (PWM_OSS) Output Selection Set for PWML output of the channel 4 */
#define PWM_OSS_OSSL5 (0x1u << 21) /**< \brief (PWM_OSS) Output Selection Set for PWML output of the channel 5 */
#define PWM_OSS_OSSL6 (0x1u << 22) /**< \brief (PWM_OSS) Output Selection Set for PWML output of the channel 6 */
#define PWM_OSS_OSSL7 (0x1u << 23) /**< \brief (PWM_OSS) Output Selection Set for PWML output of the channel 7 */
/* -------- PWM_OSC : (PWM Offset: 0x50) PWM Output Selection Clear Register -------- */
#define PWM_OSC_OSCH0 (0x1u << 0) /**< \brief (PWM_OSC) Output Selection Clear for PWMH output of the channel 0 */
#define PWM_OSC_OSCH1 (0x1u << 1) /**< \brief (PWM_OSC) Output Selection Clear for PWMH output of the channel 1 */
#define PWM_OSC_OSCH2 (0x1u << 2) /**< \brief (PWM_OSC) Output Selection Clear for PWMH output of the channel 2 */
#define PWM_OSC_OSCH3 (0x1u << 3) /**< \brief (PWM_OSC) Output Selection Clear for PWMH output of the channel 3 */
#define PWM_OSC_OSCH4 (0x1u << 4) /**< \brief (PWM_OSC) Output Selection Clear for PWMH output of the channel 4 */
#define PWM_OSC_OSCH5 (0x1u << 5) /**< \brief (PWM_OSC) Output Selection Clear for PWMH output of the channel 5 */
#define PWM_OSC_OSCH6 (0x1u << 6) /**< \brief (PWM_OSC) Output Selection Clear for PWMH output of the channel 6 */
#define PWM_OSC_OSCH7 (0x1u << 7) /**< \brief (PWM_OSC) Output Selection Clear for PWMH output of the channel 7 */
#define PWM_OSC_OSCL0 (0x1u << 16) /**< \brief (PWM_OSC) Output Selection Clear for PWML output of the channel 0 */
#define PWM_OSC_OSCL1 (0x1u << 17) /**< \brief (PWM_OSC) Output Selection Clear for PWML output of the channel 1 */
#define PWM_OSC_OSCL2 (0x1u << 18) /**< \brief (PWM_OSC) Output Selection Clear for PWML output of the channel 2 */
#define PWM_OSC_OSCL3 (0x1u << 19) /**< \brief (PWM_OSC) Output Selection Clear for PWML output of the channel 3 */
#define PWM_OSC_OSCL4 (0x1u << 20) /**< \brief (PWM_OSC) Output Selection Clear for PWML output of the channel 4 */
#define PWM_OSC_OSCL5 (0x1u << 21) /**< \brief (PWM_OSC) Output Selection Clear for PWML output of the channel 5 */
#define PWM_OSC_OSCL6 (0x1u << 22) /**< \brief (PWM_OSC) Output Selection Clear for PWML output of the channel 6 */
#define PWM_OSC_OSCL7 (0x1u << 23) /**< \brief (PWM_OSC) Output Selection Clear for PWML output of the channel 7 */
/* -------- PWM_OSSUPD : (PWM Offset: 0x54) PWM Output Selection Set Update Register -------- */
#define PWM_OSSUPD_OSSUPH0 (0x1u << 0) /**< \brief (PWM_OSSUPD) Output Selection Set for PWMH output of the channel 0 */
#define PWM_OSSUPD_OSSUPH1 (0x1u << 1) /**< \brief (PWM_OSSUPD) Output Selection Set for PWMH output of the channel 1 */
#define PWM_OSSUPD_OSSUPH2 (0x1u << 2) /**< \brief (PWM_OSSUPD) Output Selection Set for PWMH output of the channel 2 */
#define PWM_OSSUPD_OSSUPH3 (0x1u << 3) /**< \brief (PWM_OSSUPD) Output Selection Set for PWMH output of the channel 3 */
#define PWM_OSSUPD_OSSUPH4 (0x1u << 4) /**< \brief (PWM_OSSUPD) Output Selection Set for PWMH output of the channel 4 */
#define PWM_OSSUPD_OSSUPH5 (0x1u << 5) /**< \brief (PWM_OSSUPD) Output Selection Set for PWMH output of the channel 5 */
#define PWM_OSSUPD_OSSUPH6 (0x1u << 6) /**< \brief (PWM_OSSUPD) Output Selection Set for PWMH output of the channel 6 */
#define PWM_OSSUPD_OSSUPH7 (0x1u << 7) /**< \brief (PWM_OSSUPD) Output Selection Set for PWMH output of the channel 7 */
#define PWM_OSSUPD_OSSUPL0 (0x1u << 16) /**< \brief (PWM_OSSUPD) Output Selection Set for PWML output of the channel 0 */
#define PWM_OSSUPD_OSSUPL1 (0x1u << 17) /**< \brief (PWM_OSSUPD) Output Selection Set for PWML output of the channel 1 */
#define PWM_OSSUPD_OSSUPL2 (0x1u << 18) /**< \brief (PWM_OSSUPD) Output Selection Set for PWML output of the channel 2 */
#define PWM_OSSUPD_OSSUPL3 (0x1u << 19) /**< \brief (PWM_OSSUPD) Output Selection Set for PWML output of the channel 3 */
#define PWM_OSSUPD_OSSUPL4 (0x1u << 20) /**< \brief (PWM_OSSUPD) Output Selection Set for PWML output of the channel 4 */
#define PWM_OSSUPD_OSSUPL5 (0x1u << 21) /**< \brief (PWM_OSSUPD) Output Selection Set for PWML output of the channel 5 */
#define PWM_OSSUPD_OSSUPL6 (0x1u << 22) /**< \brief (PWM_OSSUPD) Output Selection Set for PWML output of the channel 6 */
#define PWM_OSSUPD_OSSUPL7 (0x1u << 23) /**< \brief (PWM_OSSUPD) Output Selection Set for PWML output of the channel 7 */
/* -------- PWM_OSCUPD : (PWM Offset: 0x58) PWM Output Selection Clear Update Register -------- */
#define PWM_OSCUPD_OSCUPH0 (0x1u << 0) /**< \brief (PWM_OSCUPD) Output Selection Clear for PWMH output of the channel 0 */
#define PWM_OSCUPD_OSCUPH1 (0x1u << 1) /**< \brief (PWM_OSCUPD) Output Selection Clear for PWMH output of the channel 1 */
#define PWM_OSCUPD_OSCUPH2 (0x1u << 2) /**< \brief (PWM_OSCUPD) Output Selection Clear for PWMH output of the channel 2 */
#define PWM_OSCUPD_OSCUPH3 (0x1u << 3) /**< \brief (PWM_OSCUPD) Output Selection Clear for PWMH output of the channel 3 */
#define PWM_OSCUPD_OSCUPH4 (0x1u << 4) /**< \brief (PWM_OSCUPD) Output Selection Clear for PWMH output of the channel 4 */
#define PWM_OSCUPD_OSCUPH5 (0x1u << 5) /**< \brief (PWM_OSCUPD) Output Selection Clear for PWMH output of the channel 5 */
#define PWM_OSCUPD_OSCUPH6 (0x1u << 6) /**< \brief (PWM_OSCUPD) Output Selection Clear for PWMH output of the channel 6 */
#define PWM_OSCUPD_OSCUPH7 (0x1u << 7) /**< \brief (PWM_OSCUPD) Output Selection Clear for PWMH output of the channel 7 */
#define PWM_OSCUPD_OSCUPL0 (0x1u << 16) /**< \brief (PWM_OSCUPD) Output Selection Clear for PWML output of the channel 0 */
#define PWM_OSCUPD_OSCUPL1 (0x1u << 17) /**< \brief (PWM_OSCUPD) Output Selection Clear for PWML output of the channel 1 */
#define PWM_OSCUPD_OSCUPL2 (0x1u << 18) /**< \brief (PWM_OSCUPD) Output Selection Clear for PWML output of the channel 2 */
#define PWM_OSCUPD_OSCUPL3 (0x1u << 19) /**< \brief (PWM_OSCUPD) Output Selection Clear for PWML output of the channel 3 */
#define PWM_OSCUPD_OSCUPL4 (0x1u << 20) /**< \brief (PWM_OSCUPD) Output Selection Clear for PWML output of the channel 4 */
#define PWM_OSCUPD_OSCUPL5 (0x1u << 21) /**< \brief (PWM_OSCUPD) Output Selection Clear for PWML output of the channel 5 */
#define PWM_OSCUPD_OSCUPDL6 (0x1u << 22) /**< \brief (PWM_OSCUPD)  */
#define PWM_OSCUPD_OSCUPL7 (0x1u << 23) /**< \brief (PWM_OSCUPD) Output Selection Clear for PWML output of the channel 7 */
/* -------- PWM_FMR : (PWM Offset: 0x5C) PWM Fault Mode Register -------- */
#define PWM_FMR_FPOL_Pos 0
#define PWM_FMR_FPOL_Msk (0xffu << PWM_FMR_FPOL_Pos) /**< \brief (PWM_FMR) Fault Polarity (fault input bit varies from 0 to 5) */
#define PWM_FMR_FPOL(value) ((PWM_FMR_FPOL_Msk & ((value) << PWM_FMR_FPOL_Pos)))
#define PWM_FMR_FMOD_Pos 8
#define PWM_FMR_FMOD_Msk (0xffu << PWM_FMR_FMOD_Pos) /**< \brief (PWM_FMR) Fault Activation Mode (fault input bit varies from 0 to 5) */
#define PWM_FMR_FMOD(value) ((PWM_FMR_FMOD_Msk & ((value) << PWM_FMR_FMOD_Pos)))
#define PWM_FMR_FFIL_Pos 16
#define PWM_FMR_FFIL_Msk (0xffu << PWM_FMR_FFIL_Pos) /**< \brief (PWM_FMR) Fault Filtering (fault input bit varies from 0 to 5) */
#define PWM_FMR_FFIL(value) ((PWM_FMR_FFIL_Msk & ((value) << PWM_FMR_FFIL_Pos)))
/* -------- PWM_FSR : (PWM Offset: 0x60) PWM Fault Status Register -------- */
#define PWM_FSR_FIV_Pos 0
#define PWM_FSR_FIV_Msk (0xffu << PWM_FSR_FIV_Pos) /**< \brief (PWM_FSR) Fault Input Value (fault input bit varies from 0 to 5) */
#define PWM_FSR_FS_Pos 8
#define PWM_FSR_FS_Msk (0xffu << PWM_FSR_FS_Pos) /**< \brief (PWM_FSR) Fault Status (fault input bit varies from 0 to 5) */
/* -------- PWM_FCR : (PWM Offset: 0x64) PWM Fault Clear Register -------- */
#define PWM_FCR_FCLR_Pos 0
#define PWM_FCR_FCLR_Msk (0xffu << PWM_FCR_FCLR_Pos) /**< \brief (PWM_FCR) Fault Clear (fault input bit varies from 0 to 5) */
#define PWM_FCR_FCLR(value) ((PWM_FCR_FCLR_Msk & ((value) << PWM_FCR_FCLR_Pos)))
/* -------- PWM_FPV : (PWM Offset: 0x68) PWM Fault Protection Value Register -------- */
#define PWM_FPV_FPVH0 (0x1u << 0) /**< \brief (PWM_FPV) Fault Protection Value for PWMH output on channel 0 */
#define PWM_FPV_FPVH1 (0x1u << 1) /**< \brief (PWM_FPV) Fault Protection Value for PWMH output on channel 1 */
#define PWM_FPV_FPVH2 (0x1u << 2) /**< \brief (PWM_FPV) Fault Protection Value for PWMH output on channel 2 */
#define PWM_FPV_FPVH3 (0x1u << 3) /**< \brief (PWM_FPV) Fault Protection Value for PWMH output on channel 3 */
#define PWM_FPV_FPVH4 (0x1u << 4) /**< \brief (PWM_FPV) Fault Protection Value for PWMH output on channel 4 */
#define PWM_FPV_FPVH5 (0x1u << 5) /**< \brief (PWM_FPV) Fault Protection Value for PWMH output on channel 5 */
#define PWM_FPV_FPVH6 (0x1u << 6) /**< \brief (PWM_FPV) Fault Protection Value for PWMH output on channel 6 */
#define PWM_FPV_FPVH7 (0x1u << 7) /**< \brief (PWM_FPV) Fault Protection Value for PWMH output on channel 7 */
#define PWM_FPV_FPVL0 (0x1u << 16) /**< \brief (PWM_FPV) Fault Protection Value for PWML output on channel 0 */
#define PWM_FPV_FPVL1 (0x1u << 17) /**< \brief (PWM_FPV) Fault Protection Value for PWML output on channel 1 */
#define PWM_FPV_FPVL2 (0x1u << 18) /**< \brief (PWM_FPV) Fault Protection Value for PWML output on channel 2 */
#define PWM_FPV_FPVL3 (0x1u << 19) /**< \brief (PWM_FPV) Fault Protection Value for PWML output on channel 3 */
#define PWM_FPV_FPVL4 (0x1u << 20) /**< \brief (PWM_FPV) Fault Protection Value for PWML output on channel 4 */
#define PWM_FPV_FPVL5 (0x1u << 21) /**< \brief (PWM_FPV) Fault Protection Value for PWML output on channel 5 */
#define PWM_FPV_FPVL6 (0x1u << 22) /**< \brief (PWM_FPV) Fault Protection Value for PWML output on channel 6 */
#define PWM_FPV_FPVL7 (0x1u << 23) /**< \brief (PWM_FPV) Fault Protection Value for PWML output on channel 7 */
/* -------- PWM_FPE1 : (PWM Offset: 0x6C) PWM Fault Protection Enable Register 1 -------- */
#define PWM_FPE1_FPE0_Pos 0
#define PWM_FPE1_FPE0_Msk (0xffu << PWM_FPE1_FPE0_Pos) /**< \brief (PWM_FPE1) Fault Protection Enable for channel 0 (fault input bit varies from 0 to 5) */
#define PWM_FPE1_FPE0(value) ((PWM_FPE1_FPE0_Msk & ((value) << PWM_FPE1_FPE0_Pos)))
#define PWM_FPE1_FPE1_Pos 8
#define PWM_FPE1_FPE1_Msk (0xffu << PWM_FPE1_FPE1_Pos) /**< \brief (PWM_FPE1) Fault Protection Enable for channel 1 (fault input bit varies from 0 to 5) */
#define PWM_FPE1_FPE1(value) ((PWM_FPE1_FPE1_Msk & ((value) << PWM_FPE1_FPE1_Pos)))
#define PWM_FPE1_FPE2_Pos 16
#define PWM_FPE1_FPE2_Msk (0xffu << PWM_FPE1_FPE2_Pos) /**< \brief (PWM_FPE1) Fault Protection Enable for channel 2 (fault input bit varies from 0 to 5) */
#define PWM_FPE1_FPE2(value) ((PWM_FPE1_FPE2_Msk & ((value) << PWM_FPE1_FPE2_Pos)))
#define PWM_FPE1_FPE3_Pos 24
#define PWM_FPE1_FPE3_Msk (0xffu << PWM_FPE1_FPE3_Pos) /**< \brief (PWM_FPE1) Fault Protection Enable for channel 3 (fault input bit varies from 0 to 5) */
#define PWM_FPE1_FPE3(value) ((PWM_FPE1_FPE3_Msk & ((value) << PWM_FPE1_FPE3_Pos)))
/* -------- PWM_FPE2 : (PWM Offset: 0x70) PWM Fault Protection Enable Register 2 -------- */
#define PWM_FPE2_FPE4_Pos 0
#define PWM_FPE2_FPE4_Msk (0xffu << PWM_FPE2_FPE4_Pos) /**< \brief (PWM_FPE2) Fault Protection Enable for channel 4 (fault input bit varies from 0 to 5) */
#define PWM_FPE2_FPE4(value) ((PWM_FPE2_FPE4_Msk & ((value) << PWM_FPE2_FPE4_Pos)))
#define PWM_FPE2_FPE5_Pos 8
#define PWM_FPE2_FPE5_Msk (0xffu << PWM_FPE2_FPE5_Pos) /**< \brief (PWM_FPE2) Fault Protection Enable for channel 5 (fault input bit varies from 0 to 5) */
#define PWM_FPE2_FPE5(value) ((PWM_FPE2_FPE5_Msk & ((value) << PWM_FPE2_FPE5_Pos)))
#define PWM_FPE2_FPE6_Pos 16
#define PWM_FPE2_FPE6_Msk (0xffu << PWM_FPE2_FPE6_Pos) /**< \brief (PWM_FPE2) Fault Protection Enable for channel 6 (fault input bit varies from 0 to 5) */
#define PWM_FPE2_FPE6(value) ((PWM_FPE2_FPE6_Msk & ((value) << PWM_FPE2_FPE6_Pos)))
#define PWM_FPE2_FPE7_Pos 24
#define PWM_FPE2_FPE7_Msk (0xffu << PWM_FPE2_FPE7_Pos) /**< \brief (PWM_FPE2) Fault Protection Enable for channel 7 (fault input bit varies from 0 to 5) */
#define PWM_FPE2_FPE7(value) ((PWM_FPE2_FPE7_Msk & ((value) << PWM_FPE2_FPE7_Pos)))
/* -------- PWM_ELMR[2] : (PWM Offset: 0x7C) PWM Event Line 0 Mode Register -------- */
#define PWM_ELMR_CSEL0 (0x1u << 0) /**< \brief (PWM_ELMR[2]) Comparison 0 Selection */
#define PWM_ELMR_CSEL1 (0x1u << 1) /**< \brief (PWM_ELMR[2]) Comparison 1 Selection */
#define PWM_ELMR_CSEL2 (0x1u << 2) /**< \brief (PWM_ELMR[2]) Comparison 2 Selection */
#define PWM_ELMR_CSEL3 (0x1u << 3) /**< \brief (PWM_ELMR[2]) Comparison 3 Selection */
#define PWM_ELMR_CSEL4 (0x1u << 4) /**< \brief (PWM_ELMR[2]) Comparison 4 Selection */
#define PWM_ELMR_CSEL5 (0x1u << 5) /**< \brief (PWM_ELMR[2]) Comparison 5 Selection */
#define PWM_ELMR_CSEL6 (0x1u << 6) /**< \brief (PWM_ELMR[2]) Comparison 6 Selection */
#define PWM_ELMR_CSEL7 (0x1u << 7) /**< \brief (PWM_ELMR[2]) Comparison 7 Selection */
/* -------- PWM_SMMR : (PWM Offset: 0xB0) PWM Stepper Motor Mode Register -------- */
#define PWM_SMMR_GCEN0 (0x1u << 0) /**< \brief (PWM_SMMR) Gray Count ENable */
#define PWM_SMMR_GCEN1 (0x1u << 1) /**< \brief (PWM_SMMR) Gray Count ENable */
#define PWM_SMMR_GCEN2 (0x1u << 2) /**< \brief (PWM_SMMR) Gray Count ENable */
#define PWM_SMMR_GCEN3 (0x1u << 3) /**< \brief (PWM_SMMR) Gray Count ENable */
#define PWM_SMMR_DOWN0 (0x1u << 16) /**< \brief (PWM_SMMR) DOWN Count */
#define PWM_SMMR_DOWN1 (0x1u << 17) /**< \brief (PWM_SMMR) DOWN Count */
#define PWM_SMMR_DOWN2 (0x1u << 18) /**< \brief (PWM_SMMR) DOWN Count */
#define PWM_SMMR_DOWN3 (0x1u << 19) /**< \brief (PWM_SMMR) DOWN Count */
/* -------- PWM_WPCR : (PWM Offset: 0xE4) PWM Write Protect Control Register -------- */
#define PWM_WPCR_WPCMD_Pos 0
#define PWM_WPCR_WPCMD_Msk (0x3u << PWM_WPCR_WPCMD_Pos) /**< \brief (PWM_WPCR) Write Protect Command */
#define PWM_WPCR_WPCMD(value) ((PWM_WPCR_WPCMD_Msk & ((value) << PWM_WPCR_WPCMD_Pos)))
#define PWM_WPCR_WPRG0 (0x1u << 2) /**< \brief (PWM_WPCR) Write Protect Register Group 0 */
#define PWM_WPCR_WPRG1 (0x1u << 3) /**< \brief (PWM_WPCR) Write Protect Register Group 1 */
#define PWM_WPCR_WPRG2 (0x1u << 4) /**< \brief (PWM_WPCR) Write Protect Register Group 2 */
#define PWM_WPCR_WPRG3 (0x1u << 5) /**< \brief (PWM_WPCR) Write Protect Register Group 3 */
#define PWM_WPCR_WPRG4 (0x1u << 6) /**< \brief (PWM_WPCR) Write Protect Register Group 4 */
#define PWM_WPCR_WPRG5 (0x1u << 7) /**< \brief (PWM_WPCR) Write Protect Register Group 5 */
#define PWM_WPCR_WPKEY_Pos 8
#define PWM_WPCR_WPKEY_Msk (0xffffffu << PWM_WPCR_WPKEY_Pos) /**< \brief (PWM_WPCR) Write Protect Key */
#define PWM_WPCR_WPKEY(value) ((PWM_WPCR_WPKEY_Msk & ((value) << PWM_WPCR_WPKEY_Pos)))
/* -------- PWM_WPSR : (PWM Offset: 0xE8) PWM Write Protect Status Register -------- */
#define PWM_WPSR_WPSWS0 (0x1u << 0) /**< \brief (PWM_WPSR) Write Protect SW Status */
#define PWM_WPSR_WPSWS1 (0x1u << 1) /**< \brief (PWM_WPSR) Write Protect SW Status */
#define PWM_WPSR_WPSWS2 (0x1u << 2) /**< \brief (PWM_WPSR) Write Protect SW Status */
#define PWM_WPSR_WPSWS3 (0x1u << 3) /**< \brief (PWM_WPSR) Write Protect SW Status */
#define PWM_WPSR_WPSWS4 (0x1u << 4) /**< \brief (PWM_WPSR) Write Protect SW Status */
#define PWM_WPSR_WPSWS5 (0x1u << 5) /**< \brief (PWM_WPSR) Write Protect SW Status */
#define PWM_WPSR_WPVS (0x1u << 7) /**< \brief (PWM_WPSR) Write Protect Violation Status */
#define PWM_WPSR_WPHWS0 (0x1u << 8) /**< \brief (PWM_WPSR) Write Protect HW Status */
#define PWM_WPSR_WPHWS1 (0x1u << 9) /**< \brief (PWM_WPSR) Write Protect HW Status */
#define PWM_WPSR_WPHWS2 (0x1u << 10) /**< \brief (PWM_WPSR) Write Protect HW Status */
#define PWM_WPSR_WPHWS3 (0x1u << 11) /**< \brief (PWM_WPSR) Write Protect HW Status */
#define PWM_WPSR_WPHWS4 (0x1u << 12) /**< \brief (PWM_WPSR) Write Protect HW Status */
#define PWM_WPSR_WPHWS5 (0x1u << 13) /**< \brief (PWM_WPSR) Write Protect HW Status */
#define PWM_WPSR_WPVSRC_Pos 16
#define PWM_WPSR_WPVSRC_Msk (0xffffu << PWM_WPSR_WPVSRC_Pos) /**< \brief (PWM_WPSR) Write Protect Violation Source */
/* -------- PWM_TPR : (PWM Offset: 0x108) Transmit Pointer Register -------- */
#define PWM_TPR_TXPTR_Pos 0
#define PWM_TPR_TXPTR_Msk (0xffffffffu << PWM_TPR_TXPTR_Pos) /**< \brief (PWM_TPR) Transmit Counter Register */
#define PWM_TPR_TXPTR(value) ((PWM_TPR_TXPTR_Msk & ((value) << PWM_TPR_TXPTR_Pos)))
/* -------- PWM_TCR : (PWM Offset: 0x10C) Transmit Counter Register -------- */
#define PWM_TCR_TXCTR_Pos 0
#define PWM_TCR_TXCTR_Msk (0xffffu << PWM_TCR_TXCTR_Pos) /**< \brief (PWM_TCR) Transmit Counter Register */
#define PWM_TCR_TXCTR(value) ((PWM_TCR_TXCTR_Msk & ((value) << PWM_TCR_TXCTR_Pos)))
/* -------- PWM_TNPR : (PWM Offset: 0x118) Transmit Next Pointer Register -------- */
#define PWM_TNPR_TXNPTR_Pos 0
#define PWM_TNPR_TXNPTR_Msk (0xffffffffu << PWM_TNPR_TXNPTR_Pos) /**< \brief (PWM_TNPR) Transmit Next Pointer */
#define PWM_TNPR_TXNPTR(value) ((PWM_TNPR_TXNPTR_Msk & ((value) << PWM_TNPR_TXNPTR_Pos)))
/* -------- PWM_TNCR : (PWM Offset: 0x11C) Transmit Next Counter Register -------- */
#define PWM_TNCR_TXNCTR_Pos 0
#define PWM_TNCR_TXNCTR_Msk (0xffffu << PWM_TNCR_TXNCTR_Pos) /**< \brief (PWM_TNCR) Transmit Counter Next */
#define PWM_TNCR_TXNCTR(value) ((PWM_TNCR_TXNCTR_Msk & ((value) << PWM_TNCR_TXNCTR_Pos)))
/* -------- PWM_PTCR : (PWM Offset: 0x120) Transfer Control Register -------- */
#define PWM_PTCR_RXTEN (0x1u << 0) /**< \brief (PWM_PTCR) Receiver Transfer Enable */
#define PWM_PTCR_RXTDIS (0x1u << 1) /**< \brief (PWM_PTCR) Receiver Transfer Disable */
#define PWM_PTCR_TXTEN (0x1u << 8) /**< \brief (PWM_PTCR) Transmitter Transfer Enable */
#define PWM_PTCR_TXTDIS (0x1u << 9) /**< \brief (PWM_PTCR) Transmitter Transfer Disable */
/* -------- PWM_PTSR : (PWM Offset: 0x124) Transfer Status Register -------- */
#define PWM_PTSR_RXTEN (0x1u << 0) /**< \brief (PWM_PTSR) Receiver Transfer Enable */
#define PWM_PTSR_TXTEN (0x1u << 8) /**< \brief (PWM_PTSR) Transmitter Transfer Enable */
/* -------- PWM_CMPV : (PWM Offset: N/A) PWM Comparison 0 Value Register -------- */
#define PWM_CMPV_CV_Pos 0
#define PWM_CMPV_CV_Msk (0xffffffu << PWM_CMPV_CV_Pos) /**< \brief (PWM_CMPV) Comparison x Value */
#define PWM_CMPV_CV(value) ((PWM_CMPV_CV_Msk & ((value) << PWM_CMPV_CV_Pos)))
#define PWM_CMPV_CVM (0x1u << 24) /**< \brief (PWM_CMPV) Comparison x Value Mode */
/* -------- PWM_CMPVUPD : (PWM Offset: N/A) PWM Comparison 0 Value Update Register -------- */
#define PWM_CMPVUPD_CVUPD_Pos 0
#define PWM_CMPVUPD_CVUPD_Msk (0xffffffu << PWM_CMPVUPD_CVUPD_Pos) /**< \brief (PWM_CMPVUPD) Comparison x Value Update */
#define PWM_CMPVUPD_CVUPD(value) ((PWM_CMPVUPD_CVUPD_Msk & ((value) << PWM_CMPVUPD_CVUPD_Pos)))
#define PWM_CMPVUPD_CVMUPD (0x1u << 24) /**< \brief (PWM_CMPVUPD) Comparison x Value Mode Update */
/* -------- PWM_CMPM : (PWM Offset: N/A) PWM Comparison 0 Mode Register -------- */
#define PWM_CMPM_CEN (0x1u << 0) /**< \brief (PWM_CMPM) Comparison x Enable */
#define PWM_CMPM_CTR_Pos 4
#define PWM_CMPM_CTR_Msk (0xfu << PWM_CMPM_CTR_Pos) /**< \brief (PWM_CMPM) Comparison x Trigger */
#define PWM_CMPM_CTR(value) ((PWM_CMPM_CTR_Msk & ((value) << PWM_CMPM_CTR_Pos)))
#define PWM_CMPM_CPR_Pos 8
#define PWM_CMPM_CPR_Msk (0xfu << PWM_CMPM_CPR_Pos) /**< \brief (PWM_CMPM) Comparison x Period */
#define PWM_CMPM_CPR(value) ((PWM_CMPM_CPR_Msk & ((value) << PWM_CMPM_CPR_Pos)))
#define PWM_CMPM_CPRCNT_Pos 12
#define PWM_CMPM_CPRCNT_Msk (0xfu << PWM_CMPM_CPRCNT_Pos) /**< \brief (PWM_CMPM) Comparison x Period Counter */
#define PWM_CMPM_CPRCNT(value) ((PWM_CMPM_CPRCNT_Msk & ((value) << PWM_CMPM_CPRCNT_Pos)))
#define PWM_CMPM_CUPR_Pos 16
#define PWM_CMPM_CUPR_Msk (0xfu << PWM_CMPM_CUPR_Pos) /**< \brief (PWM_CMPM) Comparison x Update Period */
#define PWM_CMPM_CUPR(value) ((PWM_CMPM_CUPR_Msk & ((value) << PWM_CMPM_CUPR_Pos)))
#define PWM_CMPM_CUPRCNT_Pos 20
#define PWM_CMPM_CUPRCNT_Msk (0xfu << PWM_CMPM_CUPRCNT_Pos) /**< \brief (PWM_CMPM) Comparison x Update Period Counter */
#define PWM_CMPM_CUPRCNT(value) ((PWM_CMPM_CUPRCNT_Msk & ((value) << PWM_CMPM_CUPRCNT_Pos)))
/* -------- PWM_CMPMUPD : (PWM Offset: N/A) PWM Comparison 0 Mode Update Register -------- */
#define PWM_CMPMUPD_CENUPD (0x1u << 0) /**< \brief (PWM_CMPMUPD) Comparison x Enable Update */
#define PWM_CMPMUPD_CTRUPD_Pos 4
#define PWM_CMPMUPD_CTRUPD_Msk (0xfu << PWM_CMPMUPD_CTRUPD_Pos) /**< \brief (PWM_CMPMUPD) Comparison x Trigger Update */
#define PWM_CMPMUPD_CTRUPD(value) ((PWM_CMPMUPD_CTRUPD_Msk & ((value) << PWM_CMPMUPD_CTRUPD_Pos)))
#define PWM_CMPMUPD_CPRUPD_Pos 8
#define PWM_CMPMUPD_CPRUPD_Msk (0xfu << PWM_CMPMUPD_CPRUPD_Pos) /**< \brief (PWM_CMPMUPD) Comparison x Period Update */
#define PWM_CMPMUPD_CPRUPD(value) ((PWM_CMPMUPD_CPRUPD_Msk & ((value) << PWM_CMPMUPD_CPRUPD_Pos)))
#define PWM_CMPMUPD_CUPRUPD_Pos 16
#define PWM_CMPMUPD_CUPRUPD_Msk (0xfu << PWM_CMPMUPD_CUPRUPD_Pos) /**< \brief (PWM_CMPMUPD) Comparison x Update Period Update */
#define PWM_CMPMUPD_CUPRUPD(value) ((PWM_CMPMUPD_CUPRUPD_Msk & ((value) << PWM_CMPMUPD_CUPRUPD_Pos)))
/* -------- PWM_CMR : (PWM Offset: N/A) PWM Channel Mode Register -------- */
#define PWM_CMR_CPRE_Pos 0
#define PWM_CMR_CPRE_Msk (0xfu << PWM_CMR_CPRE_Pos) /**< \brief (PWM_CMR) Channel Pre-scaler */
#define   PWM_CMR_CPRE_MCK (0x0u << 0) /**< \brief (PWM_CMR) Master clock */
#define   PWM_CMR_CPRE_MCK_DIV_2 (0x1u << 0) /**< \brief (PWM_CMR) Master clock/2 */
#define   PWM_CMR_CPRE_MCK_DIV_4 (0x2u << 0) /**< \brief (PWM_CMR) Master clock/4 */
#define   PWM_CMR_CPRE_MCK_DIV_8 (0x3u << 0) /**< \brief (PWM_CMR) Master clock/8 */
#define   PWM_CMR_CPRE_MCK_DIV_16 (0x4u << 0) /**< \brief (PWM_CMR) Master clock/16 */
#define   PWM_CMR_CPRE_MCK_DIV_32 (0x5u << 0) /**< \brief (PWM_CMR) Master clock/32 */
#define   PWM_CMR_CPRE_MCK_DIV_64 (0x6u << 0) /**< \brief (PWM_CMR) Master clock/64 */
#define   PWM_CMR_CPRE_MCK_DIV_128 (0x7u << 0) /**< \brief (PWM_CMR) Master clock/128 */
#define   PWM_CMR_CPRE_MCK_DIV_256 (0x8u << 0) /**< \brief (PWM_CMR) Master clock/256 */
#define   PWM_CMR_CPRE_MCK_DIV_512 (0x9u << 0) /**< \brief (PWM_CMR) Master clock/512 */
#define   PWM_CMR_CPRE_MCK_DIV_1024 (0xAu << 0) /**< \brief (PWM_CMR) Master clock/1024 */
#define   PWM_CMR_CPRE_CLKA (0xBu << 0) /**< \brief (PWM_CMR) Clock A */
#define   PWM_CMR_CPRE_CLKB (0xCu << 0) /**< \brief (PWM_CMR) Clock B */
#define PWM_CMR_CALG (0x1u << 8) /**< \brief (PWM_CMR) Channel Alignment */
#define PWM_CMR_CPOL (0x1u << 9) /**< \brief (PWM_CMR) Channel Polarity */
#define PWM_CMR_CES (0x1u << 10) /**< \brief (PWM_CMR) Counter Event Selection */
#define PWM_CMR_DTE (0x1u << 16) /**< \brief (PWM_CMR) Dead-Time Generator Enable */
#define PWM_CMR_DTHI (0x1u << 17) /**< \brief (PWM_CMR) Dead-Time PWMHx Output Inverted */
#define PWM_CMR_DTLI (0x1u << 18) /**< \brief (PWM_CMR) Dead-Time PWMLx Output Inverted */
/* -------- PWM_CDTY : (PWM Offset: N/A) PWM Channel Duty Cycle Register -------- */
#define PWM_CDTY_CDTY_Pos 0
#define PWM_CDTY_CDTY_Msk (0xffffffu << PWM_CDTY_CDTY_Pos) /**< \brief (PWM_CDTY) Channel Duty-Cycle */
#define PWM_CDTY_CDTY(value) ((PWM_CDTY_CDTY_Msk & ((value) << PWM_CDTY_CDTY_Pos)))
/* -------- PWM_CDTYUPD : (PWM Offset: N/A) PWM Channel Duty Cycle Update Register -------- */
#define PWM_CDTYUPD_CDTYUPD_Pos 0
#define PWM_CDTYUPD_CDTYUPD_Msk (0xffffffu << PWM_CDTYUPD_CDTYUPD_Pos) /**< \brief (PWM_CDTYUPD) Channel Duty-Cycle Update */
#define PWM_CDTYUPD_CDTYUPD(value) ((PWM_CDTYUPD_CDTYUPD_Msk & ((value) << PWM_CDTYUPD_CDTYUPD_Pos)))
/* -------- PWM_CPRD : (PWM Offset: N/A) PWM Channel Period Register -------- */
#define PWM_CPRD_CPRD_Pos 0
#define PWM_CPRD_CPRD_Msk (0xffffffu << PWM_CPRD_CPRD_Pos) /**< \brief (PWM_CPRD) Channel Period */
#define PWM_CPRD_CPRD(value) ((PWM_CPRD_CPRD_Msk & ((value) << PWM_CPRD_CPRD_Pos)))
/* -------- PWM_CPRDUPD : (PWM Offset: N/A) PWM Channel Period Update Register -------- */
#define PWM_CPRDUPD_CPRDUPD_Pos 0
#define PWM_CPRDUPD_CPRDUPD_Msk (0xffffffu << PWM_CPRDUPD_CPRDUPD_Pos) /**< \brief (PWM_CPRDUPD) Channel Period Update */
#define PWM_CPRDUPD_CPRDUPD(value) ((PWM_CPRDUPD_CPRDUPD_Msk & ((value) << PWM_CPRDUPD_CPRDUPD_Pos)))
/* -------- PWM_CCNT : (PWM Offset: N/A) PWM Channel Counter Register -------- */
#define PWM_CCNT_CNT_Pos 0
#define PWM_CCNT_CNT_Msk (0xffffffu << PWM_CCNT_CNT_Pos) /**< \brief (PWM_CCNT) Channel Counter Register */
/* -------- PWM_DT : (PWM Offset: N/A) PWM Channel Dead Time Register -------- */
#define PWM_DT_DTH_Pos 0
#define PWM_DT_DTH_Msk (0xffffu << PWM_DT_DTH_Pos) /**< \brief (PWM_DT) Dead-Time Value for PWMHx Output */
#define PWM_DT_DTH(value) ((PWM_DT_DTH_Msk & ((value) << PWM_DT_DTH_Pos)))
#define PWM_DT_DTL_Pos 16
#define PWM_DT_DTL_Msk (0xffffu << PWM_DT_DTL_Pos) /**< \brief (PWM_DT) Dead-Time Value for PWMLx Output */
#define PWM_DT_DTL(value) ((PWM_DT_DTL_Msk & ((value) << PWM_DT_DTL_Pos)))
/* -------- PWM_DTUPD : (PWM Offset: N/A) PWM Channel Dead Time Update Register -------- */
#define PWM_DTUPD_DTHUPD_Pos 0
#define PWM_DTUPD_DTHUPD_Msk (0xffffu << PWM_DTUPD_DTHUPD_Pos) /**< \brief (PWM_DTUPD) Dead-Time Value Update for PWMHx Output */
#define PWM_DTUPD_DTHUPD(value) ((PWM_DTUPD_DTHUPD_Msk & ((value) << PWM_DTUPD_DTHUPD_Pos)))
#define PWM_DTUPD_DTLUPD_Pos 16
#define PWM_DTUPD_DTLUPD_Msk (0xffffu << PWM_DTUPD_DTLUPD_Pos) /**< \brief (PWM_DTUPD) Dead-Time Value Update for PWMLx Output */
#define PWM_DTUPD_DTLUPD(value) ((PWM_DTUPD_DTLUPD_Msk & ((value) << PWM_DTUPD_DTLUPD_Pos)))




/* -------- TC_CCR : (TC Offset: N/A) Channel Control Register -------- */
#define TC_CCR_CLKEN (0x1u << 0) /**< \brief (TC_CCR) Counter Clock Enable Command */
#define TC_CCR_CLKDIS (0x1u << 1) /**< \brief (TC_CCR) Counter Clock Disable Command */
#define TC_CCR_SWTRG (0x1u << 2) /**< \brief (TC_CCR) Software Trigger Command */
/* -------- TC_CMR : (TC Offset: N/A) Channel Mode Register -------- */
#define TC_CMR_TCCLKS_Pos 0
#define TC_CMR_TCCLKS_Msk (0x7u << TC_CMR_TCCLKS_Pos) /**< \brief (TC_CMR) Clock Selection */
#define   TC_CMR_TCCLKS_TIMER_CLOCK1 (0x0u << 0) /**< \brief (TC_CMR) Clock selected: TCLK1 */
#define   TC_CMR_TCCLKS_TIMER_CLOCK2 (0x1u << 0) /**< \brief (TC_CMR) Clock selected: TCLK2 */
#define   TC_CMR_TCCLKS_TIMER_CLOCK3 (0x2u << 0) /**< \brief (TC_CMR) Clock selected: TCLK3 */
#define   TC_CMR_TCCLKS_TIMER_CLOCK4 (0x3u << 0) /**< \brief (TC_CMR) Clock selected: TCLK4 */
#define   TC_CMR_TCCLKS_TIMER_CLOCK5 (0x4u << 0) /**< \brief (TC_CMR) Clock selected: TCLK5 */
#define   TC_CMR_TCCLKS_XC0 (0x5u << 0) /**< \brief (TC_CMR) Clock selected: XC0 */
#define   TC_CMR_TCCLKS_XC1 (0x6u << 0) /**< \brief (TC_CMR) Clock selected: XC1 */
#define   TC_CMR_TCCLKS_XC2 (0x7u << 0) /**< \brief (TC_CMR) Clock selected: XC2 */
#define TC_CMR_CLKI (0x1u << 3) /**< \brief (TC_CMR) Clock Invert */
#define TC_CMR_BURST_Pos 4
#define TC_CMR_BURST_Msk (0x3u << TC_CMR_BURST_Pos) /**< \brief (TC_CMR) Burst Signal Selection */
#define   TC_CMR_BURST_NONE (0x0u << 4) /**< \brief (TC_CMR) The clock is not gated by an external signal. */
#define   TC_CMR_BURST_XC0 (0x1u << 4) /**< \brief (TC_CMR) XC0 is ANDed with the selected clock. */
#define   TC_CMR_BURST_XC1 (0x2u << 4) /**< \brief (TC_CMR) XC1 is ANDed with the selected clock. */
#define   TC_CMR_BURST_XC2 (0x3u << 4) /**< \brief (TC_CMR) XC2 is ANDed with the selected clock. */
#define TC_CMR_LDBSTOP (0x1u << 6) /**< \brief (TC_CMR) Counter Clock Stopped with RB Loading */
#define TC_CMR_LDBDIS (0x1u << 7) /**< \brief (TC_CMR) Counter Clock Disable with RB Loading */
#define TC_CMR_ETRGEDG_Pos 8
#define TC_CMR_ETRGEDG_Msk (0x3u << TC_CMR_ETRGEDG_Pos) /**< \brief (TC_CMR) External Trigger Edge Selection */
#define   TC_CMR_ETRGEDG_NONE (0x0u << 8) /**< \brief (TC_CMR) The clock is not gated by an external signal. */
#define   TC_CMR_ETRGEDG_RISING (0x1u << 8) /**< \brief (TC_CMR) Rising edge */
#define   TC_CMR_ETRGEDG_FALLING (0x2u << 8) /**< \brief (TC_CMR) Falling edge */
#define   TC_CMR_ETRGEDG_EDGE (0x3u << 8) /**< \brief (TC_CMR) Each edge */
#define TC_CMR_ABETRG (0x1u << 10) /**< \brief (TC_CMR) TIOA or TIOB External Trigger Selection */
#define TC_CMR_CPCTRG (0x1u << 14) /**< \brief (TC_CMR) RC Compare Trigger Enable */
#define TC_CMR_WAVE (0x1u << 15) /**< \brief (TC_CMR) Waveform Mode */
#define TC_CMR_LDRA_Pos 16
#define TC_CMR_LDRA_Msk (0x3u << TC_CMR_LDRA_Pos) /**< \brief (TC_CMR) RA Loading Edge Selection */
#define   TC_CMR_LDRA_NONE (0x0u << 16) /**< \brief (TC_CMR) None */
#define   TC_CMR_LDRA_RISING (0x1u << 16) /**< \brief (TC_CMR) Rising edge of TIOA */
#define   TC_CMR_LDRA_FALLING (0x2u << 16) /**< \brief (TC_CMR) Falling edge of TIOA */
#define   TC_CMR_LDRA_EDGE (0x3u << 16) /**< \brief (TC_CMR) Each edge of TIOA */
#define TC_CMR_LDRB_Pos 18
#define TC_CMR_LDRB_Msk (0x3u << TC_CMR_LDRB_Pos) /**< \brief (TC_CMR) RB Loading Edge Selection */
#define   TC_CMR_LDRB_NONE (0x0u << 18) /**< \brief (TC_CMR) None */
#define   TC_CMR_LDRB_RISING (0x1u << 18) /**< \brief (TC_CMR) Rising edge of TIOA */
#define   TC_CMR_LDRB_FALLING (0x2u << 18) /**< \brief (TC_CMR) Falling edge of TIOA */
#define   TC_CMR_LDRB_EDGE (0x3u << 18) /**< \brief (TC_CMR) Each edge of TIOA */
#define TC_CMR_CPCSTOP (0x1u << 6) /**< \brief (TC_CMR) Counter Clock Stopped with RC Compare */
#define TC_CMR_CPCDIS (0x1u << 7) /**< \brief (TC_CMR) Counter Clock Disable with RC Compare */
#define TC_CMR_EEVTEDG_Pos 8
#define TC_CMR_EEVTEDG_Msk (0x3u << TC_CMR_EEVTEDG_Pos) /**< \brief (TC_CMR) External Event Edge Selection */
#define   TC_CMR_EEVTEDG_NONE (0x0u << 8) /**< \brief (TC_CMR) None */
#define   TC_CMR_EEVTEDG_RISING (0x1u << 8) /**< \brief (TC_CMR) Rising edge */
#define   TC_CMR_EEVTEDG_FALLING (0x2u << 8) /**< \brief (TC_CMR) Falling edge */
#define   TC_CMR_EEVTEDG_EDGE (0x3u << 8) /**< \brief (TC_CMR) Each edge */
#define TC_CMR_EEVT_Pos 10
#define TC_CMR_EEVT_Msk (0x3u << TC_CMR_EEVT_Pos) /**< \brief (TC_CMR) External Event Selection */
#define   TC_CMR_EEVT_TIOB (0x0u << 10) /**< \brief (TC_CMR) TIOB */
#define   TC_CMR_EEVT_XC0 (0x1u << 10) /**< \brief (TC_CMR) XC0 */
#define   TC_CMR_EEVT_XC1 (0x2u << 10) /**< \brief (TC_CMR) XC1 */
#define   TC_CMR_EEVT_XC2 (0x3u << 10) /**< \brief (TC_CMR) XC2 */
#define TC_CMR_ENETRG (0x1u << 12) /**< \brief (TC_CMR) External Event Trigger Enable */
#define TC_CMR_WAVSEL_Pos 13
#define TC_CMR_WAVSEL_Msk (0x3u << TC_CMR_WAVSEL_Pos) /**< \brief (TC_CMR) Waveform Selection */
#define   TC_CMR_WAVSEL_UP (0x0u << 13) /**< \brief (TC_CMR) UP mode without automatic trigger on RC Compare */
#define   TC_CMR_WAVSEL_UPDOWN (0x1u << 13) /**< \brief (TC_CMR) UPDOWN mode without automatic trigger on RC Compare */
#define   TC_CMR_WAVSEL_UP_RC (0x2u << 13) /**< \brief (TC_CMR) UP mode with automatic trigger on RC Compare */
#define   TC_CMR_WAVSEL_UPDOWN_RC (0x3u << 13) /**< \brief (TC_CMR) UPDOWN mode with automatic trigger on RC Compare */
#define TC_CMR_ACPA_Pos 16
#define TC_CMR_ACPA_Msk (0x3u << TC_CMR_ACPA_Pos) /**< \brief (TC_CMR) RA Compare Effect on TIOA */
#define   TC_CMR_ACPA_NONE (0x0u << 16) /**< \brief (TC_CMR) None */
#define   TC_CMR_ACPA_SET (0x1u << 16) /**< \brief (TC_CMR) Set */
#define   TC_CMR_ACPA_CLEAR (0x2u << 16) /**< \brief (TC_CMR) Clear */
#define   TC_CMR_ACPA_TOGGLE (0x3u << 16) /**< \brief (TC_CMR) Toggle */
#define TC_CMR_ACPC_Pos 18
#define TC_CMR_ACPC_Msk (0x3u << TC_CMR_ACPC_Pos) /**< \brief (TC_CMR) RC Compare Effect on TIOA */
#define   TC_CMR_ACPC_NONE (0x0u << 18) /**< \brief (TC_CMR) None */
#define   TC_CMR_ACPC_SET (0x1u << 18) /**< \brief (TC_CMR) Set */
#define   TC_CMR_ACPC_CLEAR (0x2u << 18) /**< \brief (TC_CMR) Clear */
#define   TC_CMR_ACPC_TOGGLE (0x3u << 18) /**< \brief (TC_CMR) Toggle */
#define TC_CMR_AEEVT_Pos 20
#define TC_CMR_AEEVT_Msk (0x3u << TC_CMR_AEEVT_Pos) /**< \brief (TC_CMR) External Event Effect on TIOA */
#define   TC_CMR_AEEVT_NONE (0x0u << 20) /**< \brief (TC_CMR) None */
#define   TC_CMR_AEEVT_SET (0x1u << 20) /**< \brief (TC_CMR) Set */
#define   TC_CMR_AEEVT_CLEAR (0x2u << 20) /**< \brief (TC_CMR) Clear */
#define   TC_CMR_AEEVT_TOGGLE (0x3u << 20) /**< \brief (TC_CMR) Toggle */
#define TC_CMR_ASWTRG_Pos 22
#define TC_CMR_ASWTRG_Msk (0x3u << TC_CMR_ASWTRG_Pos) /**< \brief (TC_CMR) Software Trigger Effect on TIOA */
#define   TC_CMR_ASWTRG_NONE (0x0u << 22) /**< \brief (TC_CMR) None */
#define   TC_CMR_ASWTRG_SET (0x1u << 22) /**< \brief (TC_CMR) Set */
#define   TC_CMR_ASWTRG_CLEAR (0x2u << 22) /**< \brief (TC_CMR) Clear */
#define   TC_CMR_ASWTRG_TOGGLE (0x3u << 22) /**< \brief (TC_CMR) Toggle */
#define TC_CMR_BCPB_Pos 24
#define TC_CMR_BCPB_Msk (0x3u << TC_CMR_BCPB_Pos) /**< \brief (TC_CMR) RB Compare Effect on TIOB */
#define   TC_CMR_BCPB_NONE (0x0u << 24) /**< \brief (TC_CMR) None */
#define   TC_CMR_BCPB_SET (0x1u << 24) /**< \brief (TC_CMR) Set */
#define   TC_CMR_BCPB_CLEAR (0x2u << 24) /**< \brief (TC_CMR) Clear */
#define   TC_CMR_BCPB_TOGGLE (0x3u << 24) /**< \brief (TC_CMR) Toggle */
#define TC_CMR_BCPC_Pos 26
#define TC_CMR_BCPC_Msk (0x3u << TC_CMR_BCPC_Pos) /**< \brief (TC_CMR) RC Compare Effect on TIOB */
#define   TC_CMR_BCPC_NONE (0x0u << 26) /**< \brief (TC_CMR) None */
#define   TC_CMR_BCPC_SET (0x1u << 26) /**< \brief (TC_CMR) Set */
#define   TC_CMR_BCPC_CLEAR (0x2u << 26) /**< \brief (TC_CMR) Clear */
#define   TC_CMR_BCPC_TOGGLE (0x3u << 26) /**< \brief (TC_CMR) Toggle */
#define TC_CMR_BEEVT_Pos 28
#define TC_CMR_BEEVT_Msk (0x3u << TC_CMR_BEEVT_Pos) /**< \brief (TC_CMR) External Event Effect on TIOB */
#define   TC_CMR_BEEVT_NONE (0x0u << 28) /**< \brief (TC_CMR) None */
#define   TC_CMR_BEEVT_SET (0x1u << 28) /**< \brief (TC_CMR) Set */
#define   TC_CMR_BEEVT_CLEAR (0x2u << 28) /**< \brief (TC_CMR) Clear */
#define   TC_CMR_BEEVT_TOGGLE (0x3u << 28) /**< \brief (TC_CMR) Toggle */
#define TC_CMR_BSWTRG_Pos 30
#define TC_CMR_BSWTRG_Msk (0x3u << TC_CMR_BSWTRG_Pos) /**< \brief (TC_CMR) Software Trigger Effect on TIOB */
#define   TC_CMR_BSWTRG_NONE (0x0u << 30) /**< \brief (TC_CMR) None */
#define   TC_CMR_BSWTRG_SET (0x1u << 30) /**< \brief (TC_CMR) Set */
#define   TC_CMR_BSWTRG_CLEAR (0x2u << 30) /**< \brief (TC_CMR) Clear */
#define   TC_CMR_BSWTRG_TOGGLE (0x3u << 30) /**< \brief (TC_CMR) Toggle */
/* -------- TC_SMMR : (TC Offset: N/A) Stepper Motor Mode Register -------- */
#define TC_SMMR_GCEN (0x1u << 0) /**< \brief (TC_SMMR) Gray Count Enable */
#define TC_SMMR_DOWN (0x1u << 1) /**< \brief (TC_SMMR) DOWN Count */
/* -------- TC_CV : (TC Offset: N/A) Counter Value -------- */
#define TC_CV_CV_Pos 0
#define TC_CV_CV_Msk (0xffffffffu << TC_CV_CV_Pos) /**< \brief (TC_CV) Counter Value */
/* -------- TC_RA : (TC Offset: N/A) Register A -------- */
#define TC_RA_RA_Pos 0
#define TC_RA_RA_Msk (0xffffffffu << TC_RA_RA_Pos) /**< \brief (TC_RA) Register A */
#define TC_RA_RA(value) ((TC_RA_RA_Msk & ((value) << TC_RA_RA_Pos)))
/* -------- TC_RB : (TC Offset: N/A) Register B -------- */
#define TC_RB_RB_Pos 0
#define TC_RB_RB_Msk (0xffffffffu << TC_RB_RB_Pos) /**< \brief (TC_RB) Register B */
#define TC_RB_RB(value) ((TC_RB_RB_Msk & ((value) << TC_RB_RB_Pos)))
/* -------- TC_RC : (TC Offset: N/A) Register C -------- */
#define TC_RC_RC_Pos 0
#define TC_RC_RC_Msk (0xffffffffu << TC_RC_RC_Pos) /**< \brief (TC_RC) Register C */
#define TC_RC_RC(value) ((TC_RC_RC_Msk & ((value) << TC_RC_RC_Pos)))
/* -------- TC_SR : (TC Offset: N/A) Status Register -------- */
#define TC_SR_COVFS (0x1u << 0) /**< \brief (TC_SR) Counter Overflow Status */
#define TC_SR_LOVRS (0x1u << 1) /**< \brief (TC_SR) Load Overrun Status */
#define TC_SR_CPAS (0x1u << 2) /**< \brief (TC_SR) RA Compare Status */
#define TC_SR_CPBS (0x1u << 3) /**< \brief (TC_SR) RB Compare Status */
#define TC_SR_CPCS (0x1u << 4) /**< \brief (TC_SR) RC Compare Status */
#define TC_SR_LDRAS (0x1u << 5) /**< \brief (TC_SR) RA Loading Status */
#define TC_SR_LDRBS (0x1u << 6) /**< \brief (TC_SR) RB Loading Status */
#define TC_SR_ETRGS (0x1u << 7) /**< \brief (TC_SR) External Trigger Status */
#define TC_SR_CLKSTA (0x1u << 16) /**< \brief (TC_SR) Clock Enabling Status */
#define TC_SR_MTIOA (0x1u << 17) /**< \brief (TC_SR) TIOA Mirror */
#define TC_SR_MTIOB (0x1u << 18) /**< \brief (TC_SR) TIOB Mirror */
/* -------- TC_IER : (TC Offset: N/A) Interrupt Enable Register -------- */
#define TC_IER_COVFS (0x1u << 0) /**< \brief (TC_IER) Counter Overflow */
#define TC_IER_LOVRS (0x1u << 1) /**< \brief (TC_IER) Load Overrun */
#define TC_IER_CPAS (0x1u << 2) /**< \brief (TC_IER) RA Compare */
#define TC_IER_CPBS (0x1u << 3) /**< \brief (TC_IER) RB Compare */
#define TC_IER_CPCS (0x1u << 4) /**< \brief (TC_IER) RC Compare */
#define TC_IER_LDRAS (0x1u << 5) /**< \brief (TC_IER) RA Loading */
#define TC_IER_LDRBS (0x1u << 6) /**< \brief (TC_IER) RB Loading */
#define TC_IER_ETRGS (0x1u << 7) /**< \brief (TC_IER) External Trigger */
/* -------- TC_IDR : (TC Offset: N/A) Interrupt Disable Register -------- */
#define TC_IDR_COVFS (0x1u << 0) /**< \brief (TC_IDR) Counter Overflow */
#define TC_IDR_LOVRS (0x1u << 1) /**< \brief (TC_IDR) Load Overrun */
#define TC_IDR_CPAS (0x1u << 2) /**< \brief (TC_IDR) RA Compare */
#define TC_IDR_CPBS (0x1u << 3) /**< \brief (TC_IDR) RB Compare */
#define TC_IDR_CPCS (0x1u << 4) /**< \brief (TC_IDR) RC Compare */
#define TC_IDR_LDRAS (0x1u << 5) /**< \brief (TC_IDR) RA Loading */
#define TC_IDR_LDRBS (0x1u << 6) /**< \brief (TC_IDR) RB Loading */
#define TC_IDR_ETRGS (0x1u << 7) /**< \brief (TC_IDR) External Trigger */
/* -------- TC_IMR : (TC Offset: N/A) Interrupt Mask Register -------- */
#define TC_IMR_COVFS (0x1u << 0) /**< \brief (TC_IMR) Counter Overflow */
#define TC_IMR_LOVRS (0x1u << 1) /**< \brief (TC_IMR) Load Overrun */
#define TC_IMR_CPAS (0x1u << 2) /**< \brief (TC_IMR) RA Compare */
#define TC_IMR_CPBS (0x1u << 3) /**< \brief (TC_IMR) RB Compare */
#define TC_IMR_CPCS (0x1u << 4) /**< \brief (TC_IMR) RC Compare */
#define TC_IMR_LDRAS (0x1u << 5) /**< \brief (TC_IMR) RA Loading */
#define TC_IMR_LDRBS (0x1u << 6) /**< \brief (TC_IMR) RB Loading */
#define TC_IMR_ETRGS (0x1u << 7) /**< \brief (TC_IMR) External Trigger */
/* -------- TC_BCR : (TC Offset: 0xC0) Block Control Register -------- */
#define TC_BCR_SYNC (0x1u << 0) /**< \brief (TC_BCR) Synchro Command */
/* -------- TC_BMR : (TC Offset: 0xC4) Block Mode Register -------- */
#define TC_BMR_TC0XC0S_Pos 0
#define TC_BMR_TC0XC0S_Msk (0x3u << TC_BMR_TC0XC0S_Pos) /**< \brief (TC_BMR) External Clock Signal 0 Selection */
#define   TC_BMR_TC0XC0S_TCLK0 (0x0u << 0) /**< \brief (TC_BMR) Signal connected to XC0: TCLK0 */
#define   TC_BMR_TC0XC0S_TIOA1 (0x2u << 0) /**< \brief (TC_BMR) Signal connected to XC0: TIOA1 */
#define   TC_BMR_TC0XC0S_TIOA2 (0x3u << 0) /**< \brief (TC_BMR) Signal connected to XC0: TIOA2 */
#define TC_BMR_TC1XC1S_Pos 2
#define TC_BMR_TC1XC1S_Msk (0x3u << TC_BMR_TC1XC1S_Pos) /**< \brief (TC_BMR) External Clock Signal 1 Selection */
#define   TC_BMR_TC1XC1S_TCLK1 (0x0u << 2) /**< \brief (TC_BMR) Signal connected to XC1: TCLK1 */
#define   TC_BMR_TC1XC1S_TIOA0 (0x2u << 2) /**< \brief (TC_BMR) Signal connected to XC1: TIOA0 */
#define   TC_BMR_TC1XC1S_TIOA2 (0x3u << 2) /**< \brief (TC_BMR) Signal connected to XC1: TIOA2 */
#define TC_BMR_TC2XC2S_Pos 4
#define TC_BMR_TC2XC2S_Msk (0x3u << TC_BMR_TC2XC2S_Pos) /**< \brief (TC_BMR) External Clock Signal 2 Selection */
#define   TC_BMR_TC2XC2S_TCLK2 (0x0u << 4) /**< \brief (TC_BMR) Signal connected to XC2: TCLK2 */
#define   TC_BMR_TC2XC2S_TIOA1 (0x2u << 4) /**< \brief (TC_BMR) Signal connected to XC2: TIOA1 */
#define   TC_BMR_TC2XC2S_TIOA2 (0x3u << 4) /**< \brief (TC_BMR) Signal connected to XC2: TIOA2 */
#define TC_BMR_QDEN (0x1u << 8) /**< \brief (TC_BMR) Quadrature Decoder ENabled */
#define TC_BMR_POSEN (0x1u << 9) /**< \brief (TC_BMR) POSition ENabled */
#define TC_BMR_SPEEDEN (0x1u << 10) /**< \brief (TC_BMR) SPEED ENabled */
#define TC_BMR_QDTRANS (0x1u << 11) /**< \brief (TC_BMR) Quadrature Decoding TRANSparent */
#define TC_BMR_EDGPHA (0x1u << 12) /**< \brief (TC_BMR) EDGe on PHA count mode */
#define TC_BMR_INVA (0x1u << 13) /**< \brief (TC_BMR) INVerted phA */
#define TC_BMR_INVB (0x1u << 14) /**< \brief (TC_BMR) INVerted phB */
#define TC_BMR_INVIDX (0x1u << 15) /**< \brief (TC_BMR) INVerted InDeX */
#define TC_BMR_SWAP (0x1u << 16) /**< \brief (TC_BMR) SWAP PHA and PHB */
#define TC_BMR_IDXPHB (0x1u << 17) /**< \brief (TC_BMR) InDeX pin is PHB pin */
#define TC_BMR_FILTER (0x1u << 19) /**< \brief (TC_BMR)  */
#define TC_BMR_MAXFILT_Pos 20
#define TC_BMR_MAXFILT_Msk (0x3fu << TC_BMR_MAXFILT_Pos) /**< \brief (TC_BMR) MAXimum FILTer */
#define TC_BMR_MAXFILT(value) ((TC_BMR_MAXFILT_Msk & ((value) << TC_BMR_MAXFILT_Pos)))
/* -------- TC_QIER : (TC Offset: 0xC8) QDEC Interrupt Enable Register -------- */
#define TC_QIER_IDX (0x1u << 0) /**< \brief (TC_QIER) InDeX */
#define TC_QIER_DIRCHG (0x1u << 1) /**< \brief (TC_QIER) DIRection CHanGe */
#define TC_QIER_QERR (0x1u << 2) /**< \brief (TC_QIER) Quadrature ERRor */
/* -------- TC_QIDR : (TC Offset: 0xCC) QDEC Interrupt Disable Register -------- */
#define TC_QIDR_IDX (0x1u << 0) /**< \brief (TC_QIDR) InDeX */
#define TC_QIDR_DIRCHG (0x1u << 1) /**< \brief (TC_QIDR) DIRection CHanGe */
#define TC_QIDR_QERR (0x1u << 2) /**< \brief (TC_QIDR) Quadrature ERRor */
/* -------- TC_QIMR : (TC Offset: 0xD0) QDEC Interrupt Mask Register -------- */
#define TC_QIMR_IDX (0x1u << 0) /**< \brief (TC_QIMR) InDeX */
#define TC_QIMR_DIRCHG (0x1u << 1) /**< \brief (TC_QIMR) DIRection CHanGe */
#define TC_QIMR_QERR (0x1u << 2) /**< \brief (TC_QIMR) Quadrature ERRor */
/* -------- TC_QISR : (TC Offset: 0xD4) QDEC Interrupt Status Register -------- */
#define TC_QISR_IDX (0x1u << 0) /**< \brief (TC_QISR) InDeX */
#define TC_QISR_DIRCHG (0x1u << 1) /**< \brief (TC_QISR) DIRection CHanGe */
#define TC_QISR_QERR (0x1u << 2) /**< \brief (TC_QISR) Quadrature ERRor */
#define TC_QISR_DIR (0x1u << 8) /**< \brief (TC_QISR) Direction */
/* -------- TC_FMR : (TC Offset: 0xD8) Fault Mode Register -------- */
#define TC_FMR_ENCF0 (0x1u << 0) /**< \brief (TC_FMR) ENable Compare Fault Channel 0 */
#define TC_FMR_ENCF1 (0x1u << 1) /**< \brief (TC_FMR) ENable Compare Fault Channel 1 */
/* -------- TC_WPMR : (TC Offset: 0xE4) Write Protect Mode Register -------- */
#define TC_WPMR_WPEN (0x1u << 0) /**< \brief (TC_WPMR) Write Protect Enable */
#define TC_WPMR_WPKEY_Pos 8
#define TC_WPMR_WPKEY_Msk (0xffffffu << TC_WPMR_WPKEY_Pos) /**< \brief (TC_WPMR) Write Protect KEY */
#define TC_WPMR_WPKEY(value) ((TC_WPMR_WPKEY_Msk & ((value) << TC_WPMR_WPKEY_Pos)))


#endif
/*
int vhalInitTIM(void *data);
int vhalHtmGetFreeTimer(void);
int vhalHtmOneShot(uint32_t tm, uint32_t delay, htmFn fn, void *args);
int vhalHtmRecurrent(uint32_t tm, uint32_t delay, htmFn fn, void *args);
int vhalPwmStart(int vpin, uint32_t period, uint32_t pulse);
int vhalIcuStart(int vpin, vhalIcuConf *conf);
*/
#endif