#ifndef SCENE3D_H
#define SCENE3D_H

#include <QGLWidget>
#include <QFile>
#include <fstream>
#include <QDebug>
#include <QObject>
#include <vector>
#include "PathCalculator.h"

class Scene3D : public QGLWidget
{
   Q_OBJECT

   private:
      GLfloat xRot;
      GLfloat yRot;
      GLfloat zRot;
      GLfloat xTra;
      GLfloat yTra;
      GLfloat nSca;
      double anX;
      double anY;
      double anZ;
      QPoint ptrMousePosition;

      PathCalculator* pathCalculator;
      int pNum;

      bool var;
      void scale_plus();
      void scale_minus();
      void rotate_up();
      void rotate_down();
      void rotate_left();
      void rotate_right();
      void rotate_forward();
      void rotate_backward();
      void translate_down();
      void translate_up();
      void translate_left();
      void translate_right();
      void defaultScene();

      void drawAxis();
      void drawCube(double x, double y, double z, double a,int color=0);
      void drawCam(double x, double y, double z, double a, double rx, double ry, double rz, unsigned char* color);
      void drawLine(Vector3D p1, Vector3D p2, double scale, int color);
      void drawPoints();
      void drawTheWay();
      void myRotateGL(GLfloat xr, GLfloat yr, GLfloat zr);
   protected:
      void initializeGL();
      void resizeGL(int nWidth, int nHeight);
      void paintGL();
      void mousePressEvent(QMouseEvent* pe);
      void mouseMoveEvent(QMouseEvent* pe);
      void mouseReleaseEvent(QMouseEvent* pe);
      void wheelEvent(QWheelEvent* pe);
      void keyPressEvent(QKeyEvent* pe);
   public:
      Scene3D(PathCalculator*, QWidget* parent = 0);
      ~Scene3D();
   public slots:

      void dgordan();
};
#endif
