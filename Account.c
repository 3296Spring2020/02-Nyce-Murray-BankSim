#include "Account.h"

Account *Account_new(int id, int initialBalance) {
    Account *a = (Account *)malloc(sizeof(Account));
    a->id = id;
    a->balance = initialBalance;
    //a->accountlock = NULL;
    pthread_mutex_t newlock = PTHREAD_MUTEX_INITIALIZER;
    a->accountlock = newlock;
    return a;
}

void Account_destroy(Account *a) {
    free(a);
}

void Account_deposit(Account *a, int amount) {
    pthread_mutex_lock(&a->accountlock);
    int newBalance = a->balance + amount;
    a->balance = newBalance;
    pthread_mutex_unlock(&a->accountlock);
}

int Account_withdraw(Account *a, int amount) {
    pthread_mutex_lock(&a->accountlock);
    if(amount <= a->balance) {
        int newBalance = a->balance - amount;
        a->balance = newBalance;
        pthread_mutex_unlock(&a->accountlock);
        return 1;
    } else {
        pthread_mutex_unlock(&a->accountlock);
        return 0;
    }
}
