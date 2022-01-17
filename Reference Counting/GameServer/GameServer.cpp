#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <future>
#include <Windows.h>
#include "ThreadManager.h"

class Wraight
{
public:
	int _hp = 150;
	int _posX = 0;
	int _posY = 0;


};

class Missile
{
public:
	void SetTarget(Wraight* target)
	{
		_target = target;
	}

	void Update()
	{
		int _posX = _target->_posX;
		int _posY = _target->_posY;
	}

	// TODO: 쫒아간다.


	Wraight* _target = nullptr;


};

int main()
{
	Wraight* wraight = new Wraight();
	Missile* missile = new Missile();
	missile->SetTarget(wraight);

	//레이스가 피격 당함
	wraight->_hp = 0;
	delete wraight;


	while (true)
	{
		if (missile)
		{
			missile->Update();

		}
	}


}
