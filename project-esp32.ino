#define LED 15
hw_timer_t *My_timer = NULL;
int timer_cnt = 0;

void IRAM_ATTR onTimer() {
  timer_cnt++;
  if(timer_cnt>=10){
    digitalWrite(LED,!digitalRead(LED));
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
