#include "application.h"


extern ST_accountsDB_t accountsDB[255];
extern ST_transaction_t transactionsDB[255];
extern int account_index;
extern int transactionNumber;

void another_transaction(void) {
	int response;
	printf("do you want another transaction? press 0 for no and 1 for yes \n");
	scanf_s("%d", &response);
	if (response) {
		appStart();
	}
	else {
		return;
	}
}

void appStart(void) {

	// creating a card and a card error object
	ST_cardData_t card1;
	EN_cardError_t card_error = CARD_OK;


	// performing the card module 
	// getting the card holder name 
	card_error = getCardHolderName(&card1);
	if (card_error == WRONG_NAME) {
		printf("wrong name, please try again \n");
		//return 0;
		another_transaction();
	}

	// getting card expiration date
	card_error = getCardExpiryDate(&card1);
	if (card_error == WRONG_EXP_DATE) {
		printf("wrong expiratioin date, please try again \n");
		//return 0;
		another_transaction();
	}

	// get card PAN
	card_error = getCardPAN(&card1);
	if (card_error == WRONG_PAN) {
		printf("wrong PAN number, please try again \n");
		//return 0;
		another_transaction();
	}

	// creating a terminal and a terminal error object
	ST_terminalData_t terminal1;
	EN_terminalError_t terminal_error = TERMINAL_OK;

	terminal_error = setMaxAmount(&terminal1);
	if (terminal_error == INVALID_MAX_AMOUNT) {
		printf("invalid maximum amount, please try again \n");
		//return 0;
		another_transaction();
	}


	// performing the terminal module 
	// getting transaction date
	terminal_error = getTransactionDate(&terminal1);
	if (terminal_error == WRONG_DATE) {
		printf("wrong date, please try again \n");
		another_transaction();
		//return 0;
	}

	// checking if card is expired
	terminal_error = isCardExpired(&card1, &terminal1);
	if (terminal_error == EXPIRED_CARD) {
		printf("expired card, please try again \n");
		another_transaction();
		//return 0;
	}

	// getting transaction amount
	terminal_error = getTransactionAmount(&terminal1);
	if (terminal_error == INVALID_AMOUNT) {
		printf("invalid amount, please try again \n");
		another_transaction();
		//return 0;
	}

	// checking if maximum amount exceeded
	terminal_error = isBelowMaxAmount(&terminal1);
	if (terminal_error == EXCEED_MAX_AMOUNT) {
		printf("exceeds maximum amount, please try again \n");
		another_transaction();
		//return 0;
	}

	// creating a transaction and a transaction error object
	ST_transaction_t transaction1;
	EN_transState_t transaction_error;
	// fill the transaction object with the card adn terminal information 
	transaction1.cardHolderData = card1;
	transaction1.terminalData = terminal1;
	// call the function of the server that recieves the transaction 
	transaction_error = recieveTransactionData(&transaction1);

	// print results to user
	printf("your balance is: %f\n", accountsDB[account_index].balance);
	printf("server status: %d\n", transaction_error);
	another_transaction();

}
// menna hassan abdou ali