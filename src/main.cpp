#include <Arduino.h>
#include "Arduino_LED_Matrix.h"

ArduinoLEDMatrix matrix;

byte buffer[8][12] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

byte O[8][8] = {
  { 0, 1, 1, 1, 1, 1, 0, 0 },
  { 1, 1, 0, 0, 0, 1, 1, 0 },
  { 1, 1, 0, 0, 0, 1, 1, 0 },
  { 1, 1, 0, 0, 0, 1, 1, 0 },
  { 1, 1, 0, 0, 0, 1, 1, 0 },
  { 1, 1, 0, 0, 0, 1, 1, 0 },
  { 0, 1, 1, 1, 1, 1, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 }
};

byte T[8][8] = {
  { 0, 1, 1, 1, 1, 1, 1, 0 },
  { 0, 0, 0, 1, 1, 0, 0, 0 },
  { 0, 0, 0, 1, 1, 0, 0, 0 },
  { 0, 0, 0, 1, 1, 0, 0, 0 },
  { 0, 0, 0, 1, 1, 0, 0, 0 },
  { 0, 0, 0, 1, 1, 0, 0, 0 },
  { 0, 0, 0, 1, 1, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 }
};

byte I[8][8] = {
  { 0, 1, 1, 1, 1, 1, 1, 0 },
  { 0, 0, 0, 1, 1, 0, 0, 0 },
  { 0, 0, 0, 1, 1, 0, 0, 0 },
  { 0, 0, 0, 1, 1, 0, 0, 0 },
  { 0, 0, 0, 1, 1, 0, 0, 0 },
  { 0, 0, 0, 1, 1, 0, 0, 0 },
  { 0, 1, 1, 1, 1, 1, 1, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 }
};

byte S[8][8] = {
  { 0, 1, 1, 1, 1, 0, 0, 0 },
  { 1, 1, 0, 0, 1, 1, 0, 0 },
  { 1, 1, 0, 0, 0, 0, 0, 0 },
  { 0, 1, 1, 1, 1, 1, 0, 0 },
  { 0, 0, 0, 0, 0, 1, 1, 0 },
  { 1, 1, 0, 0, 0, 1, 1, 0 },
  { 0, 1, 1, 1, 1, 1, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 }
};

byte Blank[8][8] = {
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 }
};

void shiftLeft(byte frame[8][12], int height, int width)
{
  // Shift all but the first column to the left one
  for(int y = 0; y < height; y++)
  {
    for (int x = 1; x < width; x++)
    {
      frame[y][x - 1] = frame[y][x];
    }
  }

  // fill the right most column with 0s
  for(int y = 0; y < width; y++)
  {
    frame[y][width - 1] = 0;
  }
}

// Writes part of frame 1 and part of frame 2 to the output frame.
// SInce frame1 and frame2 are 24 total in lenght, pass in a number for starting point as
// an index 0 to 12 for where the output should start writing if both frames were merged together.
//  0 would be output all of frame 1. 6 would be the right half of frame1 and the left half of frame 2. 12 would be all of frame 2.
void mergeFrames(byte letter1[8][8], byte letter2[8][8], byte output[8][12], int outputHeight, int outputWidth, int letterHeight, int letterWidth, int spaceBetweenLetters, int startingPoint)
{
  // Loop through the points of output and write the appropriate points from frame 1 or 2.
  // Shift all but the first column to the left one
  for(int y = 0; y < outputHeight; y++)
  {
    for (int x = 0; x < outputWidth; x++)
    {
      int pointToTake = startingPoint + x;
      if (pointToTake < letterWidth) //Writing from the first letter
      {
        // writing letter 1
        output[y][x] = letter1[y][pointToTake];
      }
      else if (pointToTake < letterWidth + spaceBetweenLetters)
      {
        // Writing the space
        output[y][x] = 0;
      }
      else if (pointToTake < letterWidth + letterWidth + spaceBetweenLetters)
      {
        // From letter 2
        output[y][x] = letter2[y][pointToTake - letterWidth - spaceBetweenLetters];
      }
      else
      {
        // not really expected to get here but draw something
         output[y][x] = 1;
      }
    }
  }
}

// For scrolling text this method will write a vertical strip of pixels to the right side of the buffer
void WriteToRightSideBuffer(byte letter1[8][8], byte output[8][12], int outputHeight, int outputWidth, int letterHeight, int letterWidth, int spaceBetweenLetters, int startingPoint)
{
  // Loop through the points of output and write the appropriate points from frame 1 or 2.
  // Shift all but the first column to the left one
  for(int y = 0; y < outputHeight; y++)
  {
    int x = outputWidth - 1;
    int pointToTake = startingPoint;
    if (pointToTake < letterWidth) //Writing from the first letter
    {
      // writing letter 1
      output[y][x] = letter1[y][pointToTake];
    }
    else if (pointToTake < letterWidth + spaceBetweenLetters)
    {
      // Writing the space
      output[y][x] = 0;
    }
    else
    {
      // not really expected to get here but draw something
        output[y][x] = 1;
    }
  }

}


 // Text to scroll
byte (*letters[5])[8][8] = {&Blank, &O, &T, &I, &S};

const int letterCount = 5;
const int letterWidth = 8;
const int spaceBetweenLetters = 1;

// The current letter to show
int letterIndex = 0;

int fadeOffset = 0; // 1-12 as we fade from one letter to the next

void setup() {
  // put your setup code here, to run once:
  matrix.begin();

  //mergeFrames(O, T, buffer, 8, 12, 6);
  //matrix.renderBitmap(*letters[0], 8, 12);
}


void loop() {
  
  delay(25);
  
  fadeOffset++;

  if (fadeOffset >= letterWidth + spaceBetweenLetters)
  {
    // Start with the next letter.
    fadeOffset = 0;
    letterIndex++;

    if (letterIndex >= letterCount)
    {
      letterIndex = 0;
    }
  }

 int nextLetter = letterIndex + 1;
 if (nextLetter >= letterCount)
 {
  nextLetter = 0;
 }

  //mergeFrames(*letters[letterIndex], *letters[nextLetter], buffer, 8, 12, letterWidth, letterWidth, spaceBetweenLetters, fadeOffset);
  
  shiftLeft(buffer, 8, 12);
  WriteToRightSideBuffer(*letters[letterIndex], buffer, 8, 12, letterWidth, letterWidth, spaceBetweenLetters, fadeOffset);
  matrix.renderBitmap(buffer, 8, 12);
  
  //shiftLeft(frame, 8, 12);
  //matrix.renderBitmap(buffer, 8, 12);
 
}
