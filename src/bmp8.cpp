/**
 *   libBMPP - Bmp8.hpp
 *   Developed by Christian Visintin
 * 
 * MIT License
 * Copyright (c) 2019 Christian Visintin
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
**/

#include <bmp8.hpp>

#ifdef BMP_DEBUG
#include <iostream>
#include <string>
#endif

using namespace bmp;

/**
 * @function Bmp8
 * @description Bmp8 class constructor
**/

Bmp8::Bmp8() : Bmp() {
  pixelArray.clear();
}

/**
 * @function Bmp
 * @description Bmp class constructor
**/

Bmp8::Bmp8(std::vector<Pixel*> pixelArray, size_t width, size_t height) : Bmp(pixelArray, width, height) {
  //Set bits per pixel
  header->bitsPerPixel = 8;
  //FileSize must be set by child class
  size_t nextMultipleOf4 = roundToMultiple(width * (header->bitsPerPixel / 8), 4);
  size_t paddingSize = nextMultipleOf4 - (header->width * (header->bitsPerPixel / 8));
  size_t rowSize = paddingSize + ((width * header->bitsPerPixel / 8));
  size_t dataSize = rowSize * height;
  header->fileSize = 54 + dataSize;
  //DataSize must be set by child class
  header->dataSize = dataSize;
}

/**
 * @function ~Bmp8
 * @description Bmp8 class destructor
**/

Bmp8::~Bmp8() {
  
}


bool Bmp8::decodeBmp(uint8_t* bmpData, size_t dataSize) {

  //Call superclass decodeBmp to decode header
  if (!Bmp::decodeBmp(bmpData, dataSize)) {
    return false;
  }
  //Get data
  size_t nextMultipleOf4 = roundToMultiple(header->width * (header->bitsPerPixel / 8), 4);
  size_t paddingSize = nextMultipleOf4 - (header->width * (header->bitsPerPixel / 8));
  size_t realRowSize = (header->width * (header->bitsPerPixel / 8));
  size_t rowPositionCounter = 0;
  for (size_t dataPtr = header->dataOffset - 1; dataPtr < header->fileSize - 1;) {
    //Store Pixels for each byte NOTE: BMP is 0/1
    uint8_t value = bmpData[++dataPtr];
    //Create new pixel
    pixelArray.push_back(new BytePixel(value));
    rowPositionCounter++;
    //If row has been parsed, go to next line
    if (rowPositionCounter >= realRowSize) {
      rowPositionCounter = 0;
      dataPtr += paddingSize;
    }
  }
  return true;

}

/**
 * @function encodeBmp
 * @description: encodes bitmap to buffer
 * @param size_t*
 * @returns uint8_t*
**/

uint8_t* Bmp8::encodeBmp(size_t* dataSize) {
  //Get our fundamental parameters
  size_t nextMultipleOf4 = roundToMultiple(header->width * (header->bitsPerPixel / 8), 4);
  size_t paddingSize = nextMultipleOf4 - header->width * (header->bitsPerPixel / 8);
  size_t realRowSize = (header->width * (header->bitsPerPixel / 8));
  //Fill header and get bmpData with fixed size
  uint8_t* bmpData = Bmp::encodeBmp(dataSize);
  //Return nullptr if needed
  if (bmpData == nullptr) {
    return nullptr;
  }
  
  //Fill data
  int pxIndex = -1;
  size_t rowPositionCounter = 0;
  for (size_t dataPtr = header->dataOffset - 1; dataPtr < *dataSize - 1;) {
    //Set pixel
    BytePixel* currPixel = reinterpret_cast<BytePixel*>(pixelArray.at(++pxIndex));
    bmpData[++dataPtr] = currPixel->getValue();
    //Check if row has to be closed
    rowPositionCounter++;
    if (rowPositionCounter >= realRowSize) {
      //Fill row with padding and go on new line
      rowPositionCounter = 0;
      for (size_t i = 0; i < paddingSize; i++) {
        bmpData[++dataPtr] = 0;
      }
    }
  }
  return bmpData;
}

/**
 * @function setPixelAt
 * @description: replace pixel in a certain position with the provided one
 * @param int
 * @param uint8_t
 * @param uint8_t
 * @param uint8_t
 * @returns bool
**/

bool Bmp8::setPixelAt(int index, uint8_t value) {

  if (index >= static_cast<int>(pixelArray.size())) {
    return false;
  }
  BytePixel* reqPixel = reinterpret_cast<BytePixel*>(pixelArray.at(index));
  reqPixel->setPixel(value);
  return true;
}

/**
 * @function getPixelAt
 * @description return pointer to pixel in the provided position
 * @param int
 * @returns BytePixel*
**/

BytePixel* Bmp8::getPixelAt(int index) {

  if (index >= static_cast<int>(pixelArray.size())) {
    return nullptr;
  }
  return reinterpret_cast<BytePixel*>(pixelArray.at(index));
}