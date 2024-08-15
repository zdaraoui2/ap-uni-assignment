#include <ctime>

class GameComponent
{
public:
    GameComponent();
    virtual void Update(const tm *eventTime); // Virtual to enable class to be overriden for polymorphism

private:
    int id;
    static int instances; // By having this as static, it makes this a property of the class, enabling the class to have better encapsulation.
};
