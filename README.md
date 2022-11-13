# Linkuit Studio

## FAQ

- **How do I zoom and pan the scene?**
  - To zoom, hold the control key and scroll using the mouse wheel
  - To pan the scene, ho,d the control key and drag using the left mouse button
  
- **Can I export my circuits to VHDL or Verilog?**
  - No, not at the moment. VHDL and/or Verilog export is planned for a future release but not a priority right now.
  
- **Can I create custom components from my circuits to import them into other circuits?**
  - Custom components are not yet included but they will be among the first new features after version 1.0.
  
- **Is there a Linux/macOS version of Linkuit Studio**
  - Linkuit Studio is developed on Windows and at the moment the installer is available for Windows only. Because the project is built on the Qt framework, it should be relatively easy to build for Linux/macOS. Depending on demand, there could be support for Linux/macOS in the future. Meanwhile, you can of course try to compile it yourself for your target system.
  
Your question is not on the list? Please do not hesitate to contact us at linkuit@outlook.com or on Twitter [@goLinkuit](https://twitter.com/goLinkuit/) (german or english).

![Traffic Light Controller](https://github.com/SimonBuxx/Linkuit-Studio/blob/master/images/traffic_lights.png?raw=true)
*A simple traffic light controller*

## How to compile

*Qt Creator for open source* can be downloaded here: https://www.qt.io/download-qt-installer

I am currently using **Qt Creator 8.0.2** on Windows with **Qt 6.4.0**. The compiler I use is **MinGW 64-bit 11.2.0**.
After installing Qt Creator, the **.pro** file can be opened with Qt Creator, from where you can compile the project.
