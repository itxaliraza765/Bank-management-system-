#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
using namespace std;

class CustomerNode;
CustomerNode* customerHead = NULL;

class Transaction;
Transaction* transactionHead;

class Transaction {
public:
    string type;
    double amount;
    double balanceAfter;
    Transaction* next;

    Transaction(const string& typ, double amnt, double balance) {
        type = typ;
        amount = amnt;
        balanceAfter = balance;
        next = NULL;
    }
};

class CustomerNode {
public:
    int customerID;
    string customerName;
    double balance;
    CustomerNode* next;

    CustomerNode(int id, const string& name, double initialBalance) {
        customerID = id;
        customerName = name;
        balance = initialBalance;
        next = NULL;
        transactionHead = NULL;
    }

    // Deposit money and add transaction
    void deposit(double amount) {
        addTransaction("Deposit", amount);
        balance += amount;
        cout << "->Deposit successfully New balance is ->" << balance << endl;
    }

    // Withdraw money and add transaction
    bool withdraw(double amount) {
        if (amount > balance) {
            cout << "-> Insufficient balance -?\n";
            return false;
        }
        addTransaction("Withdrawal", amount);
        balance -= amount;
        cout << "-> Withdrawal successfully New balance is -> " << balance << endl;
        return true;
    }

    void addTransaction(const string& type, double amount) {
        double newBalance;
        if (type == "Deposit") {
            newBalance = balance + amount;
        }
        else {
            newBalance = balance - amount;
        }
        Transaction* newTransaction = new Transaction(type, amount, newBalance);
        newTransaction->next = transactionHead;
        transactionHead = newTransaction;
    }
    void displayTransactionHistory() const {
        Transaction* current = transactionHead;
        cout << "<--- Transaction History --->\n";
        while (current) {
            cout << current->type << " " << current->amount << " Balance After " << current->balanceAfter << "\n";
            current = current->next;
        }
    }
    static CustomerNode* getCustomerByID(CustomerNode* head, int customerID) {
        CustomerNode* current = head;
        while (current) {
            if (current->customerID == customerID)
                return current;
            current = current->next;
        }
        return nullptr;
    }

    static CustomerNode* getCustomerByAccount(CustomerNode* head, int accountID) {
        return getCustomerByID(head, accountID);
    }

    static bool isCustomerExists(CustomerNode* head, int customerID) {
        return getCustomerByID(head, customerID) != nullptr;
    }

    
};

class Bank {
private:
    void saveCustomers() {
        ofstream outFile("customers.txt");
        CustomerNode* current = customerHead;
        while (current) {
            outFile << current->customerID << " " << current->customerName << " " << current->balance << endl;
            current = current->next;
        }
        outFile.close();
    }

    void loadCustomers() {
        ifstream inFile("customers.txt");
        int id;
        string name;
        double balance;
       
        while (inFile >> id >> ws && getline(inFile, name, ' ') && inFile >> balance)
        //>> ws ignore whitespaces eg. spaces, tabs  for the next input.
       //(inFile, name, ' ') Reads a name from the file up to the next space (' ').
        {
            addCustomer(id, name, balance);
        }
        inFile.close();
    }
public:
    Bank() {
        loadCustomers();
    }

    void addCustomer(int customerID, const string& customerName, double initialBalance) {
        if (CustomerNode::isCustomerExists(customerHead, customerID)) {
            cout << "-> Customer with ID " << customerID << " already found -?\n";
            return;
        }
        CustomerNode* newCustomer = new CustomerNode(customerID, customerName, initialBalance);
        newCustomer->next = customerHead;
        customerHead = newCustomer;
        cout << "<** Customer added successfully **>\n";
        saveCustomers();
    }

    void deposit(int customerID, double amount) {
        CustomerNode* customer = CustomerNode::getCustomerByID(customerHead, customerID);
        if (customer) {
            customer->deposit(amount);
            saveCustomers();
        }
        else {
            cout << "--> Customer not found --?\n";
        }
    }

    void withdraw(int customerID, double amount) {
        CustomerNode* customer = CustomerNode::getCustomerByID(customerHead, customerID);
        if (customer) {
            customer->withdraw(amount);
            saveCustomers();
        }
        else {
            cout << "--> Customer not found --?\n";
        }
    }

    void transfer(int fromAccount, int toAccount, double amount) {
        CustomerNode* sender = CustomerNode::getCustomerByAccount(customerHead, fromAccount);
        CustomerNode* receiver = CustomerNode::getCustomerByAccount(customerHead, toAccount);
        if (!sender || !receiver) {
            cout << "--> One or both accounts are not found --?\n";
            return;
        }
        if (sender->withdraw(amount)) {
            receiver->deposit(amount);
            cout << "<** Transfer successfully **>\n";
            saveCustomers();
        }
    }

    void displayCustomerDetails(int customerID) {
        CustomerNode* customer = CustomerNode::getCustomerByID(customerHead, customerID);
        if (customer) {
            cout << "<---------------------------------->\n";
            cout << "Customer ID --> " << customer->customerID << endl;
            cout << "Customer Name --> " << customer->customerName << endl;
            cout << "Customer Balance --> " << fixed << setprecision(2) << customer->balance << endl;
        }
        else {
            cout << "--> Customer not found --?\n";
        }
    }

    void displayAllCustomers() {
        CustomerNode* current = customerHead;
        if (!current) {
            cout << "<** No customers in the bank **>\n";
            return;
        }
        while (current) {
            cout << "<---------------------------------->\n";
            cout << "Customer ID --> " << current->customerID << endl;
            cout << "Customer Name --> " << current->customerName << endl;
            cout << "Customer Balance --> " << fixed << setprecision(2) << current->balance << endl;
            current = current->next;
        }
    }
    void displayTransactionHistory(int customerID) {
        CustomerNode* customer = CustomerNode::getCustomerByID(customerHead, customerID);
        if (customer) {
            customer->displayTransactionHistory();
        }
        else {
            cout << "--> Customer not found --?\n";
        }
    }

    void deleteCustomer(int customerID) {
        CustomerNode* current = customerHead;
        CustomerNode* previous = NULL;
        while (current && current->customerID != customerID) {
            previous = current;
            current = current->next;
        }
        if (!current) {
            cout << "Customer not found\n";
            return;
        }
        if (previous)
            previous->next = current->next;
        else
            customerHead = current->next;

        delete current;
        cout << "Customer deleted successfully.\n";
        saveCustomers();
    }

};

int main() {
    Bank bank;
    int choice, customerID, toAccount;
    string customerName;
    double amount, initialBalance;

    while (true) {
        cout << "\n\t\t\t\t<-_-_-_-_-_-_- WELCOME TO Bank MANAGEMENT SYSTEM -_-_-_-_-_-_->\t\t\t\n\n\n";
        cout << "\t\t\t<----  Press 1-> Add customer\t\t\t" << "Press 2-> Deposit money              ---->\n\n";
        cout << "\t\t\t<----  Press 3-> withdraw money\t\t\t" << "Press 4-> display Customer Details   ---->\n\n";
        cout << "\t\t\t<----  Press 5-> display All Customers\t\t" << "Press 6->display Transaction History ---->\n\n";
        cout << "\t\t\t<----  Press 7-> delete Customer\t\t" << "Press 8-> transfer money             ---->\n\n";
        cout << "\t\t\t<----  Press 0->Exit program   \t\t\t\t\t\t             ---->\t\t\t\n\n\n\n";

        cout << "<-_- Enter your choice -_->";
        cin >> choice;
        cout << "\n\n";
        switch (choice) {
        case 1:
            cout << "Enter Customer ID ";
            cin >> customerID;
            cout << "Enter Customer Name ";
            cin.ignore();
            getline(cin, customerName);
            cout << "Enter Initial Balance ";
            cin >> initialBalance;
            bank.addCustomer(customerID, customerName, initialBalance);
            break;

        case 2:
            cout << "Enter Customer ID ";
            cin >> customerID;
            cout << "Enter Amount to Deposit ";
            cin >> amount;
            bank.deposit(customerID, amount);
            break;

        case 3:
            cout << "Enter Customer ID ";
            cin >> customerID;
            cout << "Enter Amount to Withdraw ";
            cin >> amount;
            bank.withdraw(customerID, amount);
            break;

        case 4:
            cout << "Enter Customer ID ";
            cin >> customerID;
            bank.displayCustomerDetails(customerID);
            break;

        case 5:
            bank.displayAllCustomers();
            break;

        case 6:
            cout << "Enter Customer ID  ";
            cin >> customerID;
            bank.displayTransactionHistory(customerID);
            break;
        case 7:
            cout << "Enter Customer ID to delete ";
            cin >> customerID;
            bank.deleteCustomer(customerID);
            break;

        case 8:
            cout << "Enter the Sender's Customer ID ";
            cin >> customerID;
            cout << "Enter the Receiver's Customer ID ";
            cin >> toAccount;
            cout << "Enter Amount to Transfer ";
            cin >> amount;
            bank.transfer(customerID, toAccount, amount);
            break;

        case 0:
            cout << "Exiting the system.\n";
            return 0;

        default:
            cout << "Invalid choice Try again.\n";
        }
    }
}
