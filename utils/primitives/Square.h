#include <cmath>

class Square : public Tree::Primitives::Primitive
{
public:
    Square()
    {
        nArguments_ = 1;
        name_ = "sqrt";
    }

    void execute(void* evalOp, Tree::Tree& tree)
    {
        double& arg = *(double*)evalOp;
        getNextArgument(&arg, tree);
        arg = arg * arg;
    }

    ~Square()
    {	}
};