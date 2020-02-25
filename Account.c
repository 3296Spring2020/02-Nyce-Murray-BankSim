#include "Account.h"

Account *Account_new(int id, int initialBalance)
{
    Account *a = (Account *)malloc(sizeof(Account));
    a->id = id;
    a->balance = initialBalance;
    // a->accountlock = NULL;
    pthread_mutex_t newlock = PTHREAD_MUTEX_INITIALIZER;
    a->accountlock = newlock;
    pthread_mutex_t newlock2 = PTHREAD_MUTEX_INITIALIZER;
    a->testlock = newlock2;
    pthread_cond_t newcond = PTHREAD_COND_INITIALIZER;
    a->lowfunds = newcond;
    return a;
}

void Account_destroy(Account *a) { free(a); }

void Account_deposit(Account *a, int amount)
{
    pthread_mutex_lock(&a->accountlock);
<<<<<<< HEAD
    a->balance += amount;
    pthread_cond_signal(&a->lowfunds);
    printf("Deposited %d to acc %d.\n", amount, a->id);
    pthread_mutex_unlock(&a->accountlock);
}

int Account_withdraw(Account *a, int amount)
{
    if (a->balance >= amount)
    {
        a->balance -= amount;
        printf("Withdrew %d from acc %d.\n", amount, a->id);
=======
    int newBalance = a->balance + amount;
    a->balance = newBalance;
    if((a->fundsneeded > 0) && (a->balance >= a->fundsneeded)) {
        a->fundsneeded = -1;
        pthread_cond_broadcast(&a->lowfunds);
    }
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
        //wait/notify condition here
        a->fundsneeded = amount;
        if((a->fundsneeded < a->balance) && (a->fundsneeded > 0)) {
            pthread_cond_wait(&a->lowfunds, &a->accountlock);
        }
        printf("a->balance: %d\n", a->balance);
        printf("a->fundsneeded: %d\n", a->fundsneeded);
        if(a->balance >= a->fundsneeded) {
            int newBalance = a->balance - amount;
            a->balance = newBalance;
        }else {
            pthread_mutex_unlock(&a->accountlock);
            return 0;
        }
        //TEST
        //if(a->balance < 0) printf("NEGATIVE BALANCE\n");
        pthread_mutex_unlock(&a->accountlock);
>>>>>>> c06064b1d849c3dd9ad84550f35026e577293f6b
        return 1;
    }

    return -1;
}
