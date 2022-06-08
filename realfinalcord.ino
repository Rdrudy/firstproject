#include <virtuabotixRTC.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int SPEAKER = 9;
const int colPins[4] = {26, 27, 28, 29};
const int rowPins[4] = {25, 24, 23, 22};
const char keymap[4][4] ={
  {'M', 'L', 'X', 'X'}, //* Dotmatrix 상에서 Menu, Up, Cancel, Home
  {'U', 'O', 'D', 'X'}, //                Left, Ok, Right, X
  {'C', 'R', 'X', 'X'}, //                 X, Down, X, X
  {'H', 'X', 'X', 'X'} //                   X, X, X, X 
  };
long lastDebounce = 0;
 
virtuabotixRTC myRTC(6, 7, 8); // RTC 핀
LiquidCrystal_I2C lcd(0x3F, 16, 2); // 0x3F, 가로 16칸 세로2칸 LCD

int boom = 0; //* 메뉴로딩화면과 메뉴화면 구분에 쓰임. 'C'버튼 작동 위해서도 쓰임
// 이하 'C' 버튼 작동 위해 쓰임 설명 생략
char controlkey = 'H'; // 초기화면(시계)와 메뉴 화면을 구분하기 위해 쓰임

int switch123 = 1; //* LCD 메뉴 화면(수면, 일, 휴식)에서 포인터('<')를 이동하기 위해 쓰임
int set123 = 0;  // LCD 메뉴 화면(수면, 일, 휴식)구분에 쓰임

int awakeswitch = 0; //* 수면시각과 기상시각 설정화면 구분에 쓰임
int awake123 = 1; // 기상시각 시, 분, 초 구분에 쓰임
int awakehour = 8; // 기상시각 기본값은 8시
int awakemin = 0; 
int awakesec = 0;
int sleep123 = 1; // 수면시각 시, 분, 초 구분에 쓰임
int sleephour = 21; // 수면시각 기본값은 21시
int sleepmin = 0;
int sleepsec = 0;
int alarmhour = 0; // 기상시각이 되면 알람이 울리도록 하기 위해
int alarmmin = 0; // 코드를 명확하게 하기 위해 한 번 더 변수를 설정해주었음
int alarmsec = 0;

int workswitch = 0; // 일 시작시각과 일 종료시각 설정화면 구분에 쓰임
int workst123 = 1; // 일 시작시각 시, 분, 초 구분에 쓰임
int worksthour = 8; // 일 시작시각 기본값은 8시
int workstmin = 0;
int workstsec = 0;
int workend123 = 1; // 일 종료시각 시, 분, 초 구분에 쓰임
int workendhour = 16; // 일 종료시각 기본값은 16시
int workendmin = 0;
int workendsec = 0;

int restswitch = 0; // 휴식 시작시각과 휴식 종료시각 설정화면 구분에 쓰임
int restst123 = 1; // 휴식 시작시각 시, 분, 초 구분에 쓰임
int reststhour = 0; // 휴식 시작시각 기본값은 0시
int reststmin = 0;
int reststsec = 0;
int restend123 = 1; // 휴식 종료시각 시, 분, 초 구분에 쓰임
int restendhour = 0; // 휴식 종료시각 기본값은 0시
int restendmin = 0; 
int restendsec = 0;


void setup()  {
  Serial.begin(9600);
  pinMode(12,OUTPUT); // RGBLED RED
  pinMode(11,OUTPUT); // RGBLED GREEN
  pinMode(10,OUTPUT); // RGBLED BLUE

for(int index = 0; index<4; index++){
  pinMode(rowPins[index], INPUT_PULLUP);
  pinMode(colPins[index], OUTPUT);
  digitalWrite(colPins[index], HIGH);
  } // dotmatrix
  
  lcd.init();
  lcd.backlight(); // LCD setup
  myRTC.setDS1302Time(30, 59, 7, 4, 8, 6, 2022); // 처음 시작시각 세팅
}
 
void loop()  {
  char key = 'X';
  
  for(int column = 0; column<4; column++){
    digitalWrite(colPins[column], LOW);
    for(int row = 0; row<4; row++){
      if(digitalRead(rowPins[row]) == LOW){
        if(millis() - lastDebounce>200){
          lastDebounce = millis();
          if(keymap[column][row]=='M' || keymap[column][row]=='H') 
          {//'M'이나 'H'를 입력받았을 경우 controlkey로
            controlkey = keymap[column][row];
            }
            else{key = keymap[column][row];}// 그 외에는 key로
            }
        }
      }
      digitalWrite(colPins[column], HIGH);
      }   
      
    if(controlkey == 'H'){//초기화면
      boom = 0; //'C'버튼 작동 위해 루프문 안에서도 boom을 0으로 초기화
      myRTC.updateTime();
    lcd.setCursor(1,0); // lcd.print 출력의 시작 위치를 정해준다
    lcd.print(myRTC.year);
    lcd.print("/");
    lcd.print(myRTC.month);
    lcd.print("/");
    lcd.print(myRTC.dayofmonth); 
    lcd.print("/"); // 년, 월, 일 출력
    switch (myRTC.dayofweek) {//switch문을 이용해 요일 출력
      case 1: 
      lcd.print("Sun");
      break;
      case 2: 
      lcd.print("MON");
      break;
      case 3: 
      lcd.print("TUE");
      break;
      case 4:   
      lcd.print("WED");
      break;
      case 5:  
      lcd.print("THU");
      break;
      case 6:   
      lcd.print("FRI");
      break;
      case 7:    
      lcd.print("SAT");
      break;
      }     
      lcd.setCursor(5,1); // lcd.print 출력의 시작 위치를 정해준다. 이하생략
      lcd.print(myRTC.hours);
      lcd.print(":");
      lcd.print(myRTC.minutes);
      lcd.print(":");
      lcd.print(myRTC.seconds); // 시, 분, 초 출력
      
      delay(1000);
      lcd.clear();
      }
     
      if(controlkey == 'M'){
        if(boom==0){//메뉴로딩화면
          lcd.clear();
        lcd.setCursor(1,0);
        lcd.print(">LifePattern<");
        lcd.setCursor(1,1);
        lcd.print("Loading");
        delay(2000);
        boom = 1;
            }
          }
          if(boom ==1 && set123 == 0){
//set123==0이면 메뉴선택화면, (set123=1 || set123=2 || set123=3)이면 각각의 시작/종료시각설정화면
          lcd.clear();
          switch(switch123){// 포인터('<')위치 이동 위해 switch문 사용
            case 1:
            lcd.setCursor(1,0);
            lcd.print("1.Sleep< 2.Work");
            lcd.setCursor(5,1);
            lcd.print("3.Rest");
            
              digitalWrite(colPins[1], LOW);
              delay(1000);
              if(digitalRead(rowPins[1]) == LOW){// 'O'버튼을 누르면
                set123 =1; // 수면패턴설정화면 출력 위한 변수
                switch123 =1; // 초기값인 1로 초기화(불필요하지만 코드의 통일성을 위해)
                boom = 2; // 코드의 확장가능성을 염두에 두어 boom은 2로 초기화
                }
              digitalWrite(colPins[1], HIGH);
              digitalWrite(colPins[2], LOW);              
              if(digitalRead(rowPins[0]) == LOW){// 'C'버튼을 누르면
                controlkey = 'H'; //이전화면이 초기화면이므로 conrtolkey를 'H'로 초기화
                switch123 = 1; // 초기값인 1로 초기화(불필요하지만 코드의 통일성을 위해)
                boom = 0; // 이전화면으로 돌아가니 boom은 0으로 초기화             
                }
              digitalWrite(colPins[2], HIGH);
              break;
            case 2:
            lcd.setCursor(1,0);
            lcd.print("1.Sleep 2.Work<");
            lcd.setCursor(5,1);
            lcd.print("3.Rest");
            
              digitalWrite(colPins[1], LOW);
              delay(1000);
              if(digitalRead(rowPins[1]) == LOW){// 'O'버튼을 누르면
                set123 =2; // 일패턴설정화면 출력 위한 변수
                switch123 =1; // 초기값인 1로 초기화
                boom = 2; // 코드의 확장가능성을 위해 2로 초기화(설명 이하 생략)
                }
              digitalWrite(colPins[1], HIGH);
              digitalWrite(colPins[2], LOW);              
              if(digitalRead(rowPins[0]) == LOW){// 'C'버튼을 누르면
                controlkey = 'H'; //이전화면이 초기화면이므로 'H'로 초기화
                switch123 =1; // 초기값인 1로 초기화
                boom = 0; // boom은 0으로 초기화(설명 이하 생략)               
                }
              digitalWrite(colPins[2], HIGH);            
            break;
            case 3:
            lcd.setCursor(1,0);
            lcd.print("1.Sleep 2.Work");
            lcd.setCursor(5,1);
            lcd.print("3.Rest<");
            
              digitalWrite(colPins[1], LOW);
              delay(1000);
              if(digitalRead(rowPins[1]) == LOW){
                set123 =3;
                switch123 =1;
                boom = 2;
                }
              digitalWrite(colPins[1], HIGH);
              digitalWrite(colPins[2], LOW);              
              if(digitalRead(rowPins[0]) == LOW){
                controlkey = 'H';
                switch123 =1;
                boom = 0; //설명 생략               
                }
              digitalWrite(colPins[2], HIGH);            
            break;     
            }
          digitalWrite(colPins[1], LOW);
          delay(1000);    
          if(digitalRead(rowPins[2]) == LOW){//'D'버튼을 누르면
            switch123 = switch123 +1;//switch123이 1 증가하여 메뉴선택화면이 변화
            digitalWrite(colPins[1], HIGH);
            if(switch123 == 4){switch123 = 1;} //4번 이상 메뉴가 없으므로 1번 메뉴로 가도록 설정
            }
          if(digitalRead(rowPins[0]) == LOW){//'U'버튼을 누르면
            switch123 = switch123 -1;//switch123이 1 감소하여 메뉴선택화면이 변화
            digitalWrite(colPins[1], HIGH);            
            if(switch123 == 0){switch123 = 3;} //0번 이하 메뉴가 없으므로 3번 메뉴로 가도록 설정
            }                  
          }
          if(set123 == 1 || set123 == 2 || set123 == 3){//LCD메뉴화면
            switch(set123){//switch문을 통해 메뉴를 선택했을 때 각각의 메뉴화면 구분
            case 1:
            if(awakeswitch == 0){//수면시각설정화면과 기상시각설정화면 구분을 위한 변수
            switch(sleep123){//포인터('<')위치 이동 위해 switch문 사용
              case 1:
              lcd.clear();
              lcd.setCursor(1,0);
              lcd.print("When you sleep?");
              lcd.setCursor(1,1);
              lcd.print(sleephour);
              lcd.print("<:");
              lcd.print(sleepmin);
              lcd.print(":");
              lcd.print(sleepsec);
              digitalWrite(colPins[1], LOW);
              delay(500);    
              if(digitalRead(rowPins[0]) == LOW){//'U'버튼을 누르면
                sleephour = sleephour +1;//sleephour가 1씩 증가
                if(sleephour == 24){sleephour = 0;}//24시 = 0시 이므로 24가 되면 0이 되도록 설정
                }
              digitalWrite(colPins[1], HIGH);              
              digitalWrite(colPins[1], LOW);
              delay(500);    
              if(digitalRead(rowPins[2]) == LOW){//'D'버튼을 누르면
                  sleephour = sleephour -1;//sleephour가 1씩 감소
                  if(sleephour == -1){sleephour = 23;}//위와 같은 원리로 -1이 되면 23이 되도록 설정
                }
              digitalWrite(colPins[1], HIGH);  
              digitalWrite(colPins[2], LOW);
              delay(500);    
              if(digitalRead(rowPins[1]) == LOW){//'R'버튼을 누르면
                sleep123 = sleep123 +1;//sleep123가 1씩 증가, 최종적으로 포인터위치 오른쪽으로 이동
                if(sleep123 == 4){sleep123 = 1;}//원하는 위치가 세 군데이므로 sleep123이 4가 되면 1이 되도록 설정
                }
              digitalWrite(colPins[2], HIGH);  
              digitalWrite(colPins[0], LOW);
              delay(500);    
              if(digitalRead(rowPins[1]) == LOW){//'L'버튼을 누르면
                  sleep123 = sleep123 -1;//sleep123가 1씩 감소, 최종적으로 포인터위치 왼쪽으로 이동
                  if(sleep123 == 0){sleep123 = 3;}//원하는 위치가 세 군데 이므로 sleep123이 0이 되면 3이 되도록 설정
                }
              digitalWrite(colPins[0], HIGH);
                    digitalWrite(colPins[1], LOW);
                    delay(1000);
                    if(digitalRead(rowPins[1]) == LOW){//'O'버튼을 누르면
                      awakeswitch = 1;//기상시각설정화면으로 이동하기 위해 awakeswitch=1이 된다!
                    }
                    digitalWrite(colPins[1], HIGH);  
                    digitalWrite(colPins[2], LOW);              
                    if(digitalRead(rowPins[0]) == LOW){//'C'버튼을 누르면
                    set123 = 0;
                    boom = 1;//이전화면인 메뉴선택화면으로 돌아가기 위해 변수초기화
                    }
                    digitalWrite(colPins[2], HIGH);                  
                    break;//같은 방식으로 구동하는 코드이므로 이하 설명생략
              case 2:                                   
              lcd.clear();
              lcd.setCursor(1,0);
              lcd.print("When you sleep?");
              lcd.setCursor(1,1);
              lcd.print(sleephour);
              lcd.print(":");
              lcd.print(sleepmin);
              lcd.print("<:");
              lcd.print(sleepsec);
              digitalWrite(colPins[1], LOW);
              delay(500);    
              if(digitalRead(rowPins[0]) == LOW){
                sleepmin = sleepmin +1;
                if(sleepmin == 60){sleepmin = 0;}
                }
              digitalWrite(colPins[1], HIGH);              
              digitalWrite(colPins[1], LOW);
              delay(500);    
              if(digitalRead(rowPins[2]) == LOW){
                  sleepmin = sleepmin -1;
                  if(sleepmin == -1){sleepmin = 59;}
                }
              digitalWrite(colPins[1], HIGH);  
              digitalWrite(colPins[2], LOW);
              delay(500);    
              if(digitalRead(rowPins[1]) == LOW){
                sleep123 = sleep123 +1;
                if(sleep123 == 4){sleep123 = 1;}
                }
              digitalWrite(colPins[2], HIGH);  
              digitalWrite(colPins[0], LOW);
              delay(500);    
              if(digitalRead(rowPins[1]) == LOW){
                  sleep123 = sleep123 -1;
                  if(sleep123 == 0){sleep123 = 3;}
                }
                    digitalWrite(colPins[1], LOW);
                    delay(1000);
                    if(digitalRead(rowPins[1]) == LOW){
                      awakeswitch = 1;
                      }
                      digitalWrite(colPins[1], HIGH);
                    digitalWrite(colPins[2], LOW);              
                    if(digitalRead(rowPins[0]) == LOW){
                    set123 = 0;
                    boom = 1;
                    }
                    digitalWrite(colPins[2], HIGH);                        
                      break;
              case 3:
              lcd.clear();
              lcd.setCursor(1,0);
              lcd.print("When you sleep?");
              lcd.setCursor(1,1);
              lcd.print(sleephour);
              lcd.print(":");
              lcd.print(sleepmin);
              lcd.print(":");
              lcd.print(sleepsec);
              lcd.print("<");
              digitalWrite(colPins[1], LOW);
              delay(500);    
              if(digitalRead(rowPins[0]) == LOW){
                sleepsec = sleepsec +1;
                if(sleepsec == 60){sleepsec = 0;}
                }
              digitalWrite(colPins[1], HIGH);              
              digitalWrite(colPins[1], LOW);
              delay(500);    
              if(digitalRead(rowPins[2]) == LOW){
                  sleepsec = sleepsec -1;
                  if(sleepsec == -1){sleepsec = 59;}
                }
              digitalWrite(colPins[1], HIGH);  
              digitalWrite(colPins[2], LOW);
              delay(500);    
              if(digitalRead(rowPins[1]) == LOW){
                sleep123 = sleep123 +1;
                if(sleep123 == 4){sleep123 = 1;}
                }
              digitalWrite(colPins[2], HIGH);  
              digitalWrite(colPins[0], LOW);
              delay(500);    
              if(digitalRead(rowPins[1]) == LOW){
                  sleep123 = sleep123 -1;
                  if(sleep123 == 0){sleep123 = 3;}
                }
                    digitalWrite(colPins[1], LOW);
                    delay(1000);
                    if(digitalRead(rowPins[1]) == LOW){
                      awakeswitch = 1;                  
                  }
                  digitalWrite(colPins[1], HIGH);
                    digitalWrite(colPins[2], LOW);              
                    if(digitalRead(rowPins[0]) == LOW){
                    set123 = 0;
                    boom = 1;
                    }
                    digitalWrite(colPins[2], HIGH);                    
                  break;                                
            }
            }
            if(awakeswitch == 1){//기상시각설정화면
              switch(awake123){
              case 1:
              lcd.clear();
              lcd.setCursor(1,0);
              lcd.print("When you awake?");
              lcd.setCursor(1,1);
              lcd.print(awakehour);
              lcd.print("<:");
              lcd.print(awakemin);
              lcd.print(":");
              lcd.print(awakesec);
              digitalWrite(colPins[1], LOW);
              delay(500);    
              if(digitalRead(rowPins[0]) == LOW){
                awakehour = awakehour +1;
                if(awakehour == 24){awakehour = 0;}
                }
              digitalWrite(colPins[1], HIGH);              
              digitalWrite(colPins[1], LOW);
              delay(500);    
              if(digitalRead(rowPins[2]) == LOW){
                  awakehour = awakehour -1;
                  if(awakehour == -1){awakehour = 23;}
                }
              digitalWrite(colPins[1], HIGH);  
              digitalWrite(colPins[2], LOW);
              delay(500);    
              if(digitalRead(rowPins[1]) == LOW){
                awake123 = awake123 +1;
                if(awake123 == 4){awake123 = 1;}
                }
              digitalWrite(colPins[2], HIGH);  
              digitalWrite(colPins[0], LOW);
              delay(500);    
              if(digitalRead(rowPins[1]) == LOW){
                  awake123 = awake123 -1;
                  if(awake123 == 0){awake123 = 3;}
                }
              digitalWrite(colPins[0], HIGH);
                    digitalWrite(colPins[1], LOW);
                    delay(1000);
                    if(digitalRead(rowPins[1]) == LOW){  
                      alarmhour = awakehour;
                      alarmmin = awakemin;
                      alarmsec = awakesec;//명확성을 위해 한 번 더 변수 설정
                      set123 = 0;
                      boom = 0;
                      awakeswitch = 0;
                    controlkey = 'H';//설정이 끝났으므로 초기화면으로 돌아간다!!
                    }
                    digitalWrite(colPins[1], HIGH);                    
                    digitalWrite(colPins[2], LOW);              
                    if(digitalRead(rowPins[0]) == LOW){//'C'버튼을 누르면
                    awakeswitch = 0;//수면시각설정화면으로 복귀
                    }
                    digitalWrite(colPins[2], HIGH);                
                    break;//설명 이하생략
              case 2:                                   
              lcd.clear();
              lcd.setCursor(1,0);
              lcd.print("When you awake?");
              lcd.setCursor(1,1);
              lcd.print(awakehour);
              lcd.print(":");
              lcd.print(awakemin);
              lcd.print("<:");
              lcd.print(awakesec);
              digitalWrite(colPins[1], LOW);
              delay(500);    
              if(digitalRead(rowPins[0]) == LOW){
                awakemin = awakemin +1;
                if(awakemin == 60){awakemin = 0;}
                }
              digitalWrite(colPins[1], HIGH);              
              digitalWrite(colPins[1], LOW);
              delay(500);    
              if(digitalRead(rowPins[2]) == LOW){
                  awakemin = awakemin -1;
                  if(awakemin == -1){awakemin = 59;}
                }
              digitalWrite(colPins[1], HIGH);  
              digitalWrite(colPins[2], LOW);
              delay(500);    
              if(digitalRead(rowPins[1]) == LOW){
                awake123 = awake123 +1;
                if(awake123 == 4){awake123 = 1;}
                }
              digitalWrite(colPins[2], HIGH);  
              digitalWrite(colPins[0], LOW);
              delay(500);    
              if(digitalRead(rowPins[1]) == LOW){
                  awake123 = awake123 -1;
                  if(awake123 == 0){awake123 = 3;}
                }
                    digitalWrite(colPins[1], LOW);
                    delay(1000);
                    if(digitalRead(rowPins[1]) == LOW){
                      alarmhour = awakehour;
                      alarmmin = awakemin;
                      alarmsec = awakesec;
                      set123 = 0;
                      boom = 0;
                      awakeswitch = 0;
                      controlkey = 'H';}   
                      digitalWrite(colPins[1], HIGH);
                    digitalWrite(colPins[2], LOW);              
                    if(digitalRead(rowPins[0]) == LOW){
                    awakeswitch = 0;
                    }
                    digitalWrite(colPins[2], HIGH);                                            
                      break;
              case 3:
              lcd.clear();
              lcd.setCursor(1,0);
              lcd.print("When you awake?");
              lcd.setCursor(1,1);
              lcd.print(awakehour);
              lcd.print(":");
              lcd.print(awakemin);
              lcd.print(":");
              lcd.print(awakesec);
              lcd.print("<");
              digitalWrite(colPins[1], LOW);
              delay(500);    
              if(digitalRead(rowPins[0]) == LOW){
                awakesec = awakesec +1;
                if(awakesec == 60){awakesec = 0;}
                }
              digitalWrite(colPins[1], HIGH);              
              digitalWrite(colPins[1], LOW);
              delay(500);    
              if(digitalRead(rowPins[2]) == LOW){
                  awakesec = awakesec -1;
                  if(awakesec == -1){awakesec = 59;}
                }
              digitalWrite(colPins[1], HIGH);  
              digitalWrite(colPins[2], LOW);
              delay(500);    
              if(digitalRead(rowPins[1]) == LOW){
                awake123 = awake123 +1;
                if(awake123 == 4){awake123 = 1;}
                }
              digitalWrite(colPins[2], HIGH);  
              digitalWrite(colPins[0], LOW);
              delay(500);    
              if(digitalRead(rowPins[1]) == LOW){
                  awake123 = awake123 -1;
                  if(awake123 == 0){awake123 = 3;}
                }
                    digitalWrite(colPins[1], LOW);
                    delay(1000);
                    if(digitalRead(rowPins[1]) == LOW){
                      alarmhour = awakehour;
                      alarmmin = awakemin;
                      alarmsec = awakesec;
                      set123 = 0;
                      boom = 0;
                      awakeswitch = 0;
                      controlkey = 'H';}
                      digitalWrite(colPins[1], HIGH);
                    digitalWrite(colPins[2], LOW);              
                    if(digitalRead(rowPins[0]) == LOW){
                    awakeswitch = 0;
                    }
                    digitalWrite(colPins[2], HIGH);                            
                      break;                                
            }
            }
            break;
            case 2:
            if(workswitch == 0){//업무시각설정화면과 업무종료시각설정화면 구분
            switch(workst123){//위에서 설명한 코드와 같은 구동방식이므로 설명생략
              case 1:
              lcd.clear();
              lcd.setCursor(1,0);
              lcd.print("Work Start Time?");
              lcd.setCursor(1,1);
              lcd.print(worksthour);
              lcd.print("<:");
              lcd.print(workstmin);
              lcd.print(":");
              lcd.print(workstsec);
              digitalWrite(colPins[1], LOW);
              delay(500);    
              if(digitalRead(rowPins[0]) == LOW){
                worksthour = worksthour +1;
                if(worksthour == 24){worksthour = 0;}
                }
              digitalWrite(colPins[1], HIGH);              
              digitalWrite(colPins[1], LOW);
              delay(500);    
              if(digitalRead(rowPins[2]) == LOW){
                  worksthour = worksthour -1;
                  if(worksthour == -1){worksthour = 23;}
                }
              digitalWrite(colPins[1], HIGH);  
              digitalWrite(colPins[2], LOW);
              delay(500);    
              if(digitalRead(rowPins[1]) == LOW){
                workst123 = workst123 +1;
                if(workst123 == 4){workst123 = 1;}
                }
              digitalWrite(colPins[2], HIGH);  
              digitalWrite(colPins[0], LOW);
              delay(500);    
              if(digitalRead(rowPins[1]) == LOW){
                  workst123 = workst123 -1;
                  if(workst123 == 0){workst123 = 3;}
                }
              digitalWrite(colPins[0], HIGH);
                    digitalWrite(colPins[1], LOW);
                    delay(1000);
                    if(digitalRead(rowPins[1]) == LOW){
                      workswitch = 1;
                    }
                    digitalWrite(colPins[1], HIGH);  
                    digitalWrite(colPins[2], LOW);              
                    if(digitalRead(rowPins[0]) == LOW){
                    set123 = 0;
                    boom = 1;
                    }
                    digitalWrite(colPins[2], HIGH);                  
                    break;
              case 2:                                   
              lcd.clear();
              lcd.setCursor(1,0);
              lcd.print("Work Start Time?");
              lcd.setCursor(1,1);
              lcd.print(worksthour);
              lcd.print(":");
              lcd.print(workstmin);
              lcd.print("<:");
              lcd.print(workstsec);
              digitalWrite(colPins[1], LOW);
              delay(500);    
              if(digitalRead(rowPins[0]) == LOW){
                workstmin = workstmin +1;
                if(workstmin == 60){workstmin = 0;}
                }
              digitalWrite(colPins[1], HIGH);              
              digitalWrite(colPins[1], LOW);
              delay(500);    
              if(digitalRead(rowPins[2]) == LOW){
                  workstmin = workstmin -1;
                  if(workstmin == -1){workstmin = 59;}
                }
              digitalWrite(colPins[1], HIGH);  
              digitalWrite(colPins[2], LOW);
              delay(500);    
              if(digitalRead(rowPins[1]) == LOW){
                workst123 = workst123 +1;
                if(workst123 == 4){workst123 = 1;}
                }
              digitalWrite(colPins[2], HIGH);  
              digitalWrite(colPins[0], LOW);
              delay(500);    
              if(digitalRead(rowPins[1]) == LOW){
                  workst123 = workst123 -1;
                  if(workst123 == 0){workst123 = 3;}
                }
                    digitalWrite(colPins[1], LOW);
                    delay(1000);
                    if(digitalRead(rowPins[1]) == LOW){
                      workswitch = 1;
                      }
                      digitalWrite(colPins[1], HIGH);
                    digitalWrite(colPins[2], LOW);              
                    if(digitalRead(rowPins[0]) == LOW){
                    set123 = 0;
                    boom = 1;
                    }
                    digitalWrite(colPins[2], HIGH);                          
                      break;
              case 3:
              lcd.clear();
              lcd.setCursor(1,0);
              lcd.print("Work Start Time?");
              lcd.setCursor(1,1);
              lcd.print(worksthour);
              lcd.print(":");
              lcd.print(workstmin);
              lcd.print(":");
              lcd.print(workstsec);
              lcd.print("<");
              digitalWrite(colPins[1], LOW);
              delay(500);    
              if(digitalRead(rowPins[0]) == LOW){
                workstsec = workstsec +1;
                if(workstsec == 60){workstsec = 0;}
                }
              digitalWrite(colPins[1], HIGH);              
              digitalWrite(colPins[1], LOW);
              delay(500);    
              if(digitalRead(rowPins[2]) == LOW){
                  workstsec = workstsec -1;
                  if(workstsec == -1){workstsec = 59;}
                }
              digitalWrite(colPins[1], HIGH);  
              digitalWrite(colPins[2], LOW);
              delay(500);    
              if(digitalRead(rowPins[1]) == LOW){
                workst123 = workst123 +1;
                if(workst123 == 4){workst123 = 1;}
                }
              digitalWrite(colPins[2], HIGH);  
              digitalWrite(colPins[0], LOW);
              delay(500);    
              if(digitalRead(rowPins[1]) == LOW){
                  workst123 = workst123 -1;
                  if(workst123 == 0){workst123 = 3;}
                }
                    digitalWrite(colPins[1], LOW);
                    delay(1000);
                    if(digitalRead(rowPins[1]) == LOW){
                      workswitch = 1;                  
                  }
                  digitalWrite(colPins[1], HIGH);
                    digitalWrite(colPins[2], LOW);              
                    if(digitalRead(rowPins[0]) == LOW){
                    set123 = 0;
                    boom = 1;
                    }
                    digitalWrite(colPins[2], HIGH);                      
                  break;                                
            }
            }
            if(workswitch == 1){//업무종료시각설정화면
              switch(workend123){
              case 1:
              lcd.clear();
              lcd.setCursor(1,0);
              lcd.print("Work End Time?");
              lcd.setCursor(1,1);
              lcd.print(workendhour);
              lcd.print("<:");
              lcd.print(workendmin);
              lcd.print(":");
              lcd.print(workendsec);
              digitalWrite(colPins[1], LOW);
              delay(500);    
              if(digitalRead(rowPins[0]) == LOW){
                workendhour = workendhour +1;
                if(workendhour == 24){workendhour = 0;}
                }
              digitalWrite(colPins[1], HIGH);              
              digitalWrite(colPins[1], LOW);
              delay(500);    
              if(digitalRead(rowPins[2]) == LOW){
                  workendhour = workendhour -1;
                  if(workendhour == -1){workendhour = 23;}
                }
              digitalWrite(colPins[1], HIGH);  
              digitalWrite(colPins[2], LOW);
              delay(500);    
              if(digitalRead(rowPins[1]) == LOW){
                workend123 = workend123 +1;
                if(workend123 == 4){workend123 = 1;}
                }
              digitalWrite(colPins[2], HIGH);  
              digitalWrite(colPins[0], LOW);
              delay(500);    
              if(digitalRead(rowPins[1]) == LOW){
                  workend123 = workend123 -1;
                  if(workend123 == 0){workend123 = 3;}
                }
              digitalWrite(colPins[0], HIGH);
                    digitalWrite(colPins[1], LOW);
                    delay(1000);
                    if(digitalRead(rowPins[1]) == LOW){
                      set123 = 0;
                      boom = 0;
                      workswitch = 0;
                    controlkey = 'H';
                    }
                    digitalWrite(colPins[1], HIGH);                    
                    digitalWrite(colPins[2], LOW);              
                    if(digitalRead(rowPins[0]) == LOW){
                    workswitch = 0;
                    }
                    digitalWrite(colPins[2], HIGH);                
                    break;
              case 2:                                   
              lcd.clear();
              lcd.setCursor(1,0);
              lcd.print("Work End Time?");
              lcd.setCursor(1,1);
              lcd.print(workendhour);
              lcd.print(":");
              lcd.print(workendmin);
              lcd.print("<:");
              lcd.print(workendsec);
              digitalWrite(colPins[1], LOW);
              delay(500);    
              if(digitalRead(rowPins[0]) == LOW){
                workendmin = workendmin +1;
                if(workendmin == 60){workendmin = 0;}
                }
              digitalWrite(colPins[1], HIGH);              
              digitalWrite(colPins[1], LOW);
              delay(500);    
              if(digitalRead(rowPins[2]) == LOW){
                  workendmin = workendmin -1;
                  if(workendmin == -1){workendmin = 59;}
                }
              digitalWrite(colPins[1], HIGH);  
              digitalWrite(colPins[2], LOW);
              delay(500);    
              if(digitalRead(rowPins[1]) == LOW){
                workend123 = workend123 +1;
                if(workend123 == 4){workend123 = 1;}
                }
              digitalWrite(colPins[2], HIGH);  
              digitalWrite(colPins[0], LOW);
              delay(500);    
              if(digitalRead(rowPins[1]) == LOW){
                  workend123 = workend123 -1;
                  if(workend123 == 0){workend123 = 3;}
                }
                    digitalWrite(colPins[1], LOW);
                    delay(1000);
                    if(digitalRead(rowPins[1]) == LOW){
                      set123 = 0;
                      boom = 0;
                      workswitch = 0;
                      controlkey = 'H';}   
                      digitalWrite(colPins[1], HIGH);   
                    digitalWrite(colPins[2], LOW);              
                    if(digitalRead(rowPins[0]) == LOW){
                    workswitch = 0;
                    }
                    digitalWrite(colPins[2], HIGH);                                     
                      break;
              case 3:
              lcd.clear();
              lcd.setCursor(1,0);
              lcd.print("Work End Time?");
              lcd.setCursor(1,1);
              lcd.print(workendhour);
              lcd.print(":");
              lcd.print(workendmin);
              lcd.print(":");
              lcd.print(workendsec);
              lcd.print("<");
              digitalWrite(colPins[1], LOW);
              delay(500);    
              if(digitalRead(rowPins[0]) == LOW){
                workendsec = workendsec +1;
                if(workendsec == 60){workendsec = 0;}
                }
              digitalWrite(colPins[1], HIGH);              
              digitalWrite(colPins[1], LOW);
              delay(500);    
              if(digitalRead(rowPins[2]) == LOW){
                  workendsec = workendsec -1;
                  if(workendsec == -1){workendsec = 59;}
                }
              digitalWrite(colPins[1], HIGH);  
              digitalWrite(colPins[2], LOW);
              delay(500);    
              if(digitalRead(rowPins[1]) == LOW){
                workend123 = workend123 +1;
                if(workend123 == 4){workend123 = 1;}
                }
              digitalWrite(colPins[2], HIGH);  
              digitalWrite(colPins[0], LOW);
              delay(500);    
              if(digitalRead(rowPins[1]) == LOW){
                  workend123 = workend123 -1;
                  if(workend123 == 0){workend123 = 3;}
                }
                    digitalWrite(colPins[1], LOW);
                    delay(1000);
                    if(digitalRead(rowPins[1]) == LOW){
                      set123 = 0;
                      boom = 0;
                      workswitch = 0;
                      controlkey = 'H';}
                      digitalWrite(colPins[1], HIGH);
                    digitalWrite(colPins[2], LOW);              
                    if(digitalRead(rowPins[0]) == LOW){
                    workswitch = 0;
                    }
                    digitalWrite(colPins[2], HIGH);                        
                      break;                                
            }
            }
            break;
            case 3:
            if(restswitch == 0){//휴식시작시각과 휴식종료시각 구분
            switch(restst123){//위에서 설명한 코드와 같은 구동방식이므로 이하 설명생략
              case 1:
              lcd.clear();
              lcd.setCursor(1,0);
              lcd.print("Rest Start Time?");
              lcd.setCursor(1,1);
              lcd.print(reststhour);
              lcd.print("<:");
              lcd.print(reststmin);
              lcd.print(":");
              lcd.print(reststsec);
              digitalWrite(colPins[1], LOW);
              delay(500);    
              if(digitalRead(rowPins[0]) == LOW){
                reststhour = reststhour +1;
                if(reststhour == 24){reststhour = 0;}
                }
              digitalWrite(colPins[1], HIGH);              
              digitalWrite(colPins[1], LOW);
              delay(500);    
              if(digitalRead(rowPins[2]) == LOW){
                  reststhour = reststhour -1;
                  if(reststhour == -1){reststhour = 23;}
                }
              digitalWrite(colPins[1], HIGH);  
              digitalWrite(colPins[2], LOW);
              delay(500);    
              if(digitalRead(rowPins[1]) == LOW){
                restst123 = restst123 +1;
                if(restst123 == 4){restst123 = 1;}
                }
              digitalWrite(colPins[2], HIGH);  
              digitalWrite(colPins[0], LOW);
              delay(500);    
              if(digitalRead(rowPins[1]) == LOW){
                  restst123 = restst123 -1;
                  if(restst123 == 0){restst123 = 3;}
                }
              digitalWrite(colPins[0], HIGH);
                    digitalWrite(colPins[1], LOW);
                    delay(1000);
                    if(digitalRead(rowPins[1]) == LOW){
                      restswitch = 1;
                    }
                    digitalWrite(colPins[1], HIGH);  
                    digitalWrite(colPins[2], LOW);              
                    if(digitalRead(rowPins[0]) == LOW){
                    set123 = 0;
                    boom = 1;
                    }
                    digitalWrite(colPins[2], HIGH);                  
                    break;
              case 2:                                   
              lcd.clear();
              lcd.setCursor(1,0);
              lcd.print("Rest Start Time?");
              lcd.setCursor(1,1);
              lcd.print(reststhour);
              lcd.print(":");
              lcd.print(reststmin);
              lcd.print("<:");
              lcd.print(reststsec);
              digitalWrite(colPins[1], LOW);
              delay(500);    
              if(digitalRead(rowPins[0]) == LOW){
                reststmin = reststmin +1;
                if(reststmin == 60){reststmin = 0;}
                }
              digitalWrite(colPins[1], HIGH);              
              digitalWrite(colPins[1], LOW);
              delay(500);    
              if(digitalRead(rowPins[2]) == LOW){
                  reststmin = reststmin -1;
                  if(reststmin == -1){reststmin = 59;}
                }
              digitalWrite(colPins[1], HIGH);  
              digitalWrite(colPins[2], LOW);
              delay(500);    
              if(digitalRead(rowPins[1]) == LOW){
                restst123 = restst123 +1;
                if(restst123 == 4){restst123 = 1;}
                }
              digitalWrite(colPins[2], HIGH);  
              digitalWrite(colPins[0], LOW);
              delay(500);    
              if(digitalRead(rowPins[1]) == LOW){
                  restst123 = restst123 -1;
                  if(restst123 == 0){restst123 = 3;}
                }
                    digitalWrite(colPins[1], LOW);
                    delay(1000);
                    if(digitalRead(rowPins[1]) == LOW){
                      restswitch = 1;
                      }
                      digitalWrite(colPins[1], HIGH);
                    digitalWrite(colPins[2], LOW);              
                    if(digitalRead(rowPins[0]) == LOW){
                    set123 = 0;
                    boom = 1;
                    }
                    digitalWrite(colPins[2], HIGH);                          
                      break;
              case 3:
              lcd.clear();
              lcd.setCursor(1,0);
              lcd.print("Rest Start Time?");
              lcd.setCursor(1,1);
              lcd.print(reststhour);
              lcd.print(":");
              lcd.print(reststmin);
              lcd.print(":");
              lcd.print(reststsec);
              lcd.print("<");
              digitalWrite(colPins[1], LOW);
              delay(500);    
              if(digitalRead(rowPins[0]) == LOW){
                reststsec = reststsec +1;
                if(reststsec == 60){reststsec = 0;}
                }
              digitalWrite(colPins[1], HIGH);              
              digitalWrite(colPins[1], LOW);
              delay(500);    
              if(digitalRead(rowPins[2]) == LOW){
                  reststsec = reststsec -1;
                  if(reststsec == -1){reststsec = 59;}
                }
              digitalWrite(colPins[1], HIGH);  
              digitalWrite(colPins[2], LOW);
              delay(500);    
              if(digitalRead(rowPins[1]) == LOW){
                restst123 = restst123 +1;
                if(restst123 == 4){restst123 = 1;}
                }
              digitalWrite(colPins[2], HIGH);  
              digitalWrite(colPins[0], LOW);
              delay(500);    
              if(digitalRead(rowPins[1]) == LOW){
                  restst123 = restst123 -1;
                  if(restst123 == 0){restst123 = 3;}
                }
                    digitalWrite(colPins[1], LOW);
                    delay(1000);
                    if(digitalRead(rowPins[1]) == LOW){
                      restswitch = 1;                  
                  }
                  digitalWrite(colPins[1], HIGH);
                    digitalWrite(colPins[2], LOW);              
                    if(digitalRead(rowPins[0]) == LOW){
                    set123 = 0;
                    boom = 1;
                    }
                    digitalWrite(colPins[2], HIGH);                      
                  break;                                
            }
            }
            if(restswitch == 1){//휴식종료시각설정화면
              switch(restend123){
              case 1:
              lcd.clear();
              lcd.setCursor(1,0);
              lcd.print("Rest End Time?");
              lcd.setCursor(1,1);
              lcd.print(restendhour);
              lcd.print("<:");
              lcd.print(restendmin);
              lcd.print(":");
              lcd.print(restendsec);
              digitalWrite(colPins[1], LOW);
              delay(500);    
              if(digitalRead(rowPins[0]) == LOW){
                restendhour = restendhour +1;
                if(restendhour == 24){restendhour = 0;}
                }
              digitalWrite(colPins[1], HIGH);              
              digitalWrite(colPins[1], LOW);
              delay(500);    
              if(digitalRead(rowPins[2]) == LOW){
                  restendhour = restendhour -1;
                  if(restendhour == -1){restendhour = 23;}
                }
              digitalWrite(colPins[1], HIGH);  
              digitalWrite(colPins[2], LOW);
              delay(500);    
              if(digitalRead(rowPins[1]) == LOW){
                restend123 = restend123 +1;
                if(restend123 == 4){restend123 = 1;}
                }
              digitalWrite(colPins[2], HIGH);  
              digitalWrite(colPins[0], LOW);
              delay(500);    
              if(digitalRead(rowPins[1]) == LOW){
                  restend123 = restend123 -1;
                  if(restend123 == 0){restend123 = 3;}
                }
              digitalWrite(colPins[0], HIGH);
                    digitalWrite(colPins[1], LOW);
                    delay(1000);
                    if(digitalRead(rowPins[1]) == LOW){
                      set123 = 0;
                      boom = 0;
                      restswitch = 0;
                    controlkey = 'H';
                    }
                    digitalWrite(colPins[1], HIGH);                    
                    digitalWrite(colPins[2], LOW);              
                    if(digitalRead(rowPins[0]) == LOW){
                    restswitch = 0;
                    }
                    digitalWrite(colPins[2], HIGH);                
                    break;
              case 2:                                   
              lcd.clear();
              lcd.setCursor(1,0);
              lcd.print("Rest End Time?");
              lcd.setCursor(1,1);
              lcd.print(restendhour);
              lcd.print(":");
              lcd.print(restendmin);
              lcd.print("<:");
              lcd.print(restendsec);
              digitalWrite(colPins[1], LOW);
              delay(500);    
              if(digitalRead(rowPins[0]) == LOW){
                restendmin = restendmin +1;
                if(restendmin == 60){restendmin = 0;}
                }
              digitalWrite(colPins[1], HIGH);              
              digitalWrite(colPins[1], LOW);
              delay(500);    
              if(digitalRead(rowPins[2]) == LOW){
                  restendmin = restendmin -1;
                  if(restendmin == -1){restendmin = 59;}
                }
              digitalWrite(colPins[1], HIGH);  
              digitalWrite(colPins[2], LOW);
              delay(500);    
              if(digitalRead(rowPins[1]) == LOW){
                restend123 = restend123 +1;
                if(restend123 == 4){restend123 = 1;}
                }
              digitalWrite(colPins[2], HIGH);  
              digitalWrite(colPins[0], LOW);
              delay(500);    
              if(digitalRead(rowPins[1]) == LOW){
                  restend123 = restend123 -1;
                  if(restend123 == 0){restend123 = 3;}
                }
                    digitalWrite(colPins[1], LOW);
                    delay(1000);
                    if(digitalRead(rowPins[1]) == LOW){
                      set123 = 0;
                      boom = 0;
                      restswitch = 0;
                      controlkey = 'H';}   
                      digitalWrite(colPins[1], HIGH);   
                    digitalWrite(colPins[2], LOW);              
                    if(digitalRead(rowPins[0]) == LOW){
                    restswitch = 0;
                    }
                    digitalWrite(colPins[2], HIGH);                                     
                      break;
              case 3:
              lcd.clear();
              lcd.setCursor(1,0);
              lcd.print("Rest End Time?");
              lcd.setCursor(1,1);
              lcd.print(restendhour);
              lcd.print(":");
              lcd.print(restendmin);
              lcd.print(":");
              lcd.print(restendsec);
              lcd.print("<");
              digitalWrite(colPins[1], LOW);
              delay(500);    
              if(digitalRead(rowPins[0]) == LOW){
                restendsec = restendsec +1;
                if(restendsec == 60){restendsec = 0;}
                }
              digitalWrite(colPins[1], HIGH);              
              digitalWrite(colPins[1], LOW);
              delay(500);    
              if(digitalRead(rowPins[2]) == LOW){
                  restendsec = restendsec -1;
                  if(restendsec == -1){restendsec = 59;}
                }
              digitalWrite(colPins[1], HIGH);  
              digitalWrite(colPins[2], LOW);
              delay(500);    
              if(digitalRead(rowPins[1]) == LOW){
                restend123 = restend123 +1;
                if(restend123 == 4){restend123 = 1;}
                }
              digitalWrite(colPins[2], HIGH);  
              digitalWrite(colPins[0], LOW);
              delay(500);    
              if(digitalRead(rowPins[1]) == LOW){
                  restend123 = restend123 -1;
                  if(restend123 == 0){restend123 = 3;}
                }
                    digitalWrite(colPins[1], LOW);
                    delay(1000);
                    if(digitalRead(rowPins[1]) == LOW){
                      set123 = 0;
                      boom = 0;
                      restswitch = 0;
                      controlkey = 'H';}
                      digitalWrite(colPins[1], HIGH);
                    digitalWrite(colPins[2], LOW);              
                    if(digitalRead(rowPins[0]) == LOW){
                    restswitch = 0;
                    }
                    digitalWrite(colPins[2], HIGH);                        
                      break;                                
            }
            }
            break;           
            }
            }
            if(myRTC.hours == alarmhour && myRTC.minutes == alarmmin &&
            myRTC.seconds == alarmsec){//알람시각이되면
              tone(SPEAKER, 262, 3000);
              delay(3000);//피에조부저를 통해 알람이 울린다!
              noTone(9);              
              }
              if((myRTC.hours >= worksthour) || (worksthour > workendhour && myRTC.hours < workendhour)){
                analogWrite(12,50);
                analogWrite(11, 180);
                analogWrite(10, 107);
                if((myRTC.hours >= reststhour) || (reststhour > restendhour && myRTC.hours < restendhour)){
                  analogWrite(12,255);
                  analogWrite(11,196);
                  analogWrite(10,137);                              
                  } 
                  if((myRTC.hours >= sleephour) || (sleephour > awakehour && myRTC.hours < awakehour)){
                    analogWrite(12,255);
                    analogWrite(11,1);
                    analogWrite(10,1);                              
                    }
                    
                    // 겹치는 시간대를 설정했다면 1수면 > 2휴식 > 3업무 순으로 RGBLED 출력의 우선권을 가진다!                                
              }
              if((myRTC.hours >= reststhour) || (reststhour > restendhour && myRTC.hours < restendhour)){
                analogWrite(12,255);
                analogWrite(11,196);
                analogWrite(10,137);                                 
                if((myRTC.hours >= sleephour) || (sleephour > awakehour && myRTC.hours < awakehour)){
                  analogWrite(12,255);
                  analogWrite(11,180);
                  analogWrite(10,107);                 
                  }
                  
              }
              if((myRTC.hours >= sleephour) || (sleephour > awakehour && myRTC.hours < awakehour)){
                analogWrite(12,255);
                analogWrite(11,1);
                analogWrite(10,1);
                                 
                }//우선순위설정을 위해 설정한 수면/업무/휴식 패턴에 맞는 RGBLED 출력 코드를 위와 같이 짰다
                                 
            lcd.clear();
      }
     
                  
