#include "board.h"
#include "bsp_spi.h"
#include "hpm_spi.h"
#include "hpm_spi_drv.h"

static volatile bool rxdma_complete;
static volatile bool txdma_complete;

void bsp_spi_init()
{
    spi_timing_config_t timing_config = {0};
    spi_format_config_t format_config = {0};

    uint32_t spi_clock;

    HPM_IOC->PAD[IOC_PAD_PB11].FUNC_CTL = IOC_PB11_FUNC_CTL_SPI2_SCLK | IOC_PAD_FUNC_CTL_LOOP_BACK_MASK;
    HPM_IOC->PAD[IOC_PAD_PB11].PAD_CTL = IOC_PAD_PAD_CTL_SPD_SET(3) | IOC_PAD_PAD_CTL_SR_SET(1) | IOC_PAD_PAD_CTL_DS_SET(7);

    HPM_IOC->PAD[IOC_PAD_PB13].FUNC_CTL = IOC_PB13_FUNC_CTL_SPI2_MOSI;
    HPM_IOC->PAD[IOC_PAD_PB13].PAD_CTL = IOC_PAD_PAD_CTL_DS_SET(7) | IOC_PAD_PAD_CTL_SPD_SET(3) | IOC_PAD_PAD_CTL_SR_SET(1);

    HPM_IOC->PAD[IOC_PAD_PB12].FUNC_CTL = IOC_PB12_FUNC_CTL_SPI2_MISO;
    HPM_IOC->PAD[IOC_PAD_PB12].PAD_CTL = IOC_PAD_PAD_CTL_SPD_SET(3) | IOC_PAD_PAD_CTL_SR_SET(1) | IOC_PAD_PAD_CTL_OD_SET(0) | IOC_PAD_PAD_CTL_DS_SET(7);

    HPM_IOC->PAD[IOC_PAD_PB10].FUNC_CTL = IOC_PB10_FUNC_CTL_SPI2_CS_0;
    HPM_IOC->PAD[IOC_PAD_PB10].PAD_CTL = IOC_PAD_PAD_CTL_SPD_SET(3) | IOC_PAD_PAD_CTL_SR_SET(1) | IOC_PAD_PAD_CTL_DS_SET(7);
    spi_clock = board_init_spi_clock(HPM_SPI2);

    spi_master_get_default_timing_config(&timing_config);
    timing_config.master_config.clk_src_freq_in_hz = spi_clock;
    timing_config.master_config.sclk_freq_in_hz = 5000000; // 5MHz

    if (status_success != spi_master_timing_init(HPM_SPI2, &timing_config))
    {
        printf("SPI master timing init failed\n");
        while (1)
        {
        }
    }

    spi_master_get_default_format_config(&format_config);
    format_config.master_config.addr_len_in_bytes = 1U;
    format_config.common_config.data_len_in_bits = 8U;
    format_config.common_config.data_merge = false;
    format_config.common_config.mosi_bidir = false;
    format_config.common_config.lsb = false;
    format_config.common_config.mode = spi_master_mode;
    format_config.common_config.cpol = spi_sclk_high_idle;
    format_config.common_config.cpha = spi_sclk_sampling_even_clk_edges;
    spi_format_init(HPM_SPI2, &format_config);
}

void spi_rxdma_complete_callback(uint32_t channel)
{
    (void)channel;
    rxdma_complete = true;
}

void spi_txdma_complete_callback(uint32_t channel)
{
    (void)channel;
    txdma_complete = true;
}

void bsp_spi_dma_init(uint8_t size)
{
    hpm_spi_transmit_receive_setup_dma(HPM_SPI2,size);
}

static hpm_stat_t hpm_spi_tx_trigger_dma(DMA_Type *dma_ptr, uint8_t ch_num, SPI_Type *spi_ptr, uint32_t src, uint8_t data_width, uint32_t size)
{
    dma_handshake_config_t config;

    dma_default_handshake_config(dma_ptr, &config);
    config.ch_index = ch_num;
    config.dst = (uint32_t)&spi_ptr->DATA;
    config.dst_fixed = true;
    config.src = src;
    config.src_fixed = false;
    config.data_width = data_width;
    config.size_in_byte = size;

    return dma_setup_handshake(dma_ptr, &config, true);
}

static hpm_stat_t hpm_spi_rx_trigger_dma(DMA_Type *dma_ptr, uint8_t ch_num, SPI_Type *spi_ptr, uint32_t dst, uint8_t data_width, uint32_t size)
{
    dma_handshake_config_t config;

    dma_default_handshake_config(dma_ptr, &config);
    config.ch_index = ch_num;
    config.dst = dst;
    config.dst_fixed = false;
    config.src = (uint32_t)&spi_ptr->DATA;
    config.src_fixed = true;
    config.data_width = data_width;
    config.size_in_byte = size;

    return dma_setup_handshake(dma_ptr, &config, true);
}

void bsp_spi_dma_tx_rx(uint8_t * tx_data,uint8_t * rx_data,uint8_t size)
{
    bsp_spi_dma_init(size);
    dmamux_config(HPM_DMAMUX, DMA_SOC_CHN_TO_DMAMUX_CHN(HPM_HDMA,1), HPM_DMA_SRC_SPI2_TX, true);
    hpm_spi_tx_trigger_dma(HPM_HDMA,1,HPM_SPI2,core_local_mem_to_sys_address(HPM_CORE0,(uint32_t)tx_data),DMA_TRANSFER_WIDTH_BYTE,size);


    dmamux_config(HPM_DMAMUX, DMA_SOC_CHN_TO_DMAMUX_CHN(HPM_HDMA,2), HPM_DMA_SRC_SPI2_RX, true);
    hpm_spi_rx_trigger_dma(HPM_HDMA,2,HPM_SPI2,core_local_mem_to_sys_address(HPM_CORE0,(uint32_t)rx_data),DMA_TRANSFER_WIDTH_BYTE,size);
}

void bsp_spi_tx_rx(uint8_t * tx_data,uint8_t * rx_data,uint8_t size)
{
    spi_control_config_t control_config = {0};

    spi_master_get_default_control_config(&control_config);
    control_config.master_config.cmd_enable = false;
    control_config.master_config.addr_enable = false;
    control_config.common_config.trans_mode = spi_trans_write_read_together;

    spi_transfer(HPM_SPI2,&control_config,NULL,NULL,tx_data,size,rx_data,size);
}


