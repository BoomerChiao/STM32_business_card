# STM32_business_card
STM32G030-based electronic business card with SHT40 sensor and OLED display for basic temperature and humidity monitoring.

此電子名片採用 STM 架構完成功能，內部預設 SHT40 和 OLED 顯示螢幕電路布局，可輕鬆實現即時溫溼度量測功能。

##  Summary　（概要）
STM32 business card 是由 STMicroelectronics 旗下的 STM32G030 系列微控制器為核心設計的開發板。

在整體電路設計中，整合了 SHT40 高精度溫溼度感測器，以用於監測周遭環境的溫度和濕度變化。此外，確保量測數據能即時更新，同時搭載一片 OLED 顯示面板，可將即時感測到的數據清晰呈現，方便使用者隨時掌握環境狀態。

##  Installation　（安裝）
安裝 STM32CubeIDE 軟體
- STM32CubeIDE 軟體下載：https://www.st.com/en/development-tools/stm32cubeide.html
- (非必要) STM32CubeProg 軟體下載：https://www.st.com/en/development-tools/stm32cubeprog.html

<img width="380" height="190" alt="image" src="https://github.com/user-attachments/assets/b5cbb4b9-5129-4945-b7db-0bba6f51969a" />

##  Import data　（導入資料）
1. 下載 GitHub 資料 code 專案：
   - 你可以從本網的 GitHub 下載所需的資料，或是自行創建一個新的 STM32 專案。

     你可以在這裡下載檔案：https://github.com/BoomerChiao/STM32_business_card/tree/main/STM32_code
    
2. 導入至STM32程式函數庫：
   - 以下將提供 3 種導入方式
      1. File → Import → General → Existing Projects into Workspace → Next，在 Select root directory 選擇專案資料夾，勾選找到的專案，然後按 Finish。
      2. File → Import → General → Archive File，選擇.zip 專案檔 → CubeIDE 就會自動幫你解壓並辨識，然後按 Finish。
      3. 解壓縮.zip 專案檔，直接點擊.project檔案。
         
4. 完成STM32導入後：
   - 插入 ST-LINK V2 USB。
   - 確認 ST-LINK 連線是否連接燒入線
      1. USB_VDD(3.3V) → 3.3V
      2. USB_GND → GND
      3. USB_SWDIO → DIO(A13)
      4. USB_SWCLK → CLK(A14)


##  Prepare Electronic Materials　（準備電子材料）
1. ST-LINK/V2
2. STM32G030F6P6 Mini Development or STM32G030F6P6 
3. 0.96' OLED Display 128×64 4Pin IIC I2C
4. SHT40 (3.3V)
5. Jumper Wires

<img width="1272" height="509" alt="image" src="https://github.com/user-attachments/assets/e86c9bad-4899-4e23-9991-dc41d4910793" />


##  Pin wiring　（引腳接線）
以下提供基礎引腳接線方式，請依據你的成品需求進行接線。

下圖分別展示 OLED Display 與 SHT40 的接線方式。



是由超可愛寶寶的超可愛帥氣男友兼未來老公的吉吉阿奇爸爸所設計，
上面有可愛吉吉商標設計，
