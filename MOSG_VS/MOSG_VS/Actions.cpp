#include "Actions.h"
#include "GameObject.h"
#include "UI_And_General.h"

void Actions::act(GameObject* op,UI_And_General* user_interface)
{   if (op->get_action_timer()<0)//�� �������
    {
        op->set_action_timer(0.1);//������ ��� ������
        action_map=op->get_action_map();
        int ca=op->get_current_action();
        op->next_action();//����������� ��������

        action u=action_map[ca];
        action_code=u.code;//�������� ��� ��������
        action_parameters=u.parameters;//�������� ��������� ��������(���� �� ��������������)
        std::list<GameObject*> selected_object;
        std::list<GameObject*>::iterator so_it,next_it;

        switch (action_code )//� ����������� �� ����, ������ ������
        {
        case 0://do nothing
            break;
        case 1:
            op->set_health(op->get_health()-1);
            break;
        case 2:
            op->set_velocity(op->get_velocity()*1.1f);//������ ����������
            break;
        case 3://walk randomly
            float a,b;
            a=rand()%5;//������� �� ������� �� 5 (�� ����� ����� ������� �����)
            b=rand();
            op->set_velocity(op->get_velocity()+sf::Vector2f(a*cosf(b),a*sinf(b)));//sin � cos ����� ��� �������������� ���������(����� ����� �����-��)
            //��������� ��������� ����� ��������� �� ���������, �� ����� ���� ����� ����������� ��������, ������� �������� ������������ �������� ������� ���������
            //���� ������ �� ��� �� ��������������?)
            //����� � ���� ���������
            break;
        case 4://������� ������
               //������ ��� ������������ ������ � ����������� ��������

            selected_object=op->are_near(user_interface,op->get_position(),10);//������� ������ ��������� ������� ��������, ��������� ���(!!!) �������

            for (so_it=selected_object.begin();so_it!=selected_object.end(); so_it++)
            {
                GameObject *b=*so_it;
                if (b->get_name()==action_parameters)//������� ��� � ���������� � ���������� ��������
                {
                    op->capture(b);//���� ������ ����� ������ �� ������
                    //std::cout<<"Gotcha!"<<std::endl;
                }

            }

            break;
        case 5://��������� �� ���������
               //��� �� ����������
            selected_object=op->get_internal_objects_list();//������� ���������
             for (so_it=selected_object.begin();so_it!=selected_object.end(); so_it = next_it)
            {	
				 next_it = so_it;
				 ++next_it;
                GameObject *b=*so_it;
                if (b->get_name()==action_parameters)
                {
                    op->release(b);//���� ������ ����� ������ �� ������
                    //std::cout<<"Released"<<std::endl;
                }
            }


            break;


        default:
            std::cout<<"There is no such a code of action: "<<action_code<<"!"<<std::endl;//��� ������������ ���� ��������
        }

    }
};


void show_available_actions(){};



Actions::Actions()
{
    //ctor
}

Actions::~Actions()
{
    //dtor
}
