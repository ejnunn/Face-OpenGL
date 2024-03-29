// Assignment 6 (Exercise 15.5) - Texture-mapped mesh 
// Eric Nunn & Yvonne Rogell
// CPSC 5700, Seattle University, Fall Quarter 2019

// Include statements
#include <glad.h>
#include <glfw3.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <float.h>
#include <algorithm>
#include "GLXtras.h"
#include "VecMat.h"
#include "Camera.h"
#include "Draw.h"
#include <Misc.h>
#include <vector>


// GPU identifiers
GLuint vBuffer = 0;
GLuint program = 0;

// Window size and camera initilization
int winWidth = 500, winHeight = 500;

// Camera parameters: screenWidth, screenHeight, rotation, translation, FOV, nearDist, farDist, invVert
Camera camera(winWidth / 2, winHeight / 2, vec3(0, 0, 0), vec3(0, 0, -1), 10, 0.001f, 500, false);
bool shift = false;
float fieldOfView = 30, cubeSize = 0.05f, cubeStretch = cubeSize;

// Identify points on face
vec3 points[] = {
	// left face
	vec3(761, -268, 1225), // 0
	vec3(392, -429, 1167), // 1
	vec3(486, -629, 1281), // 2
	vec3(292, -726, 1084), // 3
	vec3(761, -669, 1344), // 4
	vec3(761, -735, 1359), // 5
	vec3(630, -710, 1331), // 6
	vec3(397, -727, 1233), // 7
	vec3(324, -812, 1142), // 8
	vec3(303, -846, 1060), // 9
	vec3(305, -934, 1070), // 10
	vec3(347, -918, 1124), // 11
	vec3(503, -823, 1254), // 12
	vec3(761, -885, 1312), // 13
	vec3(761, -951, 1335), // 14
	vec3(601, -1058, 1250), // 15
	vec3(394, -1044, 1210), // 16
	vec3(761, -1197, 1433), // 17
	vec3(242, -1002, 918), // 18
	vec3(315, -1179, 1100), // 19 
	vec3(761, -1364, 1316), // 20
	vec3(400, -1190, 1185), // 21 
	vec3(534, -1296, 1255), // 22
	vec3(761, -1434, 1318), // 23
	vec3(602, -1421, 1256), // 24 
	vec3(360, -1356, 1137), // 25
	vec3(329, -1374, 1023), // 26
	vec3(335, -1447, 826), // 27 
	vec3(360, -1479, 1042), // 28
	vec3(453, -1513, 1157), // 29
	vec3(761, -1552, 1287), // 30
	vec3(761, -1587, 1267), // 31
	vec3(761, -1804, 1279), // 32
	vec3(529, -1458, 1206), // 33
	vec3(622, -1563, 1235), // 34
	vec3(516, -1693, 1131), // 35
	vec3(410, -1594, 994), // 36
	vec3(494, -1710, 710), // 37
	vec3(492, -1816, 643), // 38
	vec3(761, -1979, 725), // 39
	vec3(761, -1819, 878), // 40
	vec3(545, -271, 1218), // 41
	vec3(402, -925, 1190), // 42
	vec3(506, -865, 1223), // 43
	vec3(554, -852, 1223), // 44
	vec3(583, -925, 1223), // 45
	vec3(641, -900, 1223), // 46
	vec3(450, -972, 1223), // 47
	vec3(535, -981, 1223), // 48
	vec3(599, -969, 1223), // 49
	vec3(629, -937, 1223), // 50
	vec3(670, -1001, 1263), // 51
	vec3(651, -1477, 1246), // 52
	vec3(761, -1488, 1276), // 53
	vec3(692, -1069, 1284), // 55
	vec3(761, -1070, 1376), // 56
	vec3(580, -1133, 1280), // 57
	vec3(606, -1206, 1273), // 58
	vec3(761, -1271, 1446), // 59
	vec3(761, -1329, 1316), // 60
};

// Triangles made up of points (counter clockwise)
int triangles[][3] = {
	{0, 1, 2}, // 0
	{1, 3, 2}, // 1 
	{0, 2, 4}, // 2 
	{3, 7, 2}, // 3 
	{7, 12, 6}, // 4 
	{2, 7, 6}, // 5 
	{2, 6, 4}, // 6 
	{4, 6, 5}, // 7 
	{5, 6, 13}, // 8 
	{6, 12, 13}, // 9 
	{3, 8, 7}, // 10 
	{7, 8, 12}, // 11 
	{3, 18, 9}, // 12 
	{9, 18, 10}, // 13 
	{3, 9, 8}, // 14 
	{9, 10, 8}, // 15 
	{10, 11, 8}, // 16 
	{8, 11, 12}, // 17 
	{18, 16, 10}, // 18
	{10, 16, 11}, // 19 
	{18, 19, 16}, // 20 
	{19, 21, 16}, // 21 
	{21, 22, 16}, // 22 
	{19, 26, 25}, // 23 
	{19, 25, 21}, // 24 
	{25, 22, 21}, // 25  
	{26, 27, 28}, // 26 
	{26, 28, 25}, // 27 
	{28, 29, 25}, // 28 
	{25, 29, 22}, // 29 
	{22, 29, 33}, // 30 
	{33, 24, 22}, // 31 
	{24, 23, 22}, // 32 
	{22, 23, 20}, // 33 
	{28, 36, 29}, // 34 
	{36, 35, 29}, // 35 
	{29, 35, 34}, // 36 
	{29, 34, 33}, // 37 
	{33, 34, 30}, // 38 
	{34, 31, 30}, // 39 
	{36, 37, 35}, // 40 
	{37, 38, 39}, // 41 
	{35, 32, 34}, // 42 
	{34, 32, 31}, // 43 
	{37, 40, 35}, // 44
	{37, 39, 40}, // 45 
	{35, 40, 32}, // 46 
	{41, 1, 0}, // 47 
	{11, 42, 12}, // 48 
	{11, 16, 42}, // 49 
	{42, 16, 47}, // 50 
	{42, 47, 43}, // 51 
	{42, 43, 12}, // 52 
	{43, 44, 12}, // 53 
	{47, 48, 43}, // 54 
	{43, 48, 45}, // 55 
	{48, 49, 45}, // 56 
	{49, 50, 45}, // 57 
	{45, 50, 46}, // 58 
	{44, 45, 46}, // 59 
	{46, 50, 14}, // 60 
	{49, 51, 50}, // 61 
	{50, 51, 14}, // 62 
	{47, 16, 48}, // 63 
	{16, 15, 48}, // 64 
	{48, 15, 49}, // 65 
	{49, 15, 51}, // 66 
	{33, 52, 24}, // 67 
	{33, 30, 52}, // 68 
	{52, 30, 53}, // 69 
	{52, 53, 23}, // 70 
	{24, 52, 23}, // 71 
	{16, 56, 15}, // 72 
	{15, 56, 54}, // 73 
	{15, 54, 51}, // 74 
	{51, 54, 14}, // 75 
	{54, 55, 14}, // 76 
	{54, 17, 55}, // 77 
	{16, 22, 56}, // 78 
	{22, 57, 56}, // 79 
	{56, 57, 54}, // 80 
	{22, 59, 57}, // 81 
	{57, 59, 58}, // 82 
	{57, 17, 54}, // 83 
	{57, 58, 17}, // 84 
	{22, 20, 59}, // 85 
	{12, 44, 13}, // 87 
	{44, 46, 13}, // 88 
	{46, 14, 13}, // 89 
	{44, 43, 45}, // 90 
	{18, 26, 19}, // 91 
	{18, 27, 26}, // 92 
	{27, 36, 28}, // 94 
	{27, 37, 36} // 95

};

// Vertex shader
const char* vertexShader = "\
	#version 130													\n\
	in vec3 point;													\n\
	in vec3 normal;													\n\
	in vec2 uv;														\n\
	uniform mat4 modelview;											\n\
	uniform mat4 persp;												\n\
	uniform mat4 textureTransform = mat4(1);						\n\
	out vec3 vPoint;												\n\
	out vec3 vNormal;												\n\
	out vec2 vuv;													\n\
	void main() {													\n\
		vPoint = (modelview*vec4(point, 1)).xyz;					\n\
		vNormal = (modelview*vec4(normal, 0)).xyz;					\n\
		vuv = (textureTransform * vec4(uv, 0, 1)).xy;				\n\
		gl_Position = persp * vec4(vPoint, 1);						\n\
	}";

// Pixel shader
const char* pixelShader = "\
	#version 130												\n\
	in vec3 vPoint;												\n\
	in vec3 vNormal;											\n\
	in vec2 vuv;												\n\
	uniform float a = 0.1f;										\n\
	uniform vec3 lightPos = vec3(-1, 0, -2);					\n\
	uniform vec3 color = vec3(1, 1, 1);							\n\
	uniform sampler2D textureImage;								\n\
	out vec4 pColor;											\n\
	void main() {												\n\
		vec3 N = normalize(vNormal);							\n\
		vec3 L = normalize(lightPos-vPoint);					\n\
		vec3 R = reflect(L, N);									\n\
		vec3 E = normalize(vPoint);								\n\
		float d = abs(dot(L, N));								\n\
		float h = max(0, dot(R, E));							\n\
		float s = pow(h, 100);									\n\
		float intensity = clamp(a+d+s, 0, 1);					\n\
		vec4 texColor = texture(textureImage, vuv);				\n\
		pColor = vec4(texColor.rgb, 1);	// but make opaque		\n\
	}";

// Global constant variables indicating size of points, number of points, triangles, vertices, 
// normals, and uv coordinates
// Declares arrays of normals, pointsWholeFace, trianglesWholeFaces and uvs
const int npoints = sizeof(points) / sizeof(points[0]);
const int ntriangles = sizeof(triangles) / sizeof(triangles[0]);
const int midline = 761;
vec3 normals[npoints * 2];
vec3 pointsWholeFace[npoints * 2];
int trianglesWholeFace[ntriangles * 2][3];
vec2 uvs[npoints * 2];
const int nvertices = ntriangles * 2 * 3;
const int sizePts = sizeof(pointsWholeFace);
const int sizeNms = sizeof(normals);
const int sizeUVs = sizeof(uvs);

// Constant holding file name.
const char* filename = "yvonne.tga";
int textureUnit = 0;
GLuint textureName;

// Function to display image on screen.
void Display(GLFWwindow* w) {
	// Clears the buffer
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// Set camera speed
	camera.SetSpeed(0.3f, 0.01f);

	// Set window size
	int screenWidth, screenHeight;
	glfwGetWindowSize(w, &screenWidth, &screenHeight);

	// Set scale 
	mat4 scale = Scale(cubeSize, cubeSize, cubeStretch);

	// Set texture transform
	float dx = 0, dy = -0.05, s = 1;
	mat4 t = Translate(dx, dy, 0) * Scale(s);

	// Clear to gray, use app's shader
	glClearColor(0.5, 0.5, 0.5, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(program);

	// Load texture
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, textureName);

	// Set vertex attribute pointers & uniforms
	VertexAttribPointer(program, "point", 3, 0, (void*)0);
	VertexAttribPointer(program, "normal", 3, 0, (void*)sizePts);
	VertexAttribPointer(program, "uv", 2, 0, (void*)(sizePts + sizeNms));
	SetUniform(program, "modelview", camera.modelview * scale);
	SetUniform(program, "persp", camera.persp);
	SetUniform(program, "textureImage", textureUnit);
	SetUniform(program, "textureTransform", t);

	// Draw shape
	glDrawElements(GL_TRIANGLES, nvertices, GL_UNSIGNED_INT, trianglesWholeFace);

	glFlush();
}

// Function to changing the points from pixel values to lie between +/- 1.  
void Normalize() {
	// Scale and offset so that points fall within +/-1 in x, y and z
	vec3 mn(FLT_MAX), mx(-FLT_MAX);
	for (int i = 0; i < npoints * 2; i++) {
		vec3 p = pointsWholeFace[i];
		for (int k = 0; k < 3; k++) {
			if (p[k] < mn[k]) mn[k] = p[k];
			if (p[k] > mx[k]) mx[k] = p[k];
		}
	}
	vec3 center = .5f * (mn + mx), range = mx - mn;
	float maxrange = std::max(range.x, std::max(range.y, range.z));
	float s = 2 / maxrange;
	for (int i = 0; i < npoints * 2; i++) {
		pointsWholeFace[i] = s * (pointsWholeFace[i] - center);
	}
}

// Method to reflect the points and triangles of the left side of the face, so that
// the whole face can be displayed
void Reflect() {
	// Compute new number of vertices and fill with existing vertices from left side of face
	for (int i = 0; i < npoints; ++i) {
		pointsWholeFace[i] = points[i];
	}

	// Compute new number of triangles and fill with existing triangles from left side of face
	for (int i = 0; i < ntriangles; ++i) {
		int* t = triangles[i];
		trianglesWholeFace[i][0] = t[0];
		trianglesWholeFace[i][1] = t[1];
		trianglesWholeFace[i][2] = t[2];
	}

	// Fill in second half of doubled array for pointsWholeFace. New x-coordinates are calculated using the
	// value of the midline x-coordinate
	for (int i = 0; i < npoints; ++i) {
		vec3 reflectedPoint(midline + (midline - points[i].x), points[i].y, points[i].z);
		pointsWholeFace[i + npoints] = reflectedPoint;
	}

	// Fill in second half of doubled triangle array
	for (int i = 0; i < ntriangles; ++i) {
		int* triangle = triangles[i];

		// Test each triangle corner for whether it is on(or close to) the mid - line
		// If on the midline, use the same vertex id as in the original triangle
		// If not on midline, use vertex id from second half of vertex array
		trianglesWholeFace[i + ntriangles][0] = pointsWholeFace[triangle[0]].x == midline ? triangle[0] : triangle[0] + npoints;
		trianglesWholeFace[i + ntriangles][1] = pointsWholeFace[triangle[1]].x == midline ? triangle[1] : triangle[1] + npoints;
		trianglesWholeFace[i + ntriangles][2] = pointsWholeFace[triangle[2]].x == midline ? triangle[2] : triangle[2] + npoints;

		// Reverse order of triangle to ensure they are still in counter clockwise order.
		int temp = trianglesWholeFace[i + ntriangles][0];
		trianglesWholeFace[i + ntriangles][0] = trianglesWholeFace[i + ntriangles][1];
		trianglesWholeFace[i + ntriangles][1] = temp;
	}
}

// Method to compute normals
void computeNormals() {
	// Allocate normals same size as doubled vertex array, and initialize all normals to (0,0,0)
	for (int i = 0; i < npoints * 2; ++i) {
		normals[i] = vec3(0, 0, 0);
	}

	// For each triangle, compute its surface normal, and add normal to each corresponding vertex normals array
	for (int i = 0; i < ntriangles * 2; ++i) {
		int* t = trianglesWholeFace[i];
		vec3 p1(pointsWholeFace[t[0]]), p2(pointsWholeFace[t[1]]), p3(pointsWholeFace[t[2]]);
		vec3 n = normalize(cross(p3 - p2, p2 - p1));
		for (int k = 0; k < 3; k++) {
			normals[t[k]] += n;
		}
	}

	// Set vertex normals to unit length
	for (int i = 0; i < npoints * 2; ++i) {
		normals[i] = normalize(normals[i]);
	}
}


// To dynamically resize the viewport when a user resizes the application window
void Resize(GLFWwindow* w, int width, int height) {
	camera.Resize(width, height);
	glViewport(0, 0, width, height);
}

// Mouse click event handler. Called when mouse button is pressed or released
void MouseButton(GLFWwindow* w, int butn, int action, int mods) {
	if (action == GLFW_PRESS) {
		double x, y;
		glfwGetCursorPos(w, &x, &y);
		camera.MouseDown((int)x, (int)y);
	}
	if (action == GLFW_RELEASE) {
		camera.MouseUp();
	}
}

// Mouse wheel event handler.
void MouseWheel(GLFWwindow* w, double xoffset, double direction) {
	camera.MouseWheel((int)direction, shift);
}

// Function called when mouse is held down and dragged.
void MouseMove(GLFWwindow* w, double x, double y) {
	if (glfwGetMouseButton(w, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		camera.MouseDrag((int)x, (int)y, shift);
	}
}

// Event handler for when different keys are pressed on the keyboard. 
void Key(GLFWwindow* w, int key, int scancode, int action, int mods) {
	shift = mods & GLFW_MOD_SHIFT;
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(w, GLFW_TRUE);
			break;
		case 'F':
			fieldOfView += shift ? -5 : 5;
			fieldOfView = fieldOfView < 5 ? 5 : fieldOfView > 150 ? 150 : fieldOfView;
			break;
		case 'S':
			cubeStretch *= shift ? .9f : 1.1f;
			cubeStretch = cubeStretch < .02f ? .02f : cubeStretch;
			break;
		}
	}
}

// Initializes the vertex buffer
void InitVertexBuffer() {

	Reflect(); // Reflect points to create a whole face
	Normalize(); // Set points to be within +/- 1.
	computeNormals(); // Compute normals 

	// Scale uv coordinates so that all points fall within [0,1]
	for (int i = 0; i < npoints * 2; ++i) {
		uvs[i] = vec2(pointsWholeFace[i].x * 0.5 + 0.5, pointsWholeFace[i].y * 0.5 + 0.5);
	}

	// Create GPU buffer, make it the active buffer
	glGenBuffers(1, &vBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
	// Allocate memory for vertex positions and normals
	glBufferData(GL_ARRAY_BUFFER, sizePts + sizeNms + sizeUVs, NULL, GL_STATIC_DRAW);
	// Copy data
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizePts, &pointsWholeFace[0]);
	glBufferSubData(GL_ARRAY_BUFFER, sizePts, sizeNms, &normals[0]);
	glBufferSubData(GL_ARRAY_BUFFER, sizePts + sizeNms, sizeUVs, &uvs[0]);
}

// Display a message if GFLW throws an error
void ErrorGFLW(int id, const char* reason) {
	printf("GFLW error %i: %s\n", id, reason);
}

void Close() {
	// unbind vertex buffer and free GPU memory
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &vBuffer);
}

// Main function runs when program is started.
int main() {
	glfwSetErrorCallback(ErrorGFLW);
	if (!glfwInit())
		return 1;
	int screenWidth = 1200;
	GLFWwindow* w = glfwCreateWindow(screenWidth, screenWidth, "Face", NULL, NULL);
	if (!w) {
		glfwTerminate();
		return 1;
	}
	glfwSetKeyCallback(w, Key);
	glfwSetScrollCallback(w, MouseWheel);
	glfwSetMouseButtonCallback(w, MouseButton);
	glfwSetCursorPosCallback(w, MouseMove);
	glfwMakeContextCurrent(w);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	printf("GL version: %s\n", glGetString(GL_VERSION));
	PrintGLErrors();
	program = LinkProgramViaCode(&vertexShader, &pixelShader);
	InitVertexBuffer();
	camera.SetSpeed(.01f, .001f); // **** otherwise, a bit twitchy
	glfwSetWindowSizeCallback(w, Resize); // ***** so can view larger window
	glfwSwapInterval(1);
	// Set texture unit and initialize texture name
	textureName = LoadTexture(filename, textureUnit); // in Misc.h
	while (!glfwWindowShouldClose(w)) {
		Display(w);
		glfwSwapBuffers(w);
		glfwPollEvents();
	}
	Close();
	glfwDestroyWindow(w);
	glfwTerminate();
}