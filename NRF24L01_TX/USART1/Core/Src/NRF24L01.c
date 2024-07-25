#include "main.h"
#include "OLED.h"
#include "NRF24L01.h"
#include "delay.h"
#include "OLED.h"

uint8_t T_ADD[5] = {0xF0,0xF0,0xF0,0xF0,0xF0};
uint8_t R_ADD[5] = {0xF0,0xF0,0xF0,0xF0,0xF0};

void W_MOSI(uint8_t value)
{
	HAL_GPIO_WritePin(MOSI_GPIO_Port,MOSI_Pin,(GPIO_PinState)value);
}	

void W_SCK(uint8_t value)
{
	HAL_GPIO_WritePin(SCK_GPIO_Port,SCK_Pin,(GPIO_PinState)value);
}

void W_CSN(uint8_t value)
{
	HAL_GPIO_WritePin(CSN_GPIO_Port,CSN_Pin,(GPIO_PinState)value);
}

void W_CE(uint8_t value)
{
	HAL_GPIO_WritePin(CE_GPIO_Port,CE_Pin,(GPIO_PinState)value);
}

GPIO_PinState R_IRQ(void)
{
	return HAL_GPIO_ReadPin(IRQ_GPIO_Port,IRQ_Pin);
}
	
GPIO_PinState R_MISO(void)
{
	return HAL_GPIO_ReadPin(MISO_GPIO_Port,MISO_Pin);
}



uint8_t SPI_SwapByte(uint8_t Byte)
{
	uint8_t i ,ByteReceive = 0x00;
	for(i=0;i<8;i++)
	{
		W_MOSI(Byte&(0x80>>i));
		W_SCK(1);
		if(R_MISO() == 1)
		{
			ByteReceive = ByteReceive |(0x80>>i);
		}
		W_SCK(0);
	}
	return ByteReceive;
}

void W_Reg(uint8_t Reg,uint8_t Value)
{
	W_CSN(0);
	SPI_SwapByte(Reg);
	SPI_SwapByte(Value);
	W_CSN(1);
}

uint8_t R_Reg(uint8_t Reg)
{
	uint8_t Value;
	W_CSN(0);
	SPI_SwapByte(Reg);
	Value = SPI_SwapByte(NRF24L01_CMD_NOP);
	W_CSN(1);          
	return Value;
}

void W_Buf(uint8_t Reg, uint8_t *Buf,uint8_t Len)
{
	uint8_t i;
	W_CSN(0);
	SPI_SwapByte(Reg);
	for(i=0;i<Len;i++)
	{
	  SPI_SwapByte(Buf[i]);
	}
	W_CSN(1);
}

void R_Buf(uint8_t Reg,uint8_t *Buf,uint8_t Len)
{
	uint8_t i;
	W_CSN(0);
	SPI_SwapByte(Reg);
	for(i=0;i<Len;i++)
	{
	 Buf[i] = SPI_SwapByte(Buf[i]);
	  
	}
	W_CSN(1);
}

void NRF24L01_Init(void)
{
	W_CE(0);
	W_Buf(NRF24L01_CMD_W_REGISTER+NRF24L01_REG_TX_ADDR,T_ADD,5);
	W_Buf(NRF24L01_CMD_W_REGISTER+NRF24L01_REG_RX_ADDR_P0,T_ADD,5);
	W_Reg(NRF24L01_CMD_W_REGISTER+NRF24L01_REG_CONFIG,0x0F);         //配成接收模式
	W_Reg(NRF24L01_CMD_W_REGISTER+NRF24L01_REG_EN_AA,0x01);          //通道0开启自动应答
	W_Reg(NRF24L01_CMD_W_REGISTER+NRF24L01_REG_RF_CH,0x00);          //配置通信频率2.4G
	W_Reg(NRF24L01_CMD_W_REGISTER+NRF24L01_REG_RX_PW_P0,32);         //通道0的接收宽度
	W_Reg(NRF24L01_CMD_W_REGISTER+NRF24L01_REG_EN_RXADDR,0x01);      //接收通道0使能
	W_Reg(NRF24L01_CMD_W_REGISTER+NRF24L01_REG_SETUP_RETR,0x1A);     //配置重发
	
	W_Reg(NRF24L01_CMD_FLUSH_RX,NRF24L01_CMD_NOP);
	
	W_CE(1);
}


void Reveive(uint8_t *Buf)
{
	uint8_t Status;
	Status = R_Reg(NRF24L01_CMD_R_REGISTER+NRF24L01_REG_STATUS);
	if(Status & RX_OK)
	{
		R_Buf(NRF24L01_CMD_R_RX_PAYLOAD,Buf,32);
		W_Reg(NRF24L01_CMD_FLUSH_RX,NRF24L01_CMD_NOP);
		//清除状态寄存器
		W_Reg(NRF24L01_CMD_W_REGISTER+NRF24L01_REG_STATUS,Status);
		Delay_us(150);
	}
}

uint8_t Send(uint8_t *Buf)
{
	uint8_t Status;
	
	W_Buf(NRF24L01_CMD_W_TX_PAYLOAD,Buf,32);
	W_CE(0);
	W_Reg(NRF24L01_CMD_W_REGISTER+NRF24L01_REG_CONFIG,0x0E);
	W_CE(1);
	
	while(R_IRQ() == 1);
	
	Status = R_Reg(NRF24L01_CMD_R_REGISTER+NRF24L01_REG_STATUS);
	
	if(Status & MAX_TX)
	{
		W_Reg(NRF24L01_CMD_FLUSH_TX,NRF24L01_CMD_NOP);
		W_Reg(NRF24L01_CMD_W_REGISTER+NRF24L01_REG_STATUS,Status);
		return MAX_TX;

	}
	if(Status & TX_OK)
	{
		W_Reg(NRF24L01_CMD_W_REGISTER+NRF24L01_REG_STATUS,Status);
		OLED_ShowString(2,1,"TX_OK");
		return  TX_OK ;
	}		
}
