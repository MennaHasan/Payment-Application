#include "card.h"

EN_cardError_t getCardHolderName(ST_cardData_t* cardData){
	// note: make it know how to deal with spaces
	printf("Please enter the cardholder's name: \n");
	int size = sizeof(cardData->cardHolderName);

	gets(cardData->cardHolderName);

	printf("The cardholder's name = %s\n", cardData->cardHolderName);


	int name_length = 0;
	for (int i = 0; i < 50; i++) {
		if (cardData->cardHolderName[i] == '\0') {
			name_length = i;
			break;
		}
	}

	printf("The cardholder's name length = %d\n", name_length);

	if ((name_length > 24) || (name_length < 20) || ((name_length == 0))) {
		return WRONG_NAME;
	}
	else {
		return CARD_OK;
	}
}

EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData) {
	printf("Please enter the card's expiry date\n");
	printf("note that Card expiry date is 5 characters string in the format ""MM / YY"", e.g ""05 / 25"".\n");
	int size = sizeof(cardData->cardExpirationDate);

	scanf_s("%s", cardData->cardExpirationDate, size);

	printf("The expiration date is = %s\n", cardData->cardExpirationDate);


	int date_length = 0;
	for (int i = 0; i < 6; i++) {
		if (cardData->cardExpirationDate[i] == '\0') {
			date_length = i;
			break;
		}
	}

	printf("The cardholder's date length = %d\n", date_length);


	if (date_length != 5) {
		return WRONG_EXP_DATE;
	}
	else {
		if (cardData->cardExpirationDate[2] != '\/') {
			return WRONG_EXP_DATE;
		}
		else {
			if (isdigit(cardData->cardExpirationDate[0]) && isdigit(cardData->cardExpirationDate[1]) && isdigit(cardData->cardExpirationDate[3]) && isdigit(cardData->cardExpirationDate[4])) {
				int month = (cardData->cardExpirationDate[0] - '0') * 10 + (cardData->cardExpirationDate[1] - '0');
				int year = (cardData->cardExpirationDate[3] - '0') * 10 + (cardData->cardExpirationDate[4] - '0');
				printf("month is %d and year is %d\n", month, year);
				if (month > 12 || month < 1 || year < 22) {
					return WRONG_EXP_DATE;
				}
			}
			else {
				return CARD_OK;
			}
		}
	}
}

EN_cardError_t getCardPAN(ST_cardData_t* cardData) {
	printf("Please enter the card's Primary Account Number (PAN)\n");
	int size = sizeof(cardData->primaryAccountNumber);

	scanf_s("%s", cardData->primaryAccountNumber, size);
	printf("The Primary Account Number (PAN) = %s\n", cardData->primaryAccountNumber);

	int PAN_length = 0;
	for (int i = 0; i < 20; i++) {
		if (cardData->primaryAccountNumber[i] == '\0') {
			PAN_length = i;
			break;
		}
	}

	printf("The length of PAN = %d\n", PAN_length);
	if (PAN_length > 19 || PAN_length < 16 || PAN_length == 0) {
		return WRONG_PAN;
	}
	else {
		return CARD_OK;
	}

	if (isValidCardPAN(cardData) == 0) {
		return CARD_OK;
	}
	else {
		printf("Wrong PAN number, it is not a Luhn number \n");
		return WRONG_PAN;
	}
}

