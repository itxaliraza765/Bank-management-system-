#include<iostream>
#include<cstring>
using namespace std;


class Transaction {
public:
	string type;
	double amount;
	double balance;
	Transaction* next;
	Transaction* head;
	Transaction(const string& t, double a, double balanc) {
		type = t;
		amount = a;
		balance = balanc;
		next = NULL;
	}
	void addTransaction(const string& type, double amount) {
		Transaction* newTransaction = new Transaction(type, amount, balance);
		//insert t strt
		newTransaction->next = head;
		head = newTransaction;
		if (type == "Deposit") {
			balance += amount;
		}
		else if (type == "Withdrawal") {
			balance -= amount;
		}
	}
};
class BankAccount:public Transaction  {
public:
	int accountNo;
	string accountHolder;
	double initial_balance;
	BankAccount* account;

	BankAccount(int accNum, const string& name, double initialBal){
		accountNo = accNum;
	    accountHolder = name;
		initial_balance = initialBal;
		BankAccount* account=NULL;


	}
	void deposit(customerNode&getcustomer, int customerID, int accountNumber, double amount) {
		customerNode* customer =getcustomer.getcustomer_By_iD(customerID);
		if (customer->customer_id == customerID) {	
		//calling function by passing type and amount to deposit
	  addTransaction("Deposit", amount);
		cout << "Deposit successfully New balance is " << balance << endl;
	}
		}


	void withdraw(double amount) {
		if (amount > balance) {
			cout << "Insufficient balance"<<endl;
		}
		else {
		addTransaction("Withdrawal", amount);
		cout << "Withdrawal successfully new balance is " << balance <<endl;
		}
	}
	 
};
class customerNode{
    public:
		int customer_id;
		string customer_name;
		customerNode* head;
		customerNode* next;
	customerNode(int id, string name) {
			customer_id = id;
			customer_name = name;
			next = NULL;
	}
	bool isCustomerfound(int customer_id) {
		customerNode* current = head;
		while (current) {
			if (current->customer_id == customer_id)
				return true;
			current = current->next;
		}
		return false;
	}

	customerNode* getcustomer_By_iD(int customer_id) {
		customerNode* current = head;
		while (current) {
			if (current->customer_id== customer_id)
				return current;
			current = current->next;
		}
		return NULL;
	}
	customerNode* getCustomer_By_Account(int accountNumber) {
		customerNode* current = head;
		while (current) {
			if (current->account && current->account->getAccountNumber() == accountNumber)
				return current;
			current = current->next;
		}
		return nullptr;
	}
	/*void add_customer() {
		if (isCustomerfound(customer_id)) {
			cout << "customer with the given id " << customer_id << " already exists \n";
		}
			customerNode* newCustomer = new customerNode (customer_id, customer_name);
			newCustomer->next = head;
			head = newCustomer;
			cout << "customer is added successfully"<<endl;
   	}*/
	

};
int main() {

	return 0;
}