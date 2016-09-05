#ifdef VHAL_DMA

#include "vhal.h"
#include "vhal_dma.h"



#define DMA_CHANNELS 5

#define DMA_CH_INACTIVE   0
#define DMA_CH_ACTIVE     1
#define DMA_CH_STARTED    2
#define DMA_CH_FINISHED   3


static sam3x_dmaisr_t dma_callbacks[DMA_CHANNELS];
static void *dma_callbacks_args[DMA_CHANNELS];
static uint8_t dma_status[DMA_CHANNELS];
static uint8_t dma_initialized = 0;


/* ISR reg is organized as follows in SAM3X:
    [0:5] -> BTC ( buffer transfer complete )
    [8:13] -> CBTC ( chained buffer transfer complete: don't used by this driver)
    [16:21] -> ERR ( eventual r/w error )
*/

#define BTC_CH(ch) (1u<< (ch))
#define ERR_CH(ch) (1u<< ((ch)+16))

void DMA_Handler(void) {
    uint32_t status;
    uint32_t i;
    status = dma_get_status();
    vosEnterIsr();

    for (i = 0; i < DMA_CHANNELS; i++) {
        if ((status & BTC_CH(i)) || (status & ERR_CH(i))) {
            /* BTC bit set, do callback */
            if (dma_callbacks[i] && dma_status[i]) {
                dma_callbacks[i](i, status & ERR_CH(i), dma_callbacks_args[i]);
                dma_status[i] = DMA_CH_FINISHED;
            }
        }
    }
    vosExitIsr();
}



/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/


void dma_init(void) {
    if (!dma_initialized) {
        int i;
        for (i = 0; i < DMA_CHANNELS; i++) {
            dma_callbacks[i] = NULL;
            dma_status[i] = DMA_CH_INACTIVE;
        }
        pmc_enable_clk_g32(ID_DMAC);
        dma_disable();
        dma_set_priority_mode(DMAC_GCFG_ARB_CFG_ROUND_ROBIN);
        dma_enable();
        vosInstallHandler(DMAC_IRQn, DMA_Handler);
        vhalIrqEnable(DMAC_IRQn);
        dma_initialized=1;
    }
}



int dmaChannelActivate(uint32_t channel, sam3x_dmaisr_t isr, void *arg, dmatrans_t *dmat) {
    /* channel already active */
    if (dma_status[channel])
        return 1;

    dma_status[channel] = DMA_CH_ACTIVE;
    dma_callbacks[channel] = isr;
    dma_callbacks_args[channel] = arg;

    /* configure channel */
    dma_channel_set_configuration(channel, dmat->cfg);
    dma_enable_interrupt(channel);

    return 0;
}

void dmaChannelDeactivate(uint32_t channel) {
    if (dma_status[channel]) {
        dma_status[channel] = DMA_CH_INACTIVE;
        dma_disable_interrupt(channel);
        dma_channel_disable(channel);
        /* wait for eventual pending operation to finish */
        while (dma_channel_is_enabled(channel));
    }
}



void dmaSetupTransfer(uint32_t channel, dmatrans_t *dmat) {

    /* Clear any pending interrupts */
    Dmac->DMAC_EBCISR;

    dma_channel_set_source_addr(channel, dmat->srcbuf);
    dma_channel_set_destination_addr(channel, dmat->dstbuf);
    dma_channel_set_descriptor_addr(channel, 0);
    dma_channel_set_ctrlA(channel, dmat->ctrla);
    dma_channel_set_ctrlB(channel, dmat->ctrlb);
    dma_channel_set_configuration(channel, dmat->cfg);


    dma_status[channel] = DMA_CH_STARTED;
}



#endif