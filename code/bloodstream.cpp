#include "bloodstream.h"

Bloodstream::Bloodstream(QObject *parent)
    : QObject{parent}
    , insulinOnBoard(0)
{

}

double Bloodstream::getIOB(){
    return insulinOnBoard;
}

void Bloodstream::absorbUnits(double insulin){
    insulinOnBoard = std::max(0.0, insulinOnBoard - insulin);
}

void Bloodstream::injectUnits(double insulin){
    insulinOnBoard += insulin;
}
