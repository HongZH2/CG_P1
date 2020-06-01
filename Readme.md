## Summary
the Project 1 code, pictures,video and Execute file.
## You need OpenGL library first
## Programming Assignment 1
## Transformation and Heightmaps in 3D
## Name: Hong Zhang
## Email: hmz5180@psu.edu
## Date: 09/23/2019
## Course Information: Computer Graphic
## Instructor: Prof. Yanxi Liu
## Programming Language: C++


## Visualization
![GIF](https://github.com/HongZH2/Graphics_P1/blob/master/Project_1_Hong_Zhang.gif)

## Description
(1) Interaction
Due to the final submission, I have already achieve all the basic  required functions to make these boxes move and interact with Keyboard Inputs. When It starts, the boxes stay still. And If you   press U/J/I/K/O/L only, that will make the boxes rotate about their own center about X/Y/Z  axis. Besides, inputing Left_Shift+U/J/I/K/O will give the boxes a signal to scaling along X/Y/Z. Also It will work with Translating boxes along X/Y/Z axis, if inputing Left_Control + U/J/I/K/O/L.

To achieve the above functions, Pushing buttons on the keyboard would make the corresponding counters itself increment or reduction that will be used to change the magnitude of Transformation in the each gameloop.

I have made my comments on these lines I added.

### Line Number
Project1.cpp
line 37 ~ line 64 : define some variables for increment of Rotaion, Translation and Scaling of Boxes
line 442 ~ line 464: compute the rotation, translation and scaling matrices.
line 589 ~line 680: Define the (Control/Shift) + U/J/I/K/O/L button function on the keyboard.

(2)Skybox
I have created a skybox filledl with 6 skypictures that are achieved by rotating them to the corresponding position. You can see all the pictures is exactly their position and they match well. Besides, I use Camera.Position to update the 6 walls of skybox. That is, When you move the camera with Keyboard W/S/A/D, the skybox will follow you that make the whole scene looks like still right there except the 10 boxes. By the way, the heightmap will also follow the movement of camera.

### Line Number
Project1.cpp
line 473 ~ line 554: Set textures for the 6 sides of skybox and rotate them to the right position.

(3)Heightmap
I generate it by finish these functions in the file 'H=heightmap.hpp'. Before entering the gameloop, Heightmap class has been initialed. Then I make the vertices and indices information to draw the heightmap. So in the gameloop trigger it by Heightmap::Draw() Function. I make some change for the parameter of this function, because I need to update the position of heightmap when camera moving. What's more, I modify the scale of heighmap and translate it to fill the whole bottom.

### Line Number
heightmap.hpp
Just write the class member funtion.

(4)Others
1. make the skybox follow the movement of camera, just as the above mentioned.

### Line Number
Project1.cpp
line 508: translate the skybox with the camera.position 
heightmap.hpp
line 86: translate the heightmap with the camera.position

2. I set a Climbing Model by pressing V on the keyboard. the camera will drop to the top of heightmap around 10.0f that looks like someone is climbing on the surface of heightmap. At this time, you can move yourself by W/S/A/D on the keyboard and the skybox and heightmap would not be 'locked'. that means you can climb to the boundary of skybox. But, if you reach out beyond the boundart of heightmap that probably makes the program crashed. Because I don't set the limiti for it. SORRY, I don't have enough time to do that. Finally, you can back to the sky by pressing C or G.

### Line Number
Project1.cpp
line 307 ~ line 311: To set the current the y value of camera.postion to the heightmap value

3. I set  a BATMAN MODEL for fun. You can transfer to that model by pressing B on the keyboard and back to the original by pressing N. Under this model, you can see a cute batman model in front of you. his head can be manipulated by pressing I/K/J/L. Pressing I/K botton will make him look up/down. And pressing J/L botton will make him look left/right.  Besides, U/O botton is the trigger to make his arm rise and down about his shoulder that is a translation and Rotation then translation back again operation. Pressing Control+U/J will make his whole body fly up and down. Finally, you can quit this model by pressing N.

### Line Number
Project1.cpp
line 312 ~ line 420: Recompute the position of Batman Boxes. 

## How to Run
The program is built on the directory CMPSC458/Project1/build. I ran it on the MacOS.

    open the terminal
    cd  ../CMPSC458/build
    ./Project_1/Project_1

Boxes Model:
    (Control/Shift)+U/J/I/K/O/L control the translation, rotation and scaling of boxes.

Climbing Model:
    Enter it by pressing V, Quit by pressing C.
    W/S/A/D to control the climber on the surface of heightmap.

BATMAN Model:
    Enter it by pressing B, Quit by pressing N.
    I/K/J/L to control his head look around.
    U/O to control his arm arise or dowm.
    Control+ U/J to make batman fly up and down.

# Tips
## 1. You can't use Right Shift and Right Control on the keyboard. 
## 2. You can't travel beyond the heightmap under the Climbing Model that will cause crash problem.

