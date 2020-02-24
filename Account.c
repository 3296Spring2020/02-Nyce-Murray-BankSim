#include "Account.h"

Account *Account_new(int id, int initialBalance)
{
    Account *a = (Account *)malloc(sizeof(Account));
    a->id = id;
    a->balance = initialBalance;
    // a->accountlock = NULL;
    pthread_mutex_t newlock = PTHREAD_MUTEX_INITIALIZER;
    a->accountlock = newlock;
    pthread_cond_t newcond = PTHREAD_COND_INITIALIZER;
    a->lowfunds = newcond;
    a->fundsneeded = 0;
    return a;
}

void Account_destroy(Account *a) { free(a); }

void Account_deposit(Account *a, int amount)
{
    pthread_mutex_lock(&a->accountlock);
    int newBalance = a->balance + amount;
    a->balance = newBalance;
    if ((a->balance >= a->fundsneeded))
    {
        pthread_cond_signal(&a->lowfunds);
    }
    pthread_mutex_unlock(&a->accountlock);
}

int Account_withdraw(Account *a, int amount)
{
    if (a->balance >= amount)
    {
        int newBalance = a->balance - amount;
        a->balance = newBalance;
        a->fundsneeded = 0;
        return 1;
    }
    a->fundsneeded = amount;
    return 0;
}
