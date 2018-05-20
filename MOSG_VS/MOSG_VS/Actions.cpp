#include "Actions.h"
#include "GameObject.h"
#include "UI_And_General.h"

void Actions::act(GameObject* op,UI_And_General* user_interface)
{   if (op->get_action_timer()<0)//по таймеру
    {
        op->set_action_timer(0.1);//ставим его заново
        action_map=op->get_action_map();
        int ca=op->get_current_action();
        op->next_action();//переключаем действие

        action u=action_map[ca];
        action_code=u.code;//получаем код действия
        action_parameters=u.parameters;//получаем параметры действия(пока не обрабатываются)
        std::list<GameObject*> selected_object;
        std::list<GameObject*>::iterator so_it,next_it;

        switch (action_code )//в зависимости от кода, делаем разное
        {
        case 0://do nothing
            break;
        case 1:
            op->set_health(op->get_health()-1);
            break;
        case 2:
            op->set_velocity(op->get_velocity()*1.1f);//убрать торможение
            break;
        case 3://walk randomly
            float a,b;
            a=rand()%5;//остаток от деления на 5 (он выдаёт очень большие числа)
            b=rand();
            op->set_velocity(op->get_velocity()+sf::Vector2f(a*cosf(b),a*sinf(b)));//sin и cos нужны для анизотропности блужданий(иначе жесть какая-то)
            //генератор случайных чисел нормально не настроишь, всё равно есть общее направления движения, поэтому пришлось использовать полярную систему координат
            //хотя почему бы это не оптимизировать?)
            //может у тебя получится
            break;
        case 4://поймать объект
               //первый раз используется строка с параметрами действия

            selected_object=op->are_near(user_interface,op->get_position(),10);//смотрим СПИСОК ближайших игровых объектов, перебирая все(!!!) объекты

            for (so_it=selected_object.begin();so_it!=selected_object.end(); so_it++)
            {
                GameObject *b=*so_it;
                if (b->get_name()==action_parameters)//смотрит имя и сравнивает с параметром действия
                {
                    op->capture(b);//этот объект ловит другой из списка
                    //std::cout<<"Gotcha!"<<std::endl;
                }

            }

            break;
        case 5://отпускает из инвентаря
               //тут всё аналогично
            selected_object=op->get_internal_objects_list();//смотрит инвентарь
             for (so_it=selected_object.begin();so_it!=selected_object.end(); so_it = next_it)
            {	
				 next_it = so_it;
				 ++next_it;
                GameObject *b=*so_it;
                if (b->get_name()==action_parameters)
                {
                    op->release(b);//этот объект ловит другой из списка
                    //std::cout<<"Released"<<std::endl;
                }
            }


            break;


        default:
            std::cout<<"There is no such a code of action: "<<action_code<<"!"<<std::endl;//при неправильном коде действия
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
