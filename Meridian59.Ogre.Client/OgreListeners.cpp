#include "stdafx.h"

namespace Meridian59 { namespace Ogre 
{
	MyFrameListener::MyFrameListener()
	{
	};

	bool MyFrameListener::frameRenderingQueued (const FrameEvent &evt)
	{
		return true;
	};

	////////////////////////////////////////////
	
	MyWindowEventListener::MyWindowEventListener()
	{
	};

	bool MyWindowEventListener::windowClosing (RenderWindow* rw)
	{
		// mark for shutdown ourself
		OgreClient::Singleton->IsRunning = false;

		// ignore window closing here
		return false;
	};

	void MyWindowEventListener::windowFocusChange (RenderWindow* rw)
	{
	};

	////////////////////////////////////////////

	CameraListener::CameraListener()
	{
	};

	CameraListener::~CameraListener(void)
	{
	};

	void CameraListener::objectMoved(MovableObject* obj)
	{
		MovableObject::Listener::objectMoved(obj);

		if (OgreClient::Singleton->Camera != nullptr)
        {			
			::Ogre::Vector3 pos = OgreClient::Singleton->Camera->getDerivedPosition();

            // update viewer position in datalayer
            OgreClient::Singleton->Data->ViewerPosition = 
				Util::ToV3(pos); 

			// update mousover target if mouse is not on UI
			if (ControllerUI::IsInitialized &&
				ControllerInput::IsInitialized && 
				ControllerUI::TopControl == nullptr)
			{
				// get mouse coords
				const OIS::MouseState mouseState = ControllerInput::OISMouse->getMouseState();
			
				// perform mouseover update of object if no mouse down
				if (!ControllerInput::IsLeftMouseDown && !ControllerInput::IsRightMouseDown) 
					ControllerInput::PerformMouseOver(mouseState.X.abs, mouseState.Y.abs, false);
			}
        }
	};

	////////////////////////////////////////////

	void CompositorPainListener::notifyMaterialSetup( uint32 pass_id, MaterialPtr & mat )
	{
	};

	void CompositorPainListener::notifyMaterialRender( uint32 pass_id, MaterialPtr & mat )
	{
		// build a red blendcolor with alpha based on progress
		// we scale pain effect here from 30-0% blending
		float alpha = 0.3f - (0.3f * OgreClient::Singleton->Data->Effects->Pain->Progress);
		float blendcolor[4];
		blendcolor[0] = 1.0f;  // r
		blendcolor[1] = 0.0f;  // g
		blendcolor[2] = 0.0f;  // b
		blendcolor[3] = alpha; // a

		// get shader parameters
		const GpuProgramParametersSharedPtr list = 
			mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters();

		// set current blendcolor in shader
		list->setNamedConstant(SHADERBLENDCOLOR, blendcolor, 1);
	};

	void CompositorPainListener::notifyResourcesCreated(bool forResizeOnly)
	{
	};

	////////////////////////////////////////////

	void CompositorWhiteoutListener::notifyMaterialSetup( uint32 pass_id, MaterialPtr & mat )
	{

	};

	void CompositorWhiteoutListener::notifyMaterialRender( uint32 pass_id, MaterialPtr & mat )
	{
		// build a white blendcolor with alpha based on progress
		float alpha = 1.0f - OgreClient::Singleton->Data->Effects->Whiteout->Progress;
		float blendcolor[4];
		blendcolor[0] = 1.0f;  // r
		blendcolor[1] = 1.0f;  // g
		blendcolor[2] = 1.0f;  // b
		blendcolor[3] = alpha; // a

		// get shader parameters
		const GpuProgramParametersSharedPtr list = 
			mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters();

		// set current blendcolor in shader
		list->setNamedConstant(SHADERBLENDCOLOR, blendcolor, 1);
	};

	void CompositorWhiteoutListener::notifyResourcesCreated(bool forResizeOnly)
	{
	};
	
	////////////////////////////////////////////

	void LoadingBarResourceGroupListener::resourceGroupScriptingStarted(const String &groupName, size_t scriptCount)
	{
		ControllerUI::LoadingBar::ResourceGroupScriptingStarted(&groupName, scriptCount);
	};

	void LoadingBarResourceGroupListener::scriptParseStarted(const String &scriptName, bool &skipThisScript)
	{
		ControllerUI::LoadingBar::ScriptParseStarted(&scriptName, skipThisScript);
	};

	void LoadingBarResourceGroupListener::scriptParseEnded(const String &scriptName, bool skipped)
	{
		ControllerUI::LoadingBar::ScriptParseEnded(&scriptName, skipped);
	};

	void LoadingBarResourceGroupListener::resourceGroupLoadStarted(const String &groupName, size_t resourceCount)
	{
		ControllerUI::LoadingBar::ResourceGroupLoadStarted(&groupName, resourceCount);
	};

	void LoadingBarResourceGroupListener::resourceGroupPrepareStarted(const String &groupName, size_t resourceCount)
	{
		ControllerUI::LoadingBar::ResourceGroupPrepareStarted(&groupName, resourceCount);
	};

	void LoadingBarResourceGroupListener::resourceLoadStarted(const ResourcePtr &resource)
	{
		ControllerUI::LoadingBar::ResourceLoadStarted(resource);
	};

	void LoadingBarResourceGroupListener::worldGeometryStageStarted(const String &description)
	{
		ControllerUI::LoadingBar::WorldGeometryStageStarted(&description);
	};

	void LoadingBarResourceGroupListener::worldGeometryStageEnded()
	{
		ControllerUI::LoadingBar::WorldGeometryStageEnded();
	};
};};