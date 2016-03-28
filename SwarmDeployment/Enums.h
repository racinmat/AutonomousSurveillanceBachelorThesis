#pragma once

namespace App
{
	enum class Grid { Free, Obstacle, UAV, Goal };
	enum class NNMethod {Total, Min, Max};
	enum class PlacementMethod { Standard, Chain };
}