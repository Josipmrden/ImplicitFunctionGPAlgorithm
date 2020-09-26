#ifndef GEPCHROMOSOMECRSONEPOINT_H
#define GEPCHROMOSOMECRSONEPOINT_H
#include "ecf/ECF_base.h"
#include "GEPChromosome.h"
namespace GEPChromosome
{
	/**
	* \ingroup genotypes gep
	* \brief GEPChromosome genotype: one point crx operator.
	* Selects a crossing point from which to exchange genetic information.
	*/

	class GEPChromosomeCrsOnePoint : public CrossoverOp
	{
	public:
		bool initialize(StateP);
		void registerParameters(StateP);
		bool mate(GenotypeP gen1, GenotypeP gen2, GenotypeP child);
	};
}
#endif // GEPChromosomeCrsOnePoint_h