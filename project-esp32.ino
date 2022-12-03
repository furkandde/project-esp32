#define LED 15
hw_timer_t *My_timer = NULL;
int timer_cnt = 0; // for time counting
int led_on_time = 1000; //1000ms=1s
int led_off_time = 1000; //1000ms=1s

void IRAM_ATTR onTimer() {
  timer_cnt++;
  // led on-off according to the given time value
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
  My_timer = timerBegin(0, 80, true);  // timer0, 12.5ns* 80 -> 1000ns = 1us
  timerAttachInterrupt(My_timer, &onTimer, true); // edge triggered
  timerAlarmWrite(My_timer, 100000, true); //100000 * 1us = 100ms
  timerAlarmEnable(My_timer); //Just Enable
}

void loop() {

}
