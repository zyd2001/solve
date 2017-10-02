#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "utility.h"

enum state_tag {
    NUMBER,
    X,
    POWER,
    ADD,
    SUB,
    MUL,
    DIV,
    NUL
};

struct var {
    double x1;
    double x2;
    double constant;
};

struct ans {
    double x1;
    double x2;
    int flag;
};

struct var *parse();
struct ans *solve(struct var *variables);
int state_change(enum state_tag *current, enum state_tag new, UTF8_String *string_buffer, double *temp, struct var *variables, char ch);
double eval(char *str);
enum state_tag store = NUL;
int flag = 1;

int main()
{
    struct var *variables;
    struct ans *answer;
    for (;;)
    {
        flag = 1;
        store = NUL;
        printf(">>> ");
        variables = parse();
        // printf("x1 -> %lf\n", variables->x1);
        // printf("x2 -> %lf\n", variables->x2);
        // printf("constant -> %lf\n", variables->constant);
        answer = solve(variables);
        if (answer->flag == 0)
            printf("%lf\n", variables->constant);
        else if (answer->flag == 1)
            printf("The root of the function x = %lf\n", answer->x1);
        else if(answer->flag == 2)
        {
            printf("The first root of the function x1 = %lf\n", answer->x1);            
            printf("The second root of the function x2 = %lf\n", answer->x2);            
        }
        free(variables);
        free(answer);
    }
    return 0;
}

struct var *parse()
{
    struct var *new = (struct var*)calloc(1, sizeof(struct var));
    double temp = 0;
    enum state_tag state = NUL;
    char ch = 0;
    UTF8_String *string_buffer = utf8_string_new();
    for (;;)
    {
        ch = getchar();
        if (ch == EOF)
            exit(0);
        else if (ch == ' ')
            continue;
        else if (ch == '\n')
        {
            state_change(&state, NUL, string_buffer, &temp, new, ch);
            break;
        }
        else if (ch == 'e' || ch == 'q')
            exit(0);
        else
        {
            if (ch >= '0' && ch <= '9' || ch == '.')
                state_change(&state, NUMBER, string_buffer, &temp, new, ch);
            switch (ch)
            {
                case '=':
                    state_change(&state, NUL, string_buffer, &temp, new, ch);
                    flag = -1;
                    break;
                case '^':
                    state_change(&state, POWER, string_buffer, &temp, new, ch);
                    break;
                case '+':
                    state_change(&state, ADD, string_buffer, &temp, new, ch);
                    break;
                case '-':
                    state_change(&state, SUB, string_buffer, &temp, new, ch);
                    break;
                case '*':
                    state_change(&state, NUMBER, string_buffer, &temp, new, ch);
                    break;
                case '/':
                    state_change(&state, NUMBER, string_buffer, &temp, new, ch);
                    break;
                case 'x':
                    state_change(&state, X, string_buffer, &temp, new, ch);
            }
        }
    }
    return new;
}

struct ans *solve(struct var *variables)
{
    struct ans *answer = (struct ans*)calloc(1, sizeof(struct ans));
    if (variables->x1 == 0 && variables->x2 == 0)
    {
        answer->flag = 0;
        return answer;
    }
    else if (variables->x2 == 0)
    {
        answer->flag = 1;
        answer->x1 = -variables->constant / variables->x1;
    }
    else
    {
        double a = variables->x2;
        double b = variables->x1;
        double c = variables->constant;
        answer->flag = 2;
        answer->x1 = (-b + sqrt(pow(b, 2) - (4 * a * c))) / (2 * a);
        answer->x2 = (-b - sqrt(pow(b, 2) - (4 * a * c))) / (2 * a);
    }
    return answer;
}

int state_change(enum state_tag *current, enum state_tag new, UTF8_String *string_buffer, double *temp, struct var *variables, char ch)
{
    int f = 1;
    switch (new)
    {
        case NUMBER:
            if (*current != POWER)
                utf8_string_append_char(string_buffer, ch);
            else
                return 0;
            break;
        case X:
            break;
        case POWER:
            break;
        default:
            if (store == SUB)
                f = -1;
            switch (*current)
            {
                case NUMBER:
                    variables->constant += eval(string_buffer->value) * flag * f;
                    utf8_string_reassign(string_buffer, "");
                    break;
                case X:
                    if (strlen(string_buffer->value))
                    {
                        variables->x1 += eval(string_buffer->value) * flag * f;
                        utf8_string_reassign(string_buffer, "");
                    }
                    else
                        variables->x1 += 1 * flag * f;
                    break;
                case POWER:
                    if (strlen(string_buffer->value))
                    {
                        variables->x2 += eval(string_buffer->value) * flag * f;
                        utf8_string_reassign(string_buffer, "");
                    }
                    else
                        variables->x2 += 1 * flag * f;
                    break;       
            }
            break;
    }
    if (new == ADD || new == SUB || new == NUL)
        store = new;
    *current = new;
}

double eval(char *str)
{
    enum state_tag state = NUL;
    double temp;
    char *endptr;
    for (;;)
    {
        switch (state)
        {
            case MUL:
                temp *= strtof(str, &endptr);
                break;
            case DIV:
                temp /= strtof(str, &endptr);
                break;
            default:
                temp = strtof(str, &endptr);
                break;
        }
        if (strlen(endptr))
        {
            switch(endptr[0])
            {
                case '*':
                    state = MUL;
                    break;
                case '/':
                    state = DIV;
                    break;
            }
            str = endptr + 1;
            continue;
        }
        break;
    }
    return temp;
}