// ============================================================
//  Menu.cpp
//  Terminal UI — banner, menu, and pressEnter utility.
//  No logic here, purely display functions.
// ============================================================

#pragma once
#include <iostream>
#include <limits>
using namespace std;

void printBanner() {
    cout << "\033[2J\033[H";
    cout << "\n";
    cout << "   ______________________________________________________ \n";
    cout << "  |                                                      |\n";
    cout << "  |     ISLAMABAD AMBULANCE DISPATCH AND NAVIGATION      |\n";
    cout << "  |______________________________________________________|\n";
    cout << "  |   Node 0 = PIMS Hospital                             |\n";
    cout << "  |   50 Real Islamabad Locations                        |\n";
    cout << "  |______________________________________________________|\n";
    cout << "  |   COMSATS University Islamabad | DS Project SP26     |\n";
    cout << "  |   Group: BSE-039 | BSE-042 | BSE-047 | BSE-056       |\n";
    cout << "  |______________________________________________________|\n";
}

void printMenu() {
    cout << "\n";
    cout << "   _____________________________________________________ \n";
    cout << "  |                      MAIN MENU                      |\n";
    cout << "  |_____________________________________________________|\n";
    cout << "  |     --------------- Navigation ----------------     |\n";
    cout << "  |   1. Find Danger+Traffic-Aware Shortest Path        |\n";
    cout << "  |_____________________________________________________|\n";
    cout << "  |     ----------- Ambulance Dispatch ------------     |\n";
    cout << "  |   2. Register Patient Emergency Call                |\n";
    cout << "  |   3. View Patient Call Queue                        |\n";
    cout << "  |   4. Dispatch Ambulance                             |\n";
    cout << "  |_____________________________________________________|\n";
    cout << "  |     ----------- Data and Monitoring -----------     |\n";
    cout << "  |   5. Report New Incident                            |\n";
    cout << "  |   6. Report Traffic Congestion                      |\n";
    cout << "  |   7. View All Active Danger Alerts (Max-Heap)       |\n";
    cout << "  |   8. View News Ticker                               |\n";
    cout << "  |   9. View All Incidents (Incident BST In-Order)     |\n";
    cout << "  |   10.View Traffic Data (Traffic BST In-Order)       |\n";
    cout << "  |   11.View Dijkstra Search Space (Last Run)          |\n";
    cout << "  |   12.Adjust Risk Multiplier                         |\n";
    cout << "  |   13.Print All Node Names                           |\n";
    cout << "  |_____________________________________________________|\n";
    cout << "  |   14.Run Full System Demo                           |\n";
    cout << "  |   0. Exit                                           |\n";
    cout << "  |_____________________________________________________|\n";
    cout << "  Enter choice: ";
}

void pressEnter() {
    cout << "\n  Press ENTER to return to menu...";
    cin.ignore(1000, '\n');
    cin.get();
}