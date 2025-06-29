#include "ls1x.h"
#include "config.h"
#include "ls1x_rtc.h"
#include "ls1x_gpio.h"
#include "ls1x_uart.h"
#include "ls1x_string.h"
#include "ls1x_printf.h"
#include "ls1x_latimer.h"
#include "ls1c102_i2c.h"
#include "ls1c102_adc.h"
#include "ls1c102_ptimer.h"
#include "ls1c102_interrupt.h"
//#include "rtc_test.h"
#include "ls1x_printf.h"
#include "ls1x_uart.h"
#include "gpio.h"
#include "UserGpio.h"



//#include "rtc_test.h"

//#include "myiic.h"
//#include "iic_bus.h"

#include "A141A.h"
#include "A158A.h"
#include "SHT30.h"
//#include "DHT11.h"


extern void WDG_DogFeed();
unsigned char receive_flag=0;               //接收标志
extern uint8_t receive_buf[];               //缓存区数据

//unsigned char flag_bj=0;
//unsigned char flag_data=0;
//extern temperature;
//extern smog_data;
//extern light_data;
//extern Humi;
extern L_switch;
extern T_switch;
extern M_switch;

int mq2_data = 0,a158a_data = 0,sht30_humi = 0,a141a_data=0;
//int falg=0;
//unsigned char sensor_data[256] = {0};
//unsigned char sensor_ciphertext[256] = {0};
//unsigned char sensor_iv[16] = {0};

void uart_jieshou_proc()                    //串口接收
{
    if(receive_flag==1)
    {
        receive_flag=0;

        if(strncmp(receive_buf,"1jbj",4)==0)//一键报警
        {
            gpio_write_pin(GPIO_PIN_63,0);
            gpio_write_pin(GPIO_PIN_15,0);
            delay_ms(3000);
            memset(receive_buf,0,5);
        }
        else if(strncmp(receive_buf,"1mbj",4)==0)//烟雾报警
        {
            //gpio_write_pin(GPIO_PIN_63,0);
            //delay_ms(3000);
            for(int i=0;i<4;i++)
            {
                gpio_write_pin(GPIO_PIN_63,0);
                delay_ms(250);
                gpio_write_pin(GPIO_PIN_63,1);
                delay_ms(200);
            }
            memset(receive_buf,0,5);
        }
        else if(strncmp(receive_buf,"1tbj",4)==0)//温度报警
        {
            //gpio_write_pin(GPIO_PIN_15,0);
            for(int i=0;i<2;i++)
            {
                gpio_write_pin(GPIO_PIN_15,0);
                delay_ms(500);
                gpio_write_pin(GPIO_PIN_15,1);
                delay_ms(100);
            }
            memset(receive_buf,0,5);
        }
        else if(strncmp(receive_buf,"1nbj",4)==0)//一键停止报警
        {
            gpio_write_pin(GPIO_PIN_63,1);
            gpio_write_pin(GPIO_PIN_15,1);
            delay_ms(10000);
            memset(receive_buf,0,5);
        }
        else if(strncmp((const char*)receive_buf,"1L",2)==0)//设置光照阈值
        {
            char num_str[4] = {0};
            for(int k=0; k<3; k++)
            {
                num_str[k] = receive_buf[2+k];
            }
            L_switch = atoi(num_str);
            memset(receive_buf,0,5);
        }
        else if(strncmp((const char*)receive_buf,"1T",2)==0)//设置温度阈值
        {
            char num_str[4] = {0};
            for(int k=0; k<3; k++)
            {
                num_str[k] = receive_buf[2+k];
            }
            L_switch = atoi(num_str);
            memset(receive_buf,0,5);
        }
        else if(strncmp((const char*)receive_buf,"1M",2)==0)//设置烟雾阈值
        {
            char num_str[4] = {0};
            for(int k=0; k<3; k++)
            {
                num_str[k] = receive_buf[2+k];
            }
            L_switch = atoi(num_str);
            memset(receive_buf,0,5);
        }
    }
}

void print_hex(unsigned char val) {
    const char *hex_chars = "0123456789ABCDEF";
    char hex[3];
    hex[0] = hex_chars[(val >> 4) & 0x0F];
    hex[1] = hex_chars[val & 0x0F];
    hex[2] = '\0';
    printf("%s", hex);
}

int main()
{
    size_t i;
    timer_init(1);
    A141A_Init();
    A158A_Init();
    MQ2_Init();

    gpio_pin_remap(5,0);                    //iic引脚复用
    gpio_pin_remap(4,0);


    // SM4国密
    unsigned char key[16] = { 1, 2, 3, 4, 5, 6, 7, 8, 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H' };
    unsigned char ciphertext[256] = { 0 };
    unsigned char iv[16] = { 0 };

    while(1)
    {
        //delay_ms(100);
        mq2_data = MQ2_Getnum();
        a158a_data = A158A_Getnum();
        sht30_humi = SHT30_Get_Humi();
        a141a_data = A141A_Getnum();
        //SHT30_Get_Temp();

        if(strncmp(receive_buf,"1send",5) == 0)
        {
            unsigned char sensor_data[256] = {0};
            sprintf((char*)sensor_data, "01 %d %d %d %d", a158a_data, sht30_humi, a141a_data,mq2_data);
            size_t sensor_len = strlen((char*)sensor_data);
            unsigned char sensor_ciphertext[256] = {0};
            unsigned char sensor_iv[16] = {0};
            SM4_Enc(sensor_data, sensor_ciphertext, &sensor_len, sensor_iv, key);            //SM4加密
            for (i = 0; i < sensor_len; i++)
            {
                print_hex(sensor_ciphertext[i]);        //输出密文
            }
            printf("\n");
            memset(receive_buf,0,5);
        }
    }
    return 0;
}

