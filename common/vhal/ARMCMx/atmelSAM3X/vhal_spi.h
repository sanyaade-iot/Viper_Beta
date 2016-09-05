#ifndef __VHAL_SPI__
#define __VHAL_SPI__

#include "vhal_common.h"

#define ID_SPI0   (24) /**< \brief Serial Peripheral Interface (SPI0) */
#define ID_SPI1   (25) /**< \brief Serial Peripheral Interface (SPI1) */
#define SPI0       ((Spi    *)0x40008000U) /**< \brief (SPI0      ) Base Address */
#define SPI1       ((Spi    *)0x4000C000U) /**< \brief (SPI1      ) Base Address */



typedef struct {
  WoReg SPI_CR;        /**< \brief (Spi Offset: 0x00) Control Register */
  RwReg SPI_MR;        /**< \brief (Spi Offset: 0x04) Mode Register */
  RoReg SPI_RDR;       /**< \brief (Spi Offset: 0x08) Receive Data Register */
  WoReg SPI_TDR;       /**< \brief (Spi Offset: 0x0C) Transmit Data Register */
  RoReg SPI_SR;        /**< \brief (Spi Offset: 0x10) Status Register */
  WoReg SPI_IER;       /**< \brief (Spi Offset: 0x14) Interrupt Enable Register */
  WoReg SPI_IDR;       /**< \brief (Spi Offset: 0x18) Interrupt Disable Register */
  RoReg SPI_IMR;       /**< \brief (Spi Offset: 0x1C) Interrupt Mask Register */
  RoReg Reserved1[4];
  RwReg SPI_CSR[4];    /**< \brief (Spi Offset: 0x30) Chip Select Register */
  RoReg Reserved2[41];
  RwReg SPI_WPMR;      /**< \brief (Spi Offset: 0xE4) Write Protection Control Register */
  RoReg SPI_WPSR;      /**< \brief (Spi Offset: 0xE8) Write Protection Status Register */
} Spi;


/* -------- SPI_CR : (SPI Offset: 0x00) Control Register -------- */
#define SPI_CR_SPIEN (0x1u << 0) /**< \brief (SPI_CR) SPI Enable */
#define SPI_CR_SPIDIS (0x1u << 1) /**< \brief (SPI_CR) SPI Disable */
#define SPI_CR_SWRST (0x1u << 7) /**< \brief (SPI_CR) SPI Software Reset */
#define SPI_CR_LASTXFER (0x1u << 24) /**< \brief (SPI_CR) Last Transfer */
/* -------- SPI_MR : (SPI Offset: 0x04) Mode Register -------- */
#define SPI_MR_MSTR (0x1u << 0) /**< \brief (SPI_MR) Master/Slave Mode */
#define SPI_MR_PS (0x1u << 1) /**< \brief (SPI_MR) Peripheral Select */
#define SPI_MR_PCSDEC (0x1u << 2) /**< \brief (SPI_MR) Chip Select Decode */
#define SPI_MR_MODFDIS (0x1u << 4) /**< \brief (SPI_MR) Mode Fault Detection */
#define SPI_MR_WDRBT (0x1u << 5) /**< \brief (SPI_MR) Wait Data Read Before Transfer */
#define SPI_MR_LLB (0x1u << 7) /**< \brief (SPI_MR) Local Loopback Enable */
#define SPI_MR_PCS_Pos 16
#define SPI_MR_PCS_Msk (0xfu << SPI_MR_PCS_Pos) /**< \brief (SPI_MR) Peripheral Chip Select */
#define SPI_MR_PCS(value) ((SPI_MR_PCS_Msk & ((value) << SPI_MR_PCS_Pos)))
#define SPI_MR_DLYBCS_Pos 24
#define SPI_MR_DLYBCS_Msk (0xffu << SPI_MR_DLYBCS_Pos) /**< \brief (SPI_MR) Delay Between Chip Selects */
#define SPI_MR_DLYBCS(value) ((SPI_MR_DLYBCS_Msk & ((value) << SPI_MR_DLYBCS_Pos)))
/* -------- SPI_RDR : (SPI Offset: 0x08) Receive Data Register -------- */
#define SPI_RDR_RD_Pos 0
#define SPI_RDR_RD_Msk (0xffffu << SPI_RDR_RD_Pos) /**< \brief (SPI_RDR) Receive Data */
#define SPI_RDR_PCS_Pos 16
#define SPI_RDR_PCS_Msk (0xfu << SPI_RDR_PCS_Pos) /**< \brief (SPI_RDR) Peripheral Chip Select */
/* -------- SPI_TDR : (SPI Offset: 0x0C) Transmit Data Register -------- */
#define SPI_TDR_TD_Pos 0
#define SPI_TDR_TD_Msk (0xffffu << SPI_TDR_TD_Pos) /**< \brief (SPI_TDR) Transmit Data */
#define SPI_TDR_TD(value) ((SPI_TDR_TD_Msk & ((value) << SPI_TDR_TD_Pos)))
#define SPI_TDR_PCS_Pos 16
#define SPI_TDR_PCS_Msk (0xfu << SPI_TDR_PCS_Pos) /**< \brief (SPI_TDR) Peripheral Chip Select */
#define SPI_TDR_PCS(value) ((SPI_TDR_PCS_Msk & ((value) << SPI_TDR_PCS_Pos)))
#define SPI_TDR_LASTXFER (0x1u << 24) /**< \brief (SPI_TDR) Last Transfer */
/* -------- SPI_SR : (SPI Offset: 0x10) Status Register -------- */
#define SPI_SR_RDRF (0x1u << 0) /**< \brief (SPI_SR) Receive Data Register Full */
#define SPI_SR_TDRE (0x1u << 1) /**< \brief (SPI_SR) Transmit Data Register Empty */
#define SPI_SR_MODF (0x1u << 2) /**< \brief (SPI_SR) Mode Fault Error */
#define SPI_SR_OVRES (0x1u << 3) /**< \brief (SPI_SR) Overrun Error Status */
#define SPI_SR_NSSR (0x1u << 8) /**< \brief (SPI_SR) NSS Rising */
#define SPI_SR_TXEMPTY (0x1u << 9) /**< \brief (SPI_SR) Transmission Registers Empty */
#define SPI_SR_UNDES (0x1u << 10) /**< \brief (SPI_SR) Underrun Error Status (Slave Mode Only) */
#define SPI_SR_SPIENS (0x1u << 16) /**< \brief (SPI_SR) SPI Enable Status */
/* -------- SPI_IER : (SPI Offset: 0x14) Interrupt Enable Register -------- */
#define SPI_IER_RDRF (0x1u << 0) /**< \brief (SPI_IER) Receive Data Register Full Interrupt Enable */
#define SPI_IER_TDRE (0x1u << 1) /**< \brief (SPI_IER) SPI Transmit Data Register Empty Interrupt Enable */
#define SPI_IER_MODF (0x1u << 2) /**< \brief (SPI_IER) Mode Fault Error Interrupt Enable */
#define SPI_IER_OVRES (0x1u << 3) /**< \brief (SPI_IER) Overrun Error Interrupt Enable */
#define SPI_IER_NSSR (0x1u << 8) /**< \brief (SPI_IER) NSS Rising Interrupt Enable */
#define SPI_IER_TXEMPTY (0x1u << 9) /**< \brief (SPI_IER) Transmission Registers Empty Enable */
#define SPI_IER_UNDES (0x1u << 10) /**< \brief (SPI_IER) Underrun Error Interrupt Enable */
/* -------- SPI_IDR : (SPI Offset: 0x18) Interrupt Disable Register -------- */
#define SPI_IDR_RDRF (0x1u << 0) /**< \brief (SPI_IDR) Receive Data Register Full Interrupt Disable */
#define SPI_IDR_TDRE (0x1u << 1) /**< \brief (SPI_IDR) SPI Transmit Data Register Empty Interrupt Disable */
#define SPI_IDR_MODF (0x1u << 2) /**< \brief (SPI_IDR) Mode Fault Error Interrupt Disable */
#define SPI_IDR_OVRES (0x1u << 3) /**< \brief (SPI_IDR) Overrun Error Interrupt Disable */
#define SPI_IDR_NSSR (0x1u << 8) /**< \brief (SPI_IDR) NSS Rising Interrupt Disable */
#define SPI_IDR_TXEMPTY (0x1u << 9) /**< \brief (SPI_IDR) Transmission Registers Empty Disable */
#define SPI_IDR_UNDES (0x1u << 10) /**< \brief (SPI_IDR) Underrun Error Interrupt Disable */
/* -------- SPI_IMR : (SPI Offset: 0x1C) Interrupt Mask Register -------- */
#define SPI_IMR_RDRF (0x1u << 0) /**< \brief (SPI_IMR) Receive Data Register Full Interrupt Mask */
#define SPI_IMR_TDRE (0x1u << 1) /**< \brief (SPI_IMR) SPI Transmit Data Register Empty Interrupt Mask */
#define SPI_IMR_MODF (0x1u << 2) /**< \brief (SPI_IMR) Mode Fault Error Interrupt Mask */
#define SPI_IMR_OVRES (0x1u << 3) /**< \brief (SPI_IMR) Overrun Error Interrupt Mask */
#define SPI_IMR_NSSR (0x1u << 8) /**< \brief (SPI_IMR) NSS Rising Interrupt Mask */
#define SPI_IMR_TXEMPTY (0x1u << 9) /**< \brief (SPI_IMR) Transmission Registers Empty Mask */
#define SPI_IMR_UNDES (0x1u << 10) /**< \brief (SPI_IMR) Underrun Error Interrupt Mask */
/* -------- SPI_CSR[4] : (SPI Offset: 0x30) Chip Select Register -------- */
#define SPI_CSR_CPOL (0x1u << 0) /**< \brief (SPI_CSR[4]) Clock Polarity */
#define SPI_CSR_NCPHA (0x1u << 1) /**< \brief (SPI_CSR[4]) Clock Phase */
#define SPI_CSR_CSNAAT (0x1u << 2) /**< \brief (SPI_CSR[4]) Chip Select Not Active After Transfer (Ignored if CSAAT = 1) */
#define SPI_CSR_CSAAT (0x1u << 3) /**< \brief (SPI_CSR[4]) Chip Select Not Active After Transfer (Ignored if CSAAT = 1) */
#define SPI_CSR_BITS_Pos 4
#define SPI_CSR_BITS_Msk (0xfu << SPI_CSR_BITS_Pos) /**< \brief (SPI_CSR[4]) Bits Per Transfer */
#define   SPI_CSR_BITS_8_BIT (0x0u << 4) /**< \brief (SPI_CSR[4]) 8 bits for transfer */
#define   SPI_CSR_BITS_9_BIT (0x1u << 4) /**< \brief (SPI_CSR[4]) 9 bits for transfer */
#define   SPI_CSR_BITS_10_BIT (0x2u << 4) /**< \brief (SPI_CSR[4]) 10 bits for transfer */
#define   SPI_CSR_BITS_11_BIT (0x3u << 4) /**< \brief (SPI_CSR[4]) 11 bits for transfer */
#define   SPI_CSR_BITS_12_BIT (0x4u << 4) /**< \brief (SPI_CSR[4]) 12 bits for transfer */
#define   SPI_CSR_BITS_13_BIT (0x5u << 4) /**< \brief (SPI_CSR[4]) 13 bits for transfer */
#define   SPI_CSR_BITS_14_BIT (0x6u << 4) /**< \brief (SPI_CSR[4]) 14 bits for transfer */
#define   SPI_CSR_BITS_15_BIT (0x7u << 4) /**< \brief (SPI_CSR[4]) 15 bits for transfer */
#define   SPI_CSR_BITS_16_BIT (0x8u << 4) /**< \brief (SPI_CSR[4]) 16 bits for transfer */
#define SPI_CSR_SCBR_Pos 8
#define SPI_CSR_SCBR_Msk (0xffu << SPI_CSR_SCBR_Pos) /**< \brief (SPI_CSR[4]) Serial Clock Baud Rate */
#define SPI_CSR_SCBR(value) ((SPI_CSR_SCBR_Msk & ((value) << SPI_CSR_SCBR_Pos)))
#define SPI_CSR_DLYBS_Pos 16
#define SPI_CSR_DLYBS_Msk (0xffu << SPI_CSR_DLYBS_Pos) /**< \brief (SPI_CSR[4]) Delay Before SPCK */
#define SPI_CSR_DLYBS(value) ((SPI_CSR_DLYBS_Msk & ((value) << SPI_CSR_DLYBS_Pos)))
#define SPI_CSR_DLYBCT_Pos 24
#define SPI_CSR_DLYBCT_Msk (0xffu << SPI_CSR_DLYBCT_Pos) /**< \brief (SPI_CSR[4]) Delay Between Consecutive Transfers */
#define SPI_CSR_DLYBCT(value) ((SPI_CSR_DLYBCT_Msk & ((value) << SPI_CSR_DLYBCT_Pos)))
/* -------- SPI_WPMR : (SPI Offset: 0xE4) Write Protection Control Register -------- */
#define SPI_WPMR_WPEN (0x1u << 0) /**< \brief (SPI_WPMR) Write Protection Enable */
#define SPI_WPMR_WPKEY_Pos 8
#define SPI_WPMR_WPKEY_Msk (0xffffffu << SPI_WPMR_WPKEY_Pos) /**< \brief (SPI_WPMR) Write Protection Key Password */
#define SPI_WPMR_WPKEY(value) ((SPI_WPMR_WPKEY_Msk & ((value) << SPI_WPMR_WPKEY_Pos)))
/* -------- SPI_WPSR : (SPI Offset: 0xE8) Write Protection Status Register -------- */
#define SPI_WPSR_WPVS (0x7u << 0) /**< \brief (SPI_WPSR) Write Protection Violation Status */
#define SPI_WPSR_WPVS_Pos 0
#define SPI_WPSR_WPVS_Msk (0x1u << SPI_WPSR_WPVS_Pos) /**< \brief (SPI_WPSR) Write Protection Violation Status */
#define SPI_WPSR_WPVSRC_Pos 8
#define SPI_WPSR_WPVSRC_Msk (0xffu << SPI_WPSR_WPVSRC_Pos) /**< \brief (SPI_WPSR) Wri */



#define SPI_POL_ZERO  0
#define SPI_POL_ONE   1

/*sample first edge */
#define SPI_PHASE_SF  0
/*sample second edge */
#define SPI_PHASE_SS  2

/* PA28, PA29, PB21 */
#define SPI_NSS_0 0
#define SPI_NSS_1 1
#define SPI_NSS_2 2

#define SAM3X_SPI_BITS_8  SPI_CSR_BITS_8_BIT
#define SAM3X_SPI_BITS_16  SPI_CSR_BITS_16_BIT


#define SPI_SETUP_NSS(cfg,nssp) ((cfg)->nss = (nssp))
#define SPI_SETUP_DMA(cfg,rx,tx) ((cfg)->dma_active=1,(cfg)->rxch=(rx),(cfg)->txch=(tx))


#define SPI_SETUP_AS_MASTER(cfg)  (cfg)->clk = 1
#define SPI_SETUP_MASTER_CLK(cfg, clock) (cfg)->clk = (clock)
#define SPI_SETUP_TRANSFER(cfg,pol,ph,bits) (cfg)->trflags = (pol) | (ph) | (bits)
#define SPI_SETUP_BAUDRATE(cfg,baud) (cfg)->clk = SystemCoreClock/(baud)

#define SPI_SETUP_DELAYS(cfg,bts,bcs) ((cfg)->btsdelay=(bts),(cfg)->bcsdelay=(bcs))

#define SPI_SETUP_AS_SLAVE(cfg)  (cfg)->clk = 0


#define spi_get_pcs(chip_sel_id) ((~(1u<<(chip_sel_id)))&0xF)
#define spi_reset(_spi) (_spi)->SPI_CR = SPI_CR_SWRST
#define spi_enable(_spi) (_spi)->SPI_CR = SPI_CR_SPIEN
#define spi_disable(_spi) (_spi)->SPI_CR = SPI_CR_SPIDIS
#define spi_set_lastxfer(_spi) (_spi)->SPI_CR = SPI_CR_LASTXFER
#define spi_set_master_mode(_spi) (_spi)->SPI_MR |= SPI_MR_MSTR
#define spi_set_slave_mode(_spi) (_spi)->SPI_MR &= (~SPI_MR_MSTR)
#define spi_get_mode(_spi) (((_spi)->SPI_MR & SPI_MR_MSTR) ? 1:0)
#define spi_set_variable_peripheral_select(_spi) (_spi)->SPI_MR |= SPI_MR_PS
#define spi_set_fixed_peripheral_select(_spi) (_spi)->SPI_MR &= (~SPI_MR_PS)
#define spi_get_peripheral_select_mode(_spi) (((_spi)->SPI_MR & SPI_MR_PS) ? 1:0)
#define spi_enable_peripheral_select_decode(_spi) (_spi)->SPI_MR |= SPI_MR_PCSDEC
#define spi_disable_peripheral_select_decode(_spi) (_spi)->SPI_MR &= (~SPI_MR_PCSDEC)
#define spi_get_peripheral_select_decode_setting(_spi) (((_spi)->SPI_MR & SPI_MR_PCSDEC) ? 1:0)
#define spi_enable_mode_fault_detect(_spi) (_spi)->SPI_MR &= (~SPI_MR_MODFDIS)
#define spi_disable_mode_fault_detect(_spi) (_spi)->SPI_MR |= SPI_MR_MODFDIS
#define spi_get_mode_fault_detect_setting(_spi) (((_spi)->SPI_MR & SPI_MR_MODFDIS) ?1:0)
#define spi_enable_tx_on_rx_3(_spi) (_spi)->SPI_MR |= SPI_MR_WDRBT
#define spi_disable_tx_on_rx_empty(_spi) (_spi)->SPI_MR &= (~SPI_MR_WDRBT)
#define spi_get_tx_on_rx_empty_setting(_spi) (((_spi)->SPI_MR & SPI_MR_WDRBT) ? 1:0)
#define spi_enable_loopback(_spi) (_spi)->SPI_MR |= SPI_MR_LLB
#define spi_disable_loopback(_spi) (_spi)->SPI_MR &= (~SPI_MR_LLB)
#define spi_read_status(_spi) (_spi)->SPI_SR
#define spi_is_enabled(_spi) (((_spi)->SPI_SR & SPI_SR_SPIENS) ? 1:0)
#define spi_put(_spi, data) (_spi)->SPI_TDR = SPI_TDR_TD(data)
#define spi_get(_spi) (_spi)->SPI_RDR & SPI_RDR_RD_Msk
#define spi_is_tx_empty(_spi) (((_spi)->SPI_SR & SPI_SR_TXEMPTY) ?1:0)
#define spi_is_tx_ready(_spi) (((_spi)->SPI_SR & SPI_SR_TDRE) ?1:0)
#define spi_is_rx_full(_spi) (((_spi)->SPI_SR & SPI_SR_RDRF) ?1:0)
#define spi_is_rx_ready(_spi) ((((_spi)->SPI_SR & (SPI_SR_RDRF | SPI_SR_TXEMPTY)) == (SPI_SR_RDRF | SPI_SR_TXEMPTY)) ? 1:0)
#define spi_enable_interrupt(_spi, sources) (_spi)->SPI_IER = sources
#define spi_disable_interrupt(_spi, sources) (_spi)->SPI_IDR = sources
#define spi_read_interrupt_mask(_spi) (_spi)->SPI_IMR
#define spi_get_tx_access(_spi) ((void *) & ((_spi)->SPI_TDR))
#define spi_get_rx_access(_spi) ((void *) & ((_spi)->SPI_RDR))
#define spi_set_clock_polarity(_spi, nss, polarity) ((polarity) ? ((_spi)->SPI_CSR[nss] |= SPI_CSR_CPOL):((_spi)->SPI_CSR[nss] &= (~SPI_CSR_CPOL)))
//#define spi_set_clock_phase(_spi, nss, phase) ((phase) ? ((_spi)->SPI_CSR[nss] |= SPI_CSR_NCPHA):((_spi)->SPI_CSR[nss] &= (~SPI_CSR_NCPHA)))
#define spi_set_clock_phase(_spi, nss, phase) ( (!(phase)) ? ((_spi)->SPI_CSR[nss] |= SPI_CSR_NCPHA):((_spi)->SPI_CSR[nss] &= (~SPI_CSR_NCPHA)))
#define spi_set_bits_per_transfer(_spi, nss, bits) ((_spi)->SPI_CSR[nss] &= (~SPI_CSR_BITS_Msk),(_spi)->SPI_CSR[nss] |= (bits))
#define spi_set_baudrate_div(_spi, nss, clk) ((_spi)->SPI_CSR[nss] &= (~SPI_CSR_SCBR_Msk), (_spi)->SPI_CSR[nss] |= SPI_CSR_SCBR(clk))
#define spi_set_transfer_delay(_spi, nss, bcs, bts) ((_spi)->SPI_CSR[nss] &= ~(SPI_CSR_DLYBS_Msk | SPI_CSR_DLYBCT_Msk), (_spi)->SPI_CSR[nss] |= SPI_CSR_DLYBS(bcs) | SPI_CSR_DLYBCT(bts))
#define spi_set_peripheral_chip_select_value(_spi, value) ((_spi)->SPI_MR &= (~SPI_MR_PCS_Msk), (_spi)->SPI_MR |= SPI_MR_PCS(value))
#define spi_set_delay_between_chip_select(_spi, delay) ((_spi)->SPI_MR &= (~SPI_MR_DLYBCS_Msk), (_spi)->SPI_MR |= SPI_MR_DLYBCS(delay))



#endif