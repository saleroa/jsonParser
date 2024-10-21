#pragma once

#define TYPE_false 0
#define TYPE_true 1
#define TYPE_num 2
#define TYPE_string 3
#define TYPE_arrry 4
#define TYPE_object 5
#define TYPE_null 6

typedef struct JsonItem
{
    int itemType;
    //
    signed int intValue;
    char *stringValue;
    char* stringKey;
    // 
    struct JsonItem *child;
    struct JsonItem *pre, *next;
    
}JsonItem;

const char * parseValue(JsonItem *item,  const char * json);

JsonItem * newItem();