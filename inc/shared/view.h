#ifndef __DALI_DEMO_HELPER_VIEW_H__
#define __DALI_DEMO_HELPER_VIEW_H__

/*
 * Copyright (c) 2014 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <dali-toolkit/dali-toolkit.h>
//#include <dali-toolkit/devel-api/controls/tool-bar/tool-bar.h>


namespace DemoHelper
{


float ScalePointSize(int pointSize)
{
  Dali::Vector2 dpi = Dali::Stage::GetCurrent().GetDpi();
  float meanDpi = (dpi.height + dpi.width) * 0.5f;
  return pointSize * 220.0f / meanDpi;        // 220 is the default horizontal DPI defined in adaptor Application
}


Dali::Layer CreateView( Dali::Application& application,
                        Dali::Toolkit::Control& view )
{
  Dali::Stage stage = Dali::Stage::GetCurrent();

  // Hide the indicator bar
  application.GetWindow().ShowIndicator( Dali::Window::INVISIBLE );

  // Create default View.
  view = Dali::Toolkit::Control::New();
  view.SetAnchorPoint( Dali::AnchorPoint::CENTER );
  view.SetParentOrigin( Dali::ParentOrigin::CENTER );
  view.SetResizePolicy( Dali::ResizePolicy::FILL_TO_PARENT, Dali::Dimension::ALL_DIMENSIONS );

  // Add the view to the stage before setting the background.
  stage.Add( view );


  // Create a content layer.
  Dali::Layer contentLayer = Dali::Layer::New();
  contentLayer.SetAnchorPoint( Dali::AnchorPoint::CENTER );
  contentLayer.SetParentOrigin( Dali::ParentOrigin::CENTER );
  contentLayer.SetResizePolicy( Dali::ResizePolicy::FILL_TO_PARENT, Dali::Dimension::ALL_DIMENSIONS );
  view.Add( contentLayer );
  return contentLayer;
}

} // DemoHelper

#endif // __DALI_DEMO_HELPER_VIEW_H__
