#ifndef LEPRECHAUNS_ACTOR_HPP
#define LEPRECHAUNS_ACTOR_HPP

namespace leprechauns
{

    struct IActor
    {
        virtual ~IActor() {}
    };

    class ActorBase : public IActor
    {
      public:
        ~ActorBase() override {}
    };

    class Lazy : public ActorBase
    {
        ~Lazy() override {}

        void move();
    };

    class Greedy : public ActorBase
    {
        ~Greedy() override {}

        void move();
    };

} // namespace leprechauns

#endif // LEPRECHAUNS_ACTOR_HPP
