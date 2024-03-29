#include "Cube.h"


// cube ///////////////////////////////////////////////////////////////////////
//    v6----- v5
//   /|      /|
//  v1------v0|
//  | |     | |
//  | |v7---|-|v4
//  |/      |/
//  v2------v3

// Les positions des sommets
GLfloat CubeArray[72] =  {1,1,1,  -1,1,1,  -1,-1,1,  1,-1,1,        // v0-v1-v2-v3
                          1,1,1,  1,-1,1,  1,-1,-1,  1,1,-1,        // v0-v3-v4-v5
                          1,1,1,  1,1,-1,  -1,1,-1,  -1,1,1,        // v0-v5-v6-v1
                          -1,1,1,  -1,1,-1,  -1,-1,-1,  -1,-1,1,    // v1-v6-v7-v2
                          -1,-1,-1,  1,-1,-1,  1,-1,1,  -1,-1,1,    // v7-v4-v3-v2
                          1,-1,-1,  -1,-1,-1,  -1,1,-1,  1,1,-1};   // v4-v7-v6-v5

//Les couleurs des sommets
GLfloat CubeColor[72] =  {1,1,1,  1,1,0,  1,0,0,  1,0,1,              // v0-v1-v2-v3
                          1,1,1,  1,0,1,  0,0,1,  0,1,1,              // v0-v3-v4-v5
                          1,1,1,  0,1,1,  0,1,0,  1,1,0,              // v0-v5-v6-v1
                          1,1,0,  0,1,0,  0,0,0,  1,0,0,              // v1-v6-v7-v2
                          0,0,0,  0,0,1,  1,0,1,  1,0,0,              // v7-v4-v3-v2
                          0,0,1,  0,0,0,  0,1,0,  0,1,1};             // v4-v7-v6-v5

//Les normales aux sommets
GLfloat CubeNormal[72] = {0,0,1,  0,0,1,  0,0,1,  0,0,1,             // v0-v1-v2-v3
                          1,0,0,  1,0,0,  1,0,0, 1,0,0,              // v0-v3-v4-v5
                          0,1,0,  0,1,0,  0,1,0, 0,1,0,              // v0-v5-v6-v1
                          -1,0,0,  -1,0,0, -1,0,0,  -1,0,0,          // v1-v6-v7-v2
                          0,-1,0,  0,-1,0,  0,-1,0,  0,-1,0,         // v7-v4-v3-v2
                          0,0,-1,  0,0,-1,  0,0,-1,  0,0,-1};        // v4-v7-v6-v5

// Les indices des sommets
GLuint IndiceArray[24] = {0,1,2,3,
                          4,5,6,7,
                          8,9,10,11,
                          12,13,14,15,
                          16,17,18,19,
                          20,21,22,23};

Cube::Cube()
{
	_radius = 2.4;

	// G�n�ration des buffers
        glGenBuffers(5, (GLuint *)_cubeBuffers);

	// Buffer d'informations de sommets
    glBindBuffer(GL_ARRAY_BUFFER, _cubeBuffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(CubeArray), CubeArray, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Buffer des couleurs
	glBindBuffer(GL_ARRAY_BUFFER, _cubeBuffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(CubeColor), CubeColor, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

        //Buffer des normales
        glBindBuffer(GL_ARRAY_BUFFER, _cubeBuffers[2]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(CubeNormal), CubeNormal, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	// Buffer d'indices
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _cubeBuffers[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(IndiceArray), IndiceArray, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	_initVAO = false;

	reportGLError();
}

void Cube::createVAO(GQShaderRef& shader)
{
	// R�cup�ration des emplacements des deux attributs dans le shader
	_vertexPositionIndex = shader.attribLocation("vertex");
	_vertexColorIndex    = shader.attribLocation("color");
        _vertexNormalIndex   = shader.attribLocation("normal");

	// Cr�ation du VAO
        glGenVertexArrays(1, (GLuint *)&_vertexArrayObject);

	// On "bind" le VAO
	glBindVertexArray(_vertexArrayObject);

	  // On active le buffer des sommets
	  glBindBuffer(GL_ARRAY_BUFFER, _cubeBuffers[0]);
	  // On envoie les donn�es
          glVertexAttribPointer(_vertexPositionIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);
          glEnableVertexAttribArray(_vertexPositionIndex);

          reportGLError();
		
	if(_vertexColorIndex >=0)
	{
	  // On active le buffer de couleur
	  glBindBuffer(GL_ARRAY_BUFFER, _cubeBuffers[1]);

	  // On envoie les donn�es
          glVertexAttribPointer(_vertexColorIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);
          glEnableVertexAttribArray(_vertexColorIndex);

                reportGLError();
        }
        if(_vertexNormalIndex >=0)
        {
          // On active le buffer de normales
          glBindBuffer(GL_ARRAY_BUFFER, _cubeBuffers[2]);

          // On envoie les donn�es
          glVertexAttribPointer(_vertexNormalIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);
          glEnableVertexAttribArray(_vertexNormalIndex);

                reportGLError();
        }
			
	// On active le buffer des indices
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _cubeBuffers[3]);

	// On unbind le VAO, tout ceci est enregistr�
	glBindVertexArray(0);

	reportGLError();

	_initVAO = true;
}

void Cube::draw(GQShaderRef& shader)
{
	if(!_initVAO)
	  createVAO(shader);

	// On attache le VAO
	glBindVertexArray(_vertexArrayObject);

	// Rendu de la g�om�trie
        glDrawElements(GL_QUADS, 24, GL_UNSIGNED_INT, 0);

	// On d�tache le VAO
	glBindVertexArray(0);

	reportGLError();
}
