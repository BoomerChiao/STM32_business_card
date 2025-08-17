/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "main.h"
#include "i2c.h"
#include "rtc.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ssd1306.h"
#include "math.h"

#include "ssd1306_fonts.h"
#include "stdio.h"
#define SHT40_ADDR      (0x44 << 1)  // STM32 HAL 需 8-bit address
#define SHT40_CMD_HIGH_PRECISION  0xFD


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

void display_test(void) {
    char line[32];
    float temp = 23.6;
    int tempx1 = temp ,tempx10 = temp *10;
    tempx10 = tempx10-tempx1*10;

    ssd1306_Fill(Black);
    sprintf(line, "T: %d.%d  C", tempx1,tempx10);
    ssd1306_SetCursor(0, 0);
    ssd1306_WriteString(line, Font_7x10, White);
    ssd1306_UpdateScreen();
}


void OLED_ShowTempHum(float temp, float hum, uint16_t counter,int TL,int TH) {
    char line[32];
    int tempx1 = (int)temp;
    int tempx10 = (int)(temp * 10) - tempx1 * 10;
    int humx1 = (int)hum;
    int humx10 = (int)(hum * 10) - humx1 * 10;

    sprintf(line, ": %d.%d C", tempx1, tempx10);
    ssd1306_SetCursor(10, 0);
    ssd1306_WriteString(line, Font_11x18, White);

    sprintf(line, ": %d.%d %%", humx1, humx10);
    ssd1306_SetCursor(10, 20);
    ssd1306_WriteString(line, Font_11x18, White);

    sprintf(line, "Temp");
    ssd1306_SetCursor(0, 38);
    ssd1306_WriteString(line, Font_7x10, White);

    sprintf(line, "H: %d    L: %d ", TH,TL);
    ssd1306_SetCursor(0, 52);
    ssd1306_WriteString(line, Font_7x10, White);

}

void OLED_ShowTempHum_BIG(float temp, float hum, uint16_t counter) {
	char line[32];
	int tempx1 = (int)temp;
	int tempx10 = (int)(temp * 10) - tempx1 * 10;
	int humx1 = (int)hum;
	int humx10 = (int)(hum * 10) - humx1 * 10;


	sprintf(line, ": %d.%d C", tempx1, tempx10);
	ssd1306_SetCursor(10, 0);
	ssd1306_WriteString(line, Font_16x24, White);
	//ssd1306_DrawBitmap(0, 0, Temp_bitmap, 8, 18, White); // 在 x=100, y=0 顯示

	sprintf(line, ": %d.%d %%", humx1, humx10);
	ssd1306_SetCursor(10, 40);
	ssd1306_WriteString(line, Font_16x24, White);
	//ssd1306_DrawBitmap(1, 45, Humidity_bitmap, 8, 12, White); // 在 x=100, y=0 顯示

}



void ShowNormalScreen(void) {
    ssd1306_WriteCommand(0xAF);
    ssd1306_Fill(Black);
    ssd1306_SetCursor(15, 0);
    ssd1306_WriteString("Normal", Font_16x24, White);
    ssd1306_SetCursor(30, 25);
    ssd1306_WriteString("mode", Font_16x24, White);
    ssd1306_SetCursor(3, 50);
    ssd1306_WriteString("( o_o )/", Font_7x10, White);
    ssd1306_SetContrast(240);
    ssd1306_UpdateScreen();
}

void ShowSleepScreen(void) {
    ssd1306_Fill(Black);
    ssd1306_SetCursor(20, 0);
    ssd1306_WriteString("Sleep", Font_16x24, White);
    ssd1306_SetCursor(30, 23);
    ssd1306_WriteString("mode", Font_16x24, White);
    ssd1306_SetCursor(15, 50);
    ssd1306_WriteString("( -.-)Zzz...", Font_7x10, White);
    ssd1306_UpdateScreen();
}



/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

const uint8_t Humidity_bitmap[] = {
		0x0008,
		0x0008,
		0x0018,
		0x003c,
		0x003c,
		0x007e,
		0x00ff,
		0x00ff,
		0x00fb,
		0x00f3,
		0x007e,
		0x003c
};

const uint8_t water_drop_bitmapA[] = {
		0x0000, 0x0008,
		0x0000, 0x0018,
		0x0000, 0x001c,
		0x0008, 0x003e,
		0x0008, 0x003e,
		0x0018, 0x003e,
		0x003c, 0x001c,
		0x003c, 0x0000,
		0x007e, 0x0000,
		0x00ff, 0x0000,
		0x00ff, 0x0000,
		0x00fb, 0x0000,
		0x00f3, 0x0000,
		0x007e, 0x0000,
		0x003c, 0x0000
};

const uint8_t Temp_bitmap[] = {
		0x0018,
		0x0024,
		0x0024,
		0x0024,
		0x0024,
		0x002c,
		0x0024,
		0x002c,
		0x002c,
		0x002c,
		0x002c,
		0x004a,
		0x0099,
		0x00bd,
		0x00bd,
		0x0099,
		0x0042,
		0x003c
};


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */


uint8_t sht40_buffer[6];
uint8_t SHT40_CRC8(const uint8_t *data) {
    uint8_t crc = 0xFF;
    for (uint8_t i = 0; i < 2; i++) {
        crc ^= data[i];
        for (uint8_t j = 0; j < 8; j++)
            crc = (crc & 0x80) ? (crc << 1) ^ 0x31 : (crc << 1);
    }
    return crc;
}

uint8_t SHT40_Read(float* temp, float* hum) {
    uint8_t cmd = SHT40_CMD_HIGH_PRECISION;
    if (HAL_I2C_Master_Transmit(&hi2c1, SHT40_ADDR, &cmd, 1, 100) != HAL_OK) return 0;
    HAL_Delay(10);
    if (HAL_I2C_Master_Receive(&hi2c1, SHT40_ADDR, sht40_buffer, 6, 100) != HAL_OK) return 0;

    if (SHT40_CRC8(&sht40_buffer[0]) != sht40_buffer[2]) return 0;
    if (SHT40_CRC8(&sht40_buffer[3]) != sht40_buffer[5]) return 0;

    uint16_t rawT = (sht40_buffer[0] << 8) | sht40_buffer[1];
    uint16_t rawH = (sht40_buffer[3] << 8) | sht40_buffer[4];

    *temp = -45.0f + 175.0f * (rawT / 65535.0f);
    *hum  = 100.0f * (rawH / 65535.0f);
    return 1;
}


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

void OLED_BreathEffect(void) {

	static uint8_t contrast = 0;// 從最小亮度起始
	static int8_t direction = 1;
	static uint32_t last_tick = 0;

	if (HAL_GetTick() - last_tick < 10) return; // 每 10ms 更新

	last_tick = HAL_GetTick();

	contrast += direction * 36;
	if (contrast >= 240) {
	contrast = 255;
	direction = -1;
	}

	else if (contrast <= 3) { // 最低亮度為 3，不會全暗
	contrast = 3;
	direction = 1;

	// 可選：關閉 OLED 來降低電流（部分 OLED 支援）
	ssd1306_WriteCommand(0xAE);  //Display OFF

	// 進入 STOP 模式
	HAL_SuspendTick();
	HAL_PWR_EnterSTOPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
	HAL_ResumeTick();
	// 醒來後重新設置時鐘
	SystemClock_Config();
	ssd1306_WriteCommand(0xAF);  // Display ON
	}
	ssd1306_SetContrast(contrast);
	}


/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int xxz=0;
int SW_mode=0 , SW_mode_rollback=0 ;
int TH=0,TL=99,TO;
float temp , hum ;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  ssd1306_Init();
  ssd1306_WriteCommand(0xC0);  // 垂直翻轉
  ssd1306_WriteCommand(0xA0);  // 水平翻轉
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET); // 將 PB0 設為 LOW

  HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);  // 一定要先停掉
  HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 19, RTC_WAKEUPCLOCK_CK_SPRE_16BITS);  // 理論每19秒,實測16秒
  HAL_Delay(200);


  while (1)
  {

	  if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15) == 0 )  {// 若按鈕按下（低電位）

		  switch(SW_mode){
		  case 0 :
			  SW_mode = 1;
			  SW_mode_rollback = 0;
			  ShowSleepScreen();
			  HAL_Delay(2500);// 延遲
			  break;
		  case 2 :
			  SW_mode = 1;
			  SW_mode_rollback = 2;
			  ShowSleepScreen();
			  HAL_Delay(2500);// 延遲

			  break;
		  default:

			  if(SW_mode_rollback == 0){
				  SW_mode = 0;
				  ShowNormalScreen();
				  HAL_Delay(1500);// 延遲
			  }else{
				  SW_mode = 2;
				  ShowNormalScreen();
				  HAL_Delay(1500);// 延遲
			  }
			  break;
		  }
	  }

	  if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3) == 0){
		  switch(SW_mode){
		  case 0 :
			  SW_mode=2;
			  SW_mode_rollback=0;

			  break;
		  case 2 :
			  SW_mode=0;
			  SW_mode_rollback=2;
			  break;
		  default:
			  if(SW_mode_rollback == 0){
				  SW_mode = 0;
				  ShowNormalScreen();
				  HAL_Delay(1500);// 延遲
			  }else{
				  SW_mode = 2;
				  ShowNormalScreen();
				  HAL_Delay(1500);// 延遲
			  }
			  break;
		  }	  }


	  xxz++;
	  TO=temp;
	  if(TO == 0){
	  }else{
		  if(TO > TH ){
		  TH=TO;
	  }
		  if(TO < TL ){
			 TL=TO;
		  }}


	  ssd1306_Fill(Black);
	  if(SW_mode == 1){
		  ssd1306_WriteCommand(0xAE);  //Display OFF
		  // 進入 STOP 模式
		  HAL_SuspendTick();
		  HAL_PWR_EnterSTOPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
		  HAL_ResumeTick();
		  // 醒來後重新設置時鐘
		  SystemClock_Config();
	  }else if(SW_mode == 2){
		  if (SHT40_Read(&temp, &hum)) {
			  ssd1306_DrawBitmap(0, 0, Temp_bitmap, 8, 18, White); // 在 x=100, y=0 顯示
			  ssd1306_DrawBitmap(1, 45, Humidity_bitmap, 8, 12, White); // 在 x=100, y=0 顯示
			  OLED_ShowTempHum_BIG(temp, hum, xxz);
			  OLED_BreathEffect();
			  HAL_Delay(500);// 延遲
		  }
	  }else{

		  if (SHT40_Read(&temp, &hum)) {
			  ssd1306_DrawBitmap(1, 24, Humidity_bitmap, 8, 12, White); // 在 x=100, y=0 顯示
			  ssd1306_DrawBitmap(0, 0, Temp_bitmap, 8, 18, White); // 在 x=100, y=0 顯示
			  OLED_ShowTempHum(temp, hum, xxz,TL,TH);
			  OLED_BreathEffect();
			  HAL_Delay(500);// 延遲

 	  }
 	  else {
 	      ssd1306_SetCursor(20, 0);
 	      ssd1306_WriteString("SHT40", Font_16x24, White);
 	      ssd1306_SetCursor(20, 30);
 	      ssd1306_WriteString("ERROR", Font_16x24, White);

 	  }	  }
 	  ssd1306_UpdateScreen();
 	  HAL_Delay(10);

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV4;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
