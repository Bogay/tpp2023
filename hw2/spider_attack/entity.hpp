#pragma once

#include <utility>
#include <iostream>

namespace spider_attack
{
    enum EntityType
    {
        kMonster = 0,
        kMyHero = 1,
        kOpponentHero = 2,
        kUnusedType = -1,
    };

    enum TargetTo
    {
        kNone = 0,
        kMyBase = 1,
        kOpponentBase = 2,
        kUnusedTargetTo = -1,
    };

    class Entity
    {
    public:
        static Entity read(std::istream &input = std::cin);
        void debug(std::ostream &output = std::cerr) const;

        int get_id() const;

        EntityType get_type() const;
        bool is_monster() const;
        bool is_my_hero() const;
        bool is_opponent_hero() const;

        std::pair<int, int> get_position() const;
        int get_shield_life() const;
        bool is_controlled() const;

        Entity();

        // protected:
        int id_;
        EntityType entity_type_;
        std::pair<int, int> position_;
        int shield_life_;
        bool is_controlled_;
        int health_;
        std::pair<int, int> velocity_;
        bool is_near_base_;
        TargetTo target_to_;
    };

    class Monster : public Entity
    {
    public:
        int get_health() const;
        std::pair<int, int> get_velocity() const;
        bool is_near_base() const;

        TargetTo get_target_to() const;
        bool is_targeting_me() const;
        bool is_targeting_opponent() const;
        bool wont_reach_base() const;

        Monster(Entity &&e);
    };

    class Hero : public Entity
    {
    public:
        void wait();
        bool will_wait() const;
        void move(const int x, const int y);
        void move(const std::pair<int, int> pos);
        bool will_move() const;
        void wind(const std::pair<int, int> target);
        void shield(int entity_id);
        void control(int entity_id, const std::pair<int, int> target);
        bool will_cast() const;

        const std::string action() const;

        Hero(Entity &&e);

    private:
        std::string action_;
    };
}
