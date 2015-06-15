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

#include <cstdio>

#include "components/attributes.h"
#include "components/services.h"
#include "transform.h"
#include "event_system/event_manager.h"
#include "event_system/event_payload.h"
#include "events/modify_attribute.h"
#include "events/utilities.h"
#include "event_system/event_manager.h"
#include "events_generated.h"
#include "fplbase/material_manager.h"
#include "fplbase/utilities.h"
#include "imgui/imgui.h"

namespace fpl {
namespace fpl_project {

void AttributesComponent::Init() {
  ServicesComponent* services =
      entity_manager_->GetComponent<ServicesComponent>();
  input_system_ = services->input_system();
  font_manager_ = services->font_manager();
  material_manager_ = services->material_manager();
  event::EventManager* event_manager = services->event_manager();
  event_manager->RegisterListener(EventSinkUnion_ModifyAttribute, this);
}

void AttributesComponent::OnEvent(const event::EventPayload& event_payload) {
  switch (event_payload.id()) {
    case EventSinkUnion_ModifyAttribute: {
      auto* mod_event = event_payload.ToData<ModifyAttributePayload>();
      AttributesData* attributes_data = Data<AttributesData>(mod_event->target);
      if (attributes_data) {
        float* attribute =
            &attributes_data->attributes[mod_event->modify_attribute->attrib()];
        ApplyOperation(attribute, mod_event->modify_attribute->op(),
                       mod_event->modify_attribute->value());
      }
      break;
    }
    default: { assert(0); }
  }
}

void AttributesComponent::UpdateAllEntities(entity::WorldTime /*delta_time*/) {
  auto component_data = &component_data_;
  gui::Run(*material_manager_, *font_manager_, *input_system_,
           [component_data]() {
             for (auto iter = component_data->begin();
                  iter != component_data->end(); ++iter) {
               AttributesData* attributes_data = &iter->data;
               char label[16] = {0};
               int score = static_cast<int>(
                   attributes_data->attributes[AttributeDef_PatronsFed]);
               std::sprintf(label, "%i", score);
               gui::StartGroup(gui::LAYOUT_HORIZONTAL_TOP, 10);
               gui::Margin margin(8);
               gui::SetMargin(margin);
               gui::Label(label, 100);
               gui::EndGroup();
             }
           });
}

void AttributesComponent::AddFromRawData(entity::EntityRef& entity,
                                         const void* raw_data) {
  auto component_data = static_cast<const ComponentDefInstance*>(raw_data);
  (void)component_data;
  assert(component_data->data_type() == ComponentDataUnion_AttributesDef);
  AddEntity(entity);
}

}  // fpl_project
}  // fpl

