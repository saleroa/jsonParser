#include<stdio.h>
#include<string.h>
#include"parse.h"
#include"print.h"



const char * parseValue(JsonItem *item,  const char * json);
char * printValue(JsonItem *item,int depth);

JsonItem * newItem();

int main(){
  
    // char test1[] = "[\"nihao\",100,false,true,null]";
    // JsonItem * item1 = newItem();
    // const char * ptr1 = parseValue(item1,test1);
    // const char * t1 = printValue(item1,0);
    // printf("%s",t1);

    // char test2[] = "{\"name\":\"wdl\",\"age\":18,\"major\":\"software engering\"}";
    // JsonItem * item2 = newItem();
    // const char * ptr2 = parseValue(item2,test2);
    // const char * t2 = printValue(item2,0);
    // printf("%s",t2);

    char test3[] = "[\"nihao\",100,false,true,null,{\"name\":\"wdl\"}]";
    JsonItem * item3 = newItem();
    const char * ptr3 = parseValue(item3,test3);
    const char * t3 = printValue(item3,0);
    printf("%s",t3);


    char test4[] = "[\"nihao\",100,false,true,null,{\"hello\":\"world\"},{\"name\":\"wdl\",\"age\":18,\"major\":\"software engering\"}]";
    JsonItem * item4 = newItem();
    const char * ptr4 = parseValue(item4,test4);
    const char * t4 = printValue(item4,0);
    printf("%s",t4);

    return 0;
}
