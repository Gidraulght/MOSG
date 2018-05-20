#ifndef ACTIVEGAMEOBJECT_H
#define ACTIVEGAMEOBJECT_H

#include "GameObject.h"


class ActiveGameObject : public GameObject
{
    public:
        ActiveGameObject();
        ActiveGameObject(int);

        //ActiveGameObject(sf::Texture *ptr,bool,int,int, float,sf::Vector2f,std::string);
        virtual ~ActiveGameObject();
        void random_walk();

    protected:

    private:
};

#endif // ACTIVEGAMEOBJECT_H
