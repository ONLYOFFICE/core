#include <iosfwd>

namespace cpdoccore {
namespace xml {

template <class StringT>
class attributes;

::std::wostream & serialize_attributes( ::std::wostream & _Wostream, const attributes< ::std::wstring > * Attributes );
::std::ostream & serialize_attributes( ::std::ostream & _Wostream, const attributes< ::std::string > * Attributes );

}
}
