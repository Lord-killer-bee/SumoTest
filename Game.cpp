#include "Game.h"
#include "System.h"
#include "OrthoCamera.h"
#include "Background.h"
#include "Ship.h"
#include "Asteroid.h"
#include "Explosion.h"
#include "Keyboard.h"
#include "Random.h"
#include "Maths.h"
#include "Bullet.h"
#include "Collision.h"
#include "CollisionManager.h"
#include "TimerManager.h"
#include "Font.h"
#include "Graphics.h"
#include <algorithm>

Game::Game() :
	camera_(0),
	background_(0),
	player_(0),
	collisionManager_(0),
	timerManager_(0),
	bulletTimerHandle_(0),
	playerInvulnerabilityHandle_(0),
	font_(0)
{
	camera_ = new OrthoCamera();
	camera_->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	camera_->SetFrustum(800.0f, 600.0f, -100.0f, 100.0f);
	background_ = new Background(800.0f, 600.0f);
	collisionManager_ = new CollisionManager();
	CreateTimerManager();
}

Game::~Game()
{
	delete camera_;
	delete background_;
	delete player_;
	DeleteAllBullets();
	DeleteAllAsteroids();
	DeleteAllExplosions();
	DeleteTimerManager();
	delete collisionManager_;
}

void Game::Update(System *system)
{
	UpdatePlayer(system);
	UpdateAsteroids(system);
	UpdateBullets(system);
	UpdateCollisions();
	UpdateTimers(system);
}

void Game::RenderBackgroundOnly(Graphics *graphics)
{
	camera_->SetAsView(graphics);
	background_->Render(graphics);
}

void Game::RenderEverything(Graphics *graphics)
{
	camera_->SetAsView(graphics);

	background_->Render(graphics);

	if (player_)
	{
		player_->Render(graphics);
		font_->DrawText("LIVES: " + std::to_string(player_->GetLives()), 50, 50, 0xffffff00);
	}

	for (AsteroidList::const_iterator asteroidIt = asteroids_.begin(),
		end = asteroids_.end();
		asteroidIt != end;
		++asteroidIt)
	{
		(*asteroidIt)->Render(graphics);
	}

	for(BulletList::const_iterator bulletIt = bullets_.begin(),
		end = bullets_.end();
		bulletIt != end;
		++bulletIt)
	{
		(*bulletIt)->Render(graphics);
	}

	for (ExplosionList::const_iterator explosionIt = explosions_.begin(),
		end = explosions_.end();
		explosionIt != end;
		++explosionIt)
	{
		(*explosionIt)->Render(graphics);
	}
}

void Game::InitialiseLevel(int numAsteroids)
{
	DeleteAllBullets();
	DeleteAllAsteroids();
	DeleteAllExplosions();
	
	playerLives_ = 3;//Set player lives when initializing level

	SpawnPlayer();
	SpawnAsteroids(numAsteroids);
}

void Game::InitializeUI(System *system)
{
	font_ = system->GetGraphics()->CreateXFont("Arial", 16);
}

bool Game::IsLevelComplete() const
{
	return (asteroids_.empty() && explosions_.empty());
}

bool Game::IsGameOver() const
{
	return (player_ == 0 && explosions_.empty());
}

void Game::DoCollision(GameEntity *a, GameEntity *b)
{
	Ship *player = static_cast<Ship *>(a == player_ ? a : (b == player_ ? b : 0));
	Bullet* bullet = static_cast<Bullet*>(IsBullet(a) ? a : (IsBullet(b) ? b : 0)); 
	Asteroid *asteroid = static_cast<Asteroid *>(IsAsteroid(a) ? a : (IsAsteroid(b) ? b : 0));

	if (player && asteroid)
	{
		AsteroidHit(asteroid);

		if(player_->GetLives() > 0)
		{
			player_->DeductLives();
			SetPlayerInvulnerable(1.5f);
		}
		else
			DeletePlayer();
	}

	if (bullet && asteroid)
	{
		AsteroidHit(asteroid);
		DeleteBullet(bullet);
	}
}

void Game::SpawnPlayer()
{
	DeletePlayer();
	player_ = new Ship();
	player_->SetLives(playerLives_);
	player_->EnableCollisions(collisionManager_->GetCollision(), 10.0f);
	collisionManager_->AddEntityToGrid(player_);
}

void Game::DeletePlayer()
{
	collisionManager_->RemoveEntityFromGrid(player_);
	delete player_;
	player_ = 0;
}

void Game::UpdatePlayer(System *system)
{
	if (player_ == 0)
		return;

	Keyboard *keyboard = system->GetKeyboard();

	float acceleration = 0.0f;
	if (keyboard->IsKeyHeld(VK_UP) || keyboard->IsKeyHeld('W'))
	{
		acceleration = 1.0f;
	}
	else if (keyboard->IsKeyHeld(VK_DOWN) || keyboard->IsKeyHeld('S'))
	{
		acceleration = -1.0f;
	}

	float rotation = 0.0f;
	if (keyboard->IsKeyHeld(VK_RIGHT) || keyboard->IsKeyHeld('D'))
	{
		rotation = -1.0f;
	}
	else if (keyboard->IsKeyHeld(VK_LEFT) || keyboard->IsKeyHeld('A'))
	{
		rotation = 1.0f;
	}

	player_->SetControlInput(acceleration, rotation);
	player_->Update(system);
	WrapEntity(player_);

	if (keyboard->IsKeyPressed(VK_SPACE))
	{
		D3DXVECTOR3 playerForward = player_->GetForwardVector();
		D3DXVECTOR3 bulletPosition = player_->GetPosition() + playerForward * 10.0f;

		if(CanFireBullet())
			FireRound(bulletPosition, playerForward);
	}

	if(!timerManager_->IsTimerRunning(playerInvulnerabilityHandle_) && playerInvulnerabilityHandle_ != 0)
	{
		SetPlayerVunlerable();
	}

	if(keyboard->IsKeyPressed(VK_NUMPAD1))
	{
		player_->GetAttatchedWeapon()->SwitchWeapons(SIMPLE_FIRING);
	}
	else if(keyboard->IsKeyPressed(VK_NUMPAD2))
	{
		player_->GetAttatchedWeapon()->SwitchWeapons(CONICAL_FIRING);
	}
	else if(keyboard->IsKeyPressed(VK_NUMPAD3))
	{
		player_->GetAttatchedWeapon()->SwitchWeapons(FLAK_CANNON);
	}
	

}

void Game::CreateTimerManager()
{
	DeleteTimerManager();

	timerManager_ = new TimerManager();
}
void Game::DeleteTimerManager()
{
	delete timerManager_;
	timerManager_ = 0;
}

void Game::SetPlayerInvulnerable(float duration)
{
	collisionManager_->GetCollision()->DisableCollider(player_->GetCollider());
	player_->SetInvulnerability(true);
	playerInvulnerabilityHandle_ = timerManager_->StartTimer(duration);
}

void Game::SetPlayerVunlerable()
{
	player_->SetInvulnerability(false);
	collisionManager_->GetCollision()->EnableCollider(player_->GetCollider());
}

void Game::UpdateAsteroids(System *system)
{
	for (AsteroidList::const_iterator asteroidIt = asteroids_.begin(),
		end = asteroids_.end();
		asteroidIt != end;
	++asteroidIt)
	{
		(*asteroidIt)->Update(system);
		WrapEntity(*asteroidIt);
	}
}

void Game::UpdateBullets(System *system)
{
	for(BulletList::const_iterator bulletIt = bullets_.begin(),
		end = bullets_.end();
		bulletIt != end;
		++bulletIt)
	{
		(*bulletIt)->Update(system);

		if(BulletOutOfBounds(*bulletIt))
			break;
	}
}

void Game::UpdateTimers(System* system)
{
	if(timerManager_ == 0)
		return;

	timerManager_->Update(system);
}

bool Game::IsBullet(GameEntity* entity) const
{
	//TODO : Understand the logic behind this
	return (std::find(bullets_.begin(),
		bullets_.end(), entity) != bullets_.end()); 
}

bool Game::CanFireBullet()
{
	return !(timerManager_->IsTimerRunning(bulletTimerHandle_));
}

void Game::DeleteBullet(Bullet* bullet)
{
	bullets_.remove(bullet);
	collisionManager_->RemoveEntityFromGrid(bullet);

	delete bullet;
}

void Game::WrapEntity(GameEntity *entity) const
{
	D3DXVECTOR3 entityPosition = entity->GetPosition();
	entityPosition.x = Maths::WrapModulo(entityPosition.x, -400.0f, 400.0f);
	entityPosition.y = Maths::WrapModulo(entityPosition.y, -300.0f, 300.0f);
	entity->SetPosition(entityPosition);
}

bool Game::BulletOutOfBounds(Bullet* bullet)
{
	D3DXVECTOR3 bulletPosition = bullet->GetPosition();
	if(bulletPosition.x > 400.0f || bulletPosition.x < -400.0f || bulletPosition.y > 300.0f || bulletPosition.y < -300.0f)
	{
		DeleteBullet(bullet);	
		return true;
	}
	return false;
}

void Game::DeleteAllAsteroids()
{
	for (AsteroidList::const_iterator asteroidIt = asteroids_.begin(),
		end = asteroids_.end();
		asteroidIt != end;
		++asteroidIt)
	{
		collisionManager_->RemoveEntityFromGrid(*asteroidIt);
		delete (*asteroidIt);
	}

	asteroids_.clear();
}

void Game::DeleteAllExplosions()
{
	for (ExplosionList::const_iterator explosionIt = explosions_.begin(),
		end = explosions_.end();
		explosionIt != end;
	++explosionIt)
	{
		delete (*explosionIt);
	}

	explosions_.clear();
}

void Game::FireRound(const D3DXVECTOR3 &position,
	const D3DXVECTOR3 &direction)
{
	std::list<Bullet*> bulletsFired = player_->GetAttatchedWeapon()->FireRound(position, direction);

	for(std::list<Bullet*>::iterator bulletIt = bulletsFired.begin(), end = bulletsFired.end();
		bulletIt != end;
		++bulletIt)
	{
		(*bulletIt)->EnableCollisions(collisionManager_->GetCollision(), 3.0f);
		bullets_.push_back(*bulletIt);

		collisionManager_->AddEntityToGrid(*bulletIt);
	}

	bulletTimerHandle_ = timerManager_->StartTimer(1);
}

void Game::DeleteAllBullets()
{
	for (BulletList::const_iterator bulletIt = bullets_.begin(),
		end = bullets_.end();
		bulletIt != end;
	++bulletIt)
	{
		collisionManager_->RemoveEntityFromGrid(*bulletIt);
		delete (*bulletIt);
	}

	bullets_.clear();
}

void Game::SpawnAsteroids(int numAsteroids)
{
	float halfWidth = 800.0f * 0.5f;
	float halfHeight = 600.0f * 0.5f;
	for (int i = 0; i < numAsteroids; i++)
	{
		float x = Random::GetFloat(-halfWidth, halfWidth);
		float y = Random::GetFloat(-halfHeight, halfHeight);
		D3DXVECTOR3 position = D3DXVECTOR3(x, y, 0.0f);
		SpawnAsteroidAt(position, 3);
	}
}

void Game::SpawnAsteroidAt(const D3DXVECTOR3 &position, int size)
{
	const float MAX_ASTEROID_SPEED = 1.0f;

	float angle = Random::GetFloat(Maths::TWO_PI);
	D3DXMATRIX randomRotation;
	D3DXMatrixRotationZ(&randomRotation, angle);
	D3DXVECTOR3 velocity = D3DXVECTOR3(0.0f, Random::GetFloat(MAX_ASTEROID_SPEED), 0.0f);
	D3DXVec3TransformNormal(&velocity, &velocity, &randomRotation);

	Asteroid *asteroid = new Asteroid(position, velocity, size);
	asteroid->EnableCollisions(collisionManager_->GetCollision(), size * 5.0f);
	asteroids_.push_back(asteroid);

	collisionManager_->AddEntityToGrid(asteroid);
}

bool Game::IsAsteroid(GameEntity *entity) const
{
	return (std::find(asteroids_.begin(),
		asteroids_.end(), entity) != asteroids_.end()); 
}

void Game::AsteroidHit(Asteroid *asteroid)
{
	int oldSize = asteroid->GetSize();
	if (oldSize > 1)
	{
		int smallerSize = oldSize -1;
		D3DXVECTOR3 position = asteroid->GetPosition();
		SpawnAsteroidAt(position, smallerSize);
		SpawnAsteroidAt(position, smallerSize);
	}
	DeleteAsteroid(asteroid);
}

void Game::DeleteAsteroid(Asteroid *asteroid)
{
	asteroids_.remove(asteroid);
	collisionManager_->RemoveEntityFromGrid(asteroid);

	delete asteroid;
}

void Game::UpdateCollisions()
{
	collisionManager_->UpdateEntitiesInGrid();
	collisionManager_->CalculateCollisionDetection(this);// TODO: Call "DoCollisions" through this function
	//collisionManager_->GetCollision()->DoCollisions(this);
}

void Game::DeactivateUI(System* system)
{
	system->GetGraphics()->DestroyXFont(font_);
	font_ = 0;
}