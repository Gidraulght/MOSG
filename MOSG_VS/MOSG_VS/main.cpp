
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>

#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream>      // std::stringstream
#include <list>
#include <math.h>

#include "GameObject.h"
#include "Actions.h"
#include "UI_And_General.h"
#include "SaveManager.h"


using namespace sf;
/*
int w=1920;
int h=1080;
int n=800;
int hw=div(w,2).quot;
int hh=div(h,2).quot;
std::list <GameObject*> object_list;
std::list <GameObject*>::iterator it;
*/

int main()
{
    UI_And_General UI;//���� ������ �������� ������ ���� ��������� ������������ ��� ������ ���������� �����
    Actions actor;//���� ������ ���������� ���������� ���������, �������� ������� ��������(����������� ������ GameObject)

    sf::RenderWindow window(sf::VideoMode(UI.w, UI.h), "My window");//,sf::Style::Fullscreen);//������������� �������
    sf::View GameView,UI_View;//secon one is not in the UI yet
    GameView.reset(sf::FloatRect(0,0,UI.w,UI.h));//��� � �������� ���������
    UI_View.reset(sf::FloatRect(0,0,UI.w,UI.h));//��� � �����������


    window.setFramerateLimit(60);

    Text writing(L"Donut!",UI.font,24);//L is very dubious way of supporting wide characters
                                    //��� ������ L �� ������� �����������, ��� ������� ������� � ��� ������������ ��������� ������ ������. ������������.

        // Create a clock for measuring elapsed time
    sf::Clock clock;
    float time = clock.getElapsedTime().asMicroseconds();//get elapsed time
    float global_time=0;


//*********************************************************
    UI.initialise_objects();
	SaveManager savman;
	savman.load_GameSave(&UI, "saves/Save1.xml");//
	savman.save_GameSave(&UI, "saves/Save1.xml");//
//*********************************************************


    // run the program as long as the window is open
    while (window.isOpen())//�������� ����
    {
        time = clock.getElapsedTime().asSeconds();//get elapsed time
        //global_time+=time;

        clock.restart();
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))//������ ������ � ��������� ����
        { 
            // "close requested" event: we close the window
            if ((event.type == sf::Event::Closed)||(event.type == sf::Event::KeyPressed && event.key.code==sf::Keyboard::Escape))
            {	
				SaveManager savman;
				savman.save_GameSave(&UI,"saves/closeSave1.xml" );
                window.close();
                for (UI.it=UI.object_list.begin();UI.it!=UI.object_list.end();)//��� ������� ������� ���������
                {
                    GameObject *b=*UI.it;
                    UI.it=UI.object_list.erase(UI.it);//����� ��� �������� �������
                    delete b;
                    //it++;
                }
				system("pause"); // ������� �������� ������


            }




            if (event.type == sf::Event::Resized)
            {
                // update the view to the new size of the window
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                GameView=sf::View(visibleArea);//�� ������ ������
                                                //��� ���� ����� ��������� ���������(� ��� ������ ���� ���������� � ���� ������)
                                                //�������� ���� �� �������� ��� ��������� �������� ����(������� ������)
                UI_View.reset(sf::FloatRect(0,0,window.getSize().x,window.getSize().y));//��������� ��� ����������
                //window.setView(sf::View(visibleArea));
            }

            if(event.type==sf::Event::MouseWheelScrolled)           //��� ��������
            {
                if (event.mouseWheelScroll.wheel==sf::Mouse::VerticalWheel)
                {
                        float wheel_delta=0;
                        wheel_delta=event.mouseWheelScroll.delta;
                        //std::cout<<wheel_delta<<std::endl;

                    if (wheel_delta>0)
                    {
                        for (int i=0;i<trunc(wheel_delta);i++)
                        GameView.zoom(0.93);
                    }
                    else
                    {
                        for (int i=0;i<trunc(-wheel_delta);i++)
                        GameView.zoom(1.075);
                    }
                }
            }





        }

    if (( sf::Keyboard::isKeyPressed(sf::Keyboard::Add) ))          //��� ����� + � -
        GameView.zoom(0.95);

    if (( sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract) ))
        GameView.zoom(1.05);


/*
std::stringstream ss;
ss <<"X: " <<player.get_position().x<<"   Y: "<<player.get_position().y<<std::endl;;
    std::string str = ss.str();

        writing.setString(str);
*/




        // clear the window with black color
        window.clear(sf::Color::Black);             //�������� ���� ��������� � ��������� ������� ����������� �����



        //std::cout<<time<<std::endl;
        for (UI.it=UI.object_list.begin();UI.it!=UI.object_list.end();UI.it++)//������� ���� ������� ��������
        {
            GameObject *b=*UI.it;


            if (!b->get_contained_status())//���� �� ������, ���������
            {
                actor.act(b,&UI);

                if (b->is_controlable())        //���� �����������
                {
                    b->controls(&UI,&window);  //�����������)
                }

                b->update(time);//��������� ��������� ������� �������� ������� (���������� ������ GameObject)

                if (b->is_controlable())
                {
                    GameView.setCenter(b->get_position());
                    //GameView.setRotation(b->get_current_angle());         //������� �������� ������
                    window.setView(GameView);
                }

                window.draw(b->object_sprite);//������ ������, ������� � ������� �� ������ b
            }


        }

//***************************************************

//��� ���������� ������ � �����������
        std::stringstream ss;
        ss<<trunc(1/time)<<std::endl;
        std::string str = ss.str();
        writing.setString(str);//������� ���
                                //����� ��� ����� ������� ��� ������ atoi ��� ����-�� �������
                                //������� ���� ������ ���
                                //������ ����� ���� ��� ������ ����� �������

        window.setView(UI_View);// ������� � ���� ���� ���������������� ��� ��������� ���������
        writing.setPosition(window.mapPixelToCoords( Vector2i(10,10) ) );//sets writing position relatively to UI_View
        window.draw(writing);//draws fps
//***************************************************

        window.setView(GameView);//�������� ������� � ���� ������� ��������
                                //��, � ������� �� UI_View, �� ������ ��������� ��� ����������� �������, � ������ ��� ��������� ����


        window.display();
        srand(trunc(clock.getElapsedTime().asMicroseconds())+rand()%5000);//������������� �������)
        //std::cout<<trunc(clock.getElapsedTime().asMicroseconds())+rand()%5000<<std::endl;


    }
    return 0;
}
