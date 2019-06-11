#pragma once
#include <vector>
#include <functional>
#include "Shared.hpp"
#include "States/StateType.hpp"
#include "States/State.hpp"
#include "Utilities/EnumHash.hpp"
#include "States/Intro.hpp"
#include "States/S_Character.hpp"
#include "States/S_Game.hpp"

using StateContainer = std::vector< std::pair<StateType, State*>>;
using TypeContainer = std::vector<StateType>;
using StateFactory = utils::unordered_map<StateType, std::function<State*(void)>  >;

struct Shared;

class StateManager
{
private:
    Shared* m_shared;
    StateContainer m_states;
    TypeContainer m_toRemove;
    StateFactory m_stateFactory;

    void createState(const StateType& type);
    void removeState(const StateType& type);

    template<class T>
    void registerState(const StateType& type)
    {
        m_stateFactory[type] = [this]() -> State*
        {
            return new T(this);
        };
    }

public:
    StateManager(Shared* shared);
    ~StateManager();
    void update(float & dTime);
    void draw();
    void processRequests();
    Shared* getContext();
    bool hasState(const StateType& type);
    void switchTo(const StateType& type);
    void remove(const StateType& type);
};
