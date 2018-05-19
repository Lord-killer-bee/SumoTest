#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <d3dx9math.h>
#include <list>

class OrthoCamera;
class Background;
class Ship;
class Bullet;
class Asteroid;
class Explosion;
class Collision;
class System;
class Graphics;
class GameEntity;
class TimerManager;

class Game
{
public:
	Game();
	~Game();

	void Update(System *system);
	void RenderBackgroundOnly(Graphics *graphics);
	void RenderEverything(Graphics *graphics);

	void InitialiseLevel(int numAsteroids);
	bool IsLevelComplete() const;
	bool IsGameOver() const;

	void DoCollision(GameEntity *a, GameEntity *b);

private:
	Game(const Game &);
	void operator=(const Game &);

	typedef std::list<Asteroid *> AsteroidList;
	typedef std::list<Bullet *> BulletList;
	typedef std::list<Explosion *> ExplosionList;

	void SpawnPlayer();
	void DeletePlayer();

	void UpdatePlayer(System *system);
	void UpdateAsteroids(System *system);
	void UpdateBullets(System *system);
	void UpdateTimers(System* system);
	void WrapEntity(GameEntity *entity) const;

	void DeleteAllBullets();
	void DeleteAllAsteroids();
	void DeleteAllExplosions();

	void CreateTimerManager();
	void DeleteTimerManager();

	void SpawnBullet(const D3DXVECTOR3 &position,
		const D3DXVECTOR3 &direction);
	bool IsBullet(GameEntity* entity);
	bool CanFireBullet();
	void DeleteBullet(Bullet* bullet);
	bool BulletOutOfBounds(Bullet* bullet);

	void SpawnAsteroids(int numAsteroids);
	void SpawnAsteroidAt(const D3DXVECTOR3 &position, int size);
	bool IsAsteroid(GameEntity *entity) const;
	void AsteroidHit(Asteroid *asteroid);
	void DeleteAsteroid(Asteroid *asteroid);

	void UpdateCollisions();

	int bulletTimerHandle_;

	OrthoCamera *camera_;
	TimerManager *timerManager_;

	Background *background_;
	Ship *player_;

	BulletList bullets_;
	AsteroidList asteroids_;
	ExplosionList explosions_;

	Collision *collision_;
};

#endif // GAME_H_INCLUDED
