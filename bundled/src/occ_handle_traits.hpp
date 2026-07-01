// occ_handle_traits.hpp — teaches CxxWrap that opencascade::handle<T> (the
// Handle(ClassName) macro) is a smart pointer, the same way it already knows
// std::shared_ptr<T> is one (see jlcxx/smart_pointers.hpp). Include this near
// the top of any translation unit that uses Handle(X) in a wrapped signature.
#pragma once

#include <jlcxx/jlcxx.hpp>
#include <Standard_Handle.hxx>

namespace jlcxx {
  template<typename T>
  struct IsSmartPointerType<opencascade::handle<T>> : std::true_type { };
}
