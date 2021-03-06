# libBmpp

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT) [![Stars](https://img.shields.io/github/stars/ChristianVisintin/libBmpp.svg)](https://github.com/veesp/libBmpp) [![Issues](https://img.shields.io/github/issues/ChristianVisintin/libBmpp.svg)](https://github.com/veesp/libBmpp) [![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](https://github.com/veesp/libBmpp/issues)

Lib **BitMaplusplus**  
Developed by *Christian Visintin*

Version 1.1.1 (07/09/2019)

---

## What is libBmpp

libBmpp is a library for working with Bitmaps C++11 (or greater), which allows you to manipulate bitmaps images.

---

## Build

libBmpp build is achievable through autotools, issuing the following commands:

```sh
./autogen.sh
./configure
make
make install
```

---

## Documentation

### Header

```cpp
typedef struct Header {
  uint16_t bmpId;
  uint32_t fileSize;
  uint32_t unused = 0;
  uint32_t dataOffset = 54;
  //DIB
  uint32_t dibSize = 40;
  uint32_t width;
  uint32_t height;
  uint16_t colorPlanes = 1;
  uint16_t bitsPerPixel;
  uint32_t biRgb = 0;
  uint32_t dataSize;
  uint32_t printSizeW;
  uint32_t printSizeH;
  uint32_t paletteSize;
  uint32_t importantColors;
} Header;
```

Header is the bitmap header storage structure and contains all the data which are usually stored into the bitmap header.

### Bmp

Bmp class is the main class of libBmpp. It is the superclass of every kind of Bitmap and takes care of handling pixels independently from the bits per pixel which is currently used. Bmp class also encodes and decodes the header and prepare the buffer for the raster data.
Every kind of Bmp (Bmp24 etc) must inherits from Bmp class.

#### header

```cpp
bmp::Header* header;
```

header represents the bitmap header, which is stored into a Header structure.

#### Constructor

```cpp
Bmp();
```

Bmp constructor initializes header to nullptr

```cpp
Bmp(std::vector<bmp::Pixel*> pixelArray, size_t width, size_t height);
```

It is possible to create a new Bmp object passing a pixel array (of any kind), width and height of the image to create a new bitmap.

There are also 2 copy constructors which copies both the header and the pixels

```cpp
Bmp(const bmp::Bmp& bmp);
Bmp(Bmp* bmp);
```

#### Destructor

```cpp
~Bmp();
```

Bmp destructor deletes header structure.

#### decodeBmp

```cpp
bool decodeBmp(uint8_t* bmpData, size_t dataSize);
```

Given the buffer read from bitmap file, this method decodes the bitmap header and store its attributes in the struct. The raster data parsing is not provided by this method and must be implemented by the classes which extends Bmp.

If was unable to parse the bmpData or was invalid returns false.

#### encodeBmp

```cpp
uint8_t* encodeBmp(size_t& dataSize);
```

Starting from the header struct, this method creates the image buffer and returns a pointer to the buffer allocated. This method takes care only of allocating enough space to put the raster data and filling the first 54 bytes with the header. The encoding of the raster data is a task for the class which extends Bmp.

If was unable to allocate the buffer, returns nullptr

#### readBmp

Reads a bmp file and decodes it. The decoded bitmap's header and pixels become attributes of the object.

```cpp
bool readBmp(const std::string& bmpFile);
```

#### writeBmp

Encodes the bitmap stored in the object and writes a bmp file.

```cpp
bool writeBmp(const std::string& bmpFile);
```

#### rotate

```cpp
bool rotate(int degrees);
```

Rotates the BMP image of 'degrees', which must be multiple of 90 (90/180/270/360).

If was unable to rotate the image, returns false

#### flipVertical

```cpp
bool flipVertical();
```

Flip the image vertically

#### flipHorizontal

```cpp
bool flipHorizontal();
```

Flip the image vertically

#### resizeArea

Resize the image area, an offset can be specified.

```cpp
bool resizeArea(size_t width, size_t height, size_t xOffset = 0, size_t yOffset = 0);
```

#### resizeImage

Resize the image

```cpp
bool resizeImage(size_t width, size_t height);
```

#### getWidth

```cpp
size_t getWidth();
```

Returns the image width in pixels

#### getHeight

```cpp
size_t getHeight();
```

Returns the image height in pixels

### Bmp8

Bmp8 is a class which extends Bmp class and describes a 8 bits for pixel Bitmap.

In addition to Bmp methods, it provides the following methods:

#### Bmp8::setPixelAt

```cpp
bool setPixelAt(size_t row, size_t column, uint8_t value);
bool setPixelAt(size_t index,  uint8_t value);
```

set the 255 value of the pixel in index position.  

Returns false if the requested pixel, does not exist.

#### Bmp8::getPixelAt

```cpp
bmp::BytePixel* getPixelAt(size_t row, size_t column);
bmp::BytePixel* getPixel(size_t index);
```

Returns the pointer to the BytePixel in provided position. If the requested pixel does not exist, returns nullptr

### Bmp16

Bmp8 is a class which extends Bmp class and describes a 16 bits for pixel Bitmap.

In addition to Bmp methods, it provides the following methods:

#### Bmp16::setPixelAt

```cpp
bool setPixelAt(size_t row, size_t column, uint16_t value);
bool setPixelAt(size_t index, uint16_t value);
```

set the 255 value of the pixel in index position.  

Returns false if the requested pixel, does not exist.

#### Bmp16::getPixelAt

```cpp
bmp::WordPixel* getPixel(size_t row, size_t column);
bmp::WordPixel* getPixel(size_t index);
```

Returns the pointer to the BytePixel in provided position. If the requested pixel does not exist, returns nullptr

### Bmp24

Bmp24 is a class which extends Bmp class and describes a 24 bits for pixel Bitmap.

In addition to Bmp methods, it provides the following methods:

#### Bmp24::setPixelAt

```cpp
bool setPixelAt(size_t row, size_t column, uint8_t red, uint8_t green, uint8_t blue);
bool setPixelAt(size_t index, uint8_t red, uint8_t green, uint8_t blue);
```

set the RGB value of the pixel in index position.  

Returns false if the requested pixel, does not exist.

#### Bmp24::toGreyScale

```cpp
bool toGreyScale(int greyLevels = 255);
```

Converts a color image to greyScale. Is it possible to specify the amount of grey levels (2 - 255).

#### Bmp24::toSepiaTone

```cpp
bool toSepiaTone();
```

Converts image to sepia tone.

#### Bmp24::invert

```cpp
bool invert();
```

Invert image colors.

#### Bmp24::getPixelAt

```cpp
bmp::RGBPixel* getPixel(size_t row, size_t column);
bmp::RGBPixel* getPixel(size_t index);
```

Returns the pointer to the RGBPixel in provided position. If the requested pixel does not exist, returns nullptr

### Bmp32

Bmp32 is a class which extends Bmp class and describes a 32 bits for pixel Bitmap. (RGBa)

In addition to Bmp methods, it provides the following methods:

#### Bmp32::setPixelAt

```cpp
bool setPixelAt(size_t row, size_t column, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
bool setPixelAt(size_t index, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
```

set the RGBa value of the pixel in index position.  

Returns false if the requested pixel, does not exist.

#### Bmp32::toGreyScale

```cpp
bool toGreyScale(int greyLevels = 255);
```

Converts a color image to greyScale. Is it possible to specify the amount of grey levels (2 - 255).

#### Bmp32::toSepiaTone

```cpp
bool toSepiaTone();
```

Converts image to sepia tone.

#### Bmp32::invert

```cpp
bool invert();
```

Invert image colors.

#### Bmp32::getPixelAt

```cpp
bmp::RGBAPixel* getPixel(size_t row, size_t column);
bmp::RGBAPixel* getPixel(size_t index);
```

Returns the pointer to the RGBAPixel in provided position. If the requested pixel does not exist, returns nullptr

### Bmpmonochrome

Bmpmonochome is a class which extends Bmp class and describes a 1 bits for pixel Bitmap. (BW)

In addition to Bmp methods, it provides the following methods:

#### Bmpmonochrome::setPixelAt

```cpp
bool setPixelAt(size_t row, size_t column, uint8_t value);
bool setPixelAt(size_t index,  uint8_t value);
```

#### Bmpmonochrome::getPixelAt

```cpp
bmp::BWPixel* getPixel(size_t row, size_t column);
bmp::BWPixel* getPixel(size_t index);
```

Returns the pointer to the BWPixel in provided position. If the requested pixel does not exist, returns nullptr

### BmpParser

Parses a bmp and returns a pointer to a Bmp type (e.g. bmp8, bmp24 ...).

#### BmpParser::getBmp

```cpp
bmp::Bmp* getBmp(uint8_t* data, size_t dataSize, size_t& bitsPerPixel);
```

---

## Changelog

### 1.1.1 (07/09/2020)

- Added missing ```#include <string>``` in bmp.hpp

### 1.1.0 (24/08/2019)

* Added copy constructors for bitmaps
* Added new constructor for creating an empty bmp
* Removed pixel array constructor due to possible memory corruption; use setPixelAt instead
* Changed int params to size_t
* Added method to write BMP to file
* Use reference instead of pointer
* Added const type when necessary

### 1.0.1 (24/04/2019)

* Added getPixelAt with vector index as argument

---

## License

MIT License  

Copyright (c) 2019 Christian Visintin  

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:  

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.  

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
