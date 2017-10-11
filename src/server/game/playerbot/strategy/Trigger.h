#pragma once
#include "Action.h"
#include "Event.h"
#include "../PlayerbotAIAware.h"

#define NEXT_TRIGGERS(name, relevance) \
    virtual NextAction* getNextAction() { return new NextAction(name, relevance); }

#define BEGIN_TRIGGER(clazz, super) \
class clazz : public super \
    { \
    public: \
        clazz(PlayerbotAI* ai) : super(ai) {} \
    public: \
        virtual bool IsActive();

#define END_TRIGGER() \
    };

namespace ai
{
    class Trigger : public AiNamedObject
    {
    public:
        Trigger(PlayerbotAI* ai, std::string name = "trigger", int checkInterval = 1) : AiNamedObject(ai, name) {
            this->checkInterval = checkInterval;
            ticksElapsed = 0;
        }
        virtual ~Trigger() = default;

    public:
        virtual Event Check();
        virtual void ExternalEvent(std::string param, Player* owner = nullptr) {}
        virtual void ExternalEvent(WorldPacket &packet, Player* owner = nullptr) {}
        virtual bool IsActive() { return false; }
        virtual NextAction** getHandlers() { return nullptr; }
        void Update() {}
        virtual void Reset() {}
        virtual Unit* GetTarget();
        virtual Value<Unit*>* GetTargetValue();
        virtual std::string GetTargetName() { return "self target"; }

        bool needCheck() {
            if (++ticksElapsed >= checkInterval) {
                ticksElapsed = 0;
                return true;
            }
            return false;
        }

    protected:
        int checkInterval;
        int ticksElapsed;
    };


    class TriggerNode
    {
    public:
        TriggerNode(std::string name, NextAction** handlers = nullptr)
        {
            this->name = name;
            this->handlers = handlers;
            this->trigger = nullptr;
        }
        virtual ~TriggerNode()
        {
            NextAction::destroy(handlers);
        }

    public:
        Trigger* getTrigger() { return trigger; }
        void setTrigger(Trigger* _trigger) { trigger = _trigger; }
        std::string getName() { return name; }

    public:
        NextAction** getHandlers() { return NextAction::merge(NextAction::clone(handlers), trigger->getHandlers()); }

    private:
        Trigger* trigger;
        NextAction** handlers;
        std::string name;
    };
}
