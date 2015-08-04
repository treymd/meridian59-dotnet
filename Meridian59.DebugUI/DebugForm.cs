/*
 Copyright (c) 2012 Clint Banzhaf
 This file is part of "Meridian59.DebugUI".

 "Meridian59.DebugUI" is free software: 
 You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, 
 either version 3 of the License, or (at your option) any later version.

 "Meridian59.DebugUI" is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 See the GNU General Public License for more details.

 You should have received a copy of the GNU General Public License along with "Meridian59.DebugUI".
 If not, see http://www.gnu.org/licenses/.
*/

using System;
using System.Collections.Generic;
using System.Text.RegularExpressions;
using System.Windows.Forms;
using Meridian59.Data;
using Meridian59.DebugUI.Events;
using Meridian59.Protocol.Events;
using Meridian59.Protocol.GameMessages;
using Meridian59.Common.Enums;
using Meridian59.Data.Models;
using Meridian59.Files;

namespace Meridian59.DebugUI
{


    public partial class DebugForm : Form
    {
        public event GameMessageEventHandler PacketSend;
        public event PacketLogChangeEventHandler PacketLogChanged;

        private DataController dataController;
        public DataController DataController
        {
            get { return dataController; }
            set
            {
                dataController = value;
                //onlinePlayerViewer.DataSource = dataController.OnlinePlayers;
                //roomInfoViewer.DataSource = dataController.RoomInformation;
                statsConditionView.DataSource = dataController.AvatarCondition;
                statsAttributesView.DataSource = dataController.AvatarAttributes;
                statsSkillsView.DataSource = dataController.AvatarSkills;
                statsSpellsView.DataSource = dataController.AvatarSpells;
                gamePacketViewer.DataSource = dataController.GameMessageLog;
                //avatarBuffsViewer.DataSource = dataController.AvatarBuffs;
                //roomBuffsViewer.DataSource = dataController.RoomBuffs;
                //spellObjectsViewer.DataSource = dataController.SpellObjects;
                guildMemberListViewer.DataSource = dataController.GuildInfo;
                guildShieldsViewer.DataSource = dataController.GuildShieldInfo.Shields;
                guildListViewer.DataSource = dataController.DiplomacyInfo;
                chatMessageViewer.DataSource = dataController.ChatMessages;
                
                roomObjectsView1.DataSource = dataController.RoomObjects;
                inventoryObjectView.DataSource = dataController.InventoryObjects;
                spellsView.DataSource = dataController.SpellObjects;
                avatarBuffsView.DataSource = dataController.AvatarBuffs;
                roomBuffsView.DataSource = dataController.RoomBuffs;
                onlinePlayersView.DataSource = dataController.OnlinePlayers;
                backgroundOverlayView.DataSource = dataController.BackgroundOverlays;
                roomInfoView.DataSource = dataController.RoomInformation;
                lightShadingView.DataSource = dataController.LightShading;
                backgroundMusicView.DataSource = dataController.BackgroundMusic;
            }
        }

        private ResourceManager resourceManager;
        public ResourceManager ResourceManager
        {
            get { return resourceManager; }
            set
            {
                resourceManager = value;
                stringListViewer.DataSource = resourceManager.StringResources;              
            }
        }

        public AdminController AdminController { get; set; }

        public DebugForm()
        {
            InitializeComponent();

            guildMemberListViewer.PacketSend += new GameMessageEventHandler(gamePacketViewer_PacketSend);
            guildListViewer.PacketSend += new GameMessageEventHandler(gamePacketViewer_PacketSend);
            AdminController = new AdminController();
        }

        public void HandleAdminMessage(AdminMessage Message)
        {
            AdminController.HandleAdminMessage(Message);
        }

        private void gamePacketViewer_PacketSend(object sender, GameMessageEventArgs e)
        {
            if (PacketSend != null) PacketSend(this, e);
        }

        private void gamePacketViewer_PacketLogChanged(object sender, PacketLogChangeEventArgs e)
        {
            if (PacketLogChanged != null) PacketLogChanged(this, e);
        }

        private void btnRequestSkills_Click(object sender, EventArgs e)
        {
            if (PacketSend != null)
                PacketSend(this, new GameMessageEventArgs(new SendStatsMessage(StatGroup.Skills)));          
        }

        private void btnRequestSpells_Click(object sender, EventArgs e)
        {
            if (PacketSend != null)
                PacketSend(this, new GameMessageEventArgs(new SendStatsMessage(StatGroup.Spells)));  
        }

        private void btnRequestCondition_Click(object sender, EventArgs e)
        {
            if (PacketSend != null)
                PacketSend(this, new GameMessageEventArgs(new SendStatsMessage(StatGroup.Condition)));
        }

        private void btnRequestAttributes_Click(object sender, EventArgs e)
        {
            if (PacketSend != null)
                PacketSend(this, new GameMessageEventArgs(new SendStatsMessage(StatGroup.Attributes)));
        }

        private void btnRequestPlayerBuffs_Click(object sender, EventArgs e)
        {
            if (PacketSend != null)
                PacketSend(this, new GameMessageEventArgs(new SendEnchantmentsMessage(BuffType.AvatarBuff)));
        }

        private void btnRequestRoomBuffs_Click(object sender, EventArgs e)
        {
            if (PacketSend != null)
                PacketSend(this, new GameMessageEventArgs(new SendEnchantmentsMessage(BuffType.RoomBuff)));
        }

        private void btnRequestSpellObjects_Click(object sender, EventArgs e)
        {
            if (PacketSend != null)
                PacketSend(this, new GameMessageEventArgs(new SendSpellsMessage()));
        }

        private void btnRequestInventory_Click(object sender, EventArgs e)
        {
            if (PacketSend != null)
                PacketSend(this, new GameMessageEventArgs(new ReqInventoryMessage()));
        }

        private void btnRequestGuildShields_Click(object sender, EventArgs e)
        {
            if (PacketSend != null)
                PacketSend(this, new GameMessageEventArgs(new UserCommandMessage(new UserCommandGuildShieldListReq(), null)));
        }

        private void btnLeaveGuild_Click(object sender, EventArgs e)
        {
            if (PacketSend != null)
                PacketSend(this, new GameMessageEventArgs(new UserCommandMessage(new UserCommandGuildRenounce(), null)));
        }

        private void btnDisbandGuild_Click(object sender, EventArgs e)
        {
            if (PacketSend != null)
                PacketSend(this, new GameMessageEventArgs(new UserCommandMessage(new UserCommandGuildDisband(), null)));
        }

        private void txtAdminCommand_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                if (PacketSend != null)
                    PacketSend(this, new GameMessageEventArgs(new ReqAdminMessage(txtAdminCommand.Text)));
                txtAdminCommand.Text = "";
            }
        }

    }

    /// <summary>
    /// These things need to be moved
    /// </summary>

    public class AdminObjectProperty
    {
        public string PropertyName { get; set; }
        public string PropertyType { get; set; }
        public string PropertyValue { get; set; }

        public AdminObjectProperty(string name, string type, string value)
        {
            PropertyName = name;
            PropertyType = type;
            PropertyValue = value;
        }
    }

    public class AdminObject
    {
        public List<AdminObjectProperty> Properties { get; private set; }
        public string ClassName { get; set; }
        public int ObjectNumber { get; set; }

        public AdminObject(string classname, int objectnumber, List<AdminObjectProperty> props = null)
        {
            ClassName = classname;
            ObjectNumber = objectnumber;
            if (props != null)
            {
                Properties = props;
            }
            else
            {
                Properties = new List<AdminObjectProperty>();
            }
        }

        //so later we can do thing upon property update
        public void SetProperties(List<AdminObjectProperty> properties)
        {
            Properties = properties;
        }
    }

    
}
