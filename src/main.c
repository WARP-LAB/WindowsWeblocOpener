/*
 * WindowsWeblocOpener
 *
 * Copyright 2017 WARP
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 */

#ifndef DEBUG
#define DEBUG 1
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
// TODO: use WinMain to get rid of console window popup?
// #include <windows.h>

int getWeblocType(const char *fname, char *str);
bool hasExt(const char *name, const char *ext);

int main(int argc, const char * argv[]) {

#if DEBUG
  printf("Opening webloc files in MSW. DEBUG ON\n");
#endif

  // If deploy then add logic to hide console window
//#if !DEBUG
//  HWND Stealth;
//  AllocConsole();
//  Stealth = FindWindowA("ConsoleWindowClass", NULL);
//  ShowWindow(Stealth, 0);
//#endif

  // If file argument is specified
  if (argc > 1) {

#if DEBUG
    printf("File count specified: %i.\n", argc - 1);
#endif

    // For all webloc files
    int i;
    for (i = 1; i<argc; i++) {

      // Check if file has .webloc extension
      if (!hasExt(argv[i], "webloc")) {
#if DEBUG
        fprintf(stderr, "Incorrect file extension, skipping: %s\n", argv[i]);
#endif
        continue;
      }
      else {
#if DEBUG
        printf("Correct extension\n");
#endif
      }
        
      // Detect type of webloc (binary or plain)
      int result = getWeblocType(argv[i], "<?xml version");
      if (result == -1) {
#if DEBUG
        fprintf(stderr, "Could not read file at %s\n", argv[i]);
#endif
        continue;
      }

      // Read webloc file into heap
      FILE *fp;
      //if ((fp = fopen(argv[i], "rb+")) == NULL) {
      if ((fopen_s(&fp, argv[i], "rb+")) != NULL) {
        // skip file
        continue;
      }
      fseek(fp, 0, SEEK_END);
      long flen = ftell(fp);
      fseek(fp, 0, SEEK_SET);

      char *buffer = (char *)malloc(flen + 1);
      fread(buffer, flen, 1, fp);
      // file closed
      fclose(fp);

      // if file length is zero, no need to look at it
      if (flen < 1) {
        // skip file
        free(buffer);
        continue;
      }
      buffer[flen] = '\0';

      char *delimStringStart;
      char *delimStringEnd;
      int delimFoundStart, delimFoundEnd;
      int delimOffsetStart, delimOffsetEnd;

      if (result == 1) {
#if DEBUG
        printf("Processing plain plist\n");
#endif
        delimStringStart = "<string>";
        delimStringEnd = "</string>";
        delimOffsetStart = 8;
        delimOffsetEnd = 0;
      }
      else {
#if DEBUG
        printf("Processing binary plist\n");
#endif
        delimStringStart = "http";
        delimStringEnd = "\b\v";
        delimOffsetStart = 0;
        delimOffsetEnd = 0;
      }

      char *pfoundStart = strstr(buffer, delimStringStart);
      if (pfoundStart == NULL) {
        // skip file
        free(buffer);
        continue;
      }
      else {
        delimFoundStart = pfoundStart - buffer;
        delimFoundStart += delimOffsetStart;
#if DEBUG
        printf("Starting delimiter found at position: %i\n", delimFoundStart);
#endif
      }
      char *pfoundEnd = strstr(buffer, delimStringEnd);
      if (pfoundEnd == NULL) {
        // skip file
        free(buffer);
        continue;
      }
      else {
        delimFoundEnd = pfoundEnd - buffer;
        delimFoundEnd += delimOffsetEnd;
#if DEBUG
        printf("Ending delimiter found at position: %i\n", delimFoundEnd);
#endif
      }

      if (delimFoundEnd <= delimFoundStart) {
        free(buffer);
        continue;
      }

      int urlStrLen = delimFoundEnd - delimFoundStart;
      char *urlStr = (char *)malloc(urlStrLen+1);
      strncpy(urlStr, buffer + delimFoundStart, delimFoundEnd - delimFoundStart);
      urlStr[urlStrLen] = '\0';

#if DEBUG
      printf("Clean url\n%s\n", urlStr);
#endif

      // we do not escape &'s as we can use this trick https://stackoverflow.com/a/1327444
      char cmd[4096];
      snprintf(cmd, 4096, "start \"\" \"%s\"", urlStr);
      system(cmd);

      free(buffer);
      free(urlStr);

#if DEBUG
      printf("\nFile at %s processed!\n", argv[i]);
#endif
           
    }
  }
  else {
#if DEBUG
    printf("Please specify filepath(s)!\n");
#endif
  }


//#if !DEBUG
//  FreeConsole();
//#endif

  return 0;
}

int getWeblocType(const char *fname, char *str) {

  FILE *fp;
  int weblocType = 0; // 0 is binary, 1 is plain type
  char tmp[4096];
  
  // in windows this?
  //if ((fp = fopen(fname, "rb+")) == NULL) {
  if ((fopen_s(&fp, fname, "rb+")) != NULL) {
    return(-1);
  }

  // Just do line by line in constant buffer, no heap
  while (fgets(tmp, 4096, fp) != NULL) {
    if ((strstr(tmp, str)) != NULL) {
      weblocType = 1;
      break;
    }
  }

  if (fp) {
    fclose(fp);
  }
  return(weblocType);
}

bool hasExt(const char *name, const char *ext)
{
  size_t nl = strlen(name), el = strlen(ext);
  return nl >= el && !strcmp(name + nl - el, ext);
}