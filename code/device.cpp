#include "device.h"
#include "ui_device.h"

Device::Device(QWidget *parent)
    : QMainWindow{parent}
    , poweredOn(false)
    , battery(new BatteryManager)
    , log(new DataLogger)
    , insulin(new InsulinReserve)
    , cgm(new CGMReader)
    , profiles(new Profile)
    , pump(new PumpController(insulin, log))
    //, interface(new UserInterface(this))
    , window(new Ui::Device)
{
    window->setupUi(this);
    interface = new UserInterface(window->uiWidget);

    connect(window->powerButton, &QPushButton::released, this, &Device::power);

    interface->hide(); // because device starts powered off
}

void Device::power(){
    if (poweredOn || battery->getBatteryLevel() == 0){
        poweredOn = false;
        interface->hide();
        window->powerLabel->setText("Device is powered off");
    } else {
        poweredOn = true;
        interface->show();
        window->powerLabel->setText("Device is powered on");

        interface->showLoginScreen();

        //Vaguely what will happen:

        //profile currentProfile = profiles.getActiveProfile(); // "profile" being a struct from profile.h

        //while (poweredOn){
			// safety checks here

			//logEntry entry; // "logEntry" being a struct from datalogger.h
			//entry.batteryLevel = battery.getBatteryLevel();
			//entry.glucose = gcm.getCurrentGlucoseLevel();
			//ControlIQAlgorithm.adjustBasalRate(glucose); // probably should actually be instantiated so the controlIQ can have a reference to the pump
			//pump.pump();							
			//entry.insulinRemaining = insulin.getInsulinRemaining();
			//log.logTick(entry);

        // something like this:
//void UserInterface::showControlIQStats() {
//    std::vector<double> data = { cgmReader->getCurrentGlucoseLevel() };
//    controlIQ->analyzeGlucoseData(data);
//    QMessageBox::information(nullptr, "ControlIQ", "Stats updated");
//}


            //sleep(1);
        //}
    }
}
