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
   - 點擊左側IDE專案，選擇STM32_SHT40xOLED.ioc檔案雙擊兩下開啟
   - .ioc檔開啟後，點擊上面欄位🔨符號，確認配置偵錯是否可以跑
   - 偵錯跑完無錯誤，則導入專案成功
  
   <img width="421" height="373" alt="image" src="https://github.com/user-attachments/assets/c73b2eec-20de-401f-aa3d-d2e8fefae9c4" />



##  Prepare Electronic Materials　（準備電子材料）
1. ST-LINK/V2
2. STM32G030F6P6 Mini Development or STM32G030F6P6 
3. 0.96' OLED Display 128×64 4Pin IIC I2C
4. SHT40 (3.3V)
5. button switch 4 pin 6×6×4.5mm *2
6. Jumper Wires

<img width="1291" height="933" alt="image" src="https://github.com/user-attachments/assets/5296084b-6683-4ffd-9939-0d240719a148" />

##  Pin wiring　（引腳接線）
以下提供基礎引腳接線方式，請依據你的成品需求進行接線。

下圖分別展示 OLED Display 與 SHT40 的接線方式。
- 請注意，該板子設計僅持支援 3V 電壓範圍。因此，建議將 VCC 預設為 3V 電源以確保正常運作。
- STM32G030F6P6 Mini Development: PC14 (SDA) ; PB7 (SCL) ; PB3 (Switch A) ; PC15 (Switch B)
- SHT40: SDA (PC14) ; SCL (PB7)
- OLED Display: SDA (PC14) ; SCL (PB7)
- Switch A: A (GND) ; B (PB3)
- Switch B: A (GND) ; B (PC15)

<img width="466" height="178" alt="image" src="https://github.com/user-attachments/assets/f4e0aed0-f5c4-40f6-86e4-75048d66b856" />

<img width="618" height="143" alt="image" src="https://github.com/user-attachments/assets/2ed7f07d-182b-4b3c-823c-9bb49474049a" />


<img width="445" height="307" alt="image" src="https://github.com/user-attachments/assets/3d73f53a-b735-4b50-8107-3fc33a6771e7" />

##  Programming STM32 with firmware　（STM32程式代碼燒入）
1. 工具準備🔧：
   - ST-LINK V2 (或相容版本)
   - 4 根杜邦線
   - STM32 開發板或晶片
    
2. 硬體接線 (SWD 模式)⚡：
   -  USB_VDD(3.3V) → 3.3V
   -  USB_GND → GND
   -  USB_SWDIO → DIO(A13)
   -  USB_SWCLK → CLK(A14)

4. 軟體燒錄📥：
   -  安裝並打開 STM32CubeIDE
   -  開啟需要燒入專案
   -  插入 ST-LINK V2 USB。
   -  按下「Run」◀️ 或「Debug」🐞 就能直接下載程式
  
##  Software Presentation　（成品展示）
### OLED畫面
<img width="520" height="530" alt="image" src="https://github.com/user-attachments/assets/b3df9c0f-fb48-40ad-8f1b-3c3b31612042" />

- 上圖為主畫面，提供溫度及濕度和最高溫與最低溫相關資訊
   -  溫度計符號「🌡️」為 攝氏溫度
   -  水滴符號「💧」為 濕度
   -  Temp 下面「H」英文為high，專門紀錄目前探測到最高溫度；而「L」英文為low，負責記錄目前探測到最低溫度


<img width="539" height="504" alt="image" src="https://github.com/user-attachments/assets/95dd5113-53ad-46a7-bd05-6981e4adb522" />

- 上圖為子畫面，提供最基本溫度及濕度顯示


### OLED顯示功能與視覺效果
<img src="https://raw.githubusercontent.com/BoomerChiao/STM32_business_card/main/JPG/OLED_DISPLAY.gif" width="400" alt="OLED Demo" />

- 上圖子母畫面正常運轉情況下，亮4秒，滅14秒


<img src="https://github.com/BoomerChiao/STM32_business_card/blob/main/JPG/OLED_TOOL_A.gif" width="400" alt="OLED Demo" />

- 當負責 PC15 按鈕被觸發時，系統將進入切換 SLEEP 或 NORMAL 畫面；若畫面處於熄滅的狀況下，則會自動喚醒


<img src="https://github.com/BoomerChiao/STM32_business_card/blob/main/JPG/OLED_TOOL_B.gif" width="400" alt="OLED Demo" />

- 當負責 PB3 按鈕被觸發時，系統將進入切換子母畫面；若畫面處於熄滅的狀況下，則會自動喚醒

##  Advanced Design　（進階設計）

是由超可愛寶寶的超可愛帥氣男友兼未來老公的吉吉阿奇爸爸所設計，
上面有可愛吉吉商標設計，...................................................................
