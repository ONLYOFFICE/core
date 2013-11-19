#include "Material.h"

namespace PPTX
{
	namespace Limit
	{
		Material::Material()
		{
			add("clear");
			add("dkEdge");
			add("flat");
			add("legacyMatte");
			add("legacyMetal");
			add("legacyPlastic");
			add("legacyWireframe");
			add("matte");
			add("metal");
			add("plastic");
			add("powder");
			add("softEdge");
			add("softmetal");
			add("translucentPowder");
			add("warmMatte");
		}

		const std::string Material::no_find() const
		{
			return "clear";
		}
	} // namespace Limit
} // namespace PPTX