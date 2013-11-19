#include "RunElem.h"
#include "Runs/Run.h"
#include "Runs/Fld.h"
#include "Runs/Br.h"

namespace PPTX
{
	namespace Logic
	{

		RunElem::RunElem()
		{
		}


		RunElem::~RunElem()
		{
		}
	

		RunElem::RunElem(const XML::XNode& node)
		{
			fromXML(node);
		}


		const RunElem& RunElem::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void RunElem::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			std::string name = element.XName->Name;

			if(name == "r")
				Elem.reset(new Logic::Run(element));
			else if(name == "fld")
				Elem.reset(new Logic::Fld(element));
			else if(name == "br")
				Elem.reset(new Logic::Br(element));
			else Elem.reset();
		}


		const XML::XNode RunElem::toXML() const
		{
			return XML::Write(Elem);
		}

	} // namespace Logic
} // namespace PPTX