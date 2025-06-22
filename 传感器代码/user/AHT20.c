/*
 * AHT20.c
 *
 * created: 2024/5/21
 *  author:
 */

 //SDA=gpio5;
 //SCL=gpio4;

#include "AHT20.h"

/**
  * @brief  ��AHT20 �豸״̬��
  * @param  void
  * @retval uint8_t �豸״̬��
  */
static uint8_t AHT20_ReadStatusCmd(void)
{
	uint8_t tmp[1];
	Soft_I2C_Read(ATH20_SLAVE_ADDRESS, AHT20_STATUS_REG, 1, tmp);
	return tmp[0];
}

/**
  * @brief  ��AHT20 �豸״̬�� �е�Bit3: У׼ʹ��λ
  * @param  void
  * @retval uint8_t У׼ʹ��λ��1 - ��У׼; 0 - δУ׼
  */
static uint8_t AHT20_ReadCalEnableCmd(void)
{
	uint8_t tmp;
	tmp = AHT20_ReadStatusCmd();
	return (tmp>>3)&0x01;
}

/**
  * @brief  ��AHT20 �豸״̬�� �е�Bit7: æ��־
  * @param  void
  * @retval uint8_t æ��־��1 - �豸æ; 0 - �豸����
  */
static uint8_t AHT20_ReadBusyCmd(void)
{
	uint8_t tmp;
	tmp = AHT20_ReadStatusCmd();
	return (tmp>>8)&0x01;                                ///////////�޸���
}

/**
  * @brief  AHT20 оƬ��ʼ������
  * @param  void
  * @retval void
  */
static void AHT20_IcInitCmd(void)
{
	uint8_t tmp[2];
	tmp[0] = 0x08;
	tmp[1] = 0x00;
	Soft_I2C_Write(ATH20_SLAVE_ADDRESS, AHT20_INIT_REG, 2, tmp);
}

/**
  * @brief  AHT20 ������������
  * @param  void
  * @retval void
  */
static void AHT20_TrigMeasureCmd(void)
{
	uint8_t tmp[2];
	tmp[0] = 0x33;
	tmp[1] = 0x00;
	Soft_I2C_Write(ATH20_SLAVE_ADDRESS, AHT20_TrigMeasure_REG, 2, tmp);
}

/**
  * @brief  AHT20 ��λ����
  * @param  void
  * @retval void
  */
static void AHT20_SoftResetCmd(void)
{
	uint8_t tmp[1];
	Soft_I2C_Write(ATH20_SLAVE_ADDRESS, AHT20_SoftReset, 0, tmp);
}

/**
  * @brief  AHT20 �豸��ʼ��
  * @param  void
  * @retval uint8_t��0 - ��ʼ��AHT20�豸�ɹ�; 1 - ��ʼ��AHT20ʧ�ܣ������豸�����ڻ���������
  */
uint8_t AHT20_Init(void)
{
	uint8_t rcnt = 2+1;//��λ���� ���Դ�����2��
	uint8_t icnt = 2+1;//��ʼ������ ���Դ�����2��

	while(--rcnt)
	{
		icnt = 2+1;

		delay_ms(40);//�ϵ��Ҫ�ȴ�40ms
		// ��ȡ��ʪ��֮ǰ�����ȼ��[У׼ʹ��λ]�Ƿ�Ϊ1
		while((!AHT20_ReadCalEnableCmd()) && (--icnt))// 2�����Ի���
		{
			delay_ms(10);
			// �����Ϊ1��Ҫ���ͳ�ʼ������
			AHT20_IcInitCmd();
			delay_ms(200);//���ʱ�䲻ȷ�����ֲ�û��
		}

		if(icnt)//[У׼ʹ��λ]Ϊ1,У׼����
		{
			break;//�˳�rcntѭ��
		}
		else//[У׼ʹ��λ]Ϊ0,У׼����
		{
			AHT20_SoftResetCmd();//��λAHT20����������
			delay_ms(200);//���ʱ�䲻ȷ�����ֲ�û��
		}
	}

	if(rcnt)
	{
		delay_ms(200);//���ʱ�䲻ȷ�����ֲ�û��
		return 0;// AHT20�豸��ʼ������
	}
	else
	{
		return 1;// AHT20�豸��ʼ��ʧ��
	}
}

/**
  * @brief  AHT20 �豸��ȡ ���ʪ�Ⱥ��¶ȣ�ԭʼ����20Bit��
  * @param  uint32_t *HT���洢20Bitԭʼ���ݵ�uint32����
  * @retval uint8_t��0-��ȡ��������; 1-��ȡ�豸ʧ�ܣ��豸һֱ����æ״̬�����ܻ�ȡ����
  */
uint8_t AHT20_ReadHT(uint32_t *HT)
{
	uint8_t cnt=3+1;//æ��־ ���Դ�����3��
	uint8_t tmp[6];
	uint32_t RetuData = 0;

	// ���ʹ�����������
	AHT20_TrigMeasureCmd();

	do{
		delay_ms(75);//�ȴ�75ms��������ɣ�æ��־Bit7Ϊ0
	}while(AHT20_ReadBusyCmd() && (--cnt));//����3��

	if(cnt)//�豸�У����Զ���ʪ������
	{
		delay_ms(5);
		// ����ʪ������
		Soft_I2C_Read(ATH20_SLAVE_ADDRESS, AHT20_STATUS_REG, 6, tmp);
		// �������ʪ��RH��ԭʼֵ��δ����Ϊ��׼��λ%��
		RetuData = 0;
		RetuData = (RetuData|tmp[1]) << 8;
		RetuData = (RetuData|tmp[2]) << 8;
		RetuData = (RetuData|tmp[3]);
		RetuData = RetuData >> 4;
		HT[0] = RetuData;

		// �����¶�T��ԭʼֵ��δ����Ϊ��׼��λ��C��
		RetuData = 0;
		RetuData = (RetuData|tmp[3]) << 8;
		RetuData = (RetuData|tmp[4]) << 8;
		RetuData = (RetuData|tmp[5]);
		RetuData = RetuData&0xfffff;
		HT[1] = RetuData;

		return 0;
	}
	else//�豸æ�����ض�ȡʧ��
	{
		return 1;
	}
}

/**
  * @brief  AHT20 ��ʪ���ź�ת������20Bitԭʼ���ݣ�ת��Ϊ��׼��λRH=%��T=��C��
  * @param  struct m_AHT20* aht���洢AHT20��������Ϣ�Ľṹ��
  * @retval uint8_t��0-������������; 1-��������ʧ�ܣ�����ֵ����Ԫ���ֲ���Χ
  */
int wendu,shidu;
uint8_t StandardUnitCon(struct m_AHT20* aht)
{
	aht->RH = aht->HT[0] *1000/1024/1024;//2^20=1048576 //ԭʽ��(double)aht->HT[0] / 1048576 *100��Ϊ�˸��㾫�ȸ�Ϊ���ڵ�
	aht->Temp = aht->HT[1] *2000 / 1024 / 1024 -500;
	//����10��
    wendu=aht->Temp;
    shidu=aht->RH;
	//�޷�,RH=0~100%; Temp=-40~85��C
	if((aht->RH >=0)&&(aht->RH <=1000) && (aht->Temp >=-400)&&(aht->Temp <=850))
	{
		aht->flag = 0;
		return 0;//������������
	}
	else
	{
		aht->flag = 1;
		return 1;//�������ݳ�����Χ������
	}
}
struct m_AHT20 AHT20;
void collect_proc()
{
	IIC_Init();							//IIC�ܽų�ʼ��
	AHT20.alive=!AHT20_Init();	        //AHT20��ʪ�ȴ�������ʼ��
	if(AHT20.alive)// ���AHT20���������ڣ����ȡ��ʪ������
	{
		//��ȡAHT20�� 20Bitԭʼ����
		AHT20.flag = AHT20_ReadHT(AHT20.HT);
		//ʵ�ʱ�׼��λת��
		StandardUnitCon(&AHT20);
	}
	//delay_ms(2000);
}





