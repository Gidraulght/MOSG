#ifndef ACTIONS_H
#define ACTIONS_H
#include <list>
#include "GameObject.h"

//����� ����������� ��������
class GameObject;//��� ����� ��� ����, ����� � Actions ��������� �� GameObject, UI_And_General � ��������
//��������� action, ��������, ����� � GameObject
class UI_And_General;
class Actions
{
    public:
        Actions();
        virtual ~Actions();
        void act(GameObject*,UI_And_General *user_interface);
        void show_available_actions();

    protected:
    std::map<int,action> action_map;//���� �������� ������� � � ��������(� �����?)
    std::map<int,action>::iterator am_it;

//��������� �������� (��������� action �� GameObject)
    int action_code;
    std::string action_parameters;

    private:
};

#endif // ACTIONS_H
