#include "UavInterface.h"

namespace App
{
	int UavInterface::lastId = 0;

	UavInterface::UavInterface() : id(lastId++)
	{
	}

	UavInterface::UavInterface(const UavInterface& other) : pointParticle(make_shared<PointParticle>(*other.pointParticle.get()))
	{
		id = other.id;
	}

	UavInterface::UavInterface(shared_ptr<PointParticle> pointParticle) :
		pointParticle(pointParticle), 
		id(lastId++)
	{

	}
	
	UavInterface::~UavInterface()
	{
	}

	shared_ptr<PointParticle> UavInterface::getPointParticle() const
	{
		return pointParticle;
	}

	bool operator<(const UavInterface& lhs, const UavInterface& rhs)
	{
		return lhs.id < rhs.id;
	}

	bool operator<=(const UavInterface& lhs, const UavInterface& rhs)
	{
		return !(rhs < lhs);
	}

	bool operator>(const UavInterface& lhs, const UavInterface& rhs)
	{
		return rhs < lhs;
	}

	bool operator>=(const UavInterface& lhs, const UavInterface& rhs)
	{
		return !(lhs < rhs);
	}

	bool operator==(const UavInterface& lhs, const UavInterface& rhs)
	{
		return lhs.id == rhs.id;
	}

	bool operator!=(const UavInterface& lhs, const UavInterface& rhs)
	{
		return !(lhs == rhs);
	}

	int UavInterface::getId() const
	{
		return id;
	}

	void UavInterface::setId(const int id)
	{
		this->id = id;
	}

	void UavInterface::setPointParticle(const shared_ptr<PointParticle> point_particle)
	{
		pointParticle = point_particle;
	}

	size_t UavInterface::hash_value() const
	{
		size_t seed = 0x28003F72;
		seed ^= (seed << 6) + (seed >> 2) + 0x1B543A89 + static_cast<size_t>(id);
		return seed;
	}

}

