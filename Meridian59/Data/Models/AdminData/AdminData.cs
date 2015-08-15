using System;
using System.ComponentModel;
using System.Text.RegularExpressions;
using Meridian59.Common.Interfaces;
using Meridian59.Data.Lists;
using Meridian59.Protocol.GameMessages;

namespace Meridian59.Data.Models.AdminData
{
    public class AdminData : INotifyPropertyChanged, IClearable
    {

        public BaseList<AdminObject> TrackedObjects { get; set; }

        public AdminData()
        {
            TrackedObjects = new BaseList<AdminObject>();
        }



        
        public event AdminWatchObjectEventHandler WatchObjectAdded;
        protected void OnWatchObjectAdded(AdminWatchObjectEventHandlerArgs e)
        {
            if (WatchObjectAdded != null) WatchObjectAdded(this, e);
        }
        public void WatchObject(AdminObject adminObject)
        {
            TrackedObjects.Add(adminObject);
            OnWatchObjectAdded(new AdminWatchObjectEventHandlerArgs(this, adminObject));
        }



        #region INotifyPropertyChanged

        public event PropertyChangedEventHandler PropertyChanged;

        protected void RaisePropertyChanged(PropertyChangedEventArgs e)
        {
            if (PropertyChanged != null) PropertyChanged(this, e);
        }

        #endregion

        #region IClearable
        public void Clear(bool RaiseChangedEvent)
        {
            TrackedObjects.Clear();
        }
        #endregion

        public void HandleAdminShowObjectMessage(AdminMessage message)
        {
            //Look for object info
            var regex = new Regex(@":< OBJECT (?<objectnumber>\d*) is CLASS (?<classname>.*)");
            var matches = regex.Matches(message.Message);
            if (matches.Count != 1) //TODO: We should only get one of these at a time but i dont know how to just do if (match), will research
            {
                throw new Exception("Got too many or too few matches");
                return;
            }
            AdminObject obj = null;
            foreach (Match match in matches)
            {
                obj = new AdminObject(match.Groups["classname"].ToString(), Convert.ToInt32(match.Groups["objectnumber"].ToString()));
            }

            regex = new Regex(@": (?<property>\w*)\s*=\s(?<datatype>[\w$]*)\s(?<value>\d*)");
            if (regex.IsMatch((message.Message)))
            {
                BaseList<AdminObjectProperty> props = new BaseList<AdminObjectProperty>();
                matches = regex.Matches(message.Message);
                foreach (Match match in matches)
                {
                    props.Add(new AdminObjectProperty(match.Groups["property"].ToString(), match.Groups["datatype"].ToString(), match.Groups["value"].ToString(),obj));
                }
                if (obj != null)
                {
                    obj.SetProperties(props);
                    WatchObject(obj);
                }

            }
        }
    }

    public delegate void AdminWatchObjectEventHandler(object sender, AdminWatchObjectEventHandlerArgs args);

    public class AdminWatchObjectEventHandlerArgs
    {
        public AdminObject AdminObject;
        public object Sender;

        public AdminWatchObjectEventHandlerArgs(object sender, AdminObject adminObject)
        {
            AdminObject = adminObject;
            Sender = sender;
        }
    }
}
