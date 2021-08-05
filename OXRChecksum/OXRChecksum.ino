/*
   Author :Mesut Saygıoğlu
   Date   :04.08.2021

   This is a  basic checksum code that can be used for string data type.
*/

#include "XORChecksum.h"

//Set the checksum constant.In this case it is 17.
XORChecksum xorChecksum;

void setup() {
  Serial.begin(9600);
}

void loop() {


  if (Serial.available())
  {
    String data = Serial.readString();

    //===============Transmitter=============={
    String dataToSend = xorChecksum.encode(data,
                        ReturnFormats::DatawithChecksumAdded);
    //}==============Transmitter===============
    
    Serial.println("encoded and sent");

    // send the data
    //
    String receivedData=dataToSend;
    //
    // time to get data back

    //===============Receiver=============={
    bool result = xorChecksum.process(receivedData);
    //}==============Receiver===============

    
    Serial.print("Result:");Serial.println(result);

  }
}
