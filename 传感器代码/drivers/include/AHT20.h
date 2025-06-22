/*
 * AHT20.h
 *
 * created: 2024/5/21
 *  author: 
 */
#ifndef __AHT20_H
#define __AHT20_H
#include "myiic.h"

#define ATH20_SLAVE_ADDRESS		0x38		/* I2C�ӻ���ַ */

//****************************************
// ���� AHT20 �ڲ���ַ
//****************************************
#define	AHT20_STATUS_REG		0x00	//״̬�� �Ĵ�����ַ
#define	AHT20_INIT_REG			0xBE	//��ʼ�� �Ĵ�����ַ
#define	AHT20_SoftReset			0xBA	//��λ ��ָ��
#define	AHT20_TrigMeasure_REG	0xAC	//�������� �Ĵ�����ַ

// �洢AHT20��������Ϣ�Ľṹ��
struct m_AHT20
{
	uint8_t alive;	// 0-����������; 1-��������
	uint8_t flag;	// ��ȡ/��������־λ��0-��ȡ/������������; 1-��ȡ/�����豸ʧ��
	uint32_t HT[2];	// ʪ�ȡ��¶� ԭʼ��������ֵ��20Bit

	uint32_t RH;		// ʪ�ȣ�ת����λ���ʵ��ֵ����׼��λ%
    uint32_t Temp;		// �¶ȣ�ת����λ���ʵ��ֵ����׼��λ��C
};


uint8_t AHT20_Init(void);
uint8_t AHT20_ReadHT(uint32_t *HT);
uint8_t StandardUnitCon(struct m_AHT20* aht);
void collect_proc();
#include "config.h"
extern int wendu,shidu;//��ʪ��
#include "ls1c102_ptimer.h"
#include "ls1x_latimer.h"

#endif
