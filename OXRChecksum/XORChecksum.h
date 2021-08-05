/*
   Author :Mesut Saygıoğlu
   Date   :04.08.2021

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
#ifndef XORCHECKSUM_H_
#define XORCHECKSUM_H_
#include <Arduino.h>

enum ReturnFormats
{
  OnlyChecksum,
  OnlyData,
  DatawithChecksumAdded
} ;

class XORChecksum
{

  public:
    // constructor
    XORChecksum()
    {
    }
    // this will take the raw data and encode it.
    inline String encode(String rawData, int ReturnFormats )
    {
      // trim the invalid tail.
      rawData.trim();
      // delete the first two bytes and last two bytes.
      rawData = rawData.substring(2, rawData.length() - 2);
      Serial.print("delete start-stop :");
      Serial.println(rawData);
      Serial.print("rawData.length() :");
      Serial.println(rawData.length());
      // we need some info to encode the raw data.
      int len = rawData.length();
      char ascii_str[len];
      // this will convert the string data type to chararray.
      rawData.toCharArray(ascii_str, len);
      BYTE arr[len];
      int sum = 0;
      int moduloValue = 0;
      //converting chararray to BYTE array
      stringToByteArray(ascii_str, arr);

      // use xor method and return the checksum
      int checksum = xorChecksum(arr);
      Serial.print("Checksum :");
      Serial.println(checksum);

      /*
         The checksum can consist of two digits.
         So we will be dealing with two chars.
         to standardize the evaluatıon,
         we will add a zero, if the checksum is only one digit.
      */
      String strChecksum = "";
      if (checksum < 9)
      {
        strChecksum = "0" + String(checksum);
      }
      else
      {
        strChecksum = String(checksum);
      }


      //attach the checksum to raw data.
      String str = rawData + strChecksum;

      // return the desired result formats
      if (ReturnFormats == ReturnFormats::OnlyChecksum)
      {
        Serial.print("Checksum Value :");
        Serial.println(strChecksum);
        return strChecksum;
      }
      else if (ReturnFormats == ReturnFormats::OnlyData)
      {
        Serial.print("rawData :");
        Serial.println(rawData);
        return rawData;
      }
      else if (ReturnFormats == ReturnFormats::DatawithChecksumAdded)
      {
        Serial.print("Data to send:");
        Serial.println(str);
        return str;
      }
    }

    // this will take the processed data and decode it.
    inline  String decode(String incomingData, int ReturnFormats)
    {
      // trim the invalid tail.
      incomingData.trim();

      /*
         Uncomment this section, if the received message has a start and stop flag.

         @ it will delete the first two bytes and last two bytes. @
      */
      // rawData.substring(2,rawData.length()-2);

      // extract the checksum from the received message.
      String checkSumStr = incomingData.substring(incomingData.length() - 2,
                           incomingData.length());

      // extract the  raw data from the received message.
      String rawData = incomingData.substring(0, incomingData.length() - 2);


      if (ReturnFormats == ReturnFormats::OnlyChecksum)
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

    /*
        This compares the received checksum value to the calculated one and returns true if they are equals.
        Otherwise, it will return false.
    */

    inline bool process(String incomingData)
    {
      //Serial.print("string in the process :");
      //Serial.println(incomingData);
      incomingData.trim();

      String data                    = XORChecksum::decode(incomingData,
                                       ReturnFormats::OnlyData);
      String checksumValue           = XORChecksum::decode(incomingData,
                                       ReturnFormats::OnlyChecksum);

      //Serial.print("string sfter decode   :");
      //Serial.println(data);
      //data.trim();
      
      // encode will try to delete the start and stop bits.
      // But we already got rid of them.So will add some dummy start and stop data.
      // encode will clear them.
      data="xx"+data+"xx";
      String calculatedChecksumValue = XORChecksum::encode(data,
                                       ReturnFormats::OnlyChecksum);


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


    BYTE xorChecksum(BYTE * byteArray)
    {
      BYTE checkSum = byteArray[0];
      for (int i = 1; i < sizeof(byteArray); i++)
      {
        checkSum = checkSum ^ byteArray[i];
      }
      return checkSum;
    }

    int checksumConstant;
};

#endif
