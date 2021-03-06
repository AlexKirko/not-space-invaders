#pragma once

#include "Texture.h"
#include "Renderer.h"

#include "Alien.h"
#include "AlienBullet.h"
#include "Player.h"
#include "PlayerBullet.h"
#include "RenderedObject.h"
#include "RenderedString.h"

#include <algorithm>
#include <map>
#include <memory>
#include <vector>

// Manage all the game mechanics
class Game
{
private:
	float m_window_width;
	float m_window_height;
	float m_alien_speed;
	float m_player_speed;
	float m_bottom_padding;
	float m_sides_padding;
	std::shared_ptr<Renderer> m_renderer;
	std::vector<std::unique_ptr<Alien>> m_aliens;
	std::vector<std::unique_ptr<AlienBullet>> m_alien_bullets;
	std::unique_ptr<Player> m_player;
	std::vector<std::unique_ptr<PlayerBullet>> m_player_bullets;
	std::map<std::string, std::unique_ptr<RenderedString>> m_strings;
	std::shared_ptr<std::vector<std::unique_ptr<Texture>>> m_alien_textures;
	std::shared_ptr<std::vector<std::unique_ptr<Texture>>> m_player_textures;
	std::shared_ptr<std::vector<std::unique_ptr<Texture>>> m_fonts;

	int m_score;

	void move_aliens(float time_elapsed);
	void move_alien_bullets(float time_elapsed);
	void move_player_bullets(float time_elapsed);
	void move_player(float time_elapsed);

	template<typename rObjType>
	void render(std::unique_ptr<rObjType>& r_object);
	template<typename rObjType>
	void render(std::vector<std::unique_ptr<rObjType>> &r_objects);
	void render(std::map<std::string, std::unique_ptr<RenderedString>>& r_strings);
public:
	Game(float window_width, float window_height);

	void setup(int state = 0);
	void cycle(float time_elapsed, int state = 0);

	const float get_player_speed() const { return m_player_speed; }

	void spawn_alien(std::array<float, 2> bottom_left);
	void spawn_alien_row(int min_aliens, float padding);

	void aliens_shoot(float time_elapsed);

	void create_player();
	void player_shoots();

	void check_hits();
	void check_player_hits();
	void check_alien_hits();

	void display_score();
	void display_lives();

	void increase_score(int to_add);

	void move_objects(float time_elapsed);
	void render_objects();

	std::unique_ptr<Player>& get_player() { return m_player; }
};