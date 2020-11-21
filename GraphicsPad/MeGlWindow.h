#ifndef ME_GL_WINDOW
#define ME_GL_WINDOW
#include <QtOpenGL\qglwidget>
#include <QT/qtimer.h>


class MeGlWindow : public QGLWidget
{
	Q_OBJECT
	GLuint vertexBufferID;
	QTimer myTimer;

	//void GM_QtWindow::keyPressEvent(QKeyEvent *k)
	

protected:
	void initializeGL();
	void keyPressEvent(QKeyEvent *e);
	void mouseMoveEvent(QMouseEvent* e);
	void paintGL();

private:
	void myUpdate(); 
};

#endif
