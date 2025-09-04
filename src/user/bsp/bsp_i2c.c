#include "board.h"
#include "bsp_i2c.h"
#include "hpm_gpio_drv.h"
#include "hpm_gpiom_drv.h"
#include "hpm_mchtmr_drv.h"
#include "hpm_i2c_drv.h"

uint8_t i2c0_rx_buff[16];
uint8_t i2c0_tx_buff[16];

uint32_t i2c0_send_length;
uint32_t i2c0_sent_data_count;
uint32_t i2c0_receive_length;
uint32_t i2c0_received_data_count;
volatile bool i2c_receive_complete;
volatile bool i2c_transmit_complete;

void i2c_isr(void)
{
    volatile uint32_t status, irq;
    status = i2c_get_status(HPM_I2C0);
    irq = i2c_get_irq_setting(HPM_I2C0);

    /* transmit */
    if ((status & I2C_EVENT_FIFO_EMPTY) && (irq & I2C_EVENT_FIFO_EMPTY)) {
        while (!i2c_fifo_is_full(HPM_I2C0)) {
            i2c_write_byte(HPM_I2C0, i2c0_tx_buff[i2c0_sent_data_count++]);
            if (i2c0_sent_data_count == i2c0_send_length) {
                i2c_disable_irq(HPM_I2C0, I2C_EVENT_FIFO_EMPTY);
                break;
            }
        }
    }

    /* receive */
    if (status & I2C_EVENT_FIFO_FULL) {
        while (!i2c_fifo_is_empty(HPM_I2C0)) {
            i2c0_rx_buff[i2c0_received_data_count++] = i2c_read_byte(HPM_I2C0);
        }

        if (i2c0_received_data_count == i2c0_receive_length) {
            i2c_disable_irq(HPM_I2C0, I2C_EVENT_FIFO_FULL);
        }
    }

    /* complete */
    if (status & I2C_EVENT_TRANSACTION_COMPLETE) {
        if (I2C_DIR_MASTER_READ == i2c_get_direction(HPM_I2C0)) {
            while ((!i2c_fifo_is_empty(HPM_I2C0)) && (i2c0_received_data_count < i2c0_send_length)) {
                i2c0_rx_buff[i2c0_received_data_count++] = i2c_read_byte(HPM_I2C0);
                if (i2c0_received_data_count == i2c0_send_length) {
                    i2c_disable_irq(HPM_I2C0, I2C_EVENT_FIFO_FULL);
                    break;
                }
            }
            i2c_receive_complete = true;
        } else {
            i2c_transmit_complete = true;
        }
        i2c_disable_irq(HPM_I2C0, I2C_EVENT_TRANSACTION_COMPLETE);
        i2c_clear_status(HPM_I2C0, I2C_EVENT_TRANSACTION_COMPLETE);
    }
}
SDK_DECLARE_EXT_ISR_M(IRQn_I2C0, i2c_isr)

void bsp_i2c_init(void)
{
    HPM_IOC->PAD[IOC_PAD_PA18].FUNC_CTL = IOC_PA18_FUNC_CTL_I2C0_SCL | IOC_PAD_FUNC_CTL_LOOP_BACK_MASK;
    HPM_IOC->PAD[IOC_PAD_PA18].PAD_CTL = IOC_PAD_PAD_CTL_PE_SET(1) | IOC_PAD_PAD_CTL_PS_SET(1) | IOC_PAD_PAD_CTL_OD_SET(1) | IOC_PAD_PAD_CTL_PRS_SET(1);

    HPM_IOC->PAD[IOC_PAD_PA19].FUNC_CTL = IOC_PA19_FUNC_CTL_I2C0_SDA;
    HPM_IOC->PAD[IOC_PAD_PA19].PAD_CTL = IOC_PAD_PAD_CTL_PE_SET(1) | IOC_PAD_PAD_CTL_PS_SET(1) | IOC_PAD_PAD_CTL_OD_SET(1) | IOC_PAD_PAD_CTL_PRS_SET(1);


    hpm_stat_t stat;
    i2c_config_t config;
    uint32_t freq;

    config.i2c_mode = i2c_mode_normal;
    config.is_10bit_addressing = false;
    freq = clock_get_frequency(clock_i2c0);
    stat = i2c_init_master(HPM_I2C0, freq, &config);

    if (stat != status_success) {
        return;
    }

    intc_m_enable_irq_with_priority(IRQn_I2C0, 2);
    
}

void bsp_i2c_send(uint16_t slave_address,uint8_t* send_buffer,uint8_t send_length)
{
    i2c0_sent_data_count = 0;
    i2c0_received_data_count = 0;
    i2c0_send_length = send_length;
    for(uint8_t i =0;i<send_length;i++)
    {
        i2c0_tx_buff[i] = send_buffer[i];
    }
    i2c_master_configure_transfer(HPM_I2C0,slave_address,send_length,false);
    i2c_enable_irq(HPM_I2C0, I2C_EVENT_TRANSACTION_COMPLETE |I2C_EVENT_FIFO_EMPTY);
}

void bsp_i2c_receive(uint16_t slave_address,uint8_t* receive_buffer,uint8_t reg,uint8_t receive_length)
{
    i2c0_sent_data_count = 0;
    i2c0_received_data_count = 0;
    i2c0_receive_length = receive_length;
    i2c0_send_length = 1;
    uint8_t send_buff[1] = {reg};
    bsp_i2c_send(slave_address,send_buff,1);

    while(i2c_transmit_complete == false)
    {
    }

    i2c_transmit_complete = false;

    i2c_master_configure_transfer(HPM_I2C0,slave_address,receive_length,true);
    i2c_enable_irq(HPM_I2C0, I2C_EVENT_TRANSACTION_COMPLETE |I2C_EVENT_FIFO_FULL);

    while(i2c_receive_complete == false)
    {
    }

    for(uint8_t i =0;i<receive_length;i++)
    {
        receive_buffer[i] = i2c0_rx_buff[i];
    }
}

bool bsp_get_receive_flag(void)
{
    if(i2c_receive_complete == true)
    {
        i2c_receive_complete = false;
        return  true;
    }

    return i2c_receive_complete;
}
