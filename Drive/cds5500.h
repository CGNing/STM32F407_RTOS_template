/************************GUO NING*******************
���ߣ�CGN
�ļ�����ads5500.h
�������ڣ�2017.7.15
ά���ˣ�
ά�����ڣ�
������
****************************************************/
#ifndef __CDS5500_H
#define __CDS5500_H

#include "stm32f10x.h"
#include "userdef.h"
#include "delay.h"

#define BOTHWAY	//˫��ͨ��

#define CMD_PING			0X01		//��ѯ								0
#define CMD_READ			0X02		//���ֶ��������			2
#define CMD_WRITE			0X03		//���ֶ��д����			>2
#define CMD_REGWRITE	0X04		//�첽д							>2
#define CMD_ACTION		0X05		//ִ���첽д					0
#define CMD_RESET			0X06		//��λ								0
#define CMD_SWRITE		0X83		//���ֶ��ͬ��д����	0


#define CDS_SEVMODE			0			//���ģʽ
#define CDS_MOTOMODE		1			//���ģʽ

#ifdef BOTHWAY

#define UART3_TXEN (GPIOC->ODR |= GPIO_Pin_5)	//ʹ��
#define UART3_TXDS (GPIOC->ODR &= ~GPIO_Pin_5)	//ʧ��
#define UART3_RXEN (GPIOC->ODR |= GPIO_Pin_4)
#define UART3_RXDS (GPIOC->ODR &= ~GPIO_Pin_4)

#else

#define UART3_TXEN
#define UART3_TXDS
#define UART3_RXEN
#define UART3_RXDS

#endif

void CDS_conf(void);	//��ʼ��
void CDS_cmd(u8 id, u8 len, u8 cmd, u8* pData);	//��������
void CDS_SetMode(u8 id,u8 mode);	//����ģʽ
void CDS_SetAngle(u8 id,u16 angle,u16 speed);	//���ö���Ƕ�
//void CDS_SetAngleTimes(u8 id, u16 angle, s16 speed,u8 times);	//�������ֶ�������ģʽ�£��ǶȺ��ٶ�
void CDS_SetSpeed(u8 id,s16 speed);	//���õ��ת��
//void CDS_Set4MotoSpeed(u8 id[4], s16 speed[4]);	//ͬʱ����4·���ֶ�����ٶȣ����ģʽ�£�



#endif
