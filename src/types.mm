/// Copyright 2014 Alexander Böhn <fish2000@gmail.com>
/// License: MIT (see COPYING.MIT file)

#include <subjective-c/types.hh>

namespace objc {
    
    types::boolean boolean(bool value) { return types::boolean(value ? YES : NO); }
    
    bool to_bool(types::boolean value) { return static_cast<bool>(value); }
    
}
