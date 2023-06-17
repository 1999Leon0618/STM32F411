# STM32F411

* 此頁面將記錄學習STM32F411RE的過程，並不會記錄所有步驟
* 主要學習[參考網址](https://blog.csdn.net/qq_36347513/category_10508589.html/ "link")
* 此型號關鍵規格如下
    * Core: Arm® 32-bit Cortex®-M4 CPU with FPU
    * 1×12-bit A/D converter: up to 16 channels
    * 16-stream DMA
    * 11 timers
    * communication interfaces
        * 3 I2C
        * 3 USARTs
        * 5 SPI/I2Ss
        * SD IO
        * USB 2.0 full-speed
* 開發環境
  * 設定方法可[參考](https://zhuanlan.zhihu.com/p/145801160) 
  * CubeMX+
  * CLion+CMake+OpenOCD

---

## 第一個項目

* 不免俗的第一個學習的是透過UART印出"Hello World !"
* 不過在這之中還是有需注意的地方
  1. 此練習是透過printf 函數實現的
  2. 設定CMake須注意![img.png](img.png)
