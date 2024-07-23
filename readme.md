This app runs a photobooth.

It was tested with a SONY A7M2. Any camera that is compatible with gphoto2 should work with this app.

In the folder of the executelbe app you can place a folder "Overlays" and "Logos"
This foldes are scanned by the app. Logos a automaticlly imported and Overlays needs meta datas.
Also a "backgound.jpg", a "error.jpg", a "photo.jpg" and a "processing.jpg" are needed in the app folder.
Logos and Overlays musst be a "xxx.png" file. For example "Logo.png" not "Logo.PNG".
The meta file musst have the same name as the overlay but as filetype "xxx.meta"

------------------------------------------------------------------------------

to compile it on a pi 

run in terminal (install dependencies)

$ sudo apt-get install build-essential

$ sudo apt-get install libopencv-dev

$ sudo apt-get install libgphoto2*

$ sudo apt-get install libxdg*

$ sudo apt-get install qt6-base-dev

$ sudo apt-get install qt6-wayland

$ sudo apt-get install libxkbcommon*

copy "code" folder from git

create a "build" folder in your code folder

run in terminal

$ cd [path to your build folder]

$ cmake ../

$ make

copy all data from "data" folder to the build folder
connect your camera

run "Photobox" application

------------------------------------------------------------------------------

Changelog

2024-07-23 V1.0.0 init version on git

--------------------------

Copyright (c) 2024 richi9514

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. 
