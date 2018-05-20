#ifndef ACTIONS_H
#define ACTIONS_H
#include <list>
#include "GameObject.h"

//здесь описываются действия
class GameObject;//это нужно для того, чтобы в Actions ссылаться на GameObject, UI_And_General и наоборот
//структура action, например, лежит в GameObject
class UI_And_General;
class Actions
{
    public:
        Actions();
        virtual ~Actions();
        void act(GameObject*,UI_And_General *user_interface);
        void show_available_actions();

    protected:
    std::map<int,action> action_map;//мапа действий объекта и её итератор(а вдруг?)
    std::map<int,action>::iterator am_it;

//параметры действия (структура action из GameObject)
    int action_code;
    std::string action_parameters;

    private:
};

#endif // ACTIONS_H
