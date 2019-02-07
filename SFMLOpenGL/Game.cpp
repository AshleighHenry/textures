#include <Game.h>
static bool flip;

Game::Game() : window(VideoMode(800, 600), "OpenGL Cube Texturing")
{
}

Game::~Game() {}

void Game::run()
{

	initialize();

	Event event;

	while (isRunning) {

#if (DEBUG >= 2)
		DEBUG_MSG("Game running...");
#endif

		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				isRunning = false;
			}
		}
		update();
		render();
	}

}

typedef struct
{
	float coordinate[3];
	float color[4];
	float texel[2];
} Verty;


Verty initVertex[36];
Verty vertex[36];
GLubyte triangles[36];

/* Variable to hold the VBO identifier and shader data */
GLuint	index,		//Index to draw
		vsid,		//Vertex Shader ID
		fsid,		//Fragment Shader ID
		progID,		//Program ID
		vao = 0,	//Vertex Array ID
		vbo[1],		// Vertex Buffer ID
		positionID, //Position ID
		colorID,	// Color ID
		to,			// Texture ID 1 to 32
		textureID,	//Texture ID
		texelID;	// Texel ID

//const string filename = "texture.tga";
//const string filename = "cube.tga";

const string filename = "cube.tga";

int width; //width of texture
int height; //height of texture
int comp_count; //Component of texture
const int number = 4; //4 = RGBA

unsigned char* img_data;

void Game::initialize()
{
	isRunning = true;
	GLint isCompiled = 0;
	GLint isLinked = 0;

	glewInit();

	DEBUG_MSG(glGetString(GL_VENDOR));
	DEBUG_MSG(glGetString(GL_RENDERER));
	DEBUG_MSG(glGetString(GL_VERSION));

	/* Vertices counter-clockwise winding */
	

	initalizeVertex();

	/*Index of Poly / Triangle to Draw */
	for (int i = 0; i < 36; i++)
	{
		triangles[i] = i;
	}

	/* Create a new VBO using VBO id */
	glGenBuffers(1, vbo);

	/* Bind the VBO */
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	/* Upload vertex data to GPU */
	glBufferData(GL_ARRAY_BUFFER, sizeof(Verty) * 9, vertex, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &index);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte) * 36, triangles, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	/* Vertex Shader which would normally be loaded from an external file */
	const char* vs_src = "#version 400\n\r"
		"in vec4 sv_position;"
		"in vec4 sv_color;"
		"in vec2 sv_texel;"
		"out vec4 color;"
		"out vec2 texel;"
		"void main() {"
		"	color = sv_color;"
		"	texel = sv_texel;"
		"	gl_Position = sv_position;"
		"}"; //Vertex Shader Src

	DEBUG_MSG("Setting Up Vertex Shader");

	vsid = glCreateShader(GL_VERTEX_SHADER); //Create Shader and set ID
	glShaderSource(vsid, 1, (const GLchar**)&vs_src, NULL); // Set the shaders source
	glCompileShader(vsid); //Check that the shader compiles

	//Check is Shader Compiled
	glGetShaderiv(vsid, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_TRUE) {
		DEBUG_MSG("Vertex Shader Compiled");
		isCompiled = GL_FALSE;
	}
	else
	{
		DEBUG_MSG("ERROR: Vertex Shader Compilation Error");
	}

	/* Fragment Shader which would normally be loaded from an external file */
	const char* fs_src = "#version 400\n\r"
		"uniform sampler2D f_texture;"
		"in vec4 color;"
		"in vec2 texel;"
		"out vec4 fColor;"
		"void main() {"
		//"	fColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);"
		"	fColor = texture(f_texture, texel.st);"
		"}"; //Fragment Shader Src

	DEBUG_MSG("Setting Up Fragment Shader");

	fsid = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fsid, 1, (const GLchar**)&fs_src, NULL);
	glCompileShader(fsid);
	//Check is Shader Compiled
	glGetShaderiv(fsid, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_TRUE) {
		DEBUG_MSG("Fragment Shader Compiled");
		isCompiled = GL_FALSE;
	}
	else
	{
		DEBUG_MSG("ERROR: Fragment Shader Compilation Error");
	}
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	DEBUG_MSG("Setting Up and Linking Shader");
	progID = glCreateProgram();	//Create program in GPU
	glAttachShader(progID, vsid); //Attach Vertex Shader to Program
	glAttachShader(progID, fsid); //Attach Fragment Shader to Program
	glLinkProgram(progID);

	//Check is Shader Linked
	glGetProgramiv(progID, GL_LINK_STATUS, &isLinked);

	if (isLinked == 1) {
		DEBUG_MSG("Shader Linked");
	}
	else
	{
		DEBUG_MSG("ERROR: Shader Link Error");
	}

	// Use Progam on GPU
	// https://www.opengl.org/sdk/docs/man/html/glUseProgram.xhtml
	glUseProgram(progID);

	img_data = stbi_load(filename.c_str(), &width, &height, &comp_count, 4);

	if (img_data == NULL)
	{
		DEBUG_MSG("ERROR: Texture not loaded");
	}

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &to);
	glBindTexture(GL_TEXTURE_2D, to);

	//Wrap around
	//https://www.khronos.org/opengles/sdk/docs/man/xhtml/glTexParameter.xml
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Filtering
	//https://www.khronos.org/opengles/sdk/docs/man/xhtml/glTexParameter.xml
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//Bind to OpenGL
	//https://www.khronos.org/opengles/sdk/docs/man/xhtml/glTexImage2D.xml
	glTexImage2D(GL_TEXTURE_2D, //2D Texture Image
		0, //Mipmapping Level 
		GL_RGBA, //GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA, GL_RGB, GL_BGR, GL_RGBA 
		width, //Width
		height, //Height
		0, //Border
		GL_RGBA, //Bitmap
		GL_UNSIGNED_BYTE, img_data);

	// Find variables in the shader
	//https://www.khronos.org/opengles/sdk/docs/man/xhtml/glGetAttribLocation.xml
	positionID = glGetAttribLocation(progID, "sv_position");
	colorID = glGetAttribLocation(progID, "sv_color");
	texelID = glGetAttribLocation(progID, "sv_texel");
	textureID = glGetUniformLocation(progID, "f_texture");
}

void Game::update()
{
	elapsed = clock.getElapsedTime();
	
	//Move the cube up 
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		m_displacmentVector.y += 0.0005f;
	}

	//moves cube down
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		m_displacmentVector.y -= 0.0005f;
	}

	//Move the cube left
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		m_displacmentVector.x -= 0.0005f;
	}

	//moves cube right
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		m_displacmentVector.x += 0.0005f;
	}

	//Scale down.
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		m_scale -= 0.005f;
	}

	//Scale up.
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		m_scale += 0.005f;
	}

	//Rotates X clockwise.
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
	{

		m_rotationAngleX += 0.0005f;

		if (m_rotationAngleX > 360.0f)
		{
			m_rotationAngleX -= 360.0f;
		}
	}

	//Rotates Y clockwise
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y))
	{
		m_rotationAngleY += 0.0005f;

		if (m_rotationAngleY > 360.0f)
		{
			m_rotationAngleY -= 360.0f;
		}
	}

	//Rotates Z clockwise
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
	{
		m_rotationAngleZ += 0.0005f;

		if (m_rotationAngleZ > 360.0f)
		{
			m_rotationAngleZ -= 360.0f;
		}
	}
	for (int i = 0; i < 36; i++)
	{
		MyVector3 tempVec{ initVertex[i].coordinate[0], initVertex[i].coordinate[1], initVertex[i].coordinate[2] };
		tempVec = MyMatrix3::scale(m_scale) * tempVec;
		tempVec += m_displacmentVector;
		tempVec = MyMatrix3::rotationX(m_rotationAngleX) * tempVec;
		tempVec = MyMatrix3::rotationY(m_rotationAngleY) * tempVec;
		tempVec = MyMatrix3::rotationZ(m_rotationAngleZ) * tempVec;

		vertex[i].coordinate[0] = tempVec.x;
		vertex[i].coordinate[1] = tempVec.y;
		vertex[i].coordinate[2] = tempVec.z;
	}


#if (DEBUG >= 2)
	DEBUG_MSG("Update up...");
#endif

}

void Game::render()
{

#if (DEBUG >= 2)
	DEBUG_MSG("Drawing...");
#endif

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);

	/*	As the data positions will be updated by the this program on the
		CPU bind the updated data to the GPU for drawing	*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(Verty) * 36, vertex, GL_STATIC_DRAW);

	/*	Draw Triangle from VBO	(set where to start from as VBO can contain
		model components that 'are' and 'are not' to be drawn )	*/

	//Set Active Texture .... 32
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(textureID, 0);

	// Set pointers for each parameter
	// https://www.opengl.org/sdk/docs/man4/html/glVertexAttribPointer.xhtml
	glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, sizeof(Verty), 0);
	glVertexAttribPointer(colorID, 4, GL_FLOAT, GL_FALSE, sizeof(Verty), (void*)(3* sizeof(float)));
	glVertexAttribPointer(texelID, 2, GL_FLOAT, GL_FALSE, sizeof(Verty), (void*)(7 * sizeof(float)));

	//Enable Arrays
	glEnableVertexAttribArray(positionID);
	glEnableVertexAttribArray(colorID);
	glEnableVertexAttribArray(texelID);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (char*)NULL + 0);

	window.display();

}

void Game::unload()
{
#if (DEBUG >= 2)
	DEBUG_MSG("Cleaning up...");
#endif
	glDeleteProgram(progID);
	glDeleteBuffers(1, vbo);
	stbi_image_free(img_data); //Free image
}

void Game::initalizeVertex()
{
	initVertex[0].coordinate[0] = -0.5f;
	initVertex[0].coordinate[1] = -0.5f;
	initVertex[0].coordinate[2] = -0.5f;

	initVertex[1].coordinate[0] = -0.5f;
	initVertex[1].coordinate[1] = -0.5f;
	initVertex[1].coordinate[2] = 0.5f;

	initVertex[2].coordinate[0] = -0.5f;
	initVertex[2].coordinate[1] = 0.5f;
	initVertex[2].coordinate[2] = -0.5f;

	initVertex[3].coordinate[0] = 0.5f;
	initVertex[3].coordinate[1] = 0.5f;
	initVertex[3].coordinate[2] = -0.5f;

	initVertex[4].coordinate[0] = -0.5f;
	initVertex[4].coordinate[1] = -0.5f;
	initVertex[4].coordinate[2] = -0.5f;

	initVertex[5].coordinate[0] = -0.5f;
	initVertex[5].coordinate[1] = 0.5f;
	initVertex[5].coordinate[2] = -0.5f;

	initVertex[6].coordinate[0] = 0.5f;
	initVertex[6].coordinate[1] = -0.5f;
	initVertex[6].coordinate[2] = 0.5f;

	initVertex[7].coordinate[0] = -0.5f;
	initVertex[7].coordinate[1] = -0.5f;
	initVertex[7].coordinate[2] = -0.5f;

	initVertex[8].coordinate[0] = 0.5f;
	initVertex[8].coordinate[1] = -0.5f;
	initVertex[8].coordinate[2] = -0.5f;

	initVertex[9].coordinate[0] = 0.5f;
	initVertex[9].coordinate[1] = 0.5f;
	initVertex[9].coordinate[2] = -0.5f;

	initVertex[10].coordinate[0] = 0.5f;
	initVertex[10].coordinate[1] = -0.5f;
	initVertex[10].coordinate[2] = -0.5f;

	initVertex[11].coordinate[0] = -0.5f;
	initVertex[11].coordinate[1] = -0.5f;
	initVertex[11].coordinate[2] = -0.5f;

	initVertex[12].coordinate[0] = -0.5f;
	initVertex[12].coordinate[1] = -0.5f;
	initVertex[12].coordinate[2] = 0.5f;

	initVertex[13].coordinate[0] = -0.5f;
	initVertex[13].coordinate[1] = 0.5f;
	initVertex[13].coordinate[2] = 0.5f;

	initVertex[14].coordinate[0] = -0.5f;
	initVertex[14].coordinate[1] = 0.5f;
	initVertex[14].coordinate[2] = -0.5f;

	initVertex[15].coordinate[0] = 0.5f;
	initVertex[15].coordinate[1] = -0.5f;
	initVertex[15].coordinate[2] = 0.5f;

	initVertex[16].coordinate[0] = -0.5f;
	initVertex[16].coordinate[1] = -0.5f;
	initVertex[16].coordinate[2] = 0.5f;

	initVertex[17].coordinate[0] = -0.5f;
	initVertex[17].coordinate[1] = -0.5f;
	initVertex[17].coordinate[2] = -0.5f;

	initVertex[18].coordinate[0] = -0.5f;
	initVertex[18].coordinate[1] = 0.5f;
	initVertex[18].coordinate[2] = 0.5f;

	initVertex[19].coordinate[0] = -0.5f;
	initVertex[19].coordinate[1] = -0.5f;
	initVertex[19].coordinate[2] = 0.5f;

	initVertex[20].coordinate[0] = 0.5f;
	initVertex[20].coordinate[1] = -0.5f;
	initVertex[20].coordinate[2] = 0.5f;

	initVertex[21].coordinate[0] = 0.5f;
	initVertex[21].coordinate[1] = 0.5f;
	initVertex[21].coordinate[2] = 0.5f;

	initVertex[22].coordinate[0] = 0.5f;
	initVertex[22].coordinate[1] = -0.5f;
	initVertex[22].coordinate[2] = -0.5f;

	initVertex[23].coordinate[0] = 0.5f;
	initVertex[23].coordinate[1] = 0.5f;
	initVertex[23].coordinate[2] = -0.5f;

	initVertex[24].coordinate[0] = 0.5f;
	initVertex[24].coordinate[1] = -0.5f;
	initVertex[24].coordinate[2] = -0.5f;

	initVertex[25].coordinate[0] = 0.5f;
	initVertex[25].coordinate[1] = 0.5f;
	initVertex[25].coordinate[2] = 0.5f;

	initVertex[26].coordinate[0] = 0.5f;
	initVertex[26].coordinate[1] = -0.5f;
	initVertex[26].coordinate[2] = 0.5f;

	initVertex[27].coordinate[0] = 0.5f;
	initVertex[27].coordinate[1] = 0.5f;
	initVertex[27].coordinate[2] = 0.5f;

	initVertex[28].coordinate[0] = 0.5f;
	initVertex[28].coordinate[1] = 0.50f;
	initVertex[28].coordinate[2] = -0.5f;

	initVertex[29].coordinate[0] = -0.5f;
	initVertex[29].coordinate[1] = 0.5f;
	initVertex[29].coordinate[2] = -0.5f;

	initVertex[30].coordinate[0] = 0.5f;
	initVertex[30].coordinate[1] = 0.5f;
	initVertex[30].coordinate[2] = 0.5f;

	initVertex[31].coordinate[0] = -0.5f;
	initVertex[31].coordinate[1] = 0.5f;
	initVertex[31].coordinate[2] = -0.5f;

	initVertex[32].coordinate[0] = -0.5f;
	initVertex[32].coordinate[1] = 0.5f;
	initVertex[32].coordinate[2] = 0.5f;

	initVertex[33].coordinate[0] = 0.5f;
	initVertex[33].coordinate[1] = 0.5f;
	initVertex[33].coordinate[2] = 0.5f;

	initVertex[34].coordinate[0] = -0.5f;
	initVertex[34].coordinate[1] = 0.5f;
	initVertex[34].coordinate[2] = 0.5f;

	initVertex[35].coordinate[0] = 0.5f;
	initVertex[35].coordinate[1] = -0.5f;
	initVertex[35].coordinate[2] = 0.5f;

	initVertex[0].texel[0] = 0.75f;
	initVertex[0].texel[1] = 0.25f;

	initVertex[1].texel[0] = 1.0f;
	initVertex[1].texel[1] = 0.25f;

	initVertex[2].texel[0] = 1.0f;
	initVertex[2].texel[1] = 0.50f;

	initVertex[3].texel[0] = 0.75f;
	initVertex[3].texel[1] = 0.50f;

	initVertex[4].texel[0] = 0.5f;
	initVertex[4].texel[1] = 0.25f;

	initVertex[5].texel[0] = 0.5f;
	initVertex[5].texel[1] = 0.50f;

	initVertex[6].texel[0] = 0.25f;
	initVertex[6].texel[1] = 0.0f;

	initVertex[7].texel[0] = 0.0f;
	initVertex[7].texel[1] = 0.25f;

	initVertex[8].texel[0] = 0.25f;
	initVertex[8].texel[1] = 0.25f;

	initVertex[9].texel[0] = 0.50f;
	initVertex[9].texel[1] = 0.50f;

	initVertex[10].texel[0] = 0.50f;
	initVertex[10].texel[1] = 0.25f;

	initVertex[11].texel[0] = 0.75f;
	initVertex[11].texel[1] = 0.25f;

	initVertex[12].texel[0] = 0.75f;
	initVertex[12].texel[1] = 0.25f;

	initVertex[13].texel[0] = 1.0f;
	initVertex[13].texel[1] = 0.50f;

	initVertex[14].texel[0] = 0.75f;
	initVertex[14].texel[1] = 0.50f;

	initVertex[15].texel[0] = 0.25f;
	initVertex[15].texel[1] = 0.25f;

	initVertex[16].texel[0] = 0.0f;
	initVertex[16].texel[1] = 0.25f;

	initVertex[17].texel[0] = 0.0f;
	initVertex[17].texel[1] = 0.0f;

	initVertex[18].texel[0] = 0.0f;
	initVertex[18].texel[1] = 0.50f;

	initVertex[19].texel[0] = 0.0f;
	initVertex[19].texel[1] = 0.25f;

	initVertex[20].texel[0] = 0.25f;
	initVertex[20].texel[1] = 0.25f;

	initVertex[21].texel[0] = 0.25f;
	initVertex[21].texel[1] = 0.50f;

	initVertex[22].texel[0] = 0.50f;
	initVertex[22].texel[1] = 0.25f;

	initVertex[23].texel[0] = 0.50f;
	initVertex[23].texel[1] = 0.50f;

	initVertex[24].texel[0] = 0.50f;
	initVertex[24].texel[1] = 0.25f;

	initVertex[25].texel[0] = 0.25f;
	initVertex[25].texel[1] = 0.25f;

	initVertex[26].texel[0] = 0.25f;
	initVertex[26].texel[1] = 0.50f;

	initVertex[27].texel[0] = 0.0f;
	initVertex[27].texel[1] = 0.50f;

	initVertex[28].texel[0] = 0.0f;
	initVertex[28].texel[1] = 0.75f;

	initVertex[29].texel[0] = 0.25f;
	initVertex[29].texel[1] = 0.75f;

	initVertex[30].texel[0] = 0.25f;
	initVertex[30].texel[1] = 0.50f;

	initVertex[31].texel[0] = 0.0f;
	initVertex[31].texel[1] = 0.75f;

	initVertex[32].texel[0] = 0.0f;
	initVertex[32].texel[1] = 0.50f;

	initVertex[33].texel[0] = 0.25f;
	initVertex[33].texel[1] = 0.50f;

	initVertex[34].texel[0] = 0.0f;
	initVertex[34].texel[1] = 0.50f;

	initVertex[35].texel[0] = 0.25f;
	initVertex[35].texel[1] = 0.25f;


	for (int i = 0; i < 36; i++)
	{
		vertex[i].coordinate[0] = initVertex[i].coordinate[0];
		vertex[i].coordinate[1] = initVertex[i].coordinate[1];
		vertex[i].coordinate[2] = initVertex[i].coordinate[2];

		vertex[i].texel[0] = initVertex[i].texel[0];
		vertex[i].texel[1] = initVertex[i].texel[1];
	}

}

