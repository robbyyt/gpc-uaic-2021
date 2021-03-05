#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "glut.h"

// dimensiunea ferestrei in pixeli
#define dim 300

unsigned char prevKey;

// concoida lui Nicomede (concoida dreptei)
// $x = a + b \cdot cos(t), y = a \cdot tg(t) + b \cdot sin(t)$. sau
// $x = a - b \cdot cos(t), y = a \cdot tg(t) - b \cdot sin(t)$. unde
// $t \in (-\pi / 2, \pi / 2)$
void Display1() {
    double xmax, ymax, xmin, ymin;
    double a = 1, b = 2;
    double pi = 4 * atan(1.0);
    double ratia = 0.05;
    double t;

    // calculul valorilor maxime/minime ptr. x si y
    // aceste valori vor fi folosite ulterior la scalare
    xmax = a - b - 1;
    xmin = a + b + 1;
    ymax = ymin = 0;
    for (t = -pi / 2 + ratia; t < pi / 2; t += ratia) {
        double x1, y1, x2, y2;
        x1 = a + b * cos(t);
        xmax = (xmax < x1) ? x1 : xmax;
        xmin = (xmin > x1) ? x1 : xmin;

        x2 = a - b * cos(t);
        xmax = (xmax < x2) ? x2 : xmax;
        xmin = (xmin > x2) ? x2 : xmin;

        y1 = a * tan(t) + b * sin(t);
        ymax = (ymax < y1) ? y1 : ymax;
        ymin = (ymin > y1) ? y1 : ymin;

        y2 = a * tan(t) - b * sin(t);
        ymax = (ymax < y2) ? y2 : ymax;
        ymin = (ymin > y2) ? y2 : ymin;
    }

    xmax = (fabs(xmax) > fabs(xmin)) ? fabs(xmax) : fabs(xmin);
    ymax = (fabs(ymax) > fabs(ymin)) ? fabs(ymax) : fabs(ymin);

    // afisarea punctelor propriu-zise precedata de scalare
    glColor3f(1, 0.1, 0.1); // rosu
    glBegin(GL_LINE_STRIP);
    for (t = -pi / 2 + ratia; t < pi / 2; t += ratia) {
        double x1, y1, x2, y2;
        x1 = (a + b * cos(t)) / xmax;
        x2 = (a - b * cos(t)) / xmax;
        y1 = (a * tan(t) + b * sin(t)) / ymax;
        y2 = (a * tan(t) - b * sin(t)) / ymax;

        glVertex2f(x1, y1);
    }
    glEnd();

    glBegin(GL_LINE_STRIP);
    for (t = -pi / 2 + ratia; t < pi / 2; t += ratia) {
        double x1, y1, x2, y2;
        x1 = (a + b * cos(t)) / xmax;
        x2 = (a - b * cos(t)) / xmax;
        y1 = (a * tan(t) + b * sin(t)) / ymax;
        y2 = (a * tan(t) - b * sin(t)) / ymax;

        glVertex2f(x2, y2);
    }
    glEnd();
}

// graficul functiei 
// $f(x) = \bar sin(x) \bar \cdot e^{-sin(x)}, x \in \langle 0, 8 \cdot \pi \rangle$, 
void Display2() {
    double pi = 4 * atan(1.0);
    double xmax = 8 * pi;
    double ymax = exp(1.1);
    double ratia = 0.05;

    // afisarea punctelor propriu-zise precedata de scalare
    glColor3f(1, 0.1, 0.1); // rosu
    glBegin(GL_LINE_STRIP);
    for (double x = 0; x < xmax; x += ratia) {
        double x1, y1;
        x1 = x / xmax;
        y1 = (fabs(sin(x)) * exp(-sin(x))) / ymax;

        glVertex2f(x1, y1);
    }
    glEnd();
}
float distance(float x) {
    float d = 0.0;
    if (x - int(x) > (int(x) + 1) - x) {
        d = (int(x) + 1) - x;
    }
    else
    {
        d = x - int(x);
    }

    return d;

}
//f(x) = 1 if x = 0 else d(x) / x
void Display3() {
    double xmax = 30;
    double ymax = 1.0;
	// la sugestia domnului profesor, intre 1 si 5%
    double ratia = 0.01;

    // afisarea punctelor propriu-zise precedata de scalare
    glColor3f(1, 0.1, 0.1); // rosu
    glBegin(GL_LINE_STRIP);
    for (double x = 0; x < xmax; x += ratia) {
        double x1, y1;
        x1 = x/xmax;
		if (x == 0) {
			y1 = 1;
		}
		else {
			y1 = (fabs(distance(x) / x)) / ymax;
		}

        if (x1 > 0.8)
            x1 = 0.8;
        glVertex2f(x1, y1);
    }
    glEnd();
}

// melcul lui pascal
void Display4() {
    double a = 0.3, b = 0.2;
    double xmax, ymax, xmin, ymin;
    double pi = 4 * atan(1.0);
    double ratia = 0.05;
    double t;
    xmax = 1.2;
    xmin = 1.1;
    ymax = ymin = 1.2;
     
    xmax = (fabs(xmax) > fabs(xmin)) ? fabs(xmax) : fabs(xmin);
    ymax = (fabs(ymax) > fabs(ymin)) ? fabs(ymax) : fabs(ymin);
    glColor3f(1, 0.1, 0.1); // rosu
    glBegin(GL_LINE_STRIP);
    for (t = -pi + ratia; t < pi; t += ratia) {
        double x1, y1;
        x1 = 2 * (a * cos(t) + b) * cos(t) / xmax;
        y1 = 2 * (a * cos(t) + b) * sin(t) / ymax;
        
        glVertex2f(x1, y1);
    }
    glEnd();
}

// trisectoarea lui longchamps
void Display5() {
	const auto ratio = 0.005;
	const auto a = 0.2;
	double ys[250], xs[250];
	double pi = 4 * atan(1.0);
	auto t = -pi / 2 + ratio;
	auto n = 0;

	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_STRIP);
	while (true) {

		if (t >= -pi / 6) break;

		const auto output_x = a / (4 * cos(t) * cos(t) - 3);
		const auto output_y = (a * tan(t)) / (4 * cos(t) * cos(t) - 3);

		xs[n] = output_x;
		ys[n] = output_y;
		n++;

		t += ratio;

		glVertex2f(output_x, output_y);
	}
	glEnd();

	glColor3f(1, 0.1, 0.1);
	glBegin(GL_TRIANGLES);
	for (auto i = 1; i < n - 1; i += 3) {

		if (i > n / 4 && i < 3 * n / 4) continue;

		glVertex2f(-1.0, 1.0);
		glVertex2f(xs[i], ys[i]);
		glVertex2f(xs[i + 1], ys[i + 1]);
	}
	glEnd();
}

// cicloida
void Display6() {
    double a = 0.1, b = 0.2;
    double xmax, ymax, xmin, ymin;
    double ratia = 0.05;
    double t;
    xmax = 1.2;
    xmin = 1;
    ymax = ymin = 1;
    xmax = (fabs(xmax) > fabs(xmin)) ? fabs(xmax) : fabs(xmin);
    ymax = (fabs(ymax) > fabs(ymin)) ? fabs(ymax) : fabs(ymin);
    glColor3f(1, 0.1, 0.1); // rosu
    glBegin(GL_LINE_STRIP);
	// am ales acest interval ca sa nu fie prea lunga (sa fie exact ca in imagine)
    for (t = -10; t < 10; t += ratia) {
        double x1, y1;
        x1 = (a * t - b * sin(t)) / xmax;
        y1 = (a - b * cos(t)) / ymax;

        glVertex2f(x1, y1);
    }
    glEnd();
}

//epicicloida
void Display7(){
    double R = 0.1, r = 0.3;
    double pi = 4 * atan(1.0);
    double ratia = 0.01;
    double t;
    glColor3f(1, 0.1, 0.1); // rosu
    glBegin(GL_LINE_STRIP);
    for (t = 0; t < 2*pi; t += ratia) {
        double x1, y1;
        x1 = (R + r) * cos((r / R) * t) - r * cos(t + (r / R) * t);
        y1 = (R + r) * sin((r / R) * t) - r * sin(t + (r / R) * t);

        glVertex2f(x1, y1);
    }
    glEnd();
}

//hipocicloida
void Display8() {
    double R = 0.1, r = 0.3;
    double pi = 4 * atan(1.0);
    double ratia = 0.01;
    double t;
    glColor3f(1, 0.1, 0.1); // rosu
    glBegin(GL_LINE_STRIP);
    for (t = 0; t < 2 * pi; t += ratia) {
        double x1, y1;
        x1 = (R - r) * cos((r / R) * t) - r * cos(t - (r / R) * t);
        y1 = (R - r) * sin((r / R) * t) - r * sin(t - (r / R) * t);

        glVertex2f(x1, y1);
    }
    glEnd();

}

//lemniscata lui bernouli
void Display9() {
    double a=0.4;
    double pi = 4 * atan(1.0);
    double ratia = 0.01;
    double t;
    double x1, y1, x2, y2;
    glColor3f(1, 0.1, 0.1); // rosu
    glBegin(GL_LINE_STRIP);
    for (t = -pi / 4 + ratia; t < pi / 4; t += ratia) {
        double r, x, y;
        r = a * sqrt(2 * cos(2 * t));
        x = r * cos(t);
        y = r * sin(t);
        if (t == -pi / 4 + ratia) {
            x1 = x;
            y1 = y;
        }
        glVertex2f(x, y);
    }
    glEnd();
    glColor3f(1, 0.1, 0.1); // rosu
    glBegin(GL_LINE_STRIP);
    for (t = -pi / 4 + ratia; t < pi / 4; t += ratia) {
        double r,x,y;
        r = -(a * sqrt(2 * cos(2 * t)));
        x = r * cos(t);
        y= r * sin(t);
        if (t == -pi / 4 + ratia) {
            x2 = x;
            y2 = y;
        }
        glVertex2f(x, y);
    }
    glEnd();
    glColor3f(1, 0.1, 0.1); // rosu
    glBegin(GL_LINE_STRIP);
    glVertex2f(x2, y2);
    glVertex2f(x1, y1);
    glEnd();
}

//spirala logaritmica
void Display10() {
    double a = 0.02;
    double ratia = 0.05;
    double t;
    glColor3f(1, 0.1, 0.1); // rosu
    glBegin(GL_LINE_STRIP);
    for (t = 0; t <100; t += ratia) {
        double r, x, y;
        r = a * exp(1 + t);
        x = r * cos(t);
        y = r * sin(t);
		glVertex2f(x, y);
    }
    glEnd();
}

void Init(void) {

    glClearColor(1.0, 1.0, 1.0, 1.0);

    glLineWidth(1);

    //   glPointSize(4);

    glPolygonMode(GL_FRONT, GL_LINE);
}

void Display(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    switch (prevKey) {
    case '1':
        Display3();
        break;
    case '2':
        Display4();
        break;
    case '3':
        Display5();
        break;
    case '4':
        Display6();
        break;
    case '6':
        Display7();
        break;
    case '7':
        Display8();
        break;
    case '8':
        Display9();
        break;
    case '9':
        Display10();
        break;
    default:
        break;
    }

    glFlush();
}

void Reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void KeyboardFunc(unsigned char key, int x, int y) {
    prevKey = key;
    if (key == 27) // escape
        exit(0);
    glutPostRedisplay();
}

void MouseFunc(int button, int state, int x, int y) {
}

int main(int argc, char** argv) {

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