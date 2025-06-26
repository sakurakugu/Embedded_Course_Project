/*
 * FreeRTOS Kernel V11.2.0
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

/*******************************************************************************
 * This file provides an example FreeRTOSConfig.h header file, inclusive of an
 * abbreviated explanation of each configuration item.  Online and reference
 * documentation provides more information.
 * https://www.freertos.org/a00110.html
 *
 * Constant values enclosed in square brackets ('[' and ']') must be completed
 * before this file will build.
 *
 * Use the FreeRTOSConfig.h supplied with the RTOS port in use rather than this
 * generic file, if one is available.
 ******************************************************************************/

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/******************************************************************************/
/* Hardware description related definitions. **********************************/
/******************************************************************************/

/* STM32F103VCT6的系统时钟频率为72MHz */
#define configCPU_CLOCK_HZ    ( ( unsigned long ) 72000000 )

/* SysTick时钟频率与CPU时钟频率相同 */
/* #define configSYSTICK_CLOCK_HZ                  configCPU_CLOCK_HZ */

/******************************************************************************/
/* Scheduling behaviour related definitions. **********************************/
/******************************************************************************/

/* 设置系统节拍频率为1000Hz，即1ms一次中断 */
// #define configTICK_RATE_HZ                      100 （默认）
#define configTICK_RATE_HZ                         1000

/* 使用抢占式调度器 */
#define configUSE_PREEMPTION                       1

/* 使能时间片轮转调度 */
#define configUSE_TIME_SLICING                     1

/* 使用通用C算法选择下一个要运行的任务 */
#define configUSE_PORT_OPTIMISED_TASK_SELECTION    0

/* 不使用低功耗tickless模式 */
#define configUSE_TICKLESS_IDLE                    0

/* 最大优先级为32 */
#define configMAX_PRIORITIES                       5

/* 空闲任务的堆栈大小 */
#define configMINIMAL_STACK_SIZE                   128

/* 任务名称最大长度 */
#define configMAX_TASK_NAME_LEN                    16

/* 使用32位的tick计数类型 */
#define configTICK_TYPE_WIDTH_IN_BITS              TICK_TYPE_WIDTH_32_BITS

/* 空闲任务应该让出CPU */
#define configIDLE_SHOULD_YIELD                    1

/* 每个任务的通知数组条目数 */
#define configTASK_NOTIFICATION_ARRAY_ENTRIES      1

/* 队列注册表大小，仅用于调试 */
#define configQUEUE_REGISTRY_SIZE                  0

/* 不使用向后兼容功能 */
#define configENABLE_BACKWARD_COMPATIBILITY        0

/* 线程本地存储指针数量 */
#define configNUM_THREAD_LOCAL_STORAGE_POINTERS    0

/* 使用小型列表项以节省RAM */
#define configUSE_MINI_LIST_ITEM                   1

/* 堆栈深度类型 */
#define configSTACK_DEPTH_TYPE                     uint16_t

/* 消息缓冲区长度类型 */
#define configMESSAGE_BUFFER_LENGTH_TYPE           size_t

/* 释放内存时清除内存 */
#define configHEAP_CLEAR_MEMORY_ON_FREE            1

/* 统计缓冲区最大长度 */
#define configSTATS_BUFFER_MAX_LENGTH              0xFFFF

/* 不使用newlib可重入结构 */
#define configUSE_NEWLIB_REENTRANT                 0

/******************************************************************************/
/* Software timer related definitions. ****************************************/
/******************************************************************************/

/* 使用软件定时器 */
#define configUSE_TIMERS                1

/* 定时器任务优先级 */
#define configTIMER_TASK_PRIORITY       ( configMAX_PRIORITIES - 1 )

/* 定时器任务堆栈大小 */
#define configTIMER_TASK_STACK_DEPTH    ( 256 )

/* 定时器命令队列长度 */
#define configTIMER_QUEUE_LENGTH        10

/******************************************************************************/
/* Event Group related definitions. *******************************************/
/******************************************************************************/

/* 使用事件组 */
#define configUSE_EVENT_GROUPS    1

/******************************************************************************/
/* Stream Buffer related definitions. *****************************************/
/******************************************************************************/

/* 使用流缓冲区 */
#define configUSE_STREAM_BUFFERS    1

/******************************************************************************/
/* Memory allocation related definitions. *************************************/
/******************************************************************************/

/* 支持静态分配 */
#define configSUPPORT_STATIC_ALLOCATION              1

/* 支持动态分配 */
#define configSUPPORT_DYNAMIC_ALLOCATION             1

/* FreeRTOS堆大小，根据应用程序需求调整 */
#define configTOTAL_HEAP_SIZE                        ( 10 * 1024 )

/* 不使用应用程序分配堆 */
#define configAPPLICATION_ALLOCATED_HEAP             0

/* 不使用单独的堆分配任务堆栈 */
#define configSTACK_ALLOCATION_FROM_SEPARATE_HEAP    0

/* 不启用堆保护器 */
#define configENABLE_HEAP_PROTECTOR                  0

/******************************************************************************/
/* Interrupt nesting behaviour configuration. *********************************/
/******************************************************************************/

/* Cortex-M3的中断优先级配置 */
#define configKERNEL_INTERRUPT_PRIORITY         255
#define configMAX_SYSCALL_INTERRUPT_PRIORITY    191
#define configMAX_API_CALL_INTERRUPT_PRIORITY   191

/******************************************************************************/
/* Hook and callback function related definitions. ****************************/
/******************************************************************************/

/* 不使用空闲钩子 */
#define configUSE_IDLE_HOOK                   0

/* 不使用节拍钩子 */
#define configUSE_TICK_HOOK                   0

/* 使用内存分配失败钩子 */
#define configUSE_MALLOC_FAILED_HOOK          1

/* 不使用守护任务启动钩子 */
#define configUSE_DAEMON_TASK_STARTUP_HOOK    0

/* 不使用流缓冲区完成回调 */
#define configUSE_SB_COMPLETED_CALLBACK       0

/* 使用堆栈溢出检测方法2 */
#define configCHECK_FOR_STACK_OVERFLOW        2

/******************************************************************************/
/* Run time and task stats gathering related definitions. *********************/
/******************************************************************************/

/* 不生成运行时统计信息 */
#define configGENERATE_RUN_TIME_STATS           0

/* 不使用跟踪工具 */
#define configUSE_TRACE_FACILITY                0

/* 不使用统计格式化函数 */
#define configUSE_STATS_FORMATTING_FUNCTIONS    0

/******************************************************************************/
/* Co-routine related definitions. ********************************************/
/******************************************************************************/

/* 不使用协程 */
#define configUSE_CO_ROUTINES              0
#define configMAX_CO_ROUTINE_PRIORITIES    1

/******************************************************************************/
/* Debugging assistance. ******************************************************/
/******************************************************************************/

/* 断言定义 */
#define configASSERT( x )         \
    if( ( x ) == 0 )              \
    {                             \
        taskDISABLE_INTERRUPTS(); \
        for( ; ; )                \
        ;                         \
    }

/******************************************************************************/
/* FreeRTOS MPU specific definitions. *****************************************/
/******************************************************************************/

/* 不包含应用程序定义的特权函数 */
#define configINCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS    0

/* MPU区域数量 */
#define configTOTAL_MPU_REGIONS                                   8

/* Flash的TEX、S、C、B位设置 */
#define configTEX_S_C_B_FLASH                                     0x07UL

/* SRAM的TEX、S、C、B位设置 */
#define configTEX_S_C_B_SRAM                                      0x07UL

/* 只允许内核代码进行特权提升 */
#define configENFORCE_SYSTEM_CALLS_FROM_KERNEL_ONLY               1

/* 不允许非特权任务进入临界区 */
#define configALLOW_UNPRIVILEGED_CRITICAL_SECTIONS                0

/* 使用V2版MPU包装器 */
#define configUSE_MPU_WRAPPERS_V1                                 0

/* 受保护的内核对象池大小 */
#define configPROTECTED_KERNEL_OBJECT_POOL_SIZE                   10

/* 系统调用栈大小 */
#define configSYSTEM_CALL_STACK_SIZE                              128

/* 启用访问控制列表 */
#define configENABLE_ACCESS_CONTROL_LIST                          1

/******************************************************************************/
/* SMP( Symmetric MultiProcessing ) Specific Configuration definitions. *******/
/******************************************************************************/

/* Set configNUMBER_OF_CORES to the number of available processor cores.
 * Defaults to 1 if left undefined. */

/*
 #define configNUMBER_OF_CORES                     [Num of available cores]
 */

/* When using SMP (i.e. configNUMBER_OF_CORES is greater than one), set
 * configRUN_MULTIPLE_PRIORITIES to 0 to allow multiple tasks to run
 * simultaneously only if they do not have equal priority, thereby maintaining
 * the paradigm of a lower priority task never running if a higher priority task
 * is able to run. If configRUN_MULTIPLE_PRIORITIES is set to 1, multiple tasks
 * with different priorities may run simultaneously - so a higher and lower
 * priority task may run on different cores at the same time. */
#define configRUN_MULTIPLE_PRIORITIES             0

/* When using SMP (i.e. configNUMBER_OF_CORES is greater than one), set
 * configUSE_CORE_AFFINITY to 1 to enable core affinity feature. When core
 * affinity feature is enabled, the vTaskCoreAffinitySet and
 * vTaskCoreAffinityGet APIs can be used to set and retrieve which cores a task
 * can run on. If configUSE_CORE_AFFINITY is set to 0 then the FreeRTOS
 * scheduler is free to run any task on any available core. */
#define configUSE_CORE_AFFINITY                   0

/* When using SMP with core affinity feature enabled, set
 * configTASK_DEFAULT_CORE_AFFINITY to change the default core affinity mask for
 * tasks created without an affinity mask specified. Setting the define to 1
 * would make such tasks run on core 0 and setting it to (1 <<
 * portGET_CORE_ID()) would make such tasks run on the current core. This config
 * value is useful, if swapping tasks between cores is not supported (e.g.
 * Tricore) or if legacy code should be controlled. Defaults to tskNO_AFFINITY
 * if left undefined. */
#define configTASK_DEFAULT_CORE_AFFINITY          tskNO_AFFINITY

/* When using SMP (i.e. configNUMBER_OF_CORES is greater than one), if
 * configUSE_TASK_PREEMPTION_DISABLE is set to 1, individual tasks can be set to
 * either pre-emptive or co-operative mode using the vTaskPreemptionDisable and
 * vTaskPreemptionEnable APIs. */
#define configUSE_TASK_PREEMPTION_DISABLE         0

/* When using SMP (i.e. configNUMBER_OF_CORES is greater than one), set
 * configUSE_PASSIVE_IDLE_HOOK to 1 to allow the application writer to use
 * the passive idle task hook to add background functionality without the
 * overhead of a separate task. Defaults to 0 if left undefined. */
#define configUSE_PASSIVE_IDLE_HOOK               0

/* When using SMP (i.e. configNUMBER_OF_CORES is greater than one),
 * configTIMER_SERVICE_TASK_CORE_AFFINITY allows the application writer to set
 * the core affinity of the RTOS Daemon/Timer Service task. Defaults to
 * tskNO_AFFINITY if left undefined. */
#define configTIMER_SERVICE_TASK_CORE_AFFINITY    tskNO_AFFINITY

/******************************************************************************/
/* ARMv8-M secure side port related definitions. ******************************/
/******************************************************************************/

/* 安全上下文最大数量 */
#define secureconfigMAX_SECURE_CONTEXTS        5

/* 使用内核提供的静态内存 */
#define configKERNEL_PROVIDED_STATIC_MEMORY    1

/******************************************************************************/
/* ARMv8-M port Specific Configuration definitions. ***************************/
/******************************************************************************/

/* STM32F103VCT6是Cortex-M3内核，不需要ARMv8-M相关配置 */
/* #define configENABLE_TRUSTZONE            0 */
/* #define configRUN_FREERTOS_SECURE_ONLY    0 */
/* #define configENABLE_MPU                  0 */
/* #define configENABLE_FPU                  0 */
/* #define configENABLE_MVE                  0 */

/******************************************************************************/
/* ARMv7-M and ARMv8-M port Specific Configuration definitions. ***************/
/******************************************************************************/

/* 检查处理程序安装 */
#define configCHECK_HANDLER_INSTALLATION    1

/******************************************************************************/
/* Definitions that include or exclude functionality. *************************/
/******************************************************************************/

/* 功能包含/排除设置 */
#define configUSE_TASK_NOTIFICATIONS           1
#define configUSE_MUTEXES                      1
#define configUSE_RECURSIVE_MUTEXES            1
#define configUSE_COUNTING_SEMAPHORES          1
#define configUSE_QUEUE_SETS                   1
#define configUSE_APPLICATION_TASK_TAG         0

/* 不使用POSIX错误号 */
#define configUSE_POSIX_ERRNO                  0

/* API函数包含设置 */
#define INCLUDE_vTaskPrioritySet               1
#define INCLUDE_uxTaskPriorityGet              1
#define INCLUDE_vTaskDelete                    1
#define INCLUDE_vTaskSuspend                   1
#define INCLUDE_vTaskDelayUntil                1
#define INCLUDE_vTaskDelay                     1
#define INCLUDE_xTaskGetSchedulerState         1
#define INCLUDE_xTaskGetCurrentTaskHandle      1
#define INCLUDE_uxTaskGetStackHighWaterMark    1
#define INCLUDE_xTaskGetIdleTaskHandle         1
#define INCLUDE_eTaskGetState                  1
#define INCLUDE_xTimerPendFunctionCall         1
#define INCLUDE_xTaskAbortDelay                1
#define INCLUDE_xTaskGetHandle                 1
#define INCLUDE_xTaskResumeFromISR             1

#endif /* FREERTOS_CONFIG_H */
