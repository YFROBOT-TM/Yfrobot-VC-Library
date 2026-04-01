#define yfvcHSerial  // 使用硬件串口
#include <YFVClib.h>

YFVClib yfvc(0, 1, 9600); // 硬件串口 这里0 1可以忽略，无用

// #define yfvcHSerial Serial

void setup() {
  // put your setup code here, to run once:
  // yfvc.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  yfvc.getData();
}
