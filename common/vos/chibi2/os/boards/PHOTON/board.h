/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#ifndef _BOARD_H_
#define _BOARD_H_

/*
 * Setup for STMicroelectronics STM3220G-EVAL board.
 */

/*
 * Board identifier.
 */
#define BOARD_PARTICLE_PHOTON
#define BOARD_NAME              "Particle Photon"

/*
 * Board frequencies.
 * NOTE: The HSE crystal is not fitted by default on the board.
 */
//#define STM32_LSECLK            32768
//#define STM32_HSECLK            25000000



/*
 * MCU type as defined in the ST header file stm32f2xx.h.
 */
#define STM32F2XX


#define IOPORTA GPIOA
#define IOPORTB GPIOB
#define IOPORTC GPIOC
#define IOPORTD GPIOD
#define IOPORTE GPIOE
#define IOPORTF GPIOF
#define IOPORTG GPIOG
#define IOPORTH GPIOH

/*
 * I/O ports initial setup, this configuration is established soon after reset
 * in the initialization code.
 * Please refer to the STM32 Reference Manual for details.
 */
#define PIN_MODE_INPUT(n)           (0 << ((n) * 2))
#define PIN_MODE_OUTPUT(n)          (1 << ((n) * 2))
#define PIN_MODE_ALTERNATE(n)       (2 << ((n) * 2))
#define PIN_MODE_ANALOG(n)          (3 << ((n) * 2))
#define PIN_OTYPE_PUSHPULL(n)       (0 << (n))
#define PIN_OTYPE_OPENDRAIN(n)      (1 << (n))
#define PIN_OSPEED_2M(n)            (0 << ((n) * 2))
#define PIN_OSPEED_25M(n)           (1 << ((n) * 2))
#define PIN_OSPEED_50M(n)           (2 << ((n) * 2))
#define PIN_OSPEED_100M(n)          (3 << ((n) * 2))
#define PIN_PUDR_FLOATING(n)        (0 << ((n) * 2))
#define PIN_PUDR_PULLUP(n)          (1 << ((n) * 2))
#define PIN_PUDR_PULLDOWN(n)        (2 << ((n) * 2))
#define PIN_AFIO_AF(n, v)           ((v##U) << ((n % 8) * 4))
#define PIN_ODR_LOW(n)              (0U << (n))
#define PIN_ODR_HIGH(n)             (1U << (n))




/*
 * IO pins assignments.
 */
#define GPIOA_BUTTON_WKUP           0
#define GPIOA_ETH_RMII_REF_CLK      1
#define GPIOA_ETH_RMII_MDIO         2
#define GPIOA_ETH_RMII_MDINT        3
#define GPIOA_PIN4                  4
#define GPIOA_PIN5                  5
#define GPIOA_PIN6                  6
#define GPIOA_ETH_RMII_CRS_DV       7
#define GPIOA_USB_HS_BUSON          8
#define GPIOA_OTG_FS_VBUS           9
#define GPIOA_OTG_FS_ID             10
#define GPIOA_OTG_FS_DM             11
#define GPIOA_OTG_FS_DP             12
#define GPIOA_JTAG_TMS              13
#define GPIOA_JTAG_TCK              14
#define GPIOA_JTAG_TDI              15
/*
 * Port A setup.
 * All input with pull-up except:
 * PA8  - MCO 1         (alternate 0).
 * PA13 - JTMS/SWDAT    (alternate 0).
 * PA14 - JTCK/SWCLK    (alternate 0).
 * PA15 - JTDI          (alternate 0).
 */
/*
 * GPIOA setup:
 *
 * PA0  - BUTTON_WKUP               (input floating).
 * PA1  - ETH_RMII_REF_CLK          (alternate 11).
 * PA2  - ETH_RMII_MDIO             (alternate 11).
 * PA3  - ETH_RMII_MDINT            (input floating).
 * PA4  - PIN4                      (input pullup).
 * PA5  - PIN5                      (input pullup).
 * PA6  - PIN6                      (input pullup).
 * PA7  - ETH_RMII_CRS_DV           (alternate 11).
 * PA8  - USB_HS_BUSON              (output pushpull maximum).
 * PA9  - OTG_FS_VBUS               (input pulldown).
 * PA10 - OTG_FS_ID                 (alternate 10).
 * PA11 - OTG_FS_DM                 (alternate 10).
 * PA12 - OTG_FS_DP                 (alternate 10).
 * PA13 - JTAG_TMS                  (alternate 0).
 * PA14 - JTAG_TCK                  (alternate 0).
 * PA15 - JTAG_TDI                  (alternate 0).
 */
#define VAL_GPIOA_MODER             (PIN_MODE_INPUT(GPIOA_BUTTON_WKUP) |    \
                                     PIN_MODE_ALTERNATE(GPIOA_ETH_RMII_REF_CLK) |\
                                     PIN_MODE_ALTERNATE(GPIOA_ETH_RMII_MDIO) |\
                                     PIN_MODE_INPUT(GPIOA_ETH_RMII_MDINT) | \
                                     PIN_MODE_INPUT(GPIOA_PIN4) |           \
                                     PIN_MODE_INPUT(GPIOA_PIN5) |           \
                                     PIN_MODE_INPUT(GPIOA_PIN6) |           \
                                     PIN_MODE_ALTERNATE(GPIOA_ETH_RMII_CRS_DV) |\
                                     PIN_MODE_OUTPUT(GPIOA_USB_HS_BUSON) |  \
                                     PIN_MODE_INPUT(GPIOA_OTG_FS_VBUS) |    \
                                     PIN_MODE_ALTERNATE(GPIOA_OTG_FS_ID) |  \
                                     PIN_MODE_ALTERNATE(GPIOA_OTG_FS_DM) |  \
                                     PIN_MODE_ALTERNATE(GPIOA_OTG_FS_DP) |  \
                                     PIN_MODE_ALTERNATE(GPIOA_JTAG_TMS) |   \
                                     PIN_MODE_ALTERNATE(GPIOA_JTAG_TCK) |   \
                                     PIN_MODE_ALTERNATE(GPIOA_JTAG_TDI))
#define VAL_GPIOA_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOA_BUTTON_WKUP) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_ETH_RMII_REF_CLK) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_ETH_RMII_MDIO) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_ETH_RMII_MDINT) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN6) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOA_ETH_RMII_CRS_DV) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_USB_HS_BUSON) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_OTG_FS_VBUS) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_OTG_FS_ID) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOA_OTG_FS_DM) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOA_OTG_FS_DP) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOA_JTAG_TMS) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOA_JTAG_TCK) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOA_JTAG_TDI))
#define VAL_GPIOA_OSPEEDR           (PIN_OSPEED_100M(GPIOA_BUTTON_WKUP) |   \
                                     PIN_OSPEED_100M(GPIOA_ETH_RMII_REF_CLK) |\
                                     PIN_OSPEED_100M(GPIOA_ETH_RMII_MDIO) | \
                                     PIN_OSPEED_100M(GPIOA_ETH_RMII_MDINT) |\
                                     PIN_OSPEED_100M(GPIOA_PIN4) |          \
                                     PIN_OSPEED_100M(GPIOA_PIN5) |          \
                                     PIN_OSPEED_100M(GPIOA_PIN6) |          \
                                     PIN_OSPEED_100M(GPIOA_ETH_RMII_CRS_DV) |\
                                     PIN_OSPEED_100M(GPIOA_USB_HS_BUSON) |  \
                                     PIN_OSPEED_100M(GPIOA_OTG_FS_VBUS) |   \
                                     PIN_OSPEED_100M(GPIOA_OTG_FS_ID) |     \
                                     PIN_OSPEED_100M(GPIOA_OTG_FS_DM) |     \
                                     PIN_OSPEED_100M(GPIOA_OTG_FS_DP) |     \
                                     PIN_OSPEED_100M(GPIOA_JTAG_TMS) |      \
                                     PIN_OSPEED_100M(GPIOA_JTAG_TCK) |      \
                                     PIN_OSPEED_100M(GPIOA_JTAG_TDI))
#define VAL_GPIOA_PUPDR             (PIN_PUDR_FLOATING(GPIOA_BUTTON_WKUP) |\
                                     PIN_PUDR_FLOATING(GPIOA_ETH_RMII_REF_CLK) |\
                                     PIN_PUDR_FLOATING(GPIOA_ETH_RMII_MDIO) |\
                                     PIN_PUDR_FLOATING(GPIOA_ETH_RMII_MDINT) |\
                                     PIN_PUDR_PULLUP(GPIOA_PIN4) |         \
                                     PIN_PUDR_PULLUP(GPIOA_PIN5) |         \
                                     PIN_PUDR_PULLUP(GPIOA_PIN6) |         \
                                     PIN_PUDR_FLOATING(GPIOA_ETH_RMII_CRS_DV) |\
                                     PIN_PUDR_FLOATING(GPIOA_USB_HS_BUSON) |\
                                     PIN_PUDR_PULLDOWN(GPIOA_OTG_FS_VBUS) |\
                                     PIN_PUDR_FLOATING(GPIOA_OTG_FS_ID) |  \
                                     PIN_PUDR_FLOATING(GPIOA_OTG_FS_DM) |  \
                                     PIN_PUDR_FLOATING(GPIOA_OTG_FS_DP) |  \
                                     PIN_PUDR_FLOATING(GPIOA_JTAG_TMS) |   \
                                     PIN_PUDR_PULLDOWN(GPIOA_JTAG_TCK) |   \
                                     PIN_PUDR_FLOATING(GPIOA_JTAG_TDI))
#define VAL_GPIOA_ODR               (PIN_ODR_HIGH(GPIOA_BUTTON_WKUP) |      \
                                     PIN_ODR_HIGH(GPIOA_ETH_RMII_REF_CLK) | \
                                     PIN_ODR_HIGH(GPIOA_ETH_RMII_MDIO) |    \
                                     PIN_ODR_HIGH(GPIOA_ETH_RMII_MDINT) |   \
                                     PIN_ODR_HIGH(GPIOA_PIN4) |             \
                                     PIN_ODR_HIGH(GPIOA_PIN5) |             \
                                     PIN_ODR_HIGH(GPIOA_PIN6) |             \
                                     PIN_ODR_HIGH(GPIOA_ETH_RMII_CRS_DV) |  \
                                     PIN_ODR_LOW(GPIOA_USB_HS_BUSON) |      \
                                     PIN_ODR_HIGH(GPIOA_OTG_FS_VBUS) |      \
                                     PIN_ODR_HIGH(GPIOA_OTG_FS_ID) |        \
                                     PIN_ODR_HIGH(GPIOA_OTG_FS_DM) |        \
                                     PIN_ODR_HIGH(GPIOA_OTG_FS_DP) |        \
                                     PIN_ODR_HIGH(GPIOA_JTAG_TMS) |         \
                                     PIN_ODR_HIGH(GPIOA_JTAG_TCK) |         \
                                     PIN_ODR_HIGH(GPIOA_JTAG_TDI))
#define VAL_GPIOA_AFRL              (PIN_AFIO_AF(GPIOA_BUTTON_WKUP, 0) |    \
                                     PIN_AFIO_AF(GPIOA_ETH_RMII_REF_CLK, 11) |\
                                     PIN_AFIO_AF(GPIOA_ETH_RMII_MDIO, 11) | \
                                     PIN_AFIO_AF(GPIOA_ETH_RMII_MDINT, 0) | \
                                     PIN_AFIO_AF(GPIOA_PIN4, 0) |           \
                                     PIN_AFIO_AF(GPIOA_PIN5, 0) |           \
                                     PIN_AFIO_AF(GPIOA_PIN6, 0) |           \
                                     PIN_AFIO_AF(GPIOA_ETH_RMII_CRS_DV, 11))
#define VAL_GPIOA_AFRH              (PIN_AFIO_AF(GPIOA_USB_HS_BUSON, 0) |   \
                                     PIN_AFIO_AF(GPIOA_OTG_FS_VBUS, 0) |    \
                                     PIN_AFIO_AF(GPIOA_OTG_FS_ID, 10) |     \
                                     PIN_AFIO_AF(GPIOA_OTG_FS_DM, 10) |     \
                                     PIN_AFIO_AF(GPIOA_OTG_FS_DP, 10) |     \
                                     PIN_AFIO_AF(GPIOA_JTAG_TMS, 0) |       \
                                     PIN_AFIO_AF(GPIOA_JTAG_TCK, 0) |       \
                                     PIN_AFIO_AF(GPIOA_JTAG_TDI, 0))

/*
 * GPIOB setup:
 *
 * PB0  - USB_FS_BUSON              (output pushpull maximum).
 * PB1  - USB_HS_FAULT              (input floating).
 * PB2  - BOOT1                     (input floating).
 * PB3  - JTAG_TDO                  (alternate 0).
 * PB4  - JTAG_TRST                 (alternate 0).
 * PB5  - PIN5                      (input pullup).
 * PB6  - PIN6                      (input pullup).
 * PB7  - PIN7                      (input pullup).
 * PB8  - I2C1_SCL                  (alternate 4).
 * PB9  - I2C1_SDA                  (alternate 4).
 * PB10 - SPI2_SCK                  (alternate 5).
 * PB11 - PIN11                     (input pullup).
 * PB12 - OTG_HS_ID                 (alternate 12).
 * PB13 - OTG_HS_VBUS               (input pulldown).
 * PB14 - OTG_HS_DM                 (alternate 12).
 * PB15 - OTG_HS_DP                 (alternate 12).
 */
#define GPIOB_USB_FS_BUSON          0
#define GPIOB_USB_HS_FAULT          1
#define GPIOB_BOOT1                 2
#define GPIOB_JTAG_TDO              3
#define GPIOB_JTAG_TRST             4
#define GPIOB_PIN5                  5
#define GPIOB_PIN6                  6
#define GPIOB_PIN7                  7
#define GPIOB_I2C1_SCL              8
#define GPIOB_I2C1_SDA              9
#define GPIOB_SPI2_SCK              10
#define GPIOB_PIN11                 11
#define GPIOB_OTG_HS_ID             12
#define GPIOB_OTG_HS_VBUS           13
#define GPIOB_OTG_HS_DM             14
#define GPIOB_OTG_HS_DP             15


#define VAL_GPIOB_MODER             (PIN_MODE_OUTPUT(GPIOB_USB_FS_BUSON) |  \
                                     PIN_MODE_INPUT(GPIOB_USB_HS_FAULT) |   \
                                     PIN_MODE_INPUT(GPIOB_BOOT1) |          \
                                     PIN_MODE_ALTERNATE(GPIOB_JTAG_TDO) |   \
                                     PIN_MODE_ALTERNATE(GPIOB_JTAG_TRST) |  \
                                     PIN_MODE_INPUT(GPIOB_PIN5) |           \
                                     PIN_MODE_INPUT(GPIOB_PIN6) |           \
                                     PIN_MODE_INPUT(GPIOB_PIN7) |           \
                                     PIN_MODE_ALTERNATE(GPIOB_I2C1_SCL) |   \
                                     PIN_MODE_ALTERNATE(GPIOB_I2C1_SDA) |   \
                                     PIN_MODE_ALTERNATE(GPIOB_SPI2_SCK) |   \
                                     PIN_MODE_INPUT(GPIOB_PIN11) |          \
                                     PIN_MODE_INPUT(GPIOB_OTG_HS_ID) |  \
                                     PIN_MODE_INPUT(GPIOB_OTG_HS_VBUS) |    \
                                     PIN_MODE_ALTERNATE(GPIOB_OTG_HS_DM) |  \
                                     PIN_MODE_ALTERNATE(GPIOB_OTG_HS_DP))
#define VAL_GPIOB_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOB_USB_FS_BUSON) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_USB_HS_FAULT) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_BOOT1) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOB_JTAG_TDO) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOB_JTAG_TRST) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN6) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN7) |       \
                                     PIN_OTYPE_OPENDRAIN(GPIOB_I2C1_SCL) |  \
                                     PIN_OTYPE_OPENDRAIN(GPIOB_I2C1_SDA) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOB_SPI2_SCK) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN11) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOB_OTG_HS_ID) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOB_OTG_HS_VBUS) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_OTG_HS_DM) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOB_OTG_HS_DP))
#define VAL_GPIOB_OSPEEDR           (PIN_OSPEED_100M(GPIOB_USB_FS_BUSON) |  \
                                     PIN_OSPEED_100M(GPIOB_USB_HS_FAULT) |  \
                                     PIN_OSPEED_100M(GPIOB_BOOT1) |         \
                                     PIN_OSPEED_100M(GPIOB_JTAG_TDO) |      \
                                     PIN_OSPEED_100M(GPIOB_JTAG_TRST) |     \
                                     PIN_OSPEED_100M(GPIOB_PIN5) |          \
                                     PIN_OSPEED_100M(GPIOB_PIN6) |          \
                                     PIN_OSPEED_100M(GPIOB_PIN7) |          \
                                     PIN_OSPEED_100M(GPIOB_I2C1_SCL) |      \
                                     PIN_OSPEED_100M(GPIOB_I2C1_SDA) |      \
                                     PIN_OSPEED_100M(GPIOB_SPI2_SCK) |      \
                                     PIN_OSPEED_100M(GPIOB_PIN11) |         \
                                     PIN_OSPEED_100M(GPIOB_OTG_HS_ID) |     \
                                     PIN_OSPEED_100M(GPIOB_OTG_HS_VBUS) |   \
                                     PIN_OSPEED_100M(GPIOB_OTG_HS_DM) |     \
                                     PIN_OSPEED_100M(GPIOB_OTG_HS_DP))
#define VAL_GPIOB_PUPDR             (PIN_PUDR_FLOATING(GPIOB_USB_FS_BUSON) |\
                                     PIN_PUDR_FLOATING(GPIOB_USB_HS_FAULT) |\
                                     PIN_PUDR_FLOATING(GPIOB_BOOT1) |      \
                                     PIN_PUDR_FLOATING(GPIOB_JTAG_TDO) |   \
                                     PIN_PUDR_FLOATING(GPIOB_JTAG_TRST) |  \
                                     PIN_PUDR_PULLUP(GPIOB_PIN5) |         \
                                     PIN_PUDR_PULLUP(GPIOB_PIN6) |         \
                                     PIN_PUDR_PULLUP(GPIOB_PIN7) |         \
                                     PIN_PUDR_FLOATING(GPIOB_I2C1_SCL) |   \
                                     PIN_PUDR_FLOATING(GPIOB_I2C1_SDA) |   \
                                     PIN_PUDR_FLOATING(GPIOB_SPI2_SCK) |   \
                                     PIN_PUDR_PULLUP(GPIOB_PIN11) |        \
                                     PIN_PUDR_FLOATING(GPIOB_OTG_HS_ID) |  \
                                     PIN_PUDR_PULLDOWN(GPIOB_OTG_HS_VBUS) |\
                                     PIN_PUDR_FLOATING(GPIOB_OTG_HS_DM) |  \
                                     PIN_PUDR_FLOATING(GPIOB_OTG_HS_DP))
#define VAL_GPIOB_ODR               (PIN_ODR_LOW(GPIOB_USB_FS_BUSON) |      \
                                     PIN_ODR_HIGH(GPIOB_USB_HS_FAULT) |     \
                                     PIN_ODR_HIGH(GPIOB_BOOT1) |            \
                                     PIN_ODR_HIGH(GPIOB_JTAG_TDO) |         \
                                     PIN_ODR_HIGH(GPIOB_JTAG_TRST) |        \
                                     PIN_ODR_HIGH(GPIOB_PIN5) |             \
                                     PIN_ODR_HIGH(GPIOB_PIN6) |             \
                                     PIN_ODR_HIGH(GPIOB_PIN7) |             \
                                     PIN_ODR_HIGH(GPIOB_I2C1_SCL) |         \
                                     PIN_ODR_HIGH(GPIOB_I2C1_SDA) |         \
                                     PIN_ODR_HIGH(GPIOB_SPI2_SCK) |         \
                                     PIN_ODR_HIGH(GPIOB_PIN11) |            \
                                     PIN_ODR_HIGH(GPIOB_OTG_HS_ID) |        \
                                     PIN_ODR_HIGH(GPIOB_OTG_HS_VBUS) |      \
                                     PIN_ODR_HIGH(GPIOB_OTG_HS_DM) |        \
                                     PIN_ODR_HIGH(GPIOB_OTG_HS_DP))
#define VAL_GPIOB_AFRL              (PIN_AFIO_AF(GPIOB_USB_FS_BUSON, 0) |   \
                                     PIN_AFIO_AF(GPIOB_USB_HS_FAULT, 0) |   \
                                     PIN_AFIO_AF(GPIOB_BOOT1, 0) |          \
                                     PIN_AFIO_AF(GPIOB_JTAG_TDO, 0) |       \
                                     PIN_AFIO_AF(GPIOB_JTAG_TRST, 0) |      \
                                     PIN_AFIO_AF(GPIOB_PIN5, 0) |           \
                                     PIN_AFIO_AF(GPIOB_PIN6, 0) |           \
                                     PIN_AFIO_AF(GPIOB_PIN7, 0))
#define VAL_GPIOB_AFRH              (PIN_AFIO_AF(GPIOB_I2C1_SCL, 4) |       \
                                     PIN_AFIO_AF(GPIOB_I2C1_SDA, 4) |       \
                                     PIN_AFIO_AF(GPIOB_SPI2_SCK, 5) |       \
                                     PIN_AFIO_AF(GPIOB_PIN11, 0) |          \
                                     PIN_AFIO_AF(GPIOB_OTG_HS_ID, 12) |     \
                                     PIN_AFIO_AF(GPIOB_OTG_HS_VBUS, 0) |    \
                                     PIN_AFIO_AF(GPIOB_OTG_HS_DM, 12) |     \
                                     PIN_AFIO_AF(GPIOB_OTG_HS_DP, 12))

/*
 * Port C setup.
 * All input with pull-up except:
 * PC9  - MCO2          (alternate 0).
 * PC10 - USART3_TX     (alternate 7).
 * PC11 - USART3_RX     (alternate 7).
 * PC14 - OSC32_INT     (input floating).
 * PC15 - OSC32_OUT     (input floating).
 */
#define VAL_GPIOC_MODER             (PIN_MODE_ALTERNATE(9)    | \
                                    PIN_MODE_ALTERNATE(10)    | \
                                    PIN_MODE_ALTERNATE(11))
#define VAL_GPIOC_OTYPER            0x00000000
#define VAL_GPIOC_OSPEEDR           0xFFFFFFFF
#define VAL_GPIOC_PUPDR             (~(PIN_PUDR_PULLUP(11)    | \
                                       PIN_PUDR_FLOATING(14)  | \
                                       PIN_PUDR_FLOATING(15)))
#define VAL_GPIOC_ODR               0xFFFFFFFF
#define VAL_GPIOC_AFRL				0x00000000
#define VAL_GPIOC_AFRH				(PIN_AFIO_AF(7, 10)       | \
                                     PIN_AFIO_AF(7, 11))

/*
 * Port D setup.
 * All input with pull-up.
 */
#define VAL_GPIOD_MODER             0x00000000
#define VAL_GPIOD_OTYPER            0x00000000
#define VAL_GPIOD_OSPEEDR           0xFFFFFFFF
#define VAL_GPIOD_PUPDR             0xFFFFFFFF
#define VAL_GPIOD_ODR               0xFFFFFFFF
#define VAL_GPIOD_AFRL				0x00000000
#define VAL_GPIOD_AFRH				0x00000000

/*
 * Port E setup.
 * All input with pull-up.
 */
#define VAL_GPIOE_MODER             0x00000000
#define VAL_GPIOE_OTYPER            0x00000000
#define VAL_GPIOE_OSPEEDR           0xFFFFFFFF
#define VAL_GPIOE_PUPDR             0xFFFFFFFF
#define VAL_GPIOE_ODR               0xFFFFFFFF
#define VAL_GPIOE_AFRL				0x00000000
#define VAL_GPIOE_AFRH				0x00000000

/*
 * Port F setup.
 * All input with pull-up.
 */
#define VAL_GPIOF_MODER             0x00000000
#define VAL_GPIOF_OTYPER            0x00000000
#define VAL_GPIOF_OSPEEDR           0xFFFFFFFF
#define VAL_GPIOF_PUPDR             0xFFFFFFFF
#define VAL_GPIOF_ODR               0xFFFFFFFF
#define VAL_GPIOF_AFRL				0x00000000
#define VAL_GPIOF_AFRH				0x00000000

/*
 * Port G setup.
 * All input with pull-up.
 */
#define VAL_GPIOG_MODER             0x00000000
#define VAL_GPIOG_OTYPER            0x00000000
#define VAL_GPIOG_OSPEEDR           0xFFFFFFFF
#define VAL_GPIOG_PUPDR             0xFFFFFFFF
#define VAL_GPIOG_ODR               0xFFFFFFFF
#define VAL_GPIOG_AFRL				0x00000000
#define VAL_GPIOG_AFRH				0x00000000

/*
 * Port H setup.
 * All input with pull-up.
 */
#define VAL_GPIOH_MODER             0x00000000
#define VAL_GPIOH_OTYPER            0x00000000
#define VAL_GPIOH_OSPEEDR           0xFFFFFFFF
#define VAL_GPIOH_PUPDR             0xFFFFFFFF
#define VAL_GPIOH_ODR               0xFFFFFFFF
#define VAL_GPIOH_AFRL				0x00000000
#define VAL_GPIOH_AFRH				0x00000000

/*
 * Port I setup.
 * All input with pull-up.
 */
#define VAL_GPIOI_MODER             0x00000000
#define VAL_GPIOI_OTYPER            0x00000000
#define VAL_GPIOI_OSPEEDR           0xFFFFFFFF
#define VAL_GPIOI_PUPDR             0xFFFFFFFF
#define VAL_GPIOI_ODR               0xFFFFFFFF
#define VAL_GPIOI_AFRL				0x00000000
#define VAL_GPIOI_AFRH				0x00000000



#define GPIOB_USB_DISC 13
 //needed for USB to work
#define BOARD_OTG_NOVBUSSENS

/*
 * USB bus activation macro, required by the USB driver.
 */
//#define usb_lld_connect_bus(usbp) palClearPad(GPIOB, GPIOB_USB_DISC)

/*
 * USB bus de-activation macro, required by the USB driver.
 */
//#define usb_lld_disconnect_bus(usbp) palSetPad(GPIOB, GPIOB_USB_DISC)


#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif
  void boardInit(void);
#ifdef __cplusplus
}
#endif
#endif /* _FROM_ASM_ */

#endif /* _BOARD_H_ */
