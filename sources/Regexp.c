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

    pcre*       pattern;
    const char* error       = NULL;
    int         errorOffset = 0;

    options |= PCRE_UTF8 | PCRE_EXTRA;

    // If the pcre compilation fails
    if ((pattern = pcre_compile(string, options, &error, &errorOffset, NULL)) == NULL) {
        #ifdef CRAYON_DEBUG
        fprintf(stderr, "pcre_compile error:\nOffset: %d\nError: %s\n", errorOffset, error);
        #endif

        return NULL;
    }

    #ifdef CRAYON_DEBUG
    error = NULL;
    #endif

    result          = malloc(sizeof(Regexp));
    result->string  = string;
    result->options = options;
    result->pattern = pattern;
    result->study   = pcre_study(pattern, 0, &error);

    #ifdef CRAYON_DEBUG
    if (error) {
        fprintf(stderr, "pcre_study error:\nError: %s\n", error);
    }
    #endif

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

void
CR_DestroyRegexpKeepString (Regexp* regexp)
{
    if (regexp->study) {
        free(regexp->study);
    }

    pcre_free(regexp->pattern);

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

    int  length = 0;
    int* substrings;

    char* substr_start;
    int   substr_length;
    char* tmp;

    size_t i;

    pcre_fullinfo(regexp->pattern, regexp->study, PCRE_INFO_CAPTURECOUNT, &length);
    substrings = malloc((length = 3 * (length + 1)) * sizeof(int));

    length = pcre_exec(regexp->pattern, regexp->study, string->data, string->rawLength, 0, 0, substrings, length);

    if (length < 0) {
        return NULL;
    }

    matches = CR_CreateRegexpMatches(length);

    for (i = 0; i < matches->length; i++) {
        substr_start  = string->data + substrings[2 * i];
        substr_length = substrings[2 * i + 1] - substrings[2 * i];

        tmp = malloc((substr_length + 1) * sizeof(char));
        strncpy(tmp, substr_start, substr_length);

        matches->item[i] = CR_CreateString(tmp);
    }

    free(substrings);

    return matches;
}

RegexpMatches*
CR_MatchRegexpString (char* regexp, int options, String* string)
{
    Regexp*        re     = CR_CreateRegexp(regexp, options);
    RegexpMatches* result = CR_MatchRegexp(re, string);

    CR_DestroyRegexpKeepString(re);

    return result;
}

RegexpMatches*
CR_MatchRegexpString2 (char* regexp, int options, char* string)
{
    Regexp*        re     = CR_CreateRegexp(regexp, options);
    String*        str    = CR_CreateString(string);

    RegexpMatches* result = CR_MatchRegexp(re, str);

    CR_DestroyRegexpKeepString(re);
    CR_DestroyStringKeepData(str);

    return result;
}

bool
CR_TestRegexp (Regexp* regexp, String* string)
{
    size_t length = 0;
    int*   substrings;
    int    result;

    pcre_fullinfo(regexp->pattern, regexp->study, PCRE_INFO_CAPTURECOUNT, &length);
    substrings = malloc((length * 3) * sizeof(int));

    result = pcre_exec(regexp->pattern, regexp->study, string->data, string->rawLength, 0, 0, substrings, length);

    free(substrings);

    return result >= 0;
}
