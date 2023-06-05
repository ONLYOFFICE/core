#include "parser_factory.h"
#include "parser_v5_x_x.h"


namespace StarMathParser::Factory
{

std::shared_ptr<StarMathParser::Interfaces::Parser> createParser(const TString& version) {
    //
	return std::make_shared<Parser_v5_x_x>();
}

}
