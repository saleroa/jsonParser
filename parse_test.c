#include<stdio.h>
#include"parse.h"


int parseJson(JsonItem *item,  const char * json);
JsonItem * newItem();

int main(){
    char arr[] = "[\"nihao\",100,false,true,null,{\"name\":\"wdl\"}]";
    JsonItem * item = newItem();

    parseJson(item,arr);

    return 0;
}

