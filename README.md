# Linkuit Studio

Linkuit Studio is an open-source logic circuit simulator that is designed to be intuitive and easy to use. Its aim is to provide a platform for learning and experimentation with logic circuits.

![Traffic Light Controller](https://github.com/SimonBuxx/Linkuit-Studio/blob/master/images/traffic_lights.png?raw=true)
*A simple traffic light controller circuit, consisting of a counter and a diode matrix*

## FAQ

- **How do I zoom and pan the scene?**
  - To zoom, hold the control key and scroll using the mouse wheel
  - To pan the scene, hold the control key and drag using the left mouse button
  
- **Is there an official tutorial?**
  - No, currently, there is no official tutorial. An in-place tutorial is currently being developed and will be published in a later version of Linkuit Studio.
  
- **Why can I not use the "Tools" > "Save Screenshot..." feature?**
  - The screenshot feature will be implemented in a later version. This will be a simple tool to copy a screenshot of the current scene into the clipboard.
  - In the future, this feature might be extended to support automatic zooming to capture the whole circuit and other nice-to-have options.
  
- **Can I export my circuits to VHDL or Verilog?**
  - No, not at the moment. VHDL and/or Verilog export is planned for a future release but not a priority right now.
  
- **Can I create custom components from my circuits to import them into other circuits?**
  - Custom components are not yet included but they will be among the first new features after version 1.0.
  
- **Is there a Linux/macOS version of Linkuit Studio?**
  - Linkuit Studio is developed on Windows and at the moment the installer is available for Windows only. Because the project is built on the Qt framework, it should be relatively easy to build for Linux/macOS. Depending on demand, there could be support for Linux/macOS in the future. Meanwhile, you can of course try to compile it yourself for your target system.
  
- **How do I update Linkuit Studio?**
  - To update Linkuit Studio, navigate to "Help" > "Check for Updates..." or click on the symbol next to the version label on the welcome and about dialogs.
  - Download the new version, if available, and install it using the installer. Just use the same install path as the current installation and the installer will replace it and will keep your software's configuration.
  
Your question is not on the list? Please do not hesitate to contact us at linkuit@outlook.com or on Twitter [@goLinkuit](https://twitter.com/goLinkuit/) (German or English).

## How to compile

*Qt Creator for open source* can be downloaded here: https://www.qt.io/download-qt-installer

I am currently using **Qt Creator 9.0.1** on Windows with **Qt 6.4.0**. The compiler I use is **MinGW 64-bit 11.2.0**.
After installing Qt Creator, the **.pro** file can be opened with Qt Creator, from where you can compile the project.
