#include "Circuit.h"

Circuit::Circuit(const std::string& name, double length) : Name(name), Length(length)
{

}

const std::string& Circuit::GetName() const
{
    return Name;
}

double Circuit::GetLength() const
{
    return Length;
}

void Circuit::SetLength(double length)
{
    Length = length;
}