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

// ospray
#include "Mesh.h"
#include "../include/ospray/ospray.h"
#include "common/World.h"
// ispc exports
#include <cmath>
#include "Mesh_ispc.h"

namespace ospray {

  Mesh::Mesh()
  {
    ispcEquivalent = ispc::Mesh_create(this);
  }

  std::string Mesh::toString() const
  {
    return "ospray::Mesh";
  }

  void Mesh::commit()
  {
    vertexData   = getParamDataT<vec3f>("vertex.position", true);
    normalData   = getParamDataT<vec3f>("vertex.normal");
    colorData    = getParam<Data *>("vertex.color");
    texcoordData = getParamDataT<vec2f>("vertex.texcoord");
    indexData    = getParamDataT<vec3ui>("index");
    if (!indexData)
      indexData = getParamDataT<vec4ui>("index", true);

    if (embreeGeometry)
      rtcReleaseGeometry(embreeGeometry);

    const bool isTri = indexData->type == OSP_VEC3UI;
    embreeGeometry   = rtcNewGeometry(
        ispc_embreeDevice(),
        isTri ? RTC_GEOMETRY_TYPE_TRIANGLE : RTC_GEOMETRY_TYPE_QUAD);
    setEmbreeGeometryBuffer(embreeGeometry, RTC_BUFFER_TYPE_VERTEX, vertexData);
    rtcSetSharedGeometryBuffer(embreeGeometry,
                               RTC_BUFFER_TYPE_INDEX,
                               0,
                               isTri ? RTC_FORMAT_UINT3 : RTC_FORMAT_UINT4,
                               indexData->data(),
                               0,
                               isTri ? sizeof(vec3ui) : sizeof(vec4ui),
                               indexData->size());
    rtcCommitGeometry(embreeGeometry);

    ispc::Mesh_set(getIE(),
                   ispc(indexData),
                   ispc(vertexData),
                   ispc(normalData),
                   ispc(colorData),
                   ispc(texcoordData),
                   colorData && colorData->type == OSP_VEC4F,
                   isTri);

    postCreationInfo(vertexData->size());
  }

  size_t Mesh::numPrimitives() const
  {
    return indexData->size();
  }

  OSP_REGISTER_GEOMETRY(Mesh, mesh);

}  // namespace ospray
