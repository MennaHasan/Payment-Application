#include "terminal.h"

EN_terminalError_t setMaxAmount(ST_terminalData_t* termData) {
	//call this function in main so you can set the max amount in the terminal
	float max_amount = 20000;
	termData->maxTransAmount = max_amount;

	if (termData->maxTransAmount <= 0) {
		return INVALID_MAX_AMOUNT;
	}
	else {
		return TERMINAL_OK;
	}
}

EN_terminalError_t getTransactionDate(ST_terminalData_t* termData) {

	//asking for the transaction data and storing it in terminal data.

	printf("Please enter the transaction date\n");
	printf("note that transaction date is in the format dd / mm / yyyy, e.g 25 / 06 / 2022.\n");
	int size = sizeof(termData->transactionDate);
	scanf_s("%s", termData->transactionDate, size);
	printf("The transaction date is = %s\n", termData->transactionDate);

	// getting length of string from user input
	int date_length = 0;
	for (int i = 0; i < size; i++) {
		if (termData->transactionDate[i] == '\0') {
			date_length = i;
			break;
		}
	}
	printf("The transaction date length = %d\n", date_length);


	// checking for validity of input and returning error in case of error
	if (date_length < 10) {
		return WRONG_DATE;
	}
	for (int i = 0; i++; i < date_length) {
		if (i == 2 || i == 5) {
			if (termData->transactionDate[i] != '\/') {
				return WRONG_DATE;
			}
		}
		else {
			if (!isdigit(termData->transactionDate[i])) {
				return WRONG_DATE;
			}
		}
	}

	//returning OK if no errors
	return TERMINAL_OK;
}

EN_terminalError_t isCardExpired(ST_cardData_t* cardData, ST_terminalData_t* termData) {

	//getting the dates 
	int expiration_month = (cardData->cardExpirationDate[0] - '0') * 10 + (cardData->cardExpirationDate[1] - '0');
	int expiration_year = (2000 + (cardData->cardExpirationDate[3] - '0') * 10 + (cardData->cardExpirationDate[4]) - '0');
	printf("expiration month is %d and expiration year is %d\n", expiration_month, expiration_year);

	int transaction_month = (termData->transactionDate[3] - '0') * 10 + (termData->transactionDate[4] - '0');
	int transaction_year = (termData->transactionDate[6] - '0') * 1000 + (termData->transactionDate[7] - '0') * 100 + (termData->transactionDate[8] - '0') * 10 + (termData->transactionDate[9] - '0');
	printf("transaction month is %d and transaction year is %d\n", transaction_month, transaction_year);

	//checking year
	if (expiration_year > transaction_year) {
		return TERMINAL_OK;
	}
	else if (expiration_year == transaction_year) {
		if (expiration_month > transaction_month) {
			return TERMINAL_OK;
		}
		else {
			return EXPIRED_CARD;
		}
	}
	else {
		return EXPIRED_CARD;
	}
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData) {

	printf("Please enter the transaction amount\n");
	int size = sizeof(termData->transAmount);
	scanf_s("%f", &termData->transAmount);
	printf("The transaction amount is = %f\n", termData->transAmount);

	if (termData->transAmount <= 0) {
		return INVALID_AMOUNT;
	}
	else {
		return TERMINAL_OK;
	}
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData) {
	if (termData->transAmount > termData->maxTransAmount) {
		return EXCEED_MAX_AMOUNT;
	}
	else {
		return TERMINAL_OK;
	}
}

EN_terminalError_t isValidCardPAN(ST_cardData_t* cardData) {

	/* This function checks if the PAN is a Luhn number or not.
	If PAN is not a Luhn number will return INVALID_CARD, else */

	//predicting the Luhn digit 
	int num = 0;
	int Luhn_digit = 0;
	for (int k = 17; k >= 0; k--) {
		num = (cardData->primaryAccountNumber[k]) - '0';
		if (k % 2 == 0) {
			Luhn_digit += num;
		}
		else {
			if (num * 2 >= 10) {
				Luhn_digit += (num * 2) % 10 + 1;
			}
			else {
				Luhn_digit += (num * 2);
			}
		}
	}
	Luhn_digit = (10 - (Luhn_digit % 10)) % 10;



	// checking the Lunh digit and returning results
	if (cardData->primaryAccountNumber[18] - '0' == Luhn_digit) {
		return 0; // TERMINAL_OK 
	}
	else {
		return 3; // INVALID_CARD
	}
	// NOTE: 
	// the function returns 0 and 3 because card.c dosn't see the enum 
	// and I can't include it, it cuases include loop 
}
/*
optional:
the function will read the current date from your computer and store
it into terminal data with the mentioned size and format.
*/
