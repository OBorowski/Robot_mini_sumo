//biblioteki
#include <ArduinoBLE.h>
#include <Servo.h>

// definiowanie połączeń z pinami Arduino
// silnik lewy
const int dir1_L = 5;
const int dir2_L = 6;
const int speed_L = 16;

//silnik prawy
const int dir1_P = 9;
const int dir2_P = 10;
const int speed_P = 17;

// czujniki linii
const int Linia_L = 20;
const int Linia_P = 21;

//lampki


const int servo = 18;

//czujnik laserowy
//const int RX = 0;
//const int TX = 1;

//czujnik ultrasoniczny
const int TRIG = 3;
const int ECHO = 2;

// piny pwm - 3, 5, 6 , 9, 10
// piny analog - 13 - 21
// piny digital - 2 - 12

const int pomoc = 100;
Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

BLEService ledService("180A"); // Bluetooth® Low Energy LED Service

// Bluetooth® Low Energy LED Switch Characteristic - custom 128-bit UUID, read and writable by central
BLEByteCharacteristic switchCharacteristic("2A57", BLERead | BLEWrite);

//1000 = 1 sekunda

//do wyjebania, ale nie chce wyjebywac kodu w kosmos
void przod_L(){

  analogWrite(speed_L, 125); 
  digitalWrite(dir1_L, LOW);
  digitalWrite(dir2_L, HIGH);
  return;
}

void stop_L(){

  analogWrite(speed_L, 0); 
  digitalWrite(dir1_L, LOW);
  digitalWrite(dir2_L, HIGH);
  return;
}

void tyl_L(){

  analogWrite(speed_L, 125); 
  digitalWrite(dir1_L, HIGH);
  digitalWrite(dir2_L, LOW);
  return;
}

void przod_P(){
 
  analogWrite(speed_P, 125); 
  digitalWrite(dir1_P, LOW);
  digitalWrite(dir2_P, HIGH);
  return;
}

void stop_P(){

  analogWrite(speed_P, 0); 
  digitalWrite(dir1_P, LOW);
  digitalWrite(dir2_P, HIGH);
  return;
}

void tyl_P(){
 
  analogWrite(speed_P, 125); 
  digitalWrite(dir1_P, HIGH);
  digitalWrite(dir2_P, LOW);
  return;
}

 
void setup()
{
  Serial.begin(9600);  

  pinMode(dir1_L,OUTPUT);                   // tłumacząc z mądrego - zaczyna gadać z kompem
  pinMode(dir2_L,OUTPUT);
  pinMode(speed_L,OUTPUT);
  pinMode(dir1_P,OUTPUT);
  pinMode(dir2_P,OUTPUT);
  pinMode(speed_P,OUTPUT);       
  pinMode(Linia_L, INPUT);
  pinMode(Linia_P, INPUT);   
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object

  stop_L();
  stop_P();          
  Serial.println("Loading wpierdol...");
   

  pinMode(LED_BUILTIN, OUTPUT);    
    // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy failed!");

    while (1);
  }              




  pinMode(ECHO, INPUT);
  pinMode(TRIG, OUTPUT);

  // set advertised local name and service UUID:

  BLE.setLocalName("Czadowe chłopaki");
  BLE.setAdvertisedService(ledService);

  // add the characteristic to the service
  ledService.addCharacteristic(switchCharacteristic);

  // add service
  BLE.addService(ledService);

  // set the initial value for the characeristic:
  switchCharacteristic.writeValue(0);

  // start advertising
  BLE.advertise();

  Serial.println("BLE LED Peripheral");


  // Turn off the LEDs.
  // Those LEDs are active low.


while(1){
     // listen for BLE peripherals to connect:
  BLEDevice central = BLE.central();



  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());

    // while the central is still connected to peripheral:
    while (central.connected()) {
      // if the remote device wrote to the characteristic,

      // Wait until button is released.
      if (switchCharacteristic.written()) {
         Serial.println("a czy to dziala");
        // While waiting, show the status of the edge sensor for easy calibration.
        start_routine();
       
      }
    }
    return;
  }
  else{
    Serial.println("niepodloczon");
  }
}

  
  
  // Turn on the LEDs.

  Serial.println("koniec setupu");

  start_routine();
}
 

void test_wykrycia()
{
  
  Serial.print("Wykrycie obiektu: ");      //wyświetlanie wyników na ekranie w pętli co 500 ms
  if(pulseIn(ECHO, HIGH) > 0 && pulseIn(ECHO, HIGH) < 5800)                //stan niski oznacza wykrycie obiektu
  {                                        //stan wysoki natomiast - jego brak
    Serial.println("WYKRYCIE");  
    Serial.println(pulseIn(ECHO, HIGH)/58); 
    digitalWrite(LED_BUILTIN, HIGH);               // tak te linijki są z jakiegoś poradnika wyjebaneeeeeeeeeee
  }
  else
  {
    Serial.println("BRAK WYKRYCIA");
    Serial.println(pulseIn(ECHO, HIGH));
  }
  delay(1000); // czeka pół sekundy
}

void start_routine(){
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  delay(5000); //czeka 5 sekund

    Serial.println("po delayu");

  analogWrite(speed_L, 100); 
  digitalWrite(dir1_L, LOW);
  digitalWrite(dir2_L, HIGH);
  analogWrite(speed_P, 100); 
  digitalWrite(dir1_P, LOW);
  digitalWrite(dir2_P, HIGH);
  delay(500);

  analogWrite(speed_L, 60); 
  digitalWrite(dir1_L, LOW);
  digitalWrite(dir2_L, HIGH);
  analogWrite(speed_P, 120); 
  digitalWrite(dir1_P, LOW);
  digitalWrite(dir2_P, HIGH);
  delay(500); // powinno być 90 stopni

  analogWrite(speed_L, 0); 
  digitalWrite(dir1_L, LOW);
  digitalWrite(dir2_L, HIGH);
  analogWrite(speed_P, 0); 
  digitalWrite(dir1_P, LOW);
  digitalWrite(dir2_P, HIGH);


    // czasy trzeba będzie skonfigurować metodą prób i błędów
  /*
  uint32_t CzasOdStartu = millis();
  while (pulseIn(ECHO, HIGH) == 0) {
    // nazwa czujnika roboczo
    if (millis() - CzasOdStartu > 500) {
      break;
    }
    // skończy jeśli nie wykryje przeciwnika po 0,5s 
    // tbh troche po chuju ta funkcja, ale moze sie potem przyda
  }
  */
   BLEDevice central = BLE.central();
  while(central.connected()){
    loop();
    BLEDevice central = BLE.central();
  }
  while(!central.connect()){
      analogWrite(speed_L, 0); 
  digitalWrite(dir1_L, LOW);
  digitalWrite(dir2_L, HIGH);
  analogWrite(speed_P, 0); 
  digitalWrite(dir1_P, LOW);
  digitalWrite(dir2_P, HIGH);
  while(1);
  }
  

}
/*
void szukajp(){
  for(int i=0; i<50; i++){
    pattern1();
    sprawdz();
  }
  for(int i=0; i<50; i++){
    pattern2();
    sprawdz();
  }
}

// if(pulseIn(ECHO, HIGH) > 0)

void pattern1(){
  if(pulseIn(ECHO, HIGH) == 0) {
    przod_L();
    stop_P();
    delay(10);
  }
}

void pattern2(){
  if (pulseIn(ECHO, HIGH) == 0){
    przod_L();
    przod_P();
    delay(10);
  }
}
*/

void szukaj(){
  //UWAGA - to powinno runować w kółko wybierając jedną z opcji 
  // opcja - napisac mini funckje z delayem(10) jesli nie wykryje przeciwnika i 
  // zagniezdzic ja w wiekszej funkcji ktora zawiera konkretny pattern ruchu
  analogWrite(speed_L, 60); 
  digitalWrite(dir1_L, LOW);
  digitalWrite(dir2_L, HIGH);
  analogWrite(speed_P, 120); 
  digitalWrite(dir1_P, LOW);
  digitalWrite(dir2_P, HIGH);
  sprawdz();
  return;
}

void atak(){
    analogWrite(speed_L, 150); 
     digitalWrite(dir1_L, LOW);
     digitalWrite(dir2_L, HIGH);
    analogWrite(speed_P, 150); 
    digitalWrite(dir1_P, LOW);
    digitalWrite(dir2_P, HIGH);
    delay(250);
    if (pulseIn(ECHO, HIGH) == 0 || pulseIn(ECHO, HIGH) > 5799) {
      return;
    }
  // tylko na wprost, te sekcje sie rozszerzy
}

void sprawdz(){
  if (pulseIn(ECHO, HIGH) > 0 && pulseIn(ECHO, HIGH) < 5800) {
    Serial.println("dziala atak - sprawdz");
    //atak();
    return;
  }
  else if (!digitalRead(Linia_L)) {
    Serial.println("wykrycie linii lewej");
    linia(100);
    //return;
  }
  else if (!digitalRead(Linia_P)) {
    Serial.println("wykrycie linii prawej");
    linia(200);
    //return;
  }
  else {
    Serial.println("nic nie widzi");
    //return;
  }
}

void linia(uint8_t kierunek){

  analogWrite(speed_L, 0); 
  digitalWrite(dir1_L, LOW);
  digitalWrite(dir2_L, HIGH);
  analogWrite(speed_P, 0); 
  digitalWrite(dir1_P, LOW);
  digitalWrite(dir2_P, HIGH);
  delay(200);

  analogWrite(speed_L, 100); 
  digitalWrite(dir1_L, HIGH);
  digitalWrite(dir2_L, LOW);
  analogWrite(speed_P, 100); 
  digitalWrite(dir1_P, HIGH);
  digitalWrite(dir2_P, LOW);
  delay(300);

  if (kierunek == 100) {
    analogWrite(speed_L, 100); 
    digitalWrite(dir1_L, HIGH);
    digitalWrite(dir2_L, LOW);
    analogWrite(speed_P, 0); 
    digitalWrite(dir1_P, LOW);
    digitalWrite(dir2_P, HIGH);
    delay(600);
  }
  else {
    analogWrite(speed_L, 0); 
    digitalWrite(dir1_L, LOW);
    digitalWrite(dir2_L, HIGH);
    analogWrite(speed_P, 100); 
    digitalWrite(dir1_P, HIGH);
    digitalWrite(dir2_P, LOW);
    delay(600);
  }

  uint32_t czas = millis();
  while (millis() - czas < 200) {
      if (pulseIn(ECHO, HIGH) > 0 && pulseIn(ECHO, HIGH) < 5800) {
        return;
        //wraca do głównego loopa
      }
  }

  analogWrite(speed_L, 100); 
  digitalWrite(dir1_L, LOW);
  digitalWrite(dir2_L, HIGH);
  analogWrite(speed_P, 100  ); 
  digitalWrite(dir1_P, LOW);
  digitalWrite(dir2_P, HIGH);
  delay(300);

  return;

}

void loop() {
  // wykrywanie czujnikiem
  test_wykrycia(); 

  Serial.println("czy to dziala");

  szukaj();
  // wykrywanie linii
  /*
  Serial.println("dziala po setupie");

  if (!digitalRead(Linia_L)) {
    linia(100);
    Serial.println("linia lkewa");
  } 
  else if (!digitalRead(Linia_P)) {
    linia(200);
    Serial.println("dlinia prawa");
  }
  else {
    if (pulseIn(ECHO, HIGH) > 0) {
      szukaj();
      Serial.println("nie widzi");
      // szuka jeśli nie widzi nigdzie przeciwnika
    }
    else {
      atak();
      Serial.println("widzi");
    }
  }
  */

  // potrzeba breaka dla wykrycia linii i breaka dla wykrycia przeciwnika
  // stopowanie
  /*
  if (pulseIn(ECHO, HIGH) == 0) {
    stop_L();
    stop_P();

    digitalWrite(LED0, HIGH);

    // wieczny loop
    while(1);
  }
  */

}
