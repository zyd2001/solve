#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "UTF8.h"

struct UTF8_String_tag 
{
    char *value;
    int size;
    int limit;
    int length;
};

int utf8_strlen(char *str) 
{
    int result = 0;
    for (int i = 0; str[i]; i++) 
    {
        if ((str[i] & 0xc0) != 0x80) 
            result++;
    }
    return result;
}

int char_index_to_utf8_index(char *str, int index)
{
    int result = 0;
    for (int i = 0; i < index; i++)
    {
        if ((str[i] & 0xc0) != 0x80)
            result++;
    }
    return result;
}

UTF8_String *utf8_string_new()
{
    UTF8_String *str;
    str = (UTF8_String*)malloc(sizeof(UTF8_String));
    str->size = 0;
    str->limit = 10;
    str->length = 0;
    str->value = (char*)malloc(10 * sizeof(char));
    return str;
}

// UTF8_String *utf8_string_new_wrap(char *str)
// {
//     UTF8_String *new = utf8_string_new();
//     return utf8_string_append(new, str);
// }

UTF8_String *utf8_string_new_char(char ch)
{
    UTF8_String *str;
    str = (UTF8_String*)malloc(sizeof(UTF8_String));
    str->size = 0;
    str->limit = 0;
    str->length = 0;
    str->value = NULL;
    return utf8_string_append_char(str, ch);
}

UTF8_String *utf8_string_append(UTF8_String *str, char *new)
{
    int new_length, old_length;
    old_length = str->size;
    new_length = strlen(new);
    if (old_length + new_length + 1 > str->limit)
    {
        str->limit = old_length + new_length + 10;
        str->value = (char*)realloc(str->value, str->limit * sizeof(char));
    }
    strcpy(str->value + old_length, new);
    str->size = new_length + old_length;
    str->length = utf8_strlen(str->value);
    return str;
}

UTF8_String *utf8_string_append_char(UTF8_String *str, char ch)
{
    if (str->size + 2 > str->limit)
    {
        str->limit = str->size + 2;
        str->value = (char*)realloc(str->value, str->limit * sizeof(char));
    }
    char *temp;
    temp = str->value + str->size;
    temp[0] = ch;
    temp[1] = '\0';
    str->size++;
    str->length = utf8_strlen(str->value);
    return str;
}

UTF8_String *utf8_string_reassign(UTF8_String *str, char *new)
{
    str->size = 0;
    str->length = 0;
    return utf8_string_append(str, new);
}

int utf8_string_compare(UTF8_String *first, UTF8_String *second)
{
    return strcmp(first->value, second->value);
}

UTF8_String *utf8_string_copy_func(UTF8_String **destination, UTF8_String *source)
{
    if ((*destination) == NULL)
        (*destination) = utf8_string_new();
    (*destination)->size = source->size;
    (*destination)->length = source->length;
    (*destination)->limit = source->limit;
    (*destination)->value = (char*)realloc((*destination)->value, source->limit * sizeof(char));
    strcpy((*destination)->value, source->value);
    return (*destination);
}

UTF8_String *utf8_string_copy_new(UTF8_String *source)
{
    UTF8_String *i = NULL;
    return utf8_string_copy_func(&i, source);
}

void *utf8_string_delete_func(UTF8_String **str, ...)
{
    va_list args;
    va_start(args, str);
    for (UTF8_String **i = str; i != NULL; i = va_arg(args, UTF8_String**))
    {
        free((*i)->value);
        (*i)->value = NULL;
        free(*i);
        *i = NULL;
    }
    va_end(args);
    return NULL;
}

UTF8_String *utf8_string_substring(UTF8_String *str, int start, int end)
{
    int j = -1;
    int starti = -1;
    int endi = -1;
    int len;
    for (int i = 0; i < str->size; i++)
    {
        if ((str->value[i] & 0xc0) != 0x80)
            j++;
        if (j == start && starti == -1)
            starti = i;
        if (j == end)
        {
            endi = i;
            break;
        }
    }
    if (endi == -1)
        endi = str->size;
    UTF8_String *new = utf8_string_new();
    len = endi - starti;
    char *temp = (char*)malloc((len + 1) * sizeof(char));
    memcpy(temp, str->value + starti, len);
    temp[len] = '\0';
    utf8_string_append(new, temp);
    free(temp);
    return new;
}

// UTF8_String *utf8_string_substr(UTF8_String *str, int start, int length)
// {
//     return utf8_string_substring(str, start, start + length);
// }

char utf8_string_get_char_ascii(UTF8_String *str, int index)
{
    return (str->value + index)[0];
}

// UTF8_String *utf8_string_get_char(UTF8_String *str, int index)
// {
//     return utf8_string_substring(str, index, index + 1);
// }

int utf8_string_indexof(UTF8_String *str, char *target)
{
    int size = strlen(target);
    if (str->size < size)
        return -1;
    for (int i = 0; i < (str->size - size + 1); i++)
    {
        if (strncmp(str->value + i, target, size) == 0)
            return char_index_to_utf8_index(str->value, i);
    }
    return -1;
}

int utf8_string_test_indexof(UTF8_String *str, UTF8_String *target)
{
    int length = target->length;
    UTF8_String *temp;
    if (str->length < length)
        return -1;
    for (int i = 0; i < (str->length - length + 1); i++)
    {
        temp = utf8_string_substr(str, i, length);
        if (utf8_string_compare(temp, target) == 0)
        {
            utf8_string_delete(&temp);
            return i;
        }
        utf8_string_delete(&temp);
    }
    utf8_string_delete(&temp);
    return -1;
}

char *utf8_string_get_value(UTF8_String *str)
{
    return str->value;
}

int utf8_string_get_length(UTF8_String *str)
{
    return str->length;
}
