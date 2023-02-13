#include <iostream>
#include <variant>
#include <map>

using namespace std;

class closure;

using any_t = variant<closure*,int>;

class closure{
    private:
        any_t (*func)(any_t x, map<string,any_t> env);
        map<string, any_t> env;
    public:
    closure(any_t (*func)(any_t x, map<string,any_t> env), map<string, any_t> env){
        this->func = func;
        this->env = env;
    }
    any_t applyClosure(any_t x){
        return (*(this->func))(x, this->env);
    }
};
