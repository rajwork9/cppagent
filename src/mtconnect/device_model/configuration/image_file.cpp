//
// Copyright Copyright 2009-2022, AMT � The Association For Manufacturing Technology (�AMT�)
// All rights reserved.
//
//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.
//

#include "image_file.hpp"

using namespace std;

namespace mtconnect {
  using namespace entity;
  namespace device_model::configuration {
    FactoryPtr ImageFile::getFactory()
    {
      static FactoryPtr imageFiles;
      if (!imageFiles)
      {
        auto imageFile = make_shared<Factory>(
            Requirements {{"id", true}, {"name", false}, {"href", true}, {"mediaType", true}});

        imageFiles = make_shared<Factory>(
            Requirements {Requirement("ImageFile", ENTITY, imageFile, 1, Requirement::Infinite)});
      }
      return imageFiles;
    }
  }  // namespace device_model::configuration
}  // namespace mtconnect
