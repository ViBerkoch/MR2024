#include <stdio.h>


typedef long long int Money;  // Тип данных для денежных переменных


typedef struct 
{
    int month;
    int year;
} Date;


typedef struct
{
    Money price;
    int area;
    Money rent;
} House;


typedef struct
{
    Money capital; // Сумма всех активов, пассивов и денег на банковском счёте
    Money bank_account; // Счёт в банке, который сразу кладётся на вклад
    double deposite_percent;
    Money salary;
    Money annual_bonus;  // Годовая премия
    Money food_payment;  // Ежемесечные траты за еду
    Money comunal_payment;  // Траты на комунальные услуги
    Money another_payment;  //Траты на одежду, медикаменты и прочее
    Money vacation_cost;  // Ежегодные траты на отпуск
    Money ipotek_payment;  // Ежемесечная плата за ипотеку
    double inflation;  // Сделать общей
    double indexation;  // Сделать общей
    int amount_of_flats;  // Изменить название 
    House houses[10];  // Дома, которыми владеет 
    House desired_house;  // Дом, который желает купить следующим
} Hero;


House one_bedroom_apartment = {
    .price = 12 * 1000 * 1000 * 100, 
    .area = 60, 
    .rent = 30 * 1000 * 100};  // 2-х комнатная квартира
House two_berdroom_apartment = {
    .price = 16 * 1000 * 1000 * 100, 
    .area = 85, 
    .rent = 50 * 1000 * 100};  // 3-х комнатная квартира
House country_house = {
    .price = 15 * 1000 * 1000 * 100, 
    .area = 130, 
    .rent = 60 * 1000 * 100};  // Загородний дом
House penthouse = {
    .price = (Money)350 * 1000 * 1000 * 100, 
    .area = 380, 
    .rent = 800 * 1000 * 100};  // Пентхаус


House catalog[4];  // Каталог видов жилья
const int catalog_size = 4; 


void init_catalog()  // Инициализация каталога
{
    catalog[0] = one_bedroom_apartment;
    catalog[1] = two_berdroom_apartment; 
    catalog[2] = country_house;
    catalog[3] = penthouse;
    // увеличть в ручную размер каталога, при добавлении нового типа жилья
}


Hero midas;  // Мидас выбрал не заморачиваться и взять Ипотеку
Hero arno;  // Арно живёт в аренду и копит деньги


const int CURRENT_YEAR = 2024;
const int MONTH = 12;
double inflation = 0.07;  
Money comunal_per_area = 2 * 1000 * 100;  // Цена комунальных услуг за метр квадратный

void init_midas()  // Инициализация переменных Мидаса
{
    midas = (Hero){   
        .houses[0] = one_bedroom_apartment,
        .bank_account = 0,
        .salary = 300 * 1000 * 100,
        .annual_bonus = 100 * 1000 * 100,
        .food_payment = 20 * 1000 * 100,
        .comunal_payment = midas.houses[0].area * comunal_per_area,  // Зависит от площади
        .another_payment = 35 * 1000 * 100,
        .vacation_cost = 300 * 1000 * 100,
        .ipotek_payment = 200 * 1000 * 100,
        .deposite_percent = 0.18,
        .indexation = 0.07,
        .amount_of_flats = 1, 
        .desired_house = one_bedroom_apartment  
    };
}


void init_arno()  // Инициализация Арно 
{
    arno = (Hero){
        .houses[0] = one_bedroom_apartment,
        .bank_account = 1000 * 1000 * 100,
        .salary = 200 * 1000 * 100,
        .annual_bonus = 300 * 1000 * 100,  // Премия в конце года
        .food_payment = 25 * 1000 * 100 ,
        .comunal_payment =  arno.houses[0].area * comunal_per_area,  // Зависит от площади
        .another_payment = 45 * 1000 * 100,
        .vacation_cost = 250 * 1000 * 100,
        .deposite_percent = 0.18,
        .indexation = 0.07,
        .amount_of_flats = 0,
        .desired_house = one_bedroom_apartment
    };
}


// Написать функцию для покупки квартир, дач и недвижимостей
void house_operations(Hero* hero)  // Основаня функция для работы с домами
{
    
}


void buy_house(Hero* hero)
{
    hero -> bank_account -= hero -> desired_house.price;
    hero -> houses[hero->amount_of_flats] = hero->desired_house;
    hero -> amount_of_flats += 1;
}


House house_desire(Hero* hero)  // Функция для выбора покупки следующего жилья
{
    if (hero->amount_of_flats == 0){  // Добавить комментариии к условиям выбора квартиры 
        double best[2] = {1000000000, 0};
        for (int i = 0; i < 2; i++){
            double efficient[2] = {(double)catalog[i].price / catalog[i].area, (double)i};
            if (efficient[0] > best[0])
            {
                best[0] = efficient[0];
                best[1] = efficient[1];
            }
        }
        return catalog[(int)best[1]];
    }
    else if(hero->amount_of_flats < 8){
        double best[2] = {1000000000, 0};
        for (int i = 0; i < catalog_size; i++){
            double efficient[2] = {(double)catalog[i].price / catalog[i].area + (double)catalog[i].price / (catalog[i].rent / 1000 * 100) , (double)i};
            if (efficient[0] <= best[0])
            {
                best[0] = efficient[0];
                best[1] = efficient[1];
            }
        }
        return catalog[(int)best[1]];
    } 
    else{
        double best[2] = {100000000000, 0};
        for (int i = 0; i < catalog_size; i++){
            double efficient[2] = {(double)catalog[i].price / catalog[i].area, (double)i};
            if (catalog[i].area > 250 && efficient[0] < best[0]){
                best[0] = efficient[0];
                best[1] = efficient[1];
            }
        }
        if (best[0] == 100000000000){
            return hero->desired_house;
        }
        else
        {
            return catalog[(int)best[1]];
        }
    }
}


void house_misery(Hero* hero)  // Нужна ли в данный момент квартира
{
    if(hero->amount_of_flats == 0 && hero->bank_account > (Money)(hero->desired_house.price * 1.2)){
        buy_house(&*hero);
        hero->desired_house = house_desire(&*hero);
    }
    else if (hero->amount_of_flats > 0 && hero->bank_account > (Money)hero->desired_house.price * 1.2 
    && (hero->bank_account * hero->deposite_percent) < (hero->desired_house.rent * 12) && hero->amount_of_flats < 8)
    {
        buy_house(&*hero);
        hero->desired_house = house_desire(&*hero);
    }
    else if (hero->amount_of_flats >= 8 && hero->bank_account > hero->desired_house.price * 3){
        buy_house(&*hero);
        hero->desired_house = house_desire(&*hero);
    }
}


// Написать функцию для изменения процентов вклада, инфляции и индексации
// Написать функцию для смены работы
// Написть функцию для получения отпуска
// Написать функцию покупки машины
// Написать функцию домашних животных с рандомайзером
// Написать функцию для семьи и детей (В пользу Мидаса)
// Написать функцию для расчёта ипотеки и поиска размера квартиры


void global_inflation(const int current_month)  // Дорожание товаров, которыми не пользуются (происходит в январе)
{
    if (current_month == 11)
    {
        for (int i = 0; i < catalog_size; i++){
            catalog[i].price += (Money)(catalog[i].price * inflation);
            catalog[i].rent += (Money)(catalog[i].rent * inflation);   
        }
    }
    
}


void personal_inflation_cost(Hero* hero, const int current_month)  // Инфляция цен которыми пользуется Героей (происходит в январе)
{
    if (current_month == 1){
        hero -> food_payment += (Money)(hero -> food_payment * inflation);
        hero -> comunal_payment += (Money)(hero -> comunal_payment * inflation); 
        hero -> another_payment += (Money)(hero -> another_payment * inflation); 
        hero -> vacation_cost += (Money)(hero -> vacation_cost * inflation);  
        hero -> houses[0].price += (Money)(hero -> houses[0].price * inflation);
        hero -> houses[0].rent += (Money)(hero -> houses[0].rent * inflation);
    }
}


void personal_indexation(Hero* hero, const int current_month)  // Индексация зарплат и премий (происходит в октябре)
{
    if (current_month == 10)
    {
        hero -> salary += (Money)(hero -> salary * hero -> indexation);
        hero -> annual_bonus += (Money)(hero -> annual_bonus * hero -> indexation);
    }
    
}


void account_recount(Hero* hero, const int current_month)
{
    hero->bank_account += (Money)((double)(hero->bank_account) * (hero->deposite_percent / 12));
    hero->bank_account += hero->salary;
    hero->bank_account -= hero->food_payment;
    hero->bank_account -= hero->comunal_payment;
    hero->bank_account -= hero->another_payment;  
    hero->bank_account -= hero->ipotek_payment;
    if (hero->amount_of_flats == 0)
    {
        hero->bank_account -= hero->houses[0].rent;
    }
    if (current_month == 12){
        hero -> bank_account += hero -> annual_bonus;
        hero -> bank_account -= hero -> vacation_cost;
    }
}


void capital_recount(Hero* hero, const int current_month) // Пересчёт суммы капитала
{
    hero->capital = (Money)(hero->bank_account); 
    for (int i = 0; i < hero->amount_of_flats; i++){
        hero->capital +=(Money)hero->houses[i].price;
    }
}


void next_month(Date* current_date)  // Наступление следующего месяца
{
    current_date -> month += 1;
    if (current_date -> month >= 13){
            current_date -> month = 1;
            current_date -> year += 1;
    }
}


void simulation(Hero* hero, const int start_month, const int final_year)  // Симмуляция на до определённого года и того месяца
{
    Date current_date = {
        .month = start_month, 
        .year = CURRENT_YEAR};
    Date final_date = {
        .month = start_month,
        .year = final_year};

    while (current_date.year < final_date.year || current_date.month < final_date.month){
        account_recount(&*hero, current_date.month);
        personal_inflation_cost(&*hero, current_date.month);
        personal_indexation(&*hero, current_date.month); 
        global_inflation(current_date.month);

        capital_recount(&*hero, current_date.month);

        next_month(&current_date);
    }
}


void final_conclusion(){

    printf("Midas capital %lld\n", (midas.capital) / 100);  // Вывод капитаа Мидаса
    printf("Arno capital %lld\n", arno.capital / 100);  // Вывод капитала Арно
    for (int i = 0; i < catalog_size; i++){
        printf("price %lld,   rent %lld\n",catalog[i].price / 100, catalog[i].rent / 100); // Цена квартир спустя 30 лет
    } 
    printf("\n");
}


int main()
{
    // Инициализируем параметры Героев и каталога
    init_midas();
    init_arno();
    init_catalog();

    // Запускаем симуляцию до 2054 года
    simulation(&midas, 9, 2054); // Сделать одну симуляцию, нужно добавить ввод обоих героев за раз.
    simulation(&arno, 9, 2054);

    final_conclusion();  // Итоговый вывод
   /*
    printf("%lld\n", catalog[0].price);
    printf("%lld\n", catalog[1].price);
    printf("%lld\n", catalog[2].price);
    printf("%lld\n", catalog[3].price);
    printf("%lld\n", house_desire(&midas).price);
    printf("%lld\n", house_desire(&arno).price);
    */
    return 1;
    
}
