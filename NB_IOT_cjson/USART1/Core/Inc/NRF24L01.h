#ifndef _NRF24L01_H_
#define _NRF24L01_H_


#include "main.h"

// NRF24L01 寄存器地址定义

// 状态寄存器
#define NRF24L01_REG_CONFIG        0x00  // 配置寄存器
#define NRF24L01_REG_EN_AA         0x01  // 自动应答功能使能寄存器
#define NRF24L01_REG_EN_RXADDR     0x02  // 接收地址使能寄存器
#define NRF24L01_REG_SETUP_AW      0x03  // 地址宽度设置寄存器
#define NRF24L01_REG_SETUP_RETR    0x04  // 自动重传功能设置寄存器
#define NRF24L01_REG_RF_CH         0x05  // RF 通道设置寄存器
#define NRF24L01_REG_RF_SETUP      0x06  // RF 设置寄存器
#define NRF24L01_REG_STATUS        0x07  // 状态寄存器
#define NRF24L01_REG_OBSERVE_TX    0x08  // 发送监测寄存器
#define NRF24L01_REG_RPD           0x09  // 接收信号强度指示寄存器
#define NRF24L01_REG_RX_ADDR_P0    0x0A  // 接收数据管道 0 地址寄存器
#define NRF24L01_REG_RX_ADDR_P1    0x0B  // 接收数据管道 1 地址寄存器
#define NRF24L01_REG_RX_ADDR_P2    0x0C  // 接收数据管道 2 地址寄存器
#define NRF24L01_REG_RX_ADDR_P3    0x0D  // 接收数据管道 3 地址寄存器
#define NRF24L01_REG_RX_ADDR_P4    0x0E  // 接收数据管道 4 地址寄存器
#define NRF24L01_REG_RX_ADDR_P5    0x0F  // 接收数据管道 5 地址寄存器
#define NRF24L01_REG_TX_ADDR       0x10  // 发送地址寄存器
#define NRF24L01_REG_RX_PW_P0      0x11  // 接收数据管道 0 有效数据宽度设置寄存器
#define NRF24L01_REG_RX_PW_P1      0x12  // 接收数据管道 1 有效数据宽度设置寄存器
#define NRF24L01_REG_RX_PW_P2      0x13  // 接收数据管道 2 有效数据宽度设置寄存器
#define NRF24L01_REG_RX_PW_P3      0x14  // 接收数据管道 3 有效数据宽度设置寄存器
#define NRF24L01_REG_RX_PW_P4      0x15  // 接收数据管道 4 有效数据宽度设置寄存器
#define NRF24L01_REG_RX_PW_P5      0x16  // 接收数据管道 5 有效数据宽度设置寄存器
#define NRF24L01_REG_FIFO_STATUS   0x17  // FIFO 状态寄存器
#define NRF24L01_REG_DYNPD         0x1C  // 动态有效数据宽度设置寄存器
#define NRF24L01_REG_FEATURE       0x1D  // 功能控制寄存器

// 命令
#define NRF24L01_CMD_R_REGISTER     0x00  // 读取寄存器命令
#define NRF24L01_CMD_W_REGISTER     0x20  // 写入寄存器命令
#define NRF24L01_CMD_R_RX_PAYLOAD   0x61  // 读取接收数据缓冲区命令
#define NRF24L01_CMD_W_TX_PAYLOAD   0xA0  // 写入发送数据缓冲区命令
#define NRF24L01_CMD_FLUSH_TX       0xE1  // 清空发送 FIFO 缓冲区命令
#define NRF24L01_CMD_FLUSH_RX       0xE2  // 清空接收 FIFO 缓冲区命令
#define NRF24L01_CMD_REUSE_TX_PL    0xE3  // 重复发送上次数据包命令
#define NRF24L01_CMD_NOP            0xFF  // 空操作命令

//状态
#define RX_OK   0x40
#define TX_OK   0x20
#define MAX_TX  0x10


void W_Reg(uint8_t Reg,uint8_t Value);
uint8_t R_Reg(uint8_t Reg);
void W_Buf(uint8_t Reg, uint8_t *Buf,uint8_t Len);
void R_Buf(uint8_t Reg,uint8_t *Buf,uint8_t Len);
void NRF24L01_Init(void);
void Reveive(uint8_t *Buf);
uint8_t Send(uint8_t *Buf) ;

GPIO_PinState R_IRQ(void);

#endif
