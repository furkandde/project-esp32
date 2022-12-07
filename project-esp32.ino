// NodeMCU-32S 

#define LED 15
#define BUTTON 27
hw_timer_t *My_timer = NULL;
unsigned int timer_cnt = 0;      // for time counting
unsigned int led_onTime = 1000;  // default time 1000ms=1s
unsigned int led_offTime = 1000;
unsigned int adj_led_onTime = 1000;
unsigned int adj_led_offTime = 1000;
String str;
bool echo_task = false;
bool button_pressed = false;
unsigned int baud_rate[4] = { 9600, 19200, 38400, 115200 };
unsigned int baud_cnt = 0;
unsigned long lastDebounceTime = 0; // for the button

void IRAM_ATTR Timer() {  // timer interrupt

  timer_cnt++;

  if ((timer_cnt * 100 >= led_onTime) && digitalRead(LED)) {

    digitalWrite(LED, LOW);
    timer_cnt = 0;
  }
  if ((timer_cnt * 100 >= led_offTime) && !digitalRead(LED)) {

    digitalWrite(LED, HIGH);
    timer_cnt = 0;
  }
}
void IRAM_ATTR Button() { //button interrupt



  if ((millis() - lastDebounceTime) > 50) {

    button_pressed = true;
    baud_cnt++;

    if (baud_cnt > 3) {
      baud_cnt = 0;
    }

    lastDebounceTime = millis();
  }
}
void Uart(String str) {                    // uart function
  if (str == "start\n" && !(echo_task)) {  // checking the entered string

    echo_task = true;
    Serial.println("Echo task started.");
    led_onTime = adj_led_onTime;
    led_offTime = adj_led_offTime;

  } else if (str == "stop\n" && (echo_task)) {  // checking the entered string

    echo_task = false;
    Serial.println("Echo task stoped.");
    led_onTime = 1000;
    led_offTime = 1000;

  } else {
    if (str[0] == 'l' && str[1] == 'e' && str[2] == 'd') {  //checking and adjustment the entered ledon-off time

      if (str[3] == 'o' && str[4] == 'n' && str[5] == '=' && isdigit(str[6]) && isdigit(str[7]) && isdigit(str[8])) {

        int num1 = str[6] - '0';
        int num2 = str[7] - '0';
        int num3 = str[8] - '0';
        adj_led_onTime = num1 * 100 + num2 * 10 + num3 * 1;
        led_onTime = adj_led_onTime;

      } else if (str[3] == 'o' && str[4] == 'f' && str[5] == 'f' && str[6] == '=' && isdigit(str[7]) && isdigit(str[8]) && isdigit(str[9])) {

        int num1 = str[7] - '0';
        int num2 = str[8] - '0';
        int num3 = str[9] - '0';
        adj_led_offTime = num1 * 100 + num2 * 10 + num3 * 1;
        led_offTime = adj_led_offTime;

      } else {

        Serial.println("Use 'ledon=xxx' or 'ledoff=xxx' ");
      }
    }
  }
}
void setup() {

  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);

  My_timer = timerBegin(0, 80, true);            // timer0, 12.5ns* 80 -> 1000ns = 1us
  timerAttachInterrupt(My_timer, &Timer, true);  // edge triggered
  timerAlarmWrite(My_timer, 100000, true);       // 100000 * 1us = 100ms
  timerAlarmEnable(My_timer);                    // just Enable

  pinMode(BUTTON, INPUT_PULLUP);
  attachInterrupt(BUTTON, Button, RISING);

  Serial.println("Enter 'start' or 'stop' for echo task; 'ledon=xxx' or 'ledoff=xxx' for adj led on-off time");

}
void loop() { //main
  if (Serial.available() > 0) {

    str = Serial.readString();

    if (echo_task) {

      Serial.println(str);
    }

    Uart(str);

  }
  if (button_pressed) {

    Serial.end();
    Serial.begin(baud_rate[baud_cnt]);
    Serial.print("Baud Rate: ");
    Serial.println(baud_rate[baud_cnt]);
    button_pressed = false;

  }
}
