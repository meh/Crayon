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

#ifndef _CRAYON_STRING_H
#define _CRAYON_STRING_H

#include <Crayon/Common.h>

typedef struct String {
    size_t length;
    char*  data;

    size_t rawLength;
} String;

String* CR_CreateString (char* data);

String* CR_CreateStringFromOffset (String* string, int offset, int limit);

void CR_DestroyString (String* string);

void CR_DestroyStringKeepData (String* string);

String* CharAt (String* string, size_t index);

String* CR_CopyString (String* string);

#endif
