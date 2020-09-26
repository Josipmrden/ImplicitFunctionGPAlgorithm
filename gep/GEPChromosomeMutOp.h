#ifndef GEPCHROMOSOMEMUTOP_H
#define GEPCHROMOSOMEMUTOP_H
#include "ecf/ECF_base.h"
#include "GEPChromosome.h"
namespace GEPChromosome{
	/**
	* \ingroup genotypes gep
	* \brief GEPChromosome genotype: node replacement mutation operator.
	* Tries to replace the selected primitive with a different one, conforming to the restrictions of the domain where it is located.
	*/
	class GEPChromosomeMutOp : public MutationOp
	{
	public:
		void registerParameters(StateP state);
		bool initialize(StateP state);
		bool mutate(GenotypeP gene);
	};
}
#endif // GEPChromosomeMutOp_h