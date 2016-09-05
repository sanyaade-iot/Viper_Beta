#ifndef __VHAL_TIM__
#define __VHAL_TIM__
#include "vhal_common.h"


#define PIN_TO_PWM(vpin) vhalPinSetToPeripheral(vpin, PRPH_PWM, ALTERNATE_FN(VP_TOAF(vpin)) | STM32_PUDR_NOPULL | STM32_OSPEED_HIGHEST | STM32_OTYPE_PUSHPULL);
#define PIN_TO_ICU(vpin,cfg) vhalPinSetToPeripheral(vpin, PRPH_ICU, ALTERNATE_FN(VP_TOAF(vpin)) | ((ICU_CFG_GET_INPUT(cfg) == ICU_INPUT_PULLUP) ? STM32_PUDR_PULLUP : STM32_PUDR_PULLDOWN));
#define TICKS_PER_MUSEC(apb) ((_system_frequency)/1000000)
//tm starts from zero...
#define TIM_APB(tm) ((((tm)>=1 && (tm)<7)||(tm)>=11) ? 1:2) 

typedef enum 
{
  RESET = 0, 
  SET = !RESET
} FlagStatus, ITStatus;


typedef struct
{
  __IO uint16_t CR1;         /*!< TIM control register 1,              Address offset: 0x00 */
  uint16_t      RESERVED0;   /*!< Reserved, 0x02                                            */
  __IO uint16_t CR2;         /*!< TIM control register 2,              Address offset: 0x04 */
  uint16_t      RESERVED1;   /*!< Reserved, 0x06                                            */
  __IO uint16_t SMCR;        /*!< TIM slave mode control register,     Address offset: 0x08 */
  uint16_t      RESERVED2;   /*!< Reserved, 0x0A                                            */
  __IO uint16_t DIER;        /*!< TIM DMA/interrupt enable register,   Address offset: 0x0C */
  uint16_t      RESERVED3;   /*!< Reserved, 0x0E                                            */
  __IO uint16_t SR;          /*!< TIM status register,                 Address offset: 0x10 */
  uint16_t      RESERVED4;   /*!< Reserved, 0x12                                            */
  __IO uint16_t EGR;         /*!< TIM event generation register,       Address offset: 0x14 */
  uint16_t      RESERVED5;   /*!< Reserved, 0x16                                            */
  __IO uint16_t CCMR1;       /*!< TIM capture/compare mode register 1, Address offset: 0x18 */
  uint16_t      RESERVED6;   /*!< Reserved, 0x1A                                            */
  __IO uint16_t CCMR2;       /*!< TIM capture/compare mode register 2, Address offset: 0x1C */
  uint16_t      RESERVED7;   /*!< Reserved, 0x1E                                            */
  __IO uint16_t CCER;        /*!< TIM capture/compare enable register, Address offset: 0x20 */
  uint16_t      RESERVED8;   /*!< Reserved, 0x22                                            */
  __IO uint32_t CNT;         /*!< TIM counter register,                Address offset: 0x24 */
  __IO uint16_t PSC;         /*!< TIM prescaler,                       Address offset: 0x28 */
  uint16_t      RESERVED9;   /*!< Reserved, 0x2A                                            */
  __IO uint32_t ARR;         /*!< TIM auto-reload register,            Address offset: 0x2C */
  __IO uint16_t RCR;         /*!< TIM repetition counter register,     Address offset: 0x30 */
  uint16_t      RESERVED10;  /*!< Reserved, 0x32                                            */
  __IO uint32_t CCR[4];

  //__IO uint32_t CCR1;        /*!< TIM capture/compare register 1,      Address offset: 0x34 */
  //__IO uint32_t CCR2;        /*!< TIM capture/compare register 2,      Address offset: 0x38 */
  //__IO uint32_t CCR3;        /*!< TIM capture/compare register 3,      Address offset: 0x3C */
  //__IO uint32_t CCR4;        /*!< TIM capture/compare register 4,      Address offset: 0x40 */
  __IO uint16_t BDTR;        /*!< TIM break and dead-time register,    Address offset: 0x44 */
  uint16_t      RESERVED11;  /*!< Reserved, 0x46                                            */
  __IO uint16_t DCR;         /*!< TIM DMA control register,            Address offset: 0x48 */
  uint16_t      RESERVED12;  /*!< Reserved, 0x4A                                            */
  __IO uint16_t DMAR;        /*!< TIM DMA address for full transfer,   Address offset: 0x4C */
  uint16_t      RESERVED13;  /*!< Reserved, 0x4E                                            */
  __IO uint16_t OR;          /*!< TIM option register,                 Address offset: 0x50 */
  uint16_t      RESERVED14;  /*!< Reserved, 0x52                                            */
} TIM_TypeDef;


typedef struct
{

  uint16_t TIM_Channel;      /*!< Specifies the TIM channel.
                                  This parameter can be a value of @ref TIM_Channel */

  uint16_t TIM_ICPolarity;   /*!< Specifies the active edge of the input signal.
                                  This parameter can be a value of @ref TIM_Input_Capture_Polarity */

  uint16_t TIM_ICSelection;  /*!< Specifies the input.
                                  This parameter can be a value of @ref TIM_Input_Capture_Selection */

  uint16_t TIM_ICPrescaler;  /*!< Specifies the Input Capture Prescaler.
                                  This parameter can be a value of @ref TIM_Input_Capture_Prescaler */

  uint16_t TIM_ICFilter;     /*!< Specifies the input capture filter.
                                  This parameter can be a number between 0x0 and 0xF */
} TIM_ICInitTypeDef;


typedef struct
{
  uint16_t TIM_OCMode;        /*!< Specifies the TIM mode.
                                   This parameter can be a value of @ref TIM_Output_Compare_and_PWM_modes */
  uint16_t TIM_OutputState;   /*!< Specifies the TIM Output Compare state.
                                   This parameter can be a value of @ref TIM_Output_Compare_State */
  uint16_t TIM_OutputNState;  /*!< Specifies the TIM complementary Output Compare state.
                                   This parameter can be a value of @ref TIM_Output_Compare_N_State
                                   @note This parameter is valid only for TIM1 and TIM8. */
  uint32_t TIM_Pulse;         /*!< Specifies the pulse value to be loaded into the Capture Compare Register. 
                                   This parameter can be a number between 0x0000 and 0xFFFF */
  uint16_t TIM_OCPolarity;    /*!< Specifies the output polarity.
                                   This parameter can be a value of @ref TIM_Output_Compare_Polarity */
  uint16_t TIM_OCNPolarity;   /*!< Specifies the complementary output polarity.
                                   This parameter can be a value of @ref TIM_Output_Compare_N_Polarity
                                   @note This parameter is valid only for TIM1 and TIM8. */
  uint16_t TIM_OCIdleState;   /*!< Specifies the TIM Output Compare pin state during Idle state.
                                   This parameter can be a value of @ref TIM_Output_Compare_Idle_State
                                   @note This parameter is valid only for TIM1 and TIM8. */
  uint16_t TIM_OCNIdleState;  /*!< Specifies the TIM Output Compare pin state during Idle state.
                                   This parameter can be a value of @ref TIM_Output_Compare_N_Idle_State
                                   @note This parameter is valid only for TIM1 and TIM8. */
} TIM_OCInitTypeDef;



typedef struct
{
  __IO uint32_t MODER;    /*!< GPIO port mode register,               Address offset: 0x00      */
  __IO uint32_t OTYPER;   /*!< GPIO port output type register,        Address offset: 0x04      */
  __IO uint32_t OSPEEDR;  /*!< GPIO port output speed register,       Address offset: 0x08      */
  __IO uint32_t PUPDR;    /*!< GPIO port pull-up/pull-down register,  Address offset: 0x0C      */
  __IO uint32_t IDR;      /*!< GPIO port input data register,         Address offset: 0x10      */
  __IO uint32_t ODR;      /*!< GPIO port output data register,        Address offset: 0x14      */
  __IO uint16_t BSRRL;    /*!< GPIO port bit set/reset low register,  Address offset: 0x18      */
  __IO uint16_t BSRRH;    /*!< GPIO port bit set/reset high register, Address offset: 0x1A      */
  __IO uint32_t LCKR;     /*!< GPIO port configuration lock register, Address offset: 0x1C      */
  __IO uint32_t AFR[2];   /*!< GPIO alternate function registers,     Address offset: 0x20-0x24 */
} GPIO_TypeDef;


#define  TIMNUM                              11       /* TODO: set "official" num */ 
#define  CHANNUM                             4      /* TODO: set "official" num (channel for timer)*/ 

#define  TIM1                                ((TIM_TypeDef *) (APB2PERIPH_BASE + 0x0000))
#define  TIM2                                ((TIM_TypeDef *) (APB1PERIPH_BASE + 0x0000))
#define  TIM3                                ((TIM_TypeDef *) (APB1PERIPH_BASE + 0x0400))
#define  TIM4                                ((TIM_TypeDef *) (APB1PERIPH_BASE + 0x0800))
#define  TIM5                                ((TIM_TypeDef *) (APB1PERIPH_BASE + 0x0C00))
#define  TIM6                                ((TIM_TypeDef *) (APB1PERIPH_BASE + 0x1000))
#define  TIM7                                ((TIM_TypeDef *) (APB1PERIPH_BASE + 0x1400))
#define  TIM8                                ((TIM_TypeDef *) (APB2PERIPH_BASE + 0x0400))
#define  TIM9                                ((TIM_TypeDef *) (APB2PERIPH_BASE + 0x4000))
#define  TIM10                               ((TIM_TypeDef *) (APB2PERIPH_BASE + 0x4400))
#define  TIM11                               ((TIM_TypeDef *) (APB2PERIPH_BASE + 0x4800))

/* for uniformity */
#define  vhalIrq_TIM1                        vhalIrq_TIM1_UPD_TIM10
#define  vhalIrq_TIM8                        vhalIrq_TIM8_UP_TIM13
#define  vhalIrq_TIM9                        vhalIrq_TIM1_BRK_TIM9
//#define vhalIrq_TIM10 vhalIrq_TIM1_UPD_TIM10
#define  vhalIrq_TIM11                       vhalIrq_TIM1_TRG_COM_TIM11

/* TODO: identify all variant */
#define  TIM_CKD_DIV1                        ((uint16_t)0x0000)
#define  TIM_CounterMode_Up                  ((uint16_t)0x0000)
#define  TIM_IT_Update                       ((uint16_t)0x0001)
#define  TIM_BDTR_MOE                        ((uint16_t)0x8000)            /*!<Main Output enable          */

#define  TIM_OCPreload_Enable                ((uint16_t)0x0008)
#define  TIM_CCER_CC4E                       ((uint16_t)0x1000)            /*!<Capture/Compare 4 output enable                 */
#define  TIM_CCMR2_OC4M                      ((uint16_t)0x7000)            /*!<OC4M[2:0] bits (Output Compare 4 Mode) */
#define  TIM_CCMR2_CC4S                      ((uint16_t)0x0300)            /*!<CC4S[1:0] bits (Capture/Compare 4 Selection) */
#define  TIM_CR2_OIS4                        ((uint16_t)0x4000)            /*!<Output Idle state 4 (OC4 output)  */
#define  TIM_OCMode_PWM1                     ((uint16_t)0x0060)
#define  TIM_OutputState_Enable              ((uint16_t)0x0001)
#define  TIM_OutputNState_Enable             ((uint16_t)0x0004)
#define  TIM_OCPolarity_High                 ((uint16_t)0x0000)
#define  TIM_OCPreload_Enable                ((uint16_t)0x0008)
#define  TIM_CCMR2_OC4PE                     ((uint16_t)0x0800)            /*!<Output Compare 4 Preload enable */
#define  TIM_CCMR1_OC2PE                     ((uint16_t)0x0800)            /*!<Output Compare 2 Preload enable              */
#define  TIM_CCMR2_OC3PE                     ((uint16_t)0x0008)            /*!<Output Compare 3 Preload enable        */
#define  TIM_CR1_ARPE                        ((uint16_t)0x0080)            /*!<Auto-reload preload enable     */
#define  TIM_CR1_OPM                         ((uint16_t)0x0008)            /*!<One pulse mode        */
#define  TIM_CR1_URS                         ((uint16_t)0x0004)            /*!<Update request source */
#define  TIM_CR1_UDIS                        ((uint16_t)0x0002)            /*!<Update disable        */
#define  STM32_TIM_CR2_MMS_MASK              (7U << 4)
#define  TIM_CR2_MMS_UPDATE                  (0x2 << 4)
#define  TIM_CR2_MMS_MASK                    (0x7 << 4)
#define  TIM_OCIdleState_Set                 ((uint16_t)0x0100)
#define  TIM_OCIdleState_Reset                ((uint16_t)0x0000)

#define  TIM_Channel_1                       ((uint16_t)0x0000)
#define  TIM_Channel_2                       ((uint16_t)0x0004)
#define  TIM_Channel_3                       ((uint16_t)0x0008)
#define  TIM_Channel_4                       ((uint16_t)0x000C)
#define  TIM_CCMR1_IC1F                      ((uint16_t)0x00F0)            /*!<IC1F[3:0] bits (Input Capture 1 Filter)      */
#define  TIM_CCMR1_IC1PSC                    ((uint16_t)0x000C)            /*!<IC1PSC[1:0] bits (Input Capture 1 Prescaler) */
#define  TIM_CCMR1_IC2F                      ((uint16_t)0xF000)            /*!<IC2F[3:0] bits (Input Capture 2 Filter)       */
#define  TIM_CCMR1_IC2PSC                    ((uint16_t)0x0C00)            /*!<IC2PSC[1:0] bits (Input Capture 2 Prescaler)  */
#define  TIM_CCMR2_IC3F                      ((uint16_t)0x00F0)            /*!<IC3F[3:0] bits (Input Capture 3 Filter) */
#define  TIM_CCMR2_IC3PSC                    ((uint16_t)0x000C)            /*!<IC3PSC[1:0] bits (Input Capture 3 Prescaler) */
#define  TIM_CCMR2_IC4F                      ((uint16_t)0xF000)            /*!<IC4F[3:0] bits (Input Capture 4 Filter) */
#define  TIM_CCMR2_IC4PSC                    ((uint16_t)0x0C00)            /*!<IC4PSC[1:0] bits (Input Capture 4 Prescaler) */

#define  TIM_ICPolarity_Rising               ((uint16_t)0x0000)
#define  TIM_ICPolarity_Falling              ((uint16_t)0x0002)
#define  TIM_ICPolarity_BothEdge             ((uint16_t)0x000A)

#define  TIM_ICSelection_DirectTI            ((uint16_t)0x0001)               /*!< TIM Input 1, 2, 3 or 4 is selected to be */
#define  TIM_ICPSC_DIV1                      ((uint16_t)0x0000) /*!< Capture performed each time an edge is detected on the capture input. */
#define  TIM_IT_CC1                          ((uint16_t)0x0002)
#define  TIM_ICPolarity_Falling              ((uint16_t)0x0002)
#define  TIM_IT_CC1                          ((uint16_t)0x0002)

/*******************  Bit definition for TIM_DIER register  *******************/
#define  TIM_DIER_UIE                        ((uint16_t)0x0001)            /*!<Update interrupt enable */
#define  TIM_DIER_CC1IE                      ((uint16_t)0x0002)            /*!<Capture/Compare 1 interrupt enable   */
#define  TIM_DIER_CC2IE                      ((uint16_t)0x0004)            /*!<Capture/Compare 2 interrupt enable   */
#define  TIM_DIER_CC3IE                      ((uint16_t)0x0008)            /*!<Capture/Compare 3 interrupt enable   */
#define  TIM_DIER_CC4IE                      ((uint16_t)0x0010)            /*!<Capture/Compare 4 interrupt enable   */
#define  TIM_DIER_COMIE                      ((uint16_t)0x0020)            /*!<COM interrupt enable                 */
#define  TIM_DIER_TIE                        ((uint16_t)0x0040)            /*!<Trigger interrupt enable             */
#define  TIM_DIER_BIE                        ((uint16_t)0x0080)            /*!<Break interrupt enable               */
#define  TIM_DIER_UDE                        ((uint16_t)0x0100)            /*!<Update DMA request enable            */
#define  TIM_DIER_CC1DE                      ((uint16_t)0x0200)            /*!<Capture/Compare 1 DMA request enable */
#define  TIM_DIER_CC2DE                      ((uint16_t)0x0400)            /*!<Capture/Compare 2 DMA request enable */
#define  TIM_DIER_CC3DE                      ((uint16_t)0x0800)            /*!<Capture/Compare 3 DMA request enable */
#define  TIM_DIER_CC4DE                      ((uint16_t)0x1000)            /*!<Capture/Compare 4 DMA request enable */
#define  TIM_DIER_COMDE                      ((uint16_t)0x2000)            /*!<COM DMA request enable               */
#define  TIM_DIER_TDE                        ((uint16_t)0x4000)            /*!<Trigger DMA request enable           */


/*******************  Bit definition for TIM_CCER register  *******************/
#define  TIM_CCER_CC1E                       ((uint16_t)0x0001)            /*!<Capture/Compare 1 output enable                 */
#define  TIM_CCER_CC1P                       ((uint16_t)0x0002)            /*!<Capture/Compare 1 output Polarity               */
#define  TIM_CCER_CC1NE                      ((uint16_t)0x0004)            /*!<Capture/Compare 1 Complementary output enable   */
#define  TIM_CCER_CC1NP                      ((uint16_t)0x0008)            /*!<Capture/Compare 1 Complementary output Polarity */
#define  TIM_CCER_CC2E                       ((uint16_t)0x0010)            /*!<Capture/Compare 2 output enable                 */
#define  TIM_CCER_CC2P                       ((uint16_t)0x0020)            /*!<Capture/Compare 2 output Polarity               */
#define  TIM_CCER_CC2NE                      ((uint16_t)0x0040)            /*!<Capture/Compare 2 Complementary output enable   */
#define  TIM_CCER_CC2NP                      ((uint16_t)0x0080)            /*!<Capture/Compare 2 Complementary output Polarity */
#define  TIM_CCER_CC3E                       ((uint16_t)0x0100)            /*!<Capture/Compare 3 output enable                 */
#define  TIM_CCER_CC3P                       ((uint16_t)0x0200)            /*!<Capture/Compare 3 output Polarity               */
#define  TIM_CCER_CC3NE                      ((uint16_t)0x0400)            /*!<Capture/Compare 3 Complementary output enable   */
#define  TIM_CCER_CC3NP                      ((uint16_t)0x0800)            /*!<Capture/Compare 3 Complementary output Polarity */
#define  TIM_CCER_CC4E                       ((uint16_t)0x1000)            /*!<Capture/Compare 4 output enable                 */
#define  TIM_CCER_CC4P                       ((uint16_t)0x2000)            /*!<Capture/Compare 4 output Polarity               */
#define  TIM_CCER_CC4NP                      ((uint16_t)0x8000)            /*!<Capture/Compare 4 Complementary output Polarity */

/******************  Bit definition for TIM_CCMR1 register  *******************/
#define  TIM_CCMR1_CC1S                      ((uint16_t)0x0003)            /*!<CC1S[1:0] bits (Capture/Compare 1 Selection) */
#define  TIM_CCMR1_CC1S_0                    ((uint16_t)0x0001)            /*!<Bit 0 */
#define  TIM_CCMR1_CC1S_1                    ((uint16_t)0x0002)            /*!<Bit 1 */
#define  TIM_CCMR1_OC1FE                     ((uint16_t)0x0004)            /*!<Output Compare 1 Fast enable                 */
#define  TIM_CCMR1_OC1PE                     ((uint16_t)0x0008)            /*!<Output Compare 1 Preload enable              */
#define  TIM_CCMR1_OC1M                      ((uint16_t)0x0070)            /*!<OC1M[2:0] bits (Output Compare 1 Mode)       */
#define  TIM_CCMR1_OC1M_0                    ((uint16_t)0x0010)            /*!<Bit 0 */
#define  TIM_CCMR1_OC1M_1                    ((uint16_t)0x0020)            /*!<Bit 1 */
#define  TIM_CCMR1_OC1M_2                    ((uint16_t)0x0040)            /*!<Bit 2 */
#define  TIM_CCMR1_OC1CE                     ((uint16_t)0x0080)            /*!<Output Compare 1Clear Enable                 */
#define  TIM_CCMR1_CC2S                      ((uint16_t)0x0300)            /*!<CC2S[1:0] bits (Capture/Compare 2 Selection) */
#define  TIM_CCMR1_CC2S_0                    ((uint16_t)0x0100)            /*!<Bit 0 */
#define  TIM_CCMR1_CC2S_1                    ((uint16_t)0x0200)            /*!<Bit 1 */
#define  TIM_CCMR1_OC2FE                     ((uint16_t)0x0400)            /*!<Output Compare 2 Fast enable                 */
#define  TIM_CCMR1_OC2PE                     ((uint16_t)0x0800)            /*!<Output Compare 2 Preload enable              */
#define  TIM_CCMR1_OC2M                      ((uint16_t)0x7000)            /*!<OC2M[2:0] bits (Output Compare 2 Mode)       */
#define  TIM_CCMR1_OC2M_0                    ((uint16_t)0x1000)            /*!<Bit 0 */
#define  TIM_CCMR1_OC2M_1                    ((uint16_t)0x2000)            /*!<Bit 1 */
#define  TIM_CCMR1_OC2M_2                    ((uint16_t)0x4000)            /*!<Bit 2 */
#define  TIM_CCMR1_OC2CE                     ((uint16_t)0x8000)            /*!<Output Compare 2 Clear Enable */
/*******************  Bit definition for TIM_CR2 register  ********************/
#define  TIM_CR2_CCPC                        ((uint16_t)0x0001)            /*!<Capture/Compare Preloaded Control        */
#define  TIM_CR2_CCUS                        ((uint16_t)0x0004)            /*!<Capture/Compare Control Update Selection */
#define  TIM_CR2_CCDS                        ((uint16_t)0x0008)            /*!<Capture/Compare DMA Selection            */
#define  TIM_CR2_MMS                         ((uint16_t)0x0070)            /*!<MMS[2:0] bits (Master Mode Selection) */
#define  TIM_CR2_MMS_0                       ((uint16_t)0x0010)            /*!<Bit 0 */
#define  TIM_CR2_MMS_1                       ((uint16_t)0x0020)            /*!<Bit 1 */
#define  TIM_CR2_MMS_2                       ((uint16_t)0x0040)            /*!<Bit 2 */
#define  TIM_CR2_TI1S                        ((uint16_t)0x0080)            /*!<TI1 Selection */
#define  TIM_CR2_OIS1                        ((uint16_t)0x0100)            /*!<Output Idle state 1 (OC1 output)  */
#define  TIM_CR2_OIS1N                       ((uint16_t)0x0200)            /*!<Output Idle state 1 (OC1N output) */
#define  TIM_CR2_OIS2                        ((uint16_t)0x0400)            /*!<Output Idle state 2 (OC2 output)  */
#define  TIM_CR2_OIS2N                       ((uint16_t)0x0800)            /*!<Output Idle state 2 (OC2N output) */
#define  TIM_CR2_OIS3                        ((uint16_t)0x1000)            /*!<Output Idle state 3 (OC3 output)  */
#define  TIM_CR2_OIS3N                       ((uint16_t)0x2000)            /*!<Output Idle state 3 (OC3N output) */
#define  TIM_CR2_OIS4                        ((uint16_t)0x4000)            /*!<Output Idle state 4 (OC4 output)  */
/******************  Bit definition for TIM_CCMR2 register  *******************/
#define  TIM_CCMR2_CC3S                      ((uint16_t)0x0003)            /*!<CC3S[1:0] bits (Capture/Compare 3 Selection)  */
#define  TIM_CCMR2_CC3S_0                    ((uint16_t)0x0001)            /*!<Bit 0 */
#define  TIM_CCMR2_CC3S_1                    ((uint16_t)0x0002)            /*!<Bit 1 */
#define  TIM_CCMR2_OC3FE                     ((uint16_t)0x0004)            /*!<Output Compare 3 Fast enable           */
#define  TIM_CCMR2_OC3PE                     ((uint16_t)0x0008)            /*!<Output Compare 3 Preload enable        */
#define  TIM_CCMR2_OC3M                      ((uint16_t)0x0070)            /*!<OC3M[2:0] bits (Output Compare 3 Mode) */
#define  TIM_CCMR2_OC3M_0                    ((uint16_t)0x0010)            /*!<Bit 0 */
#define  TIM_CCMR2_OC3M_1                    ((uint16_t)0x0020)            /*!<Bit 1 */
#define  TIM_CCMR2_OC3M_2                    ((uint16_t)0x0040)            /*!<Bit 2 */
#define  TIM_CCMR2_OC3CE                     ((uint16_t)0x0080)            /*!<Output Compare 3 Clear Enable */
#define  TIM_CCMR2_CC4S                      ((uint16_t)0x0300)            /*!<CC4S[1:0] bits (Capture/Compare 4 Selection) */
#define  TIM_CCMR2_CC4S_0                    ((uint16_t)0x0100)            /*!<Bit 0 */
#define  TIM_CCMR2_CC4S_1                    ((uint16_t)0x0200)            /*!<Bit 1 */
#define  TIM_CCMR2_OC4FE                     ((uint16_t)0x0400)            /*!<Output Compare 4 Fast enable    */
#define  TIM_CCMR2_OC4PE                     ((uint16_t)0x0800)            /*!<Output Compare 4 Preload enable */
#define  TIM_CCMR2_OC4M                      ((uint16_t)0x7000)            /*!<OC4M[2:0] bits (Output Compare 4 Mode) */
#define  TIM_CCMR2_OC4M_0                    ((uint16_t)0x1000)            /*!<Bit 0 */
#define  TIM_CCMR2_OC4M_1                    ((uint16_t)0x2000)            /*!<Bit 1 */
#define  TIM_CCMR2_OC4M_2                    ((uint16_t)0x4000)            /*!<Bit 2 */
#define  TIM_CCMR2_OC4CE                     ((uint16_t)0x8000)            /*!<Output Compare 4 Clear Enable */


#define  GPIO_AF_TIM1                        ((uint8_t)0x01)  /* TIM1 Alternate Function mapping */
#define  GPIO_AF_TIM2                        ((uint8_t)0x01)  /* TIM2 Alternate Function mapping */
#define  GPIO_AF_TIM3                        ((uint8_t)0x02)  /* TIM3 Alternate Function mapping */
#define  GPIO_AF_TIM4                        ((uint8_t)0x02)  /* TIM4 Alternate Function mapping */
#define  GPIO_AF_TIM5                        ((uint8_t)0x02)  /* TIM5 Alternate Function mapping */
#define  GPIO_AF_TIM8                        ((uint8_t)0x03)  /* TIM8 Alternate Function mapping */
#define  GPIO_AF_TIM9                        ((uint8_t)0x03)  /* TIM9 Alternate Function mapping */
#define  GPIO_AF_TIM10                       ((uint8_t)0x03)  /* TIM10 Alternate Function mapping */
#define  GPIO_AF_TIM11                       ((uint8_t)0x03)  /* TIM11 Alternate Function mapping */

#define  TIM_FLAG_Update                     ((uint16_t)0x0001)
#define  TIM_FLAG_CC1                        ((uint16_t)0x0002)
#define  TIM_FLAG_CC2                        ((uint16_t)0x0004)
#define  TIM_FLAG_CC3                        ((uint16_t)0x0008)
#define  TIM_FLAG_CC4                        ((uint16_t)0x0010)
#define  TIM_FLAG_COM                        ((uint16_t)0x0020)
#define  TIM_FLAG_Trigger                    ((uint16_t)0x0040)
#define  TIM_FLAG_Break                      ((uint16_t)0x0080)
#define  TIM_FLAG_CC1OF                      ((uint16_t)0x0200)
#define  TIM_FLAG_CC2OF                      ((uint16_t)0x0400)
#define  TIM_FLAG_CC3OF                      ((uint16_t)0x0800)
#define  TIM_FLAG_CC4OF                      ((uint16_t)0x1000)



#define  TIM_CR1_DIR                          ((uint16_t)0x0010)
#define  TIM_CR1_CMS                          ((uint16_t)0x0060)            /*!<CMS[1:0] bits (Center-aligned mode selection) */
#define  TIM_CR1_CMS_0                        ((uint16_t)0x0020)            /*!<Bit 0 */
#define  TIM_CR1_CMS_1                        ((uint16_t)0x0040)            /*!<Bit 1 */
#define  TIM_CR1_CKD                          ((uint16_t)0x0300)            /*!<CKD[1:0] bits (clock division) */
#define  TIM_CR1_CKD_0                        ((uint16_t)0x0100)            /*!<Bit 0 */
#define  TIM_CR1_CKD_1                        ((uint16_t)0x0200)            /*!<Bit 1 */
#define  TIM_PSCReloadMode_Immediate          ((uint16_t)0x0001)
#define  TIM_CR1_CEN                          ((uint16_t)0x0001)
#define  TIM_OCMode_PWM2                      ((uint16_t)0x0070)
#define  TIM_OCPolarity_Low                   ((uint16_t)0x0002)
#define  TIM_CCER_CC1E                        ((uint16_t)0x0001)            /*!<Capture/Compare 1 output enable                 */
#define  TIM_CCMR1_OC1M                       ((uint16_t)0x0070)            /*!<OC1M[2:0] bits (Output Compare 1 Mode)       */
#define  TIM_CCMR1_CC1S                       ((uint16_t)0x0003)            /*!<CC1S[1:0] bits (Capture/Compare 1 Selection) */
#define  TIM_CCER_CC1NE                       ((uint16_t)0x0004)            /*!<Capture/Compare 1 Complementary output enable   */
#define  TIM_CR2_OIS1                         ((uint16_t)0x0100)            /*!<Output Idle state 1 (OC1 output)  */
#define  TIM_CR2_OIS1N                        ((uint16_t)0x0200)            /*!<Output Idle state 1 (OC1N output) */
#define  TIM_CCMR1_OC1PE                      ((uint16_t)0x0008)            /*!<Output Compare 1 Preload enable              */


#define  TIM_SR_UIF                          ((uint16_t)0x0001)            /*!<Update interrupt Flag              */
#define  TIM_SR_CC1IF                        ((uint16_t)0x0002)            /*!<Capture/Compare 1 interrupt Flag   */
#define  TIM_SR_CC2IF                        ((uint16_t)0x0004)            /*!<Capture/Compare 2 interrupt Flag   */
#define  TIM_SR_CC3IF                        ((uint16_t)0x0008)            /*!<Capture/Compare 3 interrupt Flag   */
#define  TIM_SR_CC4IF                        ((uint16_t)0x0010)            /*!<Capture/Compare 4 interrupt Flag   */
#define  TIM_SR_COMIF                        ((uint16_t)0x0020)            /*!<COM interrupt Flag                 */
#define  TIM_SR_TIF                          ((uint16_t)0x0040)            /*!<Trigger interrupt Flag             */
#define  TIM_SR_BIF                          ((uint16_t)0x0080)            /*!<Break interrupt Flag               */
#define  TIM_SR_CC1OF                        ((uint16_t)0x0200)            /*!<Capture/Compare 1 Overcapture Flag */
#define  TIM_SR_CC2OF                        ((uint16_t)0x0400)            /*!<Capture/Compare 2 Overcapture Flag */
#define  TIM_SR_CC3OF                        ((uint16_t)0x0800)            /*!<Capture/Compare 3 Overcapture Flag */
#define  TIM_SR_CC4OF                        ((uint16_t)0x1000)            /*!<Capture/Compare 4 Overcapture Flag */


#endif