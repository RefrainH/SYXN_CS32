/**
  * @file    cs32f0xx_dma.h
  * @brief   This file contains all the functions prototypes for the DMA firmware
  *          library.
  * @author  ChipSea MCU Group
  * @version V1.0.0
  * @date   2018.11.01         
  * @copyright CHIPSEA TECHNOLOGIES (SHENZHEN) CORP.
  * @note
  * <h2><center>&copy; COPYRIGHT 2018 ChipSea</center></h2>
  *
  */

#ifndef __CS32F0XX_DMA_H__
#define __CS32F0XX_DMA_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "cs32f0xx.h"


/** @addtogroup CS32F0xx_HAL_Driver
  * @{
  */

/** @addtogroup DMA
  * @{
  */

/** 
  * @struct dma_config_t
  * @brief DMA channel config structures definition
  */
typedef struct
{
    uint32_t peri_base_addr;     //!<  DMA Channel the peripheral base address.             
    uint32_t mem_base_addr;      //!<  DMA Channel the memory base address.               
    uint32_t transfer_direct;    //!<  DMA transfer Direction                                        
    uint32_t buf_size;           //!<  DMA specifies the buffer size(data unit equal to peri_data_width (or mem_data_width)).    
    uint32_t peri_inc_flag;      //!<  DMA enable or disable the peripheral address increment.                                      
    uint32_t mem_inc_flag;       //!<  DMA enable or disable the memory address increment.                                        
    uint32_t peri_data_width;    //!<  DMA specifies the peripheral data width.                                     
    uint32_t mem_data_width;     //!<  DMA specifies the memory data width.                                        
    uint32_t operate_mode;       //!<  DMA Channel operation mode.                                                                                     
    uint32_t priority_level;     //!<  DMA set the Channel priority.                                                  
    uint32_t m2m_flag;           //!<  DMA enable or disable the Channel used in memory-to-memory transfer.                                       
}dma_config_t;


/** 
  * @ brief DMA data transfer direction 
  */
#define DMA_CHANNEL_CHECK(channel) (((channel) == DMA1_CHANNEL1) || ((channel) == DMA1_CHANNEL2) || \
                                    ((channel) == DMA1_CHANNEL3) || ((channel) == DMA1_CHANNEL4) || \
                                    ((channel) == DMA1_CHANNEL5)) 

/** 
  * @ brief DMA data transfer direction 
  */
#define DMA_TRANS_DIR_FROM_PERI        ((uint32_t)0x00000000)  //!< DMA transfer data from peripheral 
#define DMA_TRANS_DIR_FROM_MEM          DMA_CHxCTR_DIR         //!< DMA transfer data from memory 
#define DMA_TRANS_DIR_CHECK(direct) (((direct) == DMA_TRANS_DIR_FROM_PERI) || ((direct) == DMA_TRANS_DIR_FROM_MEM))
                         


/**  
  * @ brief DMA peripheral increment enable defintion
  */
#define DMA_PERI_INC_DISABLE          ((uint32_t)0x00000000)  //!< DMA disable peripheral increment
#define DMA_PERI_INC_ENABLE           DMA_CHxCTR_PAGM         //!< DMA enable peripheral increment
#define DMA_PERI_INC_CHECK(flag) (((flag) == DMA_PERI_INC_DISABLE) || ((flag) == DMA_PERI_INC_ENABLE))
                                            


/**  
  * @ brief DMA memory increment enable defintion
  */
#define DMA_MEM_INC_DISABLE        ((uint32_t)0x00000000)     //!< DMA disable memory increment 
#define DMA_MEM_INC_ENABLE          DMA_CHxCTR_MAGM           //!< DMA enable memory increment 
#define DMA_MEM_INC_CHECK(flag)    (((flag) == DMA_MEM_INC_DISABLE) || ((flag) == DMA_MEM_INC_ENABLE))
                                        

/** 
  * @ brief DMA peripheral data width defintion
  */
#define DMA_PERI_DATA_WIDTH_BYTE        ((uint32_t)0x00000000)  //!< DMA peripheral width is BYTE 
#define DMA_PERI_DATA_WIDTH_HALFWORD      DMA_CHxCTR_PWDH_0     //!< DMA peripheral width is HALFWORD 
#define DMA_PERI_DATA_WIDTH_WORD          DMA_CHxCTR_PWDH_1     //!< DMA peripheral width is WORD 

#define DMA_PERI_DATA_WIDTH_CHECK(size) (((size) == DMA_PERI_DATA_WIDTH_BYTE) || \
                                         ((size) == DMA_PERI_DATA_WIDTH_HALFWORD) || \
                                         ((size) == DMA_PERI_DATA_WIDTH_WORD))
/** 
  * @ brief DMA memory data width defintion
  */
#define DMA_MEM_DATA_WIDTH_BYTE            ((uint32_t)0x00000000)  //!< DMA memory width is BYTE 
#define DMA_MEM_DATA_WIDTH_HALFWORD        DMA_CHxCTR_MWDH_0       //!< DMA mmeory width is HALFWORD 
#define DMA_MEM_DATA_WIDTH_WORD            DMA_CHxCTR_MWDH_1       //!< DMA memory width is WORD 

#define DMA_MEM_DATA_WIDTH_CHECK(SIZE) (((SIZE) == DMA_MEM_DATA_WIDTH_BYTE) || \
                                        ((SIZE) == DMA_MEM_DATA_WIDTH_HALFWORD) || \
                                        ((SIZE) == DMA_MEM_DATA_WIDTH_WORD))


/** 
  * @ brief DMA operation mode defintion
  */
#define DMA_OPERATE_MODE_NORMAL           ((uint32_t)0x00000000)   //!< DMA normal mode 
#define DMA_OPERATE_MODE_CIRCULAR          DMA_CHxCTR_CIRM         //!< DMA circular mode
#define DMA_OPERATE_MODE_CHECK(mode) (((mode) == DMA_OPERATE_MODE_NORMAL) || ((mode) == DMA_OPERATE_MODE_CIRCULAR))


/** 
  * @ brief DMA channel priority defintion
  */
#define DMA_CHANNEL_PRIORITY_HIGHEST       DMA_CHxCTR_PRIL         //!< DMA highest channel priority 
#define DMA_CHANNEL_PRIORITY_HIGH          DMA_CHxCTR_PRIL_1       //!< DMA high channel priority 
#define DMA_CHANNEL_PRIORITY_MEDIUM        DMA_CHxCTR_PRIL_0       //!< DMA medium channel priority 
#define DMA_CHANNEL_PRIORITY_LOW           ((uint32_t)0x00000000)  //!< DMA low channel priority 

#define DMA_CHANNEL_PRIORITY_CHECK(priority) (((priority) == DMA_CHANNEL_PRIORITY_HIGHEST) || \
                                              ((priority) == DMA_CHANNEL_PRIORITY_HIGH)    || \
                                              ((priority) == DMA_CHANNEL_PRIORITY_MEDIUM)  || \
                                              ((priority) == DMA_CHANNEL_PRIORITY_LOW))
                                                                
/** 
  * @ brief DMA memory_to_memory mode  defintion
  */
#define DMA_M2M_MODE_DISABLE       ((uint32_t)0x00000000)  //!< DMA disable M2MM (memory to memory mode)
#define DMA_M2M_MODE_ENABLE         DMA_CHxCTR_M2MM        //!< DMA enable M2MM (memory to memory mode)
#define DMA_M2M_MODE_CHECK(flag) (((flag) == DMA_M2M_MODE_DISABLE) || ((flag) == DMA_M2M_MODE_ENABLE))

/**  
  * @brief DMA buffer size check definition
  */
#define DMA_BUFFER_SIZE_CHECK(size) (((size) >= 0x1) && ((size) < 0x10000))

/**  
  * @brief DMA check definition
  */
#define DMA_BUS_CHECK(dma) 			((dma) == DMA1)
                               
/** @defgroup DMA_interrupts_definition
  * @{
  */
#define DMA_INT_CONFIG_CMP              DMA_CHxCTR_CMPIE    //!< Enable Transfer complete interrupt
#define DMA_INT_CONFIG_HLF              DMA_CHxCTR_HLFIE    //!< Enable Half transfer interrupt
#define DMA_INT_CONFIG_ERR              DMA_CHxCTR_ERRIE    //!< Enable Transfer error interrupt 
#define DMA_INT_CONFIG_CHECK(flag) ((((flag) & 0xFFFFFFF1) == 0x00) && ((flag) != 0x00))

#define DMA1_INT_G1                     DMA_STS_GIF1    //!< Global interrupt flag of Channel 1   
#define DMA1_INT_CMP1                   DMA_STS_CMPIF1  //!< Transfer complete flag of Channel 1   
#define DMA1_INT_HLF1                   DMA_STS_HLFIF1  //!< Half transfer flag of Channel 1  
#define DMA1_INT_ERR1                   DMA_STS_ERRIF1  //!< Transfer error flag of Channel 1  

#define DMA1_INT_G2                     DMA_STS_GIF2    //!< Global interrupt flag of Channe 2   
#define DMA1_INT_CMP2                   DMA_STS_CMPIF2  //!< Transfer complete flag of Channel 2   
#define DMA1_INT_HLF2                   DMA_STS_HLFIF2  //!< Half transfer flag of Channel 2  
#define DMA1_INT_ERR2                   DMA_STS_ERRIF2  //!< Transfer error flag of Channel 2 

#define DMA1_INT_G3                     DMA_STS_GIF3    //!< Global interrupt flag of Channel 3  
#define DMA1_INT_CMP3                   DMA_STS_CMPIF3  //!< Transfer complete flag of Channel 3   
#define DMA1_INT_HLF3                   DMA_STS_HLFIF3  //!< Half transfer flag of Channel 3 
#define DMA1_INT_ERR3                   DMA_STS_ERRIF3  //!< Transfer error flag of Channel 3 

#define DMA1_INT_G4                     DMA_STS_GIF4    //!< Global interrupt flag of Channel 4   
#define DMA1_INT_CMP4                   DMA_STS_CMPIF4  //!< Transfer complete flag of Channel 4   
#define DMA1_INT_HLF4                   DMA_STS_HLFIF4  //!< Half transfer flag of Channel 4  
#define DMA1_INT_ERR4                   DMA_STS_ERRIF4  //!< Transfer error flag of Channel 4  

#define DMA1_INT_G5                     DMA_STS_GIF5    //!< Global interrupt flag of Channel 5   
#define DMA1_INT_CMP5                   DMA_STS_CMPIF5  //!< Transfer complete flag of Channel 5   
#define DMA1_INT_HLF5                   DMA_STS_HLFIF5  //!< Half transfer flag of Channel 5  
#define DMA1_INT_ERR5                   DMA_STS_ERRIF5  //!< Transfer error flag of Channel 5  

#define DMA_GET_INT_CHECK(flag) (((flag) == DMA1_INT_G1)   || ((flag) == DMA1_INT_CMP1) || \
                                 ((flag) == DMA1_INT_HLF1) || ((flag) == DMA1_INT_ERR1) || \
                                 ((flag) == DMA1_INT_G2)   || ((flag) == DMA1_INT_CMP2) || \
                                 ((flag) == DMA1_INT_HLF2) || ((flag) == DMA1_INT_ERR2) || \
                                 ((flag) == DMA1_INT_G3)   || ((flag) == DMA1_INT_CMP3) || \
                                 ((flag) == DMA1_INT_HLF3) || ((flag) == DMA1_INT_ERR3) || \
                                 ((flag) == DMA1_INT_G4)   || ((flag) == DMA1_INT_CMP4) || \
                                 ((flag) == DMA1_INT_HLF4) || ((flag) == DMA1_INT_ERR4) || \
                                 ((flag) == DMA1_INT_G5)   || ((flag) == DMA1_INT_CMP5) || \
                                 ((flag) == DMA1_INT_HLF5) || ((flag) == DMA1_INT_ERR5))

#define DMA_CLEAR_INT_CHECK(flag) ((((flag) & 0xFFF00000) == 0x00) && ((flag) != 0x00))

/**
  * @}
  */

/** @defgroup DMA_flags_definition 
  * @{
  */
#define DMA1_FLAG_G1              DMA_STS_GIF1    //!< Global interrupt flag of Channel 1   
#define DMA1_FLAG_CMP1            DMA_STS_CMPIF1  //!< Transfer complete flag of Channel 1   
#define DMA1_FLAG_HLF1            DMA_STS_HLFIF1  //!< Half transfer flag of Channel 1 
#define DMA1_FLAG_ERR1            DMA_STS_ERRIF1  //!< Transfer error flag of Channel 1    

#define DMA1_FLAG_G2              DMA_STS_GIF2    //!< Global interrupt flag of Channel 2 
#define DMA1_FLAG_CMP2            DMA_STS_CMPIF2  //!< Transfer complete flag of Channel 2  
#define DMA1_FLAG_HLF2            DMA_STS_HLFIF2  //!< Half transfer flag of Channel 2
#define DMA1_FLAG_ERR2            DMA_STS_ERRIF2  //!< Transfer error flag of Channel 2    

#define DMA1_FLAG_G3              DMA_STS_GIF3    //!< Global interrupt flag of Channel 3  
#define DMA1_FLAG_CMP3            DMA_STS_CMPIF3  //!< Transfer complete flag of Channel 3  
#define DMA1_FLAG_HLF3            DMA_STS_HLFIF3  //!< Half transfer flag of Channel 3
#define DMA1_FLAG_ERR3            DMA_STS_ERRIF3  //!< Transfer error flag of Channel 3    

#define DMA1_FLAG_G4              DMA_STS_GIF4    //!< Global interrupt flag of Channel 4  
#define DMA1_FLAG_CMP4            DMA_STS_CMPIF4  //!< Transfer complete flag of Channel 4   
#define DMA1_FLAG_HLF4            DMA_STS_HLFIF4  //!< Half transfer flag of Channel 4 
#define DMA1_FLAG_ERR4            DMA_STS_ERRIF4  //!< Transfer error flag of Channel 4  

#define DMA1_FLAG_G5              DMA_STS_GIF5    //!< Global interrupt flag of Channel 5 
#define DMA1_FLAG_CMP5            DMA_STS_CMPIF5  //!< Transfer complete flag of Channel 5   
#define DMA1_FLAG_HLF5            DMA_STS_HLFIF5  //!< Half transfer flag of Channel 5
#define DMA1_FLAG_ERR5            DMA_STS_ERRIF5  //!< Transfer error flag of Channel 5    

#define DMA_GET_FLAG_CHECK(flag) (((flag) == DMA1_FLAG_G1)   || ((flag) == DMA1_FLAG_CMP1) || \
                                  ((flag) == DMA1_FLAG_HLF1) || ((flag) == DMA1_FLAG_ERR1) || \
                                  ((flag) == DMA1_FLAG_G2)   || ((flag) == DMA1_FLAG_CMP2) || \
                                  ((flag) == DMA1_FLAG_HLF2) || ((flag) == DMA1_FLAG_ERR2) || \
                                  ((flag) == DMA1_FLAG_G3)   || ((flag) == DMA1_FLAG_CMP3) || \
                                  ((flag) == DMA1_FLAG_HLF3) || ((flag) == DMA1_FLAG_ERR3) || \
                                  ((flag) == DMA1_FLAG_G4)   || ((flag) == DMA1_FLAG_CMP4) || \
                                  ((flag) == DMA1_FLAG_HLF4) || ((flag) == DMA1_FLAG_ERR4) || \
                                  ((flag) == DMA1_FLAG_G5)   || ((flag) == DMA1_FLAG_CMP5) || \
                                  ((flag) == DMA1_FLAG_HLF5) || ((flag) == DMA1_FLAG_ERR5))
                               
#define DMA_CLEAR_FLAG_CHECK(flag) ((((flag) & 0xFFF00000) == 0x00) && ((flag) != 0x00))
                              
/**
  * @}
  */


/**
  * @fn void dma_def_init(dma_channel_reg_t* ptr_channel_struct)
  * @brief  Deinitializes the DMA Channelx registers to their default reset values.   
  * @param  ptr_channel_struct: pointer to a dma_channel_reg_t struct, it can be anyone of  
  *         channel 1 to channel 5 in DMA1.
  * @return None
  */
void dma_def_init(dma_channel_reg_t* ptr_channel_struct);

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
void dma_init(dma_channel_reg_t* ptr_channel_struct, dma_config_t* ptr_config_struct);

/**
  * @fn void dma_struct_config(dma_config_t* ptr_config_struct)
  * @brief  config each ptr_config_struct member with its default value.
  * @param  ptr_config_struct: pointer to a dma_config_t structure being initialized.        
  * @return None
  */
void dma_struct_config(dma_config_t* ptr_config_struct);

/**
  * @fn void dma_enable_ctrl(dma_channel_reg_t* ptr_channel_struct, enable_state_t enable_flag)
  * @brief  Enables or disables the specified DMA Channel.
  * @param  ptr_channel_struct: pointer to a dma_channel_reg_t struct, it can be anyone of  
  *         channel 1 to channel 5 in DMA1.
  * @param  enable_flag: It can be: ENABLE or DISABLE.
  * @return None
  */
void dma_enable_ctrl(dma_channel_reg_t* ptr_channel_struct, enable_state_t enable_flag);

/**
  * @fn void dma_data_counter_set(dma_channel_reg_t* ptr_channel_struct, uint16_t counter)
  * @brief  Sets the number of data units using the current DMA Channel transfer.
  *         It can only be used when the ptr_channel_struct is disabled.
  * @param  ptr_channel_struct: pointer to a dma_channel_reg_t struct, it can be anyone of  
  *         channel 1 to channel 5 in DMA1.
  * @param  counter: The number of data units to transfer.         
  * @return None.
  */
void dma_data_counter_set(dma_channel_reg_t* ptr_channel_struct, uint16_t counter);

/**
  * @fn uint16_t dma_data_counter_get(dma_channel_reg_t* ptr_channel_struct)
  * @brief  Returns the number of remaining data units in the current DMA Channel transfer.
  * @param  ptr_channel_struct: pointer to a dma_channel_reg_t struct, it can be anyone of  
  *         channel 1 to channel 5 in DMA1. 
  * @return The number of remaining data units.
  */
uint16_t dma_data_counter_get(dma_channel_reg_t* ptr_channel_struct);

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
void dma_interrupt_set(dma_channel_reg_t* ptr_channel_struct, uint32_t set_value, enable_state_t enable_flag);

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
bit_status_t dma_flag_status_get(uint32_t flag);

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
void dma_flag_clear(uint32_t flag);

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
bit_status_t dma_interrupt_status_get(uint32_t flag);

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
void dma_interrupt_flag_clear(uint32_t flag);


#ifdef __cplusplus
}
#endif

#endif

/**
  * @}
  */

/**
  * @}
  */

