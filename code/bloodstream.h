#ifndef BLOODSTREAM_H
#define BLOODSTREAM_H

#include <QObject>

class Bloodstream : public QObject
{
    Q_OBJECT
public:
    explicit Bloodstream(QObject *parent = nullptr);
    void absorbUnits(double insulin);
    void injectUnits(double insulin);
    double getIOB();
signals:

private:
    double insulinOnBoard;

};

#endif // BLOODSTREAM_H
