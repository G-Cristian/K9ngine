#include <BaseApp.h>
#include <DemoChapter7.h>

using namespace K9;

int main() {
	App::BaseApp* app = new App::DemoChapter7("DemoChapter7", 800, 600);

	app->start();

	delete app;
	return 0;
}