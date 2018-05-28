#pragma once
#include "Component.h"
#include "InputKeyState.h"
#include "InputAlias.h"

class Input_Component : public Component
{
public:
	int key;
	Input_Alias function;
	Key_State status;
};