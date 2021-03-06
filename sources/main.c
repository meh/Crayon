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

#include <stdio.h>
#include <getopt.h>

#include <Crayon.h>

int
main (int argc, char** argv)
{
    RegexpMatches* matches;
    size_t         i;
    
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <regexp> <string>\n", argv[0]);
        return 1;
    }

    printf("Matching \"%s\" with /%s/\n\n", argv[2], argv[1]);

    matches = CR_MatchRegexpString2(argv[1], 0, argv[2]);

    if (matches == NULL) {
        puts("The regexp didn't match the string.");
    }
    else {
        printf("The matched string is: %s\n", matches->item[0]->data);

        for (i = 1; i < matches->length; i++) {
            printf("%lu: %s\n", i, matches->item[i]->data);
        }
    }

    return 0;
}

