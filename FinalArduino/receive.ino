#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
String process, rest,action, lotid;  // command �� ������
void setup() {
  Serial.begin(9600);

  lcd.init();  // initialize the lcd
  lcd.backlight();
}

void loop() {
  action = ""; //��������
  rest = "";    //������
  process= "";   // ������
  lotid = "";   // lotid
  // $Run,Mix,213 ��� �ް� �̷��� �����ֵ��� �Ѵ�
  
  if (Serial.available() > 0) {  // <- ���� ���۸� ������ while(Serial.available()) ����ؾ� �Ѵ�.  https://www.baldengineer.com/when-do-you-use-the-arduinos-to-use-serial-flush.html
    String line = Serial.readStringUntil('\n');
    line.trim();  // trim() �� void ����.
    if (line.startsWith("$")) {
      action = line.substring(1, line.indexOf(",")); 
      rest = line.substring(line.indexOf(",") + 1);
      process = rest.substring(0, line.indexOf(",")); 
      lotid = rest.substring(line.indexOf(",") + 0);
      if (action == "Run") {
        lcd.print("run");
        Serial.println("$Recieve,lotstatus,lotid");
        Serial.println(action);
        Serial.println(process);
        Serial.println(lotid);

      }
    }
  }
}
