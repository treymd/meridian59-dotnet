using System.ComponentModel;
using Meridian59.Data.Lists;

namespace Meridian59.Data.Models.AdminData
{
    /// <summary>
    /// Object that represents a displayed object in the Administration UI
    /// </summary>
    public class AdminObject : INotifyPropertyChanged
    {
        /// <summary>
        /// The Object's Properties
        /// </summary>
        public BaseList<AdminObjectProperty> Properties { get; private set; } 

        /// <summary>
        /// Kod Class
        /// </summary>
        public string ClassName { get; set; }
        /// <summary>
        /// Object Number
        /// </summary>
        public int ObjectNumber { get; set; }

        public AdminObject(string classname, int objectnumber, BaseList<AdminObjectProperty> props = null)
        {
            ClassName = classname;
            ObjectNumber = objectnumber;
            if (props != null)
            {
                Properties = props;
            }
            else
            {
                Properties = new BaseList<AdminObjectProperty>();
            }
        }

        public AdminObject()
        {
            Properties = new BaseList<AdminObjectProperty>();
        }

        //so later we can do thing upon property update
        public void SetProperties(BaseList<AdminObjectProperty> properties)
        {
            Properties = properties;
        }

        #region INotifyPropertyChanged

        public event PropertyChangedEventHandler PropertyChanged;

        protected void RaisePropertyChanged(PropertyChangedEventArgs e)
        {
            if (PropertyChanged != null) PropertyChanged(this, e);
        }

        #endregion
    }
}
