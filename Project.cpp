#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <windows.h>
#include <ctime>
#include <string>
using namespace std;

class Bill {
private:
    string item;
    int rate, quantity;

public:
    Bill() : item(""), rate(0), quantity(0) {}

    void setItem(const string& i) { item = i; }
    void setRate(int r) { rate = r; }
    void setQuantity(int q) { quantity = q; }

    string getItem() const { return item; }
    int getRate() const { return rate; }
    int getQuantity() const { return quantity; }

    int calculateAmount(int qty) const { return rate * qty; }
};

// Utility to change console text color
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// Function to add new items to inventory
void addItem() {
    Bill b;
    bool close = false;

    while (!close) {
        int choice;
        cout << "\n\t1. Add Item\n\t2. Back to Menu\n\tEnter Choice: ";
        cin >> choice;

        if (choice == 1) {
            system("cls");
            cin.ignore();

            string item;
            int rate, qty;

            cout << "\tEnter Item Name: ";
            getline(cin, item);
            b.setItem(item);

            cout << "\tEnter Rate: ";
            cin >> rate;
            b.setRate(rate);

            cout << "\tEnter Quantity: ";
            cin >> qty;
            b.setQuantity(qty);

            ofstream out("BILL.txt", ios::app);
            if (!out) {
                setColor(12); // red
                cout << "\tError: Unable to open file!" << endl;
                setColor(7);
            } else {
                out << b.getItem() << ":" << b.getRate() << ":" << b.getQuantity() << endl;
                out.close();
                setColor(10); // green
                cout << "\tItem Added Successfully!" << endl;
                setColor(7);
            }

            Sleep(1500);
            system("cls");
        } else {
            close = true;
        }
    }
}

// Function to generate bill
void printBill() {
    system("cls");
    Bill b;
    int total = 0;
    static int billNo = 1;

    time_t now = time(0);
    tm* ltm = localtime(&now);

    // Generate unique bill file
    stringstream filename;
    filename << "Bill_" << setfill('0') << setw(3) << billNo << ".txt";
    ofstream billFile(filename.str());

    // Print bill header
    cout << "\n\t====== SUPER MARKET BILL ======\n";
    cout << "\tBill No: " << billNo << endl;
    cout << "\tDate: " << 1900 + ltm->tm_year << "/"
         << 1 + ltm->tm_mon << "/"
         << ltm->tm_mday << " "
         << ltm->tm_hour << ":"
         << ltm->tm_min << ":"
         << ltm->tm_sec << endl;
    cout << "\t---------------------------------\n";
    cout << left << setw(15) << "Item"
         << setw(10) << "Rate"
         << setw(5) << "Qty"
         << setw(10) << "Amount" << endl;
    cout << "\t---------------------------------\n";

    billFile << "SUPER MARKET BILL\nBill No: " << billNo << "\nDate: "
             << 1900 + ltm->tm_year << "/"
             << 1 + ltm->tm_mon << "/"
             << ltm->tm_mday << " "
             << ltm->tm_hour << ":"
             << ltm->tm_min << ":"
             << ltm->tm_sec << "\n\n";

    billFile << left << setw(15) << "Item"
             << setw(10) << "Rate"
             << setw(5) << "Qty"
             << setw(10) << "Amount" << "\n";
    billFile << "---------------------------------\n";

    bool close = false;
    while (!close) {
        int choice;
        cout << "\n\t1. Add Item to Bill\n\t2. Finish Bill\n\tEnter Choice: ";
        cin >> choice;

        if (choice == 1) {
            cin.ignore();
            string searchItem;
            int qty;

            cout << "\tEnter Item Name: ";
            getline(cin, searchItem);

            cout << "\tEnter Quantity: ";
            cin >> qty;

            ifstream in("BILL.txt");
            ofstream temp("BILL_temp.txt");

            string line;
            bool found = false;

            while (getline(in, line)) {
                stringstream ss(line);
                string name, r, q;
                getline(ss, name, ':');
                getline(ss, r, ':');
                getline(ss, q);

                int rate = stoi(r);
                int stock = stoi(q);

                if (name == searchItem && stock >= qty) {
                    found = true;
                    b.setRate(rate);
                    int amount = b.calculateAmount(qty);
                    total += amount;

                    cout << "\t" << left << setw(15) << name
                         << setw(10) << rate
                         << setw(5) << qty
                         << setw(10) << amount << endl;

                    billFile << left << setw(15) << name
                             << setw(10) << rate
                             << setw(5) << qty
                             << setw(10) << amount << endl;

                    temp << name << ":" << rate << ":" << (stock - qty) << endl;
                } else {
                    temp << line << endl;
                }
            }

            if (!found) {
                setColor(12); // red
                cout << "\tItem not available or insufficient stock!\n";
                setColor(7);
            }

            in.close();
            temp.close();

            remove("BILL.txt");
            rename("BILL_temp.txt", "BILL.txt");
        } else {
            close = true;
        }
    }

    cout << "\t---------------------------------\n";
    cout << "\tTOTAL BILL: " << total << endl;
    cout << "\tThank You For Shopping!\n";
    billFile << "\nTOTAL BILL: " << total << "\nThank You For Shopping!\n";
    billFile.close();

    billNo++;
    Sleep(3000);
}

int main() {
    bool exit = false;

    while (!exit) {
        system("cls");
        int choice;
        cout << "\n====================================\n";
        cout << "      SUPER MARKET BILLING SYSTEM\n";
        cout << "====================================\n";
        cout << "\n\t1. Add Items\n\t2. Generate Bill\n\t3. Exit\n\tEnter Choice: ";
        cin >> choice;

        if (choice == 1)
            addItem();
        else if (choice == 2)
            printBill();
        else
            exit = true;
    }

    return 0;
}
