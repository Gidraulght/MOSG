#ifndef UI_AND_GENERAL_H
#define UI_AND_GENERAL_H


#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>


#include "GameObject.h"
#include "UI_And_General.h"
#include "Actions.h"
#include <list>
#define _USE_MATH_DEFINES // for C++  
#include <cmath>  
#include <math.h>

//сюда собираюсь перенести инициализацию текстур и прочего из main.cpp
//так же оброботку событий из главного игрового цикла ( он сейчас там же)

class GameObject;
class UI_And_General
{
    public:

        UI_And_General();//конструктор и деструктор
        virtual ~UI_And_General();

        void initialise_objects();
        void initialise_default();

        std::list <GameObject*> get_object_list();
        void set_object_list( std::list <GameObject*> val);

        float correct_angle(float x,float y);//впреки названию, угол получается с разрывом и к применению не годится

        int w=800;//размеры экрана
        int h=600;
        int n=500;//к-во Stupid Box-ов

        std::list <GameObject*> object_list;//основной список объектов
        std::list <GameObject*>::iterator it;//

        std::map <std::string,sf::Texture> textures;//сюда буду грузить текстуры, заготовка
		std::map <std::string, sf::SoundBuffer> sounds;//сюда буду грузить звуки, заготовка

		sf::Texture default_texture;
		sf::SoundBuffer buffer1, buffer2;
		sf::Sound aputi;
		sf::Sound ne_putyu;
		sf::Font font;



        const float deg_to_rad=M_PI/180;//Дегтурад) звучит страшно, но по сути-переводит углы, константа)

    protected:


    private:
};

#endif // UI_AND_GENERAL_H
