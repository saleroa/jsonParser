#include<stdio.h>
#include"parse.h"


const char * parseValue(JsonItem *item,  const char * json);
JsonItem * newItem();

int main(){
    char arr[] = "[\"nihao\",100,false,true,null,{\"name\":\"wdl\"}]";
    JsonItem * item = newItem();

    const char * ptr = parseValue(item,arr);

    return 0;
}

