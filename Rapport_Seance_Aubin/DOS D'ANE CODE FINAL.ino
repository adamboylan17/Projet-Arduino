#include<LiquidCrystal.h>
#include <Servo.h>
#include <IRremote.h>

int led_rouge = 6;
int RECV_PIN = 5; //
IRrecv irrecv (RECV_PIN); // Déclaration du module IR
decode_results results;

#define Broche_Echo 7 // Broche Echo du HC-SR04 sur D7 //
#define Broche_Trigger 8 // Broche Trigger du HC-SR04 sur D8 //
#define OK_BUTTON 0xFF02FD
#define  BAS_BUTTON = 0xFFA857
#define DROITE_BUTTON = 0xFFC23D
#define GAUCHE_BUTTON = 0xFF22DD
#define HAUT_BUTTON 0xFF629D
#define UN_BUTTON 0xFF6897
#define DEUX_BUTTON 0xFF9867
#define TROIS_BUTTON 0xFFB04F
#define QUATRE_BUTTON 0xFF30CF
#define CINQ_BUTTON 0xFF18E7
#define SIX_BUTTON 0xFF7A85
#define SEPT_BUTTON 0xFF10EF
#define HUIT_BUTTON 0xFF38C7
#define NEUF_BUTTON 0xFF5AA5
#define etoile_BUTTON 0xFF42BD
#define ZERO_BUTTON 0xFF4AB5
#define HASTAG_BUTTON 0xFF52AD



LiquidCrystal lcd(2, 3, 4, 9, 10, 11);

/* Créer un objet Servo pour contrôler le servomoteur */
Servo servomoteur;
Servo servomoteur2;
int positions = 0;

void dos_se_leve(){
  for (int position =0; position <= 110; position++) {
        servomoteur.write(position);
        int positionb=180-position;
        servomoteur2.write(positionb); 
        delay(15);
    }
}


void dos_se_baisse() {
       for (int position = 110; position >=0; position --){
        servomoteur.write(position);
        int positionc=180-position;
        servomoteur2.write(positionc);
        delay(15);  
  } 
}


// Definition des variables
int MesureMaxi = 300; // Distance maxi a mesurer //
int MesureMini = 3; // Distance mini a mesurer //

boolean bobo = true;
long Duree;
long Distance;
int distanceCm()
{
  // Debut de la mesure avec un signal de 10 µS applique sur TRIG //
  digitalWrite(Broche_Trigger, LOW); // On efface l'etat logique de TRIG //
  delayMicroseconds(2);

  digitalWrite(Broche_Trigger, HIGH); // On met la broche TRIG a "1" pendant 10µS //
  delayMicroseconds(10);
  digitalWrite(Broche_Trigger, LOW); // On remet la broche TRIG a "0" //
  // On mesure combien de temps le niveau logique haut est actif sur ECHO //

  const float DureeMicro = pulseIn(Broche_Echo, HIGH);

  Serial.print("La durée est de :");
  Serial.print(" ");
  Serial.print(DureeMicro);
  Serial.println("µs");
  Serial.println(" ");

  // Calcul de la distance grace au temps mesure //

  const float DistanceCm = DureeMicro * 0.034 / 2.0; //  (Durée totale (aller-retour))

  // Verification si valeur mesuree dans la plage //

  if (Distance >= MesureMaxi || Distance <= MesureMini) {
    // Si la distance est hors plage, on affiche un message d'erreur //
    Serial.println("Distance de mesure en dehors de la plage (3 cm à 3 m)");
  }
  else {
    // Affichage dans le moniteur serie de la distance mesuree //
    Serial.print("Distance mesuree :");
    Serial.print(DistanceCm);
    Serial.println("cm");
    Serial.println(" ");
  }

  return DistanceCm;


}



void setup()
{
  // Démarre la communication série
  irrecv.enableIRIn (); // Démarrage du récepteur IR
  pinMode(led_rouge,OUTPUT);
  pinMode(Broche_Trigger, OUTPUT); // Broche Trigger en sortie //
  pinMode(Broche_Echo, INPUT); // Broche Echo en entree //
  lcd.begin(16, 2);
 servomoteur.attach(12);// Attache le servomoteur à la broche D13                                    
 servomoteur2.attach(13);// Attache le servomoteur à la broche D13
 servomoteur.write(0);
 servomoteur2.write(0);
 Serial.begin (9600);
}


void loop() {


  
  float distCm1 = distanceCm();
  float deltaT = 500; //temps avant prochaine mesure
  delay(deltaT);
  float distCm2 = distanceCm();
  float deltaD = distCm1 - distCm2;

  float vitesseS = deltaD / deltaT * 10;
  float vitesse = vitesseS * 3.6;

 // Fait bouger le bras de 0° à 180°
 
 if (irrecv.decode (& results)) {
      Serial.println ("results.value");Serial.println (results.value, HEX); // Affichage de la valeur reçue en héxadécimal
      irrecv.resume (); // Réception de la valeur suivante
      digitalWrite(led_rouge,LOW);
      delay(15000);
  
 }
  if(vitesseS >= 0.7){

    lcd.setCursor(0, 0);
    lcd.print("Speed : ");
    lcd.print(vitesseS,2);
    lcd.print("km/h");
    lcd.setCursor(0,1);
    lcd.print("Ralentissez     ");
    if (irrecv.decode (& results)) {
      Serial.println ("results.value");Serial.println (results.value, HEX); // Affichage de la valeur reçue en héxadécimal
      irrecv.resume (); // Réception de la valeur suivante
      digitalWrite(led_rouge,LOW);
      delay(10000);

 }
    digitalWrite(led_rouge,HIGH);
    dos_se_leve();
    delay(15000);
    digitalWrite(led_rouge,LOW);
    dos_se_baisse();
    delay(3000);
  }

  else{
  lcd.setCursor(0, 0);
  lcd.print("Speed : ");
  lcd.print(vitesseS,2);
  lcd.print("km/h");
  lcd.setCursor(0,1);
  lcd.print("Bonne vitesse");
    
  }

  


  Serial.println("la vitesse est ");
  Serial.print(vitesse,4);
  Serial.println(" m/s");
  Serial.print(vitesseS);
  Serial.println("km/h");
  Serial.println(" ");






  //delay(1000); // On ajoute 1 seconde de delais entre chaque mesure //

}
