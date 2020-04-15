#include <cmath>

class Tanh : public Tree::Primitives::Primitive
{
public:
    Tanh()
    {
        nArguments_ = 1;
        name_ = "tanh";
    }

    void execute(void* evalOp, Tree::Tree& tree)
    {
        double& arg = *(double*)evalOp;
        getNextArgument(&arg, tree);
        arg = tanh(arg);
    }

    ~Tanh()
    {	}
};