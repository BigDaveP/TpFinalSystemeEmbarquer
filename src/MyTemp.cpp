/**
    Gestion d'un bouton pour permettre au système de démarrer
    @file MyTemp.cpp
    @author David Pigeon
    @version 1.0 12-05-2022  
*/
#include "MyTemp.h"

#define DHTTYPE    DHT22
#define DHTPIN 22

MyTemp::MyTemp(){
}

MyTemp::~MyTemp() {
}
//Intialisation de la classe MyTemp
void MyTemp::init() {
  dht = new DHT_Unified(DHTPIN, DHTTYPE);
  dht->begin();
}
// Permet d'obtenir la température du senseur DHT22
float MyTemp::getTemp(){
    sensors_event_t event;
    dht->temperature().getEvent(&event);
    return event.temperature;
}