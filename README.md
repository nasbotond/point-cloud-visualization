# Point Cloud Visualization

This is a point cloud visualization tool written in C++ with OpenGL and Imgui. The points are provided as a text file containing the 3D position of the point, two color coordinates, two texture coordinates, and the normal vector of the point. The texture coordinates correspond to two images of
 the object represented by the points (also provided in the Assets folder). 

The user interface is made using Imgui. Within the user interface, there are options to change the skybox, to render the points in either of the two colors, to render the points in either of the textures, to render the normal vectors of the points, and to render a variable sized rectangle for each point taken from one of the two texture images.


| ![Point cloud rendered](https://user-images.githubusercontent.com/49843205/182183355-2ffccd17-faa8-451a-9087-13cdf56ae337.JPG) |
| :--: |
| Point cloud rendered |

| ![Point cloud with normals](https://user-images.githubusercontent.com/49843205/182183359-664da501-63f5-4516-870b-da56f06664e9.JPG) |
| :--: |
| Point cloud with normals |

| ![Rectangles using textures from image #1](https://user-images.githubusercontent.com/49843205/182183360-2c2c567f-ab07-47b5-a941-47369b9191e6.JPG) |
| :--: |
| Rectangles using textures from image #1 |

| ![Imgui menu](https://user-images.githubusercontent.com/49843205/182183364-976b5379-15bd-414b-b545-328416cebcc1.JPG) |
| :--: |
| Imgui menu |
