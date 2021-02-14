#include <BaseApp.h>
#include <DemoChapter7.h>
#include <DemoChapter8.h>

using namespace K9;

int main() {
	App::BaseApp* app = new App::DemoChapter8("DemoChapter8", 800, 800);

	app->start();

	delete app;
	return 0;
}