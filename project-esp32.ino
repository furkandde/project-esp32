#define LED 15
hw_timer_t *My_timer = NULL;
int timer_cnt = 0; // for time counting
int led_on_time = 1000; // default time 1000ms=1s
int led_off_time = 1000; 
int adj_led_on_time = 1000; 
int adj_led_off_time = 1000; 
String str; 
bool echo_task = false;

void IRAM_ATTR onTimer() { //timer interrupt

  timer_cnt++;
  
  if ((timer_cnt * 100 >= led_on_time) && digitalRead(LED) ) {

    digitalWrite(LED, LOW);
    timer_cnt = 0;
  }
  if ((timer_cnt * 100 >= led_off_time) && !digitalRead(LED)) {

    digitalWrite(LED, HIGH);
    timer_cnt = 0;
  }
}
void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);

  My_timer = timerBegin(0, 40, true);  // timer0, 12.5ns* 80 -> 1000ns = 1us
  timerAttachInterrupt(My_timer, &onTimer, true); // edge triggered
  timerAlarmWrite(My_timer, 100000, true); //100000 * 1us = 100ms
  timerAlarmEnable(My_timer); //Just Enable

    Serial.println("Enter 'start' or 'stop' for echo task; 'ledon=xxx' or 'ledoff=xxx' for adj led on-off time");
}
void loop() {
  if (Serial.available() > 0) {

    str = Serial.readString();
    
    if (str == "start\n") { //checking the entered string

      echo_task = true;
      Serial.println("Echo task started.");
      led_on_time = adj_led_on_time;
      led_off_time = adj_led_off_time;

    }else if (str == "stop\n") { //checking the entered string

      echo_task = false;
      Serial.println("Echo task stoped.");
      led_on_time = 1000;
      led_off_time = 1000;

    }else{

      if(echo_task){ //echo task

        Serial.println(str);

      }
      if(str[0] == 'l' && str[1] == 'e' && str[2] == 'd'){ //checking and adjustment the entered ledon-off time

        if(str[3] == 'o' && str[4] == 'n' && str[5] == '=' && isdigit(str[6]) && isdigit(str[7]) && isdigit(str[8])){

          int num1 = str[6] - '0';
          int num2 = str[7] - '0';
          int num3 = str[8] - '0';
          adj_led_on_time = num1*100 + num2*10 + num3*1;
          led_on_time = adj_led_on_time;

        }else if(str[3] == 'o' && str[4] == 'f' && str[5] == 'f' && str[6] == '='  && isdigit(str[7]) && isdigit(str[8]) && isdigit(str[9])){

          int num1 = str[7] - '0';
          int num2 = str[8] - '0';
          int num3 = str[9] - '0';
          adj_led_off_time = num1*100 + num2*10 + num3*1;
          led_off_time = adj_led_off_time;

        }else{

          Serial.println("Use 'ledon=xxx' or 'ledoff=xxx' ");

        }       
      }
    }
  }
}


