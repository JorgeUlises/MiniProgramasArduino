int Out = 11;//Pin de salida de la señal codificación crc
int In = 12;//Pin de entrada de la codificación
int Exit = 13;//Pin de salida del voltaje para el led
int sync = 10;//Señal de sincronismo para el osciloscopio
byte dato1 = B10101010;//Datos para ser codificados
byte dato2 = B10101010;
byte dato3 = B10101010;
byte dato4 = B10100000;//los últimos 4 bits deben ser ceros, n=4
byte dato[]={dato1, dato2, dato3, dato4};

byte data1 = B00000000;//Datos del divisor o polinomio del generador
byte data2 = B00000000;
byte data3 = B00000000;
byte data4 = B00010011;//X^4+X+1
byte data[]={data1, data2, data3, data4};

unsigned long enviado;//Dato que se envía como dato a 32 bits
unsigned long enviadomod;//Dato restándo el residuo de la división
unsigned long recibido;//Dato recibido después de la comunicación
unsigned long divisor;//Generador, polinomio
unsigned long residuo1;//Resultante de la división de la tupla con ceros
unsigned long residuo2;//Resultante de la división de recibido
int pin = 2;//Pin usado para la interrupción
volatile int state = LOW;//Estado inicial que demarca la interrupción
//volatile int state = HIGH;

void setup() {
Serial.begin(9600);//Configuración del flujo serial a 9600 baudios
pinMode(Out, OUTPUT);//Configuración de todos los pines como entrada o salida
pinMode(In, INPUT);
pinMode(Exit, OUTPUT);
pinMode(pin,INPUT);
pinMode(sync,OUTPUT);
digitalWrite(pin, HIGH);//Configuración del pin con resistencia de pullup
attachInterrupt(0, cambio, CHANGE);
}

void loop() {
  crc();
}

void crc(){
//Secuencia para generar el flujo aleatorio
/*
for (int i=0; i < 4; i++){
  dato[i]=random(255);
}
*/
//Se hace una AND para eliminar los cuatro bits del dato enviado
//o sea, indiferente del dato, se pone en cero (clear) los bits
//0, 1, 2 y 3 de la tupla a enviar.
byte variable=B11110000;//son solo 4 ceros n=4?
dato[3]=dato[3] & variable;
//Se escriben los datos bit a bit. Se pasa el arreglo dato a la 
//variable enviado que es de 32 bits sin signo
int cont=0;
for (int i=0; i < 4; i++){
  for (int j=0; j < 8; j++){
    bitWrite(enviado,cont,bitRead(dato[3-i],j));
    cont++;
  }
}
//Se escribe el divisor o generador, de la misma forma que 
//el anterior, pero con el arreglo data
cont=0;
for (int i=0; i < 4; i++){
  for (int j=0; j < 8; j++){
    bitWrite(divisor,cont,bitRead(data[3-i],j));
    cont++;
  }
}
//Secuencia para imprimir el dato que quiere ser enviado
Serial.println("Enviado");
Serial.println(enviado);
imprimir(enviado);
//Secuencia para imprimir el divisor y revisar que si está bien
/*
Serial.println("Divisor");
Serial.println(divisor);
imprimir(divisor);
*/
//Se establece el residuo con la división entre el enviado y el divisor
//y posteriormente se imprime el dato por el puerto serial
residuo1=enviado % divisor;
Serial.println("Residuo #1");
Serial.println(residuo1);
imprimir(residuo1);
//Puede eliminarse, estaba mal establecido desde la teoría
/*
enviadomod=enviado;
for (int i=0; i < 4; i++){
bitWrite(enviadomod,i,bitRead(residuo1,i));
}*/
//Se pone el dato que se va a enviar, pero antes se debe restar
//el residuo para que funcione el algoritmo detector de error
//Posteriormente se imprime su valor por el puerto serial
enviadomod=enviadomod-residuo1;
Serial.println("Enviado modificado");
Serial.println(enviadomod);
imprimir(enviadomod);
//Esta rutina detecta si ha habido cambio en la variable estado
//debido a que ha entrado a la interrupción... 
//Se cambia el bit 5 en este caso, para que el dato se corrompa.
//Se corrompe el bit antes de ser enviado, aunque podría corromperse
//después de ser enviado, en la variable recibido, pero se prefiere
//así, ya que ilustra mejor el concepto de la corrección anterior a
//la lectura del dato.
if(state==HIGH){
  if(bitRead(enviadomod,5)==1){
    bitWrite(enviadomod,5,0);
  } else {
    bitWrite(enviadomod,5,1);
  }
}
//Se pone en 1 la señal de sincronismo (set)
digitalWrite(sync,HIGH);
//Se imprimen los datos por un pin en este caso Out
//y se lee lo que hay en el pin In y se escribe en la 
//variable recibido. Se espera un tiempo para que se pueda
//ver mas claramente en el osciloscopio
for (int i=0; i < 32; i++){
  digitalWrite(Out,bitRead(enviadomod,i));
  bitWrite(recibido,i,digitalRead(In));
  delay(10);
}
//Una vez terminado el trabajo, la señal de sincronizmo 
//se pone en cero (clear)
digitalWrite(sync,LOW);
//Se imprime el valor del dato recibido
Serial.println("Recibido");
Serial.println(recibido);
imprimir(recibido);
//Se hace la división para saber si el residuo da cero 
//(el dato llegó correctamente) o si el residuo da diferente de cero
//(el dato llegó corrupto), luego se imprime su valor.
residuo2=recibido%divisor;
Serial.println("Residuo #2");
Serial.println(residuo2);
imprimir(residuo2);
Serial.print("\n");
//Si el residuo es cero, dato OK, no se prende el led
//Si el residuo es diferente de cero, dato erroneo,
//se enciende el led conectado en el pin Exit
if(residuo2==0) {//no hay error
  digitalWrite(Exit,0);
} else {
  digitalWrite(Exit,1);
}
//Se espera un tiempo antes de enviar la siguiente trama
//o tren de impulsos.
delay(10);
}

//Funció a la que se entra cuando se genera la interrupción
void cambio(){
  if(digitalRead(2)==HIGH){
    ponererror();
  } else {
    quitarerror();
  }
}

void ponererror(){
state=HIGH;
}

void quitarerror(){
state=LOW;
}

//Esta función imprime los datos al puerto serial, bit a bit
void imprimir(unsigned long entrada){
for (int i=0; i < 32; i++){
  Serial.print(bitRead(entrada,31-i));
}
Serial.print("\n");
}
