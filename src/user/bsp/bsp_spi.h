#ifndef BSP_SPI_H
#define BSP_SPI_H

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

void bsp_spi_init(void);
void bsp_spi_dma_init(uint8_t size);
void bsp_spi_dma_tx_rx(uint8_t * tx_data,uint8_t * rx_data,uint8_t size);
void bsp_spi_tx_rx(uint8_t * tx_data,uint8_t * rx_data,uint8_t size);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif