/*
  Language file for sprayercontrol
Copyright (C) 2011-2013 J.A. Woltjer.

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

// ---------------
// Serial messages
// ---------------
#define S_MEIJWORKS     "MeijWorks"
#define S_DEVICE        "Sprayercontrol v 1.00"
#define S_LIBRARY       "FarmGPS library v "
#define S_COPYRIGHT     "(c) 2011 - 2015 by J.A. Woltjer"
#define S_TIMES         "Times started: "
#define S_DIVIDE        "-------------------------------"

#define NEDERLANDS

// -----------
// Taal ENGELS TODO
// -----------
#ifdef ENGLISH

#define L_BLANK         "                    "

#define L_MEIJWORKS     "     MeijWorks      "
#define L_DEVICE        " Plantercontrol 1.00"
#define L_COPYRIGHT     "      (c) 2014      "
#define L_AUTHOR        "  by J.A. Woltjer   "

#define L_OFFSET        "Offset:             "
#define L_A_STEER       "Actual position:    "
#define L_SETPOINT      "Setpoint:           "
#define L_ANGLE         "Angle:              "

#define L_CAL_ACCEPT    "+ : accept          "
#define L_CAL_DECLINE   "- : cancel          "

#define L_CAL_DONE      "complete            "
#define L_CAL_DECLINED  "cancelled           "

#define L_CAL_ADJUST    "+ / - : to adjust   "
#define L_CAL_ENTER     "Both  : to accept   "

#define L_CAL_STEER     "Steer calibration   "
#define L_CAL_STEER_AD  "Adjust to        deg"

#define L_CAL_ANGLE     "Angle calibration   "
#define L_CAL_ANGLE_AD  "Adjust to        deg"

#define L_CAL_KP        "PID adjust KP       "
#define L_CAL_KP_AD     "KP:                 "

#define L_CAL_KI        "PID adjust KI       "
#define L_CAL_KI_AD     "KI:                 "

#define L_CAL_KD        "PID adjust KD       "
#define L_CAL_KD_AD     "KD:                 "

#define L_CAL_OFFSET    "Adjust offset       "
#define L_CAL_OFFSET_AD "Offset :         deg"

#define L_CAL_QUAL      "Correct RTK ident.  "
#define L_CAL_QUAL_AD   "Quality:            "

#define L_CAL_SPEED     "Speed calibration   "
#define L_CAL_SPEED_AD  "Accelerate to 10kph "

#define L_CAL_GPS       "GPS autodetect      "
#define L_CAL_GPS_DONE  "passed              "
#define L_CAL_GPS_FAIL  "failed...           "
#define L_CAL_GPS_M1    "Check cabling and   "
#define L_CAL_GPS_M2    "nmea output         "

#define L_CAL_COMPLETE  "Finish calibration  "
#define L_CAL_NOSAVE    "Data NOT saved      "
#define L_CAL_DDONE     "done                "
#define L_CAL_SAVE      "Data saved          "

// ---------------
// Taal NEDERLANDS
// ---------------
#elif defined NEDERLANDS

#define L_BLANK         "                    "

#define L_MEIJWORKS     "     MeijWorks      "
#define L_DEVICE        "Sprayercontrol  v0.1"
#define L_COPYRIGHT     "      (c) 2015      "
#define L_AUTHOR        "  by J.A. Woltjer   "

#define L_FLOW          "Afgifte:        L/Ha"
#define L_MEASURE       "Gemeten:        L/Ha"
#define L_SPEED         "Snelheid:       KM/h"
#define L_SETPOINT      "Setpoint:           "

#define L_CAL_ACCEPT    "+ : accepteren      "
#define L_CAL_DECLINE   "- : annuleren       "

#define L_CAL_DONE      "voltooid            "
#define L_CAL_DECLINED  "geannuleerd         "

#define L_CAL_ADJUST    "+ / - : verstellen  "
#define L_CAL_ENTER     "Beide : accepteren  "

#define L_CAL_ON        "                 aan"
#define L_CAL_OFF       "                 uit"

#define L_CAL_SIM       "Wijzig sim mode     "

#define L_CAL_SIMS      "Wijzig sim snelheid "
#define L_CAL_SIMS_AD   "Huidig:          kmh"

#define L_CAL_SIMT      "Wijzig sim tijd     "
#define L_CAL_SIMT_AD   "Huidig:          sec"

#define L_CAL_SPEED     "Snelheids calibratie"
#define L_CAL_SPEED_AD  "Gem. pulsen:        "

#define L_CAL_TEETH     "Wijzig aantal tanden"
#define L_CAL_TEETH_AD  "Huidig aantal:      "

#define L_CAL_PUMPS     "Wijzig aantal pompen"
#define L_CAL_PUMPS_AD  "Huidig aantal:      "

#define L_CAL_WIDTH     "Wijzig werkbreedte  "
#define L_CAL_WIDTH_AD  "Huidige breedte:    "

#define L_CAL_FLOW      "Flow calibratie     "
#define L_CAL_FLOW_AD   "Huidig:           cc"

#define L_CAL_PWM       "PWM calibratie      "
#define L_CAL_PWM_AD    "Volume/omw:        L"

#define L_CAL_KP        "PID wijzig KP       "
#define L_CAL_KP_AD     "KP:                 "

#define L_CAL_KI        "PID wijzig KI       "
#define L_CAL_KI_AD     "KI:                 "

#define L_CAL_KD        "PID wijzig KD       "
#define L_CAL_KD_AD     "KD:                 "

#define L_CAL_QUAL      "Corrigeer RTK ident."
#define L_CAL_QUAL_AD   "Quality:            "

#define L_CAL_GPS       "GPS autodetect      "
#define L_CAL_GPS_DONE  "geslaagd            "
#define L_CAL_GPS_FAIL  "mislukt...          "
#define L_CAL_GPS_M1    "Check kabels en     "
#define L_CAL_GPS_M2    "nmea output         "

#define L_CAL_COMPLETE  "Calibratie afronden "
#define L_CAL_NOSAVE    "Data NIET opgeslagen"
#define L_CAL_DDONE     "geslaagd            "
#define L_CAL_SAVE      "Data is opgeslagen  "

// ----------
// Taal Deens
// ----------
#elif defined DANSK

#define L_BLANK         "                    "

#define L_MEIJWORKS     "     MeijWorks      "
#define L_DEVICE        "  Plovstyring v0.74 "
#define L_COPYRIGHT     "      (c) 2013      "
#define L_AUTHOR        "    J.A. Woltjer    "

#define L_WIDTH         "Arbejdsbredde:      "
#define L_A_WIDTH       "Aktuelle bredde:    "
#define L_SLIP          "Slip:              %"
#define L_ANGLE           "ANGLE:                "

#define L_CAL_ACCEPT    "+ : acceptere       "
#define L_CAL_DECLINE   "- : annullere       "

#define L_CAL_DONE      "f�rdig              "
#define L_CAL_DECLINED  "annulleret          "

#define L_CAL_ADJUST    "+ / - : justere     "
#define L_CAL_ENTER     "Beide : acceptere   "

#define L_CAL_WIDTH     "Bredde kalibrering  "
#define L_CAL_WIDTH_AD  "Justere til       cm"

#define L_CAL_SHARES    "�ndre antal plovjern"
#define L_CAL_SHARES_AD "Antal plovjern :    "

#define L_CAL_MARGIN    "�ndre fejlmargen    "
#define L_CAL_MARGIN_AD "Fejlmargen :      cm"

#define L_CAL_MAXCOR    "�ndre correction    "
#define L_CAL_MAXCOR_AD "Max cor:          cm"

#define L_CAL_SIDE      "Corrigeer ploegzijde"
#define L_CAL_SIDE_AD   "Ploegt nu naar:     "

#define L_CAL_QUAL      "Corrigeer RTK ident."
#define L_CAL_QUAL_AD   "Quality:            "

#define L_CAL_SPEED     "Snelheids calibratie"
#define L_CAL_SPEED_AD  "Accelereer tot 10kmh"

#define L_CAL_GPS       "GPS autodetect      "
#define L_CAL_GPS_DONE  "succesfuld          "
#define L_CAL_GPS_FAIL  "mislykket...        "
#define L_CAL_GPS_M1    "Kontrollere kabler  "
#define L_CAL_GPS_M2    "og nmea output      "

#define L_CAL_COMPLETE  "Kalibrering f�rdig  "
#define L_CAL_DDONE     "f�rdig              "
#define L_CAL_NOSAVE    "Data IKKE er gemt   "
#define L_CAL_SAVE      "Data gemt           "

#endif
