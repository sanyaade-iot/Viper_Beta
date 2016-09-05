#if VHAL_DMA

#include "vhal.h"
#include "vhal_dma.h"


//#define printf(...) vbl_printf_stdout(__VA_ARGS__)
#define printf(...)

typedef struct _dmainfo {
	DMA_Channel_TypeDef *stream;
	uint8_t	irq;
	uint8_t dma;
	uint8_t shift;
	uint8_t stream_n;
} DMAInfo;


volatile  uint32_t *_dmafcr[] = {
	&DMA1->IFCR, &DMA2->IFCR
};

volatile uint32_t *_dmasr[] = {
	&DMA1->ISR, &DMA2->ISR
};

#if defined(STM32F10X_MD_VL) || defined(STM32F10X_MD) || defined(STM32F10X_LD_VL) || defined(STM32F10X_LD)
#define DMA_CHS 7
#else
#define DMA_CHS 11
#endif

//TODO: add DMA2_Ch5!!
const DMAInfo const _dmainfo[DMA_CHS] = {
	{DMA1_Channel1, DMA1_Channel1_IRQn, 0, 0, 0},
	{DMA1_Channel2, DMA1_Channel2_IRQn, 0, 4, 1},
	{DMA1_Channel3, DMA1_Channel3_IRQn, 0, 8, 2},
	{DMA1_Channel4, DMA1_Channel4_IRQn, 0, 12, 3},
	{DMA1_Channel5, DMA1_Channel5_IRQn, 0, 16, 4},
	{DMA1_Channel6, DMA1_Channel6_IRQn, 0, 20, 5},
	{DMA1_Channel7, DMA1_Channel7_IRQn, 0, 24, 6},
#if !(defined(STM32F10X_MD_VL) || defined(STM32F10X_MD) || defined(STM32F10X_LD_VL) || defined(STM32F10X_LD))
	{DMA2_Channel1, DMA2_Channel1_IRQn, 1, 0, 0},
	{DMA2_Channel2, DMA2_Channel2_IRQn, 1, 4, 1},
	{DMA2_Channel3, DMA2_Channel3_IRQn, 1, 8, 2},
	{DMA2_Channel4, DMA2_Channel4_IRQn, 1, 12, 3},
#endif
};



typedef struct _dma_callbacks {
	dma_callback callback;
	void *args;
} DMACallbacks;

DMACallbacks _dmacbks[DMA_CHS];


uint16_t streams_status = 0;
uint8_t dma_initialized = 0;




void dma_irq_1() {
	dma_handler(0);
}

void dma_irq_2() {
	dma_handler(1);
}

void dma_irq_3() {
	dma_handler(2);
}

void dma_irq_4() {
	dma_handler(3);
}

void dma_irq_5() {
	dma_handler(4);
}

void dma_irq_6() {
	dma_handler(5);
}

void dma_irq_7() {
	dma_handler(6);
}


#if !(defined(STM32F10X_MD_VL) || defined(STM32F10X_MD) || defined(STM32F10X_LD_VL) || defined(STM32F10X_LD))
void dma_irq_8() {
	dma_handler(7);
}

void dma_irq_9() {
	dma_handler(8);
}
void dma_irq_10() {
	dma_handler(9);
}
void dma_irq_11() {
	dma_handler(10);
}

#endif

#define DMA_ISR_MASK 0xf


void dma_init() {
	if (!dma_initialized) {
		int i;
		for (i = 0; i < DMA_CHS; i++) {
			_dmainfo[i].stream->CCR = 0;
			_dmacbks[i].callback = NULL;
		}
		DMA1->IFCR = 0xFFFFFFFF;
		#if !(defined(STM32F10X_MD_VL) || defined(STM32F10X_MD) || defined(STM32F10X_LD_VL) || defined(STM32F10X_LD))
		DMA2->IFCR = 0xFFFFFFFF;
		#endif
		dma_initialized = 1;
	}
}

void dma_handler(uint32_t n) {
	uint32_t sr = ((*_dmasr[_dmainfo[n].dma]) >> _dmainfo[n].shift)&DMA_ISR_MASK;
	vosEnterIsr();
	(*_dmafcr[_dmainfo[n].dma]) = (DMA_ISR_MASK << _dmainfo[n].shift);
	if (_dmacbks[n].callback) {
		_dmacbks[n].callback(n, sr, _dmacbks[n].args);
	}
	vosExitIsr();
}

const vos_irq_handler dmairqs[] = {
	dma_irq_1,
	dma_irq_2,
	dma_irq_3,
	dma_irq_4,
	dma_irq_5,
	dma_irq_6,
	dma_irq_7,
#if !(defined(STM32F10X_MD_VL) || defined(STM32F10X_MD) || defined(STM32F10X_LD_VL) || defined(STM32F10X_LD))
	dma_irq_8,
	dma_irq_9,
	dma_irq_10,
	dma_irq_11,
#endif
};




void dma_stream_disable(uint32_t n) {
	_dmainfo[n].stream->CCR &= ~(DMA_CCR1_TCIE | DMA_CCR1_HTIE |  DMA_CCR1_TEIE | DMA_CCR1_EN);
	*_dmafcr[_dmainfo[n].dma] = DMA_ISR_MASK << _dmainfo[n].shift;
}


int dma_get_stream(uint32_t stream_n, dma_callback callback, void *args) {
	//check if stream is taken
	if (streams_status & (1 << stream_n))
		return -1;
	streams_status |= (1 << stream_n);

	if (streams_status & 0x7f)
		RCC->AHBENR |= RCC_AHBENR_DMA1EN;
	if (streams_status >> 7)
		RCC->AHBENR |= RCC_AHBENR_DMA2EN;

	dma_stream_disable(stream_n);
	_dmainfo[stream_n].stream->CCR = 0;
	_dmacbks[stream_n].callback = callback;
	_dmacbks[stream_n].args = args;

	if (callback) {
		vosInstallHandler(_dmainfo[stream_n].irq, dmairqs[stream_n]);
		vhalIrqEnable(_dmainfo[stream_n].irq);
	}
	return 0;
}

int dma_free_stream(uint32_t stream_n) {

	vhalIrqDisable(_dmainfo[stream_n].irq);
	streams_status &= ~(1 << stream_n);

	//disabe clocks
	if (!(streams_status & 0x7f))
		RCC->AHBENR &= ~RCC_AHBENR_DMA1EN;
	if (!(streams_status >> 7))
		RCC->AHBENR &= ~RCC_AHBENR_DMA2EN;
}


//TODO: memory to memory not yet supported
int dma_conf_stream(uint32_t n, uint32_t from, uint32_t to, uint32_t transfers, uint32_t bitsize, uint32_t mpinc) {
	uint32_t cr = DMA_CCR1_TEIE | DMA_CCR1_TCIE | (bitsize << 8) | (bitsize << 10);

	if (from >= PERIPH_BASE) {
		//from peripheral: enable read from prph
		cr |= (0 << 4);
		_dmainfo[n].stream->CPAR = from;
	} else {
		//from memory
		_dmainfo[n].stream->CMAR = from;
	}
	if (to >= PERIPH_BASE) {
		//to peripheral: enable read from memory
		cr |= (1 << 4);
		_dmainfo[n].stream->CPAR = to;
	} else {
		//to memory
		_dmainfo[n].stream->CMAR = to;
	}
	_dmainfo[n].stream->CNDTR = transfers;
	cr |= mpinc;
	printf("dmaccr %x\n", cr);
	printf("dmandtr %x\n", _dmainfo[n].stream->CNDTR);
	printf("dmapar %x\n", _dmainfo[n].stream->CPAR);
	printf("dmamem %x\n", _dmainfo[n].stream->CMAR);
	_dmainfo[n].stream->CCR = cr;
	return 0;
}


void dma_stream_enable(uint32_t n) {
	_dmainfo[n].stream->CCR |= DMA_CCR1_EN;
}

#endif