#include "UserGpio.h"
#include "Config.h"
#include "ls1x_pmu.h"
#include "ls1x_clock.h"
#include "ls1x_common.h"
#include "ls1x_uart.h"
#include "ls1x_wdg.h"
#include "ls1x_string.h"


extern void wakeup_reset(void);
extern int do_d1(int argc,void *argv[]);
extern int do_d4(int argc,void *argv[]);
extern int do_m1(int argc,void *argv[]);
extern int do_m4(int argc,void *argv[]);
extern int v_play(int argc,void *argv[]);
extern int hpet(int argc,void *argv[]);
extern int latimer(int argc,void *argv[]);
extern int delay(int argc, void *argv[]);
extern int wakeup(int argc, void *argv[]);
extern int date(int argc, void* argv[]);
extern int touch_test(int argc, void *argv[]);
extern int watch_dog(int argc, void *argv[]);
extern int i2cp (int argc, void *argv[]);
extern int i2cw (int argc, void *argv[]);
extern int i2cr (int argc, void *argv[]);
extern int adc(int argc, void* argv[]);
extern int batdet(int argc, void* argv[]);
extern int tmp(int argc, void* argv[]);
extern int uart_init (int argc, void *argv[]);
extern int spi_init(int argc, void* argv[]);
extern int spi_erase(int argc, void* argv[]);
extern int spi_write(int argc, void* argv[]);
extern int spi_read(int argc, void* argv[]);
extern int spi_flash(int argc, void* argv[]);
extern int gpio_int(int argc, void* argv[]);
extern int m4_flash(int argc, void* argv[]);
extern int do_nothing();
extern int copy();
extern int copy_iram(int argc, void *argv[]);
extern unsigned int str2num(unsigned char);
// add
int do_help(int argc, void *argv[]);
int do_exit(int argc, void *argv[]);

unsigned int nmi_caller;

static struct cmd_struct {
	const char *cmdname;
	int (*func)(int, void *[]);
	const char *usage;
	const char *expression;
} cmd[] /*__attribute__((section(".rodata")))*/ =  {
  {"exit",      do_exit,     "[exit]",                  ""},
  {"d1",        do_d1,       "[d1 <addr> <num>]",     "--dump address byte"},
  {"d4",        do_d4,       "[d4 <addr> <num>]",     "--dump address word"},
  {"help",      do_help,     "[help <command>]",      "--cmd list"},
  {"m1",        do_m1,       "[m1 <addr> <value>]",   "--modify address byte"},
  {"m4",        do_m4,       "[m4 <addr> <value>]",   "--modify address word"},
  {"v",         v_play,      "[v]",                   "--v1"},
  {"touch",     touch_test,  "[touch]",               "--v1"},
  {"hpet",      hpet,        "[timer test]",          "--v1"},
  {"latimer",   latimer,     "[core timer test]",     "--v1"},
  {"delay",     delay,       "[delay test]",          "--v1"},
  {"wakeup",    wakeup,      "[wakeup test]",         "--v1"},
  {"date",      date,        "[rtc test]",            "--v1"},
  {"wdg",       watch_dog,   "[wdg test]",            "--v1"},
  {"i2cp",      i2cp,        "[i2cp <prescale>]",     "--i2c prescale"},
  {"i2cw",      i2cw,        "[i2cw <chipaddr> <regaddr> <wdata>]",  "--i2c write"},
  {"i2cr",      i2cr,        "[i2cr <chipaddr> <regaddr>]",          "--i2c read"},
  {"adc",       adc,         "[adc]",                 "--adc"},
  {"batdet",    batdet,      "[batdet <n>]",          "--v1"},
  {"copy",      copy,        "[spi_flash=>in_flash]", "--v1"},
  {"copy_iram", copy_iram,   "[copy]",                "--batdet <n>"},
  {"uinit",     uart_init,   "[uart init]",           "--v1"},
  {"spinit",    spi_init,    "[spi_init]",            "--v1"},
  {"sperase",   spi_erase,   "[spi_erase]",           "--v1"},
  {"spwr",      spi_write,   "[spi_write]",           "--v1"},
  {"sprd",      spi_read,    "[spi_read]",            "--v1"},
  {"sflash",    spi_flash,   "[spi_flash]",           "--v1"},
  {"gpio_int",  gpio_int,    "[exti]",                "--v1"},
  {"m4f",		m4_flash,    "[exti]",                "--v1"},
  {"t",         tmp,         "[exti]",                "--v1"},
//add
  {"NULL",      do_nothing,  "[NULL]",                "NULL"}
};

int do_exit(int argc, void *argv[])
{
    return 0;
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
//							printf("cmdpara[%d]: %s\n", j, cmdpara[j]);
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

