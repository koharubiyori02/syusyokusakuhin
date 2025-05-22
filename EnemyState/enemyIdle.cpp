#include "EnemyState.h"
#include "../enemy.h"
#include "EnemyMove.h"
#include "EnemyIdle.h"

void EnemyIdle::Start()
{
	GetEnemy()->SetEnemyState<EnemyMove>();
}