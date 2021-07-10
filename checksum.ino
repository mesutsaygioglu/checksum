/*
   Author :Mesut Saygıoğlu
   Date   :05.07.2021

   This is a  basic checksum code that can be used for string data type.
*/

#include "Checksum.h"

//Set the checksum constant.In this case it is 17.
Checksum checksum(17);

void setup() {
  Serial.begin(9600);
}

void loop() {


  if (Serial.available())
  {
    String data = Serial.readString();

    //===============Transmitter=============={
    String dataToSend = checksum.encode(data,
                        ReturnFormats::DatawithChecksumAdded);
    //}==============Transmitter===============
    
    Serial.println("encoded and sent");

    // send the data
    //
    String receivedData=dataToSend;
    //
    // time to get data back

    //===============Receiver=============={
    bool result = checksum.process(receivedData);
    //}==============Receiver===============

    
    Serial.print("Result:");Serial.println(result);

  }
}
