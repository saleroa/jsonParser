#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"parse.h"


const char * readString(JsonItem *item,const char * json);
const char * readArray(JsonItem *item,const char * json);
const char * readObject(JsonItem *item,const char * json);
const char * readNum(JsonItem *item,const char * json);
JsonItem * newItem();
void printCharArray(char *array);



// #define TESTMODE



const char * parseValue(JsonItem *item,  const char * json){
    // 如果传入的json指针为空，报错
    if(!json){ return NULL;}

    // 特定类型判断
    if(!strncmp(json,"null",4) ||!strncmp(json,"NULL",4)){
        item->itemType = TYPE_null;
        #ifdef TESTMODE
            printf("%s\n","null");
        #endif
        return json + 4;
    }

    if(!strncmp(json,"false",5) ||!strncmp(json,"FALSE",5)){
        item->itemType = TYPE_false;
        #ifdef TESTMODE
            printf("%s\n","false");
        #endif
        return json + 5;
    }
    

    if(!strncmp(json,"true",4) ||!strncmp(json,"TRUE",4)){
        item->itemType = TYPE_true;
        #ifdef TESTMODE
            printf("%s\n","true");
        #endif
        return json + 4;
    }
    
    // 读取string
    if(*json == '\"'){
        return readString(item,json);
    }

    // 读取数字
    if(*json =='-' || (*json > '0' && *json <= '9')){
        return readNum(item,json);
    }
    
    // 读取 object
    if(*json == '{'){
        return readObject(item,json);
    }

    // 读取 array
    if(*json == '['){
        return readArray(item,json);
    }
    
    // fail
    return NULL;
}


// 指针从引号到后引号的后一位
const char * readString(JsonItem *item,const char * json){
    const char* ptr = json + 1;
    char * out, *res;
    int len = 0;
    
    // 计算出 string 的长度,申请对应内存
    while(*ptr != '\"' && *ptr && ++len){ptr++;};
    res = (char*)malloc(len+1);
    out = res;
    if (!out){return NULL;}

    // 复制string
    ptr = json+1;
    while(len && (*out++ = *ptr++)){len--;}

    *out++ = '\0';
    
    #ifdef TESTMODE
        printCharArray(res);
    #endif

    item->stringValue = res;
    item->itemType = TYPE_string;

    return ++ptr;
}

// 指针从符号位到逗号
const char * readNum(JsonItem *item,const char * json){
    int num = 0, n = 0;
    const char* ptr = json;
    // 获取符号
    signed sign = 1; // 默认为 正
    if(*ptr == '-'){
        sign = -1; ptr++;
    }

    while(*ptr != ',' && *ptr >= '0' && *ptr <= '9'){
        num = 10 * n + (*ptr -'0');
        n = num;
        ptr++;
    }

    #ifdef TESTMODE
        printf("%d\n",num);
    #endif
    item->intValue = sign * num;
    item->itemType = TYPE_num;
    return ptr; // 指针指向了结束的 ，
}

const char * readArray(JsonItem *item,const char * json){
    JsonItem *child;
    item->itemType = TYPE_arrry;
    const char * ptr = json+1;

    // 空的 array
    if(*ptr == ']'){return ptr+1;}

    // 构建 array
    child = newItem();
    if(!child){return NULL;}
    item->child = child;

    // 开始读取 array
    ptr = parseValue(item->child,ptr);
    if(!ptr) {return NULL;}

    // 读取同级元素
    while(*ptr == ','){
        JsonItem *newitem = newItem();
        if(!newItem){return NULL;}

        item->child->next = newitem;
        newitem->pre = item->child;
        item->child = newitem;

        ptr = parseValue(item->child,++ptr);
        if(!ptr){return 0;}
    }
    if(*ptr == ']') return ptr+1;
    return NULL;
 }


// 指针从 { 到 } 后一位
const char * readObject(JsonItem *item,const char * json){
    JsonItem *child;
    item->itemType = TYPE_object;
    const char *ptr = json+1;

     // 空的 object
    if(*ptr == '}'){return ptr+1;} // return 到了 } 的下一位

    // 构建 object
    child = newItem();
    if(!child){return NULL;}
    item->child = child;

    // 开始读取 object
        // 先读取 string
    ptr = readString(item->child,ptr); 
    if(!ptr){return NULL;}
    item->child->stringKey = item->child->stringValue;
    item->child->stringValue = NULL;
        // 判断 ：
    if(*ptr != ':'){return NULL;}

        // 读取 value
    ptr = parseValue(item->child,++ptr);
    if(!ptr){return NULL;}

    while(*ptr == ','){
        JsonItem *newitem = newItem();
        if(!newitem){return NULL;}

        item->child->next = newitem;
        newitem->pre = item->child;
        item->child = newitem;

        ptr = readString(item->child,++ptr);
        if(!ptr){return NULL;}
        item->child->stringKey = item->child->stringValue;
        item->child->stringValue = NULL;

        if(*ptr != ':'){return NULL;}

        ptr = parseValue(item->child,++ptr);
        if(!ptr){return NULL;}
    }
	
	if (*ptr=='}') return ptr+1;	
	return NULL;
}

JsonItem * newItem(){
    JsonItem *node = (JsonItem*)malloc(sizeof(JsonItem));
    if(node) memset(node,0,sizeof(JsonItem));
    return node;
}


void printCharArray(char *array) {
    if (array != NULL) {
        while (*array != '\0') {
            printf("%c", *array);
            array++;
        }
    }
    printf("\n"); // 输出换行符，以便在控制台中格式化输出
}