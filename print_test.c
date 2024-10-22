#include<stdio.h>
#include<string.h>
#include"parse.h"
#include"print.h"



const char * parseValue(JsonItem *item,  const char * json);
char * printValue(JsonItem *item,int depth);

JsonItem * newItem();

int main(){
    char arr[] = "[\"nihao\",100,false,true,null,{\"name\":\"wdl\"}]";

    JsonItem * item = newItem();

    const char * ptr = parseValue(item,arr);

    const char * aa = printValue(item,0);

    printf("%s",aa);

    return 0;
}
