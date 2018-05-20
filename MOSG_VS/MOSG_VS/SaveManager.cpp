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
	//����� ����� ������
	if (rs.contained)
	{
		rs.container.first = (op->get_container())->get_name();
		rs.container.second = (op->get_container())->get_number();
	}
	//container �����
	std::map<int, action> temp_map;//��������� ����, � �� ��� ��������� action
	std::pair<int, std::string> temp_pair;//��� �� ����� ��������������� action � pair<int,string>

	temp_map = op->get_action_map();
	if (temp_map.size() != 0)
	{
		for (auto it = temp_map.begin(); it != temp_map.end(); it++)
		{
			temp_pair.first = it->second.code;			// ��� �� ����� ��������������� action � pair<int, string>
			temp_pair.second = it->second.parameters;

			rs.action_map[it->first] = temp_pair;
		}
	}
	//action_map ������
	
	std::pair<std::string, int> GOS_object;
	std::list<GameObject*> temp_iol;
	temp_iol = op->get_internal_objects_list();//��������� ������ ���������� ��������
	if (temp_iol.size() != 0)
	{
		for (auto it = temp_iol.begin(); it != temp_iol.end(); it++)
		{
			GameObject *b = *it;
			GOS_object.first = b->get_name();
			GOS_object.second = b->get_number();//��������� ���� ����������� � ������� ������� �� ���������
			rs.internal_objects.push_back(GOS_object);//�������� � ������ ��� ����������
		}

	//�������� � ����� �����
	}


	return rs;
}

void SaveManager::save_GameSave(UI_And_General* UI,std::string path)
{
	SaveManager SM;//������ ��� �������� GameObject* � GameObjectSave, �������, �����, ����������� � ������ � GameSave
	GameSave ns;//������ ��� ���������� ������ ���� ������� �������� � ����, ��������� ��� ����������
	std::list<GameObject*> temp_list;
	temp_list= UI->object_list;

	for (auto it = temp_list.begin(); it != temp_list.end(); it++)
	{
		GameObject *b = *it;
		ns.GOS_list.push_back(SM.GameObject_to_GOS(b) );//��������� �� ������� ������ ����������� � GOS, ������� ����� ����������� � list<GOS>		
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
	std::map<GameObject*,std::pair<std::string,int>> father_map;//����� ������������ ���������� � ����� ������� ��������
	//� ����� ������� ��������-������ ��������, � ����� ����������- ���� ��� � �����
	std::map< std::pair<std::string, int>, GameObject* > save_to_pointer;//����������� �����


	for (auto it = UI->object_list.begin(); it != UI->object_list.end();)//��� ������� ������� ���������
	{
		GameObject *b = *it;
		it = UI->object_list.erase(it);//����� ��� �������� �������
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

		restored_action_map.clear();//�������� ������ ����� ��������
		for (int i = 0; i != temp_GOS.action_map.size(); i++)//������� ����� �� ���������
		{
			action temp_action;
			temp_action.code = temp_GOS.action_map[i].first;
			temp_action.parameters = temp_GOS.action_map[i].second;
			restored_action_map[i] =temp_action;
		}

		goa->set_action_map(restored_action_map);
		UI->object_list.push_back(goa);

		father_map[goa] = temp_GOS.container;//��������� ������������
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
	for (auto it = UI->object_list.begin(); it != UI->object_list.end(); it++)//��� � UI � ��������� ��� ������� �������
	{
		GameObject *b = *it;//b-��������� �� ������� ������
		for (auto it2 = b->get_internal_objects_list.begin(); it2 != b->get_internal_objects_list.end(); it2++)//���������� ���� ��������� �������� �������
		{
			GameObject *bi = *it;//bi-��������� �� ������� ������ ���������
			b->capture(bi);
		}
	}
	*/

}
