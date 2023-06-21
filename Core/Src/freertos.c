/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "DHT11.h"
#include "usart.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
uint8_t Data[5];
bool Mode = true;
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId LED_DisplayHandle;
osThreadId ButtonHandle;
osThreadId DHT11Handle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void LED_Display_Task(void const * argument);
void Button_Task(void const * argument);
void DHT11_Task(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
void LED_Display(uint32_t number,bool isdot);
void Four_LED_Display(char number[]);
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of LED_Display */
  osThreadDef(LED_Display, LED_Display_Task, osPriorityNormal, 0, 128);
  LED_DisplayHandle = osThreadCreate(osThread(LED_Display), NULL);

  /* definition and creation of Button */
  osThreadDef(Button, Button_Task, osPriorityIdle, 0, 128);
  ButtonHandle = osThreadCreate(osThread(Button), NULL);

  /* definition and creation of DHT11 */
  osThreadDef(DHT11, DHT11_Task, osPriorityIdle, 0, 128);
  DHT11Handle = osThreadCreate(osThread(DHT11), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_LED_Display_Task */
/**
  * @brief  Function implementing the LED_Display thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_LED_Display_Task */
void LED_Display_Task(void const * argument)
{
  /* USER CODE BEGIN LED_Display_Task */
  /* Infinite loop */
  for(;;)
  {
      char str[5];
      if(!Mode){
        str[0] = Data[0]/10 + '0';
        str[1] = Data[0]%10 + '0';
        str[2] = '.';
        str[3] = Data[1]/10 + '0';
        str[4] = Data[1]%10 + '0';
      } else{
        str[0] = Data[2]/10 + '0';
        str[1] = Data[2]%10 + '0';
        str[2] = '.';
        str[3] = Data[3]/10 + '0';
        str[4] = Data[3]%10 + '0';
      }

      Four_LED_Display(str);
    osDelay(1);
  }
  /* USER CODE END LED_Display_Task */
}

/* USER CODE BEGIN Header_Button_Task */
/**
* @brief Function implementing the Button thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Button_Task */
void Button_Task(void const * argument)
{
  /* USER CODE BEGIN Button_Task */
  /* Infinite loop */
  for(;;)
  {
      if(HAL_GPIO_ReadPin(GPIOA,Mode_Output_Pin) == GPIO_PIN_SET){
          HAL_Delay(10);
          if(HAL_GPIO_ReadPin(GPIOA,Mode_Output_Pin) == GPIO_PIN_SET){
              while (HAL_GPIO_ReadPin(GPIOA,Mode_Output_Pin) == GPIO_PIN_SET){
                  HAL_Delay(10);
              }
              Mode = !Mode;
          }
      }
    osDelay(1);
  }
  /* USER CODE END Button_Task */
}

/* USER CODE BEGIN Header_DHT11_Task */
/**
* @brief Function implementing the DHT11 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_DHT11_Task */
void DHT11_Task(void const * argument)
{
  /* USER CODE BEGIN DHT11_Task */
  /* Infinite loop */
  for(;;)
  {
      int isChange = DHT11_Read_Data(Data);

      char buf[16];
      buf[0] = '0'+Data[0]/10;
      buf[1] = '0'+Data[0]%10;
      buf[2] = '.';
      buf[3] = '0'+Data[1]/10;
      buf[4] = '0'+Data[1]%10;

      buf[5] = 0x20;

      buf[6] = '0'+Data[2]/10;
      buf[7] = '0'+Data[2]%10;
      buf[8] = '.';
      buf[9] = '0'+Data[3]/10;
      buf[10] = '0'+Data[3]%10;

      buf[11] = 0x20;
      if(isChange!=1){buf[12] = 0x31;}
      else{buf[12] = 0x30;}
      buf[13] = 0x20;
      buf[14] = 0x0D;
      buf[15] = 0x0a;
      HAL_UART_Transmit(&huart1, (uint8_t*)buf, 16, 1000);
      osDelay(500);
  }
  /* USER CODE END DHT11_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */


void Four_LED_Display(char number[]){
    /*
     * 顯示範�?�在0.001~9999，並保正?��?��?��??�顯�?
     * ex 123.456->123.4,1234.567->1234,1.2345->1.234
     * */
    int LED1_Number = 0;
    int LED2_Number = 0;
    int LED3_Number = 0;
    int LED4_Number = 0;
    bool dot1 = false;
    bool dot2 = false;
    bool dot3 = false;
    bool dot4 = false;


    char * integer_part = strtok(number, ".");
    char * decimal_part = strtok(NULL, ".");

    int integer_part_len = strlen(integer_part) ;
    int decimal_part_len = strlen(decimal_part);

    if((int) (integer_part + decimal_part_len) != 4){
        if(integer_part_len==4){decimal_part[4-integer_part_len]='\0';}
        else {
            integer_part[integer_part_len]='\0';
            decimal_part[4-integer_part_len]='\0';
        }
    } else{
        memset(integer_part, '0', 4-decimal_part_len);
        memset(decimal_part+decimal_part_len, '0', 4-integer_part_len);
    }
    switch (integer_part_len) {
        case 4:
            LED1_Number = integer_part[0]-'0';
            LED2_Number = integer_part[1]-'0';
            LED3_Number = integer_part[2]-'0';
            LED4_Number = integer_part[3]-'0';
            if(decimal_part[0]!='0'){dot4=true;}
            break;
        case 3:
            LED1_Number = integer_part[0]-'0';
            LED2_Number = integer_part[1]-'0';
            LED3_Number = integer_part[2]-'0';
            LED4_Number = decimal_part[3]-'0';
            dot3=true;
            break;
        case 2:
            LED1_Number = integer_part[0]-'0';
            LED2_Number = integer_part[1]-'0';
            LED3_Number = decimal_part[0]-'0';
            LED4_Number = decimal_part[1]-'0';
            dot2=true;
            break;
        case 1:
            LED1_Number = integer_part[0]-'0';
            LED2_Number = decimal_part[0]-'0';
            LED3_Number = decimal_part[1]-'0';
            LED4_Number = decimal_part[2]-'0';
            dot1=true;
            break;
    }

    int integer_size = integer_part_len+decimal_part_len;//

    if(integer_size >= 4){
        HAL_GPIO_WritePin(GPIOC,LED1_Pin,GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB,LED2_Pin,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC,LED3_Pin,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC,LED4_Pin,GPIO_PIN_RESET);
        LED_Display(LED1_Number, dot1);

    } else{
        HAL_GPIO_WritePin(GPIOC,LED1_Pin,GPIO_PIN_RESET);
    }
    osDelay(4);
    if(integer_size >= 3){
        HAL_GPIO_WritePin(GPIOC,LED1_Pin,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOB,LED2_Pin,GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOC,LED3_Pin,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC,LED4_Pin,GPIO_PIN_RESET);
        LED_Display(LED2_Number, dot2);
    }
    else{
        HAL_GPIO_WritePin(GPIOB,LED2_Pin,GPIO_PIN_RESET);
    }
    osDelay(4);

    if(integer_size >= 2){
        HAL_GPIO_WritePin(GPIOC,LED1_Pin,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOB,LED2_Pin,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC,LED3_Pin,GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOC,LED4_Pin,GPIO_PIN_RESET);
        LED_Display(LED3_Number, dot3);
    }
    else{
        HAL_GPIO_WritePin(GPIOC,LED3_Pin,GPIO_PIN_RESET);
    }
    osDelay(4);
    if(integer_size >= 1 ){
        HAL_GPIO_WritePin(GPIOC,LED1_Pin,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOB,LED2_Pin,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC,LED3_Pin,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC,LED4_Pin,GPIO_PIN_SET);
        LED_Display(LED4_Number, dot4);
    }
    else{
        HAL_GPIO_WritePin(GPIOC,LED4_Pin,GPIO_PIN_RESET);
    }
    osDelay(2);
}
void LED_Display(uint32_t number,bool isDot) {
    switch (number) {
        case 0:
            HAL_GPIO_WritePin(GPIOA,A_LED_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB,B_LED_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB,C_LED_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB,D_LED_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB,E_LED_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB,F_LED_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB,G_LED_Pin,GPIO_PIN_SET);
            break;
        case 1:
            HAL_GPIO_WritePin(GPIOA,A_LED_Pin,GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB,B_LED_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB,C_LED_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB,D_LED_Pin,GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB,E_LED_Pin,GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB,F_LED_Pin,GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB,G_LED_Pin,GPIO_PIN_SET);
            break;
        case 2:
            HAL_GPIO_WritePin(GPIOA,A_LED_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB,B_LED_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB,C_LED_Pin,GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB,D_LED_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB,E_LED_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB,F_LED_Pin,GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB,G_LED_Pin,GPIO_PIN_RESET);
            break;
        case 3:
            HAL_GPIO_WritePin(GPIOA,A_LED_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB,B_LED_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB,C_LED_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB,D_LED_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB,E_LED_Pin,GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB,F_LED_Pin,GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB,G_LED_Pin,GPIO_PIN_RESET);
            break;
        case 4:
            HAL_GPIO_WritePin(GPIOA,A_LED_Pin,GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB,B_LED_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB,C_LED_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB,D_LED_Pin,GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB,E_LED_Pin,GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB,F_LED_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB,G_LED_Pin,GPIO_PIN_RESET);
            break;
        case 5:
            HAL_GPIO_WritePin(GPIOA,A_LED_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB,B_LED_Pin,GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB,C_LED_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB,D_LED_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB,E_LED_Pin,GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB,F_LED_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB,G_LED_Pin,GPIO_PIN_RESET);
            break;
        case 6:
            HAL_GPIO_WritePin(GPIOA,A_LED_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB,B_LED_Pin,GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB,C_LED_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB,D_LED_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB,E_LED_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB,F_LED_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB,G_LED_Pin,GPIO_PIN_RESET);
            break;
        case 7:
            HAL_GPIO_WritePin(GPIOA,A_LED_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB,B_LED_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB,C_LED_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB,D_LED_Pin,GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB,E_LED_Pin,GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB,F_LED_Pin,GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB,G_LED_Pin,GPIO_PIN_SET);
            break;
        case 8:
            HAL_GPIO_WritePin(GPIOA,A_LED_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB,B_LED_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB,C_LED_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB,D_LED_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB,E_LED_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB,F_LED_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB,G_LED_Pin,GPIO_PIN_RESET);
            break;
        case 9:
            HAL_GPIO_WritePin(GPIOA,A_LED_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB,B_LED_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB,C_LED_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB,D_LED_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB,E_LED_Pin,GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB,F_LED_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB,G_LED_Pin,GPIO_PIN_RESET);
            break;
        default:
            HAL_GPIO_WritePin(GPIOA,A_LED_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB,B_LED_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB,C_LED_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB,D_LED_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB,E_LED_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB,F_LED_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB,G_LED_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB,H_LED_Pin,GPIO_PIN_SET);
            break;
    }
    if(!isDot)HAL_GPIO_WritePin(GPIOB,H_LED_Pin,GPIO_PIN_SET);
    else HAL_GPIO_WritePin(GPIOB,H_LED_Pin,GPIO_PIN_RESET);
}
/* USER CODE END Application */
