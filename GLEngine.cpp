#include "globals.h"



static PlotPoint cube[] = 
	{
		PlotPoint(-1.0f,1.0f,-1.0f), //A
		PlotPoint(1.0f,1.0f,-1.0f), //B
		PlotPoint(1.0f,-1.0f,-1.0f), //C
		PlotPoint(-1.0f,-1.0f,-1.0f), //D

		PlotPoint(-1.0f,1.0f,1.0f), //E
		PlotPoint(1.0f,1.0f,1.0f), //F
		PlotPoint(1.0f,-1.0f,1.0f), //G
		PlotPoint(-1.0f,-1.0f,1.0f),//H

		PlotPoint(-1.0f,1.0f,-1.0f), //A
		PlotPoint(1.0f,1.0f,-1.0f), //B
		PlotPoint(1.0f,1.0f,1.0f), //F
		PlotPoint(-1.0f,1.0f,1.0f), //E

		PlotPoint(-1.0f,1.0f,-1.0f), //A
		PlotPoint(-1.0f,1.0f,1.0f), //E
		PlotPoint(-1.0f,-1.0f,1.0f),//H
		PlotPoint(-1.0f,-1.0f,-1.0f), //D

		PlotPoint(1.0f,1.0f,-1.0f), //B
		PlotPoint(1.0f,1.0f,1.0f), //F
		PlotPoint(1.0f,-1.0f,1.0f), //G
		PlotPoint(1.0f,-1.0f,-1.0f), //C

		PlotPoint(-1.0f,-1.0f,-1.0f), //D
		PlotPoint(1.0f,-1.0f,-1.0f), //C
		PlotPoint(1.0f,-1.0f,1.0f), //G
		PlotPoint(-1.0f,-1.0f,1.0f)//H
	};

GLEngine::GLEngine(void)
{
	pmx = 0;
	pmy = 0;
	windowposx = 0;
	windowposy = 0;
	windowposz = 0;
	windowrotx = 0;
	windowroty = 0;
	fullscreen = false;
}

void GLEngine::ResetGLScene(void)
{
    nmx = 0;
    nmy = 0;
    zDelta = 0;
    pmx = 0;
    pmy = 0;
    windowposx = 0;
    windowposy = 0;
    windowposz = 0;
    windowrotx = 0;
    windowroty = 0;
}

GLEngine::~GLEngine(void)
{
}

void GLEngine::BeginRender()
{
    //need to select the window here
    glutSetWindow(window);
    Clear();
    TranslateP();
}

void GLEngine::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();// Reset The Current Modelview Matrix
}

void GLEngine::SwapBuffer()
{									
	glutSwapBuffers(); //swap the buffers of GLWindow
}

void GLEngine::ReSizeGLScene(int width, int height)		// Resize And Initialize The GL Window
{
	
	sc_width = width;
	sc_height = height;

	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)sc_width/(GLfloat)sc_height,0.1f,300.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}


void GLEngine::TranslateP()
{
	glTranslatef(0,0,windowposz-10.0f);
	glRotatef(windowrotx,0,1.0f,0);
	glRotatef(windowroty,1.0f,0,0);
	glTranslatef(-windowposx,-windowposy,0);
}

int GLEngine::InitGL()										// All Setup For OpenGL Goes Here
{

	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glEnable(GL_MAP1_VERTEX_3);
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	BuildFont();
	return 0;										// Initialization Went OK
}

void GLEngine::BuildFont()								// Build Our Bitmap Font
{
//	HFONT	font;										// Windows Font ID
//	HFONT	oldfont;									// Used For Good House Keeping
//
//	base = glGenLists(96);								// Storage For 96 Characters
//
//	font = CreateFont(	20,							// Height Of Font
//						0,								// Width Of Font
//						0,								// Angle Of Escapement
//						0,								// Orientation Angle
//						0,						// Font Weight
//						FALSE,							// Italic
//						FALSE,							// Underline
//						FALSE,							// Strikeout
//						ANSI_CHARSET,					// Character Set Identifier
//						OUT_TT_PRECIS,					// Output Precision
//						CLIP_DEFAULT_PRECIS,			// Clipping Precision
//						ANTIALIASED_QUALITY,			// Output Quality
//						FF_DONTCARE|DEFAULT_PITCH,		// Family And Pitch
//						L"Sylfaen");					// Font Name
//
//	oldfont = (HFONT)SelectObject(hDC, font);           // Selects The Font We Want
//	wglUseFontBitmaps(hDC, 32, 96, base);				// Builds 96 Characters Starting At Character 32
//	SelectObject(hDC, oldfont);							// Selects The Font We Want
//	DeleteObject(font);									// Delete The Font
}


void GLEngine::KillFont()									// Delete The Font List
{
//	glDeleteLists(base, 96);							// Delete All 96 Characters
}

void GLEngine::glPrint(const char *fmt, ...)					// Custom GL "Print" Routine
{
//	char		text[256];								// Holds Our String
//	va_list		ap;										// Pointer To List Of Arguments
//
//	if (fmt == NULL)									// If There's No Text
//		return;											// Do Nothing
//
//	va_start(ap, fmt);									// Parses The String For Variables
//	    vsprintf_s(text, fmt, ap);						// And Converts Symbols To Actual Numbers
//	va_end(ap);											// Results Are Stored In Text
//
//	glPushAttrib(GL_LIST_BIT);							// Pushes The Display List Bits
//	glListBase(base - 32);								// Sets The Base Character to 32
//	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);	// Draws The Display List Text
//	glPopAttrib();										// Pops The Display List Bits
}
void GLEngine::DrawText(float x,float y,float r,float g, float b, char text[])
{
	glLoadIdentity();
	glColor3f(r,g,b);
	glTranslatef(0,0,-0.1f);
	glRasterPos2f(x,y);

	
	glPrint(text);
}

int GLEngine::DrawLine(float x1, float y1, float z1, float x2, float y2, float z2, float r,float g, float b)
{
	glColor3f(r,g,b);
	glBegin(GL_LINES);
		glVertex3f(x1,y1,z1);
		glVertex3f(x2,y2,z2);
	glEnd();
	return 0;
}

void GLEngine::DrawCurve(GLfloat *ctrlpoints,int stride,int order, float r,float g,float b)
{

	glColor3f(r,g,b);
	glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, stride, order,ctrlpoints);

	glBegin(GL_LINE_STRIP);
        for (int i = 0; i <= 30; i++) 
            glEvalCoord1f((GLfloat) i/30.0);
    glEnd();
}


int GLEngine::DrawPoint(float x, float y, float z, float r,float g,float b, float point_size)	
{
	glLoadIdentity();
	TranslateP();
	glPointSize(point_size);
	glColor3f(r,g,b);
	glBegin(GL_POINTS);
		glVertex3f(x,y,z);
	glEnd();
	return 0;
}

int GLEngine::DrawCube(float posx,float posy,float posz,float edge,float r,float g,float b)
{
	float q = edge/2;
	glColor3f(r,g,b);
	glBegin(GL_QUADS);
		for(int x = 0; x<24; x++)
			glVertex3f(cube[x].x*q+posx, cube[x].y*q+posy, cube[x].z*q+posz);
	glEnd();

	return 0;
}

int GLEngine::DrawFilledSquare(float x, float y, float z,float r,float g,float b, float edge)
{
	glColor3f(r,g,b);
	glBegin(GL_QUADS);
		glVertex3f(x-edge, y+edge, 0.0f);
		glVertex3f(x+edge, y+edge, 0.0f);
		glVertex3f(x+edge,y-edge, 0.0f);
		glVertex3f(x-edge,y-edge, 0.0f);
	glEnd();
	return 0;
}

void GLEngine::KillGLWindow()								// Properly Kill The Window
{
    glutDestroyWindow(window);
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
 *	title			- Title To Appear At The Top Of The Window				*
 *	width			- Width Of The GL Window Or Fullscreen Mode				*
 *	height			- Height Of The GL Window Or Fullscreen Mode			*
 *	bits			- Number Of Bits To Use For Color (8/16/24/32)			*
 *	fullscreenflag	- Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE)	*/
 
int GLEngine::CreateGLWindow(char * title, int width, int height)
{
  /* Open a window */
  window = glutCreateWindow(title);

  glutSetWindow(window);

  

  /* Initialize our window. */
  InitGL();


	if (fullscreen)	
	{
            /* Go fullscreen.  This is as soon as possible. */
            glutFullScreen();

	}
	else
	{
	
	}

	return 0;
}

/* The function called whenever a key is pressed. */
void GLEngine::KeyPressed(unsigned char key, int x, int y)
{
    /* avoid thrashing this procedure */
    usleep(100);

    /* If escape is pressed, kill everything. */
    switch(key)
    {
        case GLUT_KEY_F1:
            fullscreen = !fullscreen;
            if(fullscreen)
            {
                glutFullScreen();
            }
            break;
        case GLUT_KEY_F6:
            ResetGLScene();
            break;
        case GLUT_KEY_UP:
            windowposy += 1;
            break;
        case GLUT_KEY_DOWN:
            windowposy -= 1;
            break;
        case GLUT_KEY_LEFT:
            windowposx -= 1;
            break;
        case GLUT_KEY_RIGHT:
            windowposx += 1;
            break;
    }



}


GLfloat GLEngine::CalcPan(int oldx, int newx)
{
	int d = newx - oldx;

	return d / 100.0f;
}

GLfloat GLEngine::CalcRot(int oldx, int newx)
{
	int d = newx - oldx;

	return d / 5.0f;
}

GLfloat GLEngine::CalcZoom(int zDelta)
{
	return zDelta / 100.0f;
}



