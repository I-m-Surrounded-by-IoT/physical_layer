#include "nbiotdriver.h"
#include <string.h>
#include "stm32f1xx_hal.h"
#include "usart.h"


static teATCmdNum ATRecCmdNum; //赋值解析命令，用于解析判断，存的命令序号
tsATCmds ATCmds[] =
{
	{"AT+CFUN=0\r\n","OK",2000,NO_REC,3},          //AT+CFUN=0关闭射频功能，AT+CFUN=1 开启射频功能
	{"AT+CGSN=1\r\n","OK",2000,NO_REC,3},          //AT+CGSN=1，查询IMEI号，
	{"AT+NRB\r\n","OK",8000,NO_REC,3},             //AT+NRB，软重启
	{"AT+NCDP=180.101.147.115,5683\r\n","OK",2000,NO_REC,3}, //AT+NCDP=180.101.147.115,5683\r\n，设置IOT平台IP地址，端口号
	{"AT+CFUN=1\r\n","OK",2000,NO_REC,3},          //AT+CFUN=1 开启射频功能
	{"AT+CIMI\r\n","OK",2000,NO_REC,3},            //AT+CIMI,查询SIM卡信息
	{"AT+CMEE=1\r\n","OK",2000,NO_REC,3},          //AT+CMEE=1,开启错误提示
	{"AT+CGDCONT=1,\"IP\",\"ctnb\"\r\n","OK",2000,NO_REC,3}, //AT+CGDCONT=1,"IP","ctnb" ,设置APN
	{"AT+NNMI=1\r\n","OK",2000,NO_REC,3},          //AT+NNMI=1，开启下行数据通知
	{"AT+CGATT=1\r\n","OK",2000,NO_REC,3},         //AT+CGATT=1，自动搜网
	{"AT+CGPADDR\r\n","+CGPADDR:1,1",2000,NO_REC,30}, //AT+CGPADDR，查询核心网分配的ip地址
	//{"AT+NMGS=","OK",3000,NO_REC,3},          		 //AT+NMGS=,Coap数据发送
};

void ATsend(teATCmdNum ATCmdNum)
{
	//清空接收缓存区
	
	//发送数据状态为未收到
	ATCmds[ATCmdNum].ATStatus = NO_REC;
	//赋值解析命令，用于解析判断
	ATRecCmdNum = ATCmdNum;
	//发送AT指令
	HAL_UART_Transmit(&huart2,(uint8_t*)ATCmds[ATCmdNum].ATSendStr,strlen(ATCmds[ATCmdNum].ATSendStr),1000);
	//打开超时定时器
	//打开发送指示灯
}