#pragma once
#include <string>
#include <fstream>
#include "GameObject.h"
#include <map>
#include <list>
#include <utility>      // std::pair

#include <cereal/types/map.hpp>
#include <cereal/types/list.hpp>
#include <cereal/types/utility.hpp> //std::pair support


#include <cereal/types/map.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/complex.hpp>

#include <cereal/archives/xml.hpp>


//������ GameObject ������������� � GameObjectSave, ������� ������������ � GOS_list � ������ GameSave

class GameObject;
class GameObjectSave//�� �� GOS. ���� ����� ��������� ��������� ��������� ������� �������
{ 
public:
	std::string name;
	int number;

	float x, y;
	float vx, vy;
	//float desired_angle;
	float current_angle;
	float angular_velocity;
	float mass;

	float control_timer = 0;//����� ������ ������ ��������, ��� ����� ��� ����
	float action_timer = 0; //���� ������� ����� ������
							// � � ��� ����� ������� ������ (�� ������ ��� ������, ��)

	int health;
	int mana;
	int temperature;

	bool controlable;   //����� �� �� ���������
	int current_action;
	bool contained;//���� ���������� � ������ ��������(���������)//+


	std::map<int, std::pair<int,std::string>> action_map;        //����������������� ��������, ������ ������ ���������
	std::list<std::pair<std::string,int>> internal_objects;//������ ���������� ��������
	std::pair<std::string, int > container;
	std::map<int, std::string> texture_files, sound_files;




	friend class cereal::access;//��� ����� ��� ��������� ����������� stl(����� ��������� ���������� ��������������� ����������� �����) 

	template<class Archive>//������ cereal ��� ������������ ������
	void serialize (Archive &archive)
	{
		
		archive(CEREAL_NVP(name), CEREAL_NVP(number), CEREAL_NVP(x), CEREAL_NVP(y),//��� � ��� �������������
			CEREAL_NVP(vx), CEREAL_NVP(vy), CEREAL_NVP(current_angle), CEREAL_NVP(angular_velocity),
			CEREAL_NVP(mass), CEREAL_NVP(control_timer), CEREAL_NVP(action_timer), CEREAL_NVP(health), CEREAL_NVP(mana),
			CEREAL_NVP(temperature), CEREAL_NVP(controlable), CEREAL_NVP(current_action), CEREAL_NVP(contained),//+
			CEREAL_NVP(action_map), CEREAL_NVP(internal_objects), CEREAL_NVP(container),
			CEREAL_NVP(texture_files), CEREAL_NVP(sound_files));
			
	}

};

class GameSave//���� ����� ��������� ��� ������� �������
{	
public:
	std::list<GameObjectSave> GOS_list;
	
	template<class Archive>//������ cereal ��� ������������ ������
	void serialize(Archive &archive)
	{

		archive(CEREAL_NVP(GOS_list));

	}


};





class SaveManager
{
public:
	SaveManager();
	~SaveManager();
	void save_GOS(GameObjectSave);
	GameObjectSave load_GOS();
	GameObjectSave GameObject_to_GOS(GameObject*);
	void save_GameSave(UI_And_General*, std::string);
	void load_GameSave(UI_And_General*, std::string);




};
