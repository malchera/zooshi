// Copyright 2015 Google Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "components/player_projectile.h"

namespace fpl {
namespace fpl_project {

void PlayerProjectileComponent::UpdateAllEntities(
    entity::WorldTime delta_time) {
  for (auto iter = entity_data_.begin(); iter != entity_data_.end(); ++iter) {
    PlayerProjectileData* data = Data<PlayerProjectileData>(iter->entity);
    if (data->fuse < delta_time) {
      entity_manager_->DeleteEntity(iter->entity);
      // destroy this entity
    } else {
      data->fuse -= delta_time;
    }
  }
}

void PlayerProjectileComponent::AddFromRawData(entity::EntityRef& entity,
                                               const void* /*raw_data*/) {
  AddEntity(entity);
}

}  // fpl_project
}  // fpl
