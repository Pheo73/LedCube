//On inclut les bibliotheques necessaires
#include <SPI.h>
#include <Ethernet.h>
#include <Server.h>
#include <Client.h>

//Variables pour le controle du led cube
#define LATCH 6
#define SHIFT1 5
#define DATA1 7
#define SHIFT2 3
#define DATA2 2
#define SHIFT3 8
#define DATA3 9
int t=4500;
int i;
int repet;

//Variable qui va stocker le motif selectionné par l'utilisateur
int choix=0;
//Tableau des équivalences pour le motif - l'index correspond au choix de l'utilisateur
String motif[]={"Aucun","Entier","Arretes","Balayage","Glissade","Debordement","Spirale","Balade","Demo"};
//Le nombre de motifs différents
int nbMotif=8;

//Variables utilisées lors de la récupérations des infos de la selection
String chaine;
String valeur;
boolean cpt;
String actuel;

//Parametrage de la carte reseau : adresse mac, adresse ip et position serveur
byte mac[] = {  0x90, 0xA2, 0xDA, 0x00, 0x7A, 0x05 };
IPAddress ip(192,168,1,65);
EthernetServer serveurHTTP(80);

//Initialisation
void setup(){
  //Pour envoyer des infos sur le moniteur série
  Serial.begin(115200); 
  //On attribut les parametres à la carte Ethernet et on la met en route
  Ethernet.begin(mac, ip);
  serveurHTTP.begin(); 
  // Declaration des sorties pour le led cube
  pinMode(LATCH, OUTPUT);	
  pinMode(SHIFT1, OUTPUT);
  pinMode(DATA1, OUTPUT);
  pinMode(SHIFT2, OUTPUT);
  pinMode(DATA2, OUTPUT);
  pinMode(SHIFT3, OUTPUT);
  pinMode(DATA3, OUTPUT); 
} 

//boucle d'execution
void loop(){
  //tant qu'il n'y a pas de client disponible, lance l'allumage du led cube
  while(!serveurHTTP.available()){
    allumage();
  }
  //Quand le client est disponible
  EthernetClient client = serveurHTTP.available();
  if (client) { 
    // cpt vaut faux tant qu'on a pas recuperer une premiere fois la partie de la requete qui nous interesse à savoir la valeur recuperer grace au GET
    cpt=false;
    // on vide la chaine
    chaine="";
    
    Serial.println(F("------------ Connexion Client ------------"));
    Serial.println (F("")); 
    //Si le client est connecté
      if (client.connected()) {
        Serial.println(F("Connexion avec client OK ... "));
        Serial.println(F(""));
        //Tant que le client est disponible
        while (client.available()) { 
          //on recupere la requete du client caractere par caractere et on l'ajoute à la chaine "chaine"
          char c = client.read();
          chaine.concat(c);
          Serial.print(c);
          //Une fois que la chaine a une taille superieure à 25 on va recuperer les infos
          if (chaine.length() > 25 && cpt==false)
          {
            //On recupere les caracteres qui nous interessent et on les convertit en un entier
            valeur = chaine.substring (19,20);
            choix=valeur.toInt();
            //on passe cpt a true pour indiquer qu'on a deja recuperer la valeur qui nous interesse
            cpt=true;
          }
        }

        
        //On retourne la page au client
        Serial.println(F("Reception requete client terminee... "));
        Serial.println(F(""));
        Serial.println(F("Envoi de la reponse HTTP au client..."));
        client.println(F("HTTP/1.1 200 OK"));
        client.println(F("Content-Type: text/html"));
        client.println(F("Connection: close"));         
        client.println();
  
        Serial.println(F("Envoi de la reponse HTML au client..."));
        Serial.println(F(""));
//HTML
        client.println(F("<!DOCTYPE html>"));
        client.println(F("<html lang=\"fr\" dir=\"ltr\">"));
        client.println(F("<head>"));
        client.println(F("<meta charset=\"utf-8\">"));
        client.println(F("<title>Controle des leds</title>"));
        client.println(F("<link rel=\"preconnect\" href=\"https://fonts.googleapis.com\">"));
        client.println(F("<link rel=\"preconnect\" href=\"https://fonts.gstatic.com\" crossorigin>"));
        client.println(F("<link href=\"https://fonts.googleapis.com/css2?family=Pacifico&display=swap\" rel=\"stylesheet\">"));
        client.println(F("<link rel=\"preconnect\" href=\"https://fonts.gstatic.com\" crossorigin>"));
        client.println(F("<style>html{font-family:Pacifico;background-size:cover;background-repeat:no-repeat;background-image:url(\"data:image/svg+xml;utf8,%3Csvg width=%22100%25%22 height=%22100%25%22 viewBox=%220 0 1000 1000%22 xmlns=%22http:%2F%2Fwww.w3.org%2F2000%2Fsvg%22 %3E %3Cdefs%3E %3Cfilter id=%22grain%22 x=%22-50vw%22 y=%22-50vh%22 width=%22100vw%22 height=%22100vh%22%3E %3CfeFlood flood-color=%22%23ffffff%22 result=%22neutral-gray%22 %2F%3E %3CfeTurbulence in=%22neutral-gray%22 type=%22fractalNoise%22 baseFrequency=%222.5%22 numOctaves=%22100%22 stitchTiles=%22stitch%22 result=%22noise%22 %2F%3E %3CfeColorMatrix in=%22noise%22 type=%22saturate%22 values=%220%22 result=%22destaturatedNoise%22 %3E%3C%2FfeColorMatrix%3E %3CfeComponentTransfer in=%22desaturatedNoise%22 result=%22theNoise%22%3E %3CfeFuncA type=%22table%22 tableValues=%220 0 0.35 0%22%3E%3C%2FfeFuncA%3E %3C%2FfeComponentTransfer%3E %3CfeBlend in=%22SourceGraphic%22 in2=%22theNoise%22 mode=%22soft-light%22 result=%22noisy-image%22 %2F%3E %3C%2Ffilter%3E %3CclipPath id=%22shape%22%3E %3Cpath fill=%22currentColor%22 d=%22M843%2C655.5Q811%2C811%2C655.5%2C835Q500%2C859%2C361%2C818.5Q222%2C778%2C152%2C639Q82%2C500%2C174.5%2C383.5Q267%2C267%2C383.5%2C223.5Q500%2C180%2C606%2C234Q712%2C288%2C793.5%2C394Q875%2C500%2C843%2C655.5Z%22%3E%3C%2Fpath%3E %3C%2FclipPath%3E %3C%2Fdefs%3E %3Cg clip-path=%22url(%23shape)%22%3E %3Cpath fill=%22%23444cf7%22 d=%22M843%2C655.5Q811%2C811%2C655.5%2C835Q500%2C859%2C361%2C818.5Q222%2C778%2C152%2C639Q82%2C500%2C174.5%2C383.5Q267%2C267%2C383.5%2C223.5Q500%2C180%2C606%2C234Q712%2C288%2C793.5%2C394Q875%2C500%2C843%2C655.5Z%22 %2F%3E %3C%2Fg%3E %3C%2Fsvg%3E\");background-size:50%;background-position-x:center}header{display:flex}header img{width:5%;height:5%;margin:38px 0 0}h2{font-weight:400;font-size:64px;color:#4E53F4;margin:38px 10px 0 54px}section{display:flex}.currentpattern{margin:10 % 8% 0 10%;width:350px;height:300px;background-size:cover;background-repeat:no-repeat;background-image:url(\"data:image/svg+xml;utf8,%3Csvg width=%22100%25%22 height=%22100%25%22 viewBox=%220 0 1000 1000%22 xmlns=%22http:%2F%2Fwww.w3.org%2F2000%2Fsvg%22 %3E %3Cdefs%3E %3CclipPath id=%22shape%22%3E %3Cpath fill=%22currentColor%22 d=%22M799%2C627Q754%2C754%2C627%2C824.5Q500%2C895%2C357.5%2C840Q215%2C785%2C154%2C642.5Q93%2C500%2C138.5%2C342Q184%2C184%2C342%2C179Q500%2C174%2C620.5%2C216.5Q741%2C259%2C792.5%2C379.5Q844%2C500%2C799%2C627Z%22%3E%3C%2Fpath%3E %3C%2FclipPath%3E %3C%2Fdefs%3E %3Cg clip-path=%22url(%23shape)%22%3E %3Cpath fill=%22%23444cf7%22 d=%22M799%2C627Q754%2C754%2C627%2C824.5Q500%2C895%2C357.5%2C840Q215%2C785%2C154%2C642.5Q93%2C500%2C138.5%2C342Q184%2C184%2C342%2C179Q500%2C174%2C620.5%2C216.5Q741%2C259%2C792.5%2C379.5Q844%2C500%2C799%2C627Z%22 %2F%3E %3C%2Fg%3E %3C%2Fsvg%3E\");background-position-y:bottom}.currentpattern1{margin:65px 0 40px 60px;font-size:28px;color:#fff}.currentpattern2{margin:0 0 0 100px;font-size:34px;color:black}.main{margin-left:20%;margin-top:5%;display:inherit;font-size:48px;align-self:center;color:#fff}#color{border-radius:18px;font:28px Pacifico}option{font-size:28px}.buttonsubmit{background-color:transparent;border:none;color:#fff;text-decoration:none;display:inline-block;margin:50px 0 0 250px;font:28px Pacifico}.buttonsubmit:hover{color:black}.buttonoff{color:#fff;width:120px;height:150px;margin-left:250px;background-color:transparent;background-size:cover;background-repeat:no-repeat;background-image:url(\"data:image/svg+xml;utf8,%3Csvg width=%22100%25%22 height=%22100%25%22 viewBox=%220 0 1000 1000%22 xmlns=%22http:%2F%2Fwww.w3.org%2F2000%2Fsvg%22 %3E %3Cdefs%3E %3Cfilter id=%22grain%22 x=%22-50vw%22 y=%22-50vh%22 width=%22100vw%22 height=%22100vh%22%3E %3CfeFlood flood-color=%22%23ffffff%22 result=%22neutral-gray%22 %2F%3E %3CfeTurbulence in=%22neutral-gray%22 type=%22fractalNoise%22 baseFrequency=%222.5%22 numOctaves=%22100%22 stitchTiles=%22stitch%22 result=%22noise%22 %2F%3E %3CfeColorMatrix in=%22noise%22 type=%22saturate%22 values=%220%22 result=%22destaturatedNoise%22 %3E%3C%2FfeColorMatrix%3E %3CfeComponentTransfer in=%22desaturatedNoise%22 result=%22theNoise%22%3E %3CfeFuncA type=%22table%22 tableValues=%220 0 0.2 0%22%3E%3C%2FfeFuncA%3E %3C%2FfeComponentTransfer%3E %3CfeBlend in=%22SourceGraphic%22 in2=%22theNoise%22 mode=%22soft-light%22 result=%22noisy-image%22 %2F%3E %3C%2Ffilter%3E %3CclipPath id=%22shape%22%3E %3Cpath fill=%22currentColor%22 d=%22M650%2C586Q556%2C672%2C337%2C724.5Q118%2C777%2C150%2C523Q182%2C269%2C379%2C268Q576%2C267%2C660%2C383.5Q744%2C500%2C650%2C586Z%22%3E%3C%2Fpath%3E %3C%2FclipPath%3E %3C%2Fdefs%3E %3Cg clip-path=%22url(%23shape)%22%3E %3Cpath fill=%22%23444cf7%22 d=%22M650%2C586Q556%2C672%2C337%2C724.5Q118%2C777%2C150%2C523Q182%2C269%2C379%2C268Q576%2C267%2C660%2C383.5Q744%2C500%2C650%2C586Z%22 %2F%3E %3C%2Fg%3E %3C%2Fsvg%3E\");border:none;font:20px Pacifico}.buttonoff:hover{color:black}.btn_demo{ font-family:Pacifico; color:white; font-size: 26px; width:84%; height:298%; margin-left: 200px; background-color: transparent; background-size: cover; background-repeat: no-repeat; background-image: url(\"data:image/svg+xml;utf8, %3Csvg width=%22100%25%22 height=%22100%25%22 viewBox=%220 0 1000 1000%22 xmlns=%22http:%2F%2Fwww.w3.org%2F2000%2Fsvg%22 %3E %3Cdefs%3E %3Cfilter id=%22grain%22 x=%22-50vw%22 y=%22-50vh%22 width=%22100vw%22 height=%22100vh%22%3E %3CfeFlood flood-color=%22%23ffffff%22 result=%22neutral-gray%22 %2F%3E %3CfeTurbulence in=%22neutral-gray%22 type=%22fractalNoise%22 baseFrequency=%222.5%22 numOctaves=%22100%22 stitchTiles=%22stitch%22 result=%22noise%22 %2F%3E %3CfeColorMatrix in=%22noise%22 type=%22saturate%22 values=%220%22 result=%22destaturatedNoise%22 %3E%3C%2FfeColorMatrix%3E %3CfeComponentTransfer in=%22desaturatedNoise%22 result=%22theNoise%22%3E %3CfeFuncA type=%22table%22 tableValues=%220 0 0.2 0%22%3E%3C%2FfeFuncA%3E %3C%2FfeComponentTransfer%3E %3CfeBlend in=%22SourceGraphic%22 in2=%22theNoise%22 mode=%22soft-light%22 result=%22noisy-image%22 %2F%3E %3C%2Ffilter%3E %3CclipPath id=%22shape%22%3E %3Cpath fill=%22currentColor%22 d=%22M650%2C586Q556%2C672%2C337%2C724.5Q118%2C777%2C150%2C523Q182%2C269%2C379%2C268Q576%2C267%2C660%2C383.5Q744%2C500%2C650%2C586Z%22%3E%3C%2Fpath%3E %3C%2FclipPath%3E %3C%2Fdefs%3E %3Cg clip-path=%22url(%23shape)%22%3E %3Cpath fill=%22%23444cf7%22 d=%22M650%2C586Q556%2C672%2C337%2C724.5Q118%2C777%2C150%2C523Q182%2C269%2C379%2C268Q576%2C267%2C660%2C383.5Q744%2C500%2C650%2C586Z%22 %2F%3E %3C%2Fg%3E %3C%2Fsvg%3E\") ; border: none; } .btn_demo:hover{ color:black; }</style>"));


  //Corps HTML
        client.println(F("<header class=\"title\">"));
        client.println(F("<h2>Led Cube</h2>"));
        client.println(F("<img src=\"https://cdn-icons-png.flaticon.com/512/2338/2338838.png\">"));
        client.println(F("</header>"));

        client.println(F("<body\">"));
        client.println(F("<section>"));
    //Le motif actuel
        client.println(F("<div class=\"currentpattern\">"));
        client.println(F("<p class=\"currentpattern1\">Motif en cours : "));
        client.println(F("<p class=\"currentpattern2\">"));
        actuel=motif[choix];
        client.println(actuel);
        client.println(F("</p>"));
        client.println(F("</div>"));
    //Choix
        client.println(F("<div class=\"main\">"));
        client.println(F("<FORM method=\"get\" action=\"http://192.168.1.65/\" style=\"display: grid;\">"));
        client.println(F("<label for=\"color\">Motif: </label>"));
        client.println(F("<select id=\"color\" name=\"color\">"));
        //Rempli les champs possibles du formulaire en fonction des motifs possibles (voir tableau motif[])
        for (int k=1; k<=nbMotif;k++){
          client.println(F("<option value=\"0"));
          client.println(k);
          client.println(F("\">"));
          client.println(motif[k]);
          client.println(F("</option>"));
        }
        client.println(F("</select>"));
        client.println(F("<input type=\"submit\" class=\"buttonsubmit\" value=\"OK\"/>"));
        client.println(F("</FORM>"));

  //Les boutons pour eteindre le led cube et lancer la demo
        client.println(F("<FORM method=\"get\" style=\"display:grid;\"action=\"http://192.168.1.65/\">"));
        client.println(F("<button type=\"submit\" class=\"buttonoff\" value=\"Eteindre\">Eteindre</button>"));
        //client.println(F("<button type=\"submit\" class=\"btn_demo\" value=\"Demo\">Lancer </br> la Demo !</button>"));
        client.println(F("</FORM>"));
        client.println(F("</section>"));
        client.println(F("</body>"));
        client.println(F("</html>"));
      }
    delay(1);
    client.stop();
    Serial.println(F("------ Fermeture de la connexion avec le client -------"));
  }
}
//Pour appeler le bon motif en fonction du choix de l'utilisateur
void allumage(){
  switch (choix){
    case 0:
      eteindre();
      break;
    case 1:
      cubeEntier();
      break;
    case 2: 
      arretes();
      break;
    case 3:
      balayage();
      break;
    case 4:
      glissade();
      break;
    case 5:
      debordement();
      break;
    case 6:
      spirale();
      break;
    case 7:
      balladeArrete();
      break;
    case 8:
      demo();
      break;
    default:
      break;
  }
}


//Les differentes procedures pour l'allumage du led cube
//Une procedure par motif
void demo(){
      balayage();
      balayage();
      balayage();
      glissade();
      glissade();
      glissade();
      debordement();
      debordement();
      debordement();
      spirale();
      spirale();
      spirale(); 
      balladeArrete();
      balladeArrete();
      balladeArrete();
  }
  
void eteindre(){
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11110110); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B01101111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000000);
     digitalWrite(LATCH,HIGH);
}

void  spirale()
{
   repet=30;
   for(i=0; i<repet; i++)
   {
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11110110); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B01101111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00001000);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
   }
   for(i=0; i<repet; i++)
   {
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B01111101); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B10111110);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000100);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
   }
   for(i=0; i<repet; i++)
   {
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11101011); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11010111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000010);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
   }
   for(i=0; i<repet; i++)
   {
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11110110); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B01101111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000001);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
   }
   for(i=0; i<repet; i++)
   {
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B01111101); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B10111110);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000010);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
   }
   for(i=0; i<repet; i++)
   {
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11101011); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11010111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000100);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
   }
   
}

void  debordement()
{
   repet=15;
   for(i=0; i<repet; i++)
   {
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B01100000); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B00000110);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00001000);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
   }
   for(i=0; i<repet; i++)
   {
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B01100000); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B00000110);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000100);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
   }
   for(i=0; i<repet; i++)
   {
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B01100000); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B00000110);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000010);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
   }
   for(i=0; i<repet; i++)
   {
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B01100000); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B00000110);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000001);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
   }
   for(i=0; i<repet; i++)
   {
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B10011111); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11111001);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000001);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
   }
   for(i=0; i<repet; i++)
   {
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B10011111); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11111001);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000010);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
   }
   for(i=0; i<repet; i++)
   {
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B10011111); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11111001);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000100);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
   }
   for(i=0; i<repet; i++)
   {
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B10011111); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11111001);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00001000);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
   }

}

void  arretes()
{
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B01100000); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B00000110);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000001);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11110110); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B01101111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000010);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11110110); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B01101111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000100);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B01100000); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B00000110);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00001000);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);

}

void  glissade()
{
   repet=7;
   for(i=0; i<repet; i++)
   {
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11111111); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B00001111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000001);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11111111); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B00001111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000010);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11111111); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B00001111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000100);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11111111); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B00001111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00001000);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
  }
  for(i=0; i<repet; i++)
   {
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11111111); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B00001111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000010);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11111111); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B00001111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000100);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11111111); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B00000000);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00001000);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
  }
  for(i=0; i<repet; i++)
   {
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11111111); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B00001111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000100);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B00001111); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B00000000);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00001000);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
  }
  for(i=0; i<repet; i++)
   {
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B00000000); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B00000000);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00001000);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
  }
  for(i=0; i<repet; i++)
   {
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11110000); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11111111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000100);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B00000000); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11110000);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00001000);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
  }
  for(i=0; i<repet; i++)
   {
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11110000); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11111111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000010);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11110000); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11111111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000100);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B00000000); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11111111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00001000);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
  }
  for(i=0; i<repet; i++)
   {
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11110000); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11111111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000001);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11110000); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11111111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000010);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11110000); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11111111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000100);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11110000); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11111111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00001000);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
  }
  for(i=0; i<repet; i++)
   {
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B00000000); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11111111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000001);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11110000); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11111111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000010);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11110000); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11111111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000100);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
  }
  for(i=0; i<repet; i++)
   {
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B00000000); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11110000);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000001);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11110000); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11111111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000010);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);     
  }
  for(i=0; i<repet; i++)
   {
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B00000000); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B00000000);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000001);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);    
  }
   for(i=0; i<repet; i++)
   {
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B00001111); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B00000000);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000001);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11111111); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B00001111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000010);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);     
  }
  for(i=0; i<repet; i++)
   {
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11111111); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B00000000);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000001);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11111111); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B00001111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000010);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11111111); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B00001111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000100);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);     
  }
}

void  balayage()
{
   repet =10;
   for(i=0;i<repet;i++)
   { 
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11111111); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B01111111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000001);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11111111); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B01111111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000010);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11111111); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B01111111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000100);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11111111); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B01111111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00001000);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
   }
   for(i=0;i<repet;i++)
   { 
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11111111); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B00110111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000001);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11111111); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B00110111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000010);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11111111); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B00110111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000100);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11111111); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B00110111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00001000);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
   }
   for(i=0;i<repet;i++)
   { 
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B01111111); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B00010011);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000001);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B01111111); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B00010011);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000010);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B01111111); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B00010011);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000100);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B01111111); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B00010011);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00001000);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
   }
   for(i=0;i<repet;i++)
   { 
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B00110111); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B10000001);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000001);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B00110111); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B10000001);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000010);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B00110111); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B10000001);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000100);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B00110111); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B10000001);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00001000);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
   }
   for(i=0;i<repet;i++)
   { 
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B00010011); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11001000);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000001);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B00010011); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11001000);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000010);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B00010011); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11001000);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000100);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B00010011); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11001000);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00001000);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
   }
    for(i=0;i<repet;i++)
   { 
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B10000001); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11101100);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000001);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B10000001); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11101100);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000010);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B10000001); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11101100);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000100);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B10000001); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11101100);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00001000);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
   }
   for(i=0;i<repet;i++)
   { 
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11001000); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11111110);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000001);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11001000); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11111110);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000010);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11001000); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11111110);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000100);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11001000); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11111110);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00001000);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
   }
   for(i=0;i<repet;i++)
   { 
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11101100); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11111111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000001);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11101100); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11111111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000010);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11101100); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11111111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000100);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11101100); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11111111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00001000);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
   }
   for(i=0;i<repet;i++)
   { 
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11111110); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11111111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000001);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11111110); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11111111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000010);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11111110); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11111111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000100);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11111110); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11111111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00001000);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
   }
   for(i=0;i<repet;i++)
   { 
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11111111); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11111111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000001);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11111111); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11111111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000010);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11111111); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11111111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000100);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11111111); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11111111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00001000);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
   }
}

void  cubeEntier()
{
     
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B00000000); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B00000000);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000001);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B00000000); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B00000000);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000010);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B00000000); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B00000000);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000100);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B00000000); 
     shiftOut(DATA2,SHIFT2,MSBFIRST,B00000000);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00001000);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
}

void  balladeArrete()
{
  
     repet=5;
  for(i=0;i<repet;i++)
  {
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11111111);
     shiftOut(DATA2,SHIFT2,MSBFIRST,B00011111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000001);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
  }
  for(i=0;i<repet;i++)
  {
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11111111);
     shiftOut(DATA2,SHIFT2,MSBFIRST,B00111111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000001);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11111111);
     shiftOut(DATA2,SHIFT2,MSBFIRST,B01111111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000010);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
  }
  for(i=0;i<repet;i++)
  {
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11111111);
     shiftOut(DATA2,SHIFT2,MSBFIRST,B01111111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000001);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11111111);
     shiftOut(DATA2,SHIFT2,MSBFIRST,B01111111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000010);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11111111);
     shiftOut(DATA2,SHIFT2,MSBFIRST,B01111111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000100);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
  }
  for(i=0;i<repet;i++)
  {
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11111111);
     shiftOut(DATA2,SHIFT2,MSBFIRST,B01111111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000010);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11111111);
     shiftOut(DATA2,SHIFT2,MSBFIRST,B01111111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000100);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11111111);
     shiftOut(DATA2,SHIFT2,MSBFIRST,B01111111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00001000);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
  }
  for(i=0;i<repet;i++)
  {
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11111111);
     shiftOut(DATA2,SHIFT2,MSBFIRST,B01111111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000100);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11111111);
     shiftOut(DATA2,SHIFT2,MSBFIRST,B01110111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00001000);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
  }
  for(i=0;i<repet;i++)
  {
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B01111111);
     shiftOut(DATA2,SHIFT2,MSBFIRST,B01110111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00001000);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
  }
  for(i=0;i<repet;i++)
  {
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B01110111);
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11110111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00001000);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
  }
  for(i=0;i<repet;i++)
  {
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B01110111);
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11111111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00001000);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11110111);
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11111111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000100);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
  }
  for(i=0;i<repet;i++)
  {
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11110111);
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11111111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00001000);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11110111);
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11111111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000100);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11110111);
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11111111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000010);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
  }
  for(i=0;i<repet;i++)
  {
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11110111);
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11111111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000100);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11110111);
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11111111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000010);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11110111);
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11111111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000001);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
  }
  for(i=0;i<repet;i++)
  {
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11110011);
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11111111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000001);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11110111);
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11111111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000010);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
  }
  for(i=0;i<repet;i++)
  {
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11110001);
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11111111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000001);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
  }
  for(i=0;i<repet;i++)
  {
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11111000);
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11111111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000001);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
  }
  for(i=0;i<repet;i++)
  {
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11111100);
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11111111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000001);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11111110);
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11111111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000010);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
  }
  for(i=0;i<repet;i++)
  {
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11111110);
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11111111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000001);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11111110);
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11111111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000010);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11111110);
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11111111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000100);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
  }
  for(i=0;i<repet;i++)
  {
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11111110);
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11111111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000010);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11111110);
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11111111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000100);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11111110);
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11111111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00001000);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
  }
  for(i=0;i<repet;i++)
  {
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11111110);
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11111111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000100);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11101110);
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11111111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00001000);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
  }
  for(i=0;i<repet;i++)
  {
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11101110);
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11111110);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00001000);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
  }
  for(i=0;i<repet;i++)
  {
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11101111);
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11101110);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00001000);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
  }
  for(i=0;i<repet;i++)
  {
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11111111);
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11101111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000100);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11111111);
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11101110);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00001000);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
  }
  for(i=0;i<repet;i++)
  {
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11111111);
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11101111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000010);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11111111);
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11101111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000100);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11111111);
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11101111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00001000);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
  }
  for(i=0;i<repet;i++)
  {
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11111111);
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11101111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000001);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11111111);
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11101111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000010);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11111111);
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11101111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000100);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
  }
  for(i=0;i<repet;i++)
  {
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11111111);
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11001111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000001);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11111111);
     shiftOut(DATA2,SHIFT2,MSBFIRST,B11101111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000010);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
  }
  for(i=0;i<repet;i++)
  {
     digitalWrite(LATCH,LOW);
     shiftOut(DATA1,SHIFT1,MSBFIRST,B11111111);
     shiftOut(DATA2,SHIFT2,MSBFIRST,B10001111);
     shiftOut(DATA3,SHIFT3,MSBFIRST,B00000001);//1 pour allumer etage
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(t);
  }
    
}
