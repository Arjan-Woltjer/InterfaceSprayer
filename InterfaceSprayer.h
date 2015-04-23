/*
  InterfaceSprayer - a library for the MeijWorks interface
 Copyright (C) 2011-2015 J.A. Woltjer.
 All rights reserved.
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef InterfaceSprayer_h
#define InterfaceSprayer_h

#include "LiquidCrystal_I2C.h"
#include "ImplementSprayer.h"
#include "VehicleTractor.h"
#include "ConfigInterfaceSprayer.h"
#include "Language.h"

#ifdef GPS
#include "VehicleGps.h"
#endif

// Software version of this library
#define INTERFACE_VERSION 0.1

class InterfaceSprayer {
private:
  //-------------
  // data members
  //-------------
  
  // Mode
  byte mode; // AUTO, SIM, MANUAL, CALIBRATE
  
  // Button flag and timer
  int buttons;
  bool button1_flag;
  bool button2_flag;
  unsigned long button1_timer;
  unsigned long button2_timer;
  
  unsigned long simTime;
  
  // Objects
  LiquidCrystal_I2C * lcd;
  ImplementSprayer * implement;
  VehicleTractor * tractor;

#ifdef GPS
  VehicleGps * gps;
#endif

public:
  // -----------------------------------------------------------
  // public member functions implemented in InterfaceSprayer.cpp
  // -----------------------------------------------------------

  // Constructor
#ifdef GPS
  InterfaceSprayer(LiquidCrystal_I2C * _lcd,
            ImplementSprayer * _implement,
            VehicleTractor * _tractor,
            VehicleGps * _gps);
#else
  InterfaceSprayer(LiquidCrystal_I2C * _lcd,
            ImplementSprayer * _implement,
            VehicleTractor * _tractor);
#endif
  void update();
  void updateScreen(boolean _rewrite);
  int checkButtons(byte _delay1, byte _delay2);
  void calibrate(); 
  
  inline int getButtons(){
    return buttons;
  };
};
#endif