/**
  * @file    cs32f0xx_dma.c
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of the Direct Memory Access controller (DMA):
  *           + Initialization and Configuration
  *           + Data Counter
  *           + Interrupts and flags management
  * @author  ChipSea MCU Group
  * @version V1.0.0
  * @date   2018.11.01         
  * @copyright CHIPSEA TECHNOLOGIES (SHENZHEN) CORP.
  * @note
  * <h2><center>&copy; COPYRIGHT 2018 ChipSea</center></h2>
  *
  * @verbatim  
  *
  ----------------------- this driver instruction for use--------------------------- 
    1. First enable the DMA controller clock,then enable and configure the peripheral 
       to be connected to the DMA channel.
    2. Initializes the DMA Channelx according to the parameters in the dma_config_t.          
    3. Enable the NVIC and the corresponding interrupt using dma_interrupt_set() ,
       if needing to use DMA interrupts.
    4. Enable the DMA channel using the dma_enable_ctrl() .   
    5. Using one of the following two methods to control DMA events:
        a.Check on DMA channel flags using the function dma_flag_status_get().
        b.Configure DMA interrupts by dma_interrupt_set() at initialization
          and check interrupt flag by dma_interrupt_status_get() during communication.  
          using dma_flag_clear() to clear flag,clear an interrupt flag  using 
          dma_interrupt_flag_clear().
    @endverbatim
  *
  */

#include "cs32f0xx_dma.h"


/** @addtogroup CS32F0xx_HAL_Driver
  * @{
  */

/** @defgroup DMA
  * @brief DMA driver modules
  * @{
  */


/** 
  * @brief DMA register bit mask  definition  
  */ 
#define DMA_CHxCTR_CONFIG_MASK          ((uint32_t)0xFFFF800F) //!< DMA channel x controller register Masks 
#define DMA_INTERRUPT_FLAG_MASK         ((uint32_t)0x10000000) //!< DMA interrupt status flag bits mask

/** 
  * @brief DMA1 Channelx interrupt  intergral flag bit definition  
  */ 
#define DMA1_CHANNEL1_INTERRUPT_FLAG    ((uint32_t)(DMA_STS_GIF1 | DMA_STS_CMPIF1 | DMA_STS_HLFIF1 | DMA_STS_ERRIF1)) //!< DMA1 Channel1 interrupt intergral flag
#define DMA1_CHANNEL2_INTERRUPT_FLAG    ((uint32_t)(DMA_STS_GIF2 | DMA_STS_CMPIF2 | DMA_STS_HLFIF2 | DMA_STS_ERRIF2)) //!< DMA1 Channel2 interrupt intergral flag
#define DMA1_CHANNEL3_INTERRUPT_FLAG    ((uint32_t)(DMA_STS_GIF3 | DMA_STS_CMPIF3 | DMA_STS_HLFIF3 | DMA_STS_ERRIF3)) //!< DMA1 Channel3 interrupt intergral flag
#define DMA1_CHANNEL4_INTERRUPT_FLAG    ((uint32_t)(DMA_STS_GIF4 | DMA_STS_CMPIF4 | DMA_STS_HLFIF4 | DMA_STS_ERRIF4)) //!< DMA1 Channel4 interrupt intergral flag
#define DMA1_CHANNEL5_INTERRUPT_FLAG    ((uint32_t)(DMA_STS_GIF5 | DMA_STS_CMPIF5 | DMA_STS_HLFIF5 | DMA_STS_ERRIF5)) //!< DMA1 Channel5 interrupt intergral flag


/**
  * @fn void dma_def_init(dma_channel_reg_t* ptr_channel_struct)
  * @brief  Deinitializes the DMA Channelx registers to their default reset values.   
  * @param  ptr_channel_struct: pointer to a dma_channel_reg_t struct, it can be anyone of  
  *         channel 1 to channel 5 in DMA1.
  * @return None
  */
void dma_def_init(dma_channel_reg_t* ptr_channel_struct)
{
    ASSERT(DMA_CHANNEL_CHECK(ptr_channel_struct));

    /* Disable the selected DMA Channel */
    ptr_channel_struct->CHxCTR &= (uint16_t)(~DMA_CHxCTR_CEN);

    /* clear DMA Channelx register value*/
    ptr_channel_struct->CHxCTR = 0;
    ptr_channel_struct->CHxNUM = 0;
    ptr_channel_struct->CHxPA = 0;
    ptr_channel_struct->CHxMA = 0;

    /* clear specifies DMA Channel interrupt intergral flag bits  */
    if (ptr_channel_struct == DMA1_CHANNEL1)
    {
        DMA1->INTFC |= DMA1_CHANNEL1_INTERRUPT_FLAG;
    }
    else if (ptr_channel_struct == DMA1_CHANNEL2)
    {
        DMA1->INTFC |= DMA1_CHANNEL2_INTERRUPT_FLAG;
    }
    else if (ptr_channel_struct == DMA1_CHANNEL3)
    {
        DMA1->INTFC |= DMA1_CHANNEL3_INTERRUPT_FLAG;
    }
    else if (ptr_channel_struct == DMA1_CHANNEL4)
    {
        DMA1->INTFC |= DMA1_CHANNEL4_INTERRUPT_FLAG;
    }
    else
    { 
        if (ptr_channel_struct == DMA1_CHANNEL5)
        {
            DMA1->INTFC |= DMA1_CHANNEL5_INTERRUPT_FLAG;
        }
    } 
}

/**
  * @fn void dma_init(dma_channel_reg_t* ptr_channel_struct, dma_config_t* ptr_config_struct)
  * @brief  Initializes the DMA Channel according to the specified parameters 
  *         in the ptr_config_struct.
  * @param  ptr_channel_struct: pointer to a dma_channel_reg_t struct,it can be anyone of  
  *         channel1 to channel 5 in DMA1.
  * @param  ptr_config_struct: pointer to a dma_config_t structure containing the configuration
  *         for the specified DMA Channel.
  * @return None
  */
void dma_init(dma_channel_reg_t* ptr_channel_struct, dma_config_t* ptr_config_struct)
{
    uint32_t reg = 0;

    ASSERT(DMA_CHANNEL_CHECK(ptr_channel_struct));
    ASSERT(DMA_TRANS_DIR_CHECK(ptr_config_struct->transfer_direct));
    ASSERT(DMA_BUFFER_SIZE_CHECK(ptr_config_struct->buf_size));
    ASSERT(DMA_PERI_INC_CHECK(ptr_config_struct->peri_inc_flag));
    ASSERT(DMA_MEM_INC_CHECK(ptr_config_struct->mem_inc_flag));
    ASSERT(DMA_PERI_DATA_WIDTH_CHECK(ptr_config_struct->peri_data_width));
    ASSERT(DMA_MEM_DATA_WIDTH_CHECK(ptr_config_struct->mem_data_width));
    ASSERT(DMA_OPERATE_MODE_CHECK(ptr_config_struct->operate_mode));
    ASSERT(DMA_CHANNEL_PRIORITY_CHECK(ptr_config_struct->priority_level));
    ASSERT(DMA_M2M_MODE_CHECK(ptr_config_struct->m2m_flag));

    if (ptr_config_struct->peri_base_addr == 0x40005428 || ptr_config_struct->peri_base_addr == 0x40005828)
    {
        ptr_config_struct->priority_level = DMA_CHANNEL_PRIORITY_HIGHEST;
    }
    /* Get the ptr_channel_struct CHxCTR value */
    reg = ptr_channel_struct->CHxCTR;

    /* Clear M2MM, PRIL, MWDH, PWDH, MAGM, PAGM, CIRM and DIR bits */
    reg &= DMA_CHxCTR_CONFIG_MASK;

    /* Configurate DMA's CHxCTR, CHxNum, CHxPA, CHxMA */
    reg |= ptr_config_struct->transfer_direct | ptr_config_struct->operate_mode |
           ptr_config_struct->peri_inc_flag | ptr_config_struct->mem_inc_flag |
           ptr_config_struct->peri_data_width | ptr_config_struct->mem_data_width |
           ptr_config_struct->priority_level | ptr_config_struct->m2m_flag;
  
    ptr_channel_struct->CHxCTR = reg;
    ptr_channel_struct->CHxNUM = ptr_config_struct->buf_size;
    
    ptr_channel_struct->CHxPA = ptr_config_struct->peri_base_addr;  
    ptr_channel_struct->CHxMA = ptr_config_struct->mem_base_addr;
}

/**
  * @fn void dma_struct_config(dma_config_t* ptr_config_struct)
  * @brief  config each ptr_config_struct member with its default value.
  * @param  ptr_config_struct: pointer to a dma_config_t structure being initialized.        
  * @return None
  */
void dma_struct_config(dma_config_t* ptr_config_struct)
{
    ptr_config_struct->peri_base_addr = 0;
    ptr_config_struct->mem_base_addr = 0;
  
    ptr_config_struct->transfer_direct = DMA_TRANS_DIR_FROM_PERI;
    ptr_config_struct->buf_size = 0;
    
    ptr_config_struct->peri_inc_flag = DMA_PERI_INC_DISABLE;
    ptr_config_struct->mem_inc_flag = DMA_MEM_INC_DISABLE;
  
    ptr_config_struct->peri_data_width = DMA_PERI_DATA_WIDTH_BYTE;
    ptr_config_struct->mem_data_width = DMA_MEM_DATA_WIDTH_BYTE;
    
    ptr_config_struct->operate_mode = DMA_OPERATE_MODE_NORMAL;
    ptr_config_struct->priority_level = DMA_CHANNEL_PRIORITY_LOW;
    
    ptr_config_struct->m2m_flag = DMA_M2M_MODE_DISABLE;
}

/**
  * @fn void dma_enable_ctrl(dma_channel_reg_t* ptr_channel_struct, enable_state_t enable_flag)
  * @brief  Enables or disables the specified DMA Channel.
  * @param  ptr_channel_struct: pointer to a dma_channel_reg_t struct, it can be anyone of  
  *         channel 1 to channel 5 in DMA1.
  * @param  enable_flag: It can be: ENABLE or DISABLE.
  * @return None
  */
void dma_enable_ctrl(dma_channel_reg_t* ptr_channel_struct, enable_state_t enable_flag)
{
    ASSERT(DMA_CHANNEL_CHECK(ptr_channel_struct));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));

    if (enable_flag != DISABLE)
    {
        /* Enable the selected DMA Channelx */
        ptr_channel_struct->CHxCTR |= DMA_CHxCTR_CEN;
    }
    else
    {
        /* Disable the selected DMA Channelx */
        ptr_channel_struct->CHxCTR &= (uint16_t)(~DMA_CHxCTR_CEN);
    }
}

/**
  * @fn void dma_data_counter_set(dma_channel_reg_t* ptr_channel_struct, uint16_t counter)
  * @brief  Sets the number of data units using the current DMA Channel transfer.
  *         It can only be used when the ptr_channel_struct is disabled.
  * @param  ptr_channel_struct: pointer to a dma_channel_reg_t struct, it can be anyone of  
  *         channel 1 to channel 5 in DMA1.
  * @param  counter: The number of data units to transfer.         
  * @return None.
  */
void dma_data_counter_set(dma_channel_reg_t* ptr_channel_struct, uint16_t counter)
{
    ASSERT(DMA_CHANNEL_CHECK(ptr_channel_struct));
    /* Set CHxNUM */
    ptr_channel_struct->CHxNUM = counter;
}

/**
  * @fn uint16_t dma_data_counter_get(dma_channel_reg_t* ptr_channel_struct)
  * @brief  Returns the number of remaining data units in the current DMA Channel transfer.
  * @param  ptr_channel_struct: pointer to a dma_channel_reg_t struct, it can be anyone of  
  *         channel 1 to channel 5 in DMA1. 
  * @return The number of remaining data units.
  */
uint16_t dma_data_counter_get(dma_channel_reg_t* ptr_channel_struct)
{
    ASSERT(DMA_CHANNEL_CHECK(ptr_channel_struct));
    /* Return CHxNUM */
    return ((uint16_t)(ptr_channel_struct->CHxNUM));
}

/**
  * @fn void dma_interrupt_set(dma_channel_reg_t* ptr_channel_struct, uint32_t set_value, enable_state_t enable_flag);
  * @brief  Enables or disables the specified DMA Channel interrupts.
  * @param  ptr_channel_struct: pointer to a dma_channel_reg_t struct,it can be anyone of  
  *         channel 1 to channel 5 in DMA1. 
  * @param  set_value: specifies the DMA interrupts sources to be enabled or disabled.      
  *          It can be any combination of the following values:
  *           @arg Transfer complete interrupt mask (DMA_INT_CONFIG_CMP) 
  *           @arg Half transfer interrupt mask (DMA_INT_CONFIG_HLF)
  *           @arg Transfer error interrupt mask (DMA_INT_CONFIG_ERR)
  * @param  enable_flag: the specified DMA interrupts ENABLE or DISABLE.
  * @return None
  */
void dma_interrupt_set(dma_channel_reg_t* ptr_channel_struct, uint32_t set_value, enable_state_t enable_flag)
{
    ASSERT(DMA_CHANNEL_CHECK(ptr_channel_struct));
    ASSERT(DMA_INT_CONFIG_CHECK(set_value));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
    
    if (enable_flag != DISABLE)
    {
        /* Enable the selected DMA interrupts */
        ptr_channel_struct->CHxCTR |= set_value;
    }
    else
    {
        /* Disable the selected DMA interrupts */
        ptr_channel_struct->CHxCTR &= ~set_value;
    }
}

/**
  * @fn bit_status_t dma_flag_status_get(uint32_t flag)
  * @brief  Checks whether the specified DMA Channel flag is set or not.
  * @param  flag: specifies the flag to check.   
  *          It can be one of the following value:
  *           @arg DMA1_FLAG_G1(Global interrupt flag of Channel 1)
  *           @arg DMA1_FLAG_CMP1(Transfer complete flag of Channel 1)
  *           @arg DMA1_FLAG_HLF1(Half transfer flag of Channel 1)
  *           @arg DMA1_FLAG_ERR1(Transfer error flag of Channel 1)    
  *           @arg DMA1_FLAG_G2(Global interrupt flag of Channel 2) 
  *           @arg DMA1_FLAG_CMP2(Transfer complete flag of Channel 2)
  *           @arg DMA1_FLAG_HLF2(Half transfer flag of Channel 2)
  *           @arg DMA1_FLAG_ERR2(Transfer error flag of Channel 2) 
  *           @arg DMA1_FLAG_G3(Global interrupt flag of Channel 3) 
  *           @arg DMA1_FLAG_CMP3(Transfer complete flag of Channel 3) 
  *           @arg DMA1_FLAG_HLF3(Half transfer flag of Channel 3)
  *           @arg DMA1_FLAG_ERR3(Transfer error flag of Channel 3) 
  *           @arg DMA1_FLAG_G4(Global interrupt flag of Channel 4) 
  *           @arg DMA1_FLAG_CMP4(Transfer complete flag of Channel 4) 
  *           @arg DMA1_FLAG_HLF4(Half transfer flag of Channel 4)
  *           @arg DMA1_FLAG_ERR4(Transfer error flag of Channel 4) 
  *           @arg DMA1_FLAG_G5(Global interrupt flag of Channel 5) 
  *           @arg DMA1_FLAG_CMP5(Transfer complete flag of Channel 5) 
  *           @arg DMA1_FLAG_HLF5(Half transfer flag of Channel 5)
  *           @arg DMA1_FLAG_ERR5(Transfer error flag of Channel 5) 
  * @return The new state (SET or RESET).
  */
bit_status_t dma_flag_status_get(uint32_t flag)
{
    bit_status_t bits_tatus = RESET;
  
    ASSERT(DMA_GET_FLAG_CHECK(flag));

    /* Check the specified DMA flag status*/
    if ((DMA1->STS & flag) != (uint32_t)RESET)
    {
        bits_tatus = SET;
    }
    else
    {   
        bits_tatus = RESET;
    }
  
    return  bits_tatus;
}

/**
  * @fn void dma_flag_clear(uint32_t flag)
  * @brief  Clears the DMA Channel flags.
  * @param  flag: specifies the flag to clear.
  *          It can be one of the following value:
  *           @arg DMA1_FLAG_G1(Global interrupt flag of Channel 1)
  *           @arg DMA1_FLAG_CMP1(Transfer complete flag of Channel 1) 
  *           @arg DMA1_FLAG_HLF1(Half transfer flag of Channel 1)
  *           @arg DMA1_FLAG_ERR1(Transfer error flag of Channel 1)    
  *           @arg DMA1_FLAG_G2(Global interrupt flag of Channel 2) 
  *           @arg DMA1_FLAG_CMP2(Transfer complete flag of Channel 2) 
  *           @arg DMA1_FLAG_HLF2(Half transfer flag of Channel 2)
  *           @arg DMA1_FLAG_ERR2(Transfer error flag of Channel 2) 
  *           @arg DMA1_FLAG_G3(Global interrupt flag of Channel 3) 
  *           @arg DMA1_FLAG_CMP3(Transfer complete flag of Channel 3)
  *           @arg DMA1_FLAG_HLF3(Half transfer flag of Channel 3)
  *           @arg DMA1_FLAG_ERR3(Transfer error flag of Channel 3) 
  *           @arg DMA1_FLAG_G4(Global interrupt flag of Channel 4) 
  *           @arg DMA1_FLAG_CMP4(Transfer complete flag of Channel 4) 
  *           @arg DMA1_FLAG_HLF4(Half transfer flag of Channel 4)
  *           @arg DMA1_FLAG_ERR4(Transfer error flag of Channel 4) 
  *           @arg DMA1_FLAG_G5(Global interrupt flag of Channel 5) 
  *           @arg DMA1_FLAG_CMP5(Transfer complete flag of Channel 5) 
  *           @arg DMA1_FLAG_HLF5(Half transfer flag of Channel 5)
  *           @arg DMA1_FLAG_ERR5(Transfer error flag of Channel 5) 
  * @return None
  */
void dma_flag_clear(uint32_t flag)
{
    ASSERT(DMA_CLEAR_FLAG_CHECK(flag));

    DMA1->INTFC = flag;
}

/**
  * @fn bit_status_t dma_interrupt_status_get(uint32_t flag)
  * @brief  Checks whether the specified DMA Channel interrupt has occurred or not.
  * @param  flag: specifies the DMA interrupt source to check.     
  *          It can be one of the following value:
  *           @arg DMA1_INT_G1(Global interrupt of Channel 1)
  *           @arg DMA1_INT_CMP1(Transfer complete interruptof Channel 1)
  *           @arg DMA1_INT_HLF1(Half transfer interrupt of Channel 1)
  *           @arg DMA1_INT_ERR1(Transfer error interrupt of Channel 1)   
  *           @arg DMA1_INT_G2(Global interrupt of Channel 2)
  *           @arg DMA1_INT_CMP2(Transfer complete interruptof Channel 2)
  *           @arg DMA1_INT_HLF2(Half transfer interrupt of Channel 2)
  *           @arg DMA1_INT_ERR2(Transfer error interrupt of Channel 2) 
  *           @arg DMA1_INT_G3(Global interrupt of Channel 3)
  *           @arg DMA1_INT_CMP3(Transfer complete interrupt of Channel 3)
  *           @arg DMA1_INT_HLF3(Half transfer interrupt of Channel 3)
  *           @arg DMA1_INT_ERR3(Transfer error interrupt of Channel 3) 
  *           @arg DMA1_INT_G4(Global interrupt of Channel 4)
  *           @arg DMA1_INT_CMP4(Transfer complete interrupt of Channel 4) 
  *           @arg DMA1_INT_HLF4(Half transfer interrupt of Channel 4)
  *           @arg DMA1_INT_ERR4(Transfer error interrupt of Channel 4) 
  *           @arg DMA1_INT_G5(Global interrupt of Channel 5)
  *           @arg DMA1_INT_CMP5(Transfer complete interrupt of Channel 5) 
  *           @arg DMA1_INT_HLF5(Half transfer interrupt of Channel 5)
  *           @arg DMA1_INT_ERR5(Transfer error interrupt of Channel 5) 
  * @return The new state (SET or RESET).
  */
bit_status_t dma_interrupt_status_get(uint32_t flag)
{
    bit_status_t bits_tatus = RESET;
    
    ASSERT(DMA_GET_INT_CHECK(flag));

    /* Check the specified DMA interrupt status */
    if ((DMA1->STS & flag) != (uint32_t)RESET)
    {
        bits_tatus = SET;
    }
    else
    {
        bits_tatus = RESET;
    }

    return  bits_tatus;
}

/**
  * @fn void dma_interrupt_flag_clear(uint32_t flag)
  * @brief  Clears the DMA Channel interrupt bits.
  * @param  flag: specifies the DMA interrupt pending bit to clear.   
  *          It can be one of the following value:
  *           @arg DMA1_INT_G1(Global interrupt of Channel 1)
  *           @arg DMA1_INT_CMP1(Transfer complete interruptof Channel 1)
  *           @arg DMA1_INT_HLF1(Half transfer interrupt of Channel 1)
  *           @arg DMA1_INT_ERR1(Transfer error interrupt of Channel 1)    
  *           @arg DMA1_INT_G2(Global interrupt of Channel 2)
  *           @arg DMA1_INT_CMP2(Transfer complete interruptof Channel 2)
  *           @arg DMA1_INT_HLF2(Half transfer interrupt of Channel 2)
  *           @arg DMA1_INT_ERR2(Transfer error interrupt of Channel 2) 
  *           @arg DMA1_INT_G3(Global interrupt of Channel 3)
  *           @arg DMA1_INT_CMP3(Transfer complete interrupt of Channel 3)
  *           @arg DMA1_INT_HLF3(Half transfer interrupt of Channel 3)
  *           @arg DMA1_INT_ERR3(Transfer error interrupt of Channel 3) 
  *           @arg DMA1_INT_G4(Global interrupt of Channel 4)
  *           @arg DMA1_INT_CMP4(Transfer complete interrupt of Channel 4)
  *           @arg DMA1_INT_HLF4(Half transfer interrupt of Channel 4)
  *           @arg DMA1_INT_ERR4(Transfer error interrupt of Channel 4) 
  *           @arg DMA1_INT_G5(Global interrupt of Channel 5)
  *           @arg DMA1_INT_CMP5(Transfer complete interrupt of Channel 5)
  *           @arg DMA1_INT_HLF5(Half transfer interrupt of Channel 5)
  *           @arg DMA1_INT_ERR5(Transfer error interrupt of Channel 5) 
  * @return None
  */
void dma_interrupt_flag_clear(uint32_t flag)
{
    ASSERT(DMA_CLEAR_INT_CHECK(flag));
  
    /* Clear the DMA1 specified interrupt flag */
    DMA1->INTFC = flag;
}

/**
  * @}
  */

/**
  * @}
  */

