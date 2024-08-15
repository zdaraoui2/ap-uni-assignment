#include <ctime>

class GameComponent
{
public:
    GameComponent();
    void Update(const tm *eventTime);

private:
    int id;
    static int instances; // By having this as static, it makes this a property of the class, enabling the class to have better encapsulation.
};
