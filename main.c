#include "server.h"

 ST_accountsDB_t accountsDB[255];
 ST_transaction_t transactionsDB[255];
 int account_index = -1;
 int transactionNumber = 0;


int main() {
	// openning the accounts database file
	FILE* AccountsDB_ptr;
	errno_t DB_file_err;
	DB_file_err = fopen_s(&AccountsDB_ptr, "AccountsDB.txt", "w");

	// checking openning the database file was done successfully 
	if (AccountsDB_ptr == NULL || DB_file_err != 0)
	{
		printf("Error! Could not open database file\n");
		exit(1);
	}

	//filling the datebase 
	for (int i = 0; i < 255; i++) {
		// filling the accounts with random balance and state
		fprintf(AccountsDB_ptr, "%d	", i + 1);
		float balance = (float)rand();
		fprintf(AccountsDB_ptr, "B %.4f	", balance);
		accountsDB[i].balance = balance;

		int x = rand() % 5;
		int s = 0;
		if (x == 0) {
			s = 1;
		}
		fprintf(AccountsDB_ptr, "sate %d	", s);
		accountsDB[i].state = s;

		//filling the first 18 PAN numbers randomly
		fprintf(AccountsDB_ptr, "PAN	");
		for (int j = 0; j < 18; j++) {
			int PAN_tmp = (rand() % 9);
			fprintf(AccountsDB_ptr, "%d", PAN_tmp);
			accountsDB[i].primaryAccountNumber[j] = 48 + PAN_tmp;
		}

		// calculating the 19th digit in PAN so it will be Luhn number
		int Luhn_digit = 0;
		int num = 0;
		int tmp = 0;
		for (int k = 17; k >= 0; k--) {
			num = (accountsDB[i].primaryAccountNumber[k]) - '0';
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

		fprintf(AccountsDB_ptr, "%d\n", Luhn_digit);
		accountsDB[i].primaryAccountNumber[18] = Luhn_digit + '0';


		// filling the 20th PAN digit with NULL
		accountsDB[i].primaryAccountNumber[19] = '\0';
	}

	fclose(AccountsDB_ptr);
	// calling the application
	appStart();

	return 0;
}