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

//���� ��������� ��������� ������������� ������� � ������� �� main.cpp
//��� �� ��������� ������� �� �������� �������� ����� ( �� ������ ��� ��)

class GameObject;
class UI_And_General
{
    public:

        UI_And_General();//����������� � ����������
        virtual ~UI_And_General();

        void initialise_objects();
        void initialise_default();

        std::list <GameObject*> get_object_list();
        void set_object_list( std::list <GameObject*> val);

        float correct_angle(float x,float y);//������ ��������, ���� ���������� � �������� � � ���������� �� �������

        int w=800;//������� ������
        int h=600;
        int n=500;//�-�� Stupid Box-��

        std::list <GameObject*> object_list;//�������� ������ ��������
        std::list <GameObject*>::iterator it;//

        std::map <std::string,sf::Texture> textures;//���� ���� ������� ��������, ���������
		std::map <std::string, sf::SoundBuffer> sounds;//���� ���� ������� �����, ���������

		sf::Texture default_texture;
		sf::SoundBuffer buffer1, buffer2;
		sf::Sound aputi;
		sf::Sound ne_putyu;
		sf::Font font;



        const float deg_to_rad=M_PI/180;//��������) ������ �������, �� �� ����-��������� ����, ���������)

    protected:


    private:
};

#endif // UI_AND_GENERAL_H
