#pragma once
#include "Report.h"
#include <string>
#include <iostream>
#include <limits>

// Przestrzeñ nazw dla globalnych funkcji pomocniczych.
namespace Utils{
    void clearInputBuffer();

    std::string readLine(std::istream& in);

	int getValidatedInt(int min, int max);
    void printReport(const ReportData& report);
    void clearScreen();
    void pause();
}