﻿/*
 Copyright (c) 2012-2013 Clint Banzhaf
 This file is part of "Meridian59 .NET".

 "Meridian59 .NET" is free software: 
 You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, 
 either version 3 of the License, or (at your option) any later version.

 "Meridian59 .NET" is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 See the GNU General Public License for more details.

 You should have received a copy of the GNU General Public License along with "Meridian59 .NET".
 If not, see http://www.gnu.org/licenses/.
*/

using System;
using Meridian59.Protocol.Enums;
using Meridian59.Common.Enums;
using Meridian59.Data.Models;
using Meridian59.Common.Constants;

namespace Meridian59.Protocol.GameMessages
{
    /// <summary>
    /// Tells the client about a new enchantment (room or player)
    /// </summary>
    [Serializable]
    public class AddEnchantmentMessage : GameModeMessage
    {       
        #region IByteSerializable implementation
        public override int ByteLength
        {
            get
            {
                return base.ByteLength + TypeSizes.BYTE + NewBuffObject.ByteLength;
            }
        }

        public override int WriteTo(byte[] Buffer, int StartIndex = 0)
        {
            int cursor = StartIndex;

            cursor += base.WriteTo(Buffer, StartIndex);

            Buffer[cursor] = (byte)BuffType;
            cursor++;
           
            cursor += NewBuffObject.WriteTo(Buffer, cursor);

            return cursor - StartIndex;
        }

        public override int ReadFrom(byte[] Buffer, int StartIndex = 0)
        {
            int cursor = StartIndex;

            cursor += base.ReadFrom(Buffer, StartIndex);

            BuffType = (BuffType)Buffer[cursor];
            cursor++;

            NewBuffObject = new ObjectBase(true, Buffer, cursor);
            cursor += NewBuffObject.ByteLength;

            return cursor - StartIndex;
        }
        #endregion

        public BuffType BuffType { get; set; }
        public ObjectBase NewBuffObject { get; set; }

        public AddEnchantmentMessage(BuffType BuffType, ObjectBase buffObject) 
            : base(MessageTypeGameMode.AddEnchantment)
        {         
            this.BuffType = BuffType;
            this.NewBuffObject = buffObject;
        }

        public AddEnchantmentMessage(byte[] Buffer, int StartIndex = 0) 
            : base (Buffer, StartIndex = 0) { }
    }
}
