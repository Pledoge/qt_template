# QT template for vscode

**Config for qt:**  
* Add
  * Path\to\qt\Tools\CMake_64\bin
  * Path\to\qt\Tools\mingw1120_64\bin
  * Path\to\qt\version\mingw_64\bin  
  
  To environment variable.

* Revise c_cpp_properties.json, change path to the qt to let vscode know where qt is.
* Press F7 to compile it.
* run run.bat to run the file. 

This only works for windows, if you wish to work in different platform, you can change CMakeLists to make it works.

