#ifndef GEPCHROMOSOMECRSTWOPOINT_H
#define GEPCHROMOSOMECRSTWOPOINT_H
#include "ecf/ECF_base.h"
#include "GEPChromosome.h"
namespace GEPChromosome
{
	/**
	* \ingroup genotypes gep
	* \brief GEPChromosome genotype: two point crx operator.
	* Selects two crossing points between which to exchange genetic information.
	*/

	class GEPChromosomeCrsTwoPoint : public CrossoverOp
	{
	public:
		bool initialize(StateP);
		void registerParameters(StateP);
		bool mate(GenotypeP gen1, GenotypeP gen2, GenotypeP child);
	};
}
#endif // GEPChromosomeCrsTwoPoint_h