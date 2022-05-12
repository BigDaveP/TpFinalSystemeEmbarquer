/* Copyright (C) 2022 David Pigeon
 * All rights reserved.
 *
 * Projet Domotique
 * Ecole du Web
* Cours Systèmes embarqués (c)2022
 *  
    @file     MyTemp.h
    @author   David Pigeon
    @version  1.0 12-05-2022 
    @description
      Gestion de la température

    platform = ESP32
    OS = Arduino
    Langage : C++

    Historique des versions
        Version    Date       Auteur       Description
        1.0        12-05-2022  David Pigeon       Première version du logiciel

**/
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Dht_U.h>

#ifndef MyTemp_H
#define MyTemp_H

class MyTemp
{
private:
    DHT_Unified *dht;
public:
    MyTemp();
    ~MyTemp();
    float getTemp();
    void init();
};



#endif