#pragma once
#include "scene.h"

#pragma once
#include "scene.h"

class Title : public Scene
{
private:
	class Audio* m_BGM{};

public:
	void Init() override;
	void Update() override;
};

