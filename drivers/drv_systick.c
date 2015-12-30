/**
  ******************************************************************************
  * @file    drv_systick.c
  * @author  mousie
  * @version V2.0.0
  * @date    2011.12.15
  * @brief   系统嘀嗒时钟, 并提供微秒和毫秒延时函数.
  ******************************************************************************
  */

/* Include -------------------------------------------------------------------*/
#include "drv_systick.h"

#include "misc.h"

/** @addtogroup Drivers
  * @{
  */
/** @defgroup DRV_SysTick
  * @{
  */



/**
  ******************************************************************************
  * @addtogroup DRV_SysTick_Configure
  ******************************************************************************
  * @{
  */
#include "stm32f10x.h"
#define systick_enable_int(mask)        __set_PRIMASK(mask)
#define systick_disable_int()           __get_PRIMASK(); __disable_irq()
/**
  * @}
  */



/** @defgroup DRV_SysTick_Private_TypeDefine
  * @brief    私有类型定义
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_SysTick_Private_MacroDefine
  * @brief    私有宏定义
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_SysTick_Variables
  * @brief    定义全局变量(私有/公有)
  * @{
  */
static volatile int32_t timer_delay_ms = 0;                                       ///< 毫秒延时计数器
/**
  * @}
  */

/** @defgroup DRV_SysTick_Private_Function
  * @brief    定义私有函数
  * @{
  */

/**
  * @}
  */



/** @defgroup DRV_SysTick_Function
  * @brief    函数原文件
  * @{
  */

/**
  ******************************************************************************
  * @brief  系统嘀嗒时钟初始化为1ms周期. 其中断处理函数见"irq_handler.c".
  * @param  none
  * @retval none
  ******************************************************************************
  */
void systick_init_1ms(void)
{
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
    SysTick_Config(SystemCoreClock / 1000);
}

/**
  ******************************************************************************
  * @brief  延时函数1ms扫描, 毫秒必须要用此函数. 建议放在定时器中断中.
  * @param  none
  * @retval none
  ******************************************************************************
  */
void delay_scan_1ms(void)
{
    timer_delay_ms++;
}

/**
  ******************************************************************************
  * @brief  获取1ms延时时钟值.
  * @param  none
  * @retval none
  ******************************************************************************
  */
uint32_t delay_tick_get(void)
{
    return timer_delay_ms;
}

/**
  ******************************************************************************
  * @brief  毫秒延时函数
  * @param  time, 要延时的毫秒时间. 误差在+10us之内. 该函数不允许在中断中使用!
  * @retval none
  ******************************************************************************
  */
void delay_ms(uint16_t time)
{
    uint32_t timer_delay_ms_head;
    uint32_t timer_delay_ms_tail;
    uint32_t timer_delay_ms_cur;
    uint32_t timer_delay_tick_head;
    uint32_t mask;

    if (time)
    {
        mask = systick_disable_int();
        timer_delay_tick_head = SysTick->VAL;
        timer_delay_ms_head = timer_delay_ms;
        systick_enable_int(mask);
        timer_delay_ms_cur  = timer_delay_ms_head;
        timer_delay_ms_tail = timer_delay_ms_head + time;

        if (timer_delay_ms_tail >= timer_delay_ms_head)                         // 画一直线, 目标时间在起始时间的右边
        {
            while((timer_delay_ms_cur >= timer_delay_ms_head) && (timer_delay_ms_cur <= timer_delay_ms_tail))
            {
                mask = systick_disable_int();
                timer_delay_ms_cur = timer_delay_ms;
                if ((timer_delay_ms_cur == timer_delay_ms_tail) && (SysTick->VAL <= timer_delay_tick_head))
                {
                    systick_enable_int(mask);
                    break;
                }
                systick_enable_int(mask);
            }
        }
        else                                                                    // 画一直线, 目标时间在起始时间的左边
        {
            while((timer_delay_ms_cur >= timer_delay_ms_head) || (timer_delay_ms <= timer_delay_ms_tail))
            {
                mask = systick_disable_int();
                timer_delay_ms_cur = timer_delay_ms;
                if ((timer_delay_ms_cur == timer_delay_ms_tail) && (SysTick->VAL <= timer_delay_tick_head))
                {
                    systick_enable_int(mask);
                    break;
                }
                systick_enable_int(mask);
            }
        }
    }
}

/**
  ******************************************************************************
  * @brief  微秒延时函数
  * @param  time, 要延时的微秒时间. <1000us时, 误差在+5us之内. 该函数不允许在中断中使用!
  * @retval none
  ******************************************************************************
  */
void delay_us(uint16_t time)
{
  uint32_t timer_delay_tick_head;
  uint32_t timer_delay_tick_tail;
  uint32_t timer_delay_tick_cur;
  uint32_t timer_delay_ms_tail;
  uint32_t mask;

  timer_delay_tick_head = SysTick->VAL;
  delay_ms(time / 1000);
  mask = systick_disable_int();
  timer_delay_ms_tail   = timer_delay_ms + time/1000 + 1;
  systick_enable_int(mask);
  timer_delay_tick_tail = (time % 1000) * (SystemCoreClock / 1000000);
  timer_delay_tick_cur  = SysTick->VAL;

  if (timer_delay_tick_tail <= timer_delay_tick_head)
  {
    timer_delay_tick_tail = timer_delay_tick_head - timer_delay_tick_tail;
    while((timer_delay_tick_cur < timer_delay_tick_head) && (timer_delay_tick_cur > timer_delay_tick_tail))
    {
      timer_delay_tick_cur = SysTick->VAL;
      mask = systick_disable_int();
      if ((timer_delay_ms > timer_delay_ms_tail) || (timer_delay_ms < (timer_delay_ms_tail - 1)))
      {
        systick_enable_int(mask);
        break;
      }
      systick_enable_int(mask);
    }
  }
  else
  {
    timer_delay_tick_tail = SysTick->LOAD + timer_delay_tick_head - timer_delay_tick_tail - 1;
    while((timer_delay_tick_cur < timer_delay_tick_head) || (timer_delay_tick_cur > timer_delay_tick_tail))
    {
      timer_delay_tick_cur = SysTick->VAL;
      mask = systick_disable_int();
      if ((timer_delay_ms > timer_delay_ms_tail) || (timer_delay_ms < (timer_delay_ms_tail - 1)))
      {
        systick_enable_int(mask);
        break;
      }
      systick_enable_int(mask);
    }
  }
}

/**
  * @}
  */



/**
  * @}
  */
/**
  * @}
  */

/* END OF FILE ---------------------------------------------------------------*/
