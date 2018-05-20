#define _USE_MATH_DEFINES // for C++  
#include <cmath>  
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>

#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream>      // std::stringstream

#include <math.h>

#include "GameObject.h"
#include "UI_And_General.h"
#include "Actions.h"
#include "SaveManager.h"

using namespace sf;

 //  void GameObject::draw(sf::RenderTarget& target, sf::RenderStates states) const {};//����� ����������� ������������� � sfml drawable


    void GameObject::shmain(sf::Texture *ptr)//��� ����� ���������� ����� � ������������ � ����� ��, ��� �� ������ � ������������
    {
        object_sprite.setTexture(*ptr);
        object_sprite.scale(0.6,0.6);//�������� ������ �������
        object_sprite.setOrigin(sf::Vector2f(ptr->getSize()) / 2.f);//����� ����� �������
        velocity=Vector2f(0,0);
        angular_velocity=0;
        desired_angle=0;
        current_angle=0;
        set_current_action(0);//"���������" �������� �� ������� ��������
		
    }


 //SETTER AND GETTERS BEGIN!

    void GameObject::set_health (int x)//������
    {
        health=x;
    }
    int GameObject::get_health ()
    {
        return health;
    }

        void GameObject::set_mana (int x)//������
    {
        mana=x;
    }
    int GameObject::get_mana ()
    {
        return mana;
    }


    void GameObject::set_temperature (int t)
    {
        GameObject::temperature=t;
    }

    int GameObject::get_temperature ()
    {
        return temperature;
    }

//���� ������ �� ����, �.�. � ��� ���� ���� � �� ���� ��������� ��� ��������� ��������
//�� ��������� �� �����
//�� �� �������� �������� �������� ������� ��������� ��������
//� ��������� �������, ������� � ������(��� �����? ��������)
//����� ��� �����������)
//����� ��� ���������� ������ �������)

//� ������ ��������� ���� ���������� ��� ����������
//��������, ��������, �����, ������������, ����� �� ����� �����
//����� ����� ����� ���� �� ������ �-�� ��, ��������� ���������� � ��������� ������ ���������� ������������� ��������
//����� ��� ����� ����, � �����-������
//����� �� �������� ������� ����� � �������� �� ��� ��, ��� ������� �������
//������ ��� � ��� ������ � controls ��� ������� Numpad1

//feel fre� to ��������� �������, ������� � ���� ��������
//max_hp, max_mana, ��������, �������� ����� ���� � �������� � ��������� �� ��������
//� GameObject::update

/*
    void GameObject::set_action_list (std::list<action> external_list)
    {
        action_list=external_list;
        for (al_it=action_list.begin(); al_it!=action_list.end();al_it++)
        {
            //��������
            //action u=*al_it;
            //std::cout<<u.code<<"   "<<u.parameters<<std::endl;

        }
    }
*/



    void GameObject::set_internal_objects_list (std::list<GameObject *> external_list){internal_objects=external_list;}//�������� � ��������� ������ ���������� ��������
    std::list<GameObject *> GameObject::get_internal_objects_list (){return internal_objects;}

    void GameObject::next_action()
    {
        if (current_action==(action_map.size()-1))
        {
         current_action=0;
        }
        else
        {
        current_action+=1;
        }

    }





    void GameObject::add_to_inventory(GameObject * object)
    {
        if (object!=this)
        internal_objects.push_back(object);
    }

    void GameObject::remove_from_inventory(GameObject * object)
    {	

        internal_objects.remove(object);
        //����� �� ������ ������ ������ ���������� ��������� � ��������� � �� �����. � ���������. (!)
        //.........
		//������ ��������, �� ��� ���� � �������� ��� ������ ���� ����������
		//��.Actions::act, case 5
    }



    void GameObject::push_item_in_container(GameObject* cntr)
    {
    cntr->add_to_inventory(this);//�����, ������ �������. ��� � ���� ������.
    is_contained=true;//�����
    }


    void GameObject::set_contained_status(bool ic){is_contained=ic;};//�������/��������� �� ��, ��� �������� �����������
    bool GameObject::get_contained_status(){return is_contained;};

    void GameObject::set_container(GameObject *cntr)
    {
    is_contained=true;
    container=cntr;
    }    //����� ������ ����� is_contained, ������ �� ��������� , internal_objects(� �������� ������ push � pop ��� ����)

    GameObject* GameObject::get_container(){if (is_contained) return container; }

    void GameObject::capture(GameObject *target)
    { if (target!=this)
        {
            target->set_container(this);//������� ������ b ���������� � ������� ����� � �������� ��� ����������
            target->push_item_in_container(this);//��������� � ���� b
        }
    }

    void GameObject::release(GameObject *target)
    {
        if (target->get_contained_status()==true && target->get_container()==this)
        {

            remove_from_inventory(target);//� ��?
            target->set_contained_status(false);

            target->set_position(position);
            target->set_velocity(velocity);
            //std::cout<<"Freedom!"<<std::endl;

        }
    }


    void GameObject::set_position (sf::Vector2f vec)//������
    {
        position=vec;
        object_sprite.setPosition(vec);
    }
    sf::Vector2f GameObject::get_position ()
    {
        return position;
    }
    void GameObject::set_velocity (sf::Vector2f vec)//������
    {
        velocity=vec;
    }
    sf::Vector2f GameObject::get_velocity ()
    {
        return velocity;
    }

    std::list<GameObject*> GameObject::are_near(UI_And_General *UI,sf::Vector2f pos, int distance)
    {
        sf::Vector2f delta;
        std::list<GameObject*> temp_list;
        std::list<GameObject*>::iterator iter;

        for(iter=UI->object_list.begin();iter!=UI->object_list.end();iter++)
        {
            GameObject *b=*iter;
            if (!b->get_contained_status())//�������� �� ��, ��� ������ �� ���������� � ������
            {
            delta=b->get_position()-pos;
            if ((abs(delta.x)<=distance && abs(delta.y)<=distance) )
            {
            temp_list.push_back(b);
            //std::cout<<delta.x<<"    "<<delta.y<<std::endl;
            }
            }

        }

    return temp_list;
    }

    //SETTER AND GETTERS END!


    void GameObject::controls(UI_And_General *UI,sf::RenderWindow *window)
    //��� ���������� ����������
    //��� ���������� ������ ��� ���, � ���� is_controlable==true

    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad1))////////////////////////////////////////
            {
                if (control_timer<0)//����� �� �������
                {
                    set_control_timer(0.05);//������ ������� �� ����������� � ������ GameObject::update
                    std::list<GameObject*> temp_list;
                    std::list<GameObject*>::iterator temp_it;
                    Vector2f delta,vec;
                    float dot;
					int counter = 0;


                    temp_list=are_near(UI,position,100);//�������� ������ ������� ��������

                    for (temp_it=temp_list.begin();temp_it!=temp_list.end();temp_it++)//�������� � ����
                    {
                        GameObject *b=*temp_it;
                        delta=b->get_position()-position;
                        vec.x=cosf(current_angle/180*M_PI);
                        vec.y=sinf(current_angle/180*M_PI);
                        dot=delta.x*vec.x+delta.y*vec.y;
                        if (dot>(cos(M_PI_4))*sqrtf(delta.x*delta.x+delta.y*delta.y) && b!=this)//if in right angle and not self
                            //��� ������ ����� ������� ����?)
                            //�� �� ������ ������ � ��������� � ������� � �������������, ��� ������� ���� ������:D

                        {
                            //std::cout<<"Object is capturable"<<std::endl;
                            if (UI->aputi.getStatus()!=sf::Sound::Paused && UI->aputi.getStatus()!=sf::Sound::Playing )
								UI->aputi.play();
                            //b->set_container(this);//������� ������ b ���������� � ������� ����� � �������� ��� ����������
                            //b->push_item_in_container(this);//��������� � ���� b
							counter += 1;
                            capture(b);//������� b

                        }
						
                    }
					std::cout << "Captured " << counter<< " objects." << std::endl;

                }

            }

//��� �� ����������
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad2))////////////////////////////////////////
        {
		std::list<GameObject*>::iterator next_it;

            if (control_timer<0)
            {
                set_control_timer(0.1);

                //std::cout<<"releasing..."<<std::endl;
                int counter=0;

				for (io_it=internal_objects.begin();io_it!=internal_objects.end();io_it = next_it)//�������� � ����
					{	
					next_it = io_it;
					++next_it;
						counter+=1;
						GameObject *b=*io_it;
						release(b); //��������� ������ b �� ���������
					}


				if (UI->ne_putyu.getStatus() != sf::Sound::Paused && UI->ne_putyu.getStatus() != sf::Sound::Playing)
					UI->ne_putyu.play();
  

                std::cout<<"i had "<<counter<<" objects."<<std::endl;

            }

        }














        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && (velocity.y)>-500)
            { velocity.y-=20; }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && (velocity.x)>-500)
            { velocity.x-=20; }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && (velocity.y)<500)
            { velocity.y+=20; }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && (velocity.x)<500)
            { velocity.x+=20; }


		if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) || angular_velocity<-360)
		{angular_velocity += 10;}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) || angular_velocity>360)
		{angular_velocity -= 10;}


        if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)))
            { velocity.y*=0.9; }
        if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)))
            { velocity.x*=0.9; }

        if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::Q) || sf::Keyboard::isKeyPressed(sf::Keyboard::E)))
            { angular_velocity=angular_velocity*0.9; }



		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{position = Vector2f(div(window->getSize().x, 2).quot, div(window->getSize().y, 2).quot);}


        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))//attract
        {
        Vector2i mwp=sf::Mouse::getPosition(*window);
        Vector2f mouse_in_game_pos=window->mapPixelToCoords(mwp);
        Vector2f to_cursor=mouse_in_game_pos-position;
        rep_attr(80,to_cursor);
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right))//repel
        {
        Vector2i mwp=sf::Mouse::getPosition(*window);
        Vector2f mouse_in_game_pos=window->mapPixelToCoords(mwp);
        Vector2f to_cursor=mouse_in_game_pos-position;
        rep_attr(-80,to_cursor);
        }

    }


      void GameObject::rep_attr(float acceleration,Vector2f to_cursor)//����������� � ������� � �������������_����������� � ����
      {
          float cang=desired_angle/180*M_PI;
          float z,xx,yy;
          to_cursor/=(0.1f+sqrtf(to_cursor.x*to_cursor.x+to_cursor.y*to_cursor.y));
          xx=cosf(cang);
          yy=sinf(cang);
          z=xx*to_cursor.y-yy*to_cursor.x;//��������� ������������
          if (z>0)                        //�� ����� ������� ���� ������� �������� ���������
          angular_velocity+=40;
          if (z<0)
          angular_velocity-=40;

          velocity+=acceleration*to_cursor;
            //std::cout<<desired_angle<<"  to_cursor.x= "<<to_cursor.x<<"   to_cursor.y= "<<to_cursor.y<<std::endl;
            //std::cout<<velocity.x<<std::endl;
      }



    void GameObject::update(float time)
    {
        control_timer-=time;
        action_timer-=time;
        desired_angle+=(angular_velocity*time);
        position.x+=(velocity.x*time);
        position.y+=(velocity.y*time);


        if (position.y>5000)//������ ����� ���������
            position.y-=5000;//������-������ �������, ��� ��� �����������
        else
        {
            if (position.y<0)
            position.y+=5000;
        }
        if (position.x>8000)
            position.x-=8000;
        else
        {
            if (position.x<0)
            position.x+=8000;

        }

        object_sprite.setPosition(position);//������������ ���� �������
        if (desired_angle!=current_angle)//��� ����� ��� ����������� ��������, ��� ��� � ���� ��� �������� �������(�� ������������ ��������� ������ ���������)
        {
            object_sprite.rotate(desired_angle-current_angle);
            current_angle=desired_angle;
        }

       // std::cout<<"Vx= "<<velocity.x<<"   Vy="<<velocity.y<<std::endl;
    }

    bool GameObject::is_controlable() {return controlable;}









GameObject::GameObject() : controlable(false),health(100),mana(100),temperature(20),name("defaul object")//����������� ����������� � � �� �� ���������!
{
  std::cout<<name<<" created  "<<std::endl;
//    GameObject::shmain(sf::Image *ptr);
}


GameObject::GameObject(sf::Texture *ptr,bool i,int h,int m, float t,sf::Vector2f p,std::string new_name,int num) ://��� ������� �����������
     texture_pointer(ptr),controlable(i), health(h),mana(m),temperature(t),position(p),name(new_name), number(num)
{
    //std::cout<<name<<" created and it's number is:"<<number<<std::endl;
    //�� ���� �������, ����� �� ������� ��������, ��� ��� Stupid Box-� ����� ���������� ���, �� ������ ������
    //����� ������� �������:"���� �������"
    //� �� ����� ������ ����� �������
    GameObject::shmain(texture_pointer);
}

GameObject::~GameObject()//����������� ����������

{
    //std::cout<<name<<" destroyed and it's number was:"<<number<<std::endl;
}


GameObject::GameObject(UI_And_General *UI,GameObjectSave gs)// create and PARTIALLY! fill from GameObjectSave
{
	name = gs.name;
	number = gs.number;

	position = sf::Vector2f(gs.x, gs.y);
	velocity = sf::Vector2f(gs.vx, gs.vy);
	current_angle = gs.current_angle;
	desired_angle = current_angle;
	angular_velocity = gs.angular_velocity;
	mass = gs.mass;

	control_timer = gs.control_timer;
	action_timer = gs.action_timer;

	health = gs.health;
	mana = gs.mana;
	temperature = gs.temperature;

	controlable = gs.controlable;
	current_action = gs.current_action;
	is_contained = gs.contained;//+

	texture_pointer = &UI->default_texture;
	GameObject::shmain(texture_pointer);

}
