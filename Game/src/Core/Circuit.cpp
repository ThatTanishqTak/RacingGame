#include "Circuit.h"

#include <fstream>

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

bool Circuit::LoadLayout(const std::string& filePath)
{
    std::ifstream File(filePath);
    if (!File.is_open())
    {
        return false;
    }

    Layout.clear();
    std::string Line;
    while (std::getline(File, Line))
    {
        for (char Tile : Line)
        {
            switch (Tile)
            {
            case '-':
            case '|':
            case '\\':
            case '/':
            case '*':
            case '&':
            case '[':
            case ']':
            case ' ':
                break;
            default:
                return false;
            }
        }

        Layout.push_back(Line);
    }

    return true;
}

const std::vector<std::string>& Circuit::GetLayout() const
{
    return Layout;
}