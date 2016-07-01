
#ifndef LIBIMREAD_PYTHON_IM_INCLUDE_MODELS_MODELS_HH_
#define LIBIMREAD_PYTHON_IM_INCLUDE_MODELS_MODELS_HH_

#include "buffermodel.hh"
#include "imagemodel.hh"
#include "batchmodel.hh"

namespace py {
    
    namespace ext {
        
        /// “Models” are python wrapper types
        using ImageModel = ImageModelBase<HalideNumpyImage, buffer_t>;
        using ImageBufferModel = ImageModel::BufferModel;
        
        using ArrayModel = ImageModelBase<ArrayImage, buffer_t>;
        using ArrayBufferModel = ArrayModel::BufferModel;
        
        /// probably don't need this next bit:
        using py::ext::BatchModel;
        
    }
    
}

#endif /// LIBIMREAD_PYTHON_IM_INCLUDE_MODELS_MODELS_HH_