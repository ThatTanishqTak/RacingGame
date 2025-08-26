#pragma once

#include <string>
#include <vector>

class Circuit
{
public:
    Circuit(const std::string& name, double length);

    const std::string& GetName() const;
    double GetLength() const;
    void SetLength(double length);

    bool LoadLayout(const std::string& filePath);
    const std::vector<std::string>& GetLayout() const;

private:
    std::string Name;
    double Length;
    std::vector<std::string> Layout;
};