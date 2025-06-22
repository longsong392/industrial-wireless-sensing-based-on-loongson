#include "ls1x.h"
#include "ls1x_flash.h"
#include "ls1x_clock.h"
#include "ls1x_latimer.h"
#include "Config.h"
#include "ls1x_string.h"
/**
  * @brief  Erases a specified FLASH page.
  * @param  Page_Address: The page address to be erased.
  * @retval None
  */
uint8_t Flash_pe;

void FLASH_ErasePage(uint32_t Page_Address)  //erase a flash page, FLASH_PAGE_SIZE
{
//	uint8_t err_code;
    ////////////////   ///////////////
    assert_param(IS_FLASH_ADDRESS(Page_Address));
//	FLASH->PET = 0x10;    // erase time, 2.5ms
    FLASH->CMD = FLASH_ERASE_CMD | (Page_Address & FLASH_ADDR_MASK);
    FLASH_WaitForPeEnd(EraseTimeout);
//	FLASH_ClearIT();
}

/**
  * @brief  Erases all User FLASH pages.
  * @param  Page_Address: The page address to be erased.
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
void FLASH_EraseUserFlash(uint32_t Page_Address)
{
    uint32_t i=0;

    Page_Address |= LS1D_FLASH_ADDR;
    assert_param(IS_FLASH_ADDRESS(Page_Address));
    uint32_t start_addr = Page_Address;                //LS1D_FLASH_ADDR片上Flash的起始地址
    uint32_t page = (0xbf01ffff - start_addr)/FLASH_PAGE_SIZE;   //0xbf01ffff片上Flash的上线，计算多少页

    for(i=0; i<page; i++)
    {
        FLASH_ErasePage(start_addr+i*FLASH_PAGE_SIZE);
        delay_us(10);
    }
}

/**
  * @brief  Read FLASH data.
  * @param  Page_Address: The page address to be erased.
  * @param  *data: The data pointer to the readed data.
  * @param  num: The number of the readed data.
  * @retval The number of the readed data.
  */
uint32_t FLASH_ReadPage(uint32_t Page_Address, uint8_t *data, uint32_t num)
{
    int i = 0;
    uint8_t *ptr;

    if (Page_Address + num > LS1D_FLASH_TOTAL)    //LS1D_FLASH_TOTAL:flash大小
        return -1;

    Page_Address |= LS1D_FLASH_ADDR;
    assert_param(IS_FLASH_ADDRESS(Page_Address));
    ptr = (uint8_t *)Page_Address;

    for(i = 0; i < num; i++)
    {
        *data++ = *ptr++;
    }

    return num;
}

/**
  * @brief  Write words at a specified address.
  * @param  Page_Address: specifies the address to be programmed.
  * @param  *data: specifies the data to be programmed.
  * @param  num: specifies the number of data to be programmed.
  * @retval FLASH Status: The returned value can be: verif_correct,
  *         verif_end, pe_end,FLASH_TIMEOUTor no_permission.
  */
uint8_t FLASH_WitePage(uint32_t Page_Address, uint8_t *data, uint32_t num)
{
    uint32_t temp_flash_buf;
    //uint8_t status;
    uint8_t *temp_data = data;

    Page_Address |= LS1D_FLASH_ADDR;									//addr加上LS1D_FLASH_ADDR（0xbf000000）基地址
    assert_param(IS_FLASH_ADDRESS(Page_Address));

    FLASH->CMD = FLASH_PAGE_LATCH_CLEAR;     				//FLASH_CMD_REG=0x40000000
    uint32_t flash_block_mask = FLASH_PAGE_SIZE - 1;  			//flash_block_mask=127（0x0000007F）
    uint32_t *page_data = (uint32_t *)(Page_Address & ~flash_block_mask) ;	//*page_data指针指向addr地址的页地址
    uint32_t *old_data = (uint32_t *)(Page_Address & ~flash_block_mask) ;	//*old_data指针指向addr地址的页地址
    uint32_t i, j, k, offset;

    j = 0;
    uint32_t count = num;
    uint32_t num_left = 0;
    uint32_t num_right = 0;
    uint32_t addr_left_align = 0xffffffff;
    uint32_t addr_right_align = 0xffffffff;
    uint32_t word_mask = WORD_WIDTH - 1;		//word_mask=3
    num = 0;

    num_left = Page_Address & word_mask;   			//addr=124928(0x1E800),word_mask=3(0x00003),addr& word_mask=0
    if (num_left) 							//假如num_left=3
    {
        num_left = WORD_WIDTH - num_left;   //num_left=1
        num++;								//num=1
        addr_left_align = (Page_Address & flash_block_mask) >> 2; //addr_left_align更改为操作地址低8位区0右移2位
    }
    j = count - num_left;   //j=操作起始地址

    num_right = (j & word_mask);
    if (num_right)
    {
        num++;
        addr_right_align = Page_Address + count - num_right;
        addr_right_align = (addr_right_align & flash_block_mask) >> 2;
    }
    j = count - num_left - num_right;
    j = j >> 2;
    num += j;

    offset = ((Page_Address & flash_block_mask)>>2);

    for(j=0, i=0; i<(FLASH_PAGE_SIZE/4); i++)
    {
        if( (offset <= i)  && (j < num) )
        {
            if (i == addr_left_align)
            {
                int m = 0;
                temp_flash_buf = old_data[i];
                k = 1 << ((WORD_WIDTH - num_left) * 8);
                temp_flash_buf &= (k - 1);
                for (m = 0; m < num_left; m++)
                {
                    temp_flash_buf |= temp_data[m] << (WORD_WIDTH - num_left + m)*8;
                }
                temp_data = temp_data + num_left;	//address change to be logic 0 again.
                page_data[i] = temp_flash_buf;
                num--;                           	//because j don't add 1, num has to sub 1.
            }
            else if (i == addr_right_align)
            {
                temp_flash_buf = old_data[i];
                k = 1 << (num_right * 8);
                temp_flash_buf &= ~(k - 1);
                for (k = 0; k < num_right; k++)
                {
                    temp_flash_buf |= temp_data[j*4+k] << k*8;
                }
                j++;
                page_data[i] = temp_flash_buf;
            }
            else
            {
                temp_flash_buf = 0;
                for (k = 0; k < WORD_WIDTH; k++)
                {
                    temp_flash_buf |= temp_data[j*4+k] << k*8;
                }
                j++;
                page_data[i] = temp_flash_buf;
            }
        }
        else
            page_data[i] = old_data[i];
    }
    Flash_pe=0;
    FLASH_ErasePage(Page_Address);  //erase a flash page, FLASH_PAGE_SIZE

    FLASH->CMD = FLASH_WRITE_CMD | (Page_Address & FLASH_ADDR_MASK) ;
    FLASH_WaitForPeEnd(ProgramTimeout);
    return TRUE;
}

bool ls1c_flash_write2(uint32_t addr, const uint8_t *data, uint32_t size)
{
    uint32_t pageAddr = (addr / FLASH_PAGE_SIZE) * FLASH_PAGE_SIZE; //写入页地址
    uint32_t writeSize = FLASH_PAGE_SIZE - addr + pageAddr;         //每页写入多少
    writeSize = writeSize > size ? size : writeSize;
    uint8_t _data[FLASH_PAGE_SIZE];
//    uint8_t *_data = (uint8_t *)Space_new(FLASH_PAGE_SIZE);
    uint8_t *_wptr = (uint8_t*)data;
    while (size > 0)
    {

        if (writeSize != FLASH_PAGE_SIZE)
        {
            if (FLASH_ReadPage(pageAddr, _data, FLASH_PAGE_SIZE) != FLASH_PAGE_SIZE)
            {
                //Space_free(_data) ;
                return FALSE;
            }
            memcpy(_data + addr - pageAddr, data, writeSize);
            _wptr = _data;
        }
        else
        {
            _wptr = (uint8_t*)data;
        }
        int res = FLASH_WitePage(pageAddr, _wptr, FLASH_PAGE_SIZE);
        if (res != TRUE)
        {
            //Space_free(_data) ;
            return FALSE;
        }
        size -= writeSize;
        data += writeSize;           //读地址后移
        pageAddr += FLASH_PAGE_SIZE; //每次后移一页
        addr = pageAddr;             //除了第一次可能不与页起始地址对齐其他都对其
        writeSize = size > FLASH_PAGE_SIZE ? FLASH_PAGE_SIZE : size;
    }
    //Space_free(_data) ;
    return TRUE;
}

bool ls1c_flash_write_data(uint32_t addr, const uint8_t c, uint32_t size)
{
    //uint8_t *dat = (uint8_t *)Space_new(FLASH_PAGE_SIZE);
    uint8_t dat[FLASH_PAGE_SIZE];

    if(size <= FLASH_PAGE_SIZE)
    {
        memset(dat,c,size);
        ls1c_flash_write2(addr,dat,size);
    }
    else
    {
        uint8_t page = size / FLASH_PAGE_SIZE;
        uint8_t off = size % FLASH_PAGE_SIZE;
        uint8_t i;
        for(i=0; i<page; i++)
        {
            memset(dat,c,FLASH_PAGE_SIZE);
            ls1c_flash_write2(addr+i*FLASH_PAGE_SIZE,dat,FLASH_PAGE_SIZE);
        }
        if(off!=0)
        {
            memset(dat,c,off);
            ls1c_flash_write2(addr+page*FLASH_PAGE_SIZE,dat,off);
        }
    }
    //Space_free(dat) ;
    return TRUE;
}
/**
  * @brief  Waits for a Flash operation to complete or a TIMEOUT to occur.
  * @param  Timeout: FLASH programming Timeout
  * @retval FLASH Status: The returned value can be: verif_correct,
  *         verif_end, pe_end,FLASH_TIMEOUTor no_permission.
  */
uint8_t FLASH_WaitForPeEnd(uint32_t Timeout)
{
    uint8_t status;

    /* Check for the Flash Status */
    /* Wait for a Flash operation to complete or a TIMEOUT to occur */
    status = FLASH_GetStatus();
    while((Timeout != 0x00)&& ((status & 0x04)==0x00))
    {
// (status != pe_end) &&	status = FLASH_GetStatus();
        status = FLASH_GetStatus();
        delay_ms(1);
        Timeout--;
    }
    if(Timeout == 0x00 )
    {
        status = FLASH_TIMEOUT;
    }
    /* Return the operation status */
    return status;
}

/**
  * @brief  Checks whether the specified FLASH flag is set or not.
  * @retval The new state of FLASH_FLAG (SET or RESET).
  */
uint8_t FLASH_GetStatus(void)
{
    uint8_t status;
    status=(FLASH->STS & 0x0F);
    return status;
}
/**
  * @brief  Checks whether the specified FLASH flag is set or not.
  * @param  FLASH_FLAG: specifies the FLASH flag to check.
  *   This parameter can be one of the following values:
  *     @arg FLASH_FLAG_VERIFCORRECT: Verify Correct flag
  *     @arg FLASH_FLAG_VERIFEND: Verify End flag
  *     @arg FLASH_FLAG_PEEND: Pe End flag
  *     @arg FLASH_FLAG_NOPERMISSION: No Permission flag
  * @retval The new state of FLASH_FLAG (SET or RESET).
  */
FlagStatus FLASH_GetFlagStatus(uint32_t FLASH_FLAG)
{
    ITStatus bitstatus = RESET;
    assert_param(IS_FLASH_GET_FLAG(FLASH_FLAG)) ;
    if((FLASH->STS & FLASH_FLAG) != (uint32_t)RESET)
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }
    return bitstatus;
}

/**
  * @brief  Enables or disables the specified FLASH interrupts.
  * @param  NewState: new state of the specified Flash interrupts.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void FLASH_ITConfig(uint32_t FLASH_IT, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FLASH_CONFIG_IT(FLASH_IT));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    (NewState != DISABLE)? (SET_BIT(FLASH->PET,FLASH_IT)) : (CLEAR_BIT(FLASH->PET,FLASH_IT));

}
/**
  * @brief  Enables or disables the FLASH interrupts.
  * @param  NewState: new state of the specified Flash interrupts.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void FLASH_ITCmd(FunctionalState NewState)
{
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    (NewState != DISABLE)? (SET_BIT(INT->INTC_EN,IRQ_FLASH)) : (CLEAR_BIT(INT->INTC_EN,IRQ_FLASH));	//清除中断标记位
}
/**
  * @brief  Clears the FLASH's interrupt flag.
  * @retval None
  */

void FLASH_ClearIT(void)
{
    /* clear data reg*/
    SET_BIT(FLASH->CMD , FLASH_INT_CLEAR);
#if !defined (LS1C103)
    SET_BIT(INT->INTC_CLR,IRQ_FLASH);	//清除中断标记位
#endif

}
/**
 *@brief  Example of FLASH Write.
 *retval None
 */
#define FLASH_TEST_LEN		128
void FLASH_WiteTest(uint32_t Page_Address,uint8_t Page_data)
{
    uint8_t temp[FLASH_TEST_LEN]= {0};
    uint32_t USER_BEGIN = Page_Address;
    //FLASH_Status status;
    int i;
    for(i=0; i<FLASH_TEST_LEN; i++)
        temp[i] = Page_data;
    Flash_pe=0;
//    status=FLASH_WitePage(USER_BEGIN, temp, FLASH_TEST_LEN);
    ls1c_flash_write2(USER_BEGIN, temp, FLASH_TEST_LEN);

}

/**
 *@brief  Example of FLASH Read.
 *retval None
 */
void FLASH_ReadTest(uint32_t Page_Address)
{
    uint8_t temp[FLASH_TEST_LEN]= {0};
    uint32_t USER_BEGIN = Page_Address;
    int i;
    FLASH_ReadPage(USER_BEGIN,temp,FLASH_TEST_LEN);
    for(i=0; i<FLASH_TEST_LEN; i++)
    {
        if(i%9 == 0)
            printf("\r\n");
        printf("%02x ",temp[i]);	//十六进制输出
    }
}

