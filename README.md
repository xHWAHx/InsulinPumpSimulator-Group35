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
`InsulinPumpSimulator-Group35.zip/`

---

### UML Diagrams
Located in `Design-Documentation/`:
- `Design-Explanation.pdf`
- `Sequence-Diagram-Main-Loop.png`  
- `State-Diagrams.pdf`
- `Traceability-Matrix.pdf`
- `UML-Class-Diagram.png`
- `Use-Cases.pdf`

---

### Source Code
Located in `code/`:

**Core Modules:**

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

## Documentation

[View the full project documentation here](https://xhwahx.github.io/InsulinPumpSimulator-Group35/)

## Compiling instructions
Make sure QtCharts is installed with `sudo apt install libqt5charts5-dev`

### In the terminal:
- `qmake insulinPump.pro`
- `make`
- `./insulinPump`

### In Qt Creator:
- Press `'Build'`
- Press `'Run'`
