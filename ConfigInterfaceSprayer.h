/*
  Config file for sprayer interface
Copyright (C) 2011-2015 J.A. Woltjer.

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

#ifndef ConfigInterfaceSprayer_h
#define ConfigInterfaceSprayer_h

//#define GPS
//#define VOORSERIE
//#define DEBUG

#ifndef VOORSERIE
// Defines for io ports
// Digital debounced inputs
#define MODE_PIN          4
#define LEFT_BUTTON       5
#define RIGHT_BUTTON      6
//#define xxx             7
//#define xxx             8
//#define xxx             13

#else

#define MODE_PIN          6
#define LEFT_BUTTON       5
#define RIGHT_BUTTON      4

#endif

#endif