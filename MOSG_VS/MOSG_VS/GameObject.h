#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>

#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream>      // std::stringstream

#include <math.h>
#include <list>
//#include "UI_And_General.h"


//************************
//��� ����� ��� ���� � ������ ������� ��������
//�� ��������� �������� ����������� ��������, ��� ��� �� ����-protected
//������ ������: cotrols-���������� ����������
            //   update-���������� ����������, �����������, ���������
            //   capture-����� ������ �� ������ �������
            //   release-��������� ������ �� ������ �������
            //



struct action   //����� ��� �������� ����� ���� ������� ��������� ���������� � ����������� � �������� ������������:
{
    int code;//��� ��������
    std::string parameters;//� ����� � �����������
    //� �� ����� �������� ��, ��� �� ����������� � ��� ��������
    //����� �����, �� ������ ������� �������
    //�������� capture � release ������ �������� ��� �������
};

//�������� ���������� � ���� �������� ������� ������������ �������� �������
//� ���� � ��� ���� ��� � �����
//����� ���� ��������-2 ��������� � ����� � ����.

class UI_And_General;
class GameObjectSave;

class GameObject//: public sf::Drawable
{
    public:


    sf::Texture object_texture;
    sf::Sprite object_sprite;
    sf::Texture *texture_pointer;

        GameObject();//������������ � �����������
		GameObject(UI_And_General *,GameObjectSave);
        GameObject(sf::Texture *ptr,bool,int,int, float,sf::Vector2f,std::string,int);
       virtual ~GameObject();

//SETTER AND GETTERS BEGIN!
    std::string get_name(){return name;};
    int get_number(){return number;};

    void set_health (int);//������
    int get_health ();

    void set_mana (int);//������
    int get_mana ();

    void set_temperature (int);//������
    int get_temperature ();

    void set_control_timer (float t){control_timer=t;};//������
    float get_control_timer (){return control_timer;};

    void set_action_timer (float t){action_timer=t;};//������
    float get_action_timer (){return action_timer;};




    void set_position (sf::Vector2f);//������
    sf::Vector2f get_position ();
    void set_velocity (sf::Vector2f);//������
    sf::Vector2f get_velocity ();

    float get_current_angle(){return current_angle;};






    void set_action_map (std::map<int,action> m){action_map=m;};
    std::map<int,action> get_action_map (){return action_map;};



    void set_current_action(int ca){current_action=ca;};
    int get_current_action(){return current_action;};
    void next_action();

//�� ����
    void set_internal_objects_list (std::list<GameObject *>);
    std::list<GameObject *> get_internal_objects_list ();

    void set_contained_status(bool);
    bool get_contained_status();

	bool get_controlable() { return controlable; };

    void set_container(GameObject *);
    GameObject* get_container();
//� �� ���� + ��. ����� ������ � ��������� � ����������� ������ capture � releease


//SETTER AND GETTERS END!

//************************************************
   //��� ���������� �������� capture � release
    void add_to_inventory(GameObject * object);
    void remove_from_inventory(GameObject * object);
    void push_item_in_container(GameObject*);//�������� ������� � ���������////////+
    //void pop_item(GameObject*);//������ ������� �� ����������

//*************************************************

    void capture (GameObject*);
    void release (GameObject*);



    bool is_controlable();//�������� �� ��, ��� �������� ����� ���������

    void update(float);
    void controls(UI_And_General *UI,sf::RenderWindow *window);

    void rep_attr(float,sf::Vector2f);

    void shmain(sf::Texture *ptr);

    std::list<GameObject*> are_near(UI_And_General *UI,sf::Vector2f, int);//d������� �������, ������� � �������������� �������� int � �������



protected:

    std::list<GameObject*> internal_objects;//������ ���������� ��������
    std::list<GameObject*>::iterator io_it;

    std::map<int,action> action_map;        //����������������� ��������, ������ ������ ���������
    std::map<int,action>::iterator am_it;
    int current_action;//32 ������ �������� ��� �������) �� ���� ������ float

    bool is_contained;//���� ���������� � ������ ��������(���������)
    GameObject* container;//������ �� ��� ������, ��� �������� ������ ������ � ����� ���������(������);



    std::string name;
    int number;

    sf::Vector2f position;
    sf::Vector2f velocity;
    float desired_angle;//��� ����� ��� ������ ��������)
    float current_angle;
    float angular_velocity;
	float mass;

    float control_timer=0;//����� ������ ������ ��������, ��� ����� ��� ����
    float action_timer=0; //���� ������� ����� ������
                          // � � ��� ����� ������� ������ (�� ������ ��� ������, ��)


    int health;
    int mana;
    int temperature;

    bool controlable;   //����� �� �� ���������


private:

   // virtual void draw(sf::RenderTarget& , sf::RenderStates )  const;//����� ��� ����������� ������������� � ������� drawable

};

#endif // GAMEOBJECT_H
