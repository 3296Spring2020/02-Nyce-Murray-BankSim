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
    return a;
}

void Account_destroy(Account *a) { free(a); }

void Account_deposit(Account *a, int amount)
{
    pthread_mutex_lock(&a->accountlock);
    printf("Balance before deposit for acc %d:\t%d.\n", a->id, a->balance);
    a->balance += amount;
    pthread_cond_signal(&a->lowfunds);
    printf("Deposited %d to acc %d.\n", amount, a->id);
    printf("Balance after deposit for acc %d:\t%d.\n", a->id, a->balance);
    pthread_mutex_unlock(&a->accountlock);
}

int Account_withdraw(Account *a, int amount)
{
    if (a->balance >= amount)
    {
        printf("Balance before withdraw for acc %d\t%d.\n", a->id, a->balance);
        a->balance -= amount;
        printf("Withdrew %d from acc %d.\n", amount, a->id);
        printf("Balance after withdraw for acc %d\t%d.\n", a->id, a->balance);
        return 1;
    }
    return -1;
}
