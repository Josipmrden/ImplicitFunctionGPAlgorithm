#include <cmath>

class Sqrt : public Tree::Primitives::Primitive
{
public:
    Sqrt()
    {
        nArguments_ = 1;
        name_ = "sqrt";
    }

    void execute(void* evalOp, Tree::Tree& tree)
    {
        double& arg = *(double*)evalOp;
        getNextArgument(&arg, tree);
        arg = sqrt(arg);
    }

    ~Sqrt()
    {	}
};