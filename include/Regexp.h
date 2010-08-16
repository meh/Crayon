/*
  Crayon, a C web framework.

  Copyleft meh. [http://meh.doesntexist.org | meh.ffff@gmail.com]
 
  This file is part of Crayon.
 
  Crayon is free software: you can redistribute it and/or modify
  it under the terms of the GNU Affero General Public License as published
  by the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
 
  Crayon is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Affero General Public License for more details.
 
  You should have received a copy of the GNU Affero General Public License
  along with Crayon. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _CRAYON_REGEXP_H
#define _CRAYON_REGEXP_H

#include <pcre.h>

#include <Crayon/Common.h>
#include <Crayon/String.h>

#define REGEXP_I PCRE_CASELESS
#define REGEXP_X PCRE_EXTENDED
#define REGEXP_M PCRE_MULTILINE
#define REGEXP_S PCRE_DOTALL

typedef struct Regexp {
    char* string;
    int   options;

    pcre*       pattern;
    pcre_extra* study;
} Regexp;

typedef struct RegexpMatches {
    size_t   length;
    String** item;
} RegexpMatches;

Regexp* CR_CreateRegexp (char* regexp, int options);

void CR_DestroyRegexp (Regexp* regexp);

void CR_DestroyRegexpKeepString (Regexp* regexp);

RegexpMatches* CR_CreateRegexpMatches (size_t length);

void CR_DestroyRegexpMatches (RegexpMatches* object);

RegexpMatches* CR_MatchRegexp (Regexp* regexp, String* string);

RegexpMatches* CR_MatchRegexpString (char* regexp, int options, String* string);

RegexpMatches* CR_MatchRegexpString2 (char* regexp, int options, char* string);

bool CR_TestRegexp (Regexp* regexp, String* string);

#endif
