// ======================================================================== //
// Copyright 2009-2019 Intel Corporation                                    //
//                                                                          //
// Licensed under the Apache License, Version 2.0 (the "License");          //
// you may not use this file except in compliance with the License.         //
// You may obtain a copy of the License at                                  //
//                                                                          //
//     http://www.apache.org/licenses/LICENSE-2.0                           //
//                                                                          //
// Unless required by applicable law or agreed to in writing, software      //
// distributed under the License is distributed on an "AS IS" BASIS,        //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. //
// See the License for the specific language governing permissions and      //
// limitations under the License.                                           //
// ======================================================================== //

#pragma once

#include "ManagedObject.h"

namespace ospray {
  namespace cpp {

    class Renderer : public ManagedObject<OSPRenderer, OSP_RENDERER>
    {
     public:
      Renderer(const std::string &type);
      Renderer(const Renderer &copy);
      Renderer(OSPRenderer existing = nullptr);
    };

    static_assert(sizeof(Renderer) == sizeof(OSPRenderer),
                  "cpp::Renderer can't have data members!");

    // Inlined function definitions ///////////////////////////////////////////

    inline Renderer::Renderer(const std::string &type)
    {
      ospObject = ospNewRenderer(type.c_str());
    }

    inline Renderer::Renderer(const Renderer &copy)
        : ManagedObject<OSPRenderer, OSP_RENDERER>(copy.handle())
    {
      ospRetain(copy.handle());
    }

    inline Renderer::Renderer(OSPRenderer existing)
        : ManagedObject<OSPRenderer, OSP_RENDERER>(existing)
    {
    }

  }  // namespace cpp

  OSPTYPEFOR_SPECIALIZATION(cpp::Renderer, OSP_RENDERER);

}  // namespace ospray
