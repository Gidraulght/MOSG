#define _USE_MATH_DEFINES // for C++  
#include <cmath>  
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>

#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream>      // std::stringstream
#include <fstream>
#include <list>
#include <math.h>


#include "UI_And_General.h"
#include "GameObject.h"
#include "Actions.h"
#include "SaveManager.h"


#include <cereal/types/map.hpp>
#include <cereal/types/list.hpp>
#include <cereal/types/utility.hpp> //std::pair support

#include <cereal/archives/binary.hpp>

//хо-хо
//добрался)


void UI_And_General::initialise_default()
{

	default_texture.loadFromFile("resources/player.png");

	buffer1.loadFromFile("resources/aputi.ogg");
	buffer2.loadFromFile("resources/ne putyu.ogg");
	aputi.setBuffer(buffer1);
	ne_putyu.setBuffer(buffer2);

	font.loadFromFile("resources/verdana.ttf");
		

}


void UI_And_General::initialise_objects()//говорящее название, не правда? :D
{
    //std::list<action> primitive_action_list,capturer_s_list;
    std::map<int,action> primitive_action_map,capturer_map,player_map;//мапы действий
    GameObject *goa;//game object address

    action zer0;//do nothing
    zer0.code=0;
    zer0.parameters='-';

    action a;//walk randomly
    a.code=3;
    a.parameters='-';

    action b;//capture objects
    b.code=4;
    b.parameters="Stupid box";

    action c;//release objects
    c.code=5;
    c.parameters="Stupid box";


    //тут я заполняю мапы действий действиями
    player_map[0]=zer0;
    primitive_action_map[0]=a;
    capturer_map[0]=b;
    capturer_map[1]=a;
    capturer_map[2]=a;
    capturer_map[3]=a;
    capturer_map[4]=c;
    capturer_map[5]=a;
    capturer_map[6]=a;
    capturer_map[7]=zer0;




    for (int q=0;q<5;q++)//create n Stupid Boxes
    {
        goa=new GameObject(&default_texture,0,100, 100,36,sf::Vector2f(rand()%w,rand()%h),"Stupid box",q);
        goa->set_action_map(primitive_action_map);
        object_list.push_back(goa);

    }

    for (int q=0;q<5;q++)
    {
        goa=new GameObject(&default_texture,0,100, 100,36,sf::Vector2f(rand()%w,rand()%h),"Capturer",q);
        goa->set_action_map(capturer_map);
        object_list.push_back(goa);
    }

	goa = new GameObject(&default_texture, 1, 100, 100, 36, sf::Vector2f(div(w, 2).quot, div(h, 2).quot), "Gid", 0);//игрок добавляется последним и, таким образом, отображается поверх всего, кроме интерфейса
	goa->set_action_map(player_map);
	object_list.push_back(goa);//создаём игрока


	{
		/*
		std::stringstream ss;
		std::ofstream ofs("data.dat");
		cereal::BinaryOutputArchive archive(ofs);
		GameObjectSave s1,s2;
		s1.x = 10;
		s1.y = 257;
		archive(s1);
		std::ifstream ifs("data.dat");
		cereal::BinaryInputArchive ar(ifs);
		ar(s2);
		//std::cout << stree << std::endl;
		*/
	}
	//std::cout << "" << std::endl;



	/*
	GameObjectSave s1, s2;
	SaveManager savman;
	s1=savman.GameObject_to_GOS(goa);//последним goa был Gid (см. выше, UI_And_General::initialise_objects)
	savman.save_GOS(s1);
	s2=savman.load_GOS();
	std::cout <<s2.name<< " " <<s2.number<< std::endl;
	*/

}







 float UI_And_General::correct_angle(float x,float y)//да хоть удалить, угол с разрывом
 {
    float desired_angle;
    if(x>=0 && y>0)
    {
        desired_angle=atan(y/x)*180/M_PI;
    }
        if(x<0 && y<0)
    {
        desired_angle=-180+atan(y/x)*180/M_PI;
    }

        if(x>0 && y<0)
    {
        desired_angle=atan(y/x)*180/M_PI;
    }

        if(x<0 && y>0)
    {
        desired_angle=180+atan(y/x)*180/M_PI;
    }
    if (x=0)
    {
        if (y<0) desired_angle=-90;
        else if (y>0) desired_angle=90;
        else desired_angle=0;
    }


    /*
    if (x==0)
    {
        if (y>0)
        {
            desired_angle=90;
        }
        else
        {
            desired_angle=-90;
        }
    }
    else
    {
        desired_angle=atan(y/x)*180/M_PI;
        */

        //std::cout<<desired_angle<<std::endl;
        //if (x<0)
        //desired_angle+=180;

    return desired_angle;
 }

        std::list <GameObject*> UI_And_General::get_object_list() { return object_list; }
        void UI_And_General::set_object_list( std::list <GameObject*> val) { object_list = val; }


UI_And_General::UI_And_General()
{
    
	initialise_default();

}

UI_And_General::~UI_And_General()
{
    //dtor
}
