#include <Qt\qapplication.h>
#include <MeGlWindow.h>

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	MeGlWindow meWindow;
	meWindow.setFixedWidth(1000);
	meWindow.setFixedHeight(1000);
	meWindow.show();
	return app.exec();
}