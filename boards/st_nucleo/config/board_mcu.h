#ifndef __BOARD_MCU__
#define __BOARD_MCU__


#define PLATFORM_ARMCMx

//defines for STM32F4 libraries
#define STM32F401xx 

#define STM32F401xE

#define STM32F401RE




//VHAL Drivers
#define VHAL_GPIO 1
#define VHAL_EXT 1
#define VHAL_SER 1
#define VHAL_NFO 1
#define VHAL_FLASH 1
#define VHAL_RNG 1
#define VHAL_SPI 0
#define VHAL_ADC 0
#define VHAL_HTM 0
#define VHAL_TIM 0
#define BOARD_HAS_RNG   0


//VHAL
#define VHAL_ADC_PRESCALER                  4

#define VHAL_SPI1_RX_DMA_STREAM 10
#define VHAL_SPI1_TX_DMA_STREAM 11
#define VHAL_SPI1_RX_DMA_CHANNEL 3
#define VHAL_SPI1_TX_DMA_CHANNEL 3

#define VHAL_SPI2_RX_DMA_STREAM 3
#define VHAL_SPI2_TX_DMA_STREAM 4
#define VHAL_SPI2_RX_DMA_CHANNEL 0
#define VHAL_SPI2_TX_DMA_CHANNEL 0

#define VHAL_SPI3_RX_DMA_STREAM 2
#define VHAL_SPI3_TX_DMA_STREAM 7
#define VHAL_SPI3_RX_DMA_CHANNEL 0
#define VHAL_SPI3_TX_DMA_CHANNEL 0



#endif