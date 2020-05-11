#include <cmath>

class Negated : public Tree::Primitives::Primitive
{
public:
    Negated()
    {
        nArguments_ = 1;
        name_ = "ngt";
    }

    void execute(void* evalOp, Tree::Tree& tree)
    {
        double& arg = *(double*)evalOp;
        getNextArgument(&arg, tree);
        arg = -arg;
    }

    ~Negated()
    {	}
};