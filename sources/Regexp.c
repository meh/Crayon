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

#include <Crayon/Regexp.h>

Regexp*
CR_CreateRegexp (char* string, int options)
{
    Regexp* result;
    pcre*   pattern;

    // If the pcre compilation fails
    if ((pattern = pcre_compile(string, options, NULL, NULL, NULL)) == NULL) {
        return NULL;
    }

    result          = malloc(sizeof(Regexp));
    result->string  = string;
    result->options = options | PCRE_UTF8 | PCRE_EXTRA;
    result->pattern = pattern;
    result->study   = pcre_study(pattern, 0, NULL);

    return result;
}

void
CR_DestroyRegexp (Regexp* regexp)
{
    if (regexp->study) {
        free(regexp->study);
    }

    pcre_free(regexp->pattern);

    free(regexp->string);

    free(regexp);
}

RegexpMatches*
CR_CreateRegexpMatches (size_t length)
{
    RegexpMatches* result;
    
    result         = malloc(sizeof(RegexpMatches));
    result->length = length;
    result->item   = malloc(length * sizeof(String*));

    return result;
}

void
CR_DestroyRegexpMatches (RegexpMatches* object)
{
    size_t i;
    for (i = 0; i < object->length; i++) {
        if (object->item[i] != NULL) {
            CR_DestroyString(object->item[i]);
        }
    }
}

RegexpMatches*
CR_MatchRegexp (Regexp* regexp, String* string)
{
    RegexpMatches* matches;

    int    length;
    size_t ovectorsize = 0;
    int*   ovector;

    char* substr_start;
    int   substr_length;
    char* tmp;

    size_t i;

    pcre_fullinfo(regexp->pattern, regexp->study, PCRE_INFO_CAPTURECOUNT, &ovectorsize);
    ovector = malloc((ovectorsize * 3) * sizeof(int));

    length = pcre_exec(regexp->pattern, regexp->study, string->data, string->rawLength, 0, 0, ovector, ovectorsize);

    if (length < 0) {
        return NULL;
    }

    matches = CR_CreateRegexpMatches(length + 1);
    matches->item[0] = CR_CopyString(string);

    for (i = 1; i < matches->length; i++) {
        substr_start  = string->data + ovector[2 * (i - 1)];
        substr_length = ovector[2 * (i - 1) + 1] - ovector[2 * (i - 1)];

        tmp = malloc(substr_length * sizeof(char));
        memcpy(tmp, substr_start, substr_length * sizeof(char));

        matches->item[i] = CR_CreateString(tmp);
    }

    free(ovector);

    return matches;
}

bool
CR_TestRegexp (Regexp* regexp, String* string)
{
    size_t ovectorsize = 0;
    int*   ovector;
    int    result;

    pcre_fullinfo(regexp->pattern, regexp->study, PCRE_INFO_CAPTURECOUNT, &ovectorsize);
    ovector = malloc((ovectorsize * 3) * sizeof(int));

    result = pcre_exec(regexp->pattern, regexp->study, string->data, string->rawLength, 0, 0, ovector, ovectorsize);

    free(ovector);

    return result >= 0;
}
