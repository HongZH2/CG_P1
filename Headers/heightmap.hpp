#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <iostream>

#include <shader.hpp>

// To include the camera.hpp for geting the current position of Camera.
// -------------by Hong Zhang-------------------------------
#include <camera.hpp>
// Reference: https://github.com/nothings/stb/blob/master/stb_image.h#L4
// To use stb_image, add this in *one* C++ source file.
#include <stb_image.h>

struct Vertex {
	// position
	glm::vec3 Position;
	// texCoords
	glm::vec2 TexCoords;
};

// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class Heightmap
{
public:
	//heightmap attributes
	int width, height;

	// VAO for heightmap
	unsigned int VAO;

	// pointer to data - data is an array so can be accessed by data[x]. 
	//       - this is an uint8 array (so values range from 0-255)
	unsigned char *data;

	// heightmap vertices
	std::vector<Vertex> vertices;
	// indices for EBO
	std::vector<unsigned int> indices;


	// constructor
	Heightmap(const char* heightmapPath)
	{
		// load heightmap data
		load_heightmap(heightmapPath);

		// create heightmap verts from the image data - (you have to write this)
		create_heightmap();

		// free image data
		stbi_image_free(data);

		// create_indices - create the indices array (you have to write this)
		//  This is an optional step so and you can ignore this if you want to just create all the triangles rather than
		//     using this to index it.
		create_indices();

		// setup the VBO, VAO, and EBO and send the information to OpenGL (you need to write this)
		setup_heightmap();
	}

	// render the heightmap mesh (you need to write this)
    void Draw(Shader shader, unsigned int textureID, Camera &camera, bool &close_2_ground)
	{
		// You must:
		// -  active proper texture unit before binding
		// -  bind the texture
		// -  draw mesh (using GL_TRIANGLES is the most reliable way)

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glBindVertexArray(VAO);

        glm::mat4 model;
        if(close_2_ground == false){
            // make the fornt, back, top, left, right side of skybox follow the movement of camera
            model = glm::translate(model, camera.Position);
        }
        model = glm::translate(model, glm::vec3(-50.0f, -50.0f, -50.0f));
        model = glm::scale(model, glm::vec3(100.0f, 50.0f, 100.0f));
        shader.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	}

private:

	unsigned int VBO, EBO;

	// Load the image data
	void load_heightmap(const char* heightmapPath)
	{
		int nrChannels;
		data = stbi_load(heightmapPath, &width, &height, &nrChannels, 0);
		if (!data)
		{
			std::cout << "Failed to load heightmap" << std::endl;
		}

	}


	// Make Vertex:  take x and y position return a new vertex for that position which includes 
	//  the position and the texture coordinates
	//     The data is in a char c-array and can be access via  
	//           float(data[x*height + y]) / 255.0f 
	//      where x and y are varables between 0 and width or height  (just use a black and white image for simplicity)


    Vertex make_vertex(int x, int y, int t_x, int t_y)

	{
        Vertex v;
        //XYZ coords
        v.Position.x = float(x)/width;
        v.Position.y = float(data[x*height + y]) / 255.0f;
        v.Position.z = float(y)/height;
        //Texture Coords
        v.TexCoords.x = t_x;
        v.TexCoords.y = t_y;

        return v;
	}


	// convert heightmap to floats, set position and texture vertices using the subfunction make_vertex
	void create_heightmap()
	{
        for(int i = 0; i< width; i++){
            for(int j = 0; j < height ;j++){
                  vertices.push_back(make_vertex(i,j,i%2,j%2));
            }
        }
	}


	// create the indicies array for the EBO (so what indicies correspond with triangles to for rendering)
	//  This is an optional step so and you can ignore this if you want to just create all the triangles rather than
	//     using this to index it.
	void create_indices()
	{
        for(int i = 0; (i+1) < width; i++){
            for(int j = 0; (j+1) < height ;j++){
                // first
                indices.push_back(i * height + j);
                indices.push_back(i * height + j + 1);
                indices.push_back((i + 1) * height + j + 1);
                // second
                indices.push_back(i * height + j);
                indices.push_back((i + 1) * height + j);
                indices.push_back((i + 1) * height + j + 1);
            }
        }
	}


	// create buffers/arrays for the VAO, VBO, and EBO 
	// Notes
	//  -  sizeof(Vertex) returns the size of the vertex
	//  -  to get the pointer the underlying array, you can use "&vertices[0]"
	void setup_heightmap()
	{

        // Bind Vertex Array Object
        glGenVertexArrays(1, &VAO);

        //  Bind the Vertex Buffer
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);


        // Copy our vertices array in a vertex buffer for OpenGL to use
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices.size(), &vertices[0], GL_STATIC_DRAW);


        // Copy our indices array in a vertex buffer for OpenGL to use
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*indices.size(), &indices[0], GL_STATIC_DRAW);

        // Position attribute for the 3D Position Coordinates and link to position 0
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // TexCoord attribute for the 2d Texture Coordinates and link to position 2
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);


	}

};
#endif
