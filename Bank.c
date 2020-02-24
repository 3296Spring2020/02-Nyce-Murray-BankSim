#include "Bank.h"

Bank *Bank_new(int numAccounts, int initialBalance)
{
    Bank *b = (Bank *)malloc(sizeof(Bank));
    b->initialBalance = initialBalance;
    b->numAccounts = numAccounts;
    b->ntransacts = 0;
    b->accounts = (Account **)malloc(numAccounts * sizeof(Account *));
    for (int i = 0; i < numAccounts; ++i)
    {
        b->accounts[i] = Account_new(i, initialBalance);
    }
    pthread_mutex_t lk = PTHREAD_MUTEX_INITIALIZER;
    b->bankLock = lk;
    pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
    b->testcond = cond;
    b->testing = 0;

    return b;
}

void Bank_destroy(Bank *b)
{
    for (int i = 0; i < b->numAccounts; ++i)
    {
        Account_destroy(b->accounts[i]);
    }
    free(b);
}

void Bank_open(Bank *b)
{
    for (int i = 0; i < b->numAccounts; ++i)
    {
        TransferThreadParameters *params = TransferThreadParameters_new(b, i, b->initialBalance);
        pthread_create(&b->accounts[i]->thread, NULL, transfer_thread, params);
    }

    for (int i = 0; i < b->numAccounts; ++i)
    {
        pthread_join(b->accounts[i]->thread, NULL);
    }

    Bank_test(b);

    puts("Bank is finished running.");
}

void Bank_transfer(Bank *b, int from, int to, int amount)
{
    // Uncomment line when race condition in Bank_test() has been resolved.
    if (Bank_shouldTest(b))
    {
        Bank_createTester(b);
    }

    // pthread_mutex_lock(&b->bankLock);
    // if (b->testing)
    // {
    //     pthread_cond_wait(&b->testcond, &b->bankLock);
    // }
    // pthread_mutex_unlock(&b->bankLock);
    // printf("Bank lock acquired for transfer from %d to %d.\n", from, to);
    Account_withdraw(b->accounts[from], amount);
    Account_deposit(b->accounts[to], amount);
}

void Bank_createTester(Bank *b)
{
    pthread_t *bankTester = (pthread_t *)malloc(sizeof(pthread_t));
    pthread_create(bankTester, NULL, Bank_test, b);
}

void *Bank_test(void *bank)
{
    Bank *b = (Bank *)bank;
    int sum = 0;

    // critical section
    // pthread_mutex_lock(&b->bankLock);
    for (int i = 0; i < b->numAccounts; ++i)
    {
        pthread_mutex_lock(&b->accounts[i]->accountlock);
    }
    for (int i = 0; i < b->numAccounts; ++i)
    {
        pthread_t currentThreadId = pthread_self();
        Account *currAccount = b->accounts[i];
        printf("%lu Account[%d] balance %d\n", pthread_self(), currAccount->id, currAccount->balance);
        sum += b->accounts[i]->balance;
    }

    // pthread_cond_broadcast(&b->testcond);
    // pthread_mutex_unlock(&b->bankLock);
    // end section
    printf("%lu Sum: %d\n", pthread_self(), sum);
    if (sum != b->numAccounts * b->initialBalance)
    {
        printf("%lu Money was gained or lost!\n", pthread_self());
        exit(0);
    }
    else
    {
        printf("%lu The bank is in balance!\n", pthread_self());
    }
    for (int i = 0; i < b->numAccounts; ++i)
    {
        pthread_mutex_unlock(&b->accounts[i]->accountlock);
    }
}

int Bank_shouldTest(Bank *b)
{
    b->ntransacts++;
    return b->ntransacts % NTEST == 0;
}