#include <cmath>

class Exp : public Tree::Primitives::Primitive
{
public:
    Exp()
    {
        nArguments_ = 1;
        name_ = "exp";
    }

    void execute(void* evalOp, Tree::Tree& tree)
    {
        double& arg = *(double*)evalOp;
        getNextArgument(&arg, tree);
        arg = exp(arg);
    }

    ~Exp()
    {	}
};