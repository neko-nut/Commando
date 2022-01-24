# Commando

## Video

The YouTube link for the video: \href{https://youtu.be/ijUu01PGspY}{https://youtu.be/ijUu01PGspY}

## Game Introduce

Commando is a 3D shooting game. In the game, the player can control the main character, move around the small island, hide and fight with enemies, and reach the target point. The enemies will come closer to the main character. The main character can shoot the enemy and kill them. The killed enemy will disappear. Besides, there are several houses as covers. Both the main character and the enemies cannot go through the cover, and the main character cannot shoot the enemy through the covers. All the objects are displayed on the ground with green grass, and the environment around it is the ocean. The movement of the character and the enemies has been limited, they can only move on the ground. 

### Objects

In this game, the main character is displayed as the center of the screen. It is able to move, tune around, and shoot the enemy. The arm and the leg can sway when it is moving. The color of the main character is green and light yellow, and the color of the enemies are orange. If the player aimed at the enemy, the color of the enemy will change to red. Besides, the target is displayed as a white cube. The following images are the objects that have been used in the game. 

![Objects](.\readmeimage\objects.JPG)

### Views

For the normal view, the camera is located behind the main character. In this view, the main character can move and tune around, but cannot shoot the enemy. There is also a shooting view. For this view, the camera is located in front of the main character, and there is an aim at the center of the screen. In this view, the player can aim and shoot the enemy or turn around, but they cannot move the main character. This aim can help the player to check whether they really point to the enemy or not. Besides, there is a minimum map located at the top-left corner of the screen. This minimum map displays the top-down view of the game, and the camera is always located directly above the main character. The following images display the two views of the game.

![Views](.\readmeimage\views.JPG)

### States

If they touch the main character, game over, and if the main character reaches the target, the player wins. If the game is over or the player wins the game, a board will be displayed on the screen to show the states of the game, and the objects cannot move until the game is restarted. The following images show the two states where the game attend the end.

![Views](.\readmeimage\states.JPG)

## Operations

### Keyboard

In this game, the keyboard has been used to control the main character. If the player press down the 'w' or 's' key, the main character can go forward or go backwards. The direction of the movement (where is the front and where is the back) is controlled by key 'a' and 'd'. When pressing down the 'a', both the view and the moving direction will tune left, and it will tune right when the user press down the 'd'. Besides, the space bar has been used to switch between two views (see figure 5 and 6). Specifically, the 'w' key and 's' key is forbidden in the aim view.

### Mouse

The function of the mouse changes in different game states and game views. 

If the game state is game over or win (the screen will be like figure 7 and 8), the lift click (press down the left button on the mouth) will restart the game. If restart, the main character and all the enemies will go back to their initial place. During the game, the left click (press down) is used for shooting. If the view is aim view (like figure 6), and the aim in the center of the screen point to an enemy (and this enemy is red instead of orange), the left click can shoot and kill the enemy. The killed enemy will disappear and cannot touch the main character anymore.

As for the right-click (press down the left button on the mouth), if the view is the normal view (like figure 5), it will switch the mouse movement tracking. If the player turns on the mouse movement tracking, the view can rotate with the mouse. The operation only affects the view but does not influence the direction of movement. Right-click (press down) again can turn off the mouse movement tracking, and the view will back to the default view. The mouse movement tracking is forbidden in the aim view. If the player switches to the aim view when the mouse movement tracking is open, it will turn it off, and the player cannot open it if they do not go back to the normal view.

## Technical Choices

### 2D objects and 3D Mesh

In this game, there are three different kinds of objects: 2D objects the display on the screen directly, the model generated by OpenGL, and the mesh load from the existing file.

The little aim in the aim view (you can see in the center of the figure 6), and the board that display the game states (see figure 7 and 8) are 2D objects that directly display on the screen. They should display above all the objects and does not move when the view changes. So they are created as 2D objects. They only have the 2D coordinates relevant to the screen, but no 3D coordinates that are relevant to the camera.

The main character is built by cubes created directly by OpenGL. To generate this cube, I define the relevant coordinates directly in the code. These 3D coordinates include the location of the points in the 3D coordinates and the normal vertex for each face. Considering that the texture is not needed for this object, the texture coordinates have not been added. These data have been stored in the cube's VBO, separated and assigned by the cube's VAO, and delivered to the shader by OpenGL. As for how to build the character through the cube, the detail will be described in the Movement and Hierarchy part.

The more complex model like the enemy and the house are loaded through the existing object from the 3D mesh files on the internet. The only difference between these models and the model created by OpenGL is the way to get the relevant coordinates. For these meshes, we use the $assimp$ library to read the file and get the 3D coordinates, normal vectors, and texture coordinate. Similar to the model generated by OpenGL, we store these relevant coordinates in VBO, separate and assign by VAO, and deliver to the shader.

### Camera View

As we all know, how objects are displayed on the screen is calculated by three matrices: the view matrix that defines the position of the camera, the projection matrix that is used to measure how objects project on the camera screen, and the model matrix that define the position of the objects. The projection matrix uses the $projection()$ function in the math library to generate the matrix that defines the horizon, including the visual field, and the nearest and the farthest place that can be seen. The view matrix and the model matrix are generated by the $identity\_mat4()$ function in the math library, which create a standard $4*4$ matrix with 1 on diagonal and 0 for others.  

In general, this game has three views: the normal view (see figure 5), the aim view (see figure 6), and a top-down view located at the left top corner of the screen. Besides, right-clicking the mouse can also change the view. The change of view is implemented by adding rotate matrix and translate the matrix to the view matrix. The rotate matrix is generated by $rotate\_y\_deg() (or rotate\_x\_deg(), rotate\_z\_deg())$ function in the math library, and the translate matrix is generated by $translate()$ function. These functions can be used to adjust and modify the view.

The mini-map is implemented by $glViewport()$ function in OpenGL. After rendering the objects we want in the main window, this function has been used to open a new window located in the top left corner. Then we define a new view and rotate $90^{\circ}$ around the x-axis to change the view to the top-down view. Then we render the objects again in this small window.

### Hierarchy

As mentioned before, there are three matrices that have been used to control the final objects that we can see. The projection matrix is used to handle the projection, and the view matrix is used to handle the position of the camera. So the model view has been used to handle the object directly. In this game, the object movement is implemented by modifying the model matrix before drawing the objects. For instance, if we have a mesh that is too small, we can scale the matrix to make it look larger. Besides, we also transform the matrix to put the object in the place we want. The hierarchy is the essential part of analysising the model matrix. It means that, if we use matrix A to multiply matrix B, the motion of matrix B will follow matrix A.

The hierarchy has been used anywhere in the game. We first create a basic model as the root of the hierarchy. Then create the ground model, scale it and translate it in a proper space. Bind the ground model to the root model. Then we create the model for the enemy, scale it to a proper size, then translate it, let it move toward the main character. The way it moves will be introduced in the following part. After analysis of the enemy mesh, the model has been bound to the ground model so the enemy can move with the ground. Then do the same operation with the house.

The actions of the main character are also implemented by the hierarchy. The main character (see in figure 1) has been built by cubes with different colors. The hierarchy of this object has been shown in the figure 9. The model that controls the overall movement is called the human matrix. Each part of the body has its own matrix that handles the cube to its proper size and place. The body model has been bound directly on the human model, and the head, shoulders, and bottoms are bound to the body. The arms and legs are bound to the shoulders and bottoms. When the main character is moving, the shoulders matrix and bottoms matrix (the little green cubes) will rotate around their x-axis. The arms and legs (the light yellow cubes) will rotate with the shoulders and bottoms. 

![Views](.\readmeimage\human.JPG)


### Light and Color

In this game, the light and color are implemented in the shader. If the mesh does not have any texture, we need to define its color. This color is a $vec3$ which has three values that represent the RGB values. As for the light, I define the light position, intensity, and color directly in the shader, and analysis it in the shader. In this game, we just assume that all the objects have the same material so we define this information directly in the shader. In this game, the source of the light is located at 10, 10, 4. Then we use the coordinate of the vertex and the normal vector of the face to calculate the light on the objects. For all the objects there is light in the environment, and there is the diffuse reflection and the specular reflection. For the refection calculation, we should first calculate the eye coordinate, which is the matrix calculated by the camera view and the model of the object. Then we use the $normalize()$ function to calculate the unit vector of the normal vector and the direction from light to the position we look at. The power of diffuse reflection is calculated by the view coordinate, object coordinate, and the direction of the normal vector. The specular reflection is similar to the diffuse reflection, but it also needs to consider the reflection angle.

### Texture

The texture in the OpenGL is independent of the mesh. If we want to add texture to a mesh (created by OpenGL or load from other files), we need to load the texture and bind it to its corresponding condensates. In this game, we use the $stb_image$ library to load the image and convert it to a character string. Then assign an ID and the spaces to store it. When we want to use this texture, we will active it ($glActiveTexture()$) with its ID, and bind it ($glBindTexture()$). As for the mesh, it must have the texture coordinate if we want to add texture to this mesh. Each vertex on the mesh has its own texture coordinates that represent the corresponding point on the texture. This coordinate has two values that represent the x and y on the texture image. Then we have to deliver the texture and the texture coordinates to the shader. The type of the texture is $sampler2D$, and the type of the coordinate is $vec2$. The function $texture()$ can be used in the shader to combine them together.  

### Skybox

Skybox is the box that is used to display the environment. It should always be located at the back of all the objects and does not move. Similar to other mesh, the skybox defines the coordinate of the vertex of a cube in OpenGL, and display it with the given view, projection and model. However, it will convert the $4*4$ matrix to $3*3$ matrix to disable the movement. Besides, it needs a specific way to load the texture, which is called cubemap. For cubemap, we need first load 6 textures for 6 faces of the cube. Then the OpenGL will combine them together as a 3D cube (by $glTexParameteri()$ with parameters $GL\_TEXTURE\_CUBE\_MAP$). Deliver this texture cube to the shader, and the type of this texture is $samplerCube$. The texture coordinate will also become 3D to represent the vertex's corresponding points in the 3D texture. We also use the $texture()$ function to combine them together. Besides, to ensure that the skybox is located at the back of all the objects, we have to disable the deep buffer when we render it, and render it before rendering other objects. 

### Movement and Collision


Although it is a 3D game, the main character can only move on a plain, so for the location of the objects, we only need to consider its 2D coordinates. Considering that the camera is always moving with the main character, we do not actually move the main character. Instead, we move the ground model (the enemy and the house will move with the ground as we mentioned in the Hierarchy part) to the opposite direction to simulate the movement of the main character. For each frame, it will move 1 unit length, and the direction is calculated by the rotate angle (rotate operations has been mentioned in the Keyboard part). We use the $sin()$ and $cos()$ function to calculate the x component and y component for the unit vector and modify the coordinate of the main character. The coordinate of the main character will deliver them to the $translate()$ function for the ground model matrix as parameters to move the ground. The movement for the enemies is similar. For each frame, it moves a unit vector from the enemy to the main character. 

ColliderBox is an important concept when we think about the collision. It is the trigger of the collision. This box is always a cube that is a little bit larger than the object with an irregular shape. If anything has an intersection with this box, we define it as a collision with this object. The ColliderBox can help to reduce the calculation difficulty when we estimate the collision for the object with an irregular shape. In this game, the ColliderBox can be defined as a rectangle rather than a cube, because we do not need to consider the height of the object. Figure 10 is an example of how the ColliderBox has been defined in this game. The black rectangle is the ColliderBox of the inside star, if anything has any intersection with the rectangle, we think that the collision happens between this object and the star.

This game has two types of collision: the collision between the main character and other objects like the enemy or house, and the collision between the shooting ray and the enemy. The first one has been used to ensure that the main character cannot go into the house, and estimate that whether the enemy touches the main character or not. The second one is used to estimate whether the player can shoot the enemy.

For the first type of collision, I assume that the main character is a point. If this point inside the enemy's ColliderBox, we can change the game state to the game over directly. This estimate needs to do in every round because the enemy will move towards the player. The estimate about whether the point of the main character inside the house's ColliderBox happens when the main character is moving. After we calculate the next step of the main character, we do the estimation. If the collision happens, we will estimate the house is in front of the main character or it is next to the main character. It can be implemented by calculating whether the x or y coordinate of the main character is between the two x or y boundaries of the ColliderBox. If x is between the x boundary of the ColliderBox, we back x to the previous step. If y is between the x boundary of the ColliderBox, we do the same operation to y. Besides, we also do a similar operation to analyse the movement of the enemy.

![Views](.\readmeimage\box.JPG)

The second collision is calculated whether the ray is going through the enemy's ColliderBox. This ray is towards the front of the main character. With the position of the main character and the angle of its rotation, we can calculate the linear equation of the ray. We can see in the figure 11, if the ray has an intersection with any boundary of the ColliderBox, we can say that the ray has an intersection with the ColliderBox. This intersection can be estimated by substituting the x coordinate or y coordinate to the learner equation, and calculating whether the result is between the boundary. There is a problem if we use this way to estimate the collision. We do not think about whether we can see the enemy or not. Sometimes the enemy is in the back of the player's view, or it has been sheltered from the house, so it cannot be shot actually. To solve this problem, we calculate two vectors: the vector from the main character to the target enemy, and the unit vector of the direction of the view. Then we calculate whether they are in the same direction or the opposite direction. Then we do similar things to the houses. If one of the houses also have a collision with the ray, and it is closer to the main character than the enemy, we will determine that the house cover the enemy so we cannot shoot it.
