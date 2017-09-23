/**************************
* 	UART Echo Example     *
* 	Echoes out input      *
***************************/


#include "chip.h"
#include <string.h>
const uint32_t OscRateIn = 0;

#define LED0 2,8

volatile uint32_t msTicks;

uint8_t Rx_Buf[8];

void SysTick_Handler(void) {
	msTicks++;
}

void Uart_Setup(void){
    Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_6, (IOCON_FUNC1 | IOCON_MODE_INACT));/* RXD */
    Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_7, (IOCON_FUNC1 | IOCON_MODE_INACT));/* TXD */

    Chip_UART_Init(LPC_USART);
    Chip_UART_SetBaud(LPC_USART, 57600);
    Chip_UART_ConfigData(LPC_USART, (UART_LCR_WLEN8 | UART_LCR_SBS_1BIT | UART_LCR_PARITY_DIS));
    Chip_UART_SetupFIFOS(LPC_USART, (UART_FCR_FIFO_EN | UART_FCR_TRG_LEV2));
    Chip_UART_TXEnable(LPC_USART);
}
int main(void)
{

	SystemCoreClockUpdate();
	const uint8_t *string = "Hello World\n\r";

	if (SysTick_Config (SystemCoreClock / 1000)) {
		//Error
		while(1);
	}

    Uart_Setup();
	Chip_GPIO_Init(LPC_GPIO);
    	Chip_GPIO_WriteDirBit(LPC_GPIO, LED0, true); //sets LED0 to an output pin

	uint8_t toggle = 0;

	while(1) {
		uint8_t count;
        // Chip_UART_SendBlocking(LPC_USART, string, strlen(string));
		if ((count = Chip_UART_Read(LPC_USART, Rx_Buf, 8)) != 0) {
            Chip_UART_Send(LPC_USART, Rx_Buf, 8);
		}
	}

	return 0;
}
