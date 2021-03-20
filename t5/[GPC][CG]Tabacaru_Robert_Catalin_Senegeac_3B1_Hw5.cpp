#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <float.h>

#include "glut.h"

// dimensiunea ferestrei in pixeli
#define dim 300

#define MAX_ITERATIONS_MANDELBROT 10000
#define MAX_ABSOLUTE_MANDELBROT 2

#define RX_Mandelbroth 0.01
#define RY_Mandelbroth 0.01
// 10000000

unsigned char prevKey;
int nivel = 0;


class C2coord
{
public:
  C2coord() 
  {
    m.x = m.y = 0;
  }

  C2coord(double x, double y) 
  {
    m.x = x;
    m.y = y;
  }

  C2coord(const C2coord &p) 
  {
    m.x = p.m.x;
    m.y = p.m.y;
  }

  C2coord &operator=(C2coord &p)
  {
    m.x = p.m.x;
    m.y = p.m.y;
    return *this;
  }

  int operator==(C2coord &p)
  {
    return ((m.x == p.m.x) && (m.y == p.m.y));
  }

protected:
  struct SDate
  {
    double x,y;
  } m;
};

class CPunct : public C2coord
{
public:
  CPunct() : C2coord(0.0, 0.0)
  {}

  CPunct(double x, double y) : C2coord(x, y)
  {}

  CPunct &operator=(const CPunct &p)
  {
    m.x = p.m.x;
    m.y = p.m.y;
    return *this;
  }

  void getxy(double &x, double &y)
  {
    x = m.x;
    y = m.y;
  }

  int operator==(CPunct &p)
  {
    return ((m.x == p.m.x) && (m.y == p.m.y));
  }

  void marcheaza()
  {
    glBegin(GL_POINTS);
      glVertex2d(m.x, m.y);
    glEnd();
  }

  void print(FILE *fis)
  {
    fprintf(fis, "(%+f,%+f)", m.x, m.y);
  }

};

class CComplex {
public:
	CComplex() : re(0.0), im(0.0) {}
	CComplex(double re1, double im1) : re(re1 * 1.0), im(im1 * 1.0) {}
	CComplex(const CComplex& c) : re(c.re), im(c.im) {}
	~CComplex() {}

	CComplex& operator=(const CComplex& c)
	{
		re = c.re;
		im = c.im;
		return *this;
	}

	double getRe() { return re; }
	void setRe(double re1) { re = re1; }

	double getIm() { return im; }
	void setIm(double im1) { im = im1; }

	double getModul() { return sqrt(re * re + im * im); }

	int operator==(CComplex& c1)
	{
		return ((re == c1.re) && (im == c1.im));
	}

	CComplex pow2()
	{
		CComplex rez;
		rez.re = powl(re * 1.0, 2) - powl(im * 1.0, 2);
		rez.im = 2.0 * re * im;
		return rez;
	}

	friend CComplex operator+(const CComplex& c1, const CComplex& c2);
	friend CComplex operator*(CComplex& c1, CComplex& c2);

	void print(FILE* f)
	{
		fprintf(f, "%.20f%+.20f i", re, im);
	}

private:
	double re, im;
};

CComplex operator+(const CComplex& c1, const CComplex& c2)
{
	CComplex rez(c1.re + c2.re, c1.im + c2.im);
	return rez;
}

CComplex operator*(CComplex& c1, CComplex& c2)
{
	CComplex rez(c1.re * c2.re - c1.im * c2.im,
		c1.re * c2.im + c1.im * c2.re);
	return rez;
}

class CVector : public C2coord
{
public:
  CVector() : C2coord(0.0, 0.0)
  {
    normalizare();
  }

  CVector(double x, double y) : C2coord(x, y)
  {
    normalizare();
  }

  CVector &operator=(CVector &p)
  {
    m.x = p.m.x;
    m.y = p.m.y;
    return *this;
  }

  int operator==(CVector &p)
  {
    return ((m.x == p.m.x) && (m.y == p.m.y));
  }

  CPunct getDest(CPunct &orig, double lungime)
  {
    double x, y;
    orig.getxy(x, y);
    CPunct p(x + m.x * lungime, y + m.y * lungime);
    return p;
  }

  void rotatie(double grade)
  {
    double x = m.x;
    double y = m.y;
    double t = 2 * (4.0 * atan(1.0)) * grade / 360.0;
    m.x = x * cos(t) - y * sin(t);
    m.y = x * sin(t) + y * cos(t);
    normalizare();
  }

  void deseneaza(CPunct p, double lungime) 
  {
    double x, y;
    p.getxy(x, y);
    glColor3f(1.0, 0.1, 0.1);
    glBegin(GL_LINE_STRIP);
      glVertex2d(x, y);
      glVertex2d(x + m.x * lungime, y + m.y * lungime);
    glEnd();
  }

  void print(FILE *fis)
  {
    fprintf(fis, "%+fi %+fj", C2coord::m.x, C2coord::m.y);
  }

private:
  void normalizare()
  {
    double d = sqrt(C2coord::m.x * C2coord::m.x + C2coord::m.y * C2coord::m.y);
    if (d != 0.0) 
    {
      C2coord::m.x = C2coord::m.x * 1.0 / d;
      C2coord::m.y = C2coord::m.y * 1.0 / d;
    }
  }
};

class CCurbaKoch
{
public:
  void segmentKoch(double lungime, int nivel, CPunct &p, CVector v)
  {
    CPunct p1;
    if (nivel == 0) 
    {
      v.deseneaza(p, lungime);
    }
    else
    {
  //    v.print(stderr);
  //    fprintf(stderr, "\n");
      segmentKoch(lungime / 3.0, nivel - 1, p, v);
      p1 = v.getDest(p, lungime / 3.0);
      v.rotatie(60);
  //    v.print(stderr);
  //    fprintf(stderr, "\n");
      segmentKoch(lungime / 3.0, nivel - 1, p1, v);
      p1 = v.getDest(p1, lungime / 3.0);
      v.rotatie(-120);
  //    v.print(stderr);
  //    fprintf(stderr, "\n");
      segmentKoch(lungime / 3.0, nivel - 1, p1, v);
      p1 = v.getDest(p1, lungime / 3.0);
      v.rotatie(60);
  //    v.print(stderr);
  //    fprintf(stderr, "\n");
      segmentKoch(lungime / 3.0, nivel - 1, p1, v);
    }
  }

  void afisare(double lungime, int nivel)
  {
    CVector v1(sqrt(3.0)/2.0, 0.5);
    CPunct p1(-1.0, 0.0);

    CVector v2(0.0, -1.0);
    CPunct p2(0.5, sqrt(3.0)/2.0);

    CVector v3(-sqrt(3.0)/2.0, 0.5);
    CPunct p3(0.5, -sqrt(3.0)/2.0);

    segmentKoch(lungime, nivel, p1, v1);
    segmentKoch(lungime, nivel, p2, v2);
    segmentKoch(lungime, nivel, p3, v3);
  }
};

class CArboreBinar
{
public:
  void arboreBinar(double lungime, int nivel, CPunct &p, CVector v)
  {
    CPunct p1;
    if (nivel == 0) 
    {
      v.deseneaza(p, lungime);
    }
    else
    {
      arboreBinar(lungime, nivel - 1, p, v);
      p1 = v.getDest(p, lungime);

      v.rotatie(-45);
      arboreBinar(lungime / 2.0, nivel - 1, p1, v);

      v.rotatie(90);
      arboreBinar(lungime / 2.0, nivel - 1, p1, v);
    }
  }

  void afisare(double lungime, int nivel)
  {
    CVector v(0.0, -1.0);
    CPunct p(0.0, 1.0);

    arboreBinar(lungime, nivel, p, v);
  }
};

class CArborePerron
{
public:
  void arborePerron(double lungime, 
                    int nivel, 
                    double factordiviziune, 
                    CPunct p, 
                    CVector v)
  {
    assert(factordiviziune != 0);
    CPunct p1, p2;
    if (nivel == 0) 
    {
    }
    else
    {
      v.rotatie(30);
      v.deseneaza(p, lungime);
      p1 = v.getDest(p, lungime);
      arborePerron(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);

      v.rotatie(-90);
      v.deseneaza(p, lungime);
      p1 = v.getDest(p, lungime);
      p2 = p1;

      v.rotatie(-30);
      v.deseneaza(p1, lungime);
      p1 = v.getDest(p1, lungime);
      arborePerron(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);

      p1 = p2;
      v.rotatie(90);
      v.deseneaza(p1, lungime);
      p1 = v.getDest(p1, lungime);
      p2 = p1;

      v.rotatie(30);
      v.deseneaza(p1, lungime);
      p1 = v.getDest(p1, lungime);
      arborePerron(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);

      p1 = p2;
      v.rotatie(-90);
      v.deseneaza(p1, lungime);
      p1 = v.getDest(p1, lungime);
      arborePerron(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);
    }
  }

  void afisare(double lungime, int nivel)
  {
    CVector v(0.0, 1.0);
    CPunct p(0.0, -1.0);

    v.deseneaza(p, 0.25);
    p = v.getDest(p, 0.25);
    arborePerron(lungime, nivel, 0.4, p, v);
  }
};



class CCurbaHilbert
{
public:
  void curbaHilbert(double lungime, int nivel, CPunct &p, CVector &v, int d)
  {
    if (nivel == 0) 
    {
    }
    else
    {
      v.rotatie(d * 90);
      curbaHilbert(lungime, nivel - 1, p, v, -d);

      v.deseneaza(p, lungime);
      p = v.getDest(p, lungime);

      v.rotatie(-d * 90);
      curbaHilbert(lungime, nivel - 1, p, v, d);

      v.deseneaza(p, lungime);
      p = v.getDest(p, lungime);

      curbaHilbert(lungime, nivel - 1, p, v, d);

      v.rotatie(-d * 90);
      v.deseneaza(p, lungime);
      p = v.getDest(p, lungime);
      
      curbaHilbert(lungime, nivel - 1, p, v, -d);

      v.rotatie(d * 90);
    }
  }

  void afisare(double lungime, int nivel)
  {
    CVector v(0.0, 1.0);
    CPunct p(0.0, 0.0);

    curbaHilbert(lungime, nivel, p, v, 1);
  }
};

class CArboreImg2
{
public:
	void arboreImg2(double lungime, int nivel, CPunct p, CVector v, double factorDiviziune)
	{
		assert(factorDiviziune != 0);
		CPunct p1, p2, p3;
		if (nivel == 0) 
		{
		}
		else {
			v.rotatie(-45);
			v.deseneaza(p, lungime);
			p1 = v.getDest(p, lungime);
			arboreImg2(lungime * factorDiviziune, nivel - 1, p1, v, factorDiviziune);


			v.rotatie(90);
			v.deseneaza(p, lungime);
			p1 = v.getDest(p, lungime);
			p2 = p1;

			v.rotatie(-45);
			v.deseneaza(p1, lungime);
			p1 = v.getDest(p1, lungime);
			p3 = p1;

			v.rotatie(-90);
			v.deseneaza(p1, lungime / 2);
			p1 = v.getDest(p3, lungime / 2);
			arboreImg2(lungime * factorDiviziune, nivel - 1, p1, v, factorDiviziune);

			v.rotatie(90); // complementam -90 
			v.rotatie(45); // complementam -45
			v.deseneaza(p3, lungime / 2);
			p1 = v.getDest(p3, lungime / 2);
			arboreImg2(lungime * factorDiviziune, nivel - 1, p1, v, factorDiviziune);

			v.rotatie(-45); // complementam 45 from earlier
			p1 = p2;
			v.rotatie(75);
			v.deseneaza(p1, lungime);
			p1 = v.getDest(p1, lungime);
			p2 = p1;
			arboreImg2(lungime * factorDiviziune, nivel - 1, p1, v, factorDiviziune);
		}
	}

	void afisare(double lungime, int nivel)
	{
		CVector v(0.0, -1.0);
		CPunct p(0.5, 2.0);
		v.deseneaza(p, 0.25);
		p = v.getDest(p, 0.25);
		arboreImg2(lungime, nivel, p, v, 0.4);
	}
};

class CMandelbrot {
public:
	CMandelbrot() {
		m.nrIterations = MAX_ITERATIONS_MANDELBROT;
		m.maxAbs = MAX_ABSOLUTE_MANDELBROT;
	}
	~CMandelbrot() {}

	// setters and getters
	void setnrIterations(int v) { assert(v <= MAX_ITERATIONS_MANDELBROT); m.nrIterations = v; }
	int getnrIterations() { return m.nrIterations; }

	void setmaxAbs(double v) { assert(v <= MAX_ABSOLUTE_MANDELBROT); m.maxAbs = v; }
	double getMaxAbs() { return m.maxAbs; }

	int isntMandelbrothNumber(CComplex& c) {
		CComplex zPrev(0, 0);
		CComplex zCurrent;
		int stoppedAt = -1;

		for(int i = 0; i < m.nrIterations; i++) 
		{
			zCurrent = zPrev * zPrev + c;

			if (zCurrent.getModul() > m.maxAbs) {
				stoppedAt = i;
				//printf("Mandelbroth: Number re(%d) im(%d) is not in set. (%d iterations expended)\n", c.getRe(), c.getIm(), i);
				break;
			}

			zPrev = zCurrent;
		}
		return stoppedAt;
	}

	double scaleBetween(double value, double minAllowed, double maxAllowed, double min, double max) 
	{
		return (maxAllowed - minAllowed) * (value - min) / (max - min) + minAllowed;
	}

	void afisare(double xmin, double ymin, double xmax, double ymax) 
	{
		glPushMatrix();
		glLoadIdentity();

		glBegin(GL_POINTS);
		double r[MAX_ITERATIONS_MANDELBROT], g[MAX_ITERATIONS_MANDELBROT], b[MAX_ITERATIONS_MANDELBROT];

		for (int i = 0; i < MAX_ITERATIONS_MANDELBROT; ++i) {
			r[i] = -1;
			g[i] = -1;
			b[i] = -1;
		}

		for (double x = xmin; x <= xmax; x += RX_Mandelbroth)
		{
			for (double y = ymin; y <= ymax; y += RY_Mandelbroth) 
			{
				CComplex c(x, y);
				int result = isntMandelbrothNumber(c);

				if (result != -1)
				{
					if (r[result] == -1 || g[result] == -1 || b[result] == -1) {
						r[result] = ((double)rand() / (RAND_MAX));
						g[result] = ((double)rand() / (RAND_MAX));
						b[result] = ((double)rand() / (RAND_MAX));
					}
					glColor3f(r[result], g[result], b[result]);
					glVertex3d(scaleBetween(x, -1, 1, xmin, xmax), scaleBetween(y, -1, 1, ymin, ymax), 0);
				}
				else {
					glColor3f(1, 0, 0);
					glVertex3d(scaleBetween(x, -1, 1, xmin, xmax), scaleBetween(y, -1, 1, ymin, ymax), 0);
				}
			}
		}

		fprintf(stdout, "STOPPED\n");
		glEnd();
		glPopMatrix();
	}
private:
	struct SDate
	{	
		int nrIterations;
		double maxAbs;
	}m;
};

class Imaginea1 {
public:
	void imaginea1(double lungime, int nivel, CPunct& p) {
		CPunct p1, p2, p3;
		if (nivel == 0) {

		}
		else {
			double l = lungime / 3;
			double x, y;
			p.getxy(x, y);
			CVector v = CVector(0, 1);
			CPunct punct = CPunct(x - l / 2, y - l / 2);
			p = punct;
			//punctul
			//prima latura a patratului
			v.deseneaza(p, l);
			p = v.getDest(p, l);
			v.rotatie(-90);

			//a doua latura a patratului
			v.deseneaza(p, l);
			p = v.getDest(p, l);
			v.rotatie(-90);

			//a treia latura a patratului
			v.deseneaza(p, l);
			p = v.getDest(p, l);
			v.rotatie(-90);

			//a 4 a latura a patratului
			v.deseneaza(p, l);
			p = v.getDest(p, l);
			//-----------------------------------------------------------------------------------------------------------------------------------------------
						//desenarea patratelor mici de langa patatul central
						//cout << x<<" , "<<y;
			CPunct stanga = CPunct(x - lungime / 3, y);
			imaginea1(lungime / 3, nivel - 1, stanga);

			CPunct dreapta = CPunct(x + lungime / 3, y);
			imaginea1(lungime / 3, nivel - 1, dreapta);

			CPunct stanga_s = CPunct(x - lungime / 3, y + lungime / 3);
			imaginea1(lungime / 3, nivel - 1, stanga_s);

			CPunct sus_m = CPunct(x, y + lungime / 3);
			imaginea1(lungime / 3, nivel - 1, sus_m);

			CPunct dreapta_s = CPunct(x + lungime / 3, y + lungime / 3);
			imaginea1(lungime / 3, nivel - 1, dreapta_s);

			CPunct stanga_j = CPunct(x - lungime / 3, y - lungime / 3);
			imaginea1(lungime / 3, nivel - 1, stanga_j);

			CPunct jos_m = CPunct(x, y - lungime / 3);
			imaginea1(lungime / 3, nivel - 1, jos_m);

			CPunct dreapta_j = CPunct(x + lungime / 3, y - lungime / 3);
			imaginea1(lungime / 3, nivel - 1, dreapta_j);




		}
	}
	void afisare(double lungime, int nivel) {
		CPunct p(0, 0);
		imaginea1(lungime, nivel, p);
	}
};

class Imagine3 {
public:
	void imagine3(double lungimea, int nivel, CPunct& p, CVector& v, int d) {
		if (nivel == 0) {
			v.deseneaza(p, lungimea);
			p = v.getDest(p, lungimea);
			return;
		}
		else {
			imagine3(lungimea / 3, nivel - 1, p, v, -d);
			v.rotatie(d * 60);
			imagine3(lungimea / 3, nivel - 1, p, v, d);
			v.rotatie(d * 60);
			imagine3(lungimea / 3, nivel - 1, p, v, -d);
		}
	}
	void afisare(double lungime, int nivel)
	{
		CVector v(0.0, 1.0);
		CPunct p(0.0, -0.5);

		imagine3(lungime * pow(3.0 / 2.0, nivel), nivel, p, v, 1);
	}
};



// afisare curba lui Koch "fulg de zapada"
void Display1() {
  CCurbaKoch cck;
  cck.afisare(sqrt(3.0), nivel);

  char c[3];
  sprintf(c, "%2d", nivel);
  glRasterPos2d(-0.98,-0.98);
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'N');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'v');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '=');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[0]);
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[1]);

  glRasterPos2d(-1.0,0.9);
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'c');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'u');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'b');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ' ');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'u');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ' ');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'K');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'o');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'c');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'h');

  nivel++;
}

// afisare arbore binar
void Display2() {
  CArboreBinar cab;
  cab.afisare(1, nivel);

  char c[3];
  sprintf(c, "%2d", nivel);
  glRasterPos2d(-0.98,-0.98);
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'N');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'v');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '=');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[0]);
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[1]);

  glRasterPos2d(-1.0,0.9);
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'b');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'o');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ' ');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'b');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'n');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');

  nivel++;
}

// afisare arborele lui Perron
void Display3() {
  CArborePerron cap;

  char c[3];
  sprintf(c, "%2d", nivel);
  glRasterPos2d(-0.98,-0.98);
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'N');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'v');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '=');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[0]);
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[1]);

  glRasterPos2d(-1.0,-0.9);
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'b');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'o');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ' ');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'P');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'o');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'n');

  glPushMatrix();
  glLoadIdentity();
  glScaled(0.4, 0.4, 1);
  glTranslated(-0.5, -0.5, 0.0);
  cap.afisare(1, nivel);
  glPopMatrix();
  nivel++;
}

// afisare curba lui Hilbert
void Display4() {
  CCurbaHilbert cch;
  cch.afisare(0.05, nivel);

  char c[3];
  sprintf(c, "%2d", nivel);
  glRasterPos2d(-0.98,-0.98);
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'N');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'v');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '=');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[0]);
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[1]);

  glRasterPos2d(-1.0,-0.9);
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'c');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'u');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'b');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ' ');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'H');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'b');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 't');

  nivel++;
}

// Imaginea 2
void Display5() {
	CArboreImg2 ca;

	char c[3];
	sprintf(c, "%2d", nivel);
	glRasterPos2d(-0.98, -0.98);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'v');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '=');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[0]);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[1]);

	glRasterPos2d(-1.0, -0.9);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'p');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'c');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 't');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'u');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ' ');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '2');

	glPushMatrix();
	glLoadIdentity();
	glScaled(0.4, 0.4, 1);
	glTranslated(-0.5, -0.5, 0.0);
	ca.afisare(0.7, nivel);
	glPopMatrix();
	nivel++;
}
//Mandelbrot
void Display6() {
	CMandelbrot cm;

	cm.setnrIterations(30);
	cm.afisare(-2.0, -2.0, 2.0, 2.0);
}

void DisplayImaginea1()
{
	Imaginea1 imagine;
	double lungime = 2;
	double x = 0;
	double y = 0;
	glColor3f(1, 0.1, 0.1);
	imagine.afisare(lungime, nivel);

	//desenarea patratului exterior
	lungime = lungime * 0.999;
	CVector v = CVector(0, 1);
	CPunct p = CPunct(x - lungime / 2, y - lungime / 2);

	v.deseneaza(p, lungime);
	p = v.getDest(p, lungime);
	v.rotatie(-90);

	v.deseneaza(p, lungime);
	p = v.getDest(p, lungime);
	v.rotatie(-90);

	v.deseneaza(p, lungime);
	p = v.getDest(p, lungime);
	v.rotatie(-90);

	v.deseneaza(p, lungime);
	p = v.getDest(p, lungime);

	char c[3];
	sprintf(c, "%2d", nivel);
	glColor3f(0, 0, 0);
	glRasterPos2d(-0.98, -0.98);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'N');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'v');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '=');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[0]);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[1]);

	glRasterPos2d(-1.0, -0.9);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'm');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'g');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'n');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ' ');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '1');

	nivel++;
}

void DisplayImaginea3()
{
	Imagine3 imagine;
	imagine.afisare(1, nivel);

	glColor3f(0.4, 0.2, 0.3);

	char c[3];
	sprintf(c, "%2d", nivel);
	glRasterPos2d(-0.98, -0.98);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'N');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'v');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '=');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[0]);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[1]);

	glRasterPos2d(-1.0, 0.9);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'm');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'g');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'n');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ' ');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '3');

	nivel++;
}

void Init(void) {

   glClearColor(1.0,1.0,1.0,1.0);

   glLineWidth(1);

   glPointSize(3);

   glPolygonMode(GL_FRONT, GL_LINE);
}

void Display(void) 
{
  switch(prevKey) 
  {
    case '0':
      glClear(GL_COLOR_BUFFER_BIT);
      nivel = 0;
      fprintf(stderr, "nivel = %d\n", nivel);
      break;
    case '1':
      glClear(GL_COLOR_BUFFER_BIT);
      Display1();
      break;
    case '2':
      glClear(GL_COLOR_BUFFER_BIT);
      Display2();
      break;
    case '3':
      glClear(GL_COLOR_BUFFER_BIT);
      Display3();
      break;
    case '4':
      glClear(GL_COLOR_BUFFER_BIT);
      Display4();
      break;
	case '5':
		glClear(GL_COLOR_BUFFER_BIT);
		Display5();
		break;
	case '6':
		glClear(GL_COLOR_BUFFER_BIT);
		Display6();
		break;
	case '7':
		glClear(GL_COLOR_BUFFER_BIT);
		DisplayImaginea1();
		break;
	case '8':
		glClear(GL_COLOR_BUFFER_BIT);
		DisplayImaginea3();
		break;
    default:
      break;
  }

  glFlush();
}

void Reshape(int w, int h) 
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
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

int main(int argc, char** argv) 
{
  glutInit(&argc, argv);

  glutInitWindowSize(dim, dim);

  glutInitWindowPosition(100, 100);

  glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);

  glutCreateWindow (argv[0]);

  Init();

  glutReshapeFunc(Reshape);

  glutKeyboardFunc(KeyboardFunc);

  glutMouseFunc(MouseFunc);

  glutDisplayFunc(Display);

  glutMainLoop();

  return 0;
}


