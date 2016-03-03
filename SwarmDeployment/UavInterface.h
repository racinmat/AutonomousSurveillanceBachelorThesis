#pragma once
#include "PointParticle.h"

namespace App
{
	class UavInterface
	{
	public:
		UavInterface();
		UavInterface(const UavInterface& other);
		explicit UavInterface(shared_ptr<PointParticle> pointParticle);
		virtual ~UavInterface();
		virtual shared_ptr<PointParticle> getPointParticle() const;
		friend bool operator<(const UavInterface& lhs, const UavInterface& rhs);	//kvùli používání jako klíèe v std::map
		friend bool operator<=(const UavInterface& lhs, const UavInterface& rhs);
		friend bool operator>(const UavInterface& lhs, const UavInterface& rhs);
		friend bool operator>=(const UavInterface& lhs, const UavInterface& rhs);
		friend bool operator==(const UavInterface& lhs, const UavInterface& rhs);
		friend bool operator!=(const UavInterface& lhs, const UavInterface& rhs);
		virtual int getId() const;
		virtual void setId(const int id);
		virtual void setPointParticle(const shared_ptr<PointParticle> point_particle);
		size_t hash_value() const;

	protected:
		shared_ptr<PointParticle> pointParticle;
		int id;
		static int lastId;
	};

	class UavHasher
	{
	public:
		size_t operator() (UavInterface const& key) const
		{
			return key.hash_value();
		}
	};

}
