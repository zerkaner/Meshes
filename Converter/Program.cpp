#include <stdio.h>
#include "Converter.h"

int main (int argc, char** argv) {

  if (argc != 3) {
    printf ("[Error] Invalid syntax!\n"
            "Usage: conv.exe <inputfile> <outputfile>\n");   
    return 0;
  }

  Converter converter = Converter ();
  converter.LoadFile (argv[1], argv[2]);

  return 0;
}