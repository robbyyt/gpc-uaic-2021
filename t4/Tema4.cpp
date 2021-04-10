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
#include <fstream>
#include <algorithm> 
using namespace std;
// dimensiunea ferestrei in pixeli
#define dim 300
#define DOM_SCAN 100

unsigned char prevKey;

struct S2 {
	pair<int, int> p0;
	pair<int, int> p1;
};

struct S3 {
	int ymax;
	double xmin;
	double ratia;
};

bool comparatorS3(S3& i, S3& j) {
	return i.xmin < j.xmin;
}

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
    int radius_c = 13;
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
    void drawCircle(double cx, double cy, double radius, int circlePoints = 100)
    {
        glPolygonMode(GL_FRONT, GL_FILL);
        glColor3f(0, 0, 0);
        glBegin(GL_POLYGON);
        const double PI = 3.1415926;
        for (int i = 0; i < circlePoints; i++)
        {
            // current angle
            double theta = (2.0 * PI * i) / double(circlePoints);
            double x = radius * cos(theta);
            double y = radius * sin(theta);
            glVertex2d(x + cx, y + cy);
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

    vector<pair<int, int>>AfisarePuncteCerc3(int x, int y, vector<pair<int, int>>M) {
        //pentru generarea pixelilor din cadranul I. Nu este nevoie de simetrie in acest caz deci este suficient sa adaugam in M doar punctele (y,x).
        if (x != y) {
            M.push_back({ (y),(x) });        
        }
        return M;
    }
    
    void aprindePixel(int coord_x, int coord_y, int w) {
        //se deschid 3 pixeli, pe linie de data, asta in functie de pixelul central aflat in M 
        for (auto i = -w / 2; i <= w / 2; i++) {
            drawCircle((xmin + (coord_x+i) * cellWidth), -1 + (coord_y) * cellHeight, this->radius);
        }
        

    }
	
    void traseazaCerc(int width) {
        glLineWidth(5); //ingroasa linia
        glColor3f(1, 0.1, 0.1);

        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < 100; i++)
        {
            float theta = 2.0f * 3.1415926f * float(i) / float(100);
            float x = radius_c * cosf(theta) * cellHeight; //ajustam cercul in functie de inaltimea unei celule
            float y = radius_c * sinf(theta) * cellWidth; // ajustam cercul in functie de lungimea unei celule

            //plecam cu centrul cercului din coltul stanga jos
            glVertex2f(x + (-1), y + (-1));

        }
        glEnd();
		glLineWidth(1);

        for (auto& points : AfisareCerc4(this->radius_c)) {
            aprindePixel(points.first, points.second, width);
        }
    }

	vector<pair<int, int>> AfisareCerc4(int R) {
		int x = 0, y = R;
		int d = 1 - R;
		int dE = 3, dSE = -2 * R + 5;
		vector<pair<int, int>> M;

		// adaugam in M punctele de start
		M = AfisarePuncteCerc3(x, y, M);

		while (y > x)
		{
			if (d < 0) {
				d += dE;
				dE += 2;
				dSE += 2;
			}
			else {
				d += dSE;
				dE += 2;
				dSE += 4;
				y--;
			}
			x++;

			// se calculeaza noul punt (noul pixel) si se adauga coordonatele in M
			M = AfisarePuncteCerc3(x, y, M);
		}
		return M;
	}

	void traseazaPoligon(vector<pair<int, int> > points) {
		glLineWidth(2);
		glPolygonMode(GL_FRONT, GL_LINE);
		glBegin(GL_POLYGON);
		glColor3f(1, 0.1, 0.1);
		int ym = columns + 1;
		int yM = -1;
		for (auto d : points)
		{
			if (d.second < ym) {
				ym = d.second;
			}

			if (d.second > yM) {
				yM = d.second;
			}
			float x = xmin + cellWidth * d.first;
			float y = ymin + cellHeight * d.second;
			glVertex2f(x, y);
		}
		glEnd();
		glLineWidth(1);

		vector<S2> P = construirePoligon(points);

		vector<pair<int, int>> to_draw;
		for (int y = ym; y <= yM; y++) {
			vector<double> current_et = initializareETv2(P, y, ym, yM);
			if (current_et.empty()) {
				continue;
			}
			for (int i = 0; i < current_et.size() - 1; i+= 2) {
				int x0 = ceil(current_et[i]);
				int x1 = floor(current_et[i + 1]);
				if (x1 == current_et[i + 1] && x1 > x0) {
					x1-= 1;
				}

				for (int j = min(x0, x1); j <= max(x0, x1); j++) {
					pair<int, int> point;
					point.first = j;
					point.second = y;
					to_draw.push_back(point);
				}
			}

		}

		for (auto p : to_draw) {
			aprindePixel(p.first, p.second, 1);
		}
		//vector<vector<S3>> et, ssms;
		//initializareET(P, et);
		//calculssm(P, et, ssms);
		//colorarePoligon(ssms);


	}
	
	vector<S2> construirePoligon(vector<pair<int, int> > points) {
		vector<S2> P;
		for (size_t i = 0; i < points.size() - 1; ++i)
		{
			S2 m;
			m.p0 = points[i];
			m.p1 = points[i + 1];
			P.push_back(m);
		}
		S2 last_m;
		last_m.p0 = points[points.size() - 1];
		last_m.p1 = points[0];
		P.push_back(last_m);
		return P;
	}

	vector<double> initializareETv2(vector<S2> P, int y, int ym, int yM) {
		// pentru o dreapta de scanare y pentru fiecare muchie din p vedem intersectiile
		// ne intereseaza doar valorile x pentru ca y e setat.
		vector<double> et;
		for (auto muchie : P) {
			// vedem daca e posibil sa intersecteze muchia
			if ((muchie.p0.second <= y && y <= muchie.p1.second) || (muchie.p0.second >= y && y >= muchie.p1.second)) {
				double intersectie;
				
				if (muchie.p0.first == muchie.p1.first) {
					intersectie = muchie.p0.first;
				}
				else {
					intersectie = muchie.p0.first + (double)(y - muchie.p0.second) / (double)(muchie.p1.second - muchie.p0.second) * (double)(muchie.p1.first - muchie.p0.first);
				}
				// pusham doar puncte interioare
				if (muchie.p1.second == y && intersectie == muchie.p1.first || 
					muchie.p0.second == y && intersectie == muchie.p0.first) {
					if (y == min(muchie.p0.second, muchie.p1.second)) {
						et.push_back(intersectie);
					}
				}
				else {
					et.push_back(intersectie);
				}
			}
		}
		sort(et.begin(), et.end());
		return et;
	}

	//void initializareET(vector<S2> P, vector<vector<S3>> &et) {
	//	int xm, ym, xM, yM;

	//	for (int i = 0; i <= DOM_SCAN; i++) {
	//		vector<S3> v;
	//		v.clear();
	//		et.push_back(v);
	//	}

	//	for (auto m : P) {
	//		ym = min(m.p0.second, m.p1.second);
	//		yM = max(m.p0.second, m.p1.second);
	//		xm = (ym == m.p0.second) ? m.p0.first : m.p1.first;
	//		xM = (yM == m.p0.second) ? m.p0.first : m.p1.first;
	//		S3 el;
	//		el.ymax = yM;
	//		el.xmin = xm;
	//		el.ratia = ((double)(xm - xM)) / (double)(ym - yM);
	//		et[ym].push_back(el);
	//	}

	//	// Sortare
	//	for(int i = 0; i <= DOM_SCAN; i++) {
	//		if (et[i].size() > 0) {
	//			sort(et[i].begin(), et[i].end(), comparatorS3);
	//		}
	//	}
	//}

	//void calculssm(vector<S2> P, vector<vector<S3>> &et, vector<vector<S3>> &finalET) {
	//	int y, k;
	//	vector<S3> activeSSM;
	//	for (int i = 0; i <= DOM_SCAN; i++) {
	//		vector<S3> v;
	//		v.clear();
	//		finalET.push_back(v);
	//	}
	//	
	//	y = -1;

	//	for (int i = 0; i <= DOM_SCAN; i++) {
	//		if (!et[i].empty()) {
	//			y = i;
	//			break;
	//		}
	//	}

	//	if (y == -1) {
	//		return;
	//	}
	//	do {
	//		activeSSM.insert(activeSSM.end(), et[y].begin(), et[y].end());
	//		for (auto iter = activeSSM.begin(); iter != activeSSM.end(); ) {
	//			if (iter->ymax == y) {
	//				iter = activeSSM.erase(iter);
	//			}
	//			else {
	//				++iter;
	//			}
	//		}

	//		sort(activeSSM.begin(), activeSSM.end(), comparatorS3);

	//		finalET[y] = activeSSM;
	//		y++;

	//		for (auto element : activeSSM) {
	//			if (element.ratia != 0) {
	//				element.xmin += element.ratia;
	//			}
	//		}


	//	} while (!activeSSM.empty() || !et[y].empty());
	//}

	//void colorarePoligon(vector<vector<S3>> ssms) {
	//	for (auto v : ssms) {
	//		for (auto s3 : v) {
	//			cout << s3.xmin << " " << s3.ymax<<endl;
	//		}
	//		cout << "-------------" << endl;
	//	}
	//}
    

private:
    double cellWidth;
    double cellHeight;
};

vector<pair<int, int> > readPolygonPointsFromFile() {
	ifstream infile;
	infile.open("input.txt");
	if (infile.is_open())
	{
		cout << "File successfully open" << endl;

	}
	else
	{
		cout << "Error opening file";
	}
	vector<pair<int, int> > points;
	pair<int, int> current;
	int n;
	infile >> n;

	for (int i = 0; i < n; i++) {
		infile >> current.first >> current.second;
		points.push_back(current);
	}

	return points;
}

void Display1() {
	GrilaCarteziana gc = GrilaCarteziana(15, 15, -1.0, -1.0, 1.0, 1.0);
	gc.afiseazaGrila();
	gc.traseazaCerc(3);
}

void Display2() {
	GrilaCarteziana gc = GrilaCarteziana(15, 15, -1.0, -1.0, 1.0, 1.0);
	gc.afiseazaGrila();
	vector<pair<int, int> > points = readPolygonPointsFromFile();
	gc.traseazaPoligon(points);
}

// aici vom desena grila si liniile
void Display()
{
	switch (prevKey)
	{
	case '0':
		glClear(GL_COLOR_BUFFER_BIT);
		Display1();
		break;
	case '1':
		glClear(GL_COLOR_BUFFER_BIT);
		Display2();
		break;
	case '2':
		glClear(GL_COLOR_BUFFER_BIT);
		break;
	}

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