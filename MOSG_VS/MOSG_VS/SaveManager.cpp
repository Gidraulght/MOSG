#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>

#include "SaveManager.h"
#include "GameObject.h"
#include "UI_And_General.h"



#include <cereal/types/map.hpp>
#include <cereal/types/list.hpp>
#include <cereal/types/utility.hpp> //std::pair support

#include <cereal/archives/xml.hpp>//archeve type

SaveManager::SaveManager()
{
};

SaveManager::~SaveManager()
{
};

void SaveManager::save_GOS(GameObjectSave s1)
{
	std::ofstream ofs("data.xml");//, std::ios::binary);
	cereal::XMLOutputArchive archive(ofs);
	archive(s1);
	std::cout << "Object saved" << std::endl;
}

GameObjectSave SaveManager::load_GOS()
{
	std::ifstream ifs("data.xml");//, std::ios::binary);
	cereal::XMLInputArchive archive(ifs);
	GameObjectSave s2;
	archive(s2);
	std::cout <<"Object loaded" << std::endl;
	return s2;
}

GameObjectSave SaveManager::GameObject_to_GOS(GameObject *op)
{
	GameObjectSave rs;
	rs.name = op->get_name();
	rs.number = op->get_number();
	rs.x = op->get_position().x;
	rs.y = op->get_position().y;
	rs.vx = op->get_velocity().x;
	rs.vy = op->get_velocity().y;
	rs.current_angle = op->get_current_angle();
	rs.mass = 1; //op->get_mass();
	rs.control_timer = op->get_control_timer();
	rs.action_timer = op->get_action_timer();
	rs.health = op->get_health();
	rs.mana = op->get_mana();
	rs.temperature = op->get_temperature();
	rs.controlable = op->get_controlable();
	rs.current_action = op->get_current_action();
	rs.contained = op->get_contained_status();
	//лЄгка€ часть готова
	if (rs.contained)
	{
		rs.container.first = (op->get_container())->get_name();
		rs.container.second = (op->get_container())->get_number();
	}
	//container готов
	std::map<int, action> temp_map;//временна€ мапа, в неЄ ещЄ структура action
	std::pair<int, std::string> temp_pair;//тут мы будем преобразовывать action в pair<int,string>

	temp_map = op->get_action_map();
	if (temp_map.size() != 0)
	{
		for (auto it = temp_map.begin(); it != temp_map.end(); it++)
		{
			temp_pair.first = it->second.code;			// тут мы будем преобразовывать action в pair<int, string>
			temp_pair.second = it->second.parameters;

			rs.action_map[it->first] = temp_pair;
		}
	}
	//action_map готова
	
	std::pair<std::string, int> GOS_object;
	std::list<GameObject*> temp_iol;
	temp_iol = op->get_internal_objects_list();//загрузили список внутренних объектов
	if (temp_iol.size() != 0)
	{
		for (auto it = temp_iol.begin(); it != temp_iol.end(); it++)
		{
			GameObject *b = *it;
			GOS_object.first = b->get_name();
			GOS_object.second = b->get_number();//заполнили пару информацией о текущем объекте из инвентар€
			rs.internal_objects.push_back(GOS_object);//закинули в список дл€ сохранени€
		}

	//текстуры и звуки позже
	}


	return rs;
}

void SaveManager::save_GameSave(UI_And_General* UI,std::string path)
{
	SaveManager SM;//служит дл€ перевода GameObject* в GameObjectSave, которые, затем, добавл€ютс€ в список в GameSave
	GameSave ns;//служит дл€ сохранени€ списка всех игровых объектов в виде, пригодном дл€ сохранени€
	std::list<GameObject*> temp_list;
	temp_list= UI->object_list;

	for (auto it = temp_list.begin(); it != temp_list.end(); it++)
	{
		GameObject *b = *it;
		ns.GOS_list.push_back(SM.GameObject_to_GOS(b) );//указатель на игровой объект переводитс€ в GOS, который затем вставл€етс€ в list<GOS>		
	}

	std::ofstream ofs(path);//, std::ios::binary);
	cereal::XMLOutputArchive archive(ofs);
	archive(ns);
	std::cout << "Game saved." << std::endl;


}

void SaveManager::load_GameSave(UI_And_General * UI, std::string path)
{	
	GameSave gs;
	GameObject *goa;//game object address
	GameObjectSave temp_GOS;
	std::map<int, action> restored_action_map;
	std::map<GameObject*,std::pair<std::string,int>> father_map;//карта соответстви€ сохранени€ и листа игровых объектов
	//в листе игровых объектов-адреса объектов, в листе сохранени€- пары им€ и номер
	std::map< std::pair<std::string, int>, GameObject* > save_to_pointer;//аналогична€ карта


	for (auto it = UI->object_list.begin(); it != UI->object_list.end();)//все игровые объекты удал€ютс€
	{
		GameObject *b = *it;
		it = UI->object_list.erase(it);//важно дл€ удалени€ объекта
		delete b;
		//it++;
	}
	std::ifstream ifs(path);//, std::ios::binary);
	cereal::XMLInputArchive archive(ifs);
	archive(gs);
	std::cout << "Game is loading..." << std::endl;

	for (auto it=gs.GOS_list.begin(); it != gs.GOS_list.end();it++)
	{
		temp_GOS = *it;
		std::cout << "beep  " <<temp_GOS.name<<"  "<<temp_GOS.number<< std::endl;
		//pair<int,string > to struc action
		goa = new GameObject(UI,temp_GOS);

		restored_action_map.clear();//очистить старую карту действий
		for (int i = 0; i != temp_GOS.action_map.size(); i++)//создать новую из сохранЄнки
		{
			action temp_action;
			temp_action.code = temp_GOS.action_map[i].first;
			temp_action.parameters = temp_GOS.action_map[i].second;
			restored_action_map[i] =temp_action;
		}

		goa->set_action_map(restored_action_map);
		UI->object_list.push_back(goa);

		father_map[goa] = temp_GOS.container;//установка соответстви€
		save_to_pointer[{ temp_GOS.name, temp_GOS.number }] = goa;
	}

	for (auto it = father_map.begin(); it != father_map.end();it++)
	{
		GameObject * this_object = it->first;
		if (this_object->get_contained_status())
		{
			std::pair<std::string, int> father_pair = it->second;
			GameObject * father_pointer = save_to_pointer[father_pair];
			father_pointer->capture(this_object);
		}
	}

	/*
	for (auto it = UI->object_list.begin(); it != UI->object_list.end(); it++)//идЄм в UI и перебиаем все игровые объекты
	{
		GameObject *b = *it;//b-указатель на текущий объект
		for (auto it2 = b->get_internal_objects_list.begin(); it2 != b->get_internal_objects_list.end(); it2++)//перебираем весь инвентарь игрового объекта
		{
			GameObject *bi = *it;//bi-указатель на текущий объект »Ќ¬≈Ќ“ј–я
			b->capture(bi);
		}
	}
	*/

}
