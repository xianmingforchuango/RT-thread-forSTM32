/**
  ******************************************************************************
  * @file    drv_systick.h
  * @author  mousie
  * @version V2.0.0
  * @date    2011.12.15
  * @brief   系统嘀嗒时钟, 并提供微秒和毫秒延时函数.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRV_SYSTICK_H
#define __DRV_SYSTICK_H

/* Include -------------------------------------------------------------------*/
#include <stdint.h>

/** @addtogroup Drivers
  * @{
  */
/** @addtogroup DRV_SysTick
  * @{
  */



/**
  ******************************************************************************
  * @defgroup DRV_SysTick_Configure
  * @brief    用户配置
  ******************************************************************************
  * @{
  */

/**
  * @}
  */



/** @defgroup DRV_SysTick_Public_TypeDefine
  * @brief    公有类型定义
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_SysTick_Public_MacroDefine
  * @brief    公有宏定义
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_SysTick_Public_Variable
  * @brief    声明公有全局变量
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_SysTick_Public_Function
  * @brief    定义公有函数
  * @{
  */
void systick_init_1ms(void);                                                    // 系统嘀嗒时钟初始化函数
void delay_scan_1ms(void);                                                      // 延时函数用的1ms扫描, 在1ms定时中断中调用
uint32_t delay_tick_get(void);                                                  // 获取1ms延时时钟值
void delay_ms(uint16_t time);                                                   // ms延时函数, 不可在中断函数中调用
void delay_us(uint16_t time);                                                   // us延时函数, 不可再中断函数中调用
/**
  * @}
  */



/**
  * @}
  */
/**
  * @}
  */

#endif
/* END OF FILE ---------------------------------------------------------------*/
