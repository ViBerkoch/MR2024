#include <stdio.h>
#include <math.h>
#include <locale.h>

#define YEARS 30
#define MONTHS_IN_YEAR 12

typedef struct {
    long long int capital;           // �������
    long long int salary;            // ��������
    long long int monthly_expenses;  // �������� ������� �� ���, ������������ � �.�.
    long long int rent;              // ������
    bool has_house;                  // ����, ���� ����� ��������
    float indexation_rate;           // ���������� ��������
    float mortgage_rate;             // ��������� ������
} Person;

typedef struct {
    float inflation_rate;            // ��������
    long long int house_cost;        // ��������� �������� � ������ ��������
    float deposit_rate;              // ������� �� �������
} Money;

Person Alice;
Person Bob;
Money money;

// ������� ��� ������� ����� ����� ���������� ��������
long long int apply_interest(long long int amount, float rate) {
    return amount * (1 + rate);
}

// ������������� ������ ������
void simulate_months(Person* person, const Money* money, int month) {
    // ��������� ����������� ��������
    person->capital -= person->monthly_expenses;

    // ���� � ���� ��� ��������, �� ������ ������
    if (!person->has_house) {
        person->capital -= person->rent;
    }

    // ��������� �������� ��������
    person->capital += person->salary / 12;

    // ��������� �������� �� ������� (�������)
    person->capital = apply_interest(person->capital, money->deposit_rate / 12);
}

// ������������� ������ ����
void simulate_years(Person* alice, Person* bob, Money* money) {
    for (int year = 1; year <= YEARS; year++) {
        // �������� ���������� �������� ��� �������
        for (int month = 1; month <= MONTHS_IN_YEAR; month++) {
            simulate_months(alice, money, month);
            simulate_months(bob, money, month);
        }

        // ��������� ���������� �������� � �������� �� ��������
        alice->monthly_expenses = apply_interest(alice->monthly_expenses, money->inflation_rate);
        bob->monthly_expenses = apply_interest(bob->monthly_expenses, money->inflation_rate);
        bob->rent = apply_interest(bob->rent, money->inflation_rate);

        // ���������� ��������� �������� �� ��������
        money->house_cost = apply_interest(money->house_cost, money->inflation_rate);

        // ���������� �������� ������� ���� ��������
        alice->salary = apply_interest(alice->salary, alice->indexation_rate);
        bob->salary = apply_interest(bob->salary, bob->indexation_rate);

        // ��������, ����� �� ��� ������ ��������
        if (!bob->has_house && bob->capital >= money->house_cost) {
            bob->capital -= money->house_cost;  // ��� �������� ��������
            bob->has_house = true;  // ������ � ���� ���� ��������
        }

        // ����� ���������� � �������� � ������� �������� �� ������ ����
        printf("Year %d:\n", year);
        printf("Alice's capital: %lld\n", alice->capital);
        printf("Bob's capital: %lld (Has house: %s)\n\n", bob->capital, bob->has_house ? "Yes" : "No");
    }
}

// ������� ��� ��������� ��������� � ������ ��������� �����������
void compare_strategies(Person* alice, Person* bob, Money* money) {
    // ������ �������� ��������� �������� � ������ ��������
    long long int final_house_cost = money->house_cost;

    // �������� ������� ����� �������� ��������� ��������
    long long int alice_final_capital = alice->capital + final_house_cost;

    // �������� ������� ���� �������� ��������� ��������, ���� �� � �����
    long long int bob_final_capital = bob->capital;
    if (bob->has_house) {
        bob_final_capital += final_house_cost;
    }

    printf("����� %d ���:\n", YEARS);
    printf("�������� ������� ����� (������� ��������� ��������): %lld\n", alice_final_capital);
    printf("�������� ������� ���� (������� ��������� ��������): %lld\n", bob_final_capital);

    // ����� ����������, ��� ��������� ���� ����� �����������
    if (alice_final_capital > bob_final_capital) {
        printf("��������� ����� (������� � �������) ��������� ����� �����������.\n");
    }
    else if (alice_final_capital < bob_final_capital) {
        printf("��������� ���� (���������� � �������) ��������� ����� �����������.\n");
    }
    else {
        printf("��� ��������� ��������� �������������.\n");
    }
}

// ������������� ��������� ������ ��� �����, ���� � ���������� ���������
void initialization() {
    money = {
        0.05,               // �������� 5% (�������� ��� ������� ��������������)
        13 * 1000000,       // ��������� �������� 13 ���
        0.20                // ������� 20%
    };

    // ����� �������� � �������
    Alice = {
        1000000,            // ��������� �������
        200000,             // ��������
        20000,              // �������� �������
        0,                  // ������ �����������
        true,               // ���������� ���� �������� (�������)
        0.06,               // ���������� �������� 6%
        0.16                // ��������� ������ 16%
    };

    // ��� �������� � �����
    Bob = {
        1000000,            // ��������� �������
        200000,             // ��������
        20000,              // �������� �������
        10000,              // �������� ������
        false,              // ��� ��������
        0.06,               // ���������� �������� 6%
        0                   // ������� �����������
    };
}

int main() {
    setlocale(LC_ALL, "Rus");

    initialization();

    simulate_years(&Alice, &Bob, &money);
    compare_strategies(&Alice, &Bob, &money);

    return 0;
}
