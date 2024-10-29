/*  wc - file words counter version 0.8

    Copyright (C) Apache License 2.0

    This program is free software: you can use or redistribute it under the terms of the project.

    Differences with others unix-like wc softwares:
       This is a different code, it means that both programs may do the samething but in different ways and algorithms
       This is specifically a NGCU project code, totally written from scratch meaning that there's not copy paste on this project

       By Yassir Daniel also known as Sam Jamsh

*/



#include <stdio.h>
#include <wchar.h>
#include <locale.h>
//#include <string.h> // temp lib


#define VERSION "1.0"
#define WEBPAGE "https://github.com/samjamsh/wc"
#define AUTHOR "Sam Jamsh"
#define SOCIAL "@sam.jamsh"
#define EMAIL "th3cyb3rguy@protonmail.com"


typedef char bool;
bool true = 1;
bool false = 0;
bool ERROR = 1;
bool SUCCESS = 0;


unsigned int file_chars = 0;
unsigned int file_bytes = 0;
unsigned int file_breaklines = 0;
unsigned int file_words = 0;
unsigned int longest_line = 0;

int parameters_lenght;
char **parameters;
int files = 0; // files lenght (quantity)
bool p_option; // probable (possible) option choiced/given
bool filewc_ret; // file_wc return

typedef struct {
    bool chars;
    bool bytes;
    bool breaklines;
    bool words;
    bool longestline;
} options;


void print_info();


bool cmpstr(char *string1, char *string2)
{
    char s1_char = 1, s2_char = 1;
    unsigned short i;

    for (i = 0; (i < 65535) && (s1_char != 0 && s2_char != 0); i++)
    {
        s1_char = string1[i];
        s2_char = string2[i];

        if (s1_char != s2_char)
        {
            return false; // different
        }
    }

    if (i == 65535 && s1_char != 0)
    {
        return 3; // err_code [limit reached] for unsigned short data type
    }

    return true; // equal
}


// compares 'number' chars in two strings
bool cmpstrn(char *string1, char *string2, unsigned short lenght)
{
    char s1_char = 1, s2_char = 1;
    int i;

    for (i = 0; (i < 65535 && i < lenght) && (s1_char != 0 && s2_char != 0); i++)
    {
        s1_char = string1[i];
        s2_char = string2[i];

        if (s1_char != s2_char)
        {
            return false; // different
        }
    }

    if (i == 65535 && s1_char != 0)
    {
        return 3; // err_code [limit reached] for unsigned short data type
    }

    return true; // equal
}


bool iswprintable(wchar_t ch)
{

  if (ch >= 32 && ch <= 126)
  {
      // ascii
      return true;

  }
  else if (ch >= 160 && ch <= 255) // 256 to   55295)
  {
      //
      return true;
  }
  else if (ch >= 57344 && ch <= 65533)
  {
      //
      return true;
  }
  else if (ch >= 65536 && ch <= 1114111)
  {
      //
      return true;
  }

  else {

      return false;
  }
}


bool file_wc(char *filename)
{
//    if (cmpstrn(argv[i], "--", 2) != true) // argument first 2 chars must be different of --
    //    return 12; // parameter error 


    wint_t ch;

    unsigned short line_lenght = 0;
    unsigned short line_chars = 0;
    unsigned short line_bytes = 0;


    FILE *file = fopen(filename, "r");

    if (file == NULL){
        printf("%s: %s: ", parameters[0], filename);
        fflush(stdout);
        perror(NULL);
        return ERROR;
    }
    files++; // probable a valid file


    wint_t byte1 = 127; // 8 bits

    wint_t byte2 = 2047; // 16 bits

    wint_t byte3 = 65535; // 32 bits

    wint_t byte4 = 1114111; // 64 bits (1115111)

    wint_t breakline = 10; // \n
    wint_t whitespace = 32; // [space]

    wint_t last_char = 0;
    
// DEBUG
//puts("This part");

    // DEBUG
    //printf("char is: %i\n", fgetwc(file));

    while ((ch = fgetwc(file)) != WEOF)
    {
        // DEBUG
        //puts("In Loop");
        //printf("Char: %i\n", ch);

          file_chars++; // character

          if (ch <= byte1) {
           // 8 bits (1 byte)
              file_bytes += 1;

          } else if (ch <= byte2) {
           // 16 bits (2 bytes)
              file_bytes += 2;

          } else if (ch <= byte3) {
           // 24 bits (3 bytes)
              file_bytes += 3;

          } else if (ch <= byte4) {
           // 32 bits (4 bytes)
              file_bytes += 4;

          } else {
           // 40 bits (5 bytes)
              file_bytes += 0; // unsupported character (value)
          }


          line_chars++;
          //line_bytes += file_bytes;

          if (ch == breakline) // (L'\n')
          {
              // file_chars--;
              file_breaklines++;

              if (line_lenght > longest_line)
              {
                 longest_line = line_lenght;
              }

              line_lenght = line_chars;
              line_chars = 0;
              //line_bytes = 0;
          }

          if (last_char != whitespace && last_char != breakline && ch == whitespace && file_chars != 1)
          {
              file_words++;
          }

          if (last_char != breakline && last_char != whitespace && ch == breakline && file_chars != 1)
          {
              file_words++;
          }

          last_char = ch;

        // DEBUG
        //puts("Loop part");
    }

    //puts("Another part");

    // if (last_char == whitespace)  
    //   file_words--;
    
    if (iswprintable(last_char) != false)
   //(last_char != breakline && last_char != whitespace)
    // if last char printable as AaZz123...
    {
        file_words++;
    }

    //if (last_char == breakline)
        //file_words--;

    fclose(file);
    //printf("there are %d breaklines\n", file_breaklines);

// DEBUG
//printf("FileChars: %d; FileBytes: %d; FileLines: %d; FileWords: %d; LongestLine: %d\n", file_chars, file_bytes, file_breaklines, file_words, longest_line);


    return SUCCESS;
}


int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "");

if (argc > 1)
{

   if (cmpstr(argv[1], "--version") == true)
   {
      // version
      printf("%s (NGCU) version %s\nCopyright (C) 2024 Free and Open Source Software.\nThis is a free software: you are free to modify and redistribute it.\nWritten by %s instagram> %s, email> %s\n", argv[0], VERSION, AUTHOR, SOCIAL, EMAIL);
   }

   else if (cmpstr(argv[1], "--help") == true)
   {
     // help
     printf("Usage: %s [option]\n\nOptions:\n  -l lines    prints number of lines\n  -w words    prints number of words\n  -c count    prints number of bytes\n  -m chars    prints number of characters\n  -L    prints longest line lenght\n  --version   prints program version information\n  --help      prints this message\n\nExamples:\n  %s  filename      returns file lines, words and bytes number\n  %s filename -m    returns file characters number\n\nreporting bugs and complete documentation in <%s>\ncopyright: this is a free software, you are free to redistribute it.\nthis software was written by %s\n", argv[0], argv[0], argv[0], WEBPAGE, AUTHOR);
   }

    // if theres -- in arguments but no filename
    else if (argc == 2 && cmpstrn(argv[1], "--", 2) == true)
    {
        printf("%s: unrecognized option '%s'\n", argv[0], argv[1]);
        printf("Try '%s --help' for more information.\n", argv[0]);
    }

   else
   {
       parameters_lenght = argc;
       parameters = argv;
       p_option = false;

       // algorithm
     
        for (int i = 1; i < argc; i++)
        {
            if (cmpstrn(argv[i], "--", 2) != true && cmpstrn(argv[i], "-", 1) != true) // argument first 2 chars must be different of -- and -
            {
                filewc_ret = file_wc(argv[i]);
                print_info();
            }
        }
   }
}
return 0;
}


void print_info()
{

    if (filewc_ret == ERROR)
        return;

    //else if (files == 0)
    //    return;

    char *modes[10] = {"--chars", "-m", "--bytes", "-c", "--lines", "-l", "--words", "-w", "--max-line-length", "-L"};
    bool options_mode[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // "--chars", "-m", "--bytes", "-c", "--lines", "-l", "--words", "-w", "-max-line-lenght", "-L"
    int modes_values[10] = {file_chars, file_chars, file_bytes, file_bytes, file_breaklines, file_breaklines, file_words, file_words, longest_line, longest_line};

    char *mode;
    const int modes_lenght = 10;  // total available options (5+5)
    bool no_option = true; // (if no option choiced/received)
    int inv_parameter = 0; // invalid parameter (indice/index)


    for (int i = 1; i < parameters_lenght; i++)    // for each parameter in parameters
    {
        for (int ii = 0; ii < modes_lenght; ii++)
        {
            mode = modes[ii];    // mode

          if (cmpstrn(parameters[i], "--", 2) == true || cmpstrn(parameters[i], "-", 1) == true) // -- or -
          {
            p_option = true; // probale option
            inv_parameter = i + 1; //

            if (cmpstr(parameters[i], modes[ii]) == true)    // if parameter is equal to mode  parameters[i] == modes[ii]
            {
                options_mode[ii] = true;    // active/enable option mode
                //printf("mode %d is ON\n", ii); // DEBUG
                no_option = false;  // 
                inv_parameter = 0;
            }
          }
        }
    }


    // if theres -- in arguments but no valid options
    if (p_option == true && no_option == true)
    {
        printf("%s: unrecognized option '%s'\n", parameters[0], parameters[inv_parameter]);
        printf("Try '%s --help' for more information.\n", parameters[0]);
    }

    // if theres no option given or received 
    else if (no_option == true)
    {
        printf("lines: %d; words: %d; bytes: %d\n", modes_values[5 - 1], modes_values[7 - 1], modes_values[3 - 1]);
        // DEBUG
        // {file_chars, file_chars, file_bytes, file_bytes, file_breaklines, file_breaklines, file_words, file_words, longest_line, longest_line}
    }

    else
    {
        // print the actually options values
        for (int i = 0; i < modes_lenght; i++)
        {
            if (options_mode[i] == true)
            {
                printf("%s : %d\n", modes[i], modes_values[i]);
            }
        }
    }
    return;

}

