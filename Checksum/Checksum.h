/*
   Author :Mesut Saygıoğlu
   Date   :05.07.2021

   This is a  basic checksum code that can be used for string data type.


MIT License

Copyright (c) 2021 Mesut Saygıoğlu

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/
#ifndef CHECKSUM_H_
#define CHECKSUM_H_
#include <Arduino.h>

enum ReturnFormats
{
  OnlyModuloResult,
  OnlyData,
  DatawithChecksumAdded
} ;

class Checksum
{

  public:

    Checksum(int _checksumConstant = 17)
    {
      checksumConstant = _checksumConstant;
    }

    inline String encode(String rawData, int ReturnFormats )
    {
      rawData.trim();
      //Serial.print("rawData.length() :");
      //Serial.println(rawData.length());
      rawData = rawData + "#";

      int len = rawData.length();
      char ascii_str[len];
      rawData.toCharArray(ascii_str, len);
      BYTE arr[len];
      int sum = 0;
      int moduloValue = 0;
      //converting string to BYTE[]
      stringToByteArray(ascii_str, arr);



      for (int i = 0; i < len - 1; i++)
      {
        //Serial.print(ascii_str[i]);
        //Serial.print(" : ");
        //Serial.println( arr[i]);
        sum = sum + arr[i];
      }


      //Serial.print("Sum :");
      //Serial.println(sum);


      moduloValue = sum % checksumConstant;
      //Serial.print("Modulo :");
      //Serial.println(moduloValue);
      rawData.trim();
      String str = "";
      if (moduloValue > 9)
      {
        str = rawData + String(moduloValue);
      }
      else
      {
        str = rawData + "0" + String(moduloValue);
      }
      // Serial.print("Data to send:");
      // Serial.println(str);

      if (ReturnFormats == ReturnFormats::OnlyModuloResult)
      {
        //Serial.print("Modulo :");
        //Serial.println(moduloValue);


        if (moduloValue > 9)
        {
          return String(moduloValue);
        }
        else
        {
          return "0" + String(moduloValue);
        }


      }
      else if (ReturnFormats == ReturnFormats::OnlyData)
      {
        //Serial.print("rawData :");
        //Serial.println(rawData);
        return rawData;
      }
      else if (ReturnFormats == ReturnFormats::DatawithChecksumAdded)
      {
        //Serial.print("Data to send:");
        //Serial.println(str);
        return str;
      }
    }

    inline  String decode(String incomingData, int ReturnFormats)
    {
      String checkSumStr = incomingData.substring(incomingData.length() - 2,
                           incomingData.length());

      String rawData = incomingData.substring(0, incomingData.length() - 3);


      if (ReturnFormats == ReturnFormats::OnlyModuloResult)
      {
        return checkSumStr;
      }
      else if (ReturnFormats == ReturnFormats::OnlyData)
      {
        return rawData;
      }
      else if (ReturnFormats == ReturnFormats::DatawithChecksumAdded)
      {
        return incomingData;
      }
    }


    inline bool process(String incomingData)
    {
      //Serial.print("string in the process :");
      //Serial.println(incomingData);
      incomingData.trim();
      String data                    = Checksum::decode(incomingData,
                                       ReturnFormats::OnlyData);
      String checksumValue           = Checksum::decode(incomingData,
                                       ReturnFormats::OnlyModuloResult);

      //Serial.print("string sfter decode   :");
      //Serial.println(data);
      //data.trim();
      String calculatedChecksumValue = Checksum::encode(data,
                                       ReturnFormats::OnlyModuloResult);


      //Serial.print("checksumValue :");
      //Serial.println(checksumValue);

      //Serial.print("calculatedChecksumValue :");
      //Serial.println(calculatedChecksumValue);

      if (checksumValue.equals(calculatedChecksumValue))
      {
        return true;
      }
      else
      {
        return false;
      }
    }

  private:
    typedef unsigned char BYTE;

    void stringToByteArray(char* input, BYTE *output)
    {
      unsigned int iterator = 0;
      unsigned int i = 0;

      while (input[iterator] != '\0')
      {
        output[i++] = input[iterator++];
      }
    }

    int checksumConstant;
};

#endif
