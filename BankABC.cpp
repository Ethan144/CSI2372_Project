//************************************************************************
//
//   Project
//   Fall 2019
//
//   Mohamed TALEB
//
// This program reads information about clients and transactions in the following 2 files:
//          - clients.txt
//          - transact.txt
//
// It also allows certain operations on the read data:
//          - Display client bank accounts
//          - Updating client accounts
//
//************************************************************************

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <cstring>
#include <stdlib.h>
#include <iomanip>
#include<array>
#include <string.h>

#include "BankABC.h"

using namespace std;

double DepositAccount::rate = 6.50;
double totalAccounts = 0.0;

//******************************************************************
// Basic functions of the BankAccount class
//******************************************************************
inline BankAccount::BankAccount()
{
	accountId = type = updateDate = 0;
	clientName = new char[1];
	balance = 0.0;
}

inline BankAccount::BankAccount(long id, int newType,
	char * name, long newDate, double newBalance) :
	accountId(id), type(newType),
	updateDate(newDate), balance(newBalance)
{
	clientName = new char[strlen(name) + 1];
	strcpy(clientName, name);
}

inline BankAccount::~BankAccount()
{
	if (clientName)
	{
		delete[] clientName;
	}
}

inline void BankAccount::setAccountId(long account)
{
	accountId = account;
}

inline void BankAccount::setType(int newType)
{
	type = newType;
}

inline void BankAccount::setClientName(char * name)
{
	if (clientName)
	{
		delete[] clientName;
	}
	clientName = new char[strlen(name) + 1];
	strcpy(clientName, name);
}

inline void BankAccount::setUpdateDate(long newDate)
{
	updateDate = newDate;
}

inline void BankAccount::setBalance(double newBalance)
{
	balance = newBalance;
}

void BankAccount::print()
{
	cout.setf(ios::fixed);
	cout.precision(2);
	cout << accountId << "\t\t\t" << type << "\t" << updateDate << "\t\t" << balance;
}

//******************************************************************
// Basic functions of the DepositAccount class
//******************************************************************
inline DepositAccount::DepositAccount(long id, int newType, char * name,
	long newDate, double balanceArg, int nbyear) :
	BankAccount(id, newType, name,
		newDate, balanceArg), nbyears(nbyear)
{}

inline void DepositAccount::setNbYears(int nbyear)
{
	nbyears = nbyear;
}

void DepositAccount::print()
{
	Bonus();
	BankAccount::print();
	cout.setf(ios::fixed);
	cout.precision(2);
	cout << "\t" << nbyears << "\t\t" << rate << endl;
}

//******************************************************************
// Functions of the class DepositAccount. It allows to calculate
// the annual bonus on deposit accounts.
// Inputs: Nothing
// Outputs: Nothing
//******************************************************************
void DepositAccount::Bonus()
{
	setBalance(getBalance() + (getBalance()* getRate() * (getNbYears()) / 36000.00));
}



//******************************************************************
// Basic functions of the LoanAccount class
//******************************************************************
inline LoanAccount::LoanAccount(long id, int newType, char * name,
	long newDate, double newBalance, int nbyear,
	double newRate) : BankAccount(id, newType,
		name, newDate, newBalance), nbyears(nbyear), rate(newRate)
{ }

inline void LoanAccount::setNbYears(int nbyear)
{
	nbyears = nbyear;
}

inline void LoanAccount::setRate(double newRate)
{
	rate = newRate;
}

void LoanAccount::print()
{
	BankAccount::print();
	cout.setf(ios::fixed);
	cout.precision(2);
	cout << "\t" << nbyears << "\t\t" << rate << endl;
}

//******************************************************************
// Basic functions of the Transaction class
//******************************************************************
inline Transaction::Transaction(long idTr, int typeTr, long dateTr,
	int codeTr = 01, double amountTr) :
	accountId(idTr), type(typeTr),
	date(dateTr), code(codeTr),
	amount(amountTr)
{ }

inline void Transaction::setAccountId(long compteTr)
{
	accountId = compteTr;
}

inline void Transaction::setType(int typeTr)
{
	type = typeTr;
}

inline void Transaction::setDate(long dateTr)
{
	date = dateTr;
}

inline void Transaction::setCode(int codeTr)
{
	code = codeTr;
}

inline void Transaction::setAmount(double amountTr)
{
	amount = amountTr;
}




//****************************************************************************
// Purpose: Sort a list of bank accounts in ascending order of ids and types.
//
// WARNING: This sort leaves the last account (of id 0) of the list which has 
// its position to assure the tests of end of list later !!!
//
// Inputs: listAccount(BankAccount *), a list of bank accounts.
// Outputs: listAccount(BankAccount *), sorted list of bank accounts.
//****************************************************************************
void sortAccounts(BankAccount ** list)
{
	for (int i = 0; i < 5; i++) {

		long op1 = list[i]->getAccountId();
		long op2 = list[i + 1]->getAccountId();

		if (op1 > op2) {
			BankAccount* temp = list[i];
			list[i] = list[i + 1];
			list[i + 1] = temp;
		}
	}

	for (int i = 0; i < 5; i++) {
		long op1 = list[i]->getType();
		long op2 = list[i + 1]->getType();
		long op3 = list[i]->getType();
		long op4 = list[i + 1]->getType();

		if ((op1 == op2) && (op3 > op4)) {
			BankAccount* temp = list[i];
			list[i] = list[i + 1];
			list[i + 1] = temp;
		}
	}






}

//******************************************************************
// Purpose: This function reads the file 'clients.txt' and builds 
// an array containing the different bank accounts of customers.
//
// Inputs: Nothing
// Output: listAccount(type: BankAccount *), the list of bank 
//         accounts read from the file 'clients.txt'.
//******************************************************************
BankAccount ** readAccounts()
{
	ifstream inputFile("clients.txt");	// Opening the input file
	if (!inputFile)            		// If the file is not found...
	{
		cout << "File not found !!!" << endl;
		exit(0);
	}

	BankAccount ** listAccounts = new BankAccount*[K_SizeMax];
	if (!listAccounts) {
		cout << "Full memory. !!!" << endl;
		return listAccounts;
	}

	BankAccount ** pAccount = listAccounts;

	long accountRead, dateRead;
	int TypeRead, nbyearRead, counter = 0;
	double balanceRead, RateRead;
	char nameRead[60], lineRead[65], buffer[4];

	inputFile >> accountRead >> TypeRead >> dateRead >> balanceRead >> nbyearRead;
	inputFile.getline(lineRead, 65);
	strncpy(buffer, lineRead + 1, 4);
	RateRead = atof(buffer);
	strncpy(nameRead, lineRead + 5, 60);
	while (!inputFile.eof() && (counter < K_SizeMax - 1)) {
		// YOU HAVE TO DO SOMETHING FROM HERE !!!
		if (TypeRead == 03) {
			double dividend = 36000.0;
			double divisor = (balanceRead * nbyearRead * 6.5);
			double result = divisor / dividend;
			balanceRead = (double)balanceRead + result;
			*pAccount = new DepositAccount(accountRead, TypeRead, nameRead, dateRead, balanceRead, nbyearRead);
		}
		else {
			if (TypeRead == 04) {
				double dividend = 36000.0;
				double divisor = (balanceRead * nbyearRead * RateRead);
				double result = divisor / dividend;
				balanceRead = (double)balanceRead + result;
				//balanceRead = balanceRead + ((balanceRead * nbyearRead * RateRead) / 36000);
				*pAccount = new LoanAccount(accountRead, TypeRead, nameRead, dateRead, balanceRead, nbyearRead, RateRead);
			}
			else {
				*pAccount = new BankAccount(accountRead, TypeRead, nameRead, dateRead, balanceRead);
			}
		}

		// UNTIL THIS POINT.

//          inputFile >> accountRead >> TypeRead >> dateRead >> balanceRead >> nbyearRead >> RateRead;
//          inputFile.getline(nameRead, 60);
		inputFile >> accountRead >> TypeRead >> dateRead >> balanceRead >> nbyearRead;
		inputFile.getline(lineRead, 65);
		strncpy(buffer, lineRead + 1, 4);
		string bufferS = buffer;
		RateRead = atof(buffer);
		strncpy(nameRead, lineRead + 5, 60);
		pAccount++;
		counter++;
	}
	//*pAccount = new BankAccount();
	return listAccounts;
}





//*****************************************************************************************
// Purpose: This function validates whether the transaction code 
//          corresponds to the correct account:
//              - 01 ==> account (01: Check, 02: Savings, 03: Deposit and 04: Loan)
//              - 02 ==> account (01: Check, 02: Savings)
//              - 03 ==> account (01: Check).
//
// Inputs: trans (Type: Transaction) an instance of the Transaction class.
// Outputs: true (Type bool) if the transaction matches one of the accounts listed above.
// false (Type bool) otherwise.
//*****************************************************************************************
Bool BankAccount::validateTransaction(const Transaction trans) const
{
	if (((trans.getCode() == 02) && (isDepositAccount() || isLoanAccount())) ||
		((trans.getCode() == 03) && (isDepositAccount() || isLoanAccount() || isSavingsAccount())))
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}

}





//******************************************************************************
// Purpose: This function is used to execute the transaction already performed 
// (update the balance of an account).
//
// Inputs: trans (Transaction Type), instance of Transaction class
// Outputs: Nothing
//*******************************************************************************
void BankAccount::executeTransaction(const Transaction trans)
{
	if (validateTransaction(trans))
	{
		if (trans.getCode() == 01)    // Deposit
		{
			setBalance(getBalance() + trans.getAmount());
		}
		else
		{
			if (trans.getCode() == 02)    // Withdrawal
			{
				if (getBalance() >= trans.getAmount())
				{
					setBalance(getBalance() - (trans.getAmount() + 0.50));
				}
				else { cout << " insufficient balance!! " << endl; }
			}
			else 			// Check
			{
				if (getBalance() >= trans.getAmount())
				{
					setBalance(getBalance() - trans.getAmount());
				}
				else { cout << " insufficient balance!! " << endl; }
			}
		}

	}

}

//***********************************************************************
// Purpose: This function is used to execute a read transaction
// (updating the balance of the loan account).
//
// Inputs: trans (Transaction Type), instance of the Transaction class
// Outputs: Nothing
//***********************************************************************
void LoanAccount::executeTransaction(const Transaction trans)
{
	if (validateTransaction(trans))
	{
		if (trans.getCode() == 01)    // Deposit
		{
			setBalance(getBalance() - trans.getAmount());

		}
	}
}





//*************************************************************************
// Purpose: This function allows to read the file 'transact.txt' 
//          and to update the accounts concerned by the transactions read.
//
// Inputs: listAccount (type BankAccount *), the list of bank accounts.
// Output: Nothing.
//*************************************************************************
void updateAccounts(BankAccount ** listAccounts) {
	ifstream inputFile("transact.txt");	// Opening the input file
	if (!inputFile)                    // If the file is not found...
	{
		cout << "File not found !!!" << endl;
		exit(0);
	}

	int acc_id, acc_type, date, trans_code;
	double amount;
	while (true) {
		inputFile >> acc_id >> acc_type >> date >> trans_code >> amount;
		if (inputFile.eof()) break;
		Transaction tran(acc_id, acc_type, date, trans_code, amount);

		for (int i = 0; i < 6; i++) {

			long id = listAccounts[i]->getAccountId();
			int type = listAccounts[i]->getType();
			if (acc_id == id && type == acc_type) {

				listAccounts[i]->executeTransaction(tran);
			}
		}
	}

}

//******************************************************************************
// Purpose: This function displays the list of bank accounts for all customers.
//
// Inputs: listAccount (type: BankAccount *), the list of bank accounts.
// Outputs: Nothing
//******************************************************************************
void displayAccounts(BankAccount ** listAccounts)
{
	cout << endl << endl << endl;
	Bool find[K_SizeMax];
	cout.precision(2);
	std::cout.setf(std::ios::fixed);
	for (int k = 0; k < K_SizeMax; k++) { find[k] = FALSE; }

	cout << "                       THE REPORT OF THE BANK ACCOUNTS OF CLIENTS" << endl;
	cout << "                       ------------------------------------------" << endl;
	int i = 0;
	while (i < 6) {
		int tempYears = 0;
		double tempRate = 0;
		if (listAccounts[i]->isLoanAccount()) {
			LoanAccount* temp = dynamic_cast<LoanAccount*>(listAccounts[i]);
			tempYears = temp->getNbYears();
			tempRate = temp->getRate();
		}
		else if (listAccounts[i]->isDepositAccount()) {
			DepositAccount* temp = dynamic_cast<DepositAccount*>(listAccounts[i]);
			tempYears = temp->getNbYears();
			tempRate = temp->getRate();
		}
		cout << endl;
		cout << "                 Client Name:" << listAccounts[i]->getClientName() << endl << endl;
		cout << "Bank Account \t" << "Type \t" << "Update Date " << "Balance \t" << "Nb.Years " << "Rate" << endl;
		cout << listAccounts[i]->getAccountId() << "\t\t" << listAccounts[i]->getType() << "\t\t" << listAccounts[i]->getUpdatedate() << "\t\t" << listAccounts[i]->getBalance() << "\t" << tempYears << "\t" << tempRate;
		cout << endl;
		long temp = listAccounts[i]->getAccountId();
		double runningSum = listAccounts[i]->getBalance();

		i++;
		if (i > 5) {
			break;
		}
		while (temp == listAccounts[i]->getAccountId()) {
			if (listAccounts[i]->isLoanAccount()) {
				LoanAccount* temp = dynamic_cast<LoanAccount*>(listAccounts[i]);
				tempYears = temp->getNbYears();
			}
			else if (listAccounts[i]->isDepositAccount()) {
				DepositAccount* temp = dynamic_cast<DepositAccount*>(listAccounts[i]);
				tempYears = temp->getNbYears();
				tempRate = temp->getRate();
			}
			else {
				tempYears = 0;
			}
			cout << listAccounts[i]->getAccountId() << "\t\t" << listAccounts[i]->getType() << "\t\t" << listAccounts[i]->getUpdatedate() << "\t\t" << listAccounts[i]->getBalance() << "\t" << tempYears << "\t" << tempRate;
			cout << endl;
			runningSum += listAccounts[i]->getBalance();
			i++;
		}
		cout << "\t\t Total Accounts " << runningSum << endl;
	}
	cout << "\t\t Total Accounts " << listAccounts[i - 1]->getBalance() << endl;









}


int main()
{
	BankAccount ** list = readAccounts();
	sortAccounts(list);
	displayAccounts(list);
	updateAccounts(list);
	cout << endl << endl;
	cout << "               ************************************************" << endl;
	cout << "               * REAFFICHAGE DES DONNEES APRES LA MISE A JOUR *" << endl;
	cout << "               ************************************************" << endl;
	displayAccounts(list);
	cout << endl;
	system("PAUSE");
	return 0;
}