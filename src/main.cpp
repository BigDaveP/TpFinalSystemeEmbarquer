/* Copyright (C) 2022 Alain Dube
 * All rights reserved.
 *
 * Projet Stone de base
 * Ecole du Web
 * Cours Systèmes embarqués (c)2022
 *
    @file     main.cpp
    @author   Alain Dubé
    @version  1.1 22/08/15
    @description
      Démonstration comment utiliser le PORT SERIE pour accèder aux fonctionnalités
      de l'écran STONE en utilisant la classe MyStone et MySerial

    platform = ESP32
    OS : Arduino
    Langage : C++

    Historique des versions
        Version    Date       Auteur       Description
        1.1        22/08/15  Alain       Première version du logiciel

    Fonctionnalités implantées
        Lecture des evénements envoyés par l'écran
        Envoyer une commande à l'écran
          Optenir la version du Firmware de l'écran
 * */

#include <Arduino.h>



#define RXD2 18//16
#define TXD2 19//17
#define BAUD_RATE 115200

#include <iostream>


#include "MyStone.h"
MyStone *myStone;

#include "MyTemp.h"
MyTemp *mytemp = new MyTemp();

std::string intToHexa(int value){
  char buffer[10];
  sprintf(buffer , "0x%4X", value);
  return (buffer);
};

std::string charToString(char str, std::string formatStr){
  char buffer[10];
  sprintf(buffer , formatStr.c_str(), str);
  return (buffer);
};

std::string intToString(int value, std::string formatStr){
  char buffer[10];
  sprintf(buffer , formatStr.c_str(), value);
  return (buffer);
};



//Thread qui permet de LOOPER et lire le contenu du port serie
//avec l'aide du la fonction getValidsDatasIfExists
void readStoneData() {

  int nbreSecondes = 0;
  char strSec[3];
  int nbreSecondesMax = 20;
  float temperatureMin = 22.5;
  float temperatureMax = 27.5;
  char strTempMin[50];
  

  datasRead rd = myStone->getValidsDatasIfExists();
  //std::cout << "GData : " << intToHexa(abs(rd.id)) << " " << rd.command << " " << rd.name << " " << rd.type << "\n";
  switch(rd.id){

      case 0x0002: { //Version
          std::cout << "GData : " << intToHexa(abs(rd.id)) << " " << rd.command << " " << rd.name << " " << rd.type << "\n";
          std::string stoneVersion = rd.name;
          std::cout << "Version : " <<  stoneVersion.c_str() << "\n";

          //std::this_thread::sleep_for(std::chrono::milliseconds(10));

          break;
          } 
    }

  if(rd.id<0) std::cout << "Data received ( id: : " << intToHexa(abs(rd.id)) << "  Command: " << rd.command << " Type: " << rd.type<< ")\n";
  //Si on apui sur le bouton pour démarrer le four
  if (rd.type == 2){
    //Permet d'obtenir la température du senseur DHT22
    mytemp->getTemp();
    //si la température du senseur est inférieur à la température min
    if(mytemp->getTemp() < temperatureMin){
      myStone->setLabel("labelcuisson", "Température trop faible !");
    }
    if(mytemp->getTemp() > temperatureMax){
      myStone->setLabel("labelcuisson", "Température trop faible !");
    }
    if(mytemp->getTemp() >= temperatureMin && mytemp->getTemp() <= temperatureMax){
      //Premet de faire la boucle jusqu'à ce que le nombre de secondes soit égal à nbreSecondesMax
      while(nbreSecondes < nbreSecondesMax){
        //Permet de d'obtenir la température en boucle si la température est au dessus de la température min
        //et inférieur à la température max
        while (mytemp->getTemp() >= temperatureMin && mytemp->getTemp() <= temperatureMax){      
          myStone->setLabel("labelcuisson", "");
          mytemp->getTemp();
          char strTemp[50];
          sprintf(strTemp, "%g", mytemp->getTemp());
          nbreSecondes++; 
          sprintf(strSec, "%d", nbreSecondes);
          delay(400);
          myStone->setLabel("labeltemperature", strTemp);
          delay(200);
          myStone->setLabel("labelseconde", strSec);
          delay(400);
          //si la température est en dessous de la température min
          if(mytemp->getTemp() < temperatureMin){
            myStone->setLabel("labelcuisson", "Température trop faible !");
            
          }
          //si la température est au dessus de la température max
          else if(mytemp->getTemp() > temperatureMax){
            myStone->setLabel("labelcuisson", "Température trop élevée !");    
          }
          //Si le timer atteint 20 secondes
          else if (nbreSecondes == 20){
            myStone->setLabel("labelcuisson", "Fin de la cuisson !");
            Serial.println("Le bois est SEC");
            break;
          }
          
        }
      }
    }
    //Si la température est en dessous de la température min avant d'entré dans la boucle
    else if(mytemp->getTemp() <= temperatureMin){
      myStone->setLabel("labelcuisson", "Température trop faible");
      myStone->setLabel("label", "<22.5");
    }
    //Si la température est au dessus de la température max avant d'entré dans la boucle
    else if (mytemp->getTemp() >= temperatureMax){
      myStone->setLabel("labelcuisson", "Température trop élevée");
      myStone->setLabel("label", ">27.5");
    }


  }

}


void setup() {
  Serial.begin(9600);
  mytemp->init();
  myStone = new MyStone(115200, SERIAL_8N1, RXD2, TXD2);
  std::cout << std::string("Début de l'exemple Stone de base pour le ESP32")  << "\n";
}

void loop() { 
  
  readStoneData(); //Appel de la fonction readStoneData
  delay(2000);
  myStone->changePage("window1");
  }