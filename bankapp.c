/*This program is  banking application that provides three features: it can ass
 * an account number, make a deposit and make a withdrawal. All of the
 * banking information is stored in a csv file.
 * Author: Anastasia Douka
 * Department of Economics, Faculty of Arts.
 * date : 2020-03-22 Started writing the program, defined the constand variables.
 * date : 2020-03-25 Defined the functions i would use.
 * date : 2020-03-26 Wrote the body of the functions.
 * date : 2020-04-02 finalised the program and added return codes.
 */

#include <stdio.h> //for stderr, fprintf
#include <string.h> //for strcmp
#include <stdlib.h> //for exit

#define OK 0

//error codes for argument checking
#define ERROR 1
#define ADD 2
#define ADD_ERROR 3
#define DEPOSIT 4
#define DEPOSIT_ERROR 5
#define WITHDRAW 6
#define WITHDRAW_ERROR 7

#define AMOUNT_DIGITS 13

/*
 * checks whether console arguments are passed to main as expected
 * returns the command to use
 */
int check_arguments(int argc, char* argv[]);

/*
 * opens file_name for reading and writing, only if it exists
 * return the FILE pointer
 */
FILE* check_file(char* file_name);

/*
 * adds account to file, returns exit code
 */
int add(FILE* file, char* account_number, char* full_name);

/*
 * adds a deposit transaction record
 */
int deposit(FILE* file, char* account_number, char* date, char* amount);

/*
 * facilitate withdrawals
 */
int withdraw(FILE* file, char* account_number, char* date, char* amount);

/*
 * write string date to file
 * exists 13 if something wrong was during file writting procedure
 */
void write_to_file(FILE* file, char* data);

int main (int argc, char* argv[]) {
	int return_code = check_arguments(argc, argv);
	if (return_code == ERROR ||
			return_code == ADD_ERROR || return_code == WITHDRAW_ERROR || return_code == DEPOSIT_ERROR) {
		return 1;
	}
	FILE* file = check_file("bankdata.csv");
	if (file == NULL) {
		return 100;
	}
	if (return_code == ADD) {
		return add(file, argv[2], argv[3]);
	}
	if (return_code == DEPOSIT) {
		return deposit(file, argv[2], argv[3], argv[4]);
	}
	if (return_code == WITHDRAW) {
		return withdraw(file, argv[2], argv[3], argv[4]);
	}

	fclose(file);
	return 0;
}

int check_arguments(int argc, char* argv[]) {
	if (argc > 1) { //arguments provided
		if (strcmp(argv[1], "-a") == 0) {
			if (argc == 4) {
				return ADD;
			} else {
				fprintf(stderr, "Error, incorrect usage!\n");
				fprintf(stderr, "-a ACCTNUM NAME\n");
				return ADD_ERROR;
			}
		}
		if (strcmp(argv[1], "-d") == 0) {
			if (argc == 5) {
				return DEPOSIT;
			} else {
				fprintf(stderr, "Error, incorrect usage!\n");
				fprintf(stderr, "-d ACCTNUM DATE AMOUNT\n");
				return DEPOSIT_ERROR;
			}
		}
		if (strcmp(argv[1], "-w") == 0) {
			if (argc == 5) {
				return WITHDRAW;
			} else {
				fprintf(stderr, "Error, incorrect usage!\n");
				fprintf(stderr, "-w ACCTNUM DATE AMOUNT\n");
				return WITHDRAW_ERROR;
			}
		}
	}
	fprintf(stderr, "Error, incorrect usage!\n");
	fprintf(stderr, "-a ACCTNUM NAME\n");
	fprintf(stderr, "-d ACCTNUM DATE AMOUNT\n");
	fprintf(stderr, "-w ACCTNUM DATE AMOUNT\n");
	return ERROR;
}

FILE* check_file(char* file_name) {
	FILE* file = fopen(file_name, "r+"); //r+ for read and write and check if exists
	if (file == NULL) {
		fprintf(stderr, "Error, unable to locate the data file bankdata.csv\n");
	}
	return file;
}

int add(FILE* file, char* account_number, char* full_name) {
	size_t n = 0;
	char* line = NULL;
	int items_read;
	do {
		items_read = getline(&line, &n, file);
		if (items_read != -1 ) {
			char* token = strtok(line, ","); //get command
			if (strcmp(token, "AC") == 0) {
				token = strtok(NULL, ","); //get account number
				if (strcmp(token, account_number) == 0) {
					fprintf(stderr, "Error, account number %s already exists\n", account_number);
					return 50;
				}
			}
		}
	} while(items_read != -1);

	//write AC,
	write_to_file(file, "AC,");

	//write account number
	write_to_file(file, account_number);

	//write comma
	write_to_file(file, ",");

	//write full name
	write_to_file(file, full_name);

	//write \n
	write_to_file(file, "\n");

	return OK;
}

int deposit(FILE* file, char* account_number, char* date, char* amount) {
	size_t n = 0;
	char* line = NULL;
	int items_read;
	int account_found = 0;
	do {
		items_read = getline(&line, &n, file);
		if (items_read != -1 ) {
			char* token = strtok(line, ","); //get command
			if (strcmp(token, "AC") == 0) {
				token = strtok(NULL, ","); //get account number
				if (strcmp(token, account_number) == 0) {
					account_found = 1;
				}
			}
		}
	} while(items_read != -1);

	if (account_found == 0) {
		fprintf(stderr, "Error, account number %s does not exists\n", account_number);
		return 50;
	}

	//write TX,
	write_to_file(file, "TX,");

	//write account number
	write_to_file(file, account_number);

	//write comma
	write_to_file(file, ",");

	//write date
	write_to_file(file, date);

	//write comma
	write_to_file(file, ",");

	//write amount
	write_to_file(file, amount);

	//write \n
	write_to_file(file, "\n");

	return OK;
}

int withdraw(FILE* file, char* account_number, char* date, char* amount) {
	size_t n = 0;
	char* line = NULL;
	int items_read;
	int account_found = 0;
	float balance = 0;
	do {
		items_read = getline(&line, &n, file);
		if (items_read != -1 ) {
			char* token = strtok(line, ","); //get command
			if (strcmp(token, "AC") == 0) {
				token = strtok(NULL, ","); //get account number
				if (strcmp(token, account_number) == 0) {
					account_found = 1;
				}
			} else if (strcmp(token, "TX") == 0) {
				token = strtok(NULL, ","); //get account number
				if (strcmp(token, account_number) == 0) {
					token = strtok(NULL, ","); //get date
					token = strtok(NULL, ","); //get amount
					balance = balance + atof(token);
				}
			}
		}
	} while(items_read != -1);

	if (account_found == 0) {
		fprintf(stderr, "Error, account number %s does not exists\n", account_number);
		return 50;
	}

	float withdrawal = atof(amount);
	if (withdrawal > balance) {
		fprintf(stderr, "Error, account number %s has only %.2f\n", account_number, balance);
		return 60;
	}

	//write TX,
	write_to_file(file, "TX,");

	//write account number
	write_to_file(file, account_number);

	//write comma
	write_to_file(file, ",");

	//write date
	write_to_file(file, date);

	//write comma
	write_to_file(file, ",-");

	//write amount
	write_to_file(file, amount);

	//write \n
	write_to_file(file, "\n");

	return OK;
}

void write_to_file(FILE* file, char* data) {
	//write data
	int items_written = fwrite(data, strlen(data), 1, file);
	if (items_written == 0) {
		fprintf(stderr, "Unexpected error occured during writting to file\n");
		fclose(file);
		exit(13);
	}
}
