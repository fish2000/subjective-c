
#include "typecode.hpp"

namespace typecode {
    
    /// static initializers
    const intmap_t typecodemaps::integral = typecodemaps::init_integral_map();
    const charmap_t typecodemaps::character = typecodemaps::init_typecode_character_map();
    const stringmap_t typecodemaps::literal = typecodemaps::init_typecode_literal_map();
    
    NPY_TYPECHAR typechar(NPY_TYPES typecode) {
        try {
            return typecodemaps::character.at(typecode);
        } catch (const std::out_of_range &err) {
            std::cerr    << ">>> Type character not found for typecode: "
                         << typecode << std::endl << ">>> Exception message: "
                         << err.what() << std::endl;
            return typecodemaps::character.at(NPY_USERDEF);
        }
    }
    
    NPY_TYPECHAR typechar(unsigned int typecode) {
        return typecode::typechar(static_cast<NPY_TYPES>(typecode));
    }
    
    std::string name(NPY_TYPES typecode) {
        try {
            return typecodemaps::literal.at(typecode);
        } catch (const std::out_of_range &err) {
            std::cerr    << ">>> Typecode literal not found for typecode: "
                         << typecode << std::endl << ">>> Exception message: "
                         << err.what() << std::endl;
            return typecodemaps::literal.at(NPY_USERDEF);
        }
    }
    
    std::string name(unsigned int typecode) {
        return typecode::name(static_cast<NPY_TYPES>(typecode));
    }
}
