#define _USE_MATH_DEFINES
#include <QtGui>
#include <math.h>
#include <QtMath>
#include "scene3D.h"
#include <QObject>
#include <stdio.h>
#include <iostream>
#include <assert.h>

/* лёжа (100%)
 * смещение влево -> красный -  !
 * смещение вправо -> красный + !
 * смещение вниз -> зеленый -   !
 * смещение вверх -> зеленый +  !
 * смещение от себя -> синий -  !
 * смецение на себя -> синий +  !
*/
const static float pi=3.141593, k=pi/180;

GLfloat VertexArray[12][3];
GLfloat ColorArray[12][3];
GLubyte IndexArray[20][3];


Scene3D::Scene3D(PathCalculator* pc, QWidget* parent) : QGLWidget(parent)
{
   xRot=180; yRot=0; zRot=180; xTra=0; yTra=0; nSca=1;
   var=0;
   pathCalculator = pc;
   pNum = pathCalculator->getRecNum();
   Vector3D lastP = pathCalculator->getPoint(pNum-1);

   double anB = atan(lastP.x / lastP.y);
   double arg = lastP.z / (cos(anB) * lastP.y + sin(anB) * lastP.x);
   double anA = atan( arg );
   anA *= 180 / M_PI;
   anB *= 180 / M_PI;
   anZ = anB + 180;
   anX = anA + 90;
   //zRot = anB;
   qDebug()<<anA<<" "<<anB;
   qDebug()<<"LAST POINT";
   //qDebug()<<lastP.x<<" "<<lastP.y<<" "<<lastP.z;
}

Scene3D::~Scene3D() {
    qDebug()<<"DELETING";
}

void Scene3D::initializeGL()
{
   qglClearColor(Qt::white);
   glEnable(GL_DEPTH_TEST);
   glShadeModel(GL_FLAT);
   glEnable(GL_CULL_FACE);

   glEnableClientState(GL_VERTEX_ARRAY);
   glEnableClientState(GL_COLOR_ARRAY);
}

void Scene3D::resizeGL(int nWidth, int nHeight)
{
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   GLfloat ratio=(GLfloat)nHeight/(GLfloat)nWidth;

   if (nWidth>=nHeight)
      glOrtho(-1.0/ratio, 1.0/ratio, -1.0, 1.0, -10.0, 100.0);
   else
      glOrtho(-1.0, 1.0, -1.0*ratio, 1.0*ratio, -10.0, 100.0);

   glViewport(0, 0, (GLint)nWidth, (GLint)nHeight);
}

void Scene3D::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   glScalef(nSca, nSca, nSca);
   glTranslatef(xTra, yTra, 0.0f);
   glRotatef(xRot, 1.0f, 0.0f, 0.0f);
   glRotatef(yRot, 0.0f, 1.0f, 0.0f);
   glRotatef(zRot, 0.0f, 0.0f, 1.0f);

   drawAxis();
   //drawPoints();
   drawTheWay();
}

void Scene3D::myRotateGL(GLfloat xr, GLfloat yr, GLfloat zr)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   glScalef(nSca, nSca, nSca);
   glTranslatef(xTra, yTra, 0.0f);
   glRotatef(xr, 1.0f, 0.0f, 0.0f);
   glRotatef(yr, 0.0f, 1.0f, 0.0f);
   glRotatef(zr, 0.0f, 0.0f, 1.0f);

   drawAxis();
   //drawPoints();
   drawTheWay();
}

void Scene3D::mousePressEvent(QMouseEvent* pe)
{
    unsigned char buf[3];
    glReadPixels(pe->x(), pe->y(), 1, 1, GL_RGB, GL_UNSIGNED_BYTE, buf);
    if(pe->button()==1)
    {
        var=0;
    }
    if(pe->button()==2)
    {
        var=1;
    }
    ptrMousePosition = pe->pos();
}

void Scene3D::mouseReleaseEvent(QMouseEvent* pe)
{

}

void Scene3D::mouseMoveEvent(QMouseEvent* pe)
{

   if(var==0)
   {
        xRot += 180/nSca*(GLfloat)(pe->y()-ptrMousePosition.y())/height();
        zRot += 180/nSca*(GLfloat)(pe->x()-ptrMousePosition.x())/width();

        ptrMousePosition = pe->pos();

   }
   if(var==1)
   {
       xTra += 180/nSca*(GLfloat)(pe->x()-ptrMousePosition.x())/width()/100;
       yTra += -180/nSca*(GLfloat)(pe->y()-ptrMousePosition.y())/height()/100;

       ptrMousePosition = pe->pos();
   }

   updateGL();
}

void Scene3D::wheelEvent(QWheelEvent* pe)
{
   if ((pe->delta())>0) scale_plus();
   else if ((pe->delta())<0) scale_minus();

   updateGL();
}

void Scene3D::keyPressEvent(QKeyEvent* pe)
{
    //qDebug() << "key: " << pe->key() << " "<< (int)pe->modifiers() << " " << Qt::CTRL <<"\n";
    switch (pe->key())
    {
    case Qt::Key_Plus:
        scale_plus();
        break;

    case Qt::Key_Equal:
        scale_plus();
        break;

    case Qt::Key_Minus:
        scale_minus();
        break;

    case Qt::Key_Up:
        rotate_up();
        break;

    case Qt::Key_Down:
        rotate_down();
        break;

    case Qt::Key_Left:
        rotate_left();
        break;

    case Qt::Key_Right:
        rotate_right();
        break;

    case Qt::Key_1:
        rotate_forward();
        break;

    case Qt::Key_2:
        rotate_backward();
        break;

    case Qt::Key_S:
        translate_up();
        break;

    case Qt::Key_W:
        translate_down();
        break;

    case Qt::Key_A:
        translate_right();
        break;

    case Qt::Key_D:
        translate_left();
        break;

    case Qt::Key_Space:
        defaultScene();
        break;
    case Qt::Key_B:
        xRot = anX;
        zRot = anZ;
        break;
    case Qt::Key_Escape:
        this->close();
        break;
    }

   updateGL();
}

void Scene3D::scale_plus()
{
   nSca = nSca*1.1;
}

void Scene3D::scale_minus()
{
   nSca = nSca/1.1;
}

void Scene3D::rotate_up()
{
   xRot += 90;
}

void Scene3D::rotate_down()
{
   xRot -= 90;
}

void Scene3D::rotate_left()
{
   zRot += 90;
}

void Scene3D::rotate_right()
{
   zRot -= 90;
}

void Scene3D::rotate_forward()
{
    yRot += 1.0;
}

void Scene3D::rotate_backward()
{
    yRot -= 1.0;
}

void Scene3D::translate_down()
{
   yTra -= 0.05;
}

void Scene3D::translate_up()
{
   yTra += 0.05;
}

void Scene3D::translate_left()
{
    xTra -= 0.05;
}

void Scene3D::translate_right()
{
    xTra += 0.05;
}

void Scene3D::defaultScene()
{
   xRot=-90; yRot=0; zRot=0; yTra=0; nSca=1;
}

void Scene3D::drawAxis()
{
   glLineWidth(3.0f);

   glColor4f(1.00f, 0.00f, 0.00f, 1.0f);
   glBegin(GL_LINES);
      glVertex3f( 1.0f,  0.0f,  0.0f);
      glVertex3f(-1.0f,  0.0f,  0.0f);
   glEnd();

   QColor halfGreen(0, 128, 0, 255);
   qglColor(halfGreen);
   glBegin(GL_LINES);
      glVertex3f( 0.0f,  1.0f,  -0.0f);
      glVertex3f( 0.0f, -1.0f,  0.0f);

      glColor4f(0.00f, 0.00f, 1.00f, 1.0f);
      glVertex3f( 0.0f,  0.0f,  1.0f);
      glVertex3f( 0.0f,  0.0f, -1.0f);
   glEnd();

   unsigned char axisColor[] = { 0, 0, 255 };
//   unsigned char *axisColor = new unsigned char[3];
//   axisColor[0] = 0; axisColor[1] = 1; axisColor[2] = 255;
   drawCam(1, 0, 0, 0.02, 0, -90, 0, axisColor);
   drawCam(0, 1, 0, 0.02, 90, 0, 0, axisColor);
   drawCam(0, 0, 1, 0.02, 180, 0, 0, axisColor);
}

void Scene3D::drawLine(Vector3D p1, Vector3D p2, double scale, int color)
{
    glBegin(GL_LINES);
        if( color == 0 ) glColor3f(0.0,1.0,0.0);
        else glColor3f(0.8,0.8,0.0);
        glVertex3f(p1.x * scale , p1.y * scale , p1.z * scale);
        glVertex3f(p2.x * scale , p2.y * scale , p2.z * scale);
    glEnd();
}

void Scene3D::drawCube(double x, double y, double z, double a, int color)
{
   // if (color==2) a+=0.004;


    glBegin(GL_QUADS);

       if (color%4==1) glColor3f(0.0f,0.0f,1.0f);              // Синий
       glVertex3d(x+a, y+a, z-a);          // Право верх квадрата (Верх)
       glVertex3d(x-a, y+a, z-a);          // Лево верх
       glVertex3d(x-a, y+a, z+ a);          // Лево низ
       glVertex3d(x+a, y+a, z+ a);          // Право низ


       if (color%4==2)   glColor3f(1.0f,0.5f,0.0f);              // Оранжевый
       glVertex3d(x+a, y-a, z+a);          // Верх право квадрата (Низ)
       glVertex3d(x-a, y-a, z+a);          // Верх лево
       glVertex3d(x-a, y-a, z-a);          // Низ лево
       glVertex3d(x+a, y-a, z-a);          // Низ право


       if (color%4==0) glColor3f(1.0f,0.0f,0.0f);              // Красный
       glVertex3d(x+a, y+a, z+a);          // Верх право квадрата (Перед)
       glVertex3d(x-a, y+a, z+a);          // Верх лево
       glVertex3d(x-a, y-a, z+a);          // Низ лево
       glVertex3d(x+a, y-a, z+a);          // Низ право


       if (color%4==3) glColor3f(1.0f,1.0f,0.0f);              // Желтый
       glVertex3d(x+a, y-a, z-a);          // Верх право квадрата (Зад)
       glVertex3d(x-a, y-a, z-a);          // Верх лево
       glVertex3d(x-a, y+a, z-a);          // Низ лево
       glVertex3d(x+a, y+a, z-a);          // Низ право


       //glColor3f(0.0f,0.0f,1.0f);              // Синий
       glVertex3d(x-a, y+a, z+a);          // Верх право квадрата (Лево)
       glVertex3d(x-a, y+a, z-a);          // Верх лево
       glVertex3d(x-a, y-a, z-a);          // Низ лево
       glVertex3d(x-a, y-a, z+a);          // Низ право


       //glColor3f(1.0f,0.0f,1.0f);              // Фиолетовый
       glVertex3d(x+ a, y+a,  z-a);          // Верх право квадрата (Право)
       glVertex3d(x+ a, y+a,  z+a);          // Верх лево
       glVertex3d(x+ a, y-a,  z+a);          // Низ лево
       glVertex3d(x+ a, y-a,  z-a);          // Низ право

    glEnd();

}

void Scene3D::drawCam(double x, double y, double z, double a, double rx, double ry, double rz, unsigned char* color)
{
    glPushMatrix();
    //glLoadIdentity();

    glTranslatef(x, y, z);

    glRotatef(rx, 1.0f, 0.0f, 0.0f);
    glRotatef(ry, 0.0f, 1.0f, 0.0f);
    glRotatef(rz, 0.0f, 0.0f, 1.0f);

    glBegin(GL_QUADS);

    glColor3f(color[0]/255, color[1]/255, color[2]/255);
       glVertex3d(-a,-a, a);
       glVertex3d(a,-a, a);
       glVertex3d(a,a, a);
       glVertex3d(-a,a, a);
    glEnd();

    glBegin(GL_TRIANGLES);

        glVertex3d(0,0, -2*a);
        glVertex3d(a,-a, a);
        glVertex3d(-a,-a, a);

        glVertex3d(0,0, -2*a);
        glVertex3d(a,a, a);
        glVertex3d(a,-a, a);

        glVertex3d(0,0, -2*a);
        glVertex3d(-a,-a, a);
        glVertex3d(-a,a, a);

        glVertex3d(0, 0, -2*a);
        glVertex3d(-a,a, a);
        glVertex3d( a,a, a);

    glEnd();

    glPopMatrix();
}

void Scene3D::dgordan()
{
    this->update();
}

void Scene3D::drawPoints()
{
    const double scale=10.0;

    glPointSize(5.0f);
    glColor3f(0.58f,0.0f,0.827f);
    glBegin(GL_POINTS);
    for (int i=0; i<pNum; i++)
    {
        Vector3D p = pathCalculator->getPoint(i);
        glVertex3f(p.x*scale, p.y*scale, p.z*scale);
    }
    glEnd();
}

void Scene3D::drawTheWay()
{
    Vector3D prevPoint;
    const double scale=10.0;

    for (int i=0; i<pNum; i++)
    {
        Vector3D p = pathCalculator->getPoint(i);
        drawLine(prevPoint, p, scale, 0);
        prevPoint = p;
    }
    Vector3D p1 = pathCalculator->getPoint( 0 );
    Vector3D p2 = prevPoint;
    //drawLine(p1, p2, scale, 1);
//    qDebug()<<xRot<<" "<<yRot<<" "<<zRot;
}
