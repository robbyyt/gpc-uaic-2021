#define _CRT_SECURE_NO_WARNINGS
# define M_PI 3.14159265358979323846
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <float.h>

#include "glut.h"
#include <vector>
#include<iostream>
using namespace std;
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
    double radius = 0.04;
    double xmin, xmax, ymin, ymax, e, dc, dl;


    GrilaCarteziana(int lines, int columns, double xmin, double ymin, double xmax, double ymax)
    {
        this->lines = lines;
        this->columns = columns;
        this->xmin = xmin;
        this->xmax = xmax;
        this->ymin = ymin;
        this->ymax = ymax;

        e = 0.001;
        dc = (2 - 2 * e) / (double)columns;
        dl = (2 - 2 * e) / (double)lines;
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
    void drawCircle(double cx, double cy, double radius, int circlePoints=100)
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
    //o alta varianta de a desena un cerc doar ca asta vine gata colorat
    void drawCircleMethod2(float r, float x, float y) {
        float i = 0.0f;
        glColor3f(0.184314, 0.309804, 0.309804);
        glBegin(GL_TRIANGLE_FAN);

        glVertex2f(x, y); // Center
        for (i = 0.0f; i <= 360; i++)
            glVertex2f(r * cos(M_PI * i / 180.0) + x, r * sin(M_PI * i / 180.0) + y);

        glEnd();
    }
    void aprindePixel(int coord_x, int coord_y, int w ) {
        for (auto i = -w / 2; i <= w / 2; i++) {
            drawCircleMethod2(this->radius, coord_x * dl, (coord_y+i) * dc);
        }
      
    }
    
    std::vector<std::pair<int, int>> vertexes_calculator(float x0, float y0, float xn, float yn, int index) {
        std::vector<std::pair<int, int>> vertexes;
        float dx = fabs(xn - x0);
        float dy = fabs(yn - y0);
        int x = x0, y = y0;
        vertexes.push_back({ (x0 - x > 0.5 ? x + 1 : x), (y0 - y > 0.5 ? y + 1 : y) });
        int delta_x = (x < xn ? 1 : -1);
        int delta_y = (y < yn ? 1 : -1);
        int d = 2.0f * dy - dx;
        int dE = 2.0f * dy;
        int dNE = 2.0f * (dy - dx);
        while (x < xn)
        {
            if (d <= 0)
            {
                d += dE;
                x++;
            }
            else
            {
                d += dNE;
                y = y + index;
                x++;
            }
            vertexes.push_back({ x, y });
        }
        return vertexes;
    }

    std::vector<std::pair<int, int>> AfisareSegmentDreapta3(float x0, float y0, float xn, float yn)
    {
        std::vector<std::pair<int, int>> vertexes;
        int index=0;
        if (x0 < xn && y0 < yn) {
            index = 1;
            vertexes = vertexes_calculator(x0, y0, xn, yn, index);
        }
        else {
            if (x0 < xn && y0 > yn) {
                index = -1;
                vertexes = vertexes_calculator(x0, y0, xn, yn, index);
            }
        }

        return vertexes;
    }
  
  
    void trasareSegment(float x0, float y0, float xn, float yn, int width) {
        for (auto& points : AfisareSegmentDreapta3(x0, y0, xn, yn)) {
            aprindePixel(points.first,points.second,width);
        }
        glColor3f(1, 0.1, 0.1);
        glBegin(GL_LINES);
        glVertex2d(x0 * dl, y0 * dc);
        glVertex2d(xn * dl, yn * dc);
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
    GrilaCarteziana gc = GrilaCarteziana(16, 16, -1.0, -1.0, 1.0, 1.0);
    gc.afiseazaGrila();
    //gc.drawCircleMethod2(0.05, 0.5, 0.47);
    gc.trasareSegment(-9, -8, 8, 0, 1);
    gc.trasareSegment(-8, 8, 8, 2.4, 3);
    glFlush();
}

int main(int argc, char** argv)
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
