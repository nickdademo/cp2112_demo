cp2112_demo
===========
* Ensure SDA and SCL lines are pulled-up to a voltage which is compatible with that of the target battery interface.
* Setting J7=VIO pulls up the bus to 3.3V - this should be compatible with the majority of applications.
* If you require a different bus voltage:
  * Set J7=EXT_PU and connect TB1(EXT_PU) to the battery interface voltage.

**Program Output:**  

*Device successfully opened.*  
*Device successfully configured.*  
*Voltage = 16403 mV*  
*Current = 74 mA*  
*RSOC = 97 %*  
*Remaining Capacity = 5709 mAh*  
*Average Time to Empty = 738 min(s)*  
*Manufacturer Name = INSPIREDE*  
*Charger connected.*  
*Done! Exiting...*  

**Tools/Environment:**  
- Silicon Labs CP2112EK: http://www.silabs.com/products/interface/Pages/CP2112EK.aspx  
- Texas Instruments bq20z65-R1 Gas Gauge: http://www.ti.com/product/bq20z65-r1  
- Windows 8.1 Professional 64-bit  
- Visual Studio 2013 (Version 12.0.30501.00 Update 2)  
