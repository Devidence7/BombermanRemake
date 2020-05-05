//#include "Entity.h"
#include "../Include/EntitiesInclude.hpp"
#include "../Map/Level.hpp"

Entity::Entity() : Sprite() {}

sf::FloatRect Entity::getGlobalBounds() const
{
	return this->sf::Sprite::getGlobalBounds();
}

sf::Vector2f Entity::getCenterPosition()
{
	sf::FloatRect p = getGlobalBounds();
	return sf::Vector2f(p.left + p.width / 2, p.top + p.height / 2);
}

bool Entity::isColliderWith(std::shared_ptr<Entity> eCollisioning)
{
	return this->collisioner;
}

bool Entity::CanHurtPlayer()
{
	return canHurtPlayers;
}

void Entity::onCollission(std::shared_ptr<Entity> eCollisioning, CollisionType colT)
{
	sf::Vector2f position = eCollisioning->getPosition();
	switch (colT)
	{
	case HORIZONTAL:
		eCollisioning->setCollision();
		position.x += this->moveOnX(eCollisioning);
		break;
	case VERTICAL:
		eCollisioning->setCollision();
		position.y += this->moveOnY(eCollisioning);
		break;
	case CORNER:
		eCollisioning->setCollision();
		position += this->moveCircleOverCorner(eCollisioning);
		break;

	default:
		break;
	}
	eCollisioning->setPosition(position);
}

void Entity::setCollision()
{
	onCollision = true;
}

bool Entity::collision(const Entity &e) const
{
	return this->getGlobalBounds().intersects(e.getGlobalBounds());
}

void Entity::update()
{
	throw NoImplementedException("Intentando ejecutar UPDATE Entity");
}

bool Entity::getExpiredEntity()
{
	return expiredEntity;
}

void Entity::setExpiredEntity()
{
	expiredEntity = true;
	BombTaked.reset();
}

bool Entity::getIsFireDestroyable()
{
	return isFireDestroyable;
}

bool Entity::getFireCanGoThroght()
{
	return fireCanGoThroght;
}

void Entity::setObjetive(sf::Vector2f pos){
	positionObjetive = Level::getMapCellCorner(pos);
}

// PRE: this esta en colision con eCollisioning
//Devuelve cuanto penetra eCollisioning en this en el eje X; Y se considera centrado
float Entity::moveOnX(const std::shared_ptr<Entity> eCollisioning)
{

	sf::Vector2f position = eCollisioning->getCenterPosition();
	sf::FloatRect body = eCollisioning->getGlobalBounds();
	sf::Vector2f position2 = this->getCenterPosition();
	sf::FloatRect body2 = this->getGlobalBounds();
	float x = calculate_penetration(body.width / 2, body2.width / 2, abs(position.x - position2.x)) + 1;
	if (position.x < position2.x)
	{
		x = -x;
	} //retroceder
	return x;
}

// PRE: this esta en colision con eCollisioning
//Devuelve cuanto penetra eCollisioning en this en el eje Y; X se considera centrado
float Entity::moveOnY(const std::shared_ptr<Entity> eCollisioning)
{

	sf::Vector2f position = eCollisioning->getCenterPosition();
	sf::FloatRect body = eCollisioning->getGlobalBounds();
	sf::Vector2f position2 = this->getCenterPosition();
	sf::FloatRect body2 = this->getGlobalBounds();

	float y = calculate_penetration(body.height / 2, body2.height / 2, abs(position.y - position2.y)) + 1;
	if (position.y < position2.y)
	{
		y = -y;
	} //retroceder
	return y;
}

//eCirle tiene una colision con eRect. El centro de no está eCricle no está dentro de eRect
//Devuelve el la direccion (cuyo modulo es la cantidad) al que se tiene que mover la entidad eCircle que esta colisionando con eRect
inline sf::Vector2f Entity::moveCircleOverCorner(std::shared_ptr<Entity> eCircle)
{

	sf::Vector2f CenterCircle = eCircle->getCenterPosition();
	float r_Circle = eCircle->getGlobalBounds().width / 2;

	sf::Vector2f centerRect = this->getCenterPosition();
	sf::FloatRect bodyRect = this->getGlobalBounds();

	sf::Vector2f dir_centros = centerRect - CenterCircle;
	float distanciaCentros = moduleVector(dir_centros);
	dir_centros = dir_centros / distanciaCentros;
	float intersec_x;
	float intersec_y;

	//Buscar coordenadas referecia
	sf::Vector2f puntoReferenciaRect(bodyRect.left, bodyRect.top);

	if (CenterCircle.y > centerRect.y)
	{
		puntoReferenciaRect.y += bodyRect.height;
	}
	if (CenterCircle.x > centerRect.x)
	{
		puntoReferenciaRect.x += bodyRect.width;
	}
	intersec_x = intersectPoint_planeVector(CenterCircle, dir_centros, sf::Vector2f(1, 0), puntoReferenciaRect);
	intersec_y = intersectPoint_planeVector(CenterCircle, dir_centros, sf::Vector2f(0, 1), puntoReferenciaRect);

	float referencia = 0;
	float intersec = 0;
	if (CenterCircle.x >= bodyRect.left && CenterCircle.x <= bodyRect.left + bodyRect.width)
	{
		referencia = intersec_y;
	}
	else if (CenterCircle.y >= bodyRect.top && CenterCircle.y <= bodyRect.top + bodyRect.height)
	{
		referencia = intersec_x;
	}
	else
	{ //Esquina
		referencia = intersec_x > intersec_y ? intersec_x : intersec_y;
	}
	const int SEPARACION_EXTRA = 3;
	intersec = (((distanciaCentros - referencia) + r_Circle) - distanciaCentros) + SEPARACION_EXTRA;
/* 
	float moduledir = moduleVector(dir_centros);
	sf::Vector2f normalizeV = dir_centros/moduledir;
	dir_centros = dir_centros * (moduledir + 1); */
	

	return (-dir_centros) * intersec;
}

void Entity::drawEntityHitbox(sf::RenderWindow &w)
{
	sf::FloatRect dim = this->getGlobalBounds();
	sf::VertexArray lines(sf::Lines, 2);

	// Left
	lines[0].position = sf::Vector2f(dim.left, dim.top);
	lines[0].color = sf::Color::Red;
	lines[1].position = sf::Vector2f(dim.left, dim.top + dim.height);
	lines[1].color = sf::Color::Red;
	w.draw(lines);

	// top
	lines[0].position = sf::Vector2f(dim.left, dim.top);
	lines[0].color = sf::Color::Red;
	lines[1].position = sf::Vector2f(dim.left + dim.width, dim.top);
	lines[1].color = sf::Color::Red;
	w.draw(lines);

	lines[0].position = sf::Vector2f(dim.left, dim.top + dim.height);
	lines[0].color = sf::Color::Red;
	lines[1].position = sf::Vector2f(dim.left + dim.width, dim.top + dim.height);
	lines[1].color = sf::Color::Red;
	w.draw(lines);

	lines[0].position = sf::Vector2f(dim.left + dim.width, dim.top);
	lines[0].color = sf::Color::Red;
	lines[1].position = sf::Vector2f(dim.left + dim.width, dim.top + dim.height);
	lines[1].color = sf::Color::Red;
	w.draw(lines);
}
