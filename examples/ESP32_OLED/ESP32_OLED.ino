/*
  YFrobot VC Library - ESP32 搭配 OLED 示例

  本示例演示带 OLED 显示的语音控制。

  硬件连接：
  语音模块        ESP32    OLED (SSD1306)
  -----------    ----    --------------
  VCC          3.3V/5V  VCC
  GND          GND      GND
  TX           GPIO16   -
  RX           GPIO17   -
  -            GPIO21   SDA
  -            GPIO22   SCL

  依赖库：
  - U8g2 库（可通过库管理器安装）

  创建时间：2024-01-20
  作者：YFROBOT
  许可证：MIT
*/

#include <YFVCLib.h>
#include <U8g2lib.h>

const int VC_RX = 16; // ESP32 的 RX 引脚
const int VC_TX = 17; // ESP32 的 TX 引脚
const int LED_PIN = 2; // 板载 LED

YFESP32HardwareSerial yfvc(&Serial2, VC_RX, VC_TX);
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* 时钟=*/ SCL, /* 数据=*/ SDA, /* 复位=*/ U8X8_PIN_NONE);

String lastCommand = "Waiting...";

void oledDisplay(String text, uint8_t x, uint8_t y) {
  u8g2.setCursor(x, y);
  u8g2.print(text);
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  
  yfvc.begin(9600);
  
  u8g2.begin();
  u8g2.enableUTF8Print();
  u8g2.setFont(u8g2_font_wqy12_t_gb2312);
  u8g2.setFontDirection(0);
  
  updateOLED();
  
  Serial.println("=========================================");
  Serial.println("YFrobot Voice Module - ESP32 OLED Demo");
  Serial.println("=========================================");
}

void updateOLED() {
  u8g2.clearBuffer();
  oledDisplay("语音控制", 30, 20);
  oledDisplay(lastCommand, 20, 40);
  u8g2.sendBuffer();
}

void loop() {
  uint8_t cmd = yfvc.getData();
  
  if (cmd != 0x01) {
    Serial.print("Voice Command: ");
    Serial.println(cmd);
    
    switch(cmd) {
      case 2:
        Serial.println("  -> Turn ON LED");
        digitalWrite(LED_PIN, HIGH);
        lastCommand = "开灯";
        break;
        
      case 3:
        Serial.println("  -> Turn OFF LED");
        digitalWrite(LED_PIN, LOW);
        lastCommand = "关灯";
        break;
        
      default:
        Serial.println("  -> Unknown command");
        lastCommand = "未知指令";
        break;
    }
    
    updateOLED();
  }
  
  delay(10);
}
