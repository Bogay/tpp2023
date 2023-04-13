#include "environment.hpp"
#include <iostream>
#include <utility>
#include <functional>
#include <algorithm>

namespace phah_titu
{
    Environment Environment::read(std::istream &input)
    {
        Environment env;
        int x, y;
        input >> x >> y;
        env.my_base_.set_position(std::make_pair(x, y));
        env.opponent_base_.set_position(std::make_pair(env.map_x - x, env.map_y - y));
        input >> env.heroes_per_player_;
        return env;
    }

    void Environment::update(std::istream &input)
    {
        this->my_base_.update(input);
        this->opponent_base_.update(input);

        size_t entity_count;
        input >> entity_count;

        this->opponent_heroes_.clear();
        this->my_heroes_.clear();
        this->monsters_.clear();

        for (size_t i = 0; i < entity_count; i++)
        {
            Entity e = Entity::read(input);
            if (e.is_monster())
            {
                this->monsters_.push_back(Monster(std::move(e)));
            }
            else if (e.is_my_hero())
            {
                this->my_heroes_.push_back(Hero(std::move(e)));
            }
            else
            {
                this->opponent_heroes_.push_back(e);
            }
        }
    }

    void Environment::debug(std::ostream &output) const
    {
        output << "Environment\n";
        output << "My Base\n";
        this->get_my_base().debug(output);
        output << "Opponent Base\n";
        this->get_opponent_base().debug(output);
        output << "Monsters\n";
        for (const Entity &mon : this->monsters_)
        {
            mon.debug(output);
        }
        output << "My Heros\n";
        for (const Entity &hero : this->my_heroes_)
        {
            hero.debug(output);
        }
        output << "Others\n";
        for (const Entity &ent : this->opponent_heroes_)
        {
            ent.debug(output);
        }
    }

    const Base &Environment::get_my_base() const
    {
        return this->my_base_;
    }

    const Base &Environment::get_opponent_base() const
    {
        return this->opponent_base_;
    }

    const std::size_t Environment::get_heroes_per_player() const
    {
        return this->heroes_per_player_;
    }

    int Environment::estimate_mana() const
    {
        int mana = this->get_my_base().get_mana();
        for (const Hero &h : *this)
        {
            mana -= this->spell_cost * h.will_cast();
        }
        return mana;
    }

    bool Environment::can_cast() const
    {
        return this->estimate_mana() >= this->spell_cost;
    }

    std::size_t Environment::get_entity_count() const
    {
        return this->opponent_heroes_.size() + this->monsters_.size() + this->my_heroes_.size();
    }

    const Entity &Environment::get_entity(std::size_t index) const
    {
        if (index < this->opponent_heroes_.size())
        {
            return this->opponent_heroes_[index];
        }
        index -= this->opponent_heroes_.size();
        if (index < this->monsters_.size())
        {
            return this->monsters_[index];
        }
        index -= this->monsters_.size();
        return this->my_heroes_[index];
    }

    // TODO: get by id instead of index?
    const Hero &Environment::get_my_hero(std::size_t index) const
    {
        return this->my_heroes_[index];
    }

    Hero &Environment::get_my_hero(std::size_t index)
    {
        return this->my_heroes_[index];
    }

    std::vector<Hero>::iterator Environment::begin()
    {
        return this->my_heroes_.begin();
    }

    std::vector<Hero>::iterator Environment::end()
    {
        return this->my_heroes_.end();
    }

    std::vector<Hero>::const_iterator Environment::begin() const
    {
        return this->my_heroes_.begin();
    }

    std::vector<Hero>::const_iterator Environment::end() const
    {
        return this->my_heroes_.end();
    }

    void Environment::sort_monsters(std::function<bool(const Environment &, const Monster &, const Monster &)> cmp_fn)
    {
        auto m_cmp_fn = [this, cmp_fn](const auto &ma, const auto &mb)
        { return cmp_fn(*this, ma, mb); };
        std::sort(this->monsters_.begin(), this->monsters_.end(), m_cmp_fn);
    }

    const std::vector<Entity> Environment::get_opponent_heroes() const
    {
        return this->opponent_heroes_;
    }

    const std::vector<Hero> Environment::get_my_heroes() const
    {
        return this->my_heroes_;
    }

    const std::vector<Monster> Environment::get_monsters() const
    {
        return this->monsters_;
    }

    void Environment::write_actions(std::ostream &output)
    {
        for (const Hero &hero : *this)
        {
            output << hero.action() << ' ' << hero.comment() << '\n';
        }
    }

    Environment::Environment() {}
}
