#include <BaseApp.h>
#include <DemoChapter7.h>
#include <DemoChapter8.h>
#include <DemoCubeMap1.h>

using namespace K9;

int main() {
	App::BaseApp* app = new App::DemoCubeMap1("DemoCubeMap1", 800, 800);

	app->start();

	delete app;
	return 0;
}