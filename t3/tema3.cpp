#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <float.h>

#include "glut.h"

// dimensiunea ferestrei in pixeli
#define dim 300

unsigned char prevKey;

void Init(void)
{

  glClearColor(1.0, 1.0, 1.0, 1.0);

  glLineWidth(1);

  glPointSize(3);

  glPolygonMode(GL_FRONT, GL_LINE);
}

void Reshape(int w, int h)
{
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void KeyboardFunc(unsigned char key, int x, int y)
{
  prevKey = key;
  if (key == 27) // escape
    exit(0);
  glutPostRedisplay();
}

void MouseFunc(int button, int state, int x, int y)
{
}

class GrilaCarteziana
{
public:
  int lines;
  int columns;
  double xmin, xmax, ymin, ymax;

  GrilaCarteziana(int lines, int columns, double xmin, double ymin, double xmax, double ymax)
  {
    this->lines = lines;
    this->columns = columns;
    this->xmin = xmin;
    this->xmax = xmax;
    this->ymin = ymin;
    this->ymax = ymax;
    // determinam marimile celulelor in functie de marimea spatiului in care vrem sa randam grila si numarul de linii si coloane
    cellWidth = double(xmax - xmin) / double(columns);
    cellHeight = double(ymax - ymin) / double(lines);
  }

  void afiseazaGrila()
  {
    //desenam liniile
    for (double i = ymin; i <= ymax; i += cellHeight)
    {
      glColor3f(0, 0, 0);
      glBegin(GL_LINES);
      glVertex2d(xmin, i);
      glVertex2d(xmax, i);
      glEnd();
    }

    // desenam coloanele
    for (double j = xmin; j <= xmax; j += cellWidth)
    {
      glColor3f(0, 0, 0);
      glBegin(GL_LINES);
      glVertex2d(j, ymin);
      glVertex2d(j, ymax);
      glEnd();
    }
  }
  // https://stackoverflow.com/questions/22444450/drawing-circle-with-opengl
  void drawCircle(double cx, double cy, double radius, int circlePoints)
  {
    glColor3f(0, 0, 0);
    glBegin(GL_LINE_LOOP);
    const double PI = 3.1415926;
    for (int i = 0; i < circlePoints; i++)
    {
      // current angle
      double theta = (2.0 * PI * i) / double(circlePoints);
      double x = radius * cos(theta);
      double y = radius * sin(theta);
      glVertex2d(x + cx, y + cy);
      printf("%f %f %f %f\n", x, y, sqrt((x - cx) * (x - cx) + (y - cy) * (y - cy)));
    }
    glEnd();
  }

private:
  double cellWidth;
  double cellHeight;
};

// aici vom desena grila si liniile
void Display()
{
  glClear(GL_COLOR_BUFFER_BIT);
  GrilaCarteziana gc = GrilaCarteziana(5, 30, -1.0, -1.0, 1.0, 1.0);
  gc.afiseazaGrila();
  // gc.drawCircle(0.5, 0.5, 0.1, 100);
  glFlush();
}

int main(int argc, char **argv)
{
  glutInit(&argc, argv);

  glutInitWindowSize(dim, dim);

  glutInitWindowPosition(100, 100);

  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

  glutCreateWindow(argv[0]);

  Init();

  glutReshapeFunc(Reshape);

  glutKeyboardFunc(KeyboardFunc);

  glutMouseFunc(MouseFunc);

  glutDisplayFunc(Display);

  glutMainLoop();

  return 0;
}
