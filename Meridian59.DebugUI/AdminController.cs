using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using Meridian59.Protocol.GameMessages;

namespace Meridian59.DebugUI
{
    public class AdminController
    {

        public AdminController()
        {
            
        }

        public void HandleAdminMessage(AdminMessage Message)
        {
            //Look for object info
            var regex = new Regex(@":< OBJECT (?<objectnumber>\d*) is CLASS (?<classname>.*)");
            if (regex.IsMatch(Message.Message))
            {
                var matches = regex.Matches(Message.Message);
                if (matches.Count < 1) //We should only get one of these at a time
                {
                    //txtAdminCommand.Text += "Error parsing object header\r\n";
                    return;
                }
                AdminObject obj = null;
                foreach (Match match in matches) //but i dont know how to just do if -match will research
                {
                    //txtAdminOutput.Text += String.Format("Found Object {0} of class {1}\r\n", match.Groups["objectnumber"],
                    //    match.Groups["classname"]);
                    obj = new AdminObject(match.Groups["classname"].ToString(), Convert.ToInt32(match.Groups["objectnumber"].ToString()));
                }

                regex = new Regex(@": (?<property>\w*)\s*=\s(?<datatype>[\w$]*)\s(?<value>\d*)");
                if (regex.IsMatch((Message.Message)))
                {
                    List<AdminObjectProperty> props = new List<AdminObjectProperty>();
                    matches = regex.Matches(Message.Message);
                    foreach (Match match in matches)
                    {
                        //txtAdminOutput.Text += String.Format("Property {0} with datatype {1} and value {2}\r\n",match.Groups["property"],match.Groups["datatype"],match.Groups["value"]);
                        props.Add(new AdminObjectProperty(match.Groups["property"].ToString(), match.Groups["datatype"].ToString(), match.Groups["value"].ToString()));
                    }
                    if (obj != null)
                    {
                        obj.SetProperties(props);
                        ObjectEditor oe = new ObjectEditor(obj);
                        oe.Show();
                    }

                }

            }
            else
            {
                //txtAdminOutput.Text += Message.Message;
            }
            /*
:< OBJECT 0 is CLASS System
: self                 = OBJECT 0
: plTemp               = $ 0
: plRooms              = LIST 49994
: plUsers              = LIST 65530
: plUsers_logged_on    = $ 0
: plTreasure_types     = LIST 58647
: plSpells             = LIST 56333
: plSkills             = LIST 56352
: plItem_Attributes    = LIST 57342
: plItemAttTreasure    = LIST 56382
*/
        }
    }
}
