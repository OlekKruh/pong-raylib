#pragma once

#include <raylib.h>
#include <vector>
#include <string>

const int window_width = 1280;
const int window_height = 800;
const std::string game_name = "PingPong Game";
const Color transparent_yellow = { 253, 249, 0, 20 };

const int max_score = 5;

enum class PlayerType { Blue, Red, Other };
enum class MenuOption { OnePlayer, TwoPlayers, Exit };

class GameController
{
	// private constructor allows creation from this class only
	GameController() {}

	// explicitly disable copying and assignment
	GameController(GameController&&) = delete;
	GameController(const GameController&) = delete;
	GameController& operator=(GameController&&) = delete;
	GameController& operator=(const GameController&) = delete;

public:
	// https://refactoring.guru/design-patterns/singleton/cpp/example#example-0
	static GameController* GetInstance();

	void IncreaseCpuScore();
	void IncreasePlayerScore();

	int GetCpuScore() const { return cpu_score; }
	int GetPlayerScore() const { return player_score; }

private:
	int player_score = 0;
	int cpu_score = 0;
};


class IGameObject
{
public:
	virtual void Draw() const = 0;
	virtual void Update() = 0;

	virtual ~IGameObject() {};
};

class Ball : public IGameObject
{
public:
	Ball(float _x, float _y, float _speed_x, float _speed_y, float _radius, Color _color) :
		x(_x),
		y(_y),
		speed_x(_speed_x),
		speed_y(_speed_y),
		radius(_radius),
		color(_color) {
	}

	~Ball() override = default;

	void Draw() const override;
	void Update() override;

private:
	void ResetBall();

public:
	float x, y;
	float speed_x, speed_y;
	float radius;
	Color color;
};

class Paddle : public IGameObject
{
public:
	Paddle (float _x, float _y, float _speed_y, float _width, float _height, Color _color, PlayerType _type) :
		x(_x),
		y(_y),
		speed_y(_speed_y),
		padl_width(_width),
		padl_height(_height),
		color(_color),
		type(_type) {
	}

	Paddle (const Paddle& paddle) :
		x(paddle.x),
		y(paddle.y),
		speed_y(paddle.speed_y),
		padl_width(paddle.padl_width),
		padl_height(paddle.padl_height),
		color(paddle.color),
		type(paddle.type) {
	}

	~Paddle() override = default;

	void Draw() const override;
	void Update() override;

protected:
	void LimitMovement();

public:
	float x, y;
	float speed_y;
	float padl_width, padl_height;
	Color color;
	PlayerType type;
};

class CpuPaddle : public Paddle
{
public:
	CpuPaddle(float _x, float _y, float _speed_y, float _width, float _height, Color _color) : 
		Paddle(_x, _y, _speed_y, _width, _height, _color, PlayerType::Other)
	{}

	CpuPaddle(const Paddle& paddle) :
		Paddle(paddle.x, paddle.y, paddle.speed_y, paddle.padl_width, paddle.padl_height, paddle.color, PlayerType::Other)
	{}

	~CpuPaddle() override = default;

	void Update() override;
	void SetBallY(int _ball_y) { ball_y = _ball_y; }

private:
	int ball_y = 0;
};
