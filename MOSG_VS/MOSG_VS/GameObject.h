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
//тут лежат все пол€ и методы игровых объектов
//их параметры мен€ютс€ посредствам сеттеров, так как их пол€-protected
//важные методы: cotrols-управление персонажем
            //   update-занимаетс€ скорост€ми, ускорени€ми, таймерами
            //   capture-ловит объект по адресу объекта
            //   release-отпускает объект по адресу объекта
            //



struct action   //чтобы все действи€ можно было описать маленькой структурой и однообразно € придумал использовать:
{
    int code;//код действи€
    std::string parameters;//и стрку с параметрами
    //в неЄ можно положить всЄ, что не поместилось в код действи€
    //очень гибко, но строку придЄтс€ парсить
    //например capture и release просто получают им€ объекта
};

//действи€ помещаютс€ в мапу действий каждого конткретного игрового объекта
//в мапе у них есть ещЄ и номер
//итого одно действие-2 параметра и номер в мапе.

class UI_And_General;
class GameObjectSave;

class GameObject//: public sf::Drawable
{
    public:


    sf::Texture object_texture;
    sf::Sprite object_sprite;
    sf::Texture *texture_pointer;

        GameObject();//конструкторы и деструкторы
		GameObject(UI_And_General *,GameObjectSave);
        GameObject(sf::Texture *ptr,bool,int,int, float,sf::Vector2f,std::string,int);
       virtual ~GameObject();

//SETTER AND GETTERS BEGIN!
    std::string get_name(){return name;};
    int get_number(){return number;};

    void set_health (int);//методы
    int get_health ();

    void set_mana (int);//методы
    int get_mana ();

    void set_temperature (int);//методы
    int get_temperature ();

    void set_control_timer (float t){control_timer=t;};//методы
    float get_control_timer (){return control_timer;};

    void set_action_timer (float t){action_timer=t;};//методы
    float get_action_timer (){return action_timer;};




    void set_position (sf::Vector2f);//методы
    sf::Vector2f get_position ();
    void set_velocity (sf::Vector2f);//методы
    sf::Vector2f get_velocity ();

    float get_current_angle(){return current_angle;};






    void set_action_map (std::map<int,action> m){action_map=m;};
    std::map<int,action> get_action_map (){return action_map;};



    void set_current_action(int ca){current_action=ca;};
    int get_current_action(){return current_action;};
    void next_action();

//от сюда
    void set_internal_objects_list (std::list<GameObject *>);
    std::list<GameObject *> get_internal_objects_list ();

    void set_contained_status(bool);
    bool get_contained_status();

	bool get_controlable() { return controlable; };

    void set_container(GameObject *);
    GameObject* get_container();
//и до сюда + см. далее работа с инвентарЄм и обеспечение работы capture и releease


//SETTER AND GETTERS END!

//************************************************
   //это заставл€ет работать capture и release
    void add_to_inventory(GameObject * object);
    void remove_from_inventory(GameObject * object);
    void push_item_in_container(GameObject*);//добавить предмет в контейнер////////+
    //void pop_item(GameObject*);//”брать предмет из контейнера

//*************************************************

    void capture (GameObject*);
    void release (GameObject*);



    bool is_controlable();//проверка на то, что объектом можно управл€ть

    void update(float);
    void controls(UI_And_General *UI,sf::RenderWindow *window);

    void rep_attr(float,sf::Vector2f);

    void shmain(sf::Texture *ptr);

    std::list<GameObject*> are_near(UI_And_General *UI,sf::Vector2f, int);//dвыводит объекты, лежащие в пр€моугольнике размером int с центром



protected:

    std::list<GameObject*> internal_objects;//список внутренних объектов
    std::list<GameObject*>::iterator io_it;

    std::map<int,action> action_map;        //поседовательность действий, кторые объект выполн€ет
    std::map<int,action>::iterator am_it;
    int current_action;//32 тыс€чи действий это безумие) не стал делать float

    bool is_contained;//‘лаг нахождени€ в другом предмете(инвентаре)
    GameObject* container;//ссылка на тот объект, что содержит данный объект в своем инвентаре(хоз€ин);



    std::string name;
    int number;

    sf::Vector2f position;
    sf::Vector2f velocity;
    float desired_angle;//это нужно дл€ физики ¬–јў≈Ќ»…)
    float current_angle;
    float angular_velocity;
	float mass;

    float control_timer=0;//потом сделаю список таймеров, еще думаю над этим
    float action_timer=0; //ведь таймеры нужны разные
                          // и к ним нужен удобный доступ (по номеру или адресу, хз)


    int health;
    int mana;
    int temperature;

    bool controlable;   //можно ли им управл€ть


private:

   // virtual void draw(sf::RenderTarget& , sf::RenderStates )  const;//метод дл€ опеспечени€ совместимости с классом drawable

};

#endif // GAMEOBJECT_H
