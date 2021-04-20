/*
                                                     @@@@@@@@@@@@@@@                                                    
                                                    @@@@@@@@@@@@@@@@                                                    
                                                    @@@@@@@@@@@@@@@@@                                                   
                                  @@@@@@@@       @@@@@@@@@@@@@@@@@@@@@@@      @@@@@@@@                                  
                               @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                               
                              @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                             
                            @@@@@@@@@@@@@@@@@@@@@@                     @@@@@@@@@@@@@@@@@@@@@                            
                             @@@@@@@@@@@@@@@@@                             @@@@@@@@@@@@@@@@                             
                               @@@@@@@@@@@@                                   @@@@@@@@@@@@                              
                                @@@@@@@@@          @@@@@@@@@@@@@@@@@            @@@@@@@@@                               
                               @@@@@@@@          @@@@@@@@@@@@@@@@@@@@@            @@@@@@@@                              
                              @@@@@@@@          @@@@@             @@@@@            @@@@@@@@          @@@@@@@@@@@@@@@@@  
  @@@@@@@@@@@@@@@@    @@@@@@@@@@@@@@@           @@@@@             @@@@@             @@@@@@@@@@@@@@   @@@@@@@@@@@@@@@@@@@
 @@@@@@@@@@@@@@@@@@  @@@@@@@@@@@@@@@            @@@@@             @@@@@              @@@@@@@@@@@@@@@                @@@@
 @@@@          @@@@  @@@@@@@@@@@@@@@            @@@@@             @@@@@              @@@@@@@@@@@@@@@ @@@@@@@@@@@@@@@@@@@
 @@@@@@@@@@@@@@@@@@  @@@@@@@@@@@@@@@            @@@@@             @@@@@              @@@@@@@@@@@@@@@ @@@@@@@@@@@@@@@@@@@
 @@@@                @@@@@@@@@@@@@@@            @@@@@@@@@@@@@@@@@@@@@@@              @@@@@@@@@@@@@@@ @@@@@          @@@@
 @@@@@@@@@@@@@@@@@@  @@@@@@@@@@@@@@@              @@@@@@@@@@@@@@@@@@@@@             @@@@@@@@@@@@@@@   @@@@@@@@@@@@@@@@@@
  @@@@@@@@@@@@@@@@@     @@@@@@@@@@@@@                             @@@@@             @@@@@@@@@@@@                        
                              @@@@@@@@                            @@@@@            @@@@@@@@                             
                               @@@@@@@@             @@@@@@@@@@@@@@@@@@            @@@@@@@@                              
                                @@@@@@@@@           @@@@@@@@@@@@@@@@            @@@@@@@@                                
                               @@@@@@@@@@@@                                   @@@@@@@@@@@@                              
                             @@@@@@@@@@@@@@@@@                             @@@@@@@@@@@@@@@@                             
                            @@@@@@@@@@@@@@@@@@@@@@                    @@@@@@@@@@@@@@@@@@@@@@                            
                              @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                             
                                @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                               
                                   @@@@@@@        @@@@@@@@@@@@@@@@@@@@@        @@@@@@@                                  
                                                    @@@@@@@@@@@@@@@@@                                                   
                                                    @@@@@@@@@@@@@@@@                                                    
                                                     @@@@@@@@@@@@@@@    
                                               Réaliser par Egalistel & Karim 

Retrouvez les inofrmations du code sur le site : http://egamaker.be/egametric-affichage-pour-octoprint
La video sur la chaine : https://www.youtube.com/c/Egalistel
                                                                                                     
 */
#include <FS.h> 
#include <OctoPrintAPI.h> //This is where the magic happens... shazam!
#include <Adafruit_GFX.h>        //  https://github.com/adafruit/Adafruit-GFX-Library
#include <Fonts/TomThumb.h>
#include <Adafruit_NeoMatrix.h>    //  https://github.com/adafruit/Adafruit_NeoMatrix
#include <Adafruit_NeoPixel.h>    // https://github.com/adafruit/Adafruit_NeoPixel
#include <NTPClient.h>
#include <WiFiUdp.h>

#include <Time.h>         //We will need these two just to do some rough time math on the timestamps we get
#include <TimeLib.h>

#include <ESP8266WiFi.h>  
#include <WiFiClient.h>
#include <WiFiManager.h>

WiFiClient client;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 7200, 60000);  // Horloge Mondial qui prends le serveur le plus proche, mais vous pouvez forcer une zone en ajoutant devant pool un des serveur suivant (asia. , europe. , north-america., oceania.,south-america.)
// pour le décalage horraire , calculez par 3600 pour +1 et 7200 pour +2 
int h = 8;   // Hauteur de la matrice
int w = 32;  // Largeur de la matrice

#define PIN 2 // D4 Sur l'esp8266

// DÉCLARATION DE MATRICE:
// Paramètre 1 = largeur de la matrice NeoPixel
// Paramètre 2 = hauteur de la matrice
// Paramètre 3 = numéro de broche (la plupart sont valides)
// Paramètre 4 = drapeaux de disposition de la matrice, additionnez-les si nécessaire:
// NEO_MATRIX_TOP, NEO_MATRIX_BOTTOM, NEO_MATRIX_LEFT, NEO_MATRIX_RIGHT:
// Position de la PREMIÈRE LED dans la matrice; choisissez-en deux, par exemple
// NEO_MATRIX_TOP + NEO_MATRIX_LEFT pour le coin supérieur gauche.
// NEO_MATRIX_ROWS, NEO_MATRIX_COLUMNS: les LED sont disposées horizontalement
// lignes ou en colonnes verticales, respectivement; choisissez l'un ou l'autre.
// NEO_MATRIX_PROGRESSIVE, NEO_MATRIX_ZIGZAG: toutes les lignes / colonnes continuent
// dans le même ordre ou dans le sens inverse des lignes alternées; choisissez-en un.
// Voir l'exemple ci-dessous pour ces valeurs en action.
// Paramètre 5 = indicateurs de type pixel, additionnez-les si nécessaire:
// NEO_KHZ800 Bitstream 800 KHz (la plupart des produits NeoPixel avec LED WS2812)
// NEO_KHZ400 400 KHz (pixels FLORA classiques 'v1' (pas v2), pilotes WS2811)
// Les pixels NEO_GRB sont câblés pour le bitstream GRB (la plupart des produits NeoPixel)
// Les pixels NEO_RGB sont câblés pour le flux binaire RVB (pixels FLORA v1, pas v2)


// fertiges Panel 8*32
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(w, h, PIN,
NEO_MATRIX_BOTTOM    + NEO_MATRIX_RIGHT +
NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
NEO_GRB            + NEO_KHZ800);


//------------ Configuration Octoprint-------------------//  
IPAddress ip(192, 168, 1, 19);                         // Adresse Ip de votre serveur Octoprint Local
// char* octoprint_host = "octoprint.example.com";  // Ou adresse de votre serveur pour connection externe

const int octoprint_httpPort = 80;  //Port du serveur , 80 par defaut en Local
String octoprint_apikey = "C87BAF832D2D447599987CCB6020479C"; //Clef de votre API , dispo sur octoprint dans setting - features - API

OctoprintApi api(client, ip, octoprint_httpPort, octoprint_apikey);               //Si vous utilisez une adresse ip
// OctoprintApi api(client, octoprint_host, octoprint_httpPort, octoprint_apikey);//Ou si vous utilisez une adresse externe

//-------------------------------------------------------//  




//----------------Liste des couleurs---------------------// 
const uint16_t colors[] = {  // les couleurs de texte en RGB 
  matrix.Color(255, 255, 255),
  matrix.Color(0, 255, 0),
  matrix.Color(0, 0, 255),
  matrix.Color(255, 0, 255),
  matrix.Color(0, 255, 255),
  matrix.Color(255, 255, 0),
  matrix.Color(255, 0, 0),
  matrix.Color(255, 127, 127)
  
};

int x    = 0;
String subs = "U30";
String heure= "";
int channel = 0;
const int api1 = 0;
const int api2 = 1;
const int api3 = 2;
const int api4 = 3;
const int NumOfChannels = 4;
int col=1;   // couleur du texte par défaut (Vert) 

void setup() {
  Serial.begin(115200);
  Serial.println("Démarrage de l'Egamétric...");
  matrix.begin();
  matrix.setTextSize(0);
  matrix.setTextWrap(true);
  matrix.setBrightness(10);                  // reglage de luninosité de la matrice
  matrix.setFont(&TomThumb);
  matrix.setTextColor(colors[3]);

  timeClient.begin();

  
  Serial.println("Config WiFi...");
  WiFiManager wifiManager;
  wifiManager.autoConnect("Egamétric"); // nom à chercher sur votre wifi pour configurer l'esp la premiere fois
  Serial.println("WIFI connecté");

  Serial.print("Local IP :");
  Serial.println(WiFi.localIP());

  x = matrix.width();
  subs = "U30"; // Texte au lancement 
}

//--------------Réglage Scrool-----------------//  
int stopRow = 0;  // valeur par défaut du scroll pour le premier texte 
const int scrollDelay = 40;  // vitesse du scroll , plus il est petit , plus il passera rapidement en mms
const int showDelay = 2500;  // temps entre chaque affichage en mms




void loop() {
    
  matrix.fillScreen(0);
  matrix.setCursor(10, 7);

  if (--x < stopRow) x = matrix.width();

  
  matrix.setTextColor(colors[col]);
  matrix.print(subs);
   
  if (channel == api1)
    printapi1Logo();
    matrix.print(subs);
  if (channel == api2)
    printapi2Logo();
    matrix.print(subs);
  if (channel == api3)  
    printapi3Logo();
    matrix.print(subs);
  if (channel == api4)  
    printapi4Logo();
    matrix.print(subs);
  
  matrix.show();

  if (x == matrix.width()) {
    delay(showDelay);

    channel++;
    if (channel == NumOfChannels) channel = 0;

    if (channel == api1)
      subs = A1counter();
    
    else if (channel == api2)
      subs = A2counter();
   
    else if (channel == api3)
      subs = A3counter();  

   else if (channel == api4)
      subs = A4counter();  
    
  } else
    delay(scrollDelay);
}

void printapi1Logo() { //Sablier temps
  matrix.drawLine(0, 0, 7, 0, matrix.Color(58, 137, 35));
  matrix.drawLine(0, 7, 7, 7, matrix.Color(58, 137, 35));
  matrix.drawPixel(0, 1, matrix.Color(58, 137, 35));
  matrix.drawPixel(3, 1, matrix.Color(58, 137, 35));
  matrix.drawPixel(4, 1, matrix.Color(58, 137, 35));
  matrix.drawPixel(7, 1, matrix.Color(58, 137, 35));
  matrix.drawPixel (1, 2, matrix.Color(58, 137, 35));
  matrix.drawPixel (6, 2, matrix.Color(58, 137, 35));
  matrix.drawPixel (2, 3, matrix.Color(58, 137, 35));
  matrix.drawPixel (5, 3, matrix.Color(58, 137, 35));
  matrix.drawPixel (2, 4, matrix.Color(58, 137, 35));
  matrix.drawPixel (5, 4, matrix.Color(58, 137, 35));
  matrix.drawPixel (1, 5, matrix.Color(58, 137, 35));
  matrix.drawPixel (6, 5, matrix.Color(58, 137, 35));
  matrix.drawPixel (0, 6, matrix.Color(58, 137, 35));
  matrix.drawPixel (7, 6, matrix.Color(58, 137, 35));
}

void printapi2Logo() { //Pourcentage 
  matrix.drawPixel(0, 0, matrix.Color(255, 165, 0));
  matrix.drawPixel(1, 0, matrix.Color(255, 165, 0));
  matrix.drawPixel(6, 0, matrix.Color(255, 165, 0));
  matrix.drawPixel(7, 0, matrix.Color(255, 165, 0));
  matrix.drawPixel(0, 1, matrix.Color(255, 165, 0));
  matrix.drawPixel(1, 1, matrix.Color(255, 165, 0));
  matrix.drawPixel(5, 1, matrix.Color(255, 165, 0));
  matrix.drawPixel(6, 1, matrix.Color(255, 165, 0));
  matrix.drawPixel(7, 1, matrix.Color(255, 165, 0));
  matrix.drawPixel(4, 2, matrix.Color(255, 165, 0));
  matrix.drawPixel(5, 2, matrix.Color(255, 165, 0));
  matrix.drawPixel(6, 2, matrix.Color(255, 165, 0));
  matrix.drawPixel(3, 3, matrix.Color(255, 165, 0));
  matrix.drawPixel(4, 3, matrix.Color(255, 165, 0));
  matrix.drawPixel(5, 3, matrix.Color(255, 165, 0));
  matrix.drawPixel(2, 4, matrix.Color(255, 165, 0));
  matrix.drawPixel(3, 4, matrix.Color(255, 165, 0));
  matrix.drawPixel(4, 4, matrix.Color(255, 165, 0));
  matrix.drawPixel(1, 5, matrix.Color(255, 165, 0));
  matrix.drawPixel(2, 5, matrix.Color(255, 165, 0));
  matrix.drawPixel(3, 5, matrix.Color(255, 165, 0));
  matrix.drawPixel(0, 6, matrix.Color(255, 165, 0));
  matrix.drawPixel(1, 6, matrix.Color(255, 165, 0));
  matrix.drawPixel(2, 6, matrix.Color(255, 165, 0));
  matrix.drawPixel(6, 6, matrix.Color(255, 165, 0));
  matrix.drawPixel(7, 6, matrix.Color(255, 165, 0));
  matrix.drawPixel(0, 7, matrix.Color(255, 165, 0));
  matrix.drawPixel(1, 7, matrix.Color(255, 165, 0));
  matrix.drawPixel(6, 7, matrix.Color(255, 165, 0));
  matrix.drawPixel(7, 7, matrix.Color(255, 165, 0));
  
}

void printapi3Logo() { //Sablier temps 
 
  matrix.drawLine(0, 0, 7, 0, matrix.Color(255, 0, 0));
  matrix.drawLine(0, 7, 7, 7, matrix.Color(255, 0, 0));
  matrix.drawPixel(0, 1, matrix.Color(255, 0, 0));
  matrix.drawPixel(3, 6, matrix.Color(255, 0, 0));
  matrix.drawPixel(4, 6, matrix.Color(255, 0, 0));
  matrix.drawPixel(7, 1, matrix.Color(255, 0, 0));
  matrix.drawPixel (1, 2, matrix.Color(255, 0, 0));
  matrix.drawPixel (6, 2, matrix.Color(255, 0, 0));
  matrix.drawPixel (2, 3, matrix.Color(255, 0, 0));
  matrix.drawPixel (5, 3, matrix.Color(255, 0, 0));
  matrix.drawPixel (2, 4, matrix.Color(255, 0, 0));
  matrix.drawPixel (5, 4, matrix.Color(255, 0, 0));
  matrix.drawPixel (1, 5, matrix.Color(255, 0, 0));
  matrix.drawPixel (6, 5, matrix.Color(255, 0, 0));
  matrix.drawPixel (0, 6, matrix.Color(255, 0, 0));
  matrix.drawPixel (7, 6, matrix.Color(255, 0, 0));
}



void printapi4Logo() { // Horloge
 
  matrix.drawLine(2, 0, 5, 0, matrix.Color(255, 255, 255));
  matrix.drawLine(2, 7, 5, 7, matrix.Color(255, 255, 255));
  matrix.drawLine(3, 3, 7, 3, matrix.Color(255, 255, 255));
  matrix.drawPixel(1, 1, matrix.Color(255, 255, 255));
  matrix.drawPixel(6, 1, matrix.Color(255, 255, 255)); 
  matrix.drawPixel(0, 2, matrix.Color(255, 255, 255));
  matrix.drawPixel(0, 3, matrix.Color(255, 255, 255));
  matrix.drawPixel(0, 4, matrix.Color(255, 255, 255));
  matrix.drawPixel(0, 5, matrix.Color(255, 255, 255));
  matrix.drawPixel(1, 6, matrix.Color(255, 255, 255));
  matrix.drawPixel(3, 4, matrix.Color(255, 255, 255));
  matrix.drawPixel(3, 5, matrix.Color(255, 255, 255));
  matrix.drawPixel(3, 6, matrix.Color(255, 255, 255));
  matrix.drawPixel(6, 6, matrix.Color(255, 255, 255));
  matrix.drawPixel(7, 2, matrix.Color(255, 255, 255));
  matrix.drawPixel(7, 4, matrix.Color(255, 255, 255));
  matrix.drawPixel(7, 5, matrix.Color(255, 255, 255));
}

String A1counter() {

  String subs = "";

  api.getPrintJob();
        api.printJob.progressPrintTime;
        int estrunHours= api.printJob.progressPrintTime/3600;
        int estsecsRemaining=api.printJob.progressPrintTime%3600;
        int estrunMinutes=estsecsRemaining/60;
        int estrunSeconds=estsecsRemaining%60;
        char estbuf[31];
        sprintf(estbuf,"%02d:%02d",estrunHours,estrunMinutes);
        Serial.println(estbuf);
        
        if(estrunSeconds<=0){
          subs="--:--";
          col=1;
          stopRow = 50;
          
        }else{
        subs = estbuf;
        col=1;
        stopRow = 6;
        }
        return subs;

        
        Serial.print("Passé:");
        Serial.print(estbuf);
        Serial.println(estbuf);
}

String A2counter() {

  String subs = "";
  
 api.printJob.printerState;
          const float temp_percent = int(api.printJob.progressCompletion*100)/100;
          char perc[31]="";
          sprintf(perc,"%3.0f%%",temp_percent);

          if(temp_percent<=0){
            subs="%";
            col=5;
            stopRow = 30;
          }else{
          subs =perc;
          col=5;
          stopRow = 1;
          }
          return subs;

          
}

String A3counter() {

  String subs = "";
  
  api.printJob.printerState;
        int runHours= api.printJob.progressPrintTimeLeft/3600;
        int secsRemaining=api.printJob.progressPrintTimeLeft%3600;
        int runMinutes=secsRemaining/60;
        int runSeconds=secsRemaining%60;
        char buf[31];
        sprintf(buf,"%02d:%02d",runHours,runMinutes);
        
        if(runSeconds<=0){
          subs="--:--";
          col=6;
          stopRow = 16;
        }else{
        subs = buf;
        col=6;
        stopRow = 1;
        }
        return subs;

        
        Serial.print("progressPrintTimeLeft (sec) long:\t");
        Serial.println(buf);
}  


String A4counter() {
timeClient.update();
//  partie a changer :wink: 
int Heure = timeClient.getHours();
int Min =timeClient.getMinutes();
char heure[31];
sprintf(heure, "%02d:%02d",Heure,Min);

  String subs = "";
  
        if(heure=="--:--"){
          subs="HEURE";
          col=0;
          stopRow = 16;
          Serial.print("heure recup");
          Serial.println(heure);
          }else{
        subs = heure;
        col=0;
       stopRow = 0;
                               
        }
        return subs;
       
         
}  
