#include <stdlib.h>
#include <stdio.h>
#include <string.h>

const char *complaints[999] = {NULL};
const char *complaints_file = NULL;
const char *admin_password = NULL;

void captcha() {
	const char ops[] = {'+', '-', '*'};
	int a = rand() % 100;
	int b = rand() % 100;
	int c = rand() % 100;
	char o1 = ops[rand() % 3];
	char o2 = ops[rand() % 3];
	printf("Captcha: %d %c %d %c %d = ", a, o1, b, o2, c);

	char *response = fgets(malloc(100), 100, stdin);

	int result = a;
	switch(o1) {
		case '+': result += b; break;
		case '-': result -= b; break;
		case '*': result *= b; break;
	}
	switch(o2) {
		case '+': result += c; break;
		case '-': result -= c; break;
		case '*': result *= c; break;
	}
	
	if(result != atoi(response)) {
		puts("No automated complaints!");
		exit(0);
	}
	free(response);
}

void dump() {
	const char **complaint = complaints;
	while(*complaint != NULL) {
		printf("%s", *complaint);
		complaint++;
	}
}

void complain() {
	const char **complaint = complaints;
	int counter = 0;
	while(*complaint != NULL) {
		complaint++;
		counter++;
	}
	if(counter > 10)
		captcha();
	printf("> ");
	*complaint = fgets(malloc(100), 100, stdin);
}

void save_complaints() {
	FILE *file = fopen(complaints_file, "a");
	const char **complaint = complaints;
	while(*complaint != NULL) {
		fprintf(file, "%.100s\n", *complaint);
		complaint++;
	}
}

void admin() {
	printf("> ");
	char *buf = fgets(malloc(100), 100, stdin);
	if(strcmp(buf, admin_password) == 0) {
		complaints_file = "/tmp/complaints";
	} else {
		puts("You're not an admin!");
		exit(0);
	}
	free(buf);
}

int main() {
	setbuf(stdin, 0);
	setbuf(stdout, 0);
	FILE *flag = fopen("./flag", "r");
	admin_password = fgets(malloc(100), 100, flag);
	fclose(flag);
	
	puts("Welcome to the complaints desk, where we appropriately handle your feedback.");
	complaints_file = "/dev/null";
	
	while(1) {
		puts("");
		puts("[w]rite a complaint");
		puts("[r]eview your complaints");
		puts("[q]uit");
		printf("> ");
		char *menu = fgets(malloc(100), 100, stdin);
		switch(menu[0]) {
			case 'w': complain(); break;
			case 'r': dump(); break;
			case 'a': admin(); break;
			case 'q': save_complaints(); puts("Goodbye!"); return 0;
			default: puts("Invalid option!"); break;
		}
		free(menu);
	}
}