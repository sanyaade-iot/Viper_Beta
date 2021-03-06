/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

/**
 * @file    GCC/ARMCMx/SAM3X/vectors.c
 * @brief   Interrupt vectors for the SAM3X family.
 *
 * @defgroup ARMCMx_SAM3X_VECTORS SAM3X Interrupt Vectors
 * @ingroup ARMCMx_SPECIFIC
 * @details Interrupt vectors for the SAM3X family.
 *
 * @{
 */

#include "ch.h"

#define NUM_VECTORS 	45

/**
 * @brief   Type of an IRQ vector.
 */
typedef void  (*irq_vector_t)(void);


/**
 * @brief   Type of a structure representing the whole vectors table.
 */
typedef struct {
  uint32_t      *init_stack;
  irq_vector_t  reset_vector;
  irq_vector_t  nmi_vector;
  irq_vector_t  hardfault_vector;
  irq_vector_t  memmanage_vector;
  irq_vector_t  busfault_vector;
  irq_vector_t  usagefault_vector;
  irq_vector_t  vector1c;
  irq_vector_t  vector20;
  irq_vector_t  vector24;
  irq_vector_t  vector28;
  irq_vector_t  svcall_vector;
  irq_vector_t  debugmonitor_vector;
  irq_vector_t  vector34;
  irq_vector_t  pendsv_vector;
  irq_vector_t  systick_vector;
  irq_vector_t  vectors[NUM_VECTORS];
} vectors_t;

#if !defined(__DOXYGEN__)
extern uint32_t __main_stack_end__;
extern void ResetHandler(void);
extern void NMIVector(void);
extern void HardFaultVector(void);
extern void MemManageVector(void);
extern void BusFaultVector(void);
extern void UsageFaultVector(void);
extern void Vector1C(void);
extern void Vector20(void);
extern void Vector24(void);
extern void Vector28(void);
extern void SVCallVector(void);
extern void DebugMonitorVector(void);
extern void Vector34(void);
extern void PendSVVector(void);
extern void SysTickVector(void);
extern void Vector40(void);	// Peripheral Id 0
extern void Vector44(void);
extern void Vector48(void);
extern void Vector4C(void);
extern void Vector50(void);
extern void Vector54(void);
extern void Vector58(void);
extern void Vector5C(void);
extern void UART_Handler(void);
extern void Vector64(void);
extern void Vector68(void);
extern void PIOA_Handler(void);
extern void PIOB_Handler(void);
extern void PIOC_Handler(void);
extern void PIOD_Handler(void);
extern void Vector7C(void);
extern void Vector80(void);
extern void USART0_Handler(void);
extern void USART1_Handler(void);
extern void USART2_Handler(void);
extern void Vector90(void);
extern void Vector94(void);
extern void TWI0_Handler(void);
extern void TWI1_Handler(void);
extern void VectorA0(void);
extern void VectorA4(void);
extern void VectorA8(void);
extern void VectorAC(void);
extern void VectorB0(void);
extern void VectorB4(void);
extern void VectorB8(void);
extern void VectorBC(void);
extern void VectorC0(void);
extern void VectorC4(void);
extern void VectorC8(void);
extern void VectorCC(void);
extern void PWM_Handler(void);
extern void ADC_Handler(void);
extern void VectorD8(void);
extern void DMA_Handler(void);
extern void VectorE0(void);
extern void VectorE4(void);
extern void VectorE8(void);
extern void VectorEC(void);
extern void VectorF0(void); // Peripheral Id 44
#endif




/**
 * @brief   STM32 vectors table.
 */
#if !defined(__DOXYGEN__)
__attribute__ ((section("vectors")))
#endif
vectors_t _vectors = {
  &__main_stack_end__,ResetHandler,       NMIVector,          HardFaultVector,
  MemManageVector,    BusFaultVector,     UsageFaultVector,   Vector1C,
  Vector20,           Vector24,           Vector28,           SVCallVector,
  DebugMonitorVector, Vector34,           PendSVVector,       SysTickVector,
  {
    Vector40,           Vector44,           Vector48,           Vector4C,
    Vector50,           Vector54,           Vector58,           Vector5C,
    UART_Handler,       Vector64,           Vector68,           PIOA_Handler,
    PIOB_Handler,       PIOC_Handler,       PIOD_Handler,       Vector7C,
    Vector80,           USART0_Handler,     USART1_Handler,     USART2_Handler,
    Vector90,           Vector94,           TWI0_Handler,       TWI1_Handler,
    VectorA0,           VectorA4,           VectorA8,           VectorAC,
    VectorB0,           VectorB4,           VectorB8,           VectorBC,
    VectorC0,           VectorC4,           VectorC8,           VectorCC,
    PWM_Handler,           ADC_Handler,        VectorD8,           DMA_Handler,
    VectorE0,           VectorE4,           VectorE8,           VectorEC,
    VectorF0
  }
};

/**
 * @brief   Unhandled exceptions handler.
 * @details Any undefined exception vector points to this function by default.
 *          This function simply stops the system into an infinite loop.
 *
 * @notapi
 */
#if !defined(__DOXYGEN__)
__attribute__ ((naked))
#endif


//void **HARDFAULT_PSP;
//register void *stack_pointer asm("sp");

void _unhandled_exception(void) {
  // Hijack the process stack pointer to make backtrace work
  //asm("mrs %0, psp" : "=r"(HARDFAULT_PSP) : :);
  //stack_pointer = HARDFAULT_PSP;
    while(1); 
}


void NMIVector(void) __attribute__((weak, alias("_unhandled_exception")));
void HardFaultVector(void) __attribute__((weak, alias("_unhandled_exception")));
void MemManageVector(void) __attribute__((weak, alias("_unhandled_exception")));
void BusFaultVector(void) __attribute__((weak, alias("_unhandled_exception")));
void UsageFaultVector(void) __attribute__((weak, alias("_unhandled_exception")));
void Vector1C(void) __attribute__((weak, alias("_unhandled_exception")));
void Vector20(void) __attribute__((weak, alias("_unhandled_exception")));
void Vector24(void) __attribute__((weak, alias("_unhandled_exception")));
void Vector28(void) __attribute__((weak, alias("_unhandled_exception")));
void SVCallVector(void) __attribute__((weak, alias("_unhandled_exception")));
void DebugMonitorVector(void) __attribute__((weak, alias("_unhandled_exception")));
void Vector34(void) __attribute__((weak, alias("_unhandled_exception")));
void PendSVVector(void) __attribute__((weak, alias("_unhandled_exception")));
void SysTickVector(void) __attribute__((weak, alias("_unhandled_exception")));
void Vector40(void) __attribute__((weak, alias("_unhandled_exception")));
void Vector44(void) __attribute__((weak, alias("_unhandled_exception")));
void Vector48(void) __attribute__((weak, alias("_unhandled_exception")));
void Vector4C(void) __attribute__((weak, alias("_unhandled_exception")));
void Vector50(void) __attribute__((weak, alias("_unhandled_exception")));
void Vector54(void) __attribute__((weak, alias("_unhandled_exception")));
void Vector58(void) __attribute__((weak, alias("_unhandled_exception")));
void Vector5C(void) __attribute__((weak, alias("_unhandled_exception")));
void UART_Handler(void) __attribute__((weak, alias("_unhandled_exception")));
void Vector64(void) __attribute__((weak, alias("_unhandled_exception")));
void Vector68(void) __attribute__((weak, alias("_unhandled_exception")));
void PIOA_Handler(void) __attribute__((weak, alias("_unhandled_exception")));
void PIOB_Handler(void) __attribute__((weak, alias("_unhandled_exception")));
void PIOC_Handler(void) __attribute__((weak, alias("_unhandled_exception")));
void PIOD_Handler(void) __attribute__((weak, alias("_unhandled_exception")));
void Vector7C(void) __attribute__((weak, alias("_unhandled_exception")));
void Vector80(void) __attribute__((weak, alias("_unhandled_exception")));
void USART0_Handler(void) __attribute__((weak, alias("_unhandled_exception")));
void USART1_Handler(void) __attribute__((weak, alias("_unhandled_exception")));
void USART2_Handler(void) __attribute__((weak, alias("_unhandled_exception")));
void Vector90(void) __attribute__((weak, alias("_unhandled_exception")));
void Vector94(void) __attribute__((weak, alias("_unhandled_exception")));
void TWI0_Handler(void) __attribute__((weak, alias("_unhandled_exception")));
void TWI1_Handler(void) __attribute__((weak, alias("_unhandled_exception")));
void VectorA0(void) __attribute__((weak, alias("_unhandled_exception")));
void VectorA4(void) __attribute__((weak, alias("_unhandled_exception")));
void VectorA8(void) __attribute__((weak, alias("_unhandled_exception")));
void VectorAC(void) __attribute__((weak, alias("_unhandled_exception")));
void VectorB0(void) __attribute__((weak, alias("_unhandled_exception")));
void VectorB4(void) __attribute__((weak, alias("_unhandled_exception")));
void VectorB8(void) __attribute__((weak, alias("_unhandled_exception")));
void VectorBC(void) __attribute__((weak, alias("_unhandled_exception")));
void VectorC0(void) __attribute__((weak, alias("_unhandled_exception")));
void VectorC4(void) __attribute__((weak, alias("_unhandled_exception")));
void VectorC8(void) __attribute__((weak, alias("_unhandled_exception")));
void VectorCC(void) __attribute__((weak, alias("_unhandled_exception")));
void PWM_Handler(void) __attribute__((weak, alias("_unhandled_exception")));
void ADC_Handler(void) __attribute__((weak, alias("_unhandled_exception")));
void VectorD8(void) __attribute__((weak, alias("_unhandled_exception")));
void DMA_Handler(void) __attribute__((weak, alias("_unhandled_exception")));
void VectorE0(void) __attribute__((weak, alias("_unhandled_exception")));
void VectorE4(void) __attribute__((weak, alias("_unhandled_exception")));
void VectorE8(void) __attribute__((weak, alias("_unhandled_exception")));
void VectorEC(void) __attribute__((weak, alias("_unhandled_exception")));
void VectorF0(void) __attribute__((weak, alias("_unhandled_exception")));

/** @} */

