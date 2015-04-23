/*
  InterfaceSprayer - a libary for the MeijWorks interface
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

#include "InterfaceSprayer.h"

// -----------
// Constructor
// -----------
#ifdef GPS
InterfaceSprayer::InterfaceSprayer(LiquidCrystal_I2C * _lcd,
                     ImplementSprayer * _implement,
                     VehicleTractor * _tractor,
                     VehicleGps * _gps){
  // Pin ass:ignments and configuration
  // Schmitt triggered inputs
  pinMode(LEFT_BUTTON, INPUT);
  digitalWrite(LEFT_BUTTON, LOW);
  pinMode(RIGHT_BUTTON, INPUT);
  digitalWrite(RIGHT_BUTTON, LOW);
  pinMode(MODE_PIN, INPUT);
  digitalWrite(MODE_PIN, LOW);
  
  // Mode
  mode = 2; // MANUAL
  
  // Button flag
  buttons = 0;
  button1_flag = false;
  button2_flag = false;

  // Connected classes
  lcd = _lcd;
  implement = _implement;
  tractor = _tractor;
  gps = _gps;
}

#else
InterfaceSprayer::InterfaceSprayer(LiquidCrystal_I2C * _lcd,
                     ImplementSprayer * _implement,
                     VehicleTractor * _tractor){
  // Pin assignments and configuration
  // Schmitt triggered inputs
  pinMode(LEFT_BUTTON, INPUT);
  pinMode(RIGHT_BUTTON, INPUT);
  pinMode(MODE_PIN, INPUT);

  digitalWrite(LEFT_BUTTON, LOW);
  digitalWrite(RIGHT_BUTTON, LOW);
  digitalWrite(MODE_PIN, LOW);
  
  // Mode
  mode = 2; // MANUAL
  
  // Button flag
  buttons = 0;
  button1_flag = false;
  button2_flag = false;

  // Connected classes
  lcd = _lcd;
  implement = _implement;
  tractor = _tractor;
  
  simTime = millis();
}
#endif

// ------------------------
// Method for updating mode
// ------------------------
void InterfaceSprayer::update(){
  // Check buttons
  checkButtons(500, 0);

  // Check for mode change
  
  // ---------
  // Calibrate
  // ---------
  if(buttons == 2){
    mode = 3;
        
    // Calibrate
    calibrate();
  }
  // ---
  // Off
  // ---
  else if(!digitalRead(MODE_PIN) || !tractor->getHitch()){  // || !implement->getSwitch()){
    // set mode to off
    mode = 2;
    simTime = millis();
  }
  else {
#ifdef GPS
    // -----
    // Start
    // -----
    if (//millis() - gps->getGgaFixAge() > 2000 ||
        //millis() - gps->getVtgFixAge() > 2000 ||
        //millis() - gps->getXteFixAge() > 2000 ||
        //gps->getQuality() != 4 ||
        !gps->simSpeed()){
      
      // set mode to sim
      mode = 1;
    }
    // --------- 
    // Automatic
    // ---------
    else {
      // set mode to automatic
      mode = 0;
    }
#else
    //-----
    // Hold
    //-----
    if ((!tractor->simSpeed() &&
      millis() - simTime < tractor->getSimTime() * 1000)){// ||
      //tractor->getSim()){
      
      // set mode to sim
      mode = 1;
    }
    // ---
    // SIM
    // ---
    if (tractor->getSim()){
      mode = 4;
    }
//---------- 
// Automatic
//----------
    else {
      // set mode to automatic
      mode = 0;
    }
#endif
  }
  
  // Update tractor or gps (speed and hitch)
#ifdef GPS
  gps->update();
#endif
  tractor->update(mode);
  
  // Update implement and adjust
  implement->update(mode, buttons);
  
  // Update screen (no rewrite) and write one character
  updateScreen(0); 
  lcd->write_screen(1);
}

// --------------------------
// Method for updating screen
// --------------------------
void InterfaceSprayer::updateScreen(boolean _rewrite){  
  int temp = 0;
  int temp2 = 0;

  // Update screen
  if (_rewrite){
    // Regel 0
    lcd->write_buffer(L_FLOW, 0);

    // Regel 1
    lcd->write_buffer(L_MEASURE, 1);

    // Regel 2
    lcd->write_buffer(L_SPEED, 2);

    // Regel 3
    lcd->write_buffer(L_BLANK, 3);
    
    lcd->write_screen(-1);
  }

  // Regel 0
  temp = implement->getDose();

  if (temp > 99){
    lcd->write_buffer(temp / 100 + '0', 0, 12);
    temp = temp % 100;
    lcd->write_buffer(temp / 10 + '0', 0, 13);
    temp = temp % 10;
    lcd->write_buffer(temp + '0', 0, 14);
  }
  else if (temp > 9){
    lcd->write_buffer(' ', 0, 12);
    lcd->write_buffer(temp / 10 + '0', 0, 13);
    temp = temp % 10;
    lcd->write_buffer(temp + '0', 0, 14);
  }
  else {
    lcd->write_buffer(' ', 0, 12);
    lcd->write_buffer(' ', 0, 13);
    lcd->write_buffer(temp + '0', 0, 14);
  }

  // Regel 1
  temp = implement->getActualDose();

  if (temp > 99){
    lcd->write_buffer(temp / 100 + '0', 1, 12);
    temp = temp % 100;
    lcd->write_buffer(temp / 10 + '0', 1, 13);
    temp = temp % 10;
    lcd->write_buffer(temp + '0', 1, 14);
  }
  else if (temp > 9){
    lcd->write_buffer(' ', 1, 12);
    lcd->write_buffer(temp / 10 + '0', 1, 13);
    temp = temp % 10;
    lcd->write_buffer(temp + '0', 1, 14);
  }
  else {
    lcd->write_buffer(' ', 1, 12);
    lcd->write_buffer(' ', 1, 13);
    lcd->write_buffer(temp + '0', 1, 14);
  }

  // Regel 2
#ifdef GPS
  temp2 = gps->getSpeedKmh();
#else
  temp2 = tractor->getSpeedKmh();
#endif
  temp = abs(temp2);

  if (temp > 99){
    if (temp2 < 0){
      lcd->write_buffer('-', 2, 10);
    }
    else {
      lcd->write_buffer(' ', 2, 10);
    }
    lcd->write_buffer(temp / 100 + '0', 2, 11);
    temp = temp % 100;
    lcd->write_buffer(temp / 10 + '0', 2, 12);
    temp = temp % 10;
    lcd->write_buffer('.', 2, 13);
    lcd->write_buffer(temp + '0', 2, 14);
  }
  else if (temp > 9){
    lcd->write_buffer(' ', 2, 10);
    if (temp2 < 0){
      lcd->write_buffer('-', 2, 11);
    }
    else {
      lcd->write_buffer(' ', 2, 11);
    }
    lcd->write_buffer(temp / 10 + '0', 2, 12);
    temp = temp % 10;
    lcd->write_buffer('.', 2, 13);
    lcd->write_buffer(temp + '0', 2, 14);
  }
  else {
    lcd->write_buffer(' ', 2, 10);
    lcd->write_buffer(' ', 2, 11);
    if (temp2 < 0){
      lcd->write_buffer('-', 2, 12);
    }
    else {
      lcd->write_buffer(' ', 2, 12);
    }
    lcd->write_buffer('.', 2, 13);
    lcd->write_buffer(temp + '0', 2, 14);
  }

  // Regel  3
  if (implement->getFlag()){
    temp2 = implement->getVolume();
    temp = abs(temp2);
    lcd->write_buffer("Volume:        L    ", 3);
  }
  else {
    temp2 = (tractor->getDistance() * implement->getWidth()) / 1000;
    temp = abs(temp2);
    lcd->write_buffer("HA:            H    ", 3);
  }

  if (temp > 9999){
    lcd->write_buffer(temp / 10000 + '0', 3, 8);
    temp = temp % 10000;
    lcd->write_buffer(temp / 1000 + '0', 3, 9);
    temp = temp % 1000;
    lcd->write_buffer(temp / 100 + '0', 3, 10);
    temp = temp % 100;
    lcd->write_buffer('.', 3, 11);
    lcd->write_buffer(temp / 10 + '0', 3, 12);
    temp = temp % 10;
    lcd->write_buffer(temp + '0', 3, 13);
  }  
  if (temp > 999){
    lcd->write_buffer(' ', 3, 8);
    lcd->write_buffer(temp / 1000 + '0', 3, 9);
    temp = temp % 1000;
    lcd->write_buffer(temp / 100 + '0', 3, 10);
    temp = temp % 100;
    lcd->write_buffer('.', 3, 11);
    lcd->write_buffer(temp / 10 + '0', 3, 12);
    temp = temp % 10;
    lcd->write_buffer(temp + '0', 3, 13);
  }  
  else if (temp > 99){
    lcd->write_buffer(' ', 3, 8);
    lcd->write_buffer(' ', 3, 9);
    lcd->write_buffer(temp / 100 + '0', 3, 10);
    temp = temp % 100;
    lcd->write_buffer('.', 3, 11);
    lcd->write_buffer(temp / 10 + '0', 3, 12);
    temp = temp % 10;
    lcd->write_buffer(temp + '0', 3, 13);
  }
  else if (temp > 9){
    lcd->write_buffer(' ', 3, 8);
    lcd->write_buffer(' ', 3, 9);
    lcd->write_buffer(' ', 3, 10);
    lcd->write_buffer('.', 3, 11);
    lcd->write_buffer(temp / 10 + '0', 3, 12);
    temp = temp % 10;
    lcd->write_buffer(temp + '0', 3, 13);
  }
  else {
    lcd->write_buffer(' ', 3, 8);
    lcd->write_buffer(' ', 3, 9);
    lcd->write_buffer(' ', 3, 10);
    lcd->write_buffer('.', 3, 11);
    lcd->write_buffer('0', 3, 12);
    lcd->write_buffer(temp + '0', 3, 13);
  }
    
  switch (mode){
  case 0: // AUTO
    lcd->write_buffer('A', 3, 17);
    lcd->write_buffer('U', 3, 18);
    lcd->write_buffer('T', 3, 19);
    break;
  case 1: // START
    lcd->write_buffer('S', 3, 17);
    lcd->write_buffer(' ', 3, 18);
    lcd->write_buffer(' ', 3, 19);
#ifdef GPS
    if (!gps->minSpeed()){
      lcd->write_buffer('S', 3, 17);
      lcd->write_buffer('!', 3, 18);
    }
    else if (gps->getQuality() != 4){
      lcd->write_buffer('Q', 3, 17);
      lcd->write_buffer('!', 3, 18);
    }
    else {
      lcd->write_buffer('G', 3, 17);
      lcd->write_buffer('!', 3, 18);
    }
#else
	if (false){//!tractor->minSpeed()){
	  lcd->write_buffer('S', 3, 18);
    lcd->write_buffer('!', 3, 19);
    }
#endif
    break;
  case 2: // OFF
    lcd->write_buffer('O', 3, 17);
    lcd->write_buffer('F', 3, 18);
    lcd->write_buffer('F', 3, 19);
    break;
  case 4: // SIM
    lcd->write_buffer('S', 3, 17);
    lcd->write_buffer('I', 3, 18);
    lcd->write_buffer('M', 3, 19);
    break;
  }
}

// ---------------------------
// Method for checking buttons
// ---------------------------
int InterfaceSprayer::checkButtons(byte _delay1, byte _delay2){
  if (button1_flag){
    button1_timer = millis();
    button1_flag = false;
  }
  
  if (button2_flag){
    button2_timer = millis();
    button2_flag = false;
  }
  
  // Check for left/right button presses
  if(digitalRead(LEFT_BUTTON) && digitalRead(RIGHT_BUTTON)){
    if(millis() - button1_timer >= _delay1 * 4){ // delay * 4 because of byte type
      button1_flag = true;
      buttons = 2;
      return 2;
    }
    else{
      button2_flag = true;
      buttons = 0;
      return 0;
    }
  }
  else if(digitalRead(LEFT_BUTTON)){
    if(millis() - button2_timer >= _delay2){
      button2_flag = true;
      buttons = -1;
      return -1;
    }
    else{
      button1_flag = true;
      buttons = 0;
      return 0;
    }
  }
  else if(digitalRead(RIGHT_BUTTON)){
    if(millis() - button2_timer >= _delay2){
      button2_flag = true;
      buttons = 1;
      return 1;
    }
    else{
      button1_flag = true;
      buttons = 0;
      return 0;
    }
  }
  else{
    button1_flag = true;
    button2_flag = true;
    buttons = 0;
    return 0;
  }
}

// --------------------------------
// Method for calibrating implement
// --------------------------------
void InterfaceSprayer::calibrate(){
  // Stop any adjusting
  implement->stop();
    
  // Write complete screen
  lcd->write_screen(-1);

  // Temporary variables
  int _temp, _temp2, _temp3;
  
  // ----------
  // Simulation
  // ----------
  lcd->write_buffer(L_CAL_SIM, 0);
  lcd->write_buffer(L_CAL_ACCEPT, 1);
  lcd->write_buffer(L_CAL_DECLINE, 2);
  lcd->write_buffer(L_BLANK, 3);

    lcd->write_screen(-1);

  while(checkButtons(0, 0) != 0){
  }

  while(true){
    if(checkButtons(0, 0) == -1){
      // print message to LCD
      lcd->write_buffer(L_CAL_DECLINED, 1);
      lcd->write_buffer(L_BLANK, 2);
      lcd->write_buffer(L_BLANK, 3);

      lcd->write_screen(-1);
      break;
    }
    else if(checkButtons(0, 0) == 1){
      // Setting sim mode
      lcd->write_buffer(L_CAL_ADJUST, 1);
      lcd->write_buffer(L_CAL_ENTER, 2);
      
      lcd->write_screen(-1);

      _temp = tractor->getSim();

      while(true){
        lcd->write_screen(1);
        checkButtons(0, 255);

        if (buttons == 1){
          _temp = 1;
        }
        else if (buttons == -1){
          _temp = 0;
        }
        else if (buttons == 2){
          break;
        }

        if (_temp){
          lcd->write_buffer(L_CAL_ON, 3);
        }
        else {
          lcd->write_buffer(L_CAL_OFF, 3);
        }

      }
      lcd->write_buffer(L_CAL_DONE, 1);
      lcd->write_buffer(L_BLANK, 2);
      
      if (_temp){
        tractor->enableSim();
      }
      else {
        tractor->disableSim();
      }
      
      lcd->write_screen(-1);

      break;
    }
  }
  delay(1000);
  
  // ------------
  // Set simspeed
  // ------------
  if (true){
    lcd->write_buffer(L_CAL_SIMS, 0);
    lcd->write_buffer(L_CAL_ACCEPT, 1);
    lcd->write_buffer(L_CAL_DECLINE, 2);
    lcd->write_buffer(L_BLANK, 3);

      lcd->write_screen(-1);

    while(checkButtons(0, 0) != 0){
    }

    while(true){
      if(checkButtons(0, 0) == -1){
        // print message to LCD
        lcd->write_buffer(L_CAL_DECLINED, 1);
        lcd->write_buffer(L_BLANK, 2);
        lcd->write_buffer(L_BLANK, 3);

        lcd->write_screen(-1);
        break;
      }
      else if(checkButtons(0, 0) == 1){
        // Sim speed setting
        lcd->write_buffer(L_CAL_ADJUST, 1);
        lcd->write_buffer(L_CAL_ENTER, 2);
        lcd->write_buffer(L_CAL_SIMS_AD, 3);
        
        lcd->write_screen(-1);

        _temp = tractor->getSimSpeedKmh();
        //_temp2 = _temp / 10;

        //lcd->write_buffer(_temp2 + '0', 3, 14);
        //lcd->write_buffer(_temp % 10 + '0', 3, 15);

        while(checkButtons(0, 0) != 0){
        }

        while(true){
          lcd->write_screen(1);
          checkButtons(0, 255);

          if (buttons == 1){
            _temp ++;
          }
          else if (buttons == -1){
            _temp --;
          }
          else if (buttons == 2){
            break;
          }

          // calculate derived variables
          _temp2 = _temp / 10;

          // write all to screen
          lcd->write_buffer(_temp2 + '0', 3, 13);
          lcd->write_buffer('.', 3, 14);
          lcd->write_buffer(_temp % 10 + '0', 3, 15);

        }
        lcd->write_buffer(L_CAL_DONE, 1);
        lcd->write_buffer(L_BLANK, 2);
            
        lcd->write_screen(-1);

        tractor->setSimSpeedKmh(_temp);
        break;
      }
    }
    delay(1000);
  }

  // -----------
  // Set simtime
  // -----------
  if (true){
    lcd->write_buffer(L_CAL_SIMT, 0);
    lcd->write_buffer(L_CAL_ACCEPT, 1);
    lcd->write_buffer(L_CAL_DECLINE, 2);
    lcd->write_buffer(L_BLANK, 3);

      lcd->write_screen(-1);

    while(checkButtons(0, 0) != 0){
    }

    while(true){
      if(checkButtons(0, 0) == -1){
        // print message to LCD
        lcd->write_buffer(L_CAL_DECLINED, 1);
        lcd->write_buffer(L_BLANK, 2);
        lcd->write_buffer(L_BLANK, 3);

        lcd->write_screen(-1);
        break;
      }
      else if(checkButtons(0, 0) == 1){
        // Sim speed setting
        lcd->write_buffer(L_CAL_ADJUST, 1);
        lcd->write_buffer(L_CAL_ENTER, 2);
        lcd->write_buffer(L_CAL_SIMT_AD, 3);
        
        lcd->write_screen(-1);

        _temp = tractor->getSimTime();

        while(checkButtons(0, 0) != 0){
        }

        while(true){
          lcd->write_screen(1);
          checkButtons(0, 255);

          if (buttons == 1){
            _temp ++;
          }
          else if (buttons == -1){
            _temp --;
          }
          else if (buttons == 2){
            break;
          }

          // calculate derived variables
          _temp2 = _temp / 10;

          // write all to screen
          lcd->write_buffer(_temp2 + '0', 3, 14);
          lcd->write_buffer(_temp % 10 + '0', 3, 15);

        }
        lcd->write_buffer(L_CAL_DONE, 1);
        lcd->write_buffer(L_BLANK, 2);
            
        lcd->write_screen(-1);

        tractor->setSimTime(_temp);
        break;
      }
    }
    delay(1000);
  }
  
  // ----------------
  // SPEED calibration
  // ----------------
  lcd->write_buffer(L_CAL_SPEED, 0);
  lcd->write_buffer(L_CAL_ACCEPT, 1);
  lcd->write_buffer(L_CAL_DECLINE, 2);
  lcd->write_buffer(L_BLANK, 3);

  lcd->write_screen(-1);

  while(checkButtons(0, 0) != 0){
  }

  while(true){

    if(checkButtons(0, 0) == -1){
      // print message to LCD
      lcd->write_buffer(L_CAL_DECLINED, 1);
      lcd->write_buffer(L_BLANK, 2);
      lcd->write_buffer(L_BLANK, 3);

      lcd->write_screen(-1);

      break;
    }
    else if(checkButtons(0, 0) == 1){
      // SPEED calibration
      lcd->write_buffer(L_BLANK, 1);
      lcd->write_buffer(L_CAL_ENTER, 2);
      lcd->write_buffer(L_CAL_SPEED_AD, 3);

      lcd->write_screen(-1);

      // Loop through calibration process
      tractor->resetWheelspeedPulses();
      
      while(checkButtons(0, 0) != 0){
      }

      // Adjust loop
      while(true){ 
        lcd->write_screen(1);
  
        checkButtons(0, 255);
        
        _temp = tractor->calibrateSpeed(buttons);

        if (buttons == 2){
          break;
        }
        
        _temp = _temp / 25;
        
        // Calculate derived variables
        _temp2 = _temp / 10;
        _temp3 = _temp / 100;
        
        lcd->write_buffer(abs(_temp3) + '0', 3, 14);
        lcd->write_buffer(abs(_temp2) % 10 + '0', 3, 15);
        lcd->write_buffer(abs(_temp) % 10 + '0', 3, 16);
      }
      
      lcd->write_buffer(L_CAL_DONE, 1);
      lcd->write_buffer(L_BLANK, 2);

      lcd->write_screen(-1);

      break;
    }
  }
  delay(1000);
  
  // ----------------
  // FLOW calibration
  // ----------------
  lcd->write_buffer(L_CAL_FLOW, 0);
  lcd->write_buffer(L_CAL_ACCEPT, 1);
  lcd->write_buffer(L_CAL_DECLINE, 2);
  lcd->write_buffer(L_BLANK, 3);

  lcd->write_screen(-1);

  while(checkButtons(0, 0) != 0){
  }

  while(true){

    if(checkButtons(0, 0) == -1){
      // print message to LCD
      lcd->write_buffer(L_CAL_DECLINED, 1);
      lcd->write_buffer(L_BLANK, 2);
      lcd->write_buffer(L_BLANK, 3);

      lcd->write_screen(-1);

      break;
    }
    else if(checkButtons(0, 0) == 1){
      // FLOW calibration
      lcd->write_buffer(L_CAL_ADJUST, 1);
      lcd->write_buffer(L_CAL_ENTER, 2);
      lcd->write_buffer(L_CAL_FLOW_AD, 3);

      lcd->write_screen(-1);

      _temp = implement->getFlowCalibration();

      while(checkButtons(0, 0) != 0){
      }

      // Adjust loop
      while(true){ 
        lcd->write_screen(1);
        checkButtons(0, 255);

        if (buttons == 1){
          _temp++;
        }
        else if (buttons == -1){
          _temp--;
        }
        else if (buttons == 2){
          break;
        }
        // Calculate derived variables
        _temp2 = _temp / 10;
        _temp3 = _temp / 100;
        
        lcd->write_buffer(abs(_temp3) + '0', 3, 14);
        lcd->write_buffer(abs(_temp2) % 10 + '0', 3, 15);
        lcd->write_buffer(abs(_temp) % 10 + '0', 3, 16);
      }

      implement->setFlowCalibration(_temp);
      
      lcd->write_buffer(L_CAL_DONE, 1);
      lcd->write_buffer(L_BLANK, 2);

      lcd->write_screen(-1);

      break;
    }
  }
  delay(1000);
  
  // ---------------
  // PWM calibration
  // ---------------
  lcd->write_buffer(L_CAL_PWM, 0);
  lcd->write_buffer(L_CAL_ACCEPT, 1);
  lcd->write_buffer(L_CAL_DECLINE, 2);
  lcd->write_buffer(L_BLANK, 3);

  lcd->write_screen(-1);

  while(checkButtons(0, 0) != 0){
  }

  while(true){

    if(checkButtons(0, 0) == -1){
      // print message to LCD
      lcd->write_buffer(L_CAL_DECLINED, 1);
      lcd->write_buffer(L_BLANK, 2);
      lcd->write_buffer(L_BLANK, 3);

      lcd->write_screen(-1);

      break;
    }
    else if(checkButtons(0, 0) == 1){
      // PWM calibration
      lcd->write_buffer(L_BLANK, 1);
      lcd->write_buffer(L_BLANK, 2);
      lcd->write_buffer(L_CAL_PWM_AD, 3);

      lcd->write_screen(-1);

      // Loop through calibration process
      _temp = implement->calibratePump();
      
      _temp2 = _temp / 10;
      _temp3 = _temp / 100;

      lcd->write_buffer(L_CAL_DONE, 1);
      
      lcd->write_buffer(abs(_temp3) + '0', 3, 14);
      lcd->write_buffer(abs(_temp2) % 10 + '0', 3, 15);
      lcd->write_buffer(abs(_temp) % 10 + '0', 3, 16);       
        

      lcd->write_screen(-1);

      break;
    }
  }
  delay(2000);
  
  // ---------
  // Adjust KP
  // ---------
  lcd->write_buffer(L_CAL_KP, 0);
  lcd->write_buffer(L_CAL_ACCEPT, 1);
  lcd->write_buffer(L_CAL_DECLINE, 2);
  lcd->write_buffer(L_BLANK, 3);

  lcd->write_screen(-1);

  while(checkButtons(0, 0) != 0){
  }

  while(true){
    if(checkButtons(0, 0) == -1){
      // print message to LCD
      lcd->write_buffer(L_CAL_DECLINED, 1);
      lcd->write_buffer(L_BLANK, 2);

      lcd->write_screen(-1);
      break;
    }
    else if(checkButtons(0, 0) == 1){
      // Adjust KP
      lcd->write_buffer(L_CAL_ADJUST, 1);
      lcd->write_buffer(L_CAL_ENTER, 2);
      lcd->write_buffer(L_CAL_KP_AD, 3);

      lcd->write_screen(-1);
      
      _temp = implement->getKP();

      while(checkButtons(0, 0) != 0){
      }
      
      // Adjust loop
      while(true){
        lcd->write_screen(1);
        checkButtons(0, 255);

        if (buttons == 1){
          _temp ++;
        }
        else if (buttons == -1){
          _temp --;
        }
        else if (buttons == 2){
          break;
        }
        // Calculate derived variables
        _temp2 = _temp / 10;
        _temp3 = _temp / 100;

        // Write to screen
        lcd->write_buffer(_temp3 + '0', 3, 13);
        lcd->write_buffer('.', 3, 14);
        lcd->write_buffer(_temp2 % 10 + '0', 3, 15);
        lcd->write_buffer(_temp  % 10 + '0', 3, 16);
      }
      lcd->write_buffer(L_CAL_DONE, 1);
      lcd->write_buffer(L_BLANK, 2);

      lcd->write_buffer(_temp3 + '0', 3, 13);
      lcd->write_buffer('.', 3, 14);
      lcd->write_buffer(_temp2 % 10 + '0', 3, 15);
      lcd->write_buffer(_temp  % 10 + '0', 3, 16);

      lcd->write_screen(-1);

      implement->setKP(_temp);
      break;
    }
  }
  delay(1000);

  // ---------
  // Adjust KI
  // ---------
  lcd->write_buffer(L_CAL_KI, 0);
  lcd->write_buffer(L_CAL_ACCEPT, 1);
  lcd->write_buffer(L_CAL_DECLINE, 2);
  lcd->write_buffer(L_BLANK, 3);

  lcd->write_screen(-1);

  while(checkButtons(0, 0) != 0){
  }

  while(true){
    if(checkButtons(0, 0) == -1){
      // print message to LCD
      lcd->write_buffer(L_CAL_DECLINED, 1);
      lcd->write_buffer(L_BLANK, 2);
      lcd->write_buffer(L_BLANK, 3);

      lcd->write_screen(-1);
      break;
    }
    else if(checkButtons(0, 0) == 1){
      // Adjust KI
      lcd->write_buffer(L_CAL_ADJUST, 1);
      lcd->write_buffer(L_CAL_ENTER, 2);
      lcd->write_buffer(L_CAL_KI_AD, 3);

      lcd->write_screen(-1);

      _temp = implement->getKI();
/*      _temp2 = _temp / 10;
      _temp3 = _temp / 100;

      lcd->write_buffer(_temp3 + '0', 3, 13);
      lcd->write_buffer('.', 3, 14);
      lcd->write_buffer(_temp2 % 10 + '0', 3, 15);
      lcd->write_buffer(_temp  % 10 + '0', 3, 16);
*/
      while(checkButtons(0, 0) != 0){
      }

      while(true){
        lcd->write_screen(1);
        checkButtons(0, 255);
        
        if (buttons == 1){
          _temp ++;
        }
        else if (buttons == -1){
          _temp --;
        }
        else if (buttons == 2){
          break;
        }
        // Calculate derived variables
        _temp2 = _temp / 10;
        _temp3 = _temp / 100;

        // Write to screen
        lcd->write_buffer(_temp3 + '0', 3, 13);
        lcd->write_buffer('.', 3, 14);
        lcd->write_buffer(_temp2 % 10 + '0', 3, 15);
        lcd->write_buffer(_temp  % 10 + '0', 3, 16);
      }
      lcd->write_buffer(L_CAL_DONE, 1);
      lcd->write_buffer(L_BLANK, 2);
      
      lcd->write_buffer(_temp3 + '0', 3, 13);
      lcd->write_buffer('.', 3, 14);
      lcd->write_buffer(_temp2 % 10 + '0', 3, 15);
      lcd->write_buffer(_temp  % 10 + '0', 3, 16);

      lcd->write_screen(-1);

      implement->setKI(_temp);
      break;
    }
  }
  delay(1000);

  // ---------
  // Adjust KD
  // ---------
  lcd->write_buffer(L_CAL_KD, 0);
  lcd->write_buffer(L_CAL_ACCEPT, 1);
  lcd->write_buffer(L_CAL_DECLINE, 2);
  lcd->write_buffer(L_BLANK, 3);

  lcd->write_screen(-1);

  while(checkButtons(0, 0) != 0){
  }

  while(true){
    if(checkButtons(0, 0) == -1){
      // print message to LCD
      lcd->write_buffer(L_CAL_DECLINED, 1);
      lcd->write_buffer(L_BLANK, 2);
      lcd->write_buffer(L_BLANK, 3);

      lcd->write_screen(-1);
      break;
    }
    else if(checkButtons(0, 0) == 1){
      // Adjust KD
      lcd->write_buffer(L_CAL_ADJUST, 1);
      lcd->write_buffer(L_CAL_ENTER, 2);
      lcd->write_buffer(L_CAL_KD_AD, 3);

      lcd->write_screen(-1);
      
      _temp = implement->getKD();
/*      _temp2 = _temp / 10;
      _temp3 = _temp / 100;

      lcd->write_buffer(_temp3 + '0', 3, 13);
      lcd->write_buffer('.', 3, 14);
      lcd->write_buffer(_temp2 % 10 + '0', 3, 15);
      lcd->write_buffer(_temp  % 10 + '0', 3, 16);
*/
      while(checkButtons(0, 0) != 0){
      }

      // Adjust loop
      while(true){
        lcd->write_screen(1);
        checkButtons(0, 255);
        
        if (buttons == 1){
          _temp ++;
        }
        else if (buttons == -1){
          _temp --;
        }
        else if (buttons == 2){
          break;
        }
        // Calculate derived variables
        _temp2 = _temp / 10;
        _temp3 = _temp / 100;

        // Write to screen
        lcd->write_buffer(_temp3 + '0', 3, 13);
        lcd->write_buffer('.', 3, 14);
        lcd->write_buffer(_temp2 % 10 + '0', 3, 15);
        lcd->write_buffer(_temp  % 10 + '0', 3, 16);
      }
      lcd->write_buffer(L_CAL_DONE, 1);
      lcd->write_buffer(L_BLANK, 2);
      
      lcd->write_buffer(_temp3 + '0', 3, 13);
      lcd->write_buffer('.', 3, 14);
      lcd->write_buffer(_temp2 % 10 + '0', 3, 15);
      lcd->write_buffer(_temp  % 10 + '0', 3, 16);

      lcd->write_screen(-1);

      implement->setKD(_temp);
      break;
    }
  }
  delay(1000);
  
  // -----------------
  // Teeth calibration
  // -----------------
  lcd->write_buffer(L_CAL_TEETH, 0);
  lcd->write_buffer(L_CAL_ACCEPT, 1);
  lcd->write_buffer(L_CAL_DECLINE, 2);
  lcd->write_buffer(L_BLANK, 3);

  lcd->write_screen(-1);

  while(checkButtons(0, 0) != 0){
  }

  while(true){
    if(checkButtons(0, 0) == -1){
      // print message to LCD
      lcd->write_buffer(L_CAL_DECLINED, 1);
      lcd->write_buffer(L_BLANK, 2);
      lcd->write_buffer(L_BLANK, 3);

      lcd->write_screen(-1);
      break;
    }
    else if(checkButtons(0, 0) == 1){
      // Width calibration
      lcd->write_buffer(L_CAL_ADJUST, 1);
      lcd->write_buffer(L_CAL_ENTER, 2);
      lcd->write_buffer(L_CAL_TEETH_AD, 3);
      
      lcd->write_screen(-1);

      _temp = implement->getTeeth();
/*      _temp2 = _temp / 10;

      lcd->write_buffer(_temp2 + '0', 3, 15);
      lcd->write_buffer(_temp % 10 + '0', 3, 16);
*/
      while(checkButtons(0, 0) != 0){
      }

      while(true){
        lcd->write_screen(1);
        checkButtons(0, 255);

        if (buttons == 1){
          _temp ++;
        }
        else if (buttons == -1){
          _temp --;
        }
        else if (buttons == 2){
          break;
        }
        // calculate derived variables
        _temp2 = _temp / 10;

        // write all to screen
        lcd->write_buffer(_temp2 + '0', 3, 15);
        lcd->write_buffer(_temp % 10 + '0', 3, 16);
      }
      lcd->write_buffer(L_CAL_DONE, 1);
      lcd->write_buffer(L_BLANK, 2);

      lcd->write_buffer(_temp2 + '0', 3, 15);
      lcd->write_buffer(_temp % 10 + '0', 3, 16);

      lcd->write_screen(-1);

      implement->setTeeth(_temp);
      break;
    }
  }
  delay(1000);

  // -----------------
  // Pumps calibration
  // -----------------
  lcd->write_buffer(L_CAL_PUMPS, 0);
  lcd->write_buffer(L_CAL_ACCEPT, 1);
  lcd->write_buffer(L_CAL_DECLINE, 2);
  lcd->write_buffer(L_BLANK, 3);

  lcd->write_screen(-1);

  while(checkButtons(0, 0) != 0){
  }

  while(true){
    if(checkButtons(0, 0) == -1){
      // print message to LCD
      lcd->write_buffer(L_CAL_DECLINED, 1);
      lcd->write_buffer(L_BLANK, 2);
      lcd->write_buffer(L_BLANK, 3);

      lcd->write_screen(-1);
      break;
    }
    else if(checkButtons(0, 0) == 1){
      // Width calibration
      lcd->write_buffer(L_CAL_ADJUST, 1);
      lcd->write_buffer(L_CAL_ENTER, 2);
      lcd->write_buffer(L_CAL_PUMPS_AD, 3);
      
      lcd->write_screen(-1);

      _temp = implement->getPumps();
/*      _temp2 = _temp / 10;

      lcd->write_buffer(_temp2 + '0', 3, 15);
      lcd->write_buffer(_temp % 10 + '0', 3, 16);
*/
      while(checkButtons(0, 0) != 0){
      }

      while(true){
        lcd->write_screen(1);
        checkButtons(0, 255);

        if (buttons == 1){
          _temp ++;
        }
        else if (buttons == -1){
          _temp --;
        }
        else if (buttons == 2){
          break;
        }
        // calculate derived variables
        _temp2 = _temp / 10;

        // write all to screen
        lcd->write_buffer(_temp2 + '0', 3, 15);
        lcd->write_buffer(_temp % 10 + '0', 3, 16);
      }
      lcd->write_buffer(L_CAL_DONE, 1);
      lcd->write_buffer(L_BLANK, 2);

      lcd->write_screen(-1);

      implement->setPumps(_temp);
      break;
    }
  }
  delay(1000);

  // -----------------
  // Width calibration
  // -----------------
  lcd->write_buffer(L_CAL_WIDTH, 0);
  lcd->write_buffer(L_CAL_ACCEPT, 1);
  lcd->write_buffer(L_CAL_DECLINE, 2);
  lcd->write_buffer(L_BLANK, 3);

  lcd->write_screen(-1);

  while(checkButtons(0, 0) != 0){
  }

  while(true){
    if(checkButtons(0, 0) == -1){
      // print message to LCD
      lcd->write_buffer(L_CAL_DECLINED, 1);
      lcd->write_buffer(L_BLANK, 2);

      lcd->write_screen(-1);
      break;
    }
    else if(checkButtons(0, 0) == 1){
      // Width calibration
      lcd->write_buffer(L_CAL_ADJUST, 1);
      lcd->write_buffer(L_CAL_ENTER, 2);
      lcd->write_buffer(L_CAL_WIDTH_AD, 3);
      
      lcd->write_screen(-1);

      _temp = implement->getWidth();
/*      _temp2 = _temp / 10;

      lcd->write_buffer(_temp2 + '0', 3, 17);
      lcd->write_buffer(_temp % 10 + '0', 3, 18);
      lcd->write_buffer('0', 3, 19);
*/
      while(checkButtons(0, 0) != 0){
      }

      while(true){
        lcd->write_screen(1);
        checkButtons(0, 255);

        if (buttons == 1){
          _temp ++;
        }
        else if (buttons == -1){
          _temp --;
        }
        else if (buttons == 2){
          break;
        }
        // calculate derived variables
        _temp2 = _temp / 10;

        // write all to screen
        lcd->write_buffer(_temp2 + '0', 3, 17);
        lcd->write_buffer(_temp % 10 + '0', 3, 18);
        lcd->write_buffer('0', 3, 19);
      }
      lcd->write_buffer(L_CAL_DONE, 1);
      lcd->write_buffer(L_BLANK, 2);

      lcd->write_screen(-1);

      implement->setWidth(_temp);
      break;
    }
  }
  delay(1000);
  
  // ----------------------
  // Store calibration data
  // ----------------------
  lcd->write_buffer(L_CAL_COMPLETE, 0);
  lcd->write_buffer(L_CAL_ACCEPT, 1);
  lcd->write_buffer(L_CAL_DECLINE, 2);
  lcd->write_buffer(L_BLANK, 3);

  lcd->write_screen(-1);

  while(checkButtons(0, 0) != 0){
  }

  while(true){
    if(checkButtons(0, 0) == -1){
      // print message to LCD
      lcd->write_buffer(L_CAL_COMPLETE, 0);
      lcd->write_buffer(L_CAL_DECLINED, 1);
      lcd->write_buffer(L_CAL_NOSAVE, 2);
      lcd->write_buffer(L_BLANK, 3);

      lcd->write_screen(-1);

      implement->resetCalibration();
      tractor->resetCalibration();

      break;
    }
    else if(checkButtons(0, 0) == 1){
      // Commit data
      implement->commitCalibration();
      tractor->commitCalibration();
      
      // Print message to LCD
      lcd->write_buffer(L_CAL_COMPLETE, 0);
      lcd->write_buffer(L_CAL_DDONE, 1);
      lcd->write_buffer(L_CAL_SAVE, 2);
      lcd->write_buffer(L_BLANK, 3);

      lcd->write_screen(-1);

      break;
    }
  }
  delay(1000);
  
  // After calibration rewrite total screen
  updateScreen(1);
  lcd->write_screen(-1);
}
