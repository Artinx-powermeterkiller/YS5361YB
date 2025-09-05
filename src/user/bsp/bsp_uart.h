#ifndef BSP_UART_H
#define BSP_UART_H_

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

#include "hpm_uart_drv.h"
#include "hpm_debug_console.h"
#include "hpm_dmav2_drv.h"
#include "hpm_dmamux_drv.h"
#include "hpm_clock_drv.h"
#include "board.h"

void bsp_uart0_init(void);
void bsp_uart1_init(void);
void bsp_uart3_init(void);
void bsp_uart5_init(void);
void bsp_uart7_init(void);

void bsp_set_uart0_buffer(uint8_t* _buffer);
void bsp_set_uart0_buffer_index(uint8_t* _buffer_index);
void bsp_set_uart0_idle_flag(uint8_t* _flag);

void bsp_set_uart1_buffer(uint8_t* _buffer);
void bsp_set_uart1_buffer_index(uint8_t* _buffer_index);
void bsp_set_uart1_idle_flag(uint8_t* _flag);

void bsp_set_uart3_buffer(uint8_t* _buffer);
void bsp_set_uart3_buffer_index(uint8_t* _buffer_index);
void bsp_set_uart3_idle_flag(uint8_t* _flag);

void bsp_set_uart5_buffer(uint8_t* _buffer);
void bsp_set_uart5_buffer_index(uint8_t* _buffer_index);
void bsp_set_uart5_idle_flag(uint8_t* _flag);

void bsp_set_uart7_buffer(uint8_t* _buffer);
void bsp_set_uart7_buffer_index(uint8_t* _buffer_index);
void bsp_set_uart7_idle_flag(uint8_t* _flag);

hpm_stat_t uart_tx_trigger_dma(DMA_Type *dma_ptr,
                               uint8_t ch_num,
                               UART_Type *uart_ptr,
                               uint32_t src,
                               uint32_t size);

hpm_stat_t uart_rx_trigger_dma(DMA_Type *dma_ptr,
                               uint8_t ch_num,
                               UART_Type *uart_ptr,
                               uint32_t dst,
                               uint32_t size);


#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif