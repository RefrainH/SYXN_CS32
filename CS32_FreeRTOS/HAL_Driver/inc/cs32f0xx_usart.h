/**
  ******************************************************************************
  * @file    cs32f0xx_usart.h
  * @brief   This file contains all the functions prototypes for the USART 
  *          firmware library.
  * @author  ChipSea MCU Group
  * @version V1.0.1
  * @date   2020.12.17         
  * @copyright CHIPSEA TECHNOLOGIES (SHENZHEN) CORP.
  * @note
  * <h2><center>&copy; COPYRIGHT 2018 ChipSea</center></h2>
  *
  */

#ifndef __CS32F0XX_USART_H__
#define __CS32F0XX_USART_H__


#ifdef __cplusplus
 extern "C" {
#endif

#include "cs32f0xx.h"

/** @addtogroup CS32F0xx_HAL_Driver
  * @{
  */

/** @addtogroup USART
  * @{
  */ 

   
/** 
  * @struct usart_config_t
  * @brief USART Init Structure definition  
  */ 

typedef struct
{
    uint32_t usart_rate;       //!< set the USART baud rate                                          
    uint32_t data_width;       //!< set data width                                          
    uint32_t stop_bits;        //!< set the number of stop bits transmitted                                    
    uint32_t usart_parity;     //!< set the parity mode                                         
    uint32_t usart_mode;       //!< enable/disable Rx/Tx mode               
    uint32_t flow_control;     //!< enable/disable the hardware flow control mode                                                                                 
} usart_config_t;

/** 
  * @struct usart_clock_t
  * @brief USART Clock Init Structure definition
  */ 
typedef struct
{
    uint32_t clock_flag;        //!< enabled or disabled the USART clock                                       
    uint32_t usart_cpol;        //!< set the steady state of the serial clock Polarity                                         
    uint32_t usart_cpha;        //!< set the clock Phase                                   
    uint32_t clock_length;      //!< indicate the clock length corresponding to the last transmitted data bit (MSB)
} usart_clock_t;


/** 
  * @brief Check select USART  definition
  */ 
#define USART_SELECT_CHECK(select) (((select) == USART1) || ((select) == USART2)  || \
                                    ((select) == USART3) || ((select) == USART4)  || \
                                    ((select) == USART5) ||  ((select) == USART6    ) || \
                                    ((select) == USART7) || ((select) == USART8))

/** 
  * @brief Check select USART definition
  */
#define USART_SELECT_123_CHECK(select) (((select) == USART1) || \
                                        ((select) == USART2) || \
                                        ((select) == USART3))
                                     
/** 
  * @brief USART data width definition
  */ 
#define USART_DATA_WIDTH_8                  ((uint32_t)0x00000000)  //!< USART transmit data_width 8 bits 
#define USART_DATA_WIDTH_9                  USART_CTR1_DL           //!< USART transmit data_width 9 bits, should be ((uint32_t)0x00001000)
#define USART_DATA_WIDTH_7                  ((uint32_t)0x10001000)  //!< USART transmit data_width 7 bits
#define USART_DATA_WIDTH_CHECK(length)      (((length) == USART_DATA_WIDTH_8) || \
                                             ((length) == USART_DATA_WIDTH_9) || \
                                             ((length) == USART_DATA_WIDTH_7))
                                     
/** 
  * @brief USART stop bits definition
  */ 
#define USART_STOP_BIT_1                     ((uint32_t)0x00000000)                            //!< USART transmit 1 stop bit                         
#define USART_STOP_BIT_2                     USART_CTR2_STOPLEN_1                              //!< USART transmit 2 stop bits                  
#define USART_STOP_BIT_1_5                   (USART_CTR2_STOPLEN_0 | USART_CTR2_STOPLEN_1)     //!< USART transmit 1.5 stop bits
#define USART_STOP_BIT_CHECK(stopbits) (((stopbits) == USART_STOP_BIT_1) || \
                                        ((stopbits) == USART_STOP_BIT_2) || \
                                        ((stopbits) == USART_STOP_BIT_1_5)) 
                                                                           
/** 
  * @brief USART parity definition
  */ 
#define USART_PARITY_NO               ((uint32_t)0x00000000)                 //!< USART with no parity 
#define USART_PARITY_EVEN             USART_CTR1_PEN                         //!< USART with even parity
#define USART_PARITY_ODD              (USART_CTR1_PEN | USART_CTR1_ODDS)     //!< USART with odd parity 
#define USART_PARITY_CHECK(parity) (((parity) == USART_PARITY_NO) || \
                                    ((parity) == USART_PARITY_EVEN) || \
                                    ((parity) == USART_PARITY_ODD))
                    
/** 
  * @brief USART Rx/Tx mode definition
  */ 
#define USART_MODE_RX              USART_CTR1_REN            //!< USART receiver enable   
#define USART_MODE_TX              USART_CTR1_TEN            //!< USART transmitter enable
#define USART_MODE_CHECK(mode) ((((mode) & (uint32_t)0xFFFFFFF3) == 0x00) && \
                                 ((mode) != (uint32_t)0x00))
                              
/** 
  * @brief USART flow control definition
  */ 
#define USART_FLOW_CONTROL_NONE       ((uint32_t)0x00000000)   //!< USART with no hardware flow control             
#define USART_FLOW_CONTROL_RTS        USART_CTR3_RTSEN         //!< USART RTS (request to send) flow control             
#define USART_FLOW_CONTROL_CTS        USART_CTR3_CTSEN         //!< USART CTS (clear to send) flow control              
#define USART_FLOW_CONTROL_RTS_CTS    (USART_CTR3_RTSEN | USART_CTR3_CTSEN)  //!< USART RTC_CTS flow control
#define USART_FLOW_CONTROL_CHECK(flag)      (((flag) == USART_FLOW_CONTROL_NONE) || \
                                             ((flag) == USART_FLOW_CONTROL_RTS) || \
                                             ((flag) == USART_FLOW_CONTROL_CTS) || \
                                             ((flag) == USART_FLOW_CONTROL_RTS_CTS))
                                                               
/** 
  * @brief USART clock polarity definition
  */  
#define USART_CPOL_LOW                       ((uint32_t)0x00000000)  //!< USART clock polarity output low level when idle 
#define USART_CPOL_HIGH                      USART_CTR2_CPOL         //!< USART clock polarity output high level when idle 
#define USART_CPOL_CHECK(cpol) (((cpol) == USART_CPOL_LOW) || ((cpol) == USART_CPOL_HIGH))

/** 
  * @brief USART clock phase definition
  */
#define USART_CPHA_EDGE_1         ((uint32_t)0x00000000)      //!< USART sampling data at the first edge      
#define USART_CPHA_EDGE_2         USART_CTR2_CPHA             //!< USART sampling data at the second edge 
#define USART_CPHA_CHECK(cpha) (((cpha) == USART_CPHA_EDGE_1) || ((cpha) == USART_CPHA_EDGE_2))


/** 
  * @brief USART CK length definition
  */
#define USART_CK_LENGTH_DISABLE            ((uint32_t)0x00000000)     //!< USART disables CK length
#define USART_CK_LENGTH_ENABLE             USART_CTR2_CKLEN           //!< USART enable CK length 
#define USART_CK_LENGTH_CHECK(flag) (((flag) == USART_CK_LENGTH_DISABLE) || ((flag) == USART_CK_LENGTH_ENABLE))

  
/** 
  * @brief USART Rx/Tx DMA definition
  */
#define USART_DMA_TX                      USART_CTR3_TXDMA     //!< USART transmit with DMA 
#define USART_DMA_RX                      USART_CTR3_RXDMA     //!< USART receive with DMA 
#define USART_DMA_CHECK(flag) ((((flag) & (uint32_t)0xFFFFFF3F) == 0x00) && ((flag) != (uint32_t)0x00))
                                  
/** 
  * @brief USART DMA receive error definition
  */
#define USART_DMA_RX_ERROR_ENABLE            ((uint32_t)0x00000000)  //!< USART enable DMA request mask on reception error
#define USART_DMA_RX_ERROR_DISABLE             USART_CTR3_DRMRE      //!< USART disable DMA request mask on reception error 
#define USART_DMA_RX_ERROR_CHECK(flag) (((flag) == USART_DMA_RX_ERROR_DISABLE)|| ((flag) == USART_DMA_RX_ERROR_ENABLE))

/** 
  * @brief USART wakeup definition
  */
#define USART_RX_WAKEUP_DISABLE                ((uint32_t)0x00000000)  //!< USART disable receiver wakeup selection 
#define USART_RX_WAKEUP_ENABLE                   USART_CTR1_RXWKUPS    //!< USART enable receiver wakeup selection
#define USART_RX_WAKEUP_CHECK(flag) ((((flag)) == USART_RX_WAKEUP_DISABLE) || (((flag)) == USART_RX_WAKEUP_ENABLE))
                                          
/** 
  * @brief USART address length definition
  */ 
#define USART_ADDRESS_4BIT               ((uint32_t)0x00000000)     //!< USART 4 bits address detection selection        
#define USART_ADDRESS_7BIT                USART_CTR2_ADRM7          //!< USART 7 bits address detection selection 
#define USART_ADDRESS_BIT_CHECK(addr) (((addr) == USART_ADDRESS_4BIT) || ((addr) == USART_ADDRESS_7BIT))

/** 
  * @brief USART wakeup methods  definition (only  for cs32F051 and cs32F072 devices ) 
  */ 
#define USART_WAKEUP_METHOD_ADDR_MATCH      ((uint32_t)0x00000000)     //!< USART wakeup methods with address matched                 
#define USART_WAKEUP_METHOD_START_BIT         USART_CTR3_WKUPMTHD_1    //!< USART wakeup methods with start bit 
#define USART_WAKEUP_METHOD_RXNE             (USART_CTR3_WKUPMTHD_0 | USART_CTR3_WKUPMTHD_1)  //!< USART wakeup methods with RXNE 
#define USART_WAKEUP_METHOD_CHECK(flag) (((flag) == USART_WAKEUP_METHOD_ADDR_MATCH) || \
                                             ((flag) == USART_WAKEUP_METHOD_START_BIT) || \
                                             ((flag) == USART_WAKEUP_METHOD_RXNE))

/** 
  * @brief USART link break frame detection 
  */  
#define USART_LIN_BREAK_10BIT       ((uint32_t)0x00000000)  //!< USART 10-bit break from frame detection     
#define USART_LIN_BREAK_11BIT       USART_CTR2_LINBRK11     //!< USART 11-bit break from frame detection    
#define USART_LIN_BREAK_BIT_CHECK(flag) \
                               (((flag) == USART_LIN_BREAK_10BIT) || \
                                ((flag) == USART_LIN_BREAK_11BIT))

/**
  * @brief USART IRDA mode selection 
  */

#define USART_IRDA_MODE_SELECT_LOW              USART_CTR3_IRMS         //!< USART IRDA low power mode selection
#define USART_IRDA_MODE_SELECT_NORMAL           ((uint32_t)0x00000000)  //!< USART IRDA normal mode selection
#define USART_IRDA_MODE_SELECT_CHECK(select) (((select) == USART_IRDA_MODE_SELECT_LOW) || \
                                                   ((select) == USART_IRDA_MODE_SELECT_NORMAL))

/** 
  * @brief USART driver polarity selection
  */

#define USART_DE_POLARITY_HIGH                ((uint32_t)0x00000000)  //!< USART driver polarity with high level
#define USART_DE_POLARITY_LOW                   USART_CTR3_DEPS       //!< USART driver polarity with low level 
#define USART_DE_POLARITY_CHECK(flag) (((flag) == USART_DE_POLARITY_LOW) || \
                                        ((flag) == USART_DE_POLARITY_HIGH))

/** 
  * @brief USART TX/RX pin inversion
  */
#define USART_INVERSION_PIN_TX            USART_CTR2_TXINV     //!< USART TX pin inversion 
#define USART_INVERSION_PIN_RX            USART_CTR2_RXINV     //!< USART RX pin inversion 
#define USART_INVERSION_PIN_CHECK(pin) ((((pin) & (uint32_t)0xFFFCFFFF) == 0x00) && ((pin) != (uint32_t)0x00))
                                       
/**  
  * @brief USART auto baud rate mode selection 
  */
#define USART_AUTO_RATE_MEASURE_START_BIT          ((uint32_t)0x00000000)
#define USART_AUTO_RATE_MEASURE_FALLING_EDGE       USART_CTR2_ABRSEL_0
#define USART_AUTO_RATE_MEASURE_CHECK(flag) (((flag) == USART_AUTO_RATE_MEASURE_START_BIT) || \
                                                  ((flag) == USART_AUTO_RATE_MEASURE_FALLING_EDGE))

/** 
  * @brief USART receive overflow detection 
  */
#define USART_RX_OVERFLOW_DETECT_ENABLE            ((uint32_t)0x00000000)  //!< USART enable receive overflow detection
#define USART_RX_OVERFLOW_DETECT_DISABLE           USART_CTR3_NORXOF       //!< USART diable receive overflow detection 
#define USART_RX_OVERFLOW_DETECT_CHECK(flag) (((flag) == USART_RX_OVERFLOW_DETECT_ENABLE)|| \
                                                  ((flag) == USART_RX_OVERFLOW_DETECT_DISABLE))

/** 
  * @brief USART trigger operate 
  */
#define USART_TRIGGER_OPERATE_ABRT          USART_SWTR_ABRT      //!< USART auto baud rate trigger 
#define USART_TRIGGER_OPERATE_BRKFST        USART_SWTR_BRKFST    //!< USART break frame send trigger
#define USART_TRIGGER_OPERATE_RXMSKT        USART_SWTR_RXMSKT    //!< USART reception mask mode trigger 
#define USART_TRIGGER_OPERATE_RXNECLR       USART_SWTR_RXNECLR   //!< USART RXNE (receive buffer not empty flag) clear trigger
#define USART_TRIGGER_OPERATE_TXESET        USART_SWTR_TXESET    //!< USART TXE (transmit buffer empty flag) set trigger 

#define USART_TRIGGER_OPERATE_CHECK(flag) (((flag) == USART_TRIGGER_OPERATE_TXESET) || \
                                   ((flag) == USART_TRIGGER_OPERATE_RXNECLR) || \
                                   ((flag) == USART_TRIGGER_OPERATE_RXMSKT) || \
                                   ((flag) == USART_TRIGGER_OPERATE_BRKFST) || \
                                   ((flag) == USART_TRIGGER_OPERATE_ABRT))
 


/** 
  * @brief USART check baud rate 
  */
#define USART_BAUD_RATE_CHECK(rate) (((rate) > 0) && ((rate) < 0x005B8D81))

/** 
  * @brief USART driver polarity start stable time
  */
#define USART_DE_START_STABLE_TIME_CHECK(counter) (((counter)) <= 0x1F)

/** 
  * @brief USART check retry num
  */
#define USART_RETRY_NUM_CHECK(counter) ((counter) <= 0x7)

/** 
  * @brief USART check time_out
  */
#define USART_TIMEOUT_CHECK(counter) ((counter) <= 0x00FFFFFF)

/** 
  * @brief USART check data value
  */
#define USART_DATA_CHECK(value) ((value) <= 0x1FF)


/** 
  * @brief usart status flag 
  */
#define USART_FLAG_RENACTF             USART_STS_RENACTF  //!< USART REN (receiver enable) active flag
#define USART_FLAG_TENACTF             USART_STS_TENACTF  //!< USART TEN (transmitter enable) active flag
#define USART_FLAG_WKUPF               USART_STS_WKUPF    //!< USART wakeup from stop mode flag (not available for CS32F030, CS32F036 devices)
#define USART_FLAG_RXMSKF              USART_STS_RXMSKF   //!< USART reception mask mode flag (not available for CS32F030, CS32F036 devices)
#define USART_FLAG_BRKSF               USART_STS_BRKSF    //!< USART break frame send flag
#define USART_FLAG_CMF                 USART_STS_CMF      //!< USART character match flag 
#define USART_FLAG_BSYF                USART_STS_BSYF     //!< USART busy flag 
#define USART_FLAG_ABRTF               USART_STS_ABRTF    //!< USART auto baud rate flag 
#define USART_FLAG_ABRTERRF            USART_STS_ABRTERRF //!< USART auto baud rate error flag 
#define USART_FLAG_ENDBLKF             USART_STS_ENDBLKF  //!< USART end of block flag (not available for CS32F030, CS32F036 devices)
#define USART_FLAG_ROTF                USART_STS_ROTF     //!< USART receiver overtime flag
#define USART_FLAG_CTSF                USART_STS_CTSF     //!< USART CTS (clear to send) flag 
#define USART_FLAG_CTSIF               USART_STS_CTSIF    //!< USART CTS (clear to send) interrupt flag 
#define USART_FLAG_LINBKF              USART_STS_LINBKF   //!< USART link break frame detection flag (not available for CS32F030, CS32F036 devices)
#define USART_FLAG_TXE                 USART_STS_TXE      //!< USART tranmsit buffer empty flag 
#define USART_FLAG_TCF                 USART_STS_TCF      //!< USART transmission complete flag 
#define USART_FLAG_RXNE                USART_STS_RXNE     //!< USART receiver buffer not empty flag 
#define USART_FLAG_IDLE                USART_STS_IDLEF    //!< USART idle frame detection flag 
#define USART_FLAG_OVRERRF             USART_STS_OVRERRF  //!< USART reception overflow error flag 
#define USART_FLAG_NF                  USART_STS_NF       //!< USART start bit noise flag 
#define USART_FLAG_FERRF               USART_STS_FERRF    //!< USART frame error flag 
#define USART_FLAG_PERRF               USART_STS_PERRF    //!< USART parity error flag 
                                          
#define USART_FLAG_CHECK(flag) (((flag) == USART_FLAG_PERRF) || ((flag) == USART_FLAG_TXE) || \
                                   ((flag) == USART_FLAG_TCF) || ((flag) == USART_FLAG_RXNE) || \
                                   ((flag) == USART_FLAG_IDLE) || ((flag) == USART_FLAG_LINBKF) || \
                                   ((flag) == USART_FLAG_CTSIF) || ((flag) == USART_FLAG_OVRERRF) || \
                                   ((flag) == USART_FLAG_NF) || ((flag) == USART_FLAG_FERRF) || \
                                   ((flag) == USART_FLAG_CTSF) || ((flag) == USART_FLAG_ROTF) || \
                                   ((flag) == USART_FLAG_ENDBLKF) || ((flag) == USART_FLAG_ABRTERRF) || \
                                   ((flag) == USART_FLAG_ABRTF) || ((flag) == USART_FLAG_BSYF) || \
                                   ((flag) == USART_FLAG_CMF) || ((flag) == USART_FLAG_BRKSF) || \
                                   ((flag) == USART_FLAG_RXMSKF) || ((flag) == USART_FLAG_WKUPF) || \
                                   ((flag) == USART_FLAG_TENACTF)|| ((flag) == USART_FLAG_RENACTF))

#define USART_CLEAR_FLAG_CHECK(flag) (((flag) == USART_FLAG_WKUPF) || ((flag) == USART_FLAG_TCF) || \
                                          ((flag) == USART_FLAG_IDLE) || ((flag) == USART_FLAG_OVRERRF) || \
                                          ((flag) == USART_FLAG_NF) || ((flag) == USART_FLAG_FERRF) || \
                                          ((flag) == USART_FLAG_LINBKF) || ((flag) == USART_FLAG_CTSIF) || \
                                          ((flag) == USART_FLAG_ROTF) || ((flag) == USART_FLAG_ENDBLKF) || \
                                          ((flag) == USART_FLAG_CMF) || ((flag) == USART_FLAG_PERRF))


/** @defgroup USART_Interrupt_definition 
  * @brief USART Interrupt definition
  * @{
  */
#define USART_INT_WKUP                ((uint32_t)0x00140316) //!< USART wakeup from stop mode (not available for CS32F030 devices)
#define USART_INT_CM                  ((uint32_t)0x0011010E) //!< USART character match 
#define USART_INT_ENDBLK              ((uint32_t)0x000C011B) //!< USART end of block (not available for CS32F030, CS32F036 devices)
#define USART_INT_ROT                 ((uint32_t)0x000B011A) //!< USART receiver overtime 
#define USART_INT_PERR                ((uint32_t)0x00000108) //!< USART parity error 
#define USART_INT_TXE                 ((uint32_t)0x00070107) //!< USART transmit empty 
#define USART_INT_TC                  ((uint32_t)0x00060106) //!< USART tranmsit complete 
#define USART_INT_RXNE                ((uint32_t)0x00050105) //!< USART receiver buffer not empty 
#define USART_INT_LINBK               ((uint32_t)0x00080206) //!< USART link break frame (not available for CS32F030, CS32F036 devices)
#define USART_INT_CTS                 ((uint32_t)0x0009030A) //!< USART clear to send 
#define USART_INT_ERR                 ((uint32_t)0x00000300) //!< USART Error interrupt
#define USART_INT_OVRERR              ((uint32_t)0x00030300) //!< USART overrun error
#define USART_INT_NE                  ((uint32_t)0x00020300) //!< USART 
#define USART_INT_FERR                ((uint32_t)0x00010300) //!< USART frame error

#define USART_INT_CONFIG_CHECK(config) (((config) == USART_INT_PERR) || ((config) == USART_INT_TXE) || \
                                ((config) == USART_INT_TC) || ((config) == USART_INT_RXNE) || \
                                ((config) == USART_INT_LINBK) || ((config) == USART_INT_CTS) || \
                                ((config) == USART_INT_ERR) || ((config) == USART_INT_ROT) || \
                                ((config) == USART_INT_ENDBLK) || ((config) == USART_INT_CM)|| \
                                ((config) == USART_INT_WKUP))

#define USART_GET_INT_CHECK(flag) (((flag) == USART_INT_PERR) || ((flag) == USART_INT_TXE) || \
                             ((flag) == USART_INT_TC) || ((flag) == USART_INT_RXNE) || \
                             ((flag) == USART_INT_LINBK) || ((flag) == USART_INT_CTS) || \
                             ((flag) == USART_INT_OVRERR) || ((flag) == USART_INT_NE) || \
                             ((flag) == USART_INT_FERR) || ((flag) == USART_INT_ROT) || \
                             ((flag) == USART_INT_ENDBLK) || ((flag) == USART_INT_CM) || \
                             ((flag) == USART_INT_WKUP))

#define USART_CLEAR_INT_CHECK(flag) (((flag) == USART_INT_TC) || ((flag) == USART_INT_PERR) || \
                               ((flag) == USART_INT_FERR) || ((flag) == USART_INT_NE) || \
                               ((flag) == USART_INT_OVRERR) ||  ((flag) == USART_INT_LINBK) || \
                               ((flag) == USART_INT_CTS) || ((flag) == USART_INT_ROT) || \
                               ((flag) == USART_INT_ENDBLK) || ((flag) == USART_INT_CM) || \
                               ((flag) == USART_INT_WKUP))

/**
  * @}
  */
  


/**
  * @fn void usart_def_init(usart_reg_t* ptr_usart)
  * @brief  Initializes registers  of the ptr_usart to default values.
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices.   
  * @return None
  */
void usart_def_init(usart_reg_t* ptr_usart);

/**
  * @fn void usart_init(usart_reg_t* ptr_usart, usart_config_t* ptr_usart_config)
  * @brief  Initializes the ptr_usart peripheral according to the specified parameters
  *         in the ptr_usart_config .
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices. 
  * @param  ptr_usart_config: pointer to a usart_config_t structure containing
  *         the configuration for the specified USART peripheral.
  * @return None
  */
void usart_init(usart_reg_t* ptr_usart, usart_config_t* ptr_usart_config);

/**
  * @fn void usart_config_struct_init(usart_config_t* ptr_usart_config)
  * @brief  Fills each ptr_usart_config member with its default value.
  * @param  ptr_usart_config: pointer to a usart_config_t structure to be initialized.
  * @return None
  */
void usart_config_struct_init(usart_config_t* ptr_usart_config);

/**
  * @fn void usart_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag)
  * @brief  Enables or disables the specified USART peripheral.
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices.  
  * @param  enable_flag: It can be: ENABLE or DISABLE.
  * @return None
  */
void usart_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag);

/**
  * @fn void usart_direction_enable_ctrl(usart_reg_t* ptr_usart, uint32_t direction, enable_state_t enable_flag)
  * @brief  Enables or disables the USART's transmitter or receiver.
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices. 
  * @param  direction: specifies the USART direction,it can be any of the following values:
  *          @arg USART_MODE_TX(USART Transmitter)
  *          @arg USART_MODE_RX(USART Receiver)
  * @param  enable_flag: ENABLE or DISABLE the USART transfer direction.
  * @return None
  */
void usart_direction_enable_ctrl(usart_reg_t* ptr_usart, uint32_t direction, enable_state_t enable_flag);

/**
  * @fn void usart_prescaler_set(usart_reg_t* ptr_usart, uint8_t value)
  * @brief  Sets the system clock prescaler.
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices. 
  * @param  value: specifies the prescaler clock.
  * @note   This function has to be called before calling usart_enable_ctrl() function.    
  * @return None
  */
void usart_prescaler_set(usart_reg_t* ptr_usart, uint8_t value);

/**
  * @fn void usart_oversampling_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag)
  * @brief  Enables or disables the USART's oversampling mode.
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices. 
  * @param  enable_flag: ENABLE or DISABLE the USART oversampling mode.
  * @return None
  */
void usart_oversampling_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag);

/**
  * @fn void usart_sample_method_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag)
  * @brief  Enables or disables the USART's one bit sampling method.
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices.  
  * @param  enable_flag: ENABLE or DISABLE the USART one bit sampling method.
  * @note   This function has to be called before calling usart_enable_ctrl() function.  
  * @return None
  */
void usart_sample_method_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag);

/**
  * @fn void usart_msb_first_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag)
  * @brief  Enables or disables the USART's most significant bit first 
  *         transmitted/received following the start bit.
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices. 
  * @param  enable_flag: ENABLE or DISABLE the USART most significant bit first
  *         transmitted/received following the start bit.
  * @return None
  */
void usart_msb_first_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag);

/**
  * @fn void usart_data_reverse_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag)
  * @brief  Enables or disables the binary data inversion.
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices.  
  * @param  enable_flag: ENABLE or DISABLE new defined levels for the USART data.
  * @return None
  */
void usart_data_reverse_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag);

/**
  * @fn void usart_pin_reverse_enable_ctrl(usart_reg_t* ptr_usart, uint32_t usart_invpin, enable_state_t enable_flag)
  * @brief  Enables or disables the Pin(s) active level inversion.
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices.  
  * @param  usart_invpin: specifies the USART pin(s) to invert.
  *          This parameter can be any combination of the following values:
  *           @arg USART_INVERSION_PIN_TX(USART Tx pin active level inversion)
  *           @arg USART_INVERSION_PIN_RX(USART Rx pin active level inversion)
  * @param  enable_flag: ENABLE or DISABLE new active level status for the USART pin(s).
  * @note   It has to be called before calling usart_enable_ctrl() function.  
  * @return None
  */
void usart_pin_reverse_enable_ctrl(usart_reg_t* ptr_usart, uint32_t usart_invpin, enable_state_t enable_flag);

/**
  * @fn void usart_pin_swap_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag)
  * @brief  Enables or disables the swap Tx/Rx pins.
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices.  
  * @param  enable_flag: ENABLE or DISABLE the USARTx TX/RX pins pinout.
  * @note   This function has to be called before calling usart_enable_ctrl() function.  
  * @return None
  */
void usart_pin_swap_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag);

/**
  * @fn void usart_recv_overtime_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag)
  * @brief  Enables or disables the receiver Time Out feature.
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices. 
  * @param  enable_flag: ENABLE or DISABLE the ptr_usart receiver Time Out.
  * @return None
  */
void usart_recv_overtime_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag);

/**
  * @fn void usart_recv_overtime_set(usart_reg_t* ptr_usart, uint32_t value)
  * @brief  Sets the receiver Time Out value.
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices.  
  * @param  value: specifies the Receiver Time Out value.
  * @return None
  */
void usart_recv_overtime_set(usart_reg_t* ptr_usart, uint32_t value);

/**
  * @fn void usart_stop_mode_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag)
  * @brief  Enables or disables the specified USART peripheral in STOP Mode.
  * @param  ptr_usart: pointer to USARTx where x can be 1 to select USART peripheral.
  * @param  enable_flag: ENABLE or DISABLE the ptr_usart peripheral state in stop mode.
  * @return None
  */
void usart_stop_mode_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag);

/**
  * @fn void usart_stop_mode_wakeup_config(usart_reg_t* ptr_usart, uint32_t usart_wakeupsource)
  * @brief  Selects the USART WakeUp method form stop mode.
  * @note   This function is not available for CS32F030 devices.   
  * @param  ptr_usart: pointer to USARTx where x can be 1 to select USART peripheral.
  * @param  usart_wakeupsource: specifies the selected USART wakeup method.
  * @note   This function has to be called before calling usart_enable_ctrl() function.   
  * @return None
  */
void usart_stop_mode_wakeup_config(usart_reg_t* ptr_usart, uint32_t usart_wakeupsource);

/**
  * @fn void usart_auto_baud_rate_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag)
  * @brief  Enables or disables the Auto Baud Rate.
  * @param  ptr_usart: pointer to USARTx where x can be 1 to select USART peripheral.  
  * @param  enable_flag: ENABLE or DISABLE the USARTx auto baud rate.
  * @return None
  */
void usart_auto_baud_rate_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag);

/**
  * @fn void usart_auto_baud_rate_config(usart_reg_t* ptr_usart, uint32_t rate_value)
  * @brief  Selects the USART auto baud rate method.
  * @param  ptr_usart: pointer to USARTx where x can be 1 to select USART peripheral.
  * @param  rate_value: specifies the selected USART auto baud rate method.
  *          This parameter can be one of the following values:
  *            @arg USART_AUTO_RATE_MEASURE_START_BIT(Start Bit duration measurement)
  *            @arg USART_AUTO_RATE_MEASURE_FALLING_EDGE(Falling edge to falling edge measurement)
  * @note   This function has to be called before calling usart_enable_ctrl() function.  
  * @return None
  */
void usart_auto_baud_rate_config(usart_reg_t* ptr_usart, uint32_t rate_value);

/* Data transfers functions ***************************************************/

/**
  * @fn void usart_data_send(usart_reg_t* ptr_usart, uint16_t value)
  * @brief  Transmits single data through the ptr_usart peripheral.
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices.   
  * @param  value: the data to transmit.
  * @return None
  */
void usart_data_send(usart_reg_t* ptr_usart, uint16_t value);

/**
  * @fn uint16_t usart_data_recv(usart_reg_t* ptr_usart)
  * @brief  Returns the most recent received data by the ptr_usart peripheral.
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices.   
  * @return The received data.
  */
uint16_t usart_data_recv(usart_reg_t* ptr_usart);

/* Multi-Processor Communication functions ************************************/

/**
  * @fn void usart_address_set(usart_reg_t* ptr_usart, uint8_t addr)
  * @brief  Sets the address of the USART node.
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices.   
  * @param  addr: Indicates the address of the USART node.
  * @return None
  */
void usart_address_set(usart_reg_t* ptr_usart, uint8_t addr);

/**
  * @fn void usart_recv_mask_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag)
  * @brief  Enables or disables the USART's reception mask mode.
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices.   
  * @param  enable_flag: It can be: ENABLE or DISABLE.
  * @return None
  */
void usart_recv_mask_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag);

/**
  * @fn void usart_recv_wakeup_config(usart_reg_t* ptr_usart, uint32_t flag)
  * @brief  Selects the USART WakeUp method from mute mode.
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices.   
  * @param  flag: specifies the USART wakeup method.
  *          This parameter can be one of the following values:
  *            @arg USART_RX_WAKEUP_DISABLE(WakeUp by an idle line detection)
  *            @arg USART_RX_WAKEUP_ENABLE(WakeUp by an address mark)
  * @return None
  */
void usart_recv_wakeup_config(usart_reg_t* ptr_usart, uint32_t flag);

/**
  * @fn void usart_addr_detect_set(usart_reg_t* ptr_usart, uint32_t addr)
  * @brief  Configure the the USART Address detection length.
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices.  
  * @param  addr: specifies the USART address length detection.
  *          This parameter can be one of the following values:
  *            @arg USART_ADDRESS_4BIT(4-bit address length detection) 
  *            @arg USART_ADDRESS_7BIT(7-bit address length detection) 
  * @return None
  */
void usart_addr_detect_set(usart_reg_t* ptr_usart, uint32_t addr);

/* LIN mode functions *********************************************************/

/**
  * @fn void usart_11bit_break_detect_set(usart_reg_t* ptr_usart, uint32_t value)
  * @brief  Sets the USART LIN Break detection length.
  * @note   This function is not available for CS32F030 devices. 
  * @note   This function is not available for CS32F036 devices.  
  * @param  ptr_usart: pointer to USARTx where x can be 1 to select USART peripheral. 
  * @param  value: specifies the LIN break detection length.
  *          This parameter can be one of the following values:
  *            @arg USART_LIN_BREAK_10BIT(10-bit break detection)
  *            @arg USART_LIN_BREAK_11BIT(11-bit break detection)
  * @return None
  */
void usart_11bit_break_detect_set(usart_reg_t* ptr_usart, uint32_t value);

/**
  * @fn void usart_lin_mode_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag)
  * @brief  Enables or disables the USART's LIN mode.
  * @note   This function is not available for CS32F030 devices.  
  * @note   This function is not available for CS32F036 devices.  
  * @param  ptr_usart: pointer to USARTx where x can be 1 to select USART peripheral. 
  * @param  enable_flag: ENABLE or DISABLE the USART LIN mode.
  * @return None
  */
void usart_lin_mode_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag);

/**
  * @fn void usart_half_duplex_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag)
  * @brief  Enables or disables the USART's Half Duplex communication.
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices.  
  * @param  enable_flag: ENABLE or DISABLE the USART Communication.
  * @return None
  */
void usart_half_duplex_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag);

/**
  * @fn void usart_smartcard_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag)
  * @brief  Enables or disables the USART's SmartCard mode.
  * @note   This function is not available for CS32F030 devices.  
  * @note   This function is not available for CS32F036 devices.    
  * @param  ptr_usart: pointer to USARTx where x can be 1 to select USART peripheral.
  * @param  enable_flag: ENABLE or DISABLE the Smart Card mode.
  * @return None
  */
void usart_smartcard_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag);

/**
  * @fn void usart_smartcard_nack_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag)
  * @brief  Enables or disables NACK transmission.
  * @note   This function is not available for CS32F030 devices.  
  * @note   This function is not available for CS32F036 devices.   
  * @param  ptr_usart: pointer to USARTx where x can be 1 to select USART peripheral.  
  * @param  enable_flag: ENABLE or DISABLE the NACK transmission.
  * @return None
  */
void usart_smartcard_nack_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag);

/**
  * @fn void usart_guard_time_set(usart_reg_t* ptr_usart, uint8_t value)
  * @brief  Sets the specified USART guard time.
  * @note   This function is not available for CS32F030 devices.  
  * @note   This function is not available for CS32F036 devices.   
  * @param  ptr_usart: pointer to USARTx where x can be 1 to select USART peripheral. 
  * @param  value: specifies the guard time.
  * @return None
  */
void usart_guard_time_set(usart_reg_t* ptr_usart, uint8_t value);

/**
  * @fn void usart_auto_retry_times_set(usart_reg_t* ptr_usart, uint8_t value)
  * @brief  Sets the Smart Card number of retries in transmit and receive.
  * @note   This function is not available for CS32F030 devices. 
  * @note   This function is not available for CS32F036 devices.   
  * @param  ptr_usart: pointer to USARTx where x can be 1 to select USART peripheral. 
  * @param  value: specifies the Smart Card auto retry times.
  * @return None
  */
void usart_auto_retry_times_set(usart_reg_t* ptr_usart, uint8_t value);

/**
  * @fn void usart_block_value_set(usart_reg_t* ptr_usart, uint8_t block_number)
  * @brief  Sets the Smar Card Block number.
  * @note   This function is not available for CS32F030 devices.  
  * @note   This function is not available for CS32F036 devices.   
  * @param  ptr_usart: pointer to USARTx where x can be 1 to select USART peripheral.  
  * @param  block_number: specifies the Smart Card block number.
  * @return None
  */
void usart_block_value_set(usart_reg_t* ptr_usart, uint8_t block_number);

/**
  * @fn void usart_irda_config(usart_reg_t* ptr_usart, uint32_t value)
  * @brief  Configures the USART's IrDA interface.
  * @note   This function is not available for CS32F030 devices.  
  * @note   This function is not available for CS32F036 devices.   
  * @param  ptr_usart: pointer to USARTx where x can be 1 to select USART peripheral.
  * @param  value: specifies the IrDA mode,it can be one of the following values:
  *          @arg USART_IRDA_MODE_SELECT_LOW 
  *          @arg USART_IRDA_MODE_SELECT_NORMAL.
  * @return None
  */
void usart_irda_config(usart_reg_t* ptr_usart, uint32_t value);

/**
  * @fn void usart_irda_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag)
  * @brief  Enables or disables the USART's IrDA interface.
  * @note   This function is not available for CS32F030 devices.  
  * @note   This function is not available for CS32F036 devices.   
  * @param  ptr_usart: pointer to USARTx where x can be 1 to select USART peripheral.
  * @param  enable_flag: ENABLE or DISABLE the IrDA mode.
  * @return None
  */
void usart_irda_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag);

/**
  * @fn void usart_driver_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag)
  * @brief  Enables or disables the USART's DE functionality.
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices.  
  * @param  enable_flag: ENABLE or DISABLE the driver enable mode.
  * @return None
  */
void usart_driver_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag);

/**
  * @fn void usart_driver_enable_polarity_config(usart_reg_t* ptr_usart, uint32_t value)
  * @brief  Configures the USART's DE polarity
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices.  
  * @param  value: specifies the DE polarity, it can be one of the following values:
  *          @arg USART_DE_POLARITY_LOW 
  *          @arg USART_DE_POLARITY_HIGH
  * @return None
  */
void usart_driver_enable_polarity_config(usart_reg_t* ptr_usart, uint32_t value);

/**
  * @fn void usart_driver_enable_preactive_time_set(usart_reg_t* ptr_usart, uint32_t value)
  * @brief  Sets the specified RS485 DE Pre-active time 
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices.  
  * @param  value: specifies DE Pre-active time between the activation of the DE
  *         signal and the the beginning of the start bit
  * @return None
  */
void usart_driver_enable_preactive_time_set(usart_reg_t* ptr_usart, uint32_t value);

/**
  * @fn void usart_driver_enable_posactive_time_set(usart_reg_t* ptr_usart, uint32_t value)
  * @brief  Sets the specified RS485 DE Pos-active time
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices.  
  * @param  value: specifies the time between the middle of the last stop bit 
  *         in a transmitted message and the Pos_activation of the DE signal .
  * @return None
  */
void usart_driver_enable_posactive_time_set(usart_reg_t* ptr_usart, uint32_t value);

/**
  * @fn void usart_dma_enable_ctrl(usart_reg_t* ptr_usart, uint32_t direction, enable_state_t enable_flag)
  * @brief  Enables or disables the USART's DMA interface.
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2 to select USART peripheral.
  * @param  direction: It can be any combination of the following values:
  *          @arg USART_DMA_TX(USART DMA transmit)
  *          @arg USART_DMA_RX(USART DMA receive)
  * @param  enable_flag: ENABLE or DISABLE the DMA Request sources.
  * @return None
  */
void usart_dma_enable_ctrl(usart_reg_t* ptr_usart, uint32_t direction, enable_state_t enable_flag);

/**
  * @fn void usart_dma_recv_error_config(usart_reg_t* ptr_usart, uint32_t value)
  * @brief  Enables or disables the USART's DMA interface when reception error occurs.
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2 to select USART peripheral.
  * @param  value: specifies the DMA status in case of reception error.
  *          It can be any combination of the following values:
  *           @arg USART_DMA_RX_ERROR_ENABLE 
  *           @arg USART_DMA_RX_ERROR_DISABLE
  * @return None
  */
void usart_dma_recv_error_config(usart_reg_t* ptr_usart, uint32_t value);

/**
  * @fn void usart_interrupt_config(usart_reg_t* ptr_usart, uint32_t set_value, enable_state_t enable_flag)
  * @brief  Enables or disables the specified USART interrupts.
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices.  
  * @param  set_value: specifies the USART interrupt sources to be enabled or disabled.
  *          It can be one of the following values:
  *           @arg USART_INT_CM(Character match interrupt)
  *           @arg USART_INT_ROT(Receive overtime interrupt)
  *           @arg USART_INT_CTS(CTS interrupt)
  *           @arg USART_INT_TC(Transmission complete interrupt)
  *           @arg USART_INT_TXE(USART transmit empty)
  *           @arg USART_INT_RXNE(USART receiver buffer not empty)
  *           @arg USART_INT_ERR(USART error interrupt )
  *           @arg USART_INT_PERR(Parity Error interrupt)
  *           @arg USART_INT_LINBK(LIN Break detection interrupt, not for CS32F030, CS32F036 devices)
  *           @arg USART_INT_ENDBLK(End of block interrupt, not for CS32F030, CS32F036 devices)
  *           @arg USART_INT_WKUP(Wakeup interrupt, not  for CS32F030 devices)
  * @param  enable_flag: ENABLE or DISABLE the specified USARTx interrupts.
  * @return None
  */
void usart_interrupt_config(usart_reg_t* ptr_usart, uint32_t set_value, enable_state_t enable_flag);

/**
  * @fn void usart_trigger_enable_ctrl(usart_reg_t* ptr_usart, uint32_t set_value, enable_state_t enable_flag)
  * @brief  Enables the specified USART's trigger.
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices.  
  * @param  set_value: specifies the USART trigger.
  *          It can be any combination of the following values:
  *           @arg USART_TRIGGER_OPERATE_TXESET(Transmit data trigger);
  *           @arg USART_TRIGGER_OPERATE_RXNECLR(Receive data clear trigger); 
  *           @arg USART_TRIGGER_OPERATE_RXMSKT(Reception mask mode trigger); 
  *           @arg USART_TRIGGER_OPERATE_BRKFST(Break frame send trigger);
  *           @arg USART_TRIGGER_OPERATE_ABRT(Auto baud rate trigger).
  * @param  enable_flag: ENABLE or DISABLE the DMA interface when reception error occurs.
  * @return None
  */
void usart_trigger_enable_ctrl(usart_reg_t* ptr_usart, uint32_t set_value, enable_state_t enable_flag);

/**
  * @fn void usart_recveive_overflow_config(usart_reg_t* ptr_usart, uint32_t set_value)
  * @brief   Enables or disables the USART's overflow detection.
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices.  
  * @param   set_value: specifies the OVR detection status in case of NORXOF error.
  *          It can be any combination of the following values:
  *           @arg USART_RX_OVERFLOW_DETECT_ENABLE(NORXOF error detection enabled); 
  *           @arg USART_RX_OVERFLOW_DETECT_DISABLE(NORXOF error detection disabled).
  * @return None
  */
void usart_recveive_overflow_config(usart_reg_t* ptr_usart, uint32_t set_value);

/**
  * @fn bit_status_t usart_flag_status_get(usart_reg_t* ptr_usart, uint32_t flag)
  * @brief  Checks whether the specified USART flag is set or not.
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices.  
  * @param  flag: specifies the flag to check, it can be one of the following values:
  *          @arg USART_FLAG_RENACTF(Receive Enable acknowledge flag);
  *          @arg USART_FLAG_TENACTF(Transmit Enable acknowledge flag);
  *          @arg USART_FLAG_BRKSF(Send Break flag)  ;
  *          @arg USART_FLAG_CMF(Character match flag)  ;
  *          @arg USART_FLAG_BSYF(Busy flag);
  *          @arg USART_FLAG_ABRTF(Auto baud rate flag)  ;
  *          @arg USART_FLAG_ABRTERRF(Auto baud rate error flag);
  *          @arg USART_FLAG_ROTF(Receive overtime flag)  ;
  *          @arg USART_FLAG_CTSF(CTS flag);
  *          @arg USART_FLAG_CTSIF(CTS interrupt flag);
  *          @arg USART_FLAG_TXE (Transmit data register empty flag);
  *          @arg USART_FLAG_TCF(Transmission Complete flag);
  *          @arg USART_FLAG_RXNE(Receive data register not empty flag);
  *          @arg USART_FLAG_IDLE(Idle frame detection flag);
  *          @arg USART_FLAG_OVRERRF(Overflow Error flag);
  *          @arg USART_FLAG_NF(Noise flag);
  *          @arg USART_FLAG_FERRF(Frame Error flag);
  *          @arg USART_FLAG_PERRF(Parity Error flag);
  *          @arg USART_FLAG_LINBKF(LIN Break detection flag, only for USART1);
  *          @arg USART_FLAG_ENDBLKF(End of block flag);
  *          @arg USART_FLAG_RXMSKF(Reception mask mode flag);
  *          @arg USART_FLAG_WKUPF(Wake up flag, only for USART1).
  * @return The new state of flag (SET or RESET).
  */
bit_status_t usart_flag_status_get(usart_reg_t* ptr_usart, uint32_t flag);

/**
  * @fn void usart_flag_clear(usart_reg_t* ptr_usart, uint32_t flag)
  * @brief  Clears the USARTx's pending flags.
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices.  
  * @param  flag: specifies the flag to clear,it can be any combination of the following values:
  *          @arg USART_FLAG_CMF(Character match flag);
  *          @arg USART_FLAG_ROTF(Receive overtime flag);
  *          @arg USART_FLAG_CTSIF(CTS interrupt flag);
  *          @arg USART_FLAG_TCF(Transmission Complete flag);
  *          @arg USART_FLAG_IDLE(Idle frame detection flag);
  *          @arg USART_FLAG_OVRERRF(Overflow Error flag);
  *          @arg USART_FLAG_NF(Noise flag);
  *          @arg USART_FLAG_FERRF(Frame Error flag);
  *          @arg USART_FLAG_PERRF(Parity Error flag);
  *          @arg USART_FLAG_LINBKF(LIN Break detection flag, only for USART1);  
  *          @arg USART_FLAG_ENDBLKF(End of block flag);
  *          @arg USART_FLAG_WKUPF(Wake up flag, only for USART1).
  * @return None
  */
void usart_flag_clear(usart_reg_t* ptr_usart, uint32_t flag);

/**
  * @fn  bit_status_t usart_interrupt_status_get(usart_reg_t* ptr_usart, uint32_t flag)
  * @brief  Checks whether the specified USART interrupt has occurred or not.
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices.  
  * @param  flag: specifies the USART interrupt source to check, it can be one of the following values:
  *           @arg USART_INT_CM(Character match interrupt)  ;
  *           @arg USART_INT_ROT(Receive overtime interrupt)  ;
  *           @arg USART_INT_CTS(CTS interrupt);
  *           @arg USART_INT_TXE(Transmit data register empty interrupt);
  *           @arg USART_INT_TC(Transmission complete interrupt);
  *           @arg USART_INT_RXNE(Receive data register not empty interrupt);
  *           @arg USART_INT_OVRERR(Overflow Error interrupt)  ;
  *           @arg USART_INT_NE(Noise Error interrupt);
  *           @arg USART_INT_FERR(Frame Error interrupt);
  *           @arg USART_INT_PERR(Parity Error interrupt);
  *           @arg USART_INT_LINBK(LIN Break detection interrupt, only for USART1);
  *           @arg USART_INT_ENDBLK(End of block interrupt);
  *           @arg USART_INT_WKUP(Wakeup interrupt, only for USART1).
  * @return The state of flag (SET or RESET).
  */
bit_status_t usart_interrupt_status_get(usart_reg_t* ptr_usart, uint32_t flag);

/**
  * @fn void usart_interrupt_bit_clear(usart_reg_t* ptr_usart, uint32_t flag)
  * @brief  Clears the ptr_usart's interrupt pending bits.
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices.  
  * @param   flag: specifies the interrupt pending bit to clear, it can be one of the following values:
  *           @arg USART_INT_CM(Character match interrupt)  ;
  *           @arg USART_INT_TXE(USART transmit empty)  ;
  *           @arg USART_INT_RXNE(USART receiver buffer not empty)  ;
  *           @arg USART_INT_ROT(Receive overtime interrupt)  ;
  *           @arg USART_INT_CTS(CTS interrupt);
  *           @arg USART_INT_TC(Transmission complete interrupt);
  *           @arg USART_INT_OVRERR(Overflow Error interrupt)  ;
  *           @arg USART_INT_NE(Noise Error interrupt);
  *           @arg USART_INT_FERR(Frame Error interrupt);
  *           @arg USART_INT_PERR(Parity Error interrupt);
  *           @arg USART_INT_LINBK(LIN Break detection interrupt, only for USART1);
  *           @arg USART_INT_ENDBLK(End of block interrupt);
  *           @arg USART_INT_WKUP(Wakeup interrupt, only for USART1).
  * @return None
  */
void usart_interrupt_bit_clear(usart_reg_t* ptr_usart, uint32_t flag);

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

