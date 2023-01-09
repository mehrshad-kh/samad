//
//  mklib.c
//
//
//  Created by Mehrshad on 4/9/1401 AP.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "mklib.h"

bool Xor(bool condition1, bool condition2)
{
    if ((condition1 == true && condition2 == false) ||
        (condition1 == false && condition2 == true))
        return true;
    else
        return false;
}

void swapc(char *c1, char *c2)
{
    char temp = *c1;
    *c1 = *c2;
    *c2 = temp;
}

void swapi(int *n1, int *n2)
{
    int temp = *n1;
    *n1 = *n2;
    *n2 = temp;
}

bool issym(char c, int n, ...)
{
    va_list ap;
    
    va_start(ap, n);
    
    for (int i = 0; i < n; i++)
        if (c == va_arg(ap, int))
            return true;
    
    va_end(ap);
    
    return false;
}

void SolveQuadratic(double a, double b, double c, double result[2])
{
    double delta = pow(b, 2.0) - 4.0 * a * c;
    
    if (delta < 0.0)
    {
        result[0] = NAN;
        result[1] = NAN;
    }
    else
    {
        result[0] = (-b - sqrt(delta)) / (2 * a);
        result[1] = (-b + sqrt(delta)) / (2 * a);
    }
}

static inline uint8_t todgt(char c)
{
    return (c - 48);
}

static inline char tochar(uint8_t n)
{
    return (char)(n + 48);
}

char *GetGreater(const char *s1, const char *s2)
{
    size_t n1 = strlen(s1);
    size_t n2 = strlen(s2);
    int comparison_result = 0;
    
    if (n1 > n2)
        return (char *)s1;
    else if (n2 > n1)
        return (char *)s2;
    else
    {
        comparison_result = strcmp(s1, s2);
        
        if (comparison_result < 0)
            return (char *)s1;
        else
            return (char *)s2;
    }
}

void RemoveLeadingZero(char **str)
{
    size_t n = strlen(*str);
    
    if (*(*str) == '0')
    {
        for (size_t i = 0; i < n; i++)
            swapc(*str + i, *str + i + 1);
        *str = realloc(*str, n * sizeof(char));
        if (*str != NULL)
            *(*str + n - 1) = '\0';
        else
            perror("Insufficient memory at RemoveLeadingZero");
    }
}

void MultiplyNByOne(const char *op1, char op2, int order, char **res)
{
    size_t n1 = strlen(op1);
    size_t n = n1 + order + 1;
    uint8_t temp = 0;
    uint8_t adder = 0;
    
    *res = (char *)realloc(*res, (n + 1) * sizeof(char));
    
    if (*res != NULL)
    {
        for (size_t i = 0; i < n + 1; i++)
            *(*res + i) = '\0';
        
        for (size_t i = n - order; i < n; i++)
            *(*res + i) = '0';
        
        for (size_t i = n1; i >= 1; i--)
        {
            temp = todgt(op1[i - 1]) * todgt(op2);
            *(*res + i) = tochar((temp += adder) % 10);
            adder = (temp - temp % 10) / 10;
        }
        *(*res) = tochar(adder);
        
        RemoveLeadingZero(res);
    }
    else
        perror("Insufficient memory at MultiplyNByOne");
}

void AddNToN(const char *op1, const char *op2, char **res)
{
    size_t n1 = strlen(op1);
    size_t n2 = strlen(op2);
    int min = fmin(n1, n2);
    int max = fmax(n1, n2);
    int n = max + 1;
    uint8_t temp = 0;
    uint8_t adder = 0;
    char *greater = NULL;
    
    *res = (char *)realloc(*res, (n + 1) * sizeof(char));
    
    if (*res != NULL)
    {
        for (int i = 0; i < n + 1; i++)
            *(*res + i) = '\0';
        
        for (int i = 1; i <= min; i++)
        {
            temp = todgt(op1[n1 - i]) + todgt(op2[n2 - i]);
            *(*res + n - i) = tochar((temp += adder) % 10);
            adder = (temp - temp % 10) / 10;
        }
        
        greater = GetGreater(op1, op2);
        for (int i = max - min - 1; i >= 0; i--)
        {
            temp = todgt(greater[i]) + adder;
            *(*res + i + 1) = tochar(temp % 10);
            adder = (temp - temp % 10) / 10;
        }
        *(*res) = tochar(adder);
        
        RemoveLeadingZero(res);
    }
    else
        perror("Insufficient memory at AddNToN");
    
}

void MultiplyNByN(const char *op1, const char *op2, char **res)
{
    size_t n = 0;
    size_t n2 = 0;
    char sign[2] = "\0";
    char *temp1 = (char *)calloc(2, sizeof(char));
    char *temp2 = (char *)calloc(2, sizeof(char));
    *res = (char *)calloc(2, sizeof(char));
    
    if (*res != NULL && temp1 != NULL && temp2 != NULL)
    {
        *temp2 = '0';
        
        if (Xor(op1[0] == '-', op2[0] == '-'))
            sign[0] = '-';
        
        if (op1[0] == '+' || op1[0] == '-')
            op1 += 1;
        
        if (op2[0] == '+' || op2[0] == '-')
            op2 += 1;
        
        n2 = strlen(op2);
        
        int j = 0;
        for (size_t i = n2; i >= 1; i--, j++)
        {
            MultiplyNByOne(op1, op2[i - 1], j, &temp1);
            AddNToN(temp1, temp2, res);
            strcpy(temp2, *res);
        }
        
        free(temp1);
        free(temp2);
        
        if (sign[0] == '-')
        {
            n = strlen(*res);
            for (size_t i = n; i >= 1; i--)
                swapc(*res + i, *res + i - 1);
            *res = (char *)realloc(*res, (n + 2) * sizeof(char));
            if (*res != NULL)
            {
                *(*res + n + 1) = '\0';
                *(*res) = '-';
            }
            else
                perror("Insufficient memory at MultiplyNByN");
        }
    }
    else
        perror("Insufficient memory at MultiplyNByN");
}

double GetDeterminant(int order, double matrix[order][order])
{
    double res = 0.0;
    
    switch (order)
    {
        case 1:
            res = matrix[0][0];
            break;
        case 2:
            res = matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
            break;
        default:
            for (int j = 0; j < order; j++)
            {
                res += matrix[0][j] * GetCofactor(order, matrix, 0, j);
            }
            break;
    }
    
    return res;
}

double GetCofactor(int order, double matrix[order][order], int i, int j)
{
    return pow(-1.0, i + j) * GetMinor(order, matrix, i, j);
}

double GetMinor(int order, double matrix[order][order], int i, int j)
{
    double minor[order - 1][order - 1];
    
    int q = 0;
    for (int m = 0; m < order; m++)
    {
        for (int n = 0; n < order; n++)
        {
            if (m != i && n != j)
            {
                *(&minor[0][0] + q) = matrix[m][n];
                q++;
            }
        }
    }
    
    return GetDeterminant(order - 1, minor);
}

void FindAndReplace(const char *src, const char *txt,
                    const char *replacement, char **dst)
{
    int number_of_occurrences = 0;
    int *indices = NULL;
    size_t src_len = 0;
    size_t txt_len = 0;
    size_t rep_len = 0;
        
    if (!Contains(src, txt))
    {
        *dst = calloc(strlen(src) + 1, sizeof(char));
        if (*dst != NULL)
            strcpy(*dst, src);
    }
    else
    {
        src_len = strlen(src);
        txt_len = strlen(txt);
        rep_len = strlen(replacement);
        
        number_of_occurrences = GetNumberOfOccurrences(src, txt);
        *dst = calloc(strlen(src) - number_of_occurrences *
                      (strlen(txt) - strlen(replacement)) + 1, sizeof(char));
        if (*dst != NULL)
        {
            GetIndicesOfOccurrence(src, txt, &indices);
            
            for (int i = 0, j = 0, n = 0; i < src_len; i++, j++)
            {
                if (i != indices[n])
                {
                    *(*dst + j) = src[i];
                }
                else
                {
                    for (int k = 0; k < rep_len; k++, j++)
                    {
                        *(*dst + j) = replacement[k];
                    }
                    
                    i += txt_len - 1;
                    j--;
                    if (n != number_of_occurrences - 1)
                        n++;
                }
            }
            
            free(indices);
        }
    }
}

void FindAndReplace2(const char *src, const char *txt,
                     const char *replacement, char **dst)
{
    int number_of_occurrences = GetNumberOfOccurrences(src, txt);
    size_t src_len = strlen(src);
    size_t txt_len = strlen(txt);
    size_t rep_len = strlen(replacement);
    
    *dst = calloc(strlen(src) - number_of_occurrences *
                  (strlen(txt) - strlen(replacement)) + 1, sizeof(char));
    
    if (*dst != NULL)
    {
        for (size_t i = 0, j = 0, l = 0; i < src_len; i++)
        {
            if (src[i] == txt[j])
            {
                if (j == txt_len - 1)
                {
                    for (size_t k = 0; k < rep_len; k++, l++)
                    {
                        *(*dst + l) = replacement[k];
                        printf("%lu%c", l, replacement[k]);
                    }
                    j = 0;
                }
                else
                {
                    j++;
                }
            }
            else
            {
                *(*dst + l) = src[i];
                printf("%lu%c", l, src[i]);
                l++;
                j = 0;
            }
        }
    }
}

bool Contains(const char *s1, const char *s2)
{
    size_t n1 = strlen(s1);
    size_t n2 = strlen(s2);
    
    if (n2 > n1)
        return false;
    else
    {
        for (int i = 0, j = 0; i < n1; i++)
            if (s1[i] == s2[j])
                if (j == n2 - 1)
                    return true;
                else
                    j++;
            else
                j = 0;
            
        return false;
    }
}

int GetNumberOfOccurrences(const char *s1, const char *s2)
{
    int n = 0;
    size_t n1 = strlen(s1);
    size_t n2 = strlen(s2);
    
    if (n2 <= n1)
    {
        for (int i = 0, j = 0; i < n1; i++)
        {
            if (s1[i] == s2[j])
            {
                if (j == n2 - 1)
                {
                    n++;
                    j = 0;
                }
                else
                {
                    j++;
                }
            }
            else
            {
                j = 0;
            }
        }
    }
    
    return n;
}

void GetIndicesOfOccurrence(const char *s1, const char *s2, int **res)
{
    int number_of_occurrences = GetNumberOfOccurrences(s1, s2);
    *res = calloc(number_of_occurrences, sizeof(int));
    
    if (*res != NULL)
    {
        size_t n1 = strlen(s1);
        size_t n2 = strlen(s2);
        
        if (n2 <= n1)
        {
            for (int i = 0, j = 0, n = 0; i < n1; i++)
            {
                if (s1[i] == s2[j])
                {
                    if (j == n2 - 1)
                    {
                        *(*res + n) = i - j;
                        n++;
                        j = 0;
                    }
                    else
                    {
                        j++;
                    }
                }
                else
                {
                    j = 0;
                }
            }
        }
    }
}

void SInsertAtEnd(const char *value, struct String **head)
{
    struct String *ptr = NULL;
    struct String *new_ptr = NULL;

    if (*head == NULL)
    {
      new_ptr = (struct String *)malloc(sizeof(struct String));

        if (new_ptr != NULL)
        {
            *head = new_ptr;
            new_ptr->next = NULL;
            new_ptr->prev = NULL;
            new_ptr->value = strdup(value);
        }
        else
        {
            perror("Insufficient memory at SInsertAtEnd");
        }
    }
    else
    {
        ptr = *head;
        
        while (ptr->next != NULL)
            ptr = ptr->next;
        
        new_ptr = (struct String *)malloc(sizeof(struct String));
        
        if (new_ptr != NULL)
        {
            ptr->next = new_ptr;
            new_ptr->prev = ptr;
            new_ptr->value = strdup(value);
        }
        else
        {
            perror("Insufficient memory at SInsertAtEnd");
        }
    }
}

void LInsertAtEnd(struct String *value, struct List **head)
{
    struct List *ptr = NULL;
    struct List *new_ptr = NULL;

    if (*head == NULL)
    {
        new_ptr = (struct List *)malloc(sizeof(struct List));

        if (new_ptr != NULL)
        {
            *head = new_ptr;
            new_ptr->next = NULL;
            new_ptr->prev = NULL;
            new_ptr->head = value;
        }
        else
        {
            perror("Insufficient memory at LInsertAtEnd");
        }
    }
    else
    {
        ptr = *head;
        
        while (ptr->next != NULL)
            ptr = ptr->next;
        
        new_ptr = (struct List *)malloc(sizeof(struct List));
        
        if (new_ptr != NULL)
        {
            ptr->next = new_ptr;
            new_ptr->prev = ptr;
            new_ptr->head = value;
        }
        else
        {
            perror("Insufficient memory at LInsertAtEnd");
        }
    }
}

int GetListLength(struct Node *head)
{
    int n = 0;
    struct Node *ptr = NULL;

    if (head != NULL)
    {
        ptr = head;

        while (ptr->next != NULL)
        {
            n++;
            ptr = ptr->next;
        }

        n++;
    }

    return n;
}

void SFreeList(struct String **head)
{
    struct String *ptr = NULL;
    struct String *one_to_last_ptr = NULL;
    
    if (*head != NULL) {
        ptr = *head;
        *head = NULL;
        
        while (ptr != NULL) {
            one_to_last_ptr = ptr;
            ptr = ptr->next;
            free(one_to_last_ptr->value);
            free(one_to_last_ptr);
        }
    }
}

void SPrintList(struct String *head)
{
    struct String *ptr = head;
    
    if (ptr != NULL) {
        printf("%s: %s (%s)\n", ptr->value, ptr->next->next->value,
               ptr->next->next->next->value);
    }
}

void LFreeList(struct List **head)
{
    struct List *ptr = NULL;
    struct List *one_to_last_ptr = NULL;
    
    if (*head != NULL) {
        ptr = *head;
        *head = NULL;
        
        while (ptr != NULL) {
            one_to_last_ptr = ptr;
            ptr = ptr->next;
            SFreeList(&one_to_last_ptr->head);
            free(one_to_last_ptr);
        }
    }
}

void LPrintList(struct List *head)
{
    struct List *ptr = head;
    
    while (ptr != NULL) {
        SPrintList(ptr->head);
        ptr = ptr->next;
    }
}

void PrintList(struct Node *head)
{
    struct Node *ptr = head;
    
        while (ptr != NULL) {
            printf("%d\n", ptr->id);
            ptr = ptr->next;
        }
}

