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


//каждый GameObject преобразуетс€ в GameObjectSave, которые объедин€ютс€ в GOS_list в классе GameSave

class GameObject;
class GameObjectSave//он же GOS. Ётот класс позвол€ет сохран€ть ќ“ƒ≈Ћ№Ќџ≈ игровые объекты
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

	float control_timer = 0;//потом сделаю список таймеров, еще думаю над этим
	float action_timer = 0; //ведь таймеры нужны разные
							// и к ним нужен удобный доступ (по номеру или адресу, хз)

	int health;
	int mana;
	int temperature;

	bool controlable;   //можно ли им управл€ть
	int current_action;
	bool contained;//‘лаг нахождени€ в другом предмете(инвентаре)//+


	std::map<int, std::pair<int,std::string>> action_map;        //поседовательность действий, кторые объект выполн€ет
	std::list<std::pair<std::string,int>> internal_objects;//список внутренних объектов
	std::pair<std::string, int > container;
	std::map<int, std::string> texture_files, sound_files;




	friend class cereal::access;//это нужно дл€ поддержки контейнеров stl(также требуетс€ подключить соответствующие заголвочные файлы) 

	template<class Archive>//шаблон cereal дл€ сериализации данных
	void serialize (Archive &archive)
	{
		
		archive(CEREAL_NVP(name), CEREAL_NVP(number), CEREAL_NVP(x), CEREAL_NVP(y),//что и как сериализовать
			CEREAL_NVP(vx), CEREAL_NVP(vy), CEREAL_NVP(current_angle), CEREAL_NVP(angular_velocity),
			CEREAL_NVP(mass), CEREAL_NVP(control_timer), CEREAL_NVP(action_timer), CEREAL_NVP(health), CEREAL_NVP(mana),
			CEREAL_NVP(temperature), CEREAL_NVP(controlable), CEREAL_NVP(current_action), CEREAL_NVP(contained),//+
			CEREAL_NVP(action_map), CEREAL_NVP(internal_objects), CEREAL_NVP(container),
			CEREAL_NVP(texture_files), CEREAL_NVP(sound_files));
			
	}

};

class GameSave//Ётот класс сохран€ет ¬—≈ игровые объекты
{	
public:
	std::list<GameObjectSave> GOS_list;
	
	template<class Archive>//шаблон cereal дл€ сериализации данных
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
