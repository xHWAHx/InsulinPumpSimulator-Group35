#include "controliqalgorithm.h"
#include <numeric> 
#include <iostream> 

void ControlIQAlgorithm::analyzeGlucoseData(const std::vector<double>& data){
  if (data.empty()){
    return; 
  }

  double average = std::accumulate(data.begin(), data.end(), 0.0) / data.size(); 

  if (average < 3.9){ 
    suspendForLowGlucose(); 
  } else if (average > 10.0) { 
    increaseInsulinForHighGlucose(); 
  } else { 
    adjustBasalRate();
  }
}

void ControlIQAlgorithm::adjustBasalRate(){
  std::cout<< "Adjusting Basal Rate...\n";
}

void ControlIQAlgorithm::suspendForLowGlucose(){
  std::cout<< "Low glucose detected...Suspending Insulin!\n";
}

void ControlIQAlgorithm::increaseInsulinForHighGlucose(){
  std::cout<< "High glucose detected...Increasing Insulin!\n";
}

bool ControlIQAlgorithm::isGlucoseLevelStable(){
  return true; 
}

