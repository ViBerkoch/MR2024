#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <math.h>

typedef long long int Money;
const double inflation = 0.01;

typedef struct Person {
    Money bank;
	Money salary;
	Money home;
	Money downplayment;
	Money mortgage_pay;
	Money food_expenses;
	Money utility_expenses;
	Money other_expenses;
	Money rent;
	int duration;
	double deposit;
	char name[6];

}Person;
Person alice;
Person bob;

void alice_init() {
	alice.bank = 1000 * 1000 * 100;
	alice.salary = 250 * 1000 * 100;
	alice.home = 13 * 1000 * 1000 * 100;
	alice.downplayment = 1000 * 1000 * 100;
	alice.mortgage_pay = 16137084;
	alice.food_expenses = 15 * 1000 * 100;
	alice.utility_expenses = 8000 * 100;
	alice.other_expenses = 17000 * 100;
	alice.deposit = 0.2;
	alice.rent = 0;
	strcpy(alice.name, "Alice");
};

void bob_init() {
	bob.bank = 1000 * 1000 * 100;
	bob.salary = 250 * 1000 * 100;
	bob.home = 0;
	bob.downplayment = 0;
	bob.mortgage_pay = 0;
	bob.food_expenses = 15000 * 100;
	bob.utility_expenses = 8000 * 100;
	bob.other_expenses = 17000 * 100;
	bob.deposit = 0.2;
	bob.rent = 30 * 1000 * 100;
	strcpy(bob.name, "Bob");
};

void alice_income() {
	alice.bank += (Money)(alice.bank * alice.deposit / 12.0);
};

void alice_salary_indexation(const int year, const int month) {
	if (month == 12) {
		alice.salary += (Money)(alice.salary * inflation);
	}
}
void alice_salary(const int year, const int month) {
	if (year == 2027 && (month == 10 || month == 11)) {

	}

    else {
		alice.bank += alice.salary;
	} 
};

void alice_home() {
	alice.bank -= alice.mortgage_pay;
	alice.mortgage_pay += (Money)(alice.mortgage_pay * inflation / 12.0);
};

void alice_expenses() {
	alice.bank -=
		(alice.food_expenses + alice.utility_expenses + alice.other_expenses);
	alice.food_expenses += (Money)(alice.food_expenses * inflation / 12.0);
	alice.utility_expenses += (Money)(alice.utility_expenses * inflation);
	alice.other_expenses += (Money)(alice.other_expenses * inflation);
};

void alice_home_index(int month) {
	if (month == 12)
		alice.home += (Money)(alice.home * (inflation));
};

void bob_income() {
	bob.bank += (Money)(bob.bank * bob.deposit / 12.0);
}

void bob_salary(const int year, const int month) {
	if (month == 12)
		bob.salary += (Money)(bob.salary * inflation);
	bob.bank += bob.salary;
}

void bob_rent() {
	bob.bank -= bob.rent;
}

void bob_expenses() {
	bob.bank -=
		(bob.food_expenses + bob.utility_expenses + bob.other_expenses);
	bob.food_expenses += (Money)(bob.food_expenses * inflation / 12.0);
	bob.utility_expenses += (Money)(bob.utility_expenses * inflation);
	bob.other_expenses += (Money)(bob.other_expenses * inflation);
};

void simulation(int month, int year) {
	int temp_year = year;
	int temp_month = month;
	alice.bank = 0;

	while (!(year == (temp_year + 30) && month == temp_month)) {
		alice_salary(year, month);
		alice_salary_indexation(year, month);
		alice_home();
		alice_expenses();
		alice_income();
		alice_home_index(month);

		bob_salary(year, month);
		bob_rent();
		bob_expenses();
		bob_rent();
		bob_income();
		++month;

		if (month == 13) {
			month = 1;
			++year;
		}
	}
	alice.bank += alice.home;
};

void print_person(const Person person) {
	printf("%s:\n", person.name);
	printf("  bank = %lld rub\n", (Money)(person.bank / 100));
};

int main() {
	alice_init();
	bob_init();

	simulation(9, 2024);

	print_person(bob);
	print_person(alice);

	if (alice.bank + alice.home > bob.bank) {
		printf("\nAlice won!\n");
	}
	else if (alice.bank + alice.home < bob.bank) {
		printf("\nBob won!\n");
	}
	else {
		printf("\nDraw!\n");
	}

	return 0;
}
