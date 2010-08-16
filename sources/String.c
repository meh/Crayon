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

#include <Crayon/String.h>

size_t
cr_UTF8_nextCharLength (char data)
{
    if ((data & 0x80) == 0x00) {
        return 1;
    }
    else if ((data & 0xE0) == 0xC0) {
        return 2;
    }
    else if ((data & 0xF0) == 0xE0) {
        return 3;
    }
    else if ((data & 0xF8) == 0xF0) {
        return 4;
    }
    else {
        return 0;
    }
}

size_t
cr_UTF8_strlen (char* data)
{
    size_t result  = 0;
    size_t i       = 0;

    while (data[i] != '\0') {
        i += cr_UTF8_nextCharLength(data[i]);
        result++;
    }

    return result;
}

size_t
cr_UTF8_offset (char* data, size_t offset)
{
    size_t result = 0;
    size_t i;

    for (i = 0; i < offset; i++) {
        result += cr_UTF8_nextCharLength(data[result]);
    }

    return result;
}

String*
CR_CreateString (char* data)
{
    String* result = malloc(sizeof(String));

    result->data      = data;
    result->length    = cr_UTF8_strlen(data);
    result->rawLength = strlen(data);

    return result;
}

String*
CR_CreateStringFromOffset (String* string, int offset, int limit)
{
    char* data;

    if (offset >= 0 && ((unsigned) offset) > string->length) {
        return NULL;
    }

    data = string->data;

    if (offset >= 0) {
        data += cr_UTF8_offset(string->data, offset);
    }

    return CR_CreateString(strndup(data, cr_UTF8_offset(data, limit)));
}

void
CR_DestroyString (String* string)
{
    free(string->data);
    free(string);
}

void
CR_DestroyStringKeepData (String* string)
{
    free(string);
}

String*
CR_CharAt (String* string, size_t index)
{
    return CR_CreateStringFromOffset(string, index, 1);
}

String*
CR_CopyString (String* string)
{
    String* result = malloc(sizeof(String));

    result->data      = strdup(string->data);
    result->length    = string->length;
    result->rawLength = string->rawLength;

    return result;
}
