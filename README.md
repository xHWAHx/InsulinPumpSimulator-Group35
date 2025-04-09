# Insulin-Pump-Simulator
A simulation for the Tandem t:slim X2 insulin pump using Qt/C++.

# Group Members:
Hamzah Hamad #101230812 

Thu Thu Chit Pyae #101229440

Justin Schoenhofer #101186399

Kamal Yassin #101265070

Aidan Casselman #101277801

# Responsibilites: 
1. Project Planning
- Outline team responsibilities, schedule and timeline (Shared responsibility)
- Plan the design (Shared responsibility)
- Create github repository (Hamzah)

2. Design
- Use cases (Thu Thu) 
- UML class diagram (Kamal) 
- UML state machine diagram (Hamzah) 
- UML sequence diagram (Justin) 
- Requirements Traceability Matrix (Aidan) 
- An explanation of our design decisions (Thu Thu)

3. Development
- Home page and lock screen (Kamal) 
- Profiles (Thu Thu)
- Basic device functions, basic indicators (battery, USB...), alert popup (Justin)
- Data Visualization (insulin fill gauge, IOB, …) (Aidan)
- Bolus calculator and manual bolus operation, ControlIQ (Hamzah)

## File Organization

### Root Directory  
`InsulinPumpSimulator-Group35.zip/`

---

### UML Diagrams  
Located in `UML Diagrams/`:

- `design explanation draft.pdf`  
- `sequence diagram - main loop.png`  
- `state diagrams.pdf`  
- `use cases.pdf`

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
- `alert.ui`
      
## Documentation

[View the full documentation here](https://xhwahx.github.io/InsulinPumpSimulator-Group35/)

## Compiling Instructions (Terminal): 
- `qmake insulinPump.pro` 
- `make`
- `./insulinPump` 

## Compiling Instructions (Qt Builder): 
- `Press 'Build'`
