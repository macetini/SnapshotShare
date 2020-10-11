#include <FMedia.h>
#include "SnapshotShare.h"
#include "FormManager.h"
#include "Main.h"

using namespace Osp::App;
using namespace Osp::Base;
using namespace Osp::System;
using namespace Osp::Ui;
using namespace Osp::Ui::Controls;
using namespace Osp::Graphics;

using namespace Osp::Media;

SnapshotShare::SnapshotShare()
{
}

SnapshotShare::~SnapshotShare()
{
}

Application*
SnapshotShare::CreateInstance(void)
{
	// Create the instance through the constructor.
	return new SnapshotShare();
}

bool SnapshotShare::OnAppInitializing(AppRegistry& appRegistry)
{
	// TODO:
	// Initialize UI resources and application specific data.
	// The application's permanent data and context can be obtained from the appRegistry.
	//
	// If this method is successful, return true; otherwise, return false.
	// If this method returns false, the application will be terminated.

	// Uncomment the following statement to listen to the screen on/off events.
	//PowerManager::SetScreenEventListener(*this);

	// Create a form
	Main* pMain = new Main();

	result r = FormManager::GetInstance()->Register(Main::K_SCREEN_ID, pMain);

	bool isInit = pMain->Initialize();

	FormManager::GetInstance()->Jump(Main::K_SCREEN_ID);

	if (isInit && !IsFailed(r))
	{
		return true;
	}
	else
	{
		return false;
	}

}

bool SnapshotShare::OnAppTerminating(AppRegistry& appRegistry, bool forcedTermination)
{
	FormManager* pFormManager = FormManager::GetInstance();

	delete pFormManager;
	pFormManager = NULL;

	return true;
}

void SnapshotShare::OnForeground(void)
{
	// TODO:
	// Start or resume drawing when the application is moved to the foreground.


}

void SnapshotShare::OnBackground(void)
{
	// TODO:
	// Stop drawing when the application is moved to the background.
}

void SnapshotShare::OnLowMemory(void)
{
	// TODO:
	// Free unused resources or close the application.
}

void SnapshotShare::OnBatteryLevelChanged(BatteryLevel batteryLevel)
{
	// TODO:
	// Handle any changes in battery level here.
	// Stop using multimedia features(camera, mp3 etc.) if the battery level is CRITICAL.
}

void SnapshotShare::OnScreenOn(void)
{
	// TODO:
	// Get the released resources or resume the operations that were paused or stopped in OnScreenOff().
}

void SnapshotShare::OnScreenOff(void)
{
	// TODO:
	//  Unless there is a strong reason to do otherwise, release resources (such as 3D, media, and sensors) to allow the device to enter the sleep mode to save the battery.
	// Invoking a lengthy asynchronous method within this listener method can be risky, because it is not guaranteed to invoke a callback before the device enters the sleep mode.
	// Similarly, do not perform lengthy operations in this listener method. Any operation must be a quick one.
}

