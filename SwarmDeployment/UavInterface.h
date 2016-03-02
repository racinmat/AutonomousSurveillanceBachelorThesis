#pragma once
#include "PointParticle.h"

namespace App
{
	class UavInterface
	{
	public:
		UavInterface();
		explicit UavInterface(shared_ptr<PointParticle> pointParticle);
		virtual ~UavInterface();
		virtual shared_ptr<PointParticle> getPointParticle() const;

	protected:
		shared_ptr<PointParticle> pointParticle;
	
	};

}
