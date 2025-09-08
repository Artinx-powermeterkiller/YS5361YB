#include "hpm_uart_drv.h"
#include "hpm_debug_console.h"
#include "hpm_dmav2_drv.h"
#include "hpm_dmamux_drv.h"
#include "hpm_clock_drv.h"
#include "board.h"
#include "bsp_uart.h"

hpm_stat_t uart_tx_trigger_dma(DMA_Type *dma_ptr,
                               uint8_t ch_num,
                               UART_Type *uart_ptr,
                               uint32_t src,
                               uint32_t size)
{
    dma_handshake_config_t config;

    dma_default_handshake_config(dma_ptr, &config);
    config.ch_index = ch_num;
    config.dst = (uint32_t)&uart_ptr->THR;
    config.dst_fixed = true;
    config.src = src;
    config.src_fixed = false;
    config.data_width = DMA_TRANSFER_WIDTH_BYTE;
    config.size_in_byte = size;

    return dma_setup_handshake(dma_ptr, &config, true);
}

hpm_stat_t uart_rx_trigger_dma(DMA_Type *dma_ptr,
                               uint8_t ch_num,
                               UART_Type *uart_ptr,
                               uint32_t dst,
                               uint32_t size)
{
    dma_handshake_config_t config;

    dma_default_handshake_config(dma_ptr, &config);
    config.ch_index = ch_num;
    config.dst = dst;
    config.dst_fixed = false;
    config.src = (uint32_t)&uart_ptr->RBR;
    config.src_fixed = true;
    config.data_width = DMA_TRANSFER_WIDTH_BYTE;
    config.size_in_byte = size;

    return dma_setup_handshake(dma_ptr, &config, true);
}


void bsp_uart0_init()
{
    HPM_IOC->PAD[IOC_PAD_PB01].FUNC_CTL = IOC_PB01_FUNC_CTL_UART0_RXD;
    HPM_IOC->PAD[IOC_PAD_PB00].FUNC_CTL = IOC_PB00_FUNC_CTL_UART0_TXD;

    clock_set_source_divider(clock_uart0, clk_src_pll0_clk2, 8);
    clock_add_to_group(clock_uart0, 0);

    hpm_stat_t stat;

    uart_config_t config = {0};
    uart_default_config(HPM_UART0, &config);

    config.baudrate = 9600;
    config.num_of_stop_bits = stop_bits_1;
    config.word_length = word_length_8_bits;
    config.parity = parity_none;
    config.fifo_enable = true;
    config.dma_enable = true;
    config.src_freq_in_hz = clock_get_frequency(clock_uart0);
    config.tx_fifo_level = uart_fifo_1_byte;
    config.rx_fifo_level = uart_fifo_1_byte;
    config.rxidle_config.detect_enable = true;
    config.rxidle_config.detect_irq_enable = true;
    config.rxidle_config.idle_cond = uart_rxline_idle_cond_rxline_logic_one;
    config.rxidle_config.threshold = 16U; /* 20bit */

    stat = uart_init(HPM_UART0, &config);

    if (stat != status_success) {
        printf("failed to initialize uart\n");
        while (1) {
        }
    }

    intc_m_enable_irq_with_priority(IRQn_UART0, 2);
    uart_enable_irq(HPM_UART0,uart_intr_rx_data_avail_or_timeout);

    dmamux_config(HPM_DMAMUX, 5, HPM_DMA_SRC_UART0_TX, true);
}

uint8_t* uart0_idle_flag;
uint8_t* uart0_buff_index;
uint8_t* uart0_data_buff;

void bsp_set_uart0_buffer(uint8_t* _buffer)
{
    uart0_data_buff = _buffer;
}

void bsp_set_uart0_buffer_index(uint8_t* _buffer_index)
{
    uart0_buff_index = _buffer_index;
}

void bsp_set_uart0_idle_flag(uint8_t* _flag)
{
    uart0_idle_flag = _flag;
}

void uart0_isr(void)
{
    uint8_t irq_id = uart_get_irq_id(HPM_UART0);

    if (uart_is_rxline_idle(HPM_UART0))
    {
        uart_clear_rxline_idle_flag(HPM_UART0);
        *uart0_idle_flag = 1;
    }
    else if(irq_id == uart_intr_id_rx_data_avail)
    {
        while (uart_check_status(HPM_UART0, uart_stat_data_ready)) {
            uart0_data_buff[*uart0_buff_index] = uart_read_byte(HPM_UART0);
            (*uart0_buff_index)++;
        }
    }
    else if(irq_id == uart_intr_id_rx_timeout)
    {
       uart_clear_rx_fifo(HPM_UART0);
    }
}
SDK_DECLARE_EXT_ISR_M(IRQn_UART0, uart0_isr)


void bsp_uart1_init()
{
    HPM_IOC->PAD[IOC_PAD_PB06].FUNC_CTL = IOC_PB06_FUNC_CTL_UART1_RXD;
    HPM_IOC->PAD[IOC_PAD_PB07].FUNC_CTL = IOC_PB07_FUNC_CTL_UART1_TXD;
    
    clock_set_source_divider(clock_uart1, clk_src_pll0_clk2, 8);
    clock_add_to_group(clock_uart1, 0);

    hpm_stat_t stat;

    uart_config_t config = {0};
    uart_default_config(HPM_UART1, &config);

    config.baudrate = 9600;
    config.num_of_stop_bits = stop_bits_1;
    config.word_length = word_length_8_bits;
    config.parity = parity_none;
    config.fifo_enable = true;
    config.dma_enable = true;
    config.src_freq_in_hz = clock_get_frequency(clock_uart1);
    config.tx_fifo_level = uart_fifo_1_byte;
    config.rx_fifo_level = uart_fifo_1_byte;
    config.rxidle_config.detect_enable = true;
    config.rxidle_config.detect_irq_enable = true;
    config.rxidle_config.idle_cond = uart_rxline_idle_cond_rxline_logic_one;
    config.rxidle_config.threshold = 16U; /* 20bit */

    stat = uart_init(HPM_UART1, &config);

    if (stat != status_success) {
        printf("failed to initialize uart\n");
        while (1) {
        }
    }

    intc_m_enable_irq_with_priority(IRQn_UART1, 2);
    uart_enable_irq(HPM_UART1,uart_intr_rx_data_avail_or_timeout);

    dmamux_config(HPM_DMAMUX, 6, HPM_DMA_SRC_UART1_TX, true);
}

uint8_t* uart1_idle_flag;
uint8_t* uart1_buff_index;
uint8_t* uart1_data_buff;

void bsp_set_uart1_buffer(uint8_t* _buffer)
{
    uart1_data_buff = _buffer;
}

void bsp_set_uart1_buffer_index(uint8_t* _buffer_index)
{
    uart1_buff_index = _buffer_index;
}

void bsp_set_uart1_idle_flag(uint8_t* _flag)
{
    uart1_idle_flag = _flag;
}

void uart1_isr(void)
{
    uint8_t irq_id = uart_get_irq_id(HPM_UART1);

    if (uart_is_rxline_idle(HPM_UART1))
    {
        uart_clear_rxline_idle_flag(HPM_UART1);
        *uart1_idle_flag = 1;
    }
    else if(irq_id == uart_intr_id_rx_data_avail)
    {
        while (uart_check_status(HPM_UART1, uart_stat_data_ready)) {
            uart1_data_buff[*uart1_buff_index] = uart_read_byte(HPM_UART1);
            (*uart1_buff_index)++;
        }
    }
    else if(irq_id == uart_intr_id_rx_timeout)
    {
       uart_clear_rx_fifo(HPM_UART1);
    }
}
SDK_DECLARE_EXT_ISR_M(IRQn_UART1, uart1_isr)

void bsp_uart3_init()
{
    HPM_IOC->PAD[IOC_PAD_PB14].FUNC_CTL = IOC_PB14_FUNC_CTL_UART3_RXD;
    HPM_IOC->PAD[IOC_PAD_PB15].FUNC_CTL = IOC_PB15_FUNC_CTL_UART3_TXD;
    
    clock_set_source_divider(clock_uart3, clk_src_pll0_clk2, 8);
    clock_add_to_group(clock_uart3, 0);

    hpm_stat_t stat;

    uart_config_t config = {0};
    uart_default_config(HPM_UART3, &config);

    config.baudrate = 9600;
    config.num_of_stop_bits = stop_bits_1;
    config.word_length = word_length_8_bits;
    config.parity = parity_none;
    config.fifo_enable = true;
    config.dma_enable = true;
    config.src_freq_in_hz = clock_get_frequency(clock_uart3);
    config.tx_fifo_level = uart_fifo_1_byte;
    config.rx_fifo_level = uart_fifo_1_byte;
    config.rxidle_config.detect_enable = true;
    config.rxidle_config.detect_irq_enable = true;
    config.rxidle_config.idle_cond = uart_rxline_idle_cond_rxline_logic_one;
    config.rxidle_config.threshold = 16U; /* 20bit */

    stat = uart_init(HPM_UART3, &config);

    if (stat != status_success) {
        printf("failed to initialize uart\n");
        while (1) {
        }
    }

    intc_m_enable_irq_with_priority(IRQn_UART3, 2);
    uart_enable_irq(HPM_UART3,uart_intr_rx_data_avail_or_timeout);

    dmamux_config(HPM_DMAMUX, 7, HPM_DMA_SRC_UART3_TX, true);
}

uint8_t* uart3_idle_flag;
uint8_t* uart3_buff_index;
uint8_t* uart3_data_buff;

void bsp_set_uart3_buffer(uint8_t* _buffer)
{
    uart3_data_buff = _buffer;
}

void bsp_set_uart3_buffer_index(uint8_t* _buffer_index)
{
    uart3_buff_index = _buffer_index;
}

void bsp_set_uart3_idle_flag(uint8_t* _flag)
{
    uart3_idle_flag = _flag;
}

void uart3_isr(void)
{
    uint8_t irq_id = uart_get_irq_id(HPM_UART3);

    if (uart_is_rxline_idle(HPM_UART3))
    {
        uart_clear_rxline_idle_flag(HPM_UART3);
        *uart3_idle_flag = 1;
    }
    else if(irq_id == uart_intr_id_rx_data_avail)
    {
        while (uart_check_status(HPM_UART3, uart_stat_data_ready)) {
            uart3_data_buff[*uart3_buff_index] = uart_read_byte(HPM_UART3);
            (*uart3_buff_index)++;
        }
    }
    else if(irq_id == uart_intr_id_rx_timeout)
    {
       uart_clear_rx_fifo(HPM_UART3);
    }
}
SDK_DECLARE_EXT_ISR_M(IRQn_UART3, uart3_isr)

void bsp_uart5_init()
{
    HPM_IOC->PAD[IOC_PAD_PA22].FUNC_CTL = IOC_PA22_FUNC_CTL_UART5_RXD;
    HPM_IOC->PAD[IOC_PAD_PA23].FUNC_CTL = IOC_PA23_FUNC_CTL_UART5_TXD;
    
    clock_set_source_divider(clock_uart5, clk_src_pll0_clk2, 8);
    clock_add_to_group(clock_uart5, 0);

    hpm_stat_t stat;

    uart_config_t config = {0};
    uart_default_config(HPM_UART5, &config);

    config.baudrate = 9600;
    config.num_of_stop_bits = stop_bits_1;
    config.word_length = word_length_8_bits;
    config.parity = parity_none;
    config.fifo_enable = true;
    config.dma_enable = true;
    config.src_freq_in_hz = clock_get_frequency(clock_uart5);
    config.tx_fifo_level = uart_fifo_1_byte;
    config.rx_fifo_level = uart_fifo_1_byte;
    config.rxidle_config.detect_enable = true;
    config.rxidle_config.detect_irq_enable = true;
    config.rxidle_config.idle_cond = uart_rxline_idle_cond_rxline_logic_one;
    config.rxidle_config.threshold = 16U; /* 20bit */

    stat = uart_init(HPM_UART5, &config);

    if (stat != status_success) {
        printf("failed to initialize uart\n");
        while (1) {
        }
    }

    intc_m_enable_irq_with_priority(IRQn_UART5, 2);
    uart_enable_irq(HPM_UART5,uart_intr_rx_data_avail_or_timeout);

    dmamux_config(HPM_DMAMUX, 8, HPM_DMA_SRC_UART5_TX, true);
}

uint8_t* uart5_idle_flag;
uint8_t* uart5_buff_index;
uint8_t* uart5_data_buff;

void bsp_set_uart5_buffer(uint8_t* _buffer)
{
    uart5_data_buff = _buffer;
}

void bsp_set_uart5_buffer_index(uint8_t* _buffer_index)
{
    uart5_buff_index = _buffer_index;
}

void bsp_set_uart5_idle_flag(uint8_t* _flag)
{
    uart5_idle_flag = _flag;
}

void uart5_isr(void)
{
    uint8_t irq_id = uart_get_irq_id(HPM_UART5);

    if (uart_is_rxline_idle(HPM_UART5))
    {
        uart_clear_rxline_idle_flag(HPM_UART5);
        *uart5_idle_flag = 1;
    }
    else if(irq_id == uart_intr_id_rx_data_avail)
    {
        while (uart_check_status(HPM_UART5, uart_stat_data_ready)) {
            uart5_data_buff[*uart5_buff_index] = uart_read_byte(HPM_UART5);
            (*uart5_buff_index)++;
        }
    }
    else if(irq_id == uart_intr_id_rx_timeout)
    {
       uart_clear_rx_fifo(HPM_UART5);
    }
}
SDK_DECLARE_EXT_ISR_M(IRQn_UART5, uart5_isr)


void bsp_uart7_init()
{
    HPM_IOC->PAD[IOC_PAD_PA30].FUNC_CTL = IOC_PA30_FUNC_CTL_UART7_RXD;
    HPM_IOC->PAD[IOC_PAD_PA31].FUNC_CTL = IOC_PA31_FUNC_CTL_UART7_TXD;

    clock_set_source_divider(clock_uart7, clk_src_pll0_clk2, 8);
    clock_add_to_group(clock_uart7, 0);

    hpm_stat_t stat;

    uart_config_t config = {0};
    uart_default_config(HPM_UART7, &config);

    config.baudrate = 9600;
    config.num_of_stop_bits = stop_bits_1;
    config.word_length = word_length_8_bits;
    config.parity = parity_none;
    config.fifo_enable = true;
    config.dma_enable = true; // 打开DMA
    config.src_freq_in_hz = clock_get_frequency(clock_uart7);
    config.tx_fifo_level = uart_fifo_1_byte;
    config.rx_fifo_level = uart_fifo_1_byte;
    config.rxidle_config.detect_enable = true;
    config.rxidle_config.detect_irq_enable = true;
    config.rxidle_config.idle_cond = uart_rxline_idle_cond_rxline_logic_one;
    config.rxidle_config.threshold = 16U;

    stat = uart_init(HPM_UART7, &config);

    if (stat != status_success)
    {
        printf("failed to initialize uart\n");
        while (1)
        {
        }
    }

    intc_m_enable_irq_with_priority(IRQn_HDMA, 1);
    dmamux_config(HPM_DMAMUX, 3, HPM_DMA_SRC_UART7_TX, true);

    intc_m_enable_irq_with_priority(IRQn_UART7, 2);
    uart_enable_irq(HPM_UART7,uart_intr_rx_data_avail_or_timeout);
}

uint8_t* uart7_idle_flag;
uint8_t* uart7_buff_index;
uint8_t* uart7_data_buff;

void bsp_set_uart7_buffer(uint8_t* _buffer)
{
    uart7_data_buff = _buffer;
}

void bsp_set_uart7_buffer_index(uint8_t* _buffer_index)
{
    uart7_buff_index = _buffer_index;
}

void bsp_set_uart7_idle_flag(uint8_t* _flag)
{
    uart7_idle_flag = _flag;
}

void uart7_isr(void)
{
    uint8_t irq_id = uart_get_irq_id(HPM_UART7);

    if (uart_is_rxline_idle(HPM_UART7))
    {
        uart_clear_rxline_idle_flag(HPM_UART7);
        *uart7_idle_flag = 1;
    }
    else if(irq_id == uart_intr_id_rx_data_avail)
    {
        while (uart_check_status(HPM_UART7, uart_stat_data_ready)) {
            uart7_data_buff[*uart7_buff_index] = uart_read_byte(HPM_UART7);
            (*uart7_buff_index)++;
        }
    }
    else if(irq_id == uart_intr_id_rx_timeout)
    {
       uart_clear_rx_fifo(HPM_UART7);
    }
}
SDK_DECLARE_EXT_ISR_M(IRQn_UART7, uart7_isr)

void dma_isr(void)
{
    volatile hpm_stat_t stat_rx_chn, stat_tx_chn;
    stat_rx_chn = dma_check_transfer_status(HPM_HDMA, 0);
    if (stat_rx_chn & DMA_CHANNEL_STATUS_TC) {
    }

    stat_tx_chn = dma_check_transfer_status(HPM_HDMA, 1);
    if (stat_tx_chn & DMA_CHANNEL_STATUS_TC) {
    }

    stat_rx_chn = dma_check_transfer_status(HPM_HDMA, 2);
    if (stat_tx_chn & DMA_CHANNEL_STATUS_TC) {
    }

    stat_tx_chn = dma_check_transfer_status(HPM_HDMA, 3);
    if (stat_tx_chn & DMA_CHANNEL_STATUS_TC) {
    }

    stat_rx_chn = dma_check_transfer_status(HPM_HDMA, 4);
    if (stat_tx_chn & DMA_CHANNEL_STATUS_TC) {
    }
    
    stat_tx_chn = dma_check_transfer_status(HPM_HDMA, 5);
    if (stat_tx_chn & DMA_CHANNEL_STATUS_TC) {
    }

    stat_tx_chn = dma_check_transfer_status(HPM_HDMA, 6);
    if (stat_tx_chn & DMA_CHANNEL_STATUS_TC) {
    }

    stat_tx_chn = dma_check_transfer_status(HPM_HDMA, 7);
    if (stat_tx_chn & DMA_CHANNEL_STATUS_TC) {
    }

    stat_tx_chn = dma_check_transfer_status(HPM_HDMA, 8);
    if (stat_tx_chn & DMA_CHANNEL_STATUS_TC) {
    }
}
SDK_DECLARE_EXT_ISR_M(IRQn_HDMA, dma_isr)
