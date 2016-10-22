# SmartEverything NXP NT3H1101
----
This is a library for the NXP NT3H1101 component.
The NTAG I2C is the first product of NXP™ NTAG family offering both contactless and contact interfaces. 
In addition to the passive NFC Forum compliant contactless interface, the IC features an I2C contact interface, 
which can communicate with a microcontroller if the NTAG I2C is powered from an external power supply.

[* NXP NT3H1101 Home Page]
(http://www.nxp.com/products/identification_and_security/nfc_and_reader_ics/connected_tag_solutions/series/NT3H1101_NT3H1201.html)

This component use I2C to communicate, no extra pin are required.

It was principally designed to work with the SmartEverything board, but could
be easily adapt and use on every Arduino and Arduino Certified boards.


## Repository Contents
-------------------

* **/examples** - Example sketches for the library (.ino). Run these from the Arduino IDE. 
* **/src** - Source files for the library (.cpp, .h).
* **library.properties** - General library properties for the Arduino package manager.

## Releases
---
#### v1.1.1 Third Release 22-Oct-2016
* Change example to be more Arduino boards compatible and not only for ASME boards family.

#### v1.1.0 Second Release 18-Dec-2015
* New Features:<br>
    Add new function to read the 7-byte serial number (UID)

#### v1.0.0 First Release
## Documentation
--------------

* **[Installing an Arduino Library
* Guide](http://www.arduino.cc/en/Guide/Libraries#toc3)** - How to install a SmartEverything library on the Arduino IDE using the Library Manager


## License Information
-------------------

Copyright (c) AxelElettronica. All right reserved.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

