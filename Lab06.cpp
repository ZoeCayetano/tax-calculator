//**************************************************************************************************
// FILE:        Lab06.cpp
//
// DESCRIPTION: Opens a file named "payroll.txt" for reading and reads payroll information for one
//              employee. Calculates the employee's paycheck information. Outputs the employee
//              paycheck data to a file named "paycheck.txt".
//
//              This lab project illustrates the following C++ concepts:
//              1. Functions. Function calls. Pass parameters by-value. Return values.
//              2. Reading from a text file.
//              3. Writing to a text file.
//              4. Formatting output to a text file.
//              5. Writing if statements, if-else statements, and if-elseif-elseif... statements.
//
// AUTHORS:     Zoe Aiyanna M. Cayetano           (zcayetan@asu.edu)
//
// COURSE:      CSE100 Principles of Programming with C++, Spring 2016
// -------------------------------------------------------------------------------------------------
// TEST CASE 1:
// -------------------------------------------------------------------------------------------------
// Test Input Data:
// Simpson Homer
// 15.25 84
// 2
//
// Expected Results:
// -----------------------------
// EMPLOYEE: Simpson, Homer
//
// PAY RATE:           $   15.25
// HOURS:                  84.00
// GROSS PAY:          $ 1311.50
// MED INS DEDUCT:     $  110.13
// 401K DEDUCT:        $   78.69
// FED TAX GROSS PAY:  $ 1122.68
// TAX - FEDERAL:      $   89.48
// TAX - OASDI:        $   69.61
// TAX - MEDICARE:     $   16.28
// TAX - STATE:        $   38.62
// TAX - TOTAL:        $  213.98
// NET PAY:            $  908.70
// -----------------------------
//
// Test Results: PASSED
//
// -------------------------------------------------------------------------------------------------
// TEST CASE 2:
// -------------------------------------------------------------------------------------------------
// Test Input Data:
// Sagan Carl
// 30.75 90
// 0
//
// Expected Results:
// -----------------------------
// EMPLOYEE: Sagan, Carl
//
// PAY RATE:           $   30.75
// HOURS:                  90.00
// GROSS PAY:          $ 2921.25
// MED INS DEDUCT:     $   32.16
// 401K DEDUCT:        $  175.28
// FED TAX GROSS PAY:  $ 2713.82
// TAX - FEDERAL:      $  529.19
// TAX - OASDI:        $  168.26
// TAX - MEDICARE:     $   39.35
// TAX - STATE:        $  210.05
// TAX - TOTAL:        $  946.85
// NET PAY:            $ 1766.96
// -----------------------------
//
// Test Results: PASSED
//
// -------------------------------------------------------------------------------------------------
// TEST CASE 3:
// -------------------------------------------------------------------------------------------------
// Test Input Data:
// Hawking Stephen
// 28.67 87
// 1
//
// Expected Results:
// -----------------------------
// EMPLOYEE: Hawking, Stephen
//
// PAY RATE:           $   28.67
// HOURS:                  87.00
// GROSS PAY:          $ 2594.64
// MED INS DEDUCT:     $   64.97
// 401K DEDUCT:        $  155.68
// FED TAX GROSS PAY:  $ 2373.99
// TAX - FEDERAL:      $  302.68
// TAX - OASDI:        $  147.19
// TAX - MEDICARE:     $   34.42
// TAX - STATE:        $  183.75
// TAX - TOTAL:        $  668.04
// NET PAY:            $ 1705.95
// -----------------------------
//
// Test Results: PASSED
//**************************************************************************************************
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;
const double OASDI_RATE = 0.062;
const double FOUR01K_RATE = 0.06;
const double MEDICARE_RATE = 0.0145;
const double MED_INS_DEDUCT_EMPL_ONLY = 32.16;  // Employee Only
const double MED_INS_DEDUCT_EMPL_ONE  = 64.97;  // Employee + One
const double MED_INS_DEDUCT_FAMILY    = 110.13;  // Family
const int MED_INS_STATUS_EMPL_ONLY = 0;  // Employee Only
const int MED_INS_STATUS_EMPL_ONE  = 1;  // Employee + One
const int MED_INS_STATUS_FAMILY    = 2;  // Family

void ErrorExit(string msg)
{
    cout << msg << endl;
    exit(-1);
}

double CalcGrossPay(double pPayRate, double pHrsWorked)
{
    // Implement the pseudocode above which uses an if-else statement.
    double grossPay = 0;
    if (pHrsWorked <= 80) {
        grossPay = pHrsWorked * pPayRate;
    } else {
        grossPay = 80 * pPayRate + (pHrsWorked - 80) * (1.5 * pPayRate);
    }
    return grossPay;
}

double CalcMedInsDeduct(int pMedInsStatus)
{
    double medInsDeduct = 0;

    if (pMedInsStatus == MED_INS_STATUS_EMPL_ONLY) {
        medInsDeduct = MED_INS_DEDUCT_EMPL_ONLY;
    } else if (pMedInsStatus == MED_INS_STATUS_EMPL_ONE) {
        medInsDeduct = MED_INS_DEDUCT_EMPL_ONE;
    } else if (pMedInsStatus == MED_INS_STATUS_FAMILY) {
        medInsDeduct = MED_INS_DEDUCT_FAMILY;
    }
    return medInsDeduct;
}

double CalcTaxFed(double pFedTaxGrossPay)
{
    double taxFed = 0;
    
    if (pFedTaxGrossPay >= 384.62 && pFedTaxGrossPay < 1413.67) {
        taxFed = 0.0797 * pFedTaxGrossPay;
    } else if (pFedTaxGrossPay >=1413.67 && pFedTaxGrossPay < 2695.43) {
        taxFed = 0.1275 * pFedTaxGrossPay;
    } else if (pFedTaxGrossPay >= 2695.43) {
        taxFed = 0.195 * pFedTaxGrossPay;
    }
    return taxFed;
}

double CalcTaxState(double pFedTaxGrossPay)
{
    double taxState = 0;
    if (pFedTaxGrossPay < 961.54) {
        taxState = pFedTaxGrossPay * 0.0119;
    } else if (pFedTaxGrossPay < 2145.66) {
        taxState = pFedTaxGrossPay * 0.0344;
    } else {
        taxState = pFedTaxGrossPay * 0.0774;
    }
    return taxState;
}

int main()
{
    ifstream fin("payroll.txt");
    if (!fin) {
        ErrorExit("Could not open 'payroll.txt' for reading.");
    }
    
    string firstName, lastName;
    fin >> lastName >> firstName;
    
    double payRate;
    fin >> payRate;
    
    double hrsWorked;
    fin >> hrsWorked;
    
    int medInsStatus;
    fin >> medInsStatus;
    
    fin.close();
    
    double grossPay = CalcGrossPay(payRate, hrsWorked);
    double four01kDeduct = grossPay * FOUR01K_RATE;
    double medInsDeduct = CalcMedInsDeduct(medInsStatus);
    double fedTaxGrossPay = grossPay - (four01kDeduct + medInsDeduct);
    double taxFed = CalcTaxFed(fedTaxGrossPay);
    double taxOASDI = fedTaxGrossPay * OASDI_RATE;
    double taxMedicare = fedTaxGrossPay * MEDICARE_RATE;
    double taxState = CalcTaxState(fedTaxGrossPay);
    double taxTotal = taxFed + taxOASDI + taxMedicare + taxState;
    double netPay = fedTaxGrossPay - taxTotal;
    
    ofstream fout("paycheck.txt");
    
    if (!fout) {
        ErrorExit("Could not open 'paycheck.txt' for writing.");
    }
    
    fout << fixed << setprecision(2);
    fout << right;
    
    fout << "-----------------------------"    << endl;
    fout << "EMPLOYEE: " << lastName << ", "   << firstName << endl << endl;
    fout << "PAY RATE:           $" << setw(8) << payRate << endl;
    fout << "HOURS:               " << setw(8) << hrsWorked << endl;
    fout << "GROSS PAY:          $" << setw(8) << grossPay << endl;
    fout << "MED INS DEDUCT:     $" << setw(8) << medInsDeduct << endl;
    fout << "401K DEDUCT:        $" << setw(8) << four01kDeduct << endl;
    fout << "FED TAX GROSS PAY:  $" << setw(8) << fedTaxGrossPay << endl;
    fout << "TAX - FEDERAL:      $" << setw(8) << taxFed << endl;
    fout << "TAX - OASDI:        $" << setw(8) << taxOASDI << endl;
    fout << "TAX - MEDICARE:     $" << setw(8) << taxMedicare << endl;
    fout << "TAX - STATE:        $" << setw(8) << taxState << endl;
    fout << "TAX - TOTAL:        $" << setw(8) << taxTotal << endl;
    fout << "NET PAY:            $" << setw(8) << netPay << endl;
    fout << "-----------------------------"    << endl;
    
    fout.close();
    
    return 0;
}