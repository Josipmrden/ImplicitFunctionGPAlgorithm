#ifndef GEPCHROMOSOMECRSGENE_H
#define GEPCHROMOSOMECRSGENE_H
#include "ecf/ECF_base.h"
#include "GEPChromosome.h"
namespace GEPChromosome
{
	/**
	* \ingroup genotypes gep
	* \brief GEPChromosome genotype: gene crx operator.
	* Selects a gene number and swaps it between both parents
	*/

	class GEPChromosomeCrsGene : public CrossoverOp
	{
	public:
		bool initialize(StateP);
		void registerParameters(StateP);
		bool mate(GenotypeP gen1, GenotypeP gen2, GenotypeP child);
	};
}
#endif // GEPChromosomeCrsGene_h