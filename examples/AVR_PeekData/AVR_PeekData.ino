/*
  YFrobot VC Library - Arduino Uno peekData 示例

  说明：
  1. getData() 用于刷新串口识别结果。
  2. peekData() 用于读取缓存，适合同一轮逻辑中多次判断。
  3. clearData() 用于在动作执行完成后清除缓存，避免重复触发。

  硬件连接：
  语音模块        Arduino Uno
  -----------    -----------
  VCC            5V
  GND            GND
  TX             D11 (RX)
  RX             D10 (TX)
*/

#include <YFVCLib.h>
#include <SoftwareSerial.h>

const int VC_RX = 11; // Arduino 接收脚，连接语音模块 TX
const int VC_TX = 10; // Arduino 发送脚，连接语音模块 RX
const int LED_PIN = 13; // 板载 LED

SoftwareSerial softSerial(VC_RX, VC_TX);
UnoSoftwareSerial yfvc(&softSerial);

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);

  yfvc.begin(9600);
}

void loop() {
  // 先刷新一次识别结果。
  yfvc.getData();

  // 后续判断统一读取缓存，避免在同一轮中重复读取串口。
  if (yfvc.peekData() != SerialBaseVC::NO_DATA) {
    if (yfvc.peekData() == 0x02) {
      // 指令 0x02：点亮 LED。
      digitalWrite(LED_PIN, HIGH);
    } else if (yfvc.peekData() == 0x03) {
      // 指令 0x03：熄灭 LED。
      digitalWrite(LED_PIN, LOW);
    } else {
      // 其他指令：闪烁一次作为提示。
      digitalWrite(LED_PIN, HIGH);
      delay(50);
      digitalWrite(LED_PIN, LOW);
    }

    // 当前指令处理完成后清除缓存，保证只执行一次。
    yfvc.clearData();
  }

  delay(10);
}
