# Insulin-Pump-Simulator
A simulation for the Tandem t:slim X2 insulin pump using Qt/C++.

## Group Members
Hamzah Hamad #101230812 

Thu Thu Chit Pyae #101229440

Justin Schoenhofer #101186399

Kamal Yassin #101265070

Aidan Casselman #101277801

## Responsibilities
1. Project Planning
- Outline team responsibilities, schedule and timeline (Shared responsibility)
- Plan the design (Shared responsibility)
- Create GitHub repository (Hamzah)

2. Design
- Use cases (Thu Thu)
- Use Case Diagram (Thu Thu)
- UML class diagram (Kamal) 
- UML state machine diagram (Hamzah) 
- UML sequence diagram (Justin) 
- Requirements Traceability Matrix & Test Video (Aidan) 
- An explanation of our design decisions (Thu Thu)

3. Development
- User Interface class, home.ui screen, CGM-charts, button interactions (options, bolus, history), login (pin), screen layout policies (Kamal)
- Settings (CRUD) and PumpController (Thu Thu)
- Basic device functions and control flow (Justin)
- Data storage, logging, history, profile management, and documentation (Aidan)
- Bolus calculator and manual bolus operation (.cpp, .h, .ui), ControlIQ (.cpp, .h) (Hamzah)

## File Organization

### Root Directory
`InsulinPumpSimulator-Group35-main/`

---

### UML Diagrams
Located in `Design-Documentation/`:
- `Sequence-Diagrams/`
    - `Create-Profile.png`
    - `Low-Battery.png`
    - `Main-Device-Loop.png`
    - `Manual-Bolus.png`
    - `Update-Profile.png`
    - `View-History.png`

- `Design-Explanation.pdf`
- `State-Diagrams.pdf`
- `Traceability-Matrix.pdf`
- `UML-Class-Diagram.png`
- `Use-Cases.pdf`
- `Use-Case-Diagram.png`

---

### Source Code
Located in `code/`:
- `main.cpp`
- `batterymanager.cpp`, `batterymanager.h`
- `boluscalculator.cpp`, `boluscalculator.h`, `boluscalculator.ui`
- `cgmreader.cpp`, `cgmreader.h`
- `controliqalgorithm.cpp`, `controliqalgorithm.h`
- `datalogger.cpp`, `datalogger.h`
- `device.cpp`, `device.h`, `device.ui`
- `history.cpp`, `history.h`, `history.ui`
- `home.cpp`, `home.h`, `home.ui`
- `insulinreserve.cpp`, `insulinreserve.h`
- `login.cpp`, `login.h`, `login.ui`
- `profile.cpp`, `profile.h`
- `pumpcontroller.cpp`, `pumpcontroller.h`
- `settings.cpp`, `settings.h`, `settings.ui`
- `userinterface.cpp`, `userinterface.h`, `userinterface.ui`
- `alert.cpp`, `alert.h`, `alert.ui`
- `bloodstream.cpp`, `bloodstream.h`

---

### Test Video
- `Test-Video.mp4`

## Documentation

[View the full project documentation here](https://xhwahx.github.io/InsulinPumpSimulator-Group35/)

## Compiling instructions
Make sure QtCharts is installed: `sudo apt install libqt5charts5-dev`

### In the terminal:
First, navigate to the `code/` directory:

- `cd code/`

Then run:

- `qmake insulinPump.pro`
- `make`
- `./insulinPump`

### In Qt Creator:
- Open `insulinPump.pro`
- Press **Build**
- Press **Run**
