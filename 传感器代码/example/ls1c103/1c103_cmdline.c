#include "UserGpio.h"
#include "Config.h"
#include "ls1x.h"
#include "ls1x_pmu.h"
#include "ls1x_string.h"
#include "ls1x_wdg.h"
#include "ls1x_gpio.h"
#include "ls1x_uart.h"
#include "ls1x_common.h"
#include "ls1x_clock.h"
#include "ls1c103_tim.h"
extern void wakeup_reset(void);
extern void TIM6_timebase(void);
extern int do_d1(int argc,void *argv[]);
extern int do_d4(int argc,void *argv[]);
extern int do_m1(int argc,void *argv[]);
extern int do_m4(int argc,void *argv[]);
// add
extern int do_nothing();
extern unsigned int str2num(unsigned char);
extern int coremark_test(int argc, void *argv[]);
/*foc*/
extern int tim_foc(int argc, void *argv[]);
extern int foc_speed_test(int argc, void *argv[]);
extern int foc_speed_set(int argc, void *argv[]);
/*bldc*/
extern int tim_motor();
extern int speed_set(int argc, void *argv[]);
extern int speed_keyset(int argc, void *argv[]);
/*dcm*/
extern int tim_dcm(int argc, void *argv[]);
extern int speed_set_dcm(int argc, void *argv[]);
extern int speed_keyset_dcm(int argc, void *argv[]);
extern int tim_dcm_enc(int argc, void *argv[]);
extern int enc_speed_set(int argc, void *argv[]);
extern int enc_speed_keyset(int argc, void *argv[]);
extern int show_local();
/*steer*/
extern int tim_steer(int argc, void *argv[]);
extern int angle_set(int argc, void *argv[]);
extern int angle_keyset(int argc, void *argv[]);
/*general*/
extern int latimer(int argc,void *argv[]);
extern int watch_dog(int argc, void *argv[]);
extern int crc(int argc, void *argv[]);
extern int dma_ram(int argc, void *argv[]);
extern int key(int argc, void *argv[]);
extern int key_int(int argc, void *argv[]);
extern int adc_single_test(int argc, void *argv[]);
extern int adc_injected_test(int argc, void *argv[]);
extern int regular_test(int argc, void *argv[]);
extern int atim_int(int argc, void *argv[]);
extern int btim_int(int argc, void *argv[]);
extern int myspi_init(int argc, void *argv[]);
extern int spi_erase(int argc, void *argv[]);
extern int spi_dma_write(int argc, void *argv[]);
extern int spi_dma_read(int argc, void *argv[]);
extern int spi_flash(int argc, void *argv[]);
extern int tim_int(int argc, void *argv[]);
extern int tim_test();
extern int adc_tim_test(int argc, void *argv[]);
extern int uart1_init(int argc, void *argv[]);
extern int uart1_dma_tx(int argc, void *argv[]);
extern int uart1_dma_rx(int argc, void *argv[]);
extern int i2c_init(int argc, void *argv[]);
extern int i2c_recover(int argc, void *argv[]);
extern int i2c_wr(int argc, void *argv[]);
extern int i2c_rd(int argc, void *argv[]);
extern int i2c_dma_rd(int argc, void *argv[]);
extern int i2c_wr_master(int argc, void *argv[]);
extern int i2c_wr_slaver(int argc, void *argv[]);
extern int copy();
extern int copy_iram();
extern int delay(int argc, void *argv[]);
extern int date(int argc, void* argv[]);
extern int wakeup(int argc, void *argv[]);
extern int ry(int argc, void *argv[]);
extern int tmp(int argc, void *argv[]);
int do_help(int argc, void *argv[]);
int do_exit(int argc, void *argv[]);

unsigned int nmi_caller;

static struct cmd_struct {
	const char *cmdname;
	int (*func)(int, void *[]);
	const char *usage;
	const char *expression;
} cmd[] /*__attribute__((section(".rodata")))*/ =  {
       {"exit",          do_exit,              "exit",                                ""},
       {"d1",            do_d1,                "[d1 <addr> <num>]",                   "--dump address byte"},
       {"d4",            do_d4,                "[d4 <addr> <num>]",                   "--dump address word"},
       {"help",          do_help,              "[help <command>]",                    "--cmd list"},
       {"m1",            do_m1,                "[m1 <addr> <value>]",                 "--modify address byte"},
       {"m4",            do_m4,                "[m4 <addr> <value>]",                 "--modify address word"},
       /*foc*/
       {"foc",           tim_foc,              "[tim_foc]",                           "--tim_foc "},
       {"foc_adjust",    foc_speed_test,       "[tim_foc]",                           "--tim_foc "},
       {"foc_speed",     foc_speed_set,        "[tim_foc]",                           "--tim_foc "},
       /*bldc*/
       {"mt",            tim_motor,            "[mt]",                                "--tim motor test "},
       {"ss",            speed_set,            "[ss]",                                "--speed_set "},
       {"sks",           speed_keyset,         "[sks]",                               "--speed_set "},
       /*dcm*/
       {"dcm1",          tim_dcm,              "[tim_dcm]",                           "--tim_dcm "},
       {"ss1",           speed_set_dcm,        "[speed_set]",                         "--speed_set "},
       {"sks1",          speed_keyset_dcm,     "[speed_keyset]",                      "--speed_keyset "},
       {"dcm2",          tim_dcm_enc,          "[tim_dcm_enc]",                       "--tim_dcm_enc "},
       {"ss2",           enc_speed_set,        "[speed_set]",                         "--speed_set "},
       {"sks2",          enc_speed_keyset,     "[speed_keyset]",                      "--speed_keyset "},
       {"enc",           show_local,           "[show_local]",                        "--show_local "},
       /*steer*/
       {"steer",         tim_steer,            "[tim_steer]",                         "--tim_steer "},
       {"ags",           angle_set,            "[angle_set]",                         "--angle_set "},
       {"agks",          angle_keyset,         "[angle_keyset]",                      "--angle_keyset "},
       {"coremark",      coremark_test,        "[coremark test]",                     "coremark_test"},
       /*general*/
       {"wdg",           watch_dog,	           "[wdg test",                           "--v1"},
       {"crc",           crc,                  "[crc]",                               "--crc test"},
       {"dma",           dma_ram,              "[dma_ram]",                           "--dma_ram test"},
       {"key",           key,                  "[key]",                               "--key test"},
       {"key_int",       key_int,              "[key]",                               "--key int test"},
       {"adc_single",    adc_single_test,      "[adc]",                               "--adc single test"},
       {"adc_injected",  adc_injected_test,    "[adc]",                               "--adc injected test"},
       {"adc_regular",   regular_test,         "[adc]",                               "--adc regular test"},
       {"atim_int",      atim_int,             "[tim]",                               "--atim int test"},
       {"btim_int",      btim_int,             "[tim]",                               "--btim int test"},
       {"tim_test",      tim_test,             "[tim]",                               "--tim"},
       {"spinit",        myspi_init,           "[spi]",                               "--spi_init"},
       {"sperase",       spi_erase,            "[spi]",                               "--spi_init"},
       {"spw",           spi_dma_write,        "[spi]",                               "--spi_dma_write"},
       {"spr",           spi_dma_read,         "[spi]",                               "--spi_dma_read"},
       {"sflash",	     spi_flash,	           "[spi_flash]",			              "--v1"},
       {"adc_tim_int",   tim_int,              "[tim]",                               "--adc tim int test"},
       {"adc_tim",       adc_tim_test,         "[adc]",                               "--adc tim test"},
       {"u1init",        uart1_init ,          "[uart1]",                             "--uart1_init"},
       {"u1tx",          uart1_dma_tx ,        "[uart1]",                             "--uart1_dma_tx"},
       {"u1rx",          uart1_dma_rx ,        "[uart1]",                             "--uart1_dma_rx"},
       {"i2cinit",       i2c_init,             "[i2c]",                               "--i2c_init"},
       {"i2crec",        i2c_recover,          "[i2c]",                               "--i2c_recover"},
       {"i2cw",          i2c_wr,               "[i2c]",                               "--i2cw"},
       {"i2cm",          i2c_wr_master,        "[i2c]",                               "--i2cr"},
       {"i2cs",          i2c_wr_slaver,        "[i2c]",                               "--i2cr"},
	   {"i2cr",          i2c_rd,               "[i2c]",                               "--i2cr"},
       {"i2cr_dma",      i2c_dma_rd,           "[i2c]",                               "--i2cr"},
       {"copy",          copy,                 "[copy]",                              "--batdet <n>"},
       {"copy_iram",     copy_iram,            "[copy]",                              "--batdet <n>"},
       {"delay",         delay,                "[latimer]",                           "--latimer"},
       {"latimer",       latimer,              "[core timer test]",                   "--v1"},
       {"date",          date,                 "[rtc test]",                          "--v1"},
       {"wakeup",        wakeup,               "[wakeup test]",                       "--v1"},
       {"ry",            ry,                   "[ymodem]",                            "--v1"},
       {"t",             tmp,                  "[tmp]",                               "--v1"},
       {"NULL",          do_nothing,           "NULL",                                "NULL"}
};

int do_exit(int argc, void *argv[])
{
}

int do_help(int argc, void *argv[])
{
	unsigned int i;
	char *s;
	switch(argc)
	{
		case 1:
			{
				printf("\ncommands:\n\n");
				for(i=0;strcmp("NULL", cmd[i].cmdname)!=0; i++)
				{
					printf("\t %2d:%s\n\r",i+1, cmd[i].cmdname);
				}
			}
			break;
		case 2:
			{
				s = (char *)argv[1];
				for(i=0; strcmp("NULL", cmd[i].cmdname)!=0; i++)
				{
					if(strcmp(s, cmd[i].cmdname)==0)
					{
						printf("\n\t%s\t%s\t%s\n", cmd[i].cmdname, cmd[i].usage, cmd[i].expression);
						break;
					}
				}
				if(strcmp("NULL", cmd[i].cmdname)==0)
				{
					printf("\n\tERROR: undefine command!!!\n");
				}
			}
			break;
		default:
			printf("\n usage: help  <cmd>\n\r");
			break;
	}
	return 0;
}

int cmdline(void)
{
	
  TIM6_timebase();
  INT_EN = 0x0;
	char c;
	char cmdbuffer[80];
	char cmdpara[8][30];
	int (*op)(int argc, void *argv[]);
	char *p[8];
	char *cmdptr;
	short cp, i, j, k, num;
	short h = 0;
	int tmp = 11;
	char history_str[13][80];
	for(j=0;j<13;j++) 
	{
		for(i=0; i<80; i++)
		{
			history_str[j][i] = '\0';
		}
	}

	while(1)
	{
START:
		for(i=0; i<80; i++)
		{
			cmdbuffer[i] = '\0';
		}
		printf("\n$ ");
		cmdptr = cmdpara[0];
		cp = 0; 
		while(1)
		{
			c = getchar();
			//printf("%x",c);
			if((c>0x1f)&&(c<0x7f))
			{
				if(h > 0)
				{
					for(i=0; i<h ; i++)
					{
						cmdbuffer[cp-i] = cmdbuffer[cp-i-1];
					}
					cmdbuffer[cp-h] = c;
					cp++;
					for(i=h; i>=0; i--)
					{
						putchar(cmdbuffer[cp-i]);
					}
					for(i=0; i<cp-1; i++)
					{
						putchar(0x8);
						putchar(0x20);
						putchar(0x8);
					}
					for(i=0; cmdbuffer[i]; i++)
					{
						putchar(cmdbuffer[i]);
					}
					for(i=h; i>0; i--)
					{
						putchar(0x08);
					}
				}
				else
				{
					cmdbuffer[cp++] = c;
					putchar(c);
				}
			}
			else if(c == 0x8)
			{
				if((h != 0) && (cp-h > 0))
				{
					for(i=h; i>=0; i--)
					{
						putchar(cmdbuffer[cp-i]);
					}
					for(i=0; i<cp; i++)
					{
						putchar(0x8);
						putchar(0x20);
						putchar(0x8);
					}
					for(i=h; i>0 ; i--)
					{
						cmdbuffer[cp-i-1] = cmdbuffer[cp-i];
					}
					cmdbuffer[--cp] = '\0';
					for(i=0; cmdbuffer[i]; i++)
					{
						putchar(cmdbuffer[i]);
					}
					for(i=h; i>0; i--)
					{
						putchar(0x08);
					}
				}
				else
				{
					if((cp != 0) && (h == 0))
					{
						cmdbuffer[--cp] = '\0';
						putchar(0x8);
						putchar(0x20);
						putchar(0x8);
					}
				}
			}
			else if((c==0xa) || (c==0xd))
			{
				if(cmdbuffer[0] == '\0')
				{
					 goto START;
				}
				h = 0;
				tmp = 11;
				for(j=0;cmdbuffer[j];j++)
				{
					history_str[11][j] = cmdbuffer[j];
				}
				break;
			}

/****************************************************************************/


			else if(c==0x1b)
			{
				c = getchar();
				if(c==0x5b)
				{
					c = getchar();
					switch(c)
					{
						case 0x44:
									if(h < cp)
									{
										putchar(0x8);
										h++;
									}
									else
										h = cp;
									break;
						case 0x41:  
									if(tmp>=1)
									{
										if(tmp==1)
										{
											tmp = 1;
											break;
										}
										else
											tmp--;
										for(; h != 0; h--)
										{
											putchar(cmdbuffer[cp-h]);
										}
										if((cmdbuffer[0] != 0)||(cp == 0))
										{
											for(i=0;cmdbuffer[i];i++)
											{
												putchar(0x8);
												putchar(0x20);
												putchar(0x8);
												cmdbuffer[i] = '\0';
											}
										}
										else
										{
											for(i=0;history_str[tmp+1][i];i++)
											{
												putchar(0x8);
												putchar(0x20);
												putchar(0x8);
											}
										}
										for(j=0;history_str[tmp][j];j++)
										{
											cmdbuffer[j] = history_str[tmp][j];
											putchar(cmdbuffer[j]);
										}
									}
									cp = j;
									break;
						case 0x43:
									if(h > 0)
									{
										putchar(cmdbuffer[cp-h]);
										h--;
									}
									else
										h = 0;
									break;
						case 0x42:
									if(tmp <= 11)
									{
										if(tmp == 11)
										{
											tmp = 11;
											break;
										}
										else
											tmp++;
										for(; h != 0; h--)
										{
											putchar(cmdbuffer[cp-h]);
										}
										if((cmdbuffer[0] != 0)||(cp == 0))
										{
											for(i=0;cmdbuffer[i];i++)
											{
													putchar(0x8);
													putchar(0x20);
													putchar(0x8);
													cmdbuffer[i] = '\0';
											}
										}
										else
										{
											for(i=0;history_str[tmp-1][i];i++)
											{
												putchar(0x8);
												putchar(0x20);
												putchar(0x8);
											}
										}
										for(j=0;history_str[tmp][j];j++)
										{
											cmdbuffer[j] = history_str[tmp][j];
											putchar(cmdbuffer[j]);
										}
									}
									cp = j;
									break;
						default :
									break;
					}
				}
				c = 0;
			}

/***********************************************************************/
		}
/***********************************************************************/
		if(tmp == 11)
		{
			for(j=1;j<12;j++)
			{
				for(i=0;i<30;i++)
					history_str[j][i]=history_str[j+1][i];
			}
			for(i=0; i<30; i++)
			{
				history_str[11][i] = '\0';
			}
		}

/***********************************************************************/
		if(cp == 0)
		{
			goto START;
		}
		else
		{
			for(i=0; i<8; i++)
				for(j=0; j<30; j++)
					cmdpara[i][j] = '\0';
			num = 0;  //argc
//			printf("\ncmd:%s\n", cmdbuffer);

			for(j = 0,i=0; i<cp; i++) //xpj
			{
				if(cmdbuffer[i] == 0x20)  //space
				{
					if((cmdbuffer[i+1] != 0x20) && ((cmdbuffer[i+1] != '\0')))
					{
						j = 0;
						num++;
						cmdptr = cmdpara[num];
					}
				}
				else
				{
					*(cmdptr + j) = cmdbuffer[i];
					j++;
				}
			}

			cmdptr = cmdpara[0];
			if(strcmp(cmdptr, cmd[0].cmdname)==0)
			{
				break;
			}
			else
			{
				for(k=1; strcmp("NULL", cmd[k].cmdname)!=0; k++)
				{
					if(strcmp(cmdptr, cmd[k].cmdname)==0)
					{
						op = cmd[k].func;
						for(j=0; j<=num; j++)
						{
							p[j] = (char *)(cmdpara[j]);
						//	printf("cmdpara[%d]: %s\n", j, cmdpara[j]);
						}
						op(j, (void *)p);
						break;
					}
				}
				if(strcmp("NULL", cmd[k].cmdname)==0)
				{
					printf("\n\tERROR: undefine command!!!\n");
				}
			}
		}
	}
	return 0;
}

