/*
 * Copyright (c) 2017 Samsung Electronics Co., Ltd.
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

// EXTERNAL INCLUDES
#include <sstream>
#include <stdio.h>
#include <app_common.h>
// INTERNAL INCLUDES
#include "shared/view.h"
#include <dali/dali.h>
#include <dali-toolkit/dali-toolkit.h>
#include <stagehand.h>
#include <app_resource_manager.h>
#include <fstream>
#include <string.h>
//#include <dali-toolkit/devel-api/controls/buttons/button-devel.h>

using namespace Dali;
using namespace Dali::Toolkit;
using namespace std;

namespace
{
const char * const APPLICATION_TITLE( "ScrollView" );
const char * const EFFECT_CAROUSEL_IMAGE(  "icon-scroll-view-carousel.jpg" );
const char * const EFFECT_CAROUSEL_IMAGE_SELECTED(  "icon-scroll-view-carousel-selected.jpg" );
const char * const EFFECT_CUBE_IMAGE(  "icon-scroll-view-inner-cube.jpg" );
const char * const EFFECT_CUBE_IMAGE_SELECTED(  "icon-scroll-view-inner-cube-selected.jpg" );
const char * const EFFECT_SPIRAL_IMAGE(  "icon-scroll-view-spiral.jpg" );
const char * const EFFECT_SPIRAL_IMAGE_SELECTED(  "icon-scroll-view-spiral-selected.jpg" );
const char * const EFFECT_WAVE_IMAGE(  "icon-effect-wave.jpg" );
const char * const EFFECT_WAVE_IMAGE_SELECTED(  "icon-effect-wave-selected.jpg" );

const Vector3 ICON_SIZE(100.0f, 100.0f, 0.0f);

const char* EFFECT_MODE_NAME[] = {
    "PageCarousel",
    "PageCube",
    "PageSpiral",
    "PageWave"
};

const char * const IMAGE_PATHS[] = {
     "gallery-medium-1.jpg",
     "gallery-medium-2.jpg",
     "gallery-medium-3.jpg",
     "gallery-medium-4.jpg",
     "gallery-medium-5.jpg",
     "gallery-medium-6.jpg",
     "gallery-medium-7.jpg",
     "gallery-medium-8.jpg",
     "gallery-medium-9.jpg",
     "gallery-medium-10.jpg",
     "gallery-medium-11.jpg",
     "gallery-medium-12.jpg",
     "gallery-medium-13.jpg",
     "gallery-medium-14.jpg",
     "gallery-medium-15.jpg",
     "gallery-medium-16.jpg",
     "gallery-medium-17.jpg",
     "gallery-medium-18.jpg",
     "gallery-medium-19.jpg",
     "gallery-medium-20.jpg",
     "gallery-medium-21.jpg",
     "gallery-medium-22.jpg",
     "gallery-medium-23.jpg",
     "gallery-medium-24.jpg",
     "gallery-medium-25.jpg",
     "gallery-medium-26.jpg",
     "gallery-medium-27.jpg",
     "gallery-medium-28.jpg",
     "gallery-medium-29.jpg",
     "gallery-medium-30.jpg",
     "gallery-medium-31.jpg",
     "gallery-medium-32.jpg",
     "gallery-medium-33.jpg",
     "gallery-medium-34.jpg",
     "gallery-medium-35.jpg",
     "gallery-medium-36.jpg",
     "gallery-medium-37.jpg",
     "gallery-medium-38.jpg",
     "gallery-medium-39.jpg",
     "gallery-medium-40.jpg",
     "gallery-medium-41.jpg",
     "gallery-medium-42.jpg",
     "gallery-medium-43.jpg",
     "gallery-medium-44.jpg",
     "gallery-medium-45.jpg",
     "gallery-medium-46.jpg",
     "gallery-medium-47.jpg",
     "gallery-medium-48.jpg",
     "gallery-medium-49.jpg",
     "gallery-medium-50.jpg",
     "gallery-medium-51.jpg",
     "gallery-medium-52.jpg",
     "gallery-medium-53.jpg",

    NULL
};

const char * const GetNextImagePath()
{
  static const char * const * imagePtr = &IMAGE_PATHS[0];

  if ( *(++imagePtr) == NULL )
  {
    imagePtr = &IMAGE_PATHS[0];
  }

  return *imagePtr;
}

const int PAGE_COLUMNS = 10;                                                ///< Number of Pages going across (columns)
const int PAGE_ROWS = 1;                                                    ///< Number of Pages going down (rows)
const int IMAGE_ROWS = 5;                                                   ///< Number of Images going down (rows) with a Page

const unsigned int IMAGE_THUMBNAIL_WIDTH  = 256;                            ///< Width of Thumbnail Image in texels
const unsigned int IMAGE_THUMBNAIL_HEIGHT = 256;                            ///< Height of Thumbnail Image in texels

const float SPIN_DURATION = 1.0f;                                           ///< Times to spin an Image by upon touching, each spin taking a second.

const float EFFECT_SNAP_DURATION(0.66f);                                    ///< Scroll Snap Duration for Effects
const float EFFECT_FLICK_DURATION(0.5f);                                    ///< Scroll Flick Duration for Effects

} // unnamed namespace

/**
 * This example shows how to do custom Scroll Effects
 */
class ExampleController : public ConnectionTracker
{
public:

  /**
   * Constructor
   * @param application class, stored as reference
   */
  ExampleController( Application& application )
  : mApplication( application ),
    mView(),
    mScrolling(false),
    mEffectMode(PageCarouselEffect)
  {
    // Connect to the Application's Init and orientation changed signal
    mApplication.InitSignal().Connect(this, &ExampleController::OnInit);
  }

  ~ExampleController()
  {
    // Nothing to do here; everything gets deleted automatically
  }

  /**
   * This method gets called once the main loop of application is up and running
   */
  void OnInit(Application& app)
  {
	app_resource_manager_init();
    Stage stage = Dali::Stage::GetCurrent();
    stage.KeyEventSignal().Connect(this, &ExampleController::OnKeyEvent);

    // Hide the indicator bar
    mApplication.GetWindow().ShowIndicator(Dali::Window::INVISIBLE);

    // Creates a default view with a default tool bar.
    // The view is added to the stage.
    mContentLayer = DemoHelper::CreateView( app,
                                            mView);

    mEffectIcon[ PageCarouselEffect ] = EFFECT_CAROUSEL_IMAGE;
    mEffectIconSelected[ PageCarouselEffect ] = EFFECT_CAROUSEL_IMAGE_SELECTED;
    mEffectIcon[ PageCubeEffect ]     = EFFECT_CUBE_IMAGE;
    mEffectIconSelected[ PageCubeEffect ]     = EFFECT_CUBE_IMAGE_SELECTED;
    mEffectIcon[ PageSpiralEffect ]   = EFFECT_SPIRAL_IMAGE;
    mEffectIconSelected[ PageSpiralEffect ]   = EFFECT_SPIRAL_IMAGE_SELECTED;
    mEffectIcon[ PageWaveEffect ]     = EFFECT_WAVE_IMAGE;
    mEffectIconSelected[ PageWaveEffect ]     = EFFECT_WAVE_IMAGE_SELECTED;

    // Create a effect change button. (right of toolbar)
    mEffectChangeButton = Toolkit::PushButton::New();
    mEffectChangeButton.ClickedSignal().Connect( this, &ExampleController::OnEffectTouched );

    // Create the content layer.
    AddContentLayer();

    // Hack to force screen refresh.
    Animation animation = Animation::New(1.0f);
    animation.AnimateTo(Property(mContentLayer, Actor::Property::POSITION), Vector3::ZERO );
    animation.Play();

    //app_resource_manager_release();
  }

private:

  /**
   * Adds content to the ContentLayer. This is everything we see
   * excluding the toolbar at the top.
   */
  void AddContentLayer()
  {
    Stage stage = Stage::GetCurrent();
    Vector2 stageSize = stage.GetSize();

    mScrollView = ScrollView::New();
    mScrollView.SetAnchorPoint(AnchorPoint::CENTER);
    mScrollView.SetParentOrigin(ParentOrigin::CENTER);
    mContentLayer.Add( mScrollView );
    mScrollView.SetSize( stageSize );
    mScrollView.SetAxisAutoLock( true );
    mScrollView.SetAxisAutoLockGradient( 1.0f );

    mScrollView.ScrollStartedSignal().Connect( this, &ExampleController::OnScrollStarted );
    mScrollView.ScrollCompletedSignal().Connect( this, &ExampleController::OnScrollCompleted );

    for(int row = 0;row<PAGE_ROWS;row++)
    {
      for(int column = 0;column<PAGE_COLUMNS;column++)
      {
        Actor page = CreatePage();

        page.SetPosition( column * stageSize.x, row * stageSize.y );
        mScrollView.Add( page );

        mPages.push_back(page);
      }
    }

    Update();
  }

  /**
   * Updates the ScrollView and it's children based
   * on the current effect.
   */
  void Update()
  {
    std::stringstream ss(APPLICATION_TITLE);
    ss << APPLICATION_TITLE << ": " << EFFECT_MODE_NAME[mEffectMode];
    //SetTitle(ss.str());

    // remove old Effect if exists.
    if(mScrollViewEffect)
    {
      mScrollView.RemoveEffect(mScrollViewEffect);
    }

    // apply new Effect to ScrollView
    ApplyEffectToScrollView();
    unsigned int pageCount(0);
    for( std::vector< Actor >::iterator pageIter = mPages.begin(); pageIter != mPages.end(); ++pageIter)
    {
      Actor page = *pageIter;
      ApplyEffectToPage( page, pageCount++ );
    }
  }

  /**
   * Creates a page using a source of images.
   */
  Actor CreatePage()
  {
    Actor page = Actor::New();
    page.SetResizePolicy( ResizePolicy::FILL_TO_PARENT, Dimension::ALL_DIMENSIONS );
    page.SetParentOrigin( ParentOrigin::CENTER );
    page.SetAnchorPoint( AnchorPoint::CENTER );

    Stage stage = Stage::GetCurrent();
    Vector2 stageSize = stage.GetSize();

    const float margin = 10.0f;

    // Calculate the number of images going across (columns) within a page, according to the screen resolution and dpi.
    int imageColumns = round(IMAGE_ROWS * (stageSize.x / stage.GetDpi().x) / (stageSize.y / stage.GetDpi().y));
    const Vector3 imageSize((stageSize.x / imageColumns) - margin, (stageSize.y / IMAGE_ROWS) - margin, 0.0f);

    for(int row = 0;row<IMAGE_ROWS;row++)
    {
      for(int column = 0;column<imageColumns;column++)
      {
    	  char* img_path;

    	  int error = app_resource_manager_get(APP_RESOURCE_TYPE_IMAGE, GetNextImagePath(), &img_path);
    	  if (error == TIZEN_ERROR_NONE) {
    		  ImageView image = CreateImage( img_path, imageSize.x, imageSize.y );

    		  image.SetParentOrigin( ParentOrigin::CENTER );
    		  image.SetAnchorPoint( AnchorPoint::CENTER );

    		  Vector3 position( margin * 0.5f + (imageSize.x + margin) * column - stageSize.width * 0.5f,
    				  margin * 0.5f + (imageSize.y + margin) * row - stageSize.height * 0.5f,
					  0.0f);
    		  image.SetPosition( position + imageSize * 0.5f );
    		  image.SetSize( imageSize );
    		  page.Add(image);
    	  }
      }
    }

    return page;
  }

  /**
   * [ScrollView]
   * Applies effect to scrollView
   */
  void ApplyEffectToScrollView()
  {
    bool snap(true);

    Stage stage = Stage::GetCurrent();
    Vector2 stageSize = stage.GetSize();

    RulerPtr rulerX = CreateRuler(snap ? stageSize.width : 0.0f);
    RulerPtr rulerY = new DefaultRuler;
    rulerX->SetDomain(RulerDomain(0.0f, stageSize.x * PAGE_COLUMNS, false));
    rulerY->Disable();

    Dali::Path path = Dali::Path::New();
    Dali::Property::Array points;
        points.Resize(3);
    Dali::Property::Array controlPoints;
        controlPoints.Resize(4);
    Vector3 forward;
    if( mEffectMode == PageCarouselEffect)
    {

      points[0] = Vector3( stageSize.x*0.75, 0.0f,  -stageSize.x*0.75f);
      points[1] = Vector3( 0.0f, 0.0f, 0.0f );
      points[2] = Vector3( -stageSize.x*0.75f, 0.0f,  -stageSize.x*0.75f);
      path.SetProperty( Path::Property::POINTS, points );

      controlPoints[0] = Vector3( stageSize.x*0.5f, 0.0f, 0.0f );
      controlPoints[1] = Vector3( stageSize.x*0.5f, 0.0f, 0.0f );
      controlPoints[2] = Vector3(-stageSize.x*0.5f, 0.0f, 0.0f );
      controlPoints[3] = Vector3(-stageSize.x*0.5f, 0.0f, 0.0f );
      path.SetProperty( Path::Property::CONTROL_POINTS, controlPoints );

      forward = Vector3::ZERO;
    }
    else if( mEffectMode == PageCubeEffect)
    {
      points[0] = Vector3( stageSize.x*0.5, 0.0f,  stageSize.x*0.5f);
      points[1] = Vector3( 0.0f, 0.0f, 0.0f );
      points[2] = Vector3( -stageSize.x*0.5f, 0.0f, stageSize.x*0.5f);
      path.SetProperty( Path::Property::POINTS, points );

      controlPoints[0] = Vector3( stageSize.x*0.5f, 0.0f, stageSize.x*0.3f );
      controlPoints[1] = Vector3( stageSize.x*0.3f, 0.0f, 0.0f );
      controlPoints[2] = Vector3(-stageSize.x*0.3f, 0.0f, 0.0f );
      controlPoints[3] = Vector3(-stageSize.x*0.5f, 0.0f,  stageSize.x*0.3f );
      path.SetProperty( Path::Property::CONTROL_POINTS, controlPoints );

      forward = Vector3(-1.0f,0.0f,0.0f);
    }
    else if( mEffectMode == PageSpiralEffect)
    {
      points[0] = Vector3( stageSize.x*0.5, 0.0f,  -stageSize.x*0.5f);
      points[1] = Vector3( 0.0f, 0.0f, 0.0f );
      points[2] = Vector3( -stageSize.x*0.5f, 0.0f, -stageSize.x*0.5f);
      path.SetProperty( Path::Property::POINTS, points );

      controlPoints[0] = Vector3( stageSize.x*0.5f, 0.0f, 0.0f );
      controlPoints[1] = Vector3( stageSize.x*0.5f, 0.0f, 0.0f );
      controlPoints[2] = Vector3(-stageSize.x*0.5f, 0.0f, 0.0f );
      controlPoints[3] = Vector3(-stageSize.x*0.5f, 0.0f, 0.0f );
      path.SetProperty( Path::Property::CONTROL_POINTS, controlPoints );

      forward = Vector3(-1.0f,0.0f,0.0f);
    }
    else if( mEffectMode == PageWaveEffect)
    {
      points[0] = Vector3( stageSize.x, 0.0f,  -stageSize.x);
      points[1] = Vector3( 0.0f, 0.0f, 0.0f );
      points[2] = Vector3( -stageSize.x, 0.0f, -stageSize.x);
      path.SetProperty( Path::Property::POINTS, points );

      controlPoints[0] = Vector3( 0.0f, 0.0f, -stageSize.x );
      controlPoints[1] = Vector3( stageSize.x*0.5f, 0.0f, 0.0f );
      controlPoints[2] = Vector3( -stageSize.x*0.5f, 0.0f, 0.0f);
      controlPoints[3] = Vector3(0.0f, 0.0f,-stageSize.x  );
      path.SetProperty( Path::Property::CONTROL_POINTS, controlPoints );

      forward = Vector3(-1.0f,0.0f,0.0f);
    }

    mScrollViewEffect = ScrollViewPagePathEffect::New(path, forward,Toolkit::ScrollView::Property::SCROLL_FINAL_X, Vector3(stageSize.x,stageSize.y,0.0f),PAGE_COLUMNS);
    mScrollView.SetScrollSnapDuration(EFFECT_SNAP_DURATION);
    mScrollView.SetScrollFlickDuration(EFFECT_FLICK_DURATION);
    mScrollView.SetScrollSnapAlphaFunction(AlphaFunction::EASE_OUT);
    mScrollView.SetScrollFlickAlphaFunction(AlphaFunction::EASE_OUT);
    mScrollView.RemoveConstraintsFromChildren();

    rulerX = CreateRuler(snap ? stageSize.width * 0.5f : 0.0f);
    rulerX->SetDomain( RulerDomain( 0.0f, stageSize.x * 0.5f * PAGE_COLUMNS, false ) );

    unsigned int currentPage = mScrollView.GetCurrentPage();
    if( mScrollViewEffect )
    {
      mScrollView.ApplyEffect(mScrollViewEffect);
    }

    mScrollView.SetWrapMode( true );
    mScrollView.SetRulerX( rulerX );
    mScrollView.SetRulerY( rulerY );

    mScrollView.ScrollTo( currentPage, 0.0f );
  }

  /**
   * Creates a Ruler that snaps to a specified grid size.
   * If that grid size is 0.0 then this ruler does not
   * snap.
   *
   * @param[in] gridSize (optional) The grid size for the ruler,
   * (Default = 0.0 i.e. no snapping)
   * @return The ruler is returned.
   */
  RulerPtr CreateRuler(float gridSize = 0.0f)
  {
    if(gridSize <= Math::MACHINE_EPSILON_0)
    {
        return new DefaultRuler();
    }
    return new FixedRuler(gridSize);
  }
  // end switch
  /**
    * [Page]
    * Applies effect to the pages within scroll view.
    *
    * @param[in] page The page Actor to apply effect to.
    */
   void ApplyEffectToPage(Actor page, unsigned int pageOrder )
   {
     page.RemoveConstraints();
     page.SetResizePolicy( ResizePolicy::FILL_TO_PARENT, Dimension::ALL_DIMENSIONS );

     ScrollViewPagePathEffect effect = ScrollViewPagePathEffect::DownCast( mScrollViewEffect );
     effect.ApplyToPage( page, pageOrder );
   }

  /**
   * Creates an Image (Helper)
   *
   * @param[in] filename the path of the image.
   * @param[in] width the width of the image in texels
   * @param[in] height the height of the image in texels.
   */
  ImageView CreateImage( const std::string& filename, int width = IMAGE_THUMBNAIL_WIDTH, int height = IMAGE_THUMBNAIL_HEIGHT )
  {
	ResourceImage img = ResourceImage::New(filename, ImageDimensions( width, height ), Dali::FittingMode::SCALE_TO_FILL, Dali::SamplingMode::BOX_THEN_LINEAR );
	ImageView actor = ImageView::New(img);

    actor.SetName( basename(filename.c_str()) );
    actor.SetParentOrigin(ParentOrigin::CENTER);
    actor.SetAnchorPoint(AnchorPoint::CENTER);

    actor.TouchSignal().Connect( this, &ExampleController::OnTouchImage );
    return actor;
  }

  /**
   * When scroll starts (i.e. user starts to drag scrollview),
   * note this state (mScrolling = true)
   * @param[in] position Current Scroll Position
   */
  void OnScrollStarted( const Vector2& position )
  {
    mScrolling = true;
  }

  /**
   * When scroll starts (i.e. user stops dragging scrollview, and scrollview has snapped to destination),
   * note this state (mScrolling = false)
   * @param[in] position Current Scroll Position
   */
  void OnScrollCompleted( const Vector2& position )
  {
    mScrolling = false;
  }

  /**
   * Upon Touching an image (Release), make it spin
   * (provided we're not scrolling).
   * @param[in] actor The actor touched
   * @param[in] event The touch information.
   */
  bool OnTouchImage( Actor actor, const TouchData& event )
  {
    if( (event.GetPointCount() > 0) && (!mScrolling) )
    {
      if( event.GetState( 0 ) == PointState::UP )
      {
        // Spin the Image a few times.
        Animation animation = Animation::New(SPIN_DURATION);
        animation.AnimateBy( Property( actor, Actor::Property::ORIENTATION ), Quaternion( Radian( Degree(360.0f * SPIN_DURATION) ), Vector3::XAXIS ), AlphaFunction::EASE_OUT );
        animation.Play();
      }
    }
    return false;
  }

  /**
   * Signal handler, called when the 'Effect' button has been touched.
   *
   * @param[in] button The button that was pressed.
   */
  bool OnEffectTouched(Button button)
  {
    mEffectMode = static_cast<EffectMode>((static_cast<int>(mEffectMode) + 1) % static_cast<int>(Total));
    Update();
    return true;
  }

  /**
   * Main key event handler
   */
  void OnKeyEvent(const KeyEvent& event)
  {
    if(event.state == KeyEvent::Down)
    {
      if( IsKey( event, Dali::DALI_KEY_ESCAPE) || IsKey( event, Dali::DALI_KEY_BACK) )
      {
        mApplication.Quit();
      }
    }
  }

private:

  Application& mApplication;                            ///< Application instance
  Toolkit::Control mView;                               ///< The View instance.
  TextLabel mTitleActor;                                ///< The Toolbar's Title.
  Layer mContentLayer;                                  ///< The content layer (contains game actors)
  ScrollView mScrollView;                               ///< ScrollView UI Component
  bool mScrolling;                                      ///< ScrollView scrolling state (true = scrolling, false = stationary)
  ScrollViewEffect mScrollViewEffect;                   ///< ScrollView Effect instance.
  std::vector< Actor > mPages;                          ///< Keeps track of all the pages for applying effects.

  /**
   * Enumeration of different effects this scrollview can operate under.
   */
  enum EffectMode
  {
    PageCarouselEffect,                                 ///< Page carousel effect
    PageCubeEffect,                                     ///< Page cube effect
    PageSpiralEffect,                                   ///< Page spiral effect
    PageWaveEffect,                                     ///< Page wave effect

    Total
  };

  EffectMode mEffectMode;                               ///< Current Effect mode

  std::string mEffectIcon[Total];                       ///< Icons for the effect button
  std::string mEffectIconSelected[Total];               ///< Icons for the effect button when its selected
  Toolkit::PushButton mEffectChangeButton;              ///< Effect Change Button
};

int DALI_EXPORT_API main(int argc, char **argv)
{
  Application app = Application::New(&argc, &argv);
  StageHand debug_server;
  ExampleController test(app);
  app.MainLoop();
  return 0;
}
