# Documentation
### Overview 
A improved implementation of the SIGGRAPH '97 paper: Tour Into the Picture

### Derivation
For derivation, please see [this](doc/inference.pdf) file.

### Usage
Note: the path of the shaders is now hard-coded as './Shaders/xxx.glsl'. If the window is white when you open the excutable file, the program does not find the shaders. Please put the 'Shader' folder to the correct relative path.

**Steps**:

1. Press Ctrl+O to open a image file. A series of control points will appear indicating the structure. There are 5 faces (front, left, right, top, bottom) initially.

2. Press Ctrl+M to move the control point. The id and the position of it is shown on the status bar.

3. You should carefully set the position of control points 0~3. 0 is origin; 1,2,3 shows the direction of X,Y,Z axis.

4. When 0~3 is set, press Ctrl+F to build the coordinate system.

   ![](./screenshots/2.png)

5. You can further edit the control points.

6. When there are less than 5 faces appear in the image, you can delete some control points by pressing Ctrl+D and clicking on it.

7. Now add extra points the each face. In particular, you should assign the face id to each corner of the image. This can be done by pressing Ctrl+1~5 and the selecting the point in image. Current face name is shown on the status bar.

8. Now you can press Ctrl+R to make 3D proxy of the scene. To view the model, press Alt+3 and use mouse to manipulate it.

   ![](./screenshots/4.png)

### Other results

Source image

![](./screenshots/7.jpg)


View 1

![](./screenshots/5.png)


View 2

![](./screenshots/6.png)

Source image 

![](./screenshots/10.jpg)


View 1

![](./screenshots/8.png)

View 2

![](./screenshots/9.png)


### Notice
To run the code, please place the `./Shader` directory to the working directory.

Rewriting the OpenGL invocation remains as further work.
