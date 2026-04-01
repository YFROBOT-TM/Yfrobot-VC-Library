
#include <YFVCLib_AVR.h>

YFVCLib_AVR yfvc_AVR_11_10(SerialType::SOFTWARE, 11, 10);
void setup() {
  // put your setup code here, to run once:
	Serial.begin(9600);
	yfvc_AVR_11_10.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(yfvc_AVR_11_10.getData());
}
