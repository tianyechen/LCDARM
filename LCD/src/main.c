/**************************
* 	UART Echo Example     *
* 	Echoes out input      *
***************************/


#include "chip.h"
#include <string.h>
const uint32_t OscRateIn = 0;

#define RS 2, 8
#define RW 2, 7
#define EN  1, 8
#define DB4 2, 6 //led3
#define DB5 3, 3 //led4
#define DB6 3, 1 //led6
#define DB7 3, 0 //led7

#define DEBUG_Print(str) Chip_UART_SendBlocking(LPC_USART, str, strlen(str))

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

static void GPIO_Init(uint8_t port, uint8_t pin) {
	Chip_GPIO_WriteDirBit(LPC_GPIO, port, pin, true);
	Chip_GPIO_SetPinState(LPC_GPIO, port, pin, false);
}

static void Pin_Write(uint8_t port, uint8_t pin, uint8_t val) {
	Chip_GPIO_SetPinState(LPC_GPIO, port, pin, val);
}

void GPIO_Setup(void){
    Chip_GPIO_Init(LPC_GPIO);
    GPIO_Init(RS);
    GPIO_Init(RW);
    GPIO_Init(EN);
    GPIO_Init(DB4);
    GPIO_Init(DB5);
    GPIO_Init(DB6);
    GPIO_Init(DB7);
}

static void Delay(uint32_t dlyTicks) {
    uint32_t startTime = msTicks;
    uint32_t diff = 0;
    while(diff < dlyTicks){
        diff = msTicks - startTime;
    }
}

void Enable(void){
    Pin_Write(EN, 1);
    Delay(1);
    Pin_Write(EN, 0);
    Delay(1);
}

void Clean_Data(void){
    Pin_Write(DB4, 0);
    Pin_Write(DB5, 0);
    Pin_Write(DB6, 0);
    Pin_Write(DB7, 0);
}
void LCD_Cnvrt(char c){
    int upper = c >> 4;
    int og = c;
    ///upper bits///
    Pin_Write(DB4, (upper&(1<<0))>>0);
    Pin_Write(DB5, (upper&(1<<1))>>1);
    Pin_Write(DB6, (upper&(1<<2))>>2);
    Pin_Write(DB7, (upper&(1<<3))>>3);
    Enable();
    // Delay(1);
    ///llower bits///
    Pin_Write(DB4, (og&(1<<0))>>0);
    Pin_Write(DB5, (og&(1<<1))>>1);
    Pin_Write(DB6, (og&(1<<2))>>2);
    Pin_Write(DB7, (og&(1<<3))>>3);
    Enable();
}

void LCD_Special_Cmd(char c){
    Pin_Write(RS, 0);
    Pin_Write(EN, 0);

    int og = c;
    Pin_Write(DB4, (og&(1<<0))>>0);
    Pin_Write(DB5, (og&(1<<1))>>1);
    Pin_Write(DB6, (og&(1<<2))>>2);
    Pin_Write(DB7, (og&(1<<3))>>3);
    Enable();
}

void LCD_Cmd(unsigned int cmd){
    Pin_Write(RS, 0);
    Pin_Write(EN, 0);
    LCD_Cnvrt(cmd);

}

void LCD_WriteChar(char c){
    Pin_Write(RS, 1);
    Pin_Write(EN, 0);
    LCD_Cnvrt(c);
}
void Init_LCD(void){
    Delay(40);
    Pin_Write(RW, 0);
    //set configuration//
    LCD_Special_Cmd(0b0011); //functionset
    Delay(1);
    LCD_Cmd(0b00100000); //functionset
    Delay(1);
    LCD_Cmd(0b00100000); //functionset
    Delay(1);
    LCD_Cmd(0b00001110); //display on/off control
    Delay(1);
    LCD_Cmd(0b00000001); //display clear
    Delay(2);
    LCD_Cmd(0b00000110); //entry mode set
    Delay(2);

}



int main(void)
{

	SystemCoreClockUpdate();
	if (SysTick_Config (SystemCoreClock / 1000)) {
		//Error

		while(1);
	}
    Uart_Setup();
    GPIO_Setup();
    Init_LCD();
    // LCD_WriteChar(0b01010000); //P
    LCD_WriteChar(0b10000000); //A

    // LCD_Cmd(0b00000001); //display clear
    // Delay(2);
    // LCD_WriteChar('a');

    // LCD_Special_Cmd(0b0011); //functionset

	while(1) {
        // LCD_Cnvrt('g');
        // Enable();
        // Clean_Data();
        // Enable();
        // DEBUG_Print("Hello Andy\n\r");
        // Delay(500);
        // Pin_Write(EN, 1);
		// if ((count = Chip_UART_Read(LPC_USART, Rx_Buf, 8)) != 0) {
        //     Chip_UART_Send(LPC_USART, Rx_Buf, 8);
		// }
	}

	return 0;
}
