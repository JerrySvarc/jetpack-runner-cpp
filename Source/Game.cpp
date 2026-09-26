#include "Game.h"
#include <memory>
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "Zapper.h"
#include "Items.h"
#include <cctype>
#include <stdlib.h>  
#include <iostream>
#include <string> 

/// <summary>
/// Starts the game loop. Game first initializes the player, zappers and the background. Then it enters a while loop while detecting events generating by the user. 
/// The loop consists of calculating the movement of the player, zappers and the background, changing their positions inside the window, detecting any collisions
/// and changing textures to animate the sprite.
/// If the player runs into a zapper, the game can be restarted by pressing 'R' on their keyboard. 
/// </summary>
void Game::run() {
    init_game();

    double run_idx = 0;
    double animation_idx = 0;

    sf::Clock clock;
    //Game loop starts here
    while (this->window_->isOpen()) {
        sf::Time elapsed = clock.getElapsedTime();
        sf::Event event; 
        while (this->window_->pollEvent(event)) {
            //If the player is dead and the player wants to restart the game
            if (this->game_over_ && event.type == sf::Event::KeyPressed 
                && event.key.code ==  sf::Keyboard::R)
            {
                reset_game();
                this->game_over_ = false;
            }
            //Player clicks on the X button to close the window
            if (event.type == sf::Event::Closed) {
                this->window_->close();
            }
            //Spacebar is pressed
            if (event.type == sf::Event::KeyPressed 
                && event.key.code == sf::Keyboard::Space) {
                this->player_->uplift = this->jetpack_power_;
                if (!this->gravity_reverser_used_ && this->player_->sprite->getPosition().y <= this->ceiling_height) {
                    this->gravity_ = 0;
                    this->player_->uplift = 0;
                }
                else if (this->gravity_reverser_used_ && this->player_->sprite->getPosition().y >= this->player_->player_y_max) {
                    this->gravity_ = 0;
                    this->player_->uplift = 0;
                }
                
            }
            //Spacebar is released
            if (event.type == sf::Event::KeyReleased 
                && event.key.code == sf::Keyboard::Space) {
                this->player_->uplift = 0;
                this->gravity_ = 8;
            }
        }
        //If the player is alive
        if (!this->game_over_)
        {
            //Every 15 milliseconds update the game
            if (elapsed > sf::milliseconds(this->refresh_rate_ms_)) {
                this->window_->clear();
                this->scroll_background();
                run_idx += this->speed_ / this->speed_divisor_;
                animation_idx += this->speedup_constatnt_;
                this->animate_entities(run_idx, animation_idx);
                this->move_entities();
                this->game_over_ = check_collision();
                sf::Text text("Coins = " + std::to_string(this->coins_), this->font_);
                this->score_text_ = text;
                this->score_text_.setCharacterSize(this->char_size);
                this->score_text_.setPosition(this->text_x_pos, this->text_y_pos);
                this->score_text_.setFillColor(sf::Color::White);
                this->render();
                this->increase_speed();
                clock.restart();
            }
        }
        else {
            if (elapsed > sf::milliseconds(this->refresh_rate_ms_)) {
               animate_player(0);
               this->render();
               animation_idx += this->speedup_constatnt_;
               this->animate_zappers((int)animation_idx % this->zapper_animation_texture_mod_);
               this->animate_coins((int)animation_idx % this->coin_animation_texture_mod_);
               clock.restart();
               }
        }
    }
}

/// <summary>
/// Game initialization consists of initiliazing the player, the sfml window, background and zappers.
/// </summary>
void Game::init_game() {
    this->player_ = std::make_unique<Player>();
    this->player_->init_player();
    init_window();
    init_text();
    init_background();
    init_coins();
    init_reverser();
    init_zappers();
}

/// <summary>
/// Creates a new sfml window. Window is purposefully not resizible.
/// </summary>
void Game::init_window() {
    this->window_ = std::make_unique<sf::RenderWindow>(
        sf::VideoMode(this->background_width, background_height), "Jetpack Runner",
        sf::Style::Titlebar | sf::Style::Close);
}

/// <summary>
/// Creates the text which shows the number of coins collected.
/// </summary>
void Game::init_text() {
    //Load and check the availability of the font file
    if (!this->font_.loadFromFile("Resources/Arialn.ttf"))
    {
        std::cout << "can't load font" << std::endl;
    }
    sf::Text text("Coins = 0", this->font_);
    this->score_text_ = text;
    //Set character size
    this->score_text_.setCharacterSize(char_size);
    this->score_text_.setPosition(text_x_pos, text_y_pos);
    //Set fill color
    this->score_text_.setFillColor(sf::Color::White);
}


/// <summary>
/// Initializes the game background. Loads the texture resources and creates background sprites and sets their position.
/// </summary>
void Game::init_background() {
    this->background_texture_ = std::make_unique<sf::Texture>();
    background_texture_->loadFromFile("Resources/BackdropMain.png");
    this->background_ = std::make_unique<sf::Sprite>(*this->background_texture_);
    this->background2_ = std::make_unique<sf::Sprite>(*this->background_texture_);
    this->background2_->setPosition(this->background_width, 0);
}

/// <summary>
/// Initializes the three zappers.
/// </summary>
void Game::init_zappers() {
    for (size_t i = 0; i < 3; i++)
    {
        this->zappers_.push_back(std::make_unique<Zapper>());
        this->zappers_[i]->init_zapper();
        this->zappers_[i]->sprite->setPosition(zapper_start_x + i * zapper_spacing, (rand() % zapper_height_mod) * entity_height_multiplier);
    }
}

/// <summary>
/// Initializes the three zappers.
/// </summary>
void Game::init_coins() {
    for (size_t i = 0; i < 3; i++)
    {
        this->coin_entities_.push_back(std::make_unique<Coin>());
        this->coin_entities_[i]->init_coin();
    }
    this->coin_entities_[0]->sprite->setPosition(500, ((rand() % coin_height_mod) * entity_height_multiplier)+ this->ceiling_height);
    this->coin_entities_[1]->sprite->setPosition(1000, ((rand() % coin_height_mod) * entity_height_multiplier) + this->ceiling_height);
    this->coin_entities_[2]->sprite->setPosition(1500, ((rand() % coin_height_mod) * entity_height_multiplier) + this->ceiling_height);
}

void Game::init_reverser() {
    this->gravity_reverser_ = std::make_unique<GravityReverser>();
    this->gravity_reverser_->init_reverser();
    this->gravity_reverser_->sprite->setPosition(2369, ((rand() % coin_height_mod) * entity_height_multiplier) + this->ceiling_height);
}


/// <summary>
/// Resets the player and zapper positions along with the game speed.
/// </summary>
void Game::reset_game()
{
    this->speed_ = 2;
    reset_player();
    reset_zappers();
    reset_coins();
    reset_reverser();
}

/// <summary>
/// Checks wether the player sprite intersects with a zapper sprite.
/// </summary>
/// <returns>True if collision happens. False otherwise.</returns>
bool Game::check_collision() {
    for (size_t i = 0; i < 3; i++)
    {
        if (this->player_->sprite->getGlobalBounds().intersects(
            this->zappers_[i]->sprite->getGlobalBounds())){
            return true;
        }
        if (this->player_->sprite->getGlobalBounds().intersects(
            this->coin_entities_[i]->sprite->getGlobalBounds())) {
            coin_entities_[i]->affect_game(*this);
            this->coin_entities_[i]->sprite->setPosition(coin_entities_[i]->sprite->getPosition().x + background_width + coin_start_x,
                ((rand() % coin_height_mod) * entity_height_multiplier) + this->ceiling_height);
        }
        if (this->player_->sprite->getGlobalBounds().intersects(
            this->gravity_reverser_->sprite->getGlobalBounds()) && !this->gravity_reverser_used_) {
            gravity_reverser_->affect_game(*this);
            this->gravity_reverser_->sprite->setPosition(gravity_reverser_->sprite->getPosition().x + reverser_spacing,
                ((rand() % coin_height_mod) * entity_height_multiplier) + this->ceiling_height);
        }
        else if ((this->player_->sprite->getGlobalBounds().intersects(
            this->gravity_reverser_->sprite->getGlobalBounds()) && this->gravity_reverser_used_)) {
            gravity_reverser_->reset_effect(*this);
            this->gravity_reverser_->sprite->setPosition(gravity_reverser_->sprite->getPosition().x + reverser_spacing,
                ((rand() % coin_height_mod) * entity_height_multiplier) + this->ceiling_height);
        }
    }
    return false;
}

/// <summary>
/// Animates all entities on the screen. 
/// </summary>
/// <param name="run_idx">The index of the running texture which should be used.</param>
/// <param name="animation_idx">The index of the texture of other entities which should be used.</param>
void Game::animate_entities(double run_idx, double animation_idx) {
    this->animate_player((int)run_idx % runner_animation_texture_mod_);
    this->animate_zappers((int)animation_idx % zapper_animation_texture_mod_);
    this->animate_coins((int)animation_idx % coin_animation_texture_mod_);
    this->animate_reverser((int)animation_idx % reverser_animation_texture_mod_);
}

/// <summary>
/// Draws all elements to the window. Also calls display() method on the window.
/// </summary>
void Game::render() {
    this->window_->draw(*this->background_);
    this->window_->draw(*this->background2_);
    this->window_->draw(*this->player_->sprite);
    for (size_t i = 0; i < 3; i++)
    {
        this->window_->draw(*this->zappers_[i]->sprite);
    }
    for (size_t i = 0; i < 3; i++)
    {
        this->window_->draw(*this->coin_entities_[i]->sprite);
    }
    this->window_->draw(*this->gravity_reverser_->sprite);
    this->window_->draw(score_text_);
    this->window_->display();
}

/// <summary>
/// Resets zappers to the starting positions.
/// </summary>
void Game::reset_zappers() {
    //maximum y position is 300
    this->zappers_[0]->sprite->setPosition(zapper_start_x, (rand() % zapper_height_mod) * entity_height_multiplier);
    this->zappers_[1]->sprite->setPosition(zapper_start_x + zapper_spacing, (rand() % zapper_height_mod) * entity_height_multiplier);
    this->zappers_[2]->sprite->setPosition(zapper_start_x + 2*zapper_spacing, (rand() % zapper_height_mod) * entity_height_multiplier);
}

void Game::increase_speed() { this->speed_ += 0.001; }


/// <summary>
/// Resets the player to the starting position.
/// </summary>
void Game::reset_player() {
    this->player_->change_sprite_texture(0);
    this->player_->sprite->setPosition(this->player_->player_x_pos, this->player_->player_y_max);
}

/// <summary>
/// Resets the coins to the starting position.
/// </summary>
void Game::reset_coins() {
    this->coin_entities_[0]->sprite->setPosition(coin_start_x, ((rand() % coin_height_mod) * entity_height_multiplier) + this->ceiling_height);
    this->coin_entities_[1]->sprite->setPosition(2*coin_start_x, ((rand() % coin_height_mod) * entity_height_multiplier) + this->ceiling_height);
    this->coin_entities_[2]->sprite->setPosition(3*coin_start_x, ((rand() % coin_height_mod) * entity_height_multiplier) + this->ceiling_height);
    this->coins_ = 0;
}

/// <summary>
/// Resets the reverser to the starting position.
/// </summary>
void Game::reset_reverser() {
    this->gravity_reverser_->sprite->setPosition(500, ((rand() % coin_height_mod) * entity_height_multiplier) + this->ceiling_height);
    if (this->gravity_reverser_used_)
    {
        this->gravity_reverser_->reset_effect(*this);
    }
}

/// <summary>
/// Animates the player based on wether he is flying or not. Also animates 
/// </summary>
/// <param name="index"></param>
void Game::animate_player(int index) {
    if (this->game_over_) {
        this->player_->change_sprite_texture(4);
    } else if ((!this->gravity_reverser_used_ && this->player_->sprite->getPosition().y < this->player_->player_y_max) || (this->gravity_reverser_used_ && this->player_->sprite->getPosition().y > this->ceiling_height)) {
        this->player_->change_sprite_texture(3);
    } else {
        this->player_->change_sprite_texture(index);
    }
}

/// <summary>
/// Animates the zappers based on the animation index.
/// </summary>
/// <param name="index">Index of the next texture for the sprite.</param>
void Game::animate_zappers(int index) {
    for (size_t i = 0; i < 3; i++)
    {
        this->zappers_[i]->change_sprite_texture(index);
    }
}

/// <summary>
/// Animates the coins based on the animation index.
/// </summary>
/// <param name="index">Index of the next texture for the sprite.</param>
void Game::animate_coins(int index) {
    for (size_t i = 0; i < 3; i++)
    {
       this->coin_entities_[i]->change_sprite_texture(index);
    }
}

/// <summary>
/// Animates the reverser based on the animation index.
/// </summary>
/// <param name="index">Index of the next texture for the sprite.</param>
void Game::animate_reverser(int index) {
    this->gravity_reverser_->change_sprite_texture(index);
}

/// <summary>
/// Moves all entites on the screen to the next location.
/// </summary>
void Game::move_entities() {
    this->move_player();
    this->move_zappers();
    this->move_coins();
    this->move_reverser();
}

/// <summary>
/// Moves the player based on his current height and wether he would run over the ceiling or the floor barriers.
/// </summary>
void Game::move_player() {
    float new_height;
    if (this->gravity_reverser_used_) {
          new_height = this->player_->sprite->getPosition().y - this->gravity_ +
            this->player_->uplift;
    }
    else {
        new_height = this->player_->sprite->getPosition().y + this->gravity_ -
            this->player_->uplift;
    }
    
    if (new_height > this->player_->player_y_max) {
        this->player_->sprite->setPosition(this->player_->player_x_pos, this->player_->player_y_max);
    } else if (new_height < this->ceiling_height) {
        this->player_->sprite->setPosition(this->player_->player_x_pos, this->ceiling_height);
    } else {
        this->player_->sprite->setPosition(this->player_->player_x_pos, new_height);
    }
}

/// <summary>
/// Moves the zappers to a new position. If a zapper moves out of frame, new random y position is generated.
/// </summary>
void Game::move_zappers() {
    for (size_t i = 0; i < 3; i++)
    {
        this->zappers_[i]->sprite->setPosition(zappers_[i]->sprite->getPosition().x 
            - this->speed_, zappers_[i]->sprite->getPosition().y);
        if (zappers_[i]->sprite->getPosition().x < this->zapper_cutoff) {
            this->zappers_[i]->sprite->setPosition(zappers_[i]->sprite->getPosition().x + background_width + zapper_spacing,
                 (rand() % zapper_height_mod) * entity_height_multiplier); //maximum y position is 300
        }
    }
}

/// <summary>
/// Moves the coins to a new position. If a coin moves out of frame, new random y position is generated.
/// </summary>
void Game::move_coins() {
    for (size_t i = 0; i < 3; i++)
    {
        this->coin_entities_[i]->sprite->setPosition(coin_entities_[i]->sprite->getPosition().x
            - this->speed_, coin_entities_[i]->sprite->getPosition().y);
        if (coin_entities_[i]->sprite->getPosition().x < -this->ceiling_height) {
            this->coin_entities_[i]->sprite->setPosition(coin_entities_[i]->sprite->getPosition().x + background_width+coin_start_x,
                ((rand() % coin_height_mod) * entity_height_multiplier) + this->ceiling_height);
        }
    }
}

/// <summary>
/// Moves the reverser to a new position. If the reverser moves out of frame, new random y position is generated.
/// </summary>
void Game::move_reverser() {
    this->gravity_reverser_->sprite->setPosition(gravity_reverser_->sprite->getPosition().x
        - this->speed_, gravity_reverser_->sprite->getPosition().y);
    if (gravity_reverser_->sprite->getPosition().x < -this->ceiling_height) {
        this->gravity_reverser_->sprite->setPosition(gravity_reverser_->sprite->getPosition().x + background_width + reverser_spacing,
            ((rand() % coin_height_mod) * entity_height_multiplier) + this->ceiling_height);
    }
}

/// <summary>
/// Scrolling animation for the background.
/// </summary>
void Game::scroll_background() {
    background_->setPosition(background_->getPosition().x - this->speed_, 0);
    background2_->setPosition(background2_->getPosition().x - this->speed_, 0);

    if (background_->getPosition().x < -this->background_width) {
        background_->setPosition(background2_->getPosition().x + this->background_width, 0);
    }

    if (background2_->getPosition().x < -this->background_width) {
        background2_->setPosition(background_->getPosition().x + this->background_width, 0);
    }
}


