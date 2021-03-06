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

namespace Meridian59.Common.Enums
{
    /// <summary>
    /// HotSpot type on BGF frames
    /// </summary>
    public enum HotSpotType
    {
        HOTSPOT_ANY = -1,       // Ignore over/under; match any hotspot
        HOTSPOT_NONE = 0,
        HOTSPOT_OVER = 1,       // Overlay on an object
        HOTSPOT_UNDER = 2,      // Underlay on an object
        HOTSPOT_OVERUNDER = 3,  // Underlay on an overlay
        HOTSPOT_OVEROVER = 4,   // Overlay on an overlay
        HOTSPOT_UNDERUNDER = 5, // Underlay on an underlay
        HOTSPOT_UNDEROVER = 6,  // Over on an underlay
    }
}
