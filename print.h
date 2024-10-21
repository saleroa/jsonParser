#include"parse.h"

char * printNorm(const char * input);
char * printString(JsonItem *item);
char * getString(const char * str);
char * printNum(JsonItem *item);
char * printArray(JsonItem *item,int depth);
char * printObject(JsonItem *item,int depth);
char * printValue(JsonItem *item,int depth);