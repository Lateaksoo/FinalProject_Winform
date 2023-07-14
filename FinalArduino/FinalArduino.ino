#define WaterSensor A0  // A0핀을 수심센서로 설정
#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd1(0x27, 16, 2);  // (LCD I2C 주소, 16자, 2라인)

int ProcessSw[] = { 22, 23, 24, 25, 26, 27 };  //설비 정지 버튼
int LedRed[] = { 30, 32, 7, 41, 46, 49 };      //설비 정지
int LedGreen[] = { 31, 34, 39, 42, 47, 50 };   //설비 준비
int LedYellow[] = { 33, 35, 40, 43, 48, 51 };  //설비 가동중

struct MyStruct {
  String action;
  String process;
  String lotid;
  long starttime;
};

MyStruct myArray[] = {
  { "", "", "", 0 },
  { "", "", "", 0 },
  { "", "", "", 0 },
  { "", "", "", 0 },
  { "", "", "", 0 },
  { "", "", "", 0 }
};

const int numProcesses = 6;  //공정 개수
bool buttonState[numProcesses] = { false, false, false, false, false, false };
bool prevButtonState[numProcesses] = { false, false, false, false, false, false };
bool timerStarted[numProcesses] = { false, false, false, false, false, false };
unsigned long startTime[numProcesses] = { 0, 0, 0, 0, 0, 0 };
unsigned long elapsedTime[numProcesses] = { 0, 0, 0, 0, 0, 0 };

// unsigned long startTimeMix = 0;     // 시작시간
// unsigned long startTimeShape = 0;   // 시작시간
// unsigned long startTimeSteam = 0;   // 시작시간
// unsigned long startTimeFry = 0;     // 시작시간
// unsigned long startTimeFreeze = 0;  // 시작시간
// unsigned long startTimePack = 0;    // 시작시간

// bool timerStartedMix = false;     // 타이머 시작 bool
// bool timerStartedShape = false;   // 타이머 시작 bool
// bool timerStartedSteam = false;   // 타이머 시작 bool
// bool timerStartedFry = false;     // 타이머 시작 bool
// bool timerStartedFreeze = false;  // 타이머 시작 bool
// bool timerStartedPack = false;    // 타이머 시작 bool


unsigned long timerDuration = 10000;  // 10 seconds



void setup() {
  Serial.begin(9600);  // Serial monitor 구동 전원입력
  pinMode(WaterSensor, INPUT);

  lcd1.init();       // LCD 초기화
  lcd1.backlight();  // LCD 백라이트 ON

  for (int i = 0; i < numProcesses; i++) {
    pinMode(ProcessSw[i], INPUT_PULLUP);
    pinMode(LedRed[i], OUTPUT);
    pinMode(LedGreen[i], OUTPUT);
    pinMode(LedYellow[i], OUTPUT);
  }
}

String action = "";   //공정상태
String rest = "";     //나머지
String process = "";  // 공정명
String lotid = "";    // lotid

void loop() {
  // 버튼을 눌렀을 때
  for (int i = 0; i < numProcesses; i++) {
    buttonState[i] = digitalRead(ProcessSw[i]);

    // 버튼의 눌림 상태 변화를 감지하여 한 번만 실행
    if (buttonState[i] != prevButtonState[i]) {
      if (buttonState[i] == LOW && prevButtonState[i] == HIGH) {  // Falling Edge
        digitalWrite(LedRed[i], HIGH);

        if (timerStarted[i]) {
          SendStop(myArray[i].process, myArray[i].lotid);
          timerDuration = timerDuration - (millis() - startTime[i]);
          Serial.println(timerDuration);
          timerStarted[i] = false;  // 타이머 상태 초기화
        } else {
          SendContinue(myArray[i].process, myArray[i].lotid);
          startTime[i] = millis();
          timerStarted[i] = true;
        }
      }
      prevButtonState[i] = buttonState[i];
    }
  }

  // 윈폼에서 통신을 받는 곳
  if (Serial.available() > 0) {
    String line = Serial.readStringUntil('\n');
    line.trim();  // trim() 은 void 리턴.
    if (line.startsWith("$")) {
      action = line.substring(1, line.indexOf(","));
      rest = line.substring(line.indexOf(",") + 1);
      process = rest.substring(0, rest.indexOf(","));
      lotid = rest.substring(rest.indexOf(",") + 1);
      Processstart(action, process, lotid);
    }
  }


  // 타이머 상태 확인 및 처리
  for (int i = 0; i < numProcesses; i++) {
    if (timerStarted[i] && millis() - startTime[i] >= timerDuration) {
      Serial.println(timerDuration);
      SendEnd(myArray[i].process, myArray[i].lotid);
      timerStarted[i] = false;  // 타이머 상태 초기화
      timerDuration = 10000;    //10초로 초기화
    }
  }

  // if (timerStartedMix && millis() - startTimeMix >= timerDuration) {
  //   SendEnd(myArray[0].process, myArray[0].lotid);
  //   timerStartedMix = false;  // 타이머 상태 초기화
  // } else if (timerStartedShape && millis() - startTimeShape >= timerDuration) {
  //   SendEnd(myArray[1].process, myArray[1].lotid);
  //   timerStartedShape = false;  // 타이머 상태 초기화
  // } else if (timerStartedSteam && millis() - startTimeSteam >= timerDuration) {
  //   SendEnd(myArray[2].process, myArray[2].lotid);
  //   timerStartedSteam = false;  // 타이머 상태 초기화
  // } else if (timerStartedFry && millis() - startTimeFry >= timerDuration) {
  //   SendEnd(myArray[3].process, myArray[3].lotid);
  //   timerStartedFry = false;  // 타이머 상태 초기화
  // } else if (timerStartedFreeze && millis() - startTimeFreeze >= timerDuration) {
  //   SendEnd(myArray[4].process, myArray[4].lotid);
  //   timerStartedFreeze = false;  // 타이머 상태 초기화
  // } else if (timerStartedPack && millis() - startTimePack >= timerDuration) {
  //   SendEnd(myArray[5].process, myArray[5].lotid);
  //   timerStartedPack = false;  // 타이머 상태 초기화
  // }


  //  WaterSenser();

  // Process1(Process1Led1, Process1Led2, Process1Motor, Process1Sw);


  // UltrasonicSensor();
}