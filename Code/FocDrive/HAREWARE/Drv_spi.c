#include "Drv_spi.h"

void Drv_SPI3_init(void)
{
	SPI_InitTypeDef SPI_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SPI3);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_SPI3);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SPI3);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;

    /*!< SPI SCK pin configuration */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /*!< SPI MOSI pin configuration */
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /*!< SPI MISO pin configuration */
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; 
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master; 
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High; 
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge; 
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; 
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2; 
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; 
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI3, &SPI_InitStructure);
    /* Enable SPI2 */
    SPI_Cmd(SPI3, ENABLE);
}

u8 Drv_SPI3_RW(u8 dat)
{
    while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI3, dat);
    while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET);
    return SPI_I2S_ReceiveData(SPI3);
}
void Drv_SPI3_Transmit(uint8_t *pData, uint16_t Size)
{
    for(uint16_t i=0; i<Size; i++)
    {
        Drv_SPI3_RW(pData[i]);
    }
}
void Drv_SPI3_Receive(uint8_t *pData, uint16_t Size)
{
    for(uint16_t i=0; i<Size; i++)
    {
        pData[i] = Drv_SPI3_RW(0);
    }
}

