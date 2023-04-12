#include "entity.hpp"
#include <iostream>
#include <utility>

namespace spider_attack
{
    Entity Entity::read(std::istream &input)
    {
        Entity ent;

        int entity_type;
        int target_to;
        int is_controlled;
        int is_near_base;

        input >>
            ent.id_ >>
            entity_type >>
            ent.position_.first >>
            ent.position_.second >>
            ent.shield_life_ >>
            is_controlled >>
            ent.health_ >>
            ent.velocity_.first >>
            ent.velocity_.second >>
            is_near_base >>
            target_to;
        input.ignore();

        ent.is_controlled_ = is_controlled;
        ent.is_near_base_ = is_near_base;
        ent.entity_type_ = static_cast<EntityType>(entity_type);
        ent.target_to_ = static_cast<TargetTo>(target_to);

        return ent;
    }

    void Entity::debug(std::ostream &output) const
    {
        output << this->id_ << ' ' << this->entity_type_ << ' ' << this->position_.first << ' ' << this->position_.second << ' ' << this->shield_life_ << ' ' << this->is_controlled_ << ' ' << this->health_ << ' ' << this->velocity_.first << ' ' << this->velocity_.second << ' ' << this->is_near_base_ << ' ' << this->target_to_ << '\n';
    }

    int Entity::get_id() const
    {
        return this->id_;
    }

    EntityType Entity::get_type() const
    {
        return this->entity_type_;
    }

    bool Entity::is_monster() const
    {
        return this->get_type() == EntityType::kMonster;
    }

    bool Entity::is_my_hero() const
    {
        return this->get_type() == EntityType::kMyHero;
    }

    bool Entity::is_opponent_hero() const
    {
        return this->get_type() == EntityType::kOpponentHero;
    }

    std::pair<int, int> Entity::get_position() const
    {
        return this->position_;
    }

    int Entity::get_shield_life() const
    {
        return this->shield_life_;
    }

    bool Entity::is_controlled() const
    {
        return this->is_controlled_;
    }

    Entity::Entity() {}

    int Monster::get_health() const
    {
        return this->health_;
    }

    std::pair<int, int> Monster::get_velocity() const
    {
        return this->velocity_;
    }

    bool Monster::is_near_base() const
    {
        return this->is_near_base_;
    }

    TargetTo Monster::get_target_to() const
    {
        return this->target_to_;
    }

    bool Monster::is_targeting_me() const
    {
        return this->target_to_ == TargetTo::kMyBase;
    }

    bool Monster::is_targeting_opponent() const
    {
        return this->target_to_ == TargetTo::kOpponentBase;
    }

    bool Monster::wont_reach_base() const
    {
        return this->target_to_ == TargetTo::kNone;
    }

    Monster::Monster(Entity &&e)
    {
        this->id_ = e.get_id();
        this->entity_type_ = e.get_type();
        this->position_ = e.get_position();
        this->shield_life_ = e.get_shield_life();
        this->is_controlled_ = e.is_controlled();
        this->health_ = e.health_;
        this->velocity_ = e.velocity_;
        this->is_near_base_ = e.is_near_base_;
        this->target_to_ = e.target_to_;
    }

    void Hero::wait()
    {
        this->action_ = "WAIT";
    }

    bool Hero::will_wait() const
    {
        return this->action_ == "WAIT";
    }

    void Hero::move(const int x, const int y)
    {
        this->action_ = "MOVE " + std::to_string(x) + " " + std::to_string(y);
    }

    void Hero::move(const std::pair<int, int> pos)
    {
        this->move(pos.first, pos.second);
    }

    bool Hero::will_move() const
    {
        return this->action_.find("MOVE") != std::string::npos;
    }

    void Hero::wind(const std::pair<int, int> target)
    {
        this->action_ = "SPELL WIND " + std::to_string(target.first) + " " + std::to_string(target.second);
    }

    void Hero::control(int entity_id, const std::pair<int, int> target)
    {

        this->action_ = "SPELL CONTROL " + std::to_string(entity_id) + " " + std::to_string(target.first) + " " + std::to_string(target.second);
    }

    void Hero::shield(int entity_id)
    {
        this->action_ = "SPELL SHIELD " + std::to_string(entity_id);
    }

    bool Hero::will_cast() const
    {
        return this->action_.find("SPELL") != std::string::npos;
    }

    const std::string &Hero::action() const
    {
        return this->action_;
    }

    void Hero::set_comment(const std::string &comment)
    {
        this->comment_ = comment;
    }

    const std::string &Hero::comment() const
    {
        return this->comment_;
    }

    Hero::Hero(Entity &&e)
    {
        this->id_ = e.get_id();
        this->entity_type_ = e.get_type();
        this->position_ = e.get_position();
        this->shield_life_ = e.get_shield_life();
        this->is_controlled_ = e.is_controlled();
        this->health_ = e.health_;
        this->velocity_ = e.velocity_;
        this->is_near_base_ = e.is_near_base_;
        this->target_to_ = e.target_to_;
        this->action_ = "WAIT";
    }
}