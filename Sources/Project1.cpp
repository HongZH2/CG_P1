/*
	Project 1 Submission for CMPSC458
    Name: Hong Zhang
    psu id: 965232865
    psu email: hmz5180@psu.edu
    Date: 09/15/2019 3:58pm
*/

#include <Project1.hpp>

// globals 
	// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

	// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

	// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;


std::string preamble =
"Project 1 code \n\n"
"Press the U,I,O to increase transformations \n"
"Press the J,K,L to decrease transformations \n"
"\tKey alone will alter rotation rate\n "
"\tShift+Key will alter scale\n "
"\tControl+Key will alter translation\n "
"Pressing G will reset transformations\n ";


/******************************************************************************
 * Set serval Static Variable to counts on Keyboard Input that will be used to
 * control the magnitude of Transformation
 * by Hong Zhang
 * Email: hmz5180@psu.edu
 * Data: 09/16/2019
********************************************************************************/
static int count_U = 0;
static int count_J = 0;
static int count_I = 0;
static int count_K = 0;
static int count_O = 0;
static int count_L = 0;

static int count_ctrol_U = 0;
static int count_ctrol_J = 0;
static int count_ctrol_I = 0;
static int count_ctrol_K = 0;
static int count_ctrol_O = 0;
static int count_ctrol_L = 0;

static int count_shift_U = 0;
static int count_shift_J = 0;
static int count_shift_I = 0;
static int count_shift_K = 0;
static int count_shift_O = 0;
static int count_shift_L = 0;
//******************************************************************************

//***********************************************************
// To make the camera walk on the heightmap
//************************************************************
bool close_2_ground = false;
bool assemble = false;
//******************************************************************************

int main(int argc, char **argv)
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X (left here as legacy, more would need to change)
    #endif

	 // Print Preamble
    std::printf(preamble.c_str());

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Project 1: Heightmap", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);


	// Set the required callback functions
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);


	// build and compile our shader program (defined in shader.hpp)
    // ------------------------------------
    Shader ourShader("../Project_1/Shaders/shader.vert", "../Project_1/Shaders/shader.frag");

	// set up vertex data (and buffer(s)) and configure vertex attributes for boxes
	// ------------------------------------------------------------------

	//   3D Coordinates   | Texture Coordinates
	//    x     y     z   |  s     t  
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};


	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

    glm::vec3 cubePositions_a[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(0.625f,  0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(-0.6255f,  0.0f, 0.0f),
        glm::vec3(-0.25f,  -1.0f, 0.0f),
        glm::vec3(0.25f,  -1.0f, 0.0f),
        glm::vec3(0.0f,  2.0f, -2.5f),
        glm::vec3(1.0f,  2.0f, -2.5f),
        glm::vec3(-1.0f,  2.0f, -2.5f),
        glm::vec3(0.0f, 1.0f, -2.5f)
    };


	unsigned int indices[] = {
		0, 1, 3, // first triangle
        1, 2, 3  // second triangle
	};


	//  1.  Create ID / Generate Buffers and for Vertex Buffer Object (VBO), 
	//      Vertex Array Buffer (VAO), and the Element Buffer Objects (EBO)
	unsigned int VBO, VAO, EBO;

	// 2. Bind Vertex Array Object
	glGenVertexArrays(1, &VAO);

	//  Bind the Vertex Buffer
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	
	// 3. Copy our vertices array in a vertex buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 4. Copy our indices array in a vertex buffer for OpenGL to use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 5.  Position attribute for the 3D Position Coordinates and link to position 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// 6.  TexCoord attribute for the 2d Texture Coordinates and link to position 2
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);



    unsigned int head_texture = loadTexture("../Project_1/Media/textures/5.png");
    unsigned int face_texture = loadTexture("../Project_1/Media/textures/1.png");
    unsigned int body_texture = loadTexture("../Project_1/Media/textures/2.png");
    unsigned int lface_texture = loadTexture("../Project_1/Media/textures/3.png");
    unsigned int rface_texture = loadTexture("../Project_1/Media/textures/4.png");
    unsigned int bbo_texture = loadTexture("../Project_1/Media/textures/6.png");
    unsigned int lt_texture = loadTexture("../Project_1/Media/textures/7.png");
    unsigned int la_texture = loadTexture("../Project_1/Media/textures/8.png");
    unsigned int bat_texture = loadTexture("../Project_1/Media/textures/9.jpeg");

    /******************************************************************************
     * Load six pictures for making the skybox
     * by Hong Zhang
     * Date: 09/22/2019
    ********************************************************************************/
    unsigned int front_texture = loadTexture("../Project_1/Media/skybox/front.tga");
    unsigned int back_texture = loadTexture("../Project_1/Media/skybox/back.tga");
    unsigned int left_texture = loadTexture("../Project_1/Media/skybox/left.tga");
    unsigned int right_texture = loadTexture("../Project_1/Media/skybox/right.tga");
    unsigned int top_texture = loadTexture("../Project_1/Media/skybox/top.tga");
    //std::cout<< top_texture <<std::endl;
    unsigned int bottom_texture = loadTexture("../Project_1/Media/skybox/bottom.tga");
     //--------------------------------------------------------------------------

	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	// -------------------------------------------------------------------------------------------
	ourShader.use(); // don't forget to activate/use the shader before setting uniforms!
					 // either set it manually like so:
	//Setting it using the texture class
	ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);

	// init heightmap (defined in heightmap.hpp)
    Heightmap heightmap("../Project_1/Media/heightmaps/hflab4.jpg");
	
    // render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
		// Set background color (shouldn't need it in the end b/c the box should fully cover everything
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        // activate shader
        ourShader.use();

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);

        // camera/view transformation
        //glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("view", view);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if(close_2_ground){
            int i = floor((camera.Position[0]+50)/100*heightmap.width);
            int j = floor((camera.Position[2]+50-3)/100*heightmap.height);
            camera.Position[1] = 50*heightmap.vertices.at(i*heightmap.height+j).Position.y-40;
         }
         if(assemble){
             for (unsigned int i = 0; i < 10; i++)
             {
                 glm::mat4 model;
                 // Translate the model to the cube starting position
                 model = glm::translate(model, cubePositions_a[i]);
                 switch (i) {
                 case 0: {
                     // calculate the model matrix for each object and pass it to shader before drawing
                     model = glm::translate(model, glm::vec3(0.0f, (0.0f+count_ctrol_U+count_ctrol_J)/10, 0.0f));

                     glActiveTexture(GL_TEXTURE0);
                     glBindTexture(GL_TEXTURE_2D, head_texture);
                     glActiveTexture(GL_TEXTURE1);
                     glBindTexture(GL_TEXTURE_2D, body_texture);
                     // render boxes
                     glBindVertexArray(VAO);
                     // Set model in shader
                     ourShader.setMat4("model", model);

                     // Draw the box with triangles
                     glDrawArrays(GL_TRIANGLES, 0, 36);
                 }
                     break;
                 case 1:
                 case 3:
                 {
                     // calculate the model matrix for each object and pass it to shader before drawing
                     model = glm::translate(model, glm::vec3(0.0f, (0.0f+count_ctrol_U+count_ctrol_J)/10, 0.0f));

                     model = glm::translate(model, glm::vec3(0.0f,0.5f,0.0f));
                     model = glm::rotate(model, glm::radians((float)(count_U-count_O)), glm::vec3(1.0f, 0.0f, 0.0f));
                     model = glm::translate(model, glm::vec3(0.0f,-0.5f,0.0f));

                     glActiveTexture(GL_TEXTURE0);
                     glBindTexture(GL_TEXTURE_2D, la_texture);
                     glActiveTexture(GL_TEXTURE1);
                     glBindTexture(GL_TEXTURE_2D, head_texture);
                     // render boxes
                     glBindVertexArray(VAO);
                     // Set model in shader

                     model = glm::scale(model, glm::vec3(0.25f,1.0f,0.5f));
                     ourShader.setMat4("model", model);

                     // Draw the box with triangles
                     glDrawArrays(GL_TRIANGLES, 0, 36);
                 }
                     break;
                 case 4:
                 case 5:
                 {
                     // calculate the model matrix for each object and pass it to shader before drawing
                     model = glm::translate(model, glm::vec3(0.0f, (0.0f+count_ctrol_U+count_ctrol_J)/10, 0.0f));

                     glActiveTexture(GL_TEXTURE0);
                     glBindTexture(GL_TEXTURE_2D, la_texture);
                     glActiveTexture(GL_TEXTURE1);
                     glBindTexture(GL_TEXTURE_2D, head_texture);
                     // render boxes
                     glBindVertexArray(VAO);
                     // Set model in shader

                     model = glm::scale(model, glm::vec3(0.5f,1.0f,0.5f));
                     ourShader.setMat4("model", model);

                     // Draw the box with triangles
                     glDrawArrays(GL_TRIANGLES, 0, 36);
                 }
                     break;
                 case 2:
                 {
                     // calculate the model matrix for each object and pass it to shader before drawing
                     model = glm::translate(model, glm::vec3(0.0f, (0.0f+count_ctrol_U+count_ctrol_J)/10, 0.0f));

                     model = glm::rotate(model, glm::radians((float)(count_J-count_L)), glm::vec3(0.0f, 1.0f, 0.0f));
                     model = glm::rotate(model, glm::radians((float)(-(count_K+count_I))), glm::vec3(1.0f, 0.0f, 0.0f));

                     glActiveTexture(GL_TEXTURE0);
                     glBindTexture(GL_TEXTURE_2D, face_texture);
                     glActiveTexture(GL_TEXTURE1);
                     glBindTexture(GL_TEXTURE_2D, head_texture);
                     // render boxes
                     glBindVertexArray(VAO);
                     // Set model in shader
                     ourShader.setMat4("model", model);

                     // Draw the box with triangles
                     glDrawArrays(GL_TRIANGLES, 0, 36);
                 }
                     break;
                 default: {
                     // calculate the model matrix for each object and pass it to shader before drawing
                     glActiveTexture(GL_TEXTURE0);
                     glBindTexture(GL_TEXTURE_2D, bat_texture);
                     glActiveTexture(GL_TEXTURE1);
                     glBindTexture(GL_TEXTURE_2D, bat_texture);
                     // render boxes
                     glBindVertexArray(VAO);
                     // Set model in shader
                     ourShader.setMat4("model", model);

                     // Draw the box with triangles
                     glDrawArrays(GL_TRIANGLES, 0, 36);
                 }
                     break;
                 }
             }
         }
         else {
                 // bind textures on corresponding texture units in fragment shader
                 glActiveTexture(GL_TEXTURE0);
                 glBindTexture(GL_TEXTURE_2D, bat_texture);
                 glActiveTexture(GL_TEXTURE1);
                 glBindTexture(GL_TEXTURE_2D, bat_texture);


                 // render boxes
                 glBindVertexArray(VAO);

                for (unsigned int i = 0; i < 10; i++)
                {
                    // calculate the model matrix for each object and pass it to shader before drawing
                    glm::mat4 model;
                    // Translate the model to the cube starting position
                    model = glm::translate(model, cubePositions[i]);
                    // Rotate the cube by an angle
                    float angle = 20.0f * i;
                    model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));

                    /******************************************************************************
                     * The following Operations are used to achieve the Rotation, Scaling, Translation
                     * transfermation on these cubes.
                     * by Hong Zhang
                     * Data: 09/16/2019
                    ********************************************************************************/

                    // Adding the Rotation Affects with the time going
                    model = glm::rotate(model, (count_U + count_J)*(float)glfwGetTime(), glm::vec3(1.0f, 0.0f, 0.0f));
                    model = glm::rotate(model, (count_I + count_K)*(float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
                    model = glm::rotate(model, (count_O + count_L)*(float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

                    //Adding the Scaling Affects on the Cubes along X/Y/Z axis.
                    model = glm::scale(model, glm::vec3(1.0f+count_shift_U+count_shift_J, 1.0f, 1.0f));
                    model = glm::scale(model, glm::vec3(1.0f, 1.0f+count_shift_I+count_shift_K, 1.0f));
                    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f+count_shift_O+count_shift_L));

                    //Adding the Translation Affects on the Cubes along X/Y/Z axis
                    model = glm::translate(model, glm::vec3(0.0f+count_ctrol_U+count_ctrol_J, 0.0f, 0.0f));
                    model = glm::translate(model, glm::vec3(0.0f, 0.0f+count_ctrol_I+count_ctrol_K, 0.0f));
                    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f+count_ctrol_O+count_ctrol_L));

                    //******************************************************************************

                    // Set model in shader
                    ourShader.setMat4("model", model);

                    // Draw the box with triangles
                    glDrawArrays(GL_TRIANGLES, 0, 36);
                }
        }
        /******************************************************************************
         * Create the skybox
         * by Hong Zhang
         * Data: 09/22/2019
        ********************************************************************************/
        for(int skypic_i = 1; skypic_i<=6 ;skypic_i++) {
             // Bind new textures to boh texture positions (do both since it has 2 textures in the vertex shader)
            glm::mat4 model;
            // Activate Texture 0
            glActiveTexture(GL_TEXTURE0);
            switch (skypic_i) {
            case 1:
                 glBindTexture(GL_TEXTURE_2D, front_texture);
                break;
            case 2:
                 glBindTexture(GL_TEXTURE_2D, back_texture);
                break;
            case 3:
                 glBindTexture(GL_TEXTURE_2D, top_texture);
               break;
            case 4:
                 glBindTexture(GL_TEXTURE_2D, bottom_texture);
                break;
            case 5:
                 glBindTexture(GL_TEXTURE_2D, left_texture);
                break;
            case 6:
                 glBindTexture(GL_TEXTURE_2D, right_texture);
                break;
            }
            // Activate Texture 1
            glActiveTexture(GL_TEXTURE1);

            if(close_2_ground == false){
                // make the fornt, back, top, left, right side of skybox follow the movement of camera
                model = glm::translate(model, camera.Position);
            }

            switch (skypic_i) {
            case 1: {
                 glBindTexture(GL_TEXTURE_2D, front_texture);
            }
                break;
            case 2:  {
                 glBindTexture(GL_TEXTURE_2D, back_texture);
                 model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));

            }
                break;
            case 3:  {
                glBindTexture(GL_TEXTURE_2D, top_texture);
                model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
                model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

            }
                break;
            case 4:  {
                glBindTexture(GL_TEXTURE_2D, bottom_texture);
                model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
                model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            }
                break;
            case 5:  {
                 glBindTexture(GL_TEXTURE_2D, left_texture);
                 model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            }
                break;
            case 6:  {
                 glBindTexture(GL_TEXTURE_2D, right_texture);
                 model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            }
                break;
            }


            // Make the model for one wall and shift/scale it
            model = glm::scale(model, glm::vec3(100.0f, 100.0f, 100.0f));
            // Set model in shader
            ourShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
        //******************************************************************************************


		// Draw the heightmap (defined in heightmap.hpp)  Similar to above but you have to write it.
        heightmap.Draw(ourShader, bottom_texture, camera, close_2_ground);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
        glfwPollEvents();

	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;


}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
		camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

	// Add other key operations here.  

    /******************************************************************************
     * operate the Keyboard Input Instruction
     * by Hong Zhang
     * Email: hmz5180@psu.edu
     * Data: 09/16/2019
    ********************************************************************************/
    // Press Key 'G' to Reset all the Transformation
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS){
        count_U = 0;
        count_U = 0;
        count_J = 0;
        count_I = 0;
        count_K = 0;
        count_O = 0;
        count_L = 0;

        count_ctrol_U = 0;
        count_ctrol_J = 0;
        count_ctrol_I = 0;
        count_ctrol_K = 0;
        count_ctrol_O = 0;
        count_ctrol_L = 0;

        count_shift_U = 0;
        count_shift_J = 0;
        count_shift_I = 0;
        count_shift_K = 0;
        count_shift_O = 0;
        count_shift_L = 0;

        close_2_ground = false;
        camera.Position = glm::vec3(0.0f, 0.0f, 3.0f);
    }
    // Press Key 'U', 'J', 'I', 'K', 'O', 'L' to change the rate of Rotation about X,Y,Z axiz.
    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)!= GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)!= GLFW_PRESS)
    {
        if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
            count_U++;
        if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
            count_J--;
        if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
            count_I++;
        if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
            count_K--;
        if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
            count_O++;
        if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
            count_L--;
    }
    else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        // Press Botton 'SHIFT + U/J/I/K/O/L' to Increase or Decrease the Scale in X/Y/Z axiz.
        if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
            count_shift_U++;
        if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
            count_shift_J--;
        if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
            count_shift_I++;
        if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
            count_shift_K--;
        if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
            count_shift_O++;
        if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
            count_shift_L--;

    }
    else
    {
        // Press Botton 'CONTROL + U/J/I/K/O/L' to Increase or Decrease the Scale in X/Y/Z axiz.
        if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
            count_ctrol_U++;
        if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
            count_ctrol_J--;
        if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
            count_ctrol_I++;
        if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
            count_ctrol_K--;
        if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
            count_ctrol_O++;
        if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
            count_ctrol_L--;
    }
    //********************************************************************
    // Transfer to Climbing Model !!!!!!!!!!!!!!!!!!!!!!!!!!
    if(glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
    {
       close_2_ground = true;
    }
    if(glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
    {
        close_2_ground =false;
    }
    // Transfer to BATMAN MODEL!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    if(glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
    {
       assemble = true;
    }
    if(glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
    {
       assemble = false;
    }

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}



// utility function for loading a 2D texture from file - Note, you might want to create another function for this with GL_CLAMP_TO_EDGE for the texture wrap param
// ---------------------------------------------------
unsigned int loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}


