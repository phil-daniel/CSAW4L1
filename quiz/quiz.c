#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// Condition variable and mutex lock global variables
pthread_cond_t condition = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
// Which question you are on?
bool timeOver = false;
bool waitingForResponse = false;

struct question {
    char *question;
    char *answer;
};
typedef struct question question_t;

struct ask_args {
    int score;
    question_t *question;
};
typedef struct ask_args ask_args_t;

void *timer(void *args) {
    sleep(5);
    printf("5s have passed\n");
    // Signals the condition
    pthread_mutex_lock(&lock);
    timeOver = true;
    pthread_cond_signal(&condition);
    pthread_mutex_unlock(&lock);
    //
    pthread_exit(NULL);
}

void *ask(void *args) {
    ask_args_t *ask_args = args;

    printf("%s? ", ask_args->question->question);

    char input[128];
    fgets(input, 128, stdin);

    // strip newline
    input[strcspn(input, "\n")] = '\0';

    if (!strcmp(input, ask_args->question->answer)) {
        printf("Correct!\n");
        ask_args->score++;

    } else {
        printf("Incorrect :-(\n");
    }

    pthread_mutex_lock(&lock);
    waitingForResponse = false;
    pthread_cond_signal(&condition);
    pthread_mutex_unlock(&lock);

    pthread_exit(NULL);
}

int main(int argc, char const *argv[]) {

    question_t questions[] = {
        {.question = "3*2", .answer = "6"},
        {.question = "50/10", .answer = "5"},
        {.question = "2+1+1+1", .answer = "5"},
        {.question = "3^3", .answer = "27"},
        {.question = "3+3", .answer = "6"},
        {.question = "4/2", .answer = "2"}};

    pthread_t timer_thread;
    if (pthread_create(&timer_thread, NULL, timer, NULL)) {
        printf("Error creating timer thread.\n");
    }

    int score = 0;
    int questionsAsked = 0;

    while (questionsAsked < 6 && !timeOver) {
        ask_args_t ask_args = {.score = score, .question = &questions[questionsAsked]};

        pthread_t ask_thread;
        if (pthread_create(&ask_thread, NULL, ask, &ask_args)) {
            printf("Error creating asker thread.\n");
        }

        waitingForResponse = true;
        pthread_mutex_lock(&lock);
        while (waitingForResponse && !timeOver) {
            pthread_cond_wait(&condition, &lock);
        }
        pthread_mutex_unlock(&lock);

        if (!timeOver) {
            score = ask_args.score;
        }
        questionsAsked++;
    }

    // while question asked < 5
    /*
    for (int i = 0; i < 6; ++i) {


        ask_args_t ask_args = {.score = score, .question = &questions[i]};

        pthread_t ask_thread;
        if (pthread_create(&ask_thread, NULL, ask, &ask_args)) {
            printf("Error creating asker thread.\n");
        }

        if (pthread_join(ask_thread, NULL)) {
            printf("Error joining asker thread.\n");
        }
        score = ask_args.score;
    }
    */

    printf("End of questions, final score %d\n", score);
    return 0;
}
