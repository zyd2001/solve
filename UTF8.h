#ifndef UTF8_H
#define UTF8_H

typedef struct UTF8_String_tag UTF8_String;

int utf8_strlen(char *str);

UTF8_String *utf8_string_new();
// UTF8_String *utf8_string_new_wrap(char *str);
#define utf8_string_new_wrap(str) utf8_string_append(utf8_string_new(), str) // char *str
UTF8_String *utf8_string_new_char(char ch);
UTF8_String *utf8_string_append(UTF8_String *str, char *new);
// return NULL
void *utf8_string_delete_func(UTF8_String **str, ...);
#define utf8_string_delete(...) utf8_string_delete_func(__VA_ARGS__, NULL) // UTF8_STRING **str
int utf8_string_compare(UTF8_String *first, UTF8_String *second);
UTF8_String *utf8_string_reassign(UTF8_String *str, char *new);
UTF8_String *utf8_string_copy_func(UTF8_String **destination, UTF8_String *source);
UTF8_String *utf8_string_copy_new(UTF8_String *source);
#define utf8_string_copy(destination, source) utf8_string_copy_func(&destination, source)
UTF8_String *utf8_string_substring(UTF8_String *str, int start, int end);
#define utf8_string_substr(str, start, length) utf8_string_substring(str, start, start + length); // UTF8_STRING **str
char utf8_string_get_char_ascii(UTF8_String *str, int index);
UTF8_String *utf8_string_get_char(UTF8_String *str, int index);
#define utf8_string_get_char(str, index) utf8_string_substring(str, index, index + 1); // UTF8_STRING **str
UTF8_String *utf8_string_append_char(UTF8_String *str, char ch);
int utf8_string_indexof(UTF8_String *str, char *target);
int utf8_string_test_indexof(UTF8_String *str, UTF8_String *target); //这个贼J8傻逼，就测试用的
char *utf8_string_get_value(UTF8_String *str);
int utf8_string_get_length(UTF8_String *str);

#endif