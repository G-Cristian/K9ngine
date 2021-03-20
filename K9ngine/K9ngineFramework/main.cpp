#include <BaseApp.h>
#include <DemoChapter7.h>
#include <DemoChapter8.h>
#include <DemoChapter10.h>
#include <DemoCubeMap1.h>

using namespace K9;

int main() {
	App::BaseApp* app = new App::DemoChapter10("DemoChapter10", 800, 800);

	app->start();

	delete app;
	return 0;
}