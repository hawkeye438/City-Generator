*******************************************************************************
Procedural World Project
*******************************************************************************
Create a procedural world with lighting, shadows and fog.

*******************************************************************************
Classes (general description)
*******************************************************************************
--BoundingBox--
-Checks for intersection base on the min/max points between two objects

--Building--
-Creates a series of cubes using CubePrimitive class.
-Applies a series of transformation and texture on these cubes and renders them.
-Set the BoundingBox min/max for each cube.

--Camera--
-Responsible for setting up the camera to view the world.
-Checks collision between the camera and objects in the world.
-Generates random scaling and texture for buildings to use.
-Set a limit for movement distance and checks for when surpassing that limit.
When the limit is passed, transport the camera to a different section of
the world to give the effect of continuous movement.Similar to walking around
a small planet.
-set up controls for camera movement and fog

--Cube Primitive--
-Creates and bind cubes using opengl VAO, VBO and etc.

--Lighting--
-Creates lighting
-Set up the lighting attributes and positioning. 
-Renders the lightinig.

--MyWindow--
-Class for redirecting callback functions.Since GLFW is built from C,
it requires us to redirect the call back functions if we wish to call a function
from a class.

---objloader (class given to us for assignments and etc)--
-Loads in obj file.  

--Shader--
-Reads the vertex and fragment shaders using the paths given.
-Creates the vertex and fragments shaders.
-Creates a shader program and links the vertex and fragment shader to it.

--Shadow--
-Set up the texture and other properties for shadows.
-Renders the shadow.

--Sphere Primitive--
-Creates and bind sphere using opengl VAO, VBO and etc.

--Skybox--
-Loads the images given by the string path.
-Creates the VAO and etc based on the skybox vertices.
-Sets up the texture for the cube map.
-Renders the cube map.

--Terrain--
-Creates a terrain mesh base on the values given.
-Creates the indices for UV, normals and EBO.
-Creates the VAO and etc based on the above values.
-Renders the terrain

--Texture--
-Set up the textures for the string path given

--Utility--
-Has a method to calculate and set the min and max vertices for vector of vertices.
-Has a method to apply matrix transformation to vector3f

*******************************************************************************
Controls (All controls are handle in Camera Class)
*******************************************************************************
------------
--Movement--
------------

--W--
-Moves the camera forward.

--S--
-Moves the camera backward.

--A--
-Moves the camera left.

--D--
-Moves the camera right.

--R--
-Moves the camera vertically upward.

--F--
-Moves the camera vertically downward.

-----------
---Mouse---
-----------

--Hold Left or Middle or Right Mouse buttons--
-Rotates camera base on mouse cursor direction.

--SPACEBAR--
-Lock the cursor into the screen and rotates the camera base on mouse cursor direction.

-----------
----Fog----
-----------

--U--
-Toggles through fog mode (forward).

--Shift+U--
-Toggles through fog mode (backward).

--J--
-Toggle debug mode

--K--
-Display fog information in console

--I--
-Increment fog density for exponential method 

--Shift+I--
-Decrement fog density for exponential method 

--O--
-Increment fog end value for linear method 

--Shift+O--
-Decrement fog end value for linear method 

--P--
-Increment fog density for linear method

--Shift+P--
-Decrement fog density for linear method

-----------
---Light---
-----------

--X--
-Increase light strength of the Sun

--C--
-Decrease light strength of the Sun

--V--
-Increase light strength of the Moon

--B--
-Decrease light strength of the Moon

--N--
-Increase light strength of the City Lights

--M--
-Decrease light strength of the City Lights
