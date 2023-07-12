# PWM Pulse Count

# Timer Set

* Timer1
    * Prescaler = 31
    * ARR = 9999
    * Pulse = 5000
    * Frequency = 100Hz
* Timer2
  * Prescaler = 63
  * ARR = 4294967295
  * Rise = 4999
  * Fall = 2499

---

## 實驗結果

* DutyCycle = (float ) (IC2Value+1)*100/(float )(IC1Value+1);
  Frequency = 1000000/(float )(IC1Value+1);
* DC = (2999+1)*100/(4999+1) = 50%
* Pulse Frequency = 1000000/(4999+1) = 200 Hz
* Frequency = Pulse Frequency/2 = 100 Hz
![圖片參考名稱](Image/1.png "Logo")