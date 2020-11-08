#include <Qt\qapplication.h>
#include <MeGlWindow.h>

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	MeGlWindow meWindow;
	meWindow.setFixedWidth(2000);
	meWindow.setFixedHeight(2000);
	meWindow.show();
	return app.exec();
}