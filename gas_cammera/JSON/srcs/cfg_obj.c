#include "../headers/cfg_obj.h"
#include <stdio.h>

Cfg_obj::Cfg_obj()
{
this->title="title";

}

void Cfg_obj::read(char* buff){
    printf("reading\n");
}
void Cfg_obj::write(){
    printf("writing\n");
}
char* Cfg_obj::toString(){
return NULL;
}
