/* Copyright Statement:
 *
 * (C) 2005-2016  MediaTek Inc. All rights reserved.
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. ("MediaTek") and/or its licensors.
 * Without the prior written permission of MediaTek and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 * You may only use, reproduce, modify, or distribute (as applicable) MediaTek Software
 * if you have agreed to and been bound by the applicable license agreement with
 * MediaTek ("License Agreement") and been granted explicit permission to do so within
 * the License Agreement ("Permitted User").  If you are not a Permitted User,
 * please cease any access or use of MediaTek Software immediately.
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT MEDIATEK SOFTWARE RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES
 * ARE PROVIDED TO RECEIVER ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 */

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* hal includes */
#include "hal.h"
#include "system_mt7687.h"
#include "top.h"
#include "oled.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

#ifdef __GNUC__
int __io_putchar(int ch)
#else
int fputc(int ch, FILE *f)
#endif
{
    /* Place your implementation of fputc here */
    /* E.g. write a character to the HAL_UART_0 one at a time */
    hal_uart_put_char(HAL_UART_0, ch);
    return ch;
}

/**
*@brief Set pinmux to UART and initialize UART hardware initialization for logging.
*@param None.
*@return None.
*/
static void plain_log_uart_init(void)
{
    hal_uart_config_t uart_config;
    /* Set Pinmux to UART */
    hal_pinmux_set_function(HAL_GPIO_0, HAL_GPIO_0_UART1_RTS_CM4);
    hal_pinmux_set_function(HAL_GPIO_1, HAL_GPIO_1_UART1_CTS_CM4);
    hal_pinmux_set_function(HAL_GPIO_2, HAL_GPIO_2_UART1_RX_CM4);
    hal_pinmux_set_function(HAL_GPIO_3, HAL_GPIO_3_UART1_TX_CM4);

    /* COM port settings */
    uart_config.baudrate = HAL_UART_BAUDRATE_115200;
    uart_config.word_length = HAL_UART_WORD_LENGTH_8;
    uart_config.stop_bit = HAL_UART_STOP_BIT_1;
    uart_config.parity = HAL_UART_PARITY_NONE;
    hal_uart_init(HAL_UART_0, &uart_config);
}

/**
*@brief Configure and initialize the systerm clock.
*@param None.
*@return None.
*/
static void SystemClock_Config(void)
{
    top_xtal_init();
}

/**
*@brief  Initialize the periperal driver in this function. In this example, we initialize UART drivers.
*@param None.
*@return None.
*/
static void prvSetupHardware(void)
{
    /* Peripherals initialization */
    plain_log_uart_init();
}


#define hal_i2c_master_send hal_i2c_master_send_polling

static void i2c_SendStream(uint8_t *str, uint16_t len)
{
    hal_i2c_status_t ret;

    ret = hal_i2c_master_send(HAL_I2C_MASTER_0, OLED_Address, str, len);
    
    switch(ret) {
        case HAL_I2C_STATUS_INVALID_PORT_NUMBER:
            printf("HAL_I2C_STATUS_INVALID_PORT_NUMBER\r\n");
            break;
        case HAL_I2C_STATUS_ERROR_BUSY:
            printf("HAL_I2C_STATUS_ERROR_BUSY\r\n");
            break;
        case HAL_I2C_STATUS_ERROR:
            printf("HAL_I2C_STATUS_ERROR\r\n");
            break;
        case HAL_I2C_STATUS_OK:
            break;
        default:
            break;
    }

}

static void delay_ms(uint32_t msec)
{
    hal_gpt_delay_ms(msec);
}

/**
 * i2c control OLED module with polling example
 */
void i2c_control_OLED_with_polling_example(void)
{
    /*
     * i2c init procedure see this: http://labs.mediatek.com/api/mt7697/group___i2_c___m_a_s_t_e_r.html
     */

    hal_i2c_config_t i2c_init;
    hal_i2c_frequency_t input_frequency = HAL_I2C_FREQUENCY_400K;
    hal_i2c_port_t i2c_port = HAL_I2C_MASTER_0;
    oled_t *oled;
    

    /* Step1: Call hal_gpio_init() to initialize the pin. For mode details about hal_gpio_init please refer to GPIO module in HAL. */
    hal_gpio_init(HAL_GPIO_27);
    hal_gpio_init(HAL_GPIO_28);
    printf("init IO pin 27 / 28\r\n");

    /* Step2: Call hal_pinmux_set_function() to set the GPIO pinmux or use the EPT tool to apply the pinmux settings. */
    hal_pinmux_set_function(HAL_GPIO_27, HAL_GPIO_27_I2C1_CLK);
    printf("Set IO pin 27 is I2C CLK\r\n");
    hal_pinmux_set_function(HAL_GPIO_28, HAL_GPIO_28_I2C1_DATA);
    printf("Set IO pin 28 is I2C DAT\r\n");

    i2c_init.frequency = input_frequency;
    if (HAL_I2C_STATUS_OK != hal_i2c_master_init(i2c_port, &i2c_init)) {
        printf("I2C initialize1 failed\r\n");
        return;
    }

    /* OLED Setup */
    printf("\r\n***** OLED *****\r\n");

    OLED_HardwareInit(delay_ms);

    oled = OLED_New(i2c_SendStream);

    oled->opt->init(oled);

    oled->opt->clearDisplay(oled);
    oled->opt->deactivateScroll(oled);
    oled->opt->setTextXY(oled, 0, 0);
    oled->opt->putNumber(oled, 54321);
    oled->opt->setTextXY(oled, 1, 0);
    oled->opt->putFloat(oled, 3.1415, 2);
    oled->opt->setTextXY(oled, 2, 0);
    oled->opt->putString(oled, "Hello");

    OLED_Free(oled);

    hal_i2c_master_deinit(i2c_port);

    printf("*** OLED Example PASS ***\r\n");
}

int main(void)
{
    /* Configure system clock */
    SystemClock_Config();

    /* Configure the hardware */
    prvSetupHardware();

    /* Enable I,F bits */
    __enable_irq();
    __enable_fault_irq();

    /* Add your application code here */
    printf("\r\n\r\n");
    /* The output UART used by printf is set by plain_log_uart_init() */
    printf("welcome to main()\r\n");
    printf("\r\n\r\n");

    i2c_control_OLED_with_polling_example();

    while (1);
}
