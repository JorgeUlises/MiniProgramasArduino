int sal= 13;
int periodo1=1;/*arriba en milisegundos*/
int periodo2=9;/*abajo*/
void setup()
{
  pinMode(sal, OUTPUT);
}

void loop()
{
  /*attachInterrupt(5,int5,RISING);*/
  digitalWrite(sal, HIGH);
  //delayMicroseconds(periodo1); // Approximately 10% duty cycle @ 1KHz
  digitalWrite(sal, LOW);
  delay(16);
  //delayMicroseconds(periodo2);
}
