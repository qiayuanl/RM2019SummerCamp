#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H
#define configUSE_STATS_FORMATTING_FUNCTIONS 1
#define configUSE_PREEMPTION			1
#define configUSE_IDLE_HOOK				0
#define configUSE_TICK_HOOK				0
#define configCPU_CLOCK_HZ				( 180000000 )
#define configTICK_RATE_HZ				( ( TickType_t ) 1000 )
#define configMAX_PRIORITIES			( 5 )
#define configMINIMAL_STACK_SIZE		( ( unsigned short ) 128 )  //最小栈分配大小
#define configTOTAL_HEAP_SIZE			( ( size_t ) ( 96 * 1024 ) )  //96KB的栈空间用作程序分配
#define configMAX_TASK_NAME_LEN			( 16 )
#define configUSE_TRACE_FACILITY		1
#define configUSE_16_BIT_TICKS			0
#define configIDLE_SHOULD_YIELD			1
#define configUSE_MUTEXES				1
#define configQUEUE_REGISTRY_SIZE		8
#define configCHECK_FOR_STACK_OVERFLOW	0
#define configUSE_RECURSIVE_MUTEXES		1
#define configUSE_MALLOC_FAILED_HOOK	0
#define configUSE_APPLICATION_TASK_TAG	0
#define configUSE_COUNTING_SEMAPHORES	1

#define configGENERATE_RUN_TIME_STATS	1

#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS()  ConfigureTimeForRunTimeStats()
#define portGET_RUN_TIME_COUNTER_VALUE() FreeRTOSRunTimeTicks

#define configUSE_CO_ROUTINES 		0
#define configMAX_CO_ROUTINE_PRIORITIES ( 2 )
#define configUSE_TIMERS				1
#define configTIMER_TASK_PRIORITY		( 2 )
#define configTIMER_QUEUE_LENGTH		10
#define configTIMER_TASK_STACK_DEPTH	( configMINIMAL_STACK_SIZE * 2 )
#define INCLUDE_vTaskPrioritySet		1
#define INCLUDE_uxTaskPriorityGet		1
#define INCLUDE_vTaskDelete				1
#define INCLUDE_vTaskCleanUpResources	1
#define INCLUDE_vTaskSuspend			1
#define INCLUDE_vTaskDelayUntil			1
#define INCLUDE_vTaskDelay				1
#define configPRIO_BITS       		4        /* 15 priority levels */
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY			0xf
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY	4
#define configKERNEL_INTERRUPT_PRIORITY 		( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
#define configASSERT( x ) if( ( x ) == 0 ) { taskDISABLE_INTERRUPTS(); for( ;; ); }	
#define vPortSVCHandler SVC_Handler
#define xPortPendSVHandler PendSV_Handler
#define xPortSysTickHandler SysTick_Handler
#endif

