#include "server.h"

extern ST_accountsDB_t accountsDB[255];
extern ST_transaction_t transactionsDB[255];
extern int account_index;
extern int transactionNumber;

EN_transState_t recieveTransactionData(ST_transaction_t* transData) {

	transData->transState = APPROVED;
	// APPROVED, DECLINED_INSUFFECIENT_FUND, DECLINED_STOLEN_CARD, INTERNAL_SERVER_ERROR

	// This function will take all transaction data and validate its data.
	// It checks the account details and amount availability.
	// If the account does not exist return FRAUD_CARD, 
	ST_accountsDB_t* myaccountRefrence = NULL;
	if (isValidAccount(&transData->cardHolderData, myaccountRefrence) == ACCOUNT_NOT_FOUND) {
		transData->transState = DECLINED_STOLEN_CARD;
		return DECLINED_STOLEN_CARD;
	}
	// it returns server ok or account not found 


	// if the amount is not available will return DECLINED_INSUFFECIENT_FUND,  

	if (isAmountAvailable(&transData->terminalData) == LOW_BALANCE) {
		transData->transState = DECLINED_INSUFFECIENT_FUND;
		printf("transaction sattus: DECLINED_INSUFFECIENT_FUND \n");
		return DECLINED_INSUFFECIENT_FUND;
	}


	// if the account is blocked will return DECLINED_STOLEN_CARD, 
	if (isBlockedAccount(&accountsDB[account_index]) == BLOCKED_ACCOUNT) {
		transData->transState = DECLINED_STOLEN_CARD;
		printf("transaction sattus: DECLINED_STOLEN_CARD\n");
		return DECLINED_STOLEN_CARD;
	}


	// if a transaction can't be saved will return INTERNAL_SERVER_ERROR and will not save the transaction, 
	if (saveTransaction(transData) == SAVING_FAILED) {
		transData->transState = INTERNAL_SERVER_ERROR;
		printf("transaction sattus: INTERNAL_SERVER_ERROR\n");
		return INTERNAL_SERVER_ERROR;
	}
	//4. It will update the database with the new balance.
	accountsDB[account_index].balance = accountsDB[account_index].balance - transData->terminalData.transAmount;

	
	return APPROVED;
}
EN_serverError_t isValidAccount(ST_cardData_t* cardData, ST_accountsDB_t* accountRefrence){
	//  This function will take card data and validate if the account related to this card exists or not.
	//	It checks if the PAN exists or not in the server's database (searches for the card PAN in the DB).
	//	If the PAN doesn't exist will return ACCOUNT_NOT_FOUND, else will return SERVER_OK 
	//  and return a reference to this account in the DB.

	//8187377135764266587
	for (int i = 0; i < 255; i++) {
		if (strcmp(cardData->primaryAccountNumber, accountsDB[i].primaryAccountNumber) == 0) {
			account_index = i;
			accountRefrence = &accountsDB[account_index];
			return SERVER_OK;
		}
	}
	// If the account does not exist return DECLINED_STOLEN_CARD
	if (account_index == -1) {
		return ACCOUNT_NOT_FOUND;

	}
}
EN_serverError_t isAmountAvailable(ST_terminalData_t* termData) {
	//This function will take terminal data and validate these data.
	//It checks if the transaction's amount is available or not.
	//If the transaction amount is greater than the balance in the database will return LOW_BALANCE
	//else will return SERVER_OK

	if (termData->transAmount <= accountsDB[account_index].balance) {
		return SERVER_OK;
	}
	else {
		return LOW_BALANCE ;
	}

}
EN_serverError_t saveTransaction(ST_transaction_t* transData) {
	//This function will store all transaction data in the transactions database.

	//1_ It gives a sequence number to a transaction,
	//this number is incremented once a transaction is processed into the server, 
	//you must check the last sequence number in the server to give the new transaction a new sequence number.

	transactionsDB[transactionNumber].transactionSequenceNumber = transactionNumber; //in database
	transactionsDB[transactionNumber].cardHolderData = transData->cardHolderData;
	transactionsDB[transactionNumber].terminalData = transData->terminalData;
	//2_ It saves any type of transactions, APPROVED or DECLINED, with the specific reason for declining / transaction state.
	transactionsDB[transactionNumber].transState = transData->transState;	
	transData->transactionSequenceNumber = transactionNumber; // in the transaction coming as input 
	
	// note: you need to save what returns of the other functions in the stransState
	transactionNumber++;
	return SERVER_OK;

//3_ If the transaction can't be saved, for any reason 
//(ex: dropped connection) will return SAVING_FAILED, 
//else will return SERVER_OK, 
//you can simulate this by commenting on the lines you where your code writes the transaction data in the database.
//It checks if the transaction is saved or not using the getTransaction function.*/

}
EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t* transData) {
	//This function takes the sequence number of a transaction 
	//and returns the transaction data if found in the transactions DB.
	//If the sequence number is not found, then the transaction is not found, 
	//the function will return TRANSACTION_NOT_FOUND, 
	//else return transaction data as well as SERVER_OK

	// in my design, the sequence number is given same number as index
	// otherwise I would have wrote a simple search algortithm 
	if (transactionsDB[transactionSequenceNumber].transactionSequenceNumber = transactionSequenceNumber) {
		*transData = transactionsDB[transactionSequenceNumber];
		return SERVER_OK;
	}
	else {
		return TRANSACTION_NOT_FOUND;
	}


}
EN_serverError_t isBlockedAccount(ST_accountsDB_t* accountRefrence) {
	if (accountRefrence->state == RUNNING) {
		return SERVER_OK;
	}
	else {
		return BLOCKED_ACCOUNT;
	}

}