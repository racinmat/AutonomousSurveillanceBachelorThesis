#include "UavInterface.h"

namespace App
{
		
	UavInterface::UavInterface()
	{
	}
	
	UavInterface::UavInterface(shared_ptr<PointParticle> pointParticle) :
		pointParticle(pointParticle)
	{
	}
	
	UavInterface::~UavInterface()
	{
	}

	shared_ptr<PointParticle> UavInterface::getPointParticle() const
	{
		return pointParticle;
	}
}

