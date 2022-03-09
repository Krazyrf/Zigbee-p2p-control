#include "hal_defs.h"
#include "hal_cc8051.h"
#include "hal_int.h"
#include "hal_mcu.h"
#include "hal_board.h"
#include "hal_led.h"
#include "hal_rf.h"
#include "basic_rf.h"
#include "hal_uart.h" 
#include "sensor_drv/sensor.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
/*****点对点通讯地址设置******/
#define RF_CHANNEL                11         // 频道 11~26
#define PAN_ID                    0x8888     //网络id 
#define MY_ADDR                   0x1015     //本机模块地址
#define SEND_ADDR                 0xA3CA  //发送地址 !要更改主从地址!
/**************************************************/

#define D4 P1_0
#define SW1 P1_2
#define btn P2_0
unsigned char dat_send;
unsigned char dat_recv;
static basicRfCfg_t basicRfConfig;
// 无线RF初始化
void ConfigRf_Init(void)
{
    basicRfConfig.panId       =   PAN_ID;
    basicRfConfig.channel     =   RF_CHANNEL;
    basicRfConfig.myAddr      =   MY_ADDR;
    basicRfConfig.ackRequest  =   TRUE;
    while(basicRfInit(&basicRfConfig) == FAILED);
    basicRfReceiveOn();
}
void Init(){
  P1SEL &= ~0x01; //p1.0
  P1DIR |= 0x01;
  
  P2SEL &= ~0x01;//p2.0
  P2DIR |= 0x01;
  
  P1SEL &= ~0x04;//p1.2 继电器模块
  P1DIR &= ~0x04;
  P1INP &= ~0x04;
  P2INP &= ~0x40;
}
/********************MAIN************************/
void main(void)
{
    halBoardInit();
    ConfigRf_Init();
    Init();
    while(1)
    {
    /* user code start */
      if(SW1==0){
        halMcuWaitUs(20);
        if(SW1 == 0){
          while(SW1==0);
          dat_send =0x02;
          basicRfSendPacket(SEND_ADDR,&dat_send,1);
        }
      }
      if(basicRfPacketIsReady()){
        basicRfReceive(&dat_recv,1,NULL);
        if(dat_recv == 0x02){
          D4 = ~D4;
          btn = ~btn;
        }
      }

    /* user code end */
    }
}