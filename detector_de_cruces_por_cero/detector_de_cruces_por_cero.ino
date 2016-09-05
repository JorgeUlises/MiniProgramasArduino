int sal= 13;
volatile int state = LOW;
int pinEntrada = 0;
void setup()
{
  pinMode(sal, OUTPUT);
  attachInterrupt(pinEntrada, blink, RISING);
}

void loop(){
  if(state){
  state = !state;
  /*attachInterrupt(5,int5,RISING);*/
  digitalWrite(sal, HIGH);
  delay(50);
  //delayMicroseconds(600);
  //delayMicroseconds(periodo1); // Approximately 10% duty cycle @ 1KHz
  digitalWrite(sal, LOW);
  //delay(16);
  //delayMicroseconds(periodo2);
  }
}

void blink()
{
  state = !state;
}
