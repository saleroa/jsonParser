#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"parse.h"

char * printNorm(const char * input);
char * printString(JsonItem *item);
char * getString(const char * str);
char * printNum(JsonItem *item);
char * printArray(JsonItem *item,int depth);
char * printObject(JsonItem *item,int depth);
char * printValue(JsonItem *item,int depth);

char * printNorm(const char * input);

char * printValue(JsonItem *item,int depth){
	char *out=0;
	if (!item) return 0;
	
	switch (item->itemType){
		case TYPE_null:	out=printNorm("null");	break;
		case TYPE_false:	out=printNorm("false");break;
		case TYPE_true:	out=printNorm("true"); break;
		case TYPE_num:	out=printNum(item);break;
		case TYPE_string:	out=printString(item);break;
		case TYPE_arrry:	out=printArray(item,depth);break;
		case TYPE_object:	out=printObject(item,depth);break;
	}
	
	return out;
}


//打印 null，false，true 这样的规格化的输出
char * printNorm(const char * input){
    size_t len = 0;
    char * out = NULL;
    // strlen 不会读取到 \0,如果用 sizeof 就会算上 \0
    len = strlen(input) + 1;
    out = (char *)malloc(len);
    if(!out) return NULL;
    memcpy(out,input,len);
    return out;
}

// 打印数字
char * printNum(JsonItem *item){
    char * out = NULL;
    if(item->child == 0){
        // 申请两个位置就好,一个 0，一个 \0
        out = (char*)malloc(2);
        if(!out) return NULL;
        strcpy(out,"0"); // 实际上 "" 包围的就是一个字符串，末尾自动为 \0
    }else{
        // sign int 的范围
        out = (char*)malloc(12);
        if(!out) return NULL;
        sprintf(out,"%d",item->intValue);
    }
    return out;
}

// 获取 kv 对里 value 部分的 string
char * printString(JsonItem *item){
    return getString(item->stringValue);
}

// 用于打印 key 部分的 string
char * getString(const char * str){
    char * out = NULL, *ptr;
    size_t len = 0;
    len = strlen(str) + 3;
    out = (char*)malloc(len);
	if(!out) return NULL;
    ptr = out;
	*ptr++ = '\"';
    strncpy(ptr,str,strlen(str)); ptr += strlen(str);
	*ptr++ = '\"';ptr ='\0';
    return out;
}

// 打印 json
char * printArray(JsonItem *item,int depth){
	char ** entities = NULL;
	char * out, *ptr;
    char * ret;
	int len = 5; /////////////////////////// ?????????? WHY 5
	JsonItem * child = item->child;
	int numsOfEntities = 0, fail = 0, i = 0;
	size_t tmplen = 0;


	// 统计 entities 的数量
	while(child) {
		numsOfEntities++;
		child = child->next;
		}

	// 如果数量为 0，输出空的 []
	if(!numsOfEntities){
		out = (char*)malloc(3);
		if(!out) return NULL;
		strcpy(out,"[]");
		return out;
	}

	entities = (char**)malloc(numsOfEntities * sizeof(char*));
	if(!entities) return NULL;
	memset(entities,0,numsOfEntities*sizeof(char*));
	child = item->child;
	while(child && !fail){
		ret = printValue(child,depth+1);
		entities[i++] = ret;
		/////////////////////////////////////////// 这个长度不知道有无 bug
		if(ret) len += strlen(ret) + 2; 
		else fail = 1;
		child = child->next;
	}

	if(!fail) out = (char*)malloc(len);
	if(!out) fail = 1;
	
	if(fail){
		for(i=0;i<numsOfEntities;i++){
			if(entities[i]){
				free(entities[i]);
			}
		}
		free(entities);
		return NULL;
	}

	*out = '[';
	ptr = out + 1; *ptr = 0;
	for(i=0;i<numsOfEntities;i++){
		tmplen = strlen(entities[i]); // 
		memcpy(ptr,entities[i],tmplen); ptr+=tmplen;
		if(i != numsOfEntities-1) *ptr++ = ',',*ptr++ = ' '; *ptr = 0;
		free(entities[i]);
	}
	free(entities);
	*ptr++ = ']';*ptr++ = 0;
	return out;
}


// 打印 object 
char * printObject(JsonItem *item,int depth){
	char **entities = NULL, **names = NULL;
	char * out = NULL, *ptr, *ret; char *str;
	JsonItem *child = item->child;
	int len = 7,i=0,j =0;
	int numOfEntities=0,fail = 0;
	size_t tmplen = 0;

	while(child) numOfEntities++,child = child->next;

	if(!numOfEntities){
		out = (char*)malloc(depth+4);  // 为什么是这个长度
		if(!out) return NULL;
		ptr = out;*ptr++ = '{';
		*ptr++ = '\0';
		for(i=0;i<depth-1;i++) *ptr++ ='\t';
		*ptr++ = '}'; *ptr++ = 0;
		return out;
	}
	entities = (char**)malloc(numOfEntities *sizeof(char*));
	if(!entities) return 0;
	names = (char**)malloc(numOfEntities *sizeof(char*));
	if(!names) return 0;
	memset(entities,0,sizeof(char*)*numOfEntities);
	memset(names,0,sizeof(char*)*numOfEntities);

	child = item->child;depth++;len+=depth;
	while(child){
		str = getString(child->stringKey);
        names[i] = str;
		entities[i++] = ret = printValue(child,depth);
		if(str && ret) len += strlen(str) + strlen(ret) + 2 + 2 + depth;
		else fail = 1;
		child = child->next;
	}
	
	if(!fail) out = (char*)malloc(len);
	if(!out) fail = 1;

	if(fail){
		for(i=0;i<numOfEntities;i++){
			if(names[i]) free(names[i]);
			if(entities[i]) free(entities[i]);
		}
		free(names);free(entities);
		return 0;
	}

	*out = '{';ptr = out + 1;*ptr++ = '\n';*ptr=0;
	for(i=0;i<numOfEntities;i++){
		for(j=0;j<depth;j++)*ptr++ = '\t';
		tmplen = strlen(names[i]);memcpy(ptr,names[i],tmplen);ptr+=tmplen;
		*ptr++ = ':';*ptr++ = '\t';
		strcpy(ptr,entities[i]);ptr+=strlen(entities[i]);
		if (i!=numOfEntities-1) *ptr++=',';
		*ptr++ = '\n';*ptr=0;
		free(names[i]);free(entities[i]);
	}
	free(names);free(entities);
	for (i=0;i<depth-1;i++) *ptr++='\t';
	*ptr++='}';*ptr++=0;

	return out;
}