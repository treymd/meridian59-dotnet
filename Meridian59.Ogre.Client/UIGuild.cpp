#include "stdafx.h"

namespace Meridian59 { namespace Ogre
{
	void ControllerUI::Guild::Initialize()
	{
		// setup references to children from xml nodes
		Window			= static_cast<CEGUI::FrameWindow*>(guiRoot->getChild(UI_NAME_GUILD_WINDOW));
		TabControl		= static_cast<CEGUI::TabControl*>(Window->getChild(UI_NAME_GUILD_TABCONTROL));
		Renounce		= static_cast<CEGUI::PushButton*>(Window->getChild(UI_NAME_GUILD_RENOUNCE));
		
		TabMembers		= static_cast<CEGUI::Window*>(TabControl->getChild(UI_NAME_GUILD_TABMEMBERS));
		TabDiplomacy	= static_cast<CEGUI::Window*>(TabControl->getChild(UI_NAME_GUILD_TABDIPLOMACY));
		TabGuildmaster	= static_cast<CEGUI::Window*>(TabControl->getChild(UI_NAME_GUILD_TABGUILDMASTER));
		TabShield		= static_cast<CEGUI::Window*>(TabControl->getChild(UI_NAME_GUILD_TABSHIELD));
		
		ListMembers		= static_cast<CEGUI::ItemListbox*>(TabMembers->getChild(UI_NAME_GUILD_LISTMEMBERS));
		ListGuilds		= static_cast<CEGUI::ItemListbox*>(TabDiplomacy->getChild(UI_NAME_GUILD_LISTGUILDS));
		
		PasswordDesc	= static_cast<CEGUI::Window*>(TabGuildmaster->getChild(UI_NAME_GUILD_PASSWORDDESC));
		PasswordVal		= static_cast<CEGUI::Editbox*>(TabGuildmaster->getChild(UI_NAME_GUILD_PASSWORDVAL));
		SetPassword		= static_cast<CEGUI::PushButton*>(TabGuildmaster->getChild(UI_NAME_GUILD_SETPASSWORD));
			
		ShieldImage			= static_cast<CEGUI::Window*>(TabShield->getChild(UI_NAME_GUILD_SHIELDIMAGE));		
		ShieldColor1Desc	= static_cast<CEGUI::Window*>(TabShield->getChild(UI_NAME_GUILD_SHIELDCOLOR1DESC));
		ShieldColor1		= static_cast<CEGUI::Slider*>(TabShield->getChild(UI_NAME_GUILD_SHIELDCOLOR1));
		ShieldColor2Desc	= static_cast<CEGUI::Window*>(TabShield->getChild(UI_NAME_GUILD_SHIELDCOLOR2DESC));
		ShieldColor2		= static_cast<CEGUI::Slider*>(TabShield->getChild(UI_NAME_GUILD_SHIELDCOLOR2));
		ShieldDesignDesc	= static_cast<CEGUI::Window*>(TabShield->getChild(UI_NAME_GUILD_SHIELDDESIGNDESC));
		ShieldDesign		= static_cast<CEGUI::Slider*>(TabShield->getChild(UI_NAME_GUILD_SHIELDDESIGN));
		ShieldClaimedByDesc = static_cast<CEGUI::Window*>(TabShield->getChild(UI_NAME_GUILD_SHIELDCLAIMEDBYDESC));
		ShieldClaimedBy		= static_cast<CEGUI::Window*>(TabShield->getChild(UI_NAME_GUILD_SHIELDCLAIMEDBY));
		ShieldClaim			= static_cast<CEGUI::PushButton*>(TabShield->getChild(UI_NAME_GUILD_SHIELDCLAIM));

		ShieldColor1->setClickStep(1.0f);
		ShieldColor2->setClickStep(1.0f);
		ShieldDesign->setClickStep(1.0f);

		ShieldColor1->setMaxValue((float)(Drawing2D::ColorTransformation::NUMGUILDCOLORS - 1));
		ShieldColor2->setMaxValue((float)(Drawing2D::ColorTransformation::NUMGUILDCOLORS - 1));

		ShieldColor1->setCurrentValue((float)OgreClient::Singleton->Data->GuildShieldInfo->Color1);
		ShieldColor2->setCurrentValue((float)OgreClient::Singleton->Data->GuildShieldInfo->Color2);
		ShieldDesign->setCurrentValue((float)(OgreClient::Singleton->Data->GuildShieldInfo->Design - 1));
	
		// image composer for shield picture (hotspot=1 is head)
		imageComposerShield = gcnew ImageComposerCEGUI<ObjectBase^>();
		imageComposerShield->ApplyYOffset = false;
		imageComposerShield->IsScalePow2 = false;
        imageComposerShield->UseViewerFrame = false;
		imageComposerShield->Width = (unsigned int)ShieldImage->getPixelSize().d_width;
        imageComposerShield->Height = (unsigned int)ShieldImage->getPixelSize().d_height;
        imageComposerShield->CenterHorizontal = true;
        imageComposerShield->CenterVertical = true;
		imageComposerShield->NewImageAvailable += gcnew ::System::EventHandler(OnNewShieldImageAvailable);
		imageComposerShield->DataSource = OgreClient::Singleton->Data->GuildShieldInfo->ExampleModel;

		// attach listener to guildinfo data
		OgreClient::Singleton->Data->GuildInfo->PropertyChanged += 
			gcnew PropertyChangedEventHandler(OnGuildInfoPropertyChanged);
        
		// attach listener to guildshieldinfo data
		OgreClient::Singleton->Data->GuildShieldInfo->PropertyChanged += 
			gcnew PropertyChangedEventHandler(OnGuildShieldInfoPropertyChanged);
        
		// attach listener to guildmembers
		OgreClient::Singleton->Data->GuildInfo->GuildMembers->ListChanged += 
			gcnew ListChangedEventHandler(OnMembersListChanged);
		
		// attach listener to guilds
		OgreClient::Singleton->Data->DiplomacyInfo->Guilds->ListChanged += 
			gcnew ListChangedEventHandler(OnGuildsListChanged);

		// subscribe passwordset button
		SetPassword->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(UICallbacks::Guild::OnSetPasswordClicked));

		// subscribe renounec/abandon button
		Renounce->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(UICallbacks::Guild::OnRenounceClicked));

		// subscribe mouse wheel to image
		ShieldImage->subscribeEvent(CEGUI::Window::EventMouseWheel, CEGUI::Event::Subscriber(UICallbacks::Guild::OnImageMouseWheel));

		// subscribe sliders
		ShieldColor1->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(UICallbacks::Guild::OnGuildShieldSettingChanged));
		ShieldColor2->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(UICallbacks::Guild::OnGuildShieldSettingChanged));
		ShieldDesign->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(UICallbacks::Guild::OnGuildShieldSettingChanged));

		// subscribe claimshield button
		ShieldClaim->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(UICallbacks::Guild::OnShieldClaimClicked));

		// subscribe close button
		Window->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked, CEGUI::Event::Subscriber(UICallbacks::Guild::OnWindowClosed));

		// subscribe keyup
		Window->subscribeEvent(CEGUI::FrameWindow::EventKeyUp, CEGUI::Event::Subscriber(UICallbacks::Guild::OnWindowKeyUp));
	};

	void ControllerUI::Guild::Destroy()
	{	
		// detach listener from guildinfo data
		OgreClient::Singleton->Data->GuildInfo->PropertyChanged -= 
			gcnew PropertyChangedEventHandler(OnGuildInfoPropertyChanged);
         
		// detach listener from guildshieldinfo data
		OgreClient::Singleton->Data->GuildShieldInfo->PropertyChanged -= 
			gcnew PropertyChangedEventHandler(OnGuildShieldInfoPropertyChanged);
        
		// detach listener from guildmembers
		OgreClient::Singleton->Data->GuildInfo->GuildMembers->ListChanged -= 
			gcnew ListChangedEventHandler(OnMembersListChanged);
		
		// detach listener from guilds
		OgreClient::Singleton->Data->DiplomacyInfo->Guilds->ListChanged -= 
			gcnew ListChangedEventHandler(OnGuildsListChanged);		

		imageComposerShield->NewImageAvailable -= gcnew ::System::EventHandler(OnNewShieldImageAvailable);
	};

	void ControllerUI::Guild::OnGuildInfoPropertyChanged(Object^ sender, PropertyChangedEventArgs^ e)
	{
		Data::Models::GuildInfo^ obj = OgreClient::Singleton->Data->GuildInfo;
		
		// visible
		if (::System::String::Equals(e->PropertyName, Data::Models::GuildInfo::PROPNAME_ISVISIBLE))
		{
			// hide or show
			Window->setVisible(obj->IsVisible);

			// bring to front
			if (obj->IsVisible)
				Window->moveToFront();
		}

		// guildname
		else if (::System::String::Equals(e->PropertyName, Data::Models::GuildInfo::PROPNAME_GUILDNAME))
		{
			Window->setText(StringConvert::CLRToCEGUI(obj->GuildName));
		}

		// chestpassword
		else if (::System::String::Equals(e->PropertyName, Data::Models::GuildInfo::PROPNAME_CHESTPASSWORD))
		{
			PasswordVal->setText(StringConvert::CLRToCEGUI(obj->ChestPassword));
		}

		// flags
		else if (::System::String::Equals(e->PropertyName, Data::Models::GuildInfo::PROPNAME_FLAGS))
		{
			if (obj->Flags->IsRenounce)
			{
				Renounce->setText("Renounce");
			}
			else if (obj->Flags->IsDisband)
			{
				Renounce->setText("Disband");
			}
		}
	};
	
	void ControllerUI::Guild::OnGuildShieldInfoPropertyChanged(Object^ sender, PropertyChangedEventArgs^ e)
	{
		GuildShieldInfo^ shieldInfo	= OgreClient::Singleton->Data->GuildShieldInfo;
		GuildInfo^ guildInfo		= OgreClient::Singleton->Data->GuildInfo;

		// Color1
		if (::System::String::Equals(e->PropertyName, Data::Models::GuildShieldInfo::PROPNAME_COLOR1))
		{
			ShieldColor1->setCurrentValue((float)shieldInfo->Color1);
		}

		// Color2
		else if (::System::String::Equals(e->PropertyName, Data::Models::GuildShieldInfo::PROPNAME_COLOR2))
		{
			ShieldColor2->setCurrentValue((float)shieldInfo->Color2);
		}

		// Design
		else if (::System::String::Equals(e->PropertyName, Data::Models::GuildShieldInfo::PROPNAME_DESIGN))
		{
			ShieldDesign->setCurrentValue((float)(shieldInfo->Design - 1));
		}

		// GuildName
		else if (::System::String::Equals(e->PropertyName, Data::Models::GuildShieldInfo::PROPNAME_GUILDNAME))
		{
			ShieldClaimedBy->setText(StringConvert::CLRToCEGUI(shieldInfo->GuildName));
		}
		
		// GuildID
		else if (::System::String::Equals(e->PropertyName, Data::Models::GuildShieldInfo::PROPNAME_GUILDID))
		{
		}

		// Shields
		else if (::System::String::Equals(e->PropertyName, Data::Models::GuildShieldInfo::PROPNAME_SHIELDS))
		{
			// designvalue is 1-based, slider is 0-based
			ShieldDesign->setMaxValue((float)shieldInfo->Shields->Length - 1.0f);
		}
	};

	void ControllerUI::Guild::OnMembersListChanged(Object^ sender, ListChangedEventArgs^ e)
	{
		switch(e->ListChangedType)
		{
			case ::System::ComponentModel::ListChangedType::ItemAdded:
				MemberAdd(e->NewIndex);			
				break;

			case ::System::ComponentModel::ListChangedType::ItemDeleted:
				MemberRemove(e->NewIndex);
				break;

			case ::System::ComponentModel::ListChangedType::ItemChanged:
				MemberChange(e->NewIndex);
				break;
		}
	};

	void ControllerUI::Guild::OnGuildsListChanged(Object^ sender, ListChangedEventArgs^ e)
	{
		switch(e->ListChangedType)
		{
			case ::System::ComponentModel::ListChangedType::ItemAdded:
				GuildAdd(e->NewIndex);			
				break;

			case ::System::ComponentModel::ListChangedType::ItemDeleted:
				GuildRemove(e->NewIndex);
				break;

			case ::System::ComponentModel::ListChangedType::ItemChanged:
				GuildChange(e->NewIndex);
				break;
		}
	};

	void ControllerUI::Guild::OnNewShieldImageAvailable(Object^ sender, ::System::EventArgs^ e)
    {
		ShieldImage->setProperty(UI_PROPNAME_IMAGE, *imageComposerShield->Image->TextureName);
	};

	void ControllerUI::Guild::MemberAdd(int Index)
	{
		CEGUI::WindowManager* wndMgr = CEGUI::WindowManager::getSingletonPtr();
		
		GuildInfo^ info			= OgreClient::Singleton->Data->GuildInfo;
		GuildMemberEntry^ obj	= info->GuildMembers[Index];
		
		// create widget (item)
		CEGUI::ItemEntry* widget = (CEGUI::ItemEntry*)wndMgr->createWindow(
			UI_WINDOWTYPE_GUILDMEMBERLISTBOXITEM);

		// set id
		widget->setID(obj->ID);

		// get children
		CEGUI::ToggleButton* supported = (CEGUI::ToggleButton*)widget->getChildAtIdx(UI_GUILD_CHILDINDEX_MEMBER_SUPPORTED);
		CEGUI::PushButton* exile = (CEGUI::PushButton*)widget->getChildAtIdx(UI_GUILD_CHILDINDEX_MEMBER_EXILE);
		CEGUI::Window* name = (CEGUI::Window*)widget->getChildAtIdx(UI_GUILD_CHILDINDEX_MEMBER_NAME);
		CEGUI::Combobox* rank = (CEGUI::Combobox*)widget->getChildAtIdx(UI_GUILD_CHILDINDEX_MEMBER_RANK);
							
		// subscribe events
		rank->subscribeEvent(
			CEGUI::Combobox::EventListSelectionAccepted, 
			CEGUI::Event::Subscriber(UICallbacks::Guild::OnRankSelectionChanged));		

		// subscribe checked event
		supported->subscribeEvent(
			CEGUI::ToggleButton::EventSelectStateChanged, 
			CEGUI::Event::Subscriber(UICallbacks::Guild::OnSupportedSelectStateChanged));
		
		// subscribe exile event
		exile->subscribeEvent(
			CEGUI::PushButton::EventClicked, 
			CEGUI::Event::Subscriber(UICallbacks::Guild::OnExileClicked));
			
		// create combobox values
		if (obj->Gender == Gender::Female)
		{
			CEGUI::ListboxTextItem* itmRank1 = new CEGUI::ListboxTextItem(
				StringConvert::CLRToCEGUI(info->Rank1Female), 1);
				
			CEGUI::ListboxTextItem* itmRank2 = new CEGUI::ListboxTextItem(
				StringConvert::CLRToCEGUI(info->Rank2Female), 2);
				
			CEGUI::ListboxTextItem* itmRank3 = new CEGUI::ListboxTextItem(
				StringConvert::CLRToCEGUI(info->Rank3Female), 3);
				
			CEGUI::ListboxTextItem* itmRank4 = new CEGUI::ListboxTextItem(
				StringConvert::CLRToCEGUI(info->Rank4Female), 4);

			CEGUI::ListboxTextItem* itmRank5 = new CEGUI::ListboxTextItem(
				StringConvert::CLRToCEGUI(info->Rank5Female), 5);

			rank->addItem(itmRank1);
			rank->addItem(itmRank2);
			rank->addItem(itmRank3);
			rank->addItem(itmRank4);
			rank->addItem(itmRank5);
		}
		else
		{
			CEGUI::ListboxTextItem* itmRank1 = new CEGUI::ListboxTextItem(
				StringConvert::CLRToCEGUI(info->Rank1Male), 1);
				
			CEGUI::ListboxTextItem* itmRank2 = new CEGUI::ListboxTextItem(
				StringConvert::CLRToCEGUI(info->Rank2Male), 2);
				
			CEGUI::ListboxTextItem* itmRank3 = new CEGUI::ListboxTextItem(
				StringConvert::CLRToCEGUI(info->Rank3Male), 3);
				
			CEGUI::ListboxTextItem* itmRank4 = new CEGUI::ListboxTextItem(
				StringConvert::CLRToCEGUI(info->Rank4Male), 4);

			CEGUI::ListboxTextItem* itmRank5 = new CEGUI::ListboxTextItem(
				StringConvert::CLRToCEGUI(info->Rank5Male), 5);

			rank->addItem(itmRank1);
			rank->addItem(itmRank2);
			rank->addItem(itmRank3);
			rank->addItem(itmRank4);
			rank->addItem(itmRank5);
		}

		// insert in ui-list
		if ((int)ListMembers->getItemCount() > Index)
			ListMembers->insertItem(widget, ListMembers->getItemFromIndex(Index));
		
		// or add
		else
			ListMembers->addItem(widget);

		// update values
		MemberChange(Index);
	};

	void ControllerUI::Guild::MemberRemove(int Index)
	{
		// check
		if ((int)ListMembers->getItemCount() > Index)		
			ListMembers->removeItem(ListMembers->getItemFromIndex(Index));
	};

	void ControllerUI::Guild::MemberChange(int Index)
	{
		GuildInfo^ info = OgreClient::Singleton->Data->GuildInfo;
		GuildMemberEntry^ obj = OgreClient::Singleton->Data->GuildInfo->GuildMembers[Index];
		
		// check
		if ((int)ListMembers->getItemCount() > Index)
		{
			CEGUI::ItemEntry* widget = (CEGUI::ItemEntry*)ListMembers->getItemFromIndex(Index);

			CEGUI::ToggleButton* supported	= (CEGUI::ToggleButton*)widget->getChildAtIdx(UI_GUILD_CHILDINDEX_MEMBER_SUPPORTED);
			CEGUI::PushButton* exile		= (CEGUI::PushButton*)widget->getChildAtIdx(UI_GUILD_CHILDINDEX_MEMBER_EXILE);
			CEGUI::Window* name				= (CEGUI::Window*)widget->getChildAtIdx(UI_GUILD_CHILDINDEX_MEMBER_NAME);
			CEGUI::Combobox* rank			= (CEGUI::Combobox*)widget->getChildAtIdx(UI_GUILD_CHILDINDEX_MEMBER_RANK);
			
			// set name
			name->setText(StringConvert::CLRToCEGUI(obj->Name));

			bool isSupportedMember = (obj->ID == info->SupportedMember->ID);
			bool isAvatar = (obj->ID == OgreClient::Singleton->Data->AvatarID);

			// set supported state
			supported->setSelected(info->Flags->IsVote && isSupportedMember);
			supported->setEnabled(info->Flags->IsVote && !isSupportedMember);
			
			// enable/disable exile
			exile->setEnabled(info->Flags->IsExile && !isAvatar);
		
			// select rank
			rank->setEnabled(info->Flags->IsSetRank && !isAvatar);
			int index = obj->Rank - 1;
			if ((int)rank->getItemCount() > index)
			{
				// select
				CEGUI::ListboxItem* itm = rank->getListboxItemFromIndex(index);
				itm->setSelected(true);

				// set selected text
				rank->setText(itm->getText());
			}
		}	
	};

	void ControllerUI::Guild::GuildAdd(int Index)
	{
		CEGUI::WindowManager* wndMgr = CEGUI::WindowManager::getSingletonPtr();
		DiplomacyInfo^ info = OgreClient::Singleton->Data->DiplomacyInfo;
		GuildInfo^ ginfo = OgreClient::Singleton->Data->GuildInfo;
		GuildEntry^ obj = info->Guilds[Index];
		
		// create widget (item)
		CEGUI::ItemEntry* widget = (CEGUI::ItemEntry*)wndMgr->createWindow(
			UI_WINDOWTYPE_GUILDENTRYLISTBOXITEM);

		// set id
		widget->setID(obj->ID);

		// get children
		CEGUI::Window* name = (CEGUI::Window*)widget->getChildAtIdx(UI_GUILD_CHILDINDEX_GUILD_NAME);
		CEGUI::Window* theirdiplo = (CEGUI::Window*)widget->getChildAtIdx(UI_GUILD_CHILDINDEX_GUILD_THEIRDIPLO);
		CEGUI::Combobox* ourdiplo = (CEGUI::Combobox*)widget->getChildAtIdx(UI_GUILD_CHILDINDEX_GUILD_OURDIPLO);
		
		// set name
		name->setText(StringConvert::CLRToCEGUI(obj->Name));

		// set their diplo
		if (info->DeclaredYouAllyList->GetItemByID(obj->ID) != nullptr)
		{
			theirdiplo->setText("Ally");
		}
		else if (info->DeclaredYouEnemyList->GetItemByID(obj->ID) != nullptr)
		{
			theirdiplo->setText("Enemy");
		}
		else
		{
			theirdiplo->setText("Neutral");
		}

		// fill combobox
		CEGUI::ListboxTextItem* itm1 = new CEGUI::ListboxTextItem("Ally", 0);				
		CEGUI::ListboxTextItem* itm2 = new CEGUI::ListboxTextItem("Neutral", 1);			
		CEGUI::ListboxTextItem* itm3 = new CEGUI::ListboxTextItem("Enemy", 2);
		ourdiplo->addItem(itm1);
		ourdiplo->addItem(itm2);
		ourdiplo->addItem(itm3);

		// set our diplo
		int index = 1;
		if (info->YouDeclaredAllyList->GetItemByID(obj->ID) != nullptr)
		{
			index = 0;
		}
		else if (info->YouDeclaredEnemyList->GetItemByID(obj->ID) != nullptr)
		{
			index = 2;
		}

		// set ourdiplo selected item/text
		CEGUI::ListboxItem* itm = ourdiplo->getListboxItemFromIndex(index);
		itm->setSelected(true);
		ourdiplo->setText(itm->getText());
		
		// subscribe events
		ourdiplo->subscribeEvent(
			CEGUI::Combobox::EventListSelectionAccepted, 
			CEGUI::Event::Subscriber(UICallbacks::Guild::OnDiploSelectionChanged));						
		
		// enable it if not our own guild and if at least one set-right
		ourdiplo->setEnabled((obj->ID != ginfo->GuildID->ID) &&
			(ginfo->Flags->IsDeclareEnemy || ginfo->Flags->IsEndEnemy || 
			 ginfo->Flags->IsEndAlliance || ginfo->Flags->IsMakeAlliance));

		// insert in ui-list
		if ((int)ListGuilds->getItemCount() > Index)
			ListGuilds->insertItem(widget, ListGuilds->getItemFromIndex(Index));
		
		// or add
		else
			ListGuilds->addItem(widget);

		// update values
		GuildChange(Index);
	};

	void ControllerUI::Guild::GuildRemove(int Index)
	{
		// check
		if ((int)ListGuilds->getItemCount() > Index)		
			ListGuilds->removeItem(ListGuilds->getItemFromIndex(Index));
	};

	void ControllerUI::Guild::GuildChange(int Index)
	{
		
	};

	bool UICallbacks::Guild::OnSupportedSelectStateChanged(const CEGUI::EventArgs& e)
	{
		const CEGUI::WindowEventArgs& args = (const CEGUI::WindowEventArgs&)e;
		CEGUI::ItemListbox* listBox = ControllerUI::Guild::ListMembers;
		CEGUI::ToggleButton* btn	= (CEGUI::ToggleButton*)args.window;
		CEGUI::ItemEntry* itm		= (CEGUI::ItemEntry*)btn->getParent();

		GuildInfo^ guildInfo		= OgreClient::Singleton->Data->GuildInfo;
		GuildMemberList^ dataModels = guildInfo->GuildMembers;
			
		// if it's selected, 
		if (btn->isSelected())
		{			
			// clear all other selections
			int index = -1;
			for (int i = 0; i < (int)listBox->getItemCount(); i++)
			{					
				CEGUI::ItemEntry* widget = (CEGUI::ItemEntry*)listBox->getItemFromIndex(i);
				CEGUI::ToggleButton* supported = (CEGUI::ToggleButton*)widget->getChildAtIdx(UI_GUILD_CHILDINDEX_MEMBER_SUPPORTED);
				
				// not the one just switched
				if (btn != supported)
				{
					supported->setSelected(false);
					supported->setEnabled(true);
				}
				else
				{
					//supported->setSelected(true);
					supported->setEnabled(false);
					index = i;
				}
			}

			// send update
			if (index > -1 && 
				dataModels->Count > index && 
				dataModels[index]->ID != guildInfo->SupportedMember->ID)
			{
				// update supported id ourself (no echo from server)
				guildInfo->SupportedMember->ID = dataModels[index]->ID;

				// send update
				OgreClient::Singleton->SendUserCommandGuildVote(dataModels[index]->ID); 
			}
		}

		return true;
	}

	bool UICallbacks::Guild::OnRankSelectionChanged(const CEGUI::EventArgs& e)
	{
		const CEGUI::WindowEventArgs& args	= (const CEGUI::WindowEventArgs&)e;
		CEGUI::Combobox* box				= (CEGUI::Combobox*)args.window;
		CEGUI::ItemEntry* itm				= (CEGUI::ItemEntry*)box->getParent();
		
		unsigned int avatarID = OgreClient::Singleton->Data->AvatarID;
		unsigned int memberID = itm->getID();
		unsigned char rank	  = (unsigned char)box->getSelectedItem()->getID();
		
		// get our own object and the one we're changing
		GuildInfo^ guildInfo	 = OgreClient::Singleton->Data->GuildInfo;
		GuildMemberEntry^ avatar = guildInfo->GuildMembers->GetItemByID(avatarID);
		GuildMemberEntry^ member = guildInfo->GuildMembers->GetItemByID(memberID);

		// send update
		if (avatar != nullptr && member != nullptr)
		{							
			// setrank
			if (guildInfo->Flags->IsSetRank && 
				avatar->Rank > rank && 
				avatar->Rank > member->Rank &&
				member->Rank != rank)
			{
				OgreClient::Singleton->SendUserCommandGuildSetRank(member->ID, rank);

				// clear and re-request (workaroung since there's no updating)
				OgreClient::Singleton->Data->GuildInfo->Clear(true);
				OgreClient::Singleton->Data->GuildShieldInfo->Clear(true);
				OgreClient::Singleton->SendUserCommandGuildInfoReq();
				OgreClient::Singleton->SendUserCommandGuildShieldInfoReq();
			}

			// abdicate
			else if (guildInfo->Flags->IsAbdicate && avatar->Rank == 5 && rank == 5)
			{
				OgreClient::Singleton->SendUserCommandGuildAbdicate(member->ID);

				// clear and re-request (workaroung since there's no updating)
				OgreClient::Singleton->Data->GuildInfo->Clear(true);
				OgreClient::Singleton->Data->GuildShieldInfo->Clear(true);
				OgreClient::Singleton->SendUserCommandGuildInfoReq();
				OgreClient::Singleton->SendUserCommandGuildShieldInfoReq();
			}

			// reset value
			else
			{
				// reset combobox to datamodel
				int index2 = member->Rank - 1;
				if ((int)box->getItemCount() > index2)
				{
					// select
					CEGUI::ListboxItem* itm = box->getListboxItemFromIndex(index2);
					itm->setSelected(true);

					// set selected text
					box->setText(itm->getText());
				}
			}		
		}
	
		return true;
	}

	bool UICallbacks::Guild::OnDiploSelectionChanged(const CEGUI::EventArgs& e)
	{
		const CEGUI::WindowEventArgs& args = static_cast<const CEGUI::WindowEventArgs&>(e);
		CEGUI::ItemListbox* listBox = ControllerUI::Guild::ListGuilds;
		CEGUI::Combobox* box		= (CEGUI::Combobox*)args.window;
		CEGUI::ItemEntry* itm		= (CEGUI::ItemEntry*)box->getParent();
		
		DiplomacyInfo^ diplInfo = OgreClient::Singleton->Data->DiplomacyInfo;
		GuildInfo^ guildInfo	= OgreClient::Singleton->Data->GuildInfo;
		
		// get index
		int index = (int)listBox->getItemIndex(itm);

		// send update
		if (diplInfo->Guilds->Count > index)
		{				
			int oldindex = 1;
			
			if (diplInfo->YouDeclaredAllyList->GetItemByID(diplInfo->Guilds[index]->ID) != nullptr)
				oldindex = 0;
			
			else if (diplInfo->YouDeclaredEnemyList->GetItemByID(diplInfo->Guilds[index]->ID) != nullptr)
				oldindex = 2;

			int newindex = (int)box->getItemIndex(box->getSelectedItem());;

			// declare enemy from neutral
			if (oldindex == 1 && newindex == 2 && 
				guildInfo->Flags->IsDeclareEnemy)
			{
				OgreClient::Singleton->SendUserCommandGuildMakeEnemy(diplInfo->Guilds[index]->ID);
				diplInfo->YouDeclaredEnemyList->Add(gcnew ObjectID(diplInfo->Guilds[index]->ID, 0));
			}

			// declare enemy from ally
			else if (oldindex == 0 && newindex == 2 &&
				guildInfo->Flags->IsEndAlliance && guildInfo->Flags->IsDeclareEnemy)
			{
				OgreClient::Singleton->SendUserCommandGuildEndAlliance(diplInfo->Guilds[index]->ID);
				OgreClient::Singleton->SendUserCommandGuildMakeEnemy(diplInfo->Guilds[index]->ID);
				diplInfo->YouDeclaredAllyList->Remove(diplInfo->YouDeclaredAllyList->GetItemByID(diplInfo->Guilds[index]->ID));
				diplInfo->YouDeclaredEnemyList->Add(gcnew ObjectID(diplInfo->Guilds[index]->ID, 0));
			}

			// declare ally from neutral
			else if (oldindex == 1 && newindex == 0 &&
				guildInfo->Flags->IsMakeAlliance)
			{
				OgreClient::Singleton->SendUserCommandGuildMakeAlliance(diplInfo->Guilds[index]->ID);
				diplInfo->YouDeclaredAllyList->Add(gcnew ObjectID(diplInfo->Guilds[index]->ID, 0));
			}

			// declare ally from enemy
			else if (oldindex == 2 && newindex == 0 &&
				guildInfo->Flags->IsEndEnemy && guildInfo->Flags->IsMakeAlliance)
			{
				OgreClient::Singleton->SendUserCommandGuildEndEnemy(diplInfo->Guilds[index]->ID);
				OgreClient::Singleton->SendUserCommandGuildMakeAlliance(diplInfo->Guilds[index]->ID);
				diplInfo->YouDeclaredEnemyList->Remove(diplInfo->YouDeclaredEnemyList->GetItemByID(diplInfo->Guilds[index]->ID));
				diplInfo->YouDeclaredAllyList->Add(gcnew ObjectID(diplInfo->Guilds[index]->ID, 0));
			}

			// declare neutral from enemy
			else if (oldindex == 2 && newindex == 1 &&
				guildInfo->Flags->IsEndEnemy)
			{
				OgreClient::Singleton->SendUserCommandGuildEndEnemy(diplInfo->Guilds[index]->ID);
				diplInfo->YouDeclaredEnemyList->Remove(diplInfo->YouDeclaredEnemyList->GetItemByID(diplInfo->Guilds[index]->ID));
			}

			// declare neutral from ally
			else if (oldindex == 0 && newindex == 1 &&
				guildInfo->Flags->IsEndAlliance)
			{
				OgreClient::Singleton->SendUserCommandGuildEndAlliance(diplInfo->Guilds[index]->ID);
				diplInfo->YouDeclaredAllyList->Remove(diplInfo->YouDeclaredAllyList->GetItemByID(diplInfo->Guilds[index]->ID));
			}

			// reset value
			else
			{
			}		
		}
	
		return true;
	};

	bool UICallbacks::Guild::OnSetPasswordClicked(const CEGUI::EventArgs& e)
	{
		// get password input as clr string
		::System::String^ password = StringConvert::CEGUIToCLR(
			ControllerUI::Guild::PasswordVal->getText());

		// request to update password
		OgreClient::Singleton->SendUserCommandGuildSetPassword(password);
		
		return true;
	};

	bool UICallbacks::Guild::OnRenounceClicked(const CEGUI::EventArgs& e)
	{
		GuildInfo^ guildInfo = OgreClient::Singleton->Data->GuildInfo;
		
		if (guildInfo->Flags->IsRenounce)
		{	
			// request
			OgreClient::Singleton->SendUserCommandGuildRenounce();

			// clear/hide
			OgreClient::Singleton->Data->GuildInfo->Clear(true);
			OgreClient::Singleton->Data->GuildShieldInfo->Clear(true);
		}
		else if (guildInfo->Flags->IsDisband)
		{
			// request
			OgreClient::Singleton->SendUserCommandGuildDisband();

			// clear/hide
			OgreClient::Singleton->Data->GuildInfo->Clear(true);
			OgreClient::Singleton->Data->GuildShieldInfo->Clear(true);
		}
				
		return true;
	};

	bool UICallbacks::Guild::OnExileClicked(const CEGUI::EventArgs& e)
	{
		const CEGUI::WindowEventArgs& args	= (const CEGUI::WindowEventArgs&)e;
		const CEGUI::PushButton* box		= (const CEGUI::PushButton*)args.window;
		const CEGUI::ItemEntry* itm			= (const CEGUI::ItemEntry*)box->getParent();
	
		GuildInfo^ guildInfo = OgreClient::Singleton->Data->GuildInfo;
		
		if (guildInfo->Flags->IsExile)
		{						
			// request to exile
			OgreClient::Singleton->SendUserCommandGuildExile(itm->getID());

			// clear and re-request (workaroung since there's no updating)
			OgreClient::Singleton->Data->GuildInfo->Clear(true);
			OgreClient::Singleton->Data->GuildShieldInfo->Clear(true);
			OgreClient::Singleton->SendUserCommandGuildInfoReq();
			OgreClient::Singleton->SendUserCommandGuildGuildListReq();
			OgreClient::Singleton->SendUserCommandGuildShieldListReq();
			OgreClient::Singleton->SendUserCommandGuildShieldInfoReq();
		}
	
		return true;
	};

	bool UICallbacks::Guild::OnGuildShieldSettingChanged(const CEGUI::EventArgs& e)
	{
		GuildShieldInfo^ shieldInfo	= OgreClient::Singleton->Data->GuildShieldInfo;
		
		int designindex = (int)ControllerUI::Guild::ShieldDesign->getCurrentValue();
		if (designindex > -1 && shieldInfo->Shields->Length > designindex)
		{
			unsigned char oldColor1 = shieldInfo->Color1;
			unsigned char oldColor2 = shieldInfo->Color2;
			unsigned char oldDesign = shieldInfo->Design;

			shieldInfo->Color1 = ::System::Convert::ToByte(ControllerUI::Guild::ShieldColor1->getCurrentValue());
			shieldInfo->Color2 = ::System::Convert::ToByte(ControllerUI::Guild::ShieldColor2->getCurrentValue());
			shieldInfo->Design = ::System::Convert::ToByte(ControllerUI::Guild::ShieldDesign->getCurrentValue() + 1.0f);

			// if any difference request info
			if (oldColor1 != shieldInfo->Color1 ||
				oldColor2 != shieldInfo->Color2 ||
				oldDesign != shieldInfo->Design)
			{
				// this requests info for the design/color selection
				OgreClient::Singleton->SendUserCommandClaimShield(false);		
			}
		}

		return true;
	};

	bool UICallbacks::Guild::OnShieldClaimClicked(const CEGUI::EventArgs& e)
	{
		// claim it REALLY
		OgreClient::Singleton->SendUserCommandClaimShield(true);
				
		return true;
	};

	bool UICallbacks::Guild::OnWindowClosed(const CEGUI::EventArgs& e)
	{
		// clear (view will react)
		OgreClient::Singleton->Data->GuildInfo->Clear(true);
		OgreClient::Singleton->Data->GuildShieldInfo->Clear(true);
		ControllerUI::ActivateRoot();

		return true;
	};

	bool UICallbacks::Guild::OnWindowKeyUp(const CEGUI::EventArgs& e)
	{
		const CEGUI::KeyEventArgs& args = static_cast<const CEGUI::KeyEventArgs&>(e);

		// close window on ESC
		if (args.scancode == CEGUI::Key::Escape)
		{
			// clear (view will react)
			OgreClient::Singleton->Data->GuildInfo->Clear(true);
			OgreClient::Singleton->Data->GuildShieldInfo->Clear(true);
			ControllerUI::ActivateRoot();
		}

		return true;
	};

	bool UICallbacks::Guild::OnImageMouseWheel(const CEGUI::EventArgs& e)
	{
		const CEGUI::MouseEventArgs& args = static_cast<const CEGUI::MouseEventArgs&>(e);
		ObjectBase^ shieldObject = OgreClient::Singleton->Data->GuildShieldInfo->ExampleModel;
		
		if (shieldObject != nullptr)
			shieldObject->ViewerAngle += (unsigned short)(args.wheelChange * 200.0f);
		
		return true;
	};
};};