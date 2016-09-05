#if VHAL_DMA

#include "vhal.h"
#include "vhal_dma.h"


typedef struct _dmainfo {
	DMA_Stream_TypeDef *stream;
	uint8_t	irq;
	uint8_t dma;
	uint8_t shift;
	uint8_t stream_n;
} DMAInfo;


volatile  uint32_t *_dmafcr[4] = {
	&DMA1->LIFCR, &DMA1->HIFCR, &DMA2->LIFCR, &DMA2->HIFCR
};

volatile uint32_t *_dmasr[4] = {
	&DMA1->LISR, &DMA1->HISR, &DMA2->LISR, &DMA2->HISR
};



const DMAInfo const _dmainfo[16] = {
	{DMA1_Stream0, DMA1_Stream0_IRQn, 0, 0, 0},
	{DMA1_Stream1, DMA1_Stream1_IRQn, 0, 6, 1},
	{DMA1_Stream2, DMA1_Stream2_IRQn, 0, 16, 2},
	{DMA1_Stream3, DMA1_Stream3_IRQn, 0, 22, 3},
	{DMA1_Stream4, DMA1_Stream4_IRQn, 1, 0, 4},
	{DMA1_Stream5, DMA1_Stream5_IRQn, 1, 6, 5},
	{DMA1_Stream6, DMA1_Stream6_IRQn, 1, 16, 6},
	{DMA1_Stream7, DMA1_Stream7_IRQn, 1, 22, 7},
	{DMA2_Stream0, DMA2_Stream0_IRQn, 2, 0, 0},
	{DMA2_Stream1, DMA2_Stream1_IRQn, 2, 6, 1},
	{DMA2_Stream2, DMA2_Stream2_IRQn, 2, 16, 2},
	{DMA2_Stream3, DMA2_Stream3_IRQn, 2, 22, 3},
	{DMA2_Stream4, DMA2_Stream4_IRQn, 3, 0, 4},
	{DMA2_Stream5, DMA2_Stream5_IRQn, 3, 6, 5},
	{DMA2_Stream6, DMA2_Stream6_IRQn, 3, 16, 6},
	{DMA2_Stream7, DMA2_Stream7_IRQn, 3, 22, 7},
};



typedef struct _dma_callbacks {
	dma_callback callback;
	void *args;
} DMACallbacks;

DMACallbacks _dmacbks[16];


uint16_t streams_status = 0;
uint8_t dma_initialized = 0;


void dma_irq_0() {
	vosEnterIsr();
	dma_handler(0);
	vosExitIsr();
}

void dma_irq_1() {
	vosEnterIsr();
	dma_handler(1);
	vosExitIsr();
}

void dma_irq_2() {
	vosEnterIsr();
	dma_handler(2);
	vosExitIsr();
}

void dma_irq_3() {
	vosEnterIsr();
	dma_handler(3);
	vosExitIsr();
}

void dma_irq_4() {
	vosEnterIsr();
	dma_handler(4);
	vosExitIsr();
}

void dma_irq_5() {
	vosEnterIsr();
	dma_handler(5);
	vosExitIsr();
}

void dma_irq_6() {
	vosEnterIsr();
	dma_handler(6);
	vosExitIsr();
}

void dma_irq_7() {
	vosEnterIsr();
	dma_handler(7);
	vosExitIsr();
}

void dma_irq_8() {
	vosEnterIsr();
	dma_handler(8);
	vosExitIsr();
}

void dma_irq_9() {
	vosEnterIsr();
	dma_handler(9);
	vosExitIsr();
}

void dma_irq_10() {
	vosEnterIsr();
	dma_handler(10);
	vosExitIsr();
}

void dma_irq_11() {
	vosEnterIsr();
	dma_handler(11);
	vosExitIsr();
}

void dma_irq_12() {
	vosEnterIsr();
	dma_handler(12);
	vosExitIsr();
}

void dma_irq_13() {
	vosEnterIsr();
	dma_handler(13);
	vosExitIsr();
}

void dma_irq_14() {
	vosEnterIsr();
	dma_handler(14);
	vosExitIsr();
}

void dma_irq_15() {
	vosEnterIsr();
	dma_handler(15);
	vosExitIsr();
}



#define DMA_ISR_MASK 0x3D


void dma_init() {
	if (!dma_initialized) {
		int i;
		for (i = 0; i < 16; i++) {
			_dmainfo[i].stream->CR = 0;
			_dmacbks[i].callback = NULL;
		}
		DMA1->LIFCR = 0xFFFFFFFF;
		DMA1->HIFCR = 0xFFFFFFFF;
		DMA2->LIFCR = 0xFFFFFFFF;
		DMA2->HIFCR = 0xFFFFFFFF;
		dma_initialized = 1;
	}
}

void dma_handler(uint32_t n) {
	uint32_t sr = ((*_dmasr[_dmainfo[n].dma]) >> _dmainfo[n].shift)&DMA_ISR_MASK;
	(*_dmafcr[_dmainfo[n].dma]) = (DMA_ISR_MASK << _dmainfo[n].shift);
	if (_dmacbks[n].callback) {
		_dmacbks[n].callback(n, sr,_dmacbks[n].args);
	}
}

const vos_irq_handler dmairqs[] = {
	dma_irq_0,
	dma_irq_1,
	dma_irq_2,
	dma_irq_3,
	dma_irq_4,
	dma_irq_5,
	dma_irq_6,
	dma_irq_7,
	dma_irq_8,
	dma_irq_9,
	dma_irq_10,
	dma_irq_11,
	dma_irq_12,
	dma_irq_13,
	dma_irq_14,
	dma_irq_15,
};




void dma_stream_disable(uint32_t n) {
	_dmainfo[n].stream->CR &= ~(DMA_SxCR_TCIE | DMA_SxCR_HTIE | DMA_SxCR_DMEIE | DMA_SxCR_TEIE | DMA_SxCR_EN);
	while (_dmainfo[n].stream->CR & DMA_SxCR_EN);
	*_dmafcr[_dmainfo[n].dma] = DMA_ISR_MASK << _dmainfo[n].shift;
}


int dma_get_stream(uint32_t stream_n, dma_callback callback, void* args) {
	//check if stream is taken
	if (streams_status & (1 << stream_n))
		return -1;
	streams_status |= (1 << stream_n);

	//enable clocks: ag 114 RM_401_BCDE
	if (streams_status & 0xff)
		RCC->AHB1ENR |= (1 << 21);
	if (streams_status >> 8)
		RCC->AHB1ENR |= (1 << 22);

	/* Putting the stream in a safe state.*/
	dma_stream_disable(stream_n);
	_dmainfo[stream_n].stream->CR = 0;
	_dmainfo[stream_n].stream->FCR = 0;

	if (callback) {
		vosInstallHandler(_dmainfo[stream_n].irq, dmairqs[stream_n]);
		//vhalIrqClearPending(_dmainfo[stream_n].irq);
		vhalIrqEnable(_dmainfo[stream_n].irq);
		_dmacbks[stream_n].callback = callback;
		_dmacbks[stream_n].args = args;
	}
	return 0;
}

int dma_free_stream(uint32_t stream_n) {

	vhalIrqDisable(_dmainfo[stream_n].irq);
	streams_status &= ~(1 << stream_n);

	//disabe clocks
	if (!(streams_status & 0xff))
		RCC->AHB1ENR &= ~(1 << 21);
	if (!(streams_status >> 8))
		RCC->AHB1ENR &= ~(1 << 22);
}


//TODO: memory to memory not yet supported
int dma_conf_stream(uint32_t n, uint32_t from, uint32_t to, uint32_t transfers, uint32_t bitsize, uint32_t channel, uint32_t mpinc) {
	uint32_t cr = (channel << 25) | DMA_SxCR_DMEIE | DMA_SxCR_TEIE | DMA_SxCR_TCIE | (bitsize << 13) | (bitsize << 11);

	if (from >= PERIPH_BASE) {
		//from peripheral
		cr |= (0 << 6);
		_dmainfo[n].stream->PAR = from;
	} else {
		//from memory
		_dmainfo[n].stream->M0AR = from;
	}
	if (to >= PERIPH_BASE) {
		//to peripheral
		cr |= (1 << 6);
		_dmainfo[n].stream->PAR = to;
	} else {
		//to memory
		_dmainfo[n].stream->M0AR = to;
	}
	_dmainfo[n].stream->NDTR = transfers;
	cr |= mpinc;
	_dmainfo[n].stream->CR = cr;
	//dma is always flow controller
	return 0;
}


void dma_stream_enable(uint32_t n) {
	_dmainfo[n].stream->CR |= DMA_SxCR_EN;
}

#endif