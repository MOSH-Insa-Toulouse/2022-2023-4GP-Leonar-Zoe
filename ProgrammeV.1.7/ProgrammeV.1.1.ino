#include <SoftwareSerial.h>
#include <SPI.h>
#include <Adafruit_SSD1306.h>

#define nombreDePixelsEnLargeur 128                                                                           // Taille de l'écran OLED, en pixel, au niveau de sa largeur
#define nombreDePixelsEnHauteur 64                                                                            // Taille de l'écran OLED, en pixel, au niveau de sa hauteur
#define brocheResetOLED         -1                                                                            // Reset de l'OLED partagé avec l'Arduino (d'où la valeur à -1, et non un numéro de pin)
#define adresseI2CecranOLED     0x3C  
#define MCP_NOP 0b00000000
#define MCP_WRITE 0b00010001
#define MCP_SHTDWN 0b00100001
#define baudrate 9600

    //PINS//

#define txPin 8                                                                                               //Broche 11 en tant que RX, � raccorder sur TX du HC-05
#define rxPin 9                                                                                               //Broche 10 en tant que RX, � raccorder sur TX du HC-05
const int ssMCPin = 10;                                                                                       // Define the slave select for the digital pot 
const int pingPin = 5;                                                                                        // Trigger Pin of Ultrasonic Sensor
const int echoPin = 6;                                                                                        // Echo Pin of Ultrasonic Sensor
const int pinGraphite = 1;   

    //VARIABLES//

int Accueil = 0;
int dist;
int menu;
int valider = 0;
int in_menu = 0;
int i_R2;
long duration;
float R2 = 5*196.08;
char conversation[10] = {0};


    //Comms//

Adafruit_SSD1306 ecranOLED(nombreDePixelsEnLargeur, nombreDePixelsEnHauteur, &Wire, brocheResetOLED);
SoftwareSerial HC06(9, 8);                                   // RX, TX

    //Potentiomètre Digital//

void SPIWrite(uint8_t cmd, uint8_t data, uint8_t ssPin)
{
  SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
  digitalWrite(ssPin, LOW);
  SPI.transfer(cmd);
  SPI.transfer(data);
  digitalWrite(ssPin, HIGH);
  SPI.endTransaction();
}

    //Capteur Distance//

int Distance() {
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(pingPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  dist = (duration / 29 / 2);
  //Serial.println(dist);
  delay(300);
  return dist;
  }

    //Setup//

void setup(){
    //SRF05//
  pinMode(pingPin, OUTPUT);                                                                                    //SRF05 Initialisation
  pinMode(echoPin, INPUT);
  pinMode(txPin, OUTPUT);                                                                                      //Bluetooth Initialisation
  pinMode(rxPin, INPUT);
  digitalWrite(pingPin, LOW);
    //BAUDRATE//
  Serial.begin(baudrate);
  pinMode(rxPin,INPUT);
  pinMode(txPin,OUTPUT);
  pinMode (ssMCPin, OUTPUT); 
  digitalWrite(ssMCPin, HIGH); 
  SPI.begin();  
  HC06.begin(9600);                                                 // initialisation connexion série Bluetooth à 9600 bauds
  //mySerial.begin(baudrate);  //pour le Bluetooth
    //Ecran OLED//
  ecranOLED.begin(SSD1306_SWITCHCAPVCC, adresseI2CecranOLED);
}

void loop() {

  Distance();

  while(Accueil==0){
    ecranOLED.clearDisplay();
    ecranOLED.setCursor(0, 10);
    ecranOLED.setTextColor(SSD1306_WHITE);
    ecranOLED.print("Bonjour,");
    ecranOLED.setCursor(0, 30);
    ecranOLED.setTextColor(SSD1306_WHITE);
    ecranOLED.print("Veuillez placer");
    ecranOLED.setCursor(0, 40);
    ecranOLED.setTextColor(SSD1306_WHITE);
    ecranOLED.print("votre main devant");
    ecranOLED.setCursor(0, 50);
    ecranOLED.setTextColor(SSD1306_WHITE);
    ecranOLED.print("le capteur");
    ecranOLED.display(); 
    delay(100);
    Distance();
    Serial.println("bouh");
    if(dist < 10) {
      Accueil = 1;
    }
  } //fin if accueil

  // si le HC-06 transmet, on écrit le message dans le moniteur série
  while (HC06.available())                  
    {
    Serial.write(HC06.read());

  }
    // si on écrit dans le moniteur série, on transmet le message dans le module HC-06
  while (Serial.available())
    {
    HC06.write(Serial.read());
  }

  if(valider == 1){
    if(menu == 1){  //Modification de la valeur de R2
        i_R2 = 0; 
        delay(100);

        while(dist < 40 || valider == 1){
            Distance();

            if (dist < 20) {
              i_R2++;
            }

            if (dist > 20) {
              i_R2--;        
            }

            ecranOLED.clearDisplay();
            ecranOLED.setCursor(0, 30);
            ecranOLED.print("R2 vaut ");
            ecranOLED.print(i_R2*196.08);
            ecranOLED.display();
            ecranOLED.setCursor(0, 30);
            delay(500);
            Distance();
          if (dist > 40) {
            valider = -1;
          }

        } //fin while

        R2 = (float)i_R2*196.08;
        //conversation[10] = (char)R2;
        delay(300);
        HC06.write("R2 set\n");  //pas mettre de phrases trop longues... 
        //dtostrf(R2, 10, 2, conversation); //ça fait planter le prgm, c'est un pb....
        //HC06.write(R2);
        SPIWrite(MCP_WRITE, i_R2, ssMCPin);

       } //fin menu 1
    
       
      else if(menu == 2){  //Lecteur de Resistance du Capteur de Graphite
        while (valider == 1) {
          int sensorValue = analogRead(pinGraphite);
          float tension_graphite = (float)sensorValue * 5.0/1023.0;
          float Rc = (((100000.0+R2)*100000.0*4.31)/(R2*tension_graphite)) - 10000.0 - 100000.0;

          Serial.println(Rc);
          ecranOLED.clearDisplay();
          ecranOLED.setCursor(0, 20);
          ecranOLED.print("La Resistance du");
          ecranOLED.setCursor(0, 30);
          ecranOLED.print("Capteur de Graphite");
          ecranOLED.setCursor(0, 40);
          ecranOLED.print("vaut :");
          ecranOLED.print(Rc/1000000);
          ecranOLED.println("MOhm");
          ecranOLED.display();
          ecranOLED.setCursor(0, 30);

          HC06.print(Rc/1000000);

          uint8_t sensorBT = sensorValue/4;

          delay(100);
          Distance();
          if (dist < 10) {
                valider = -1; //retour au menu principal
              }
        }
      } //fin menu 2


      else if (menu ==  3){   //Lecture de la Resistance du Flex
        while (valider == 1) {

          int BINVoltflex = analogRead(A0);
          float Voltflex = BINVoltflex * (4.99/1023.0);
          float Resflex = 39000*(4.99/Voltflex-1);

          float Degflex = map(Resflex, 35000, 125000, 0, 180.0);

          //int felixsensor = analogRead(A0);
          //float sensorfelix = (39000.0*4.99)/(4.99 - (float)felixsensor*(5.0/1023.0));

          ecranOLED.clearDisplay();
          ecranOLED.setCursor(0, 10);
          ecranOLED.print("Rflex vaut :");
          ecranOLED.println(Resflex);  
          ecranOLED.setCursor(0, 30);
          //ecranOLED.println("Il a une courbure de :");  //ne fonctionne pas pour aucune raison...............
          ecranOLED.setCursor(0, 50);
          ecranOLED.print(Degflex);
          //ecranOLED.print("Degres");
          ecranOLED.display();

          delay(100);
          Distance();
          if (dist < 10) {
                valider = -1; //retour au menu principal
              }
        } //fin menu 3 
      }

        else{
          Serial.println("erreur 404 not found"); 
        }
      
  }
  

    else {                                                                                                       //code du menu 
    Serial.println("Menu Principal"); 
    Serial.println(menu);
    ecranOLED.clearDisplay();
    ecranOLED.setCursor(0, 10);
    if (menu == 1) {ecranOLED.setTextColor(SSD1306_BLACK, SSD1306_WHITE);} 
    ecranOLED.print("Changer valeur R2\n");
    if (menu == 1) {ecranOLED.setTextColor(SSD1306_WHITE, SSD1306_BLACK);}
    if (menu == 2) {ecranOLED.setTextColor(SSD1306_BLACK, SSD1306_WHITE);}
    ecranOLED.print("Capteur Graphite\n");
    if (menu == 2) {ecranOLED.setTextColor(SSD1306_WHITE, SSD1306_BLACK);}
    if (menu == 3) {ecranOLED.setTextColor(SSD1306_BLACK, SSD1306_WHITE);}
    ecranOLED.print("Flex Sensor\n");
    if (menu == 3) {ecranOLED.setTextColor(SSD1306_WHITE, SSD1306_BLACK);}
    ecranOLED.setCursor(0, 50);
    ecranOLED.print("R2 vaut ");
    ecranOLED.print(R2);
    ecranOLED.display(); 
      
  
     if (Accueil == 1) {
        if(dist < 10){
          menu = 1;
          valider = 0;
        }
        else if(dist < 20 && dist > 10){
          menu = 2;
          valider = 0;

        }
        else if(dist < 35 && dist > 20){
          menu = 3;
          valider = 0;
        }     
       } //fin if distance 

    if(dist > 40 && Accueil == 1 && valider != -1){
      valider = 1;
    }

    delay(50); 
  }

} //fin loop





























