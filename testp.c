#include <stdio.h>

int main (int argc, char *argv)
{
  FILE *fptr;
  char string[10];  //Vet inte storleken innan jag läser? Varför funkar 0!?
  char string2[2];
  char string3[100];
  fptr = fopen("testing_p", "w");
  if(fptr)
    {
      fprintf(fptr, "test\n");
      fputs("second_input\n", fptr);
      fputs("third_input\n", fptr);
      fclose(fptr);
    }

  fopen("testing_p", "r");
  if(fptr)
    {
      fscanf(fptr, "%s %s", string, string2);
      fscanf(fptr, "%s", string3);
      printf("text in file = %s\n", string);
      printf("text in file = %s and %s\n", string, string2);
      printf("and %s\n", string3);
      //Hur läser man hela raden? Varunamn kan innehålla mellanslag
    }

  /*
    namn
    ...
    ...
    ...
    nr_nyllor
    ----- while(i < nr_hyllor)
    läs hyllor

    namn
    ...
    ...
    ...
   */

  return 0;
}
