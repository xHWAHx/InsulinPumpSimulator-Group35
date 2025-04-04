#include "boluscalculator.h"
#include  <iostream> 

double BolusCalculator::calculateBolus(double glucose, double carbs){
  return calculateBolus(carbs, 10.0) + calculateCorrectionDose(glucose, 5.5, 2.0);
}

double BolusCalculator::suggestDose(){
  return calculateBolus(8.5, 40.0);
}

void BolusCalculator::overrideDose(double dose){
  std::cout << "Dose Updated.\n"; 
}

bool BolusCalculator::validateBolusInput(double dose){
  return dose >= 0.0 && dose <= 25.0; 
}

double BolusCalculator::calculateCorrectionDose(double glucose, double target, double sensitivity){
  if (glucose  <= target){
    return 0.0; 
  }
  return  (glucose - target) / sensitivity; 
}

double BolusCalculator::calculateCarbBolus(double carbs, double ratio){
  return carbs / ratio; 
}

std::pair<double, double> BolusCalculator::splitBolus(double totalDose, double percentage){
  double immediateDose = totalDose * (percentage / 100.0); 
  double extendedDose = totalDose - immediateDose; 
  return { immediateDose, extendedDose }; 
}
