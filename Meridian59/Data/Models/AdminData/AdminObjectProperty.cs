using System.ComponentModel;

namespace Meridian59.Data.Models.AdminData
{
    public class AdminObjectProperty : INotifyPropertyChanged
    {
        public string PropertyName { get; set; }
        public string PropertyType { get; set; }
        public string PropertyValue { get; set; }
        private AdminObject _owner;

        public AdminObjectProperty(string name, string type, string value, AdminObject owner)
        {
            PropertyName = name;
            PropertyType = type;
            PropertyValue = value;
            _owner = owner;
        }

        public AdminObjectProperty()
        {
            
        }

        public AdminObjectProperty(string propertyName, string propertyType, string propertyValue)
        {
            PropertyName = propertyName;
            PropertyType = propertyType;
            PropertyValue = propertyValue;
        }

        #region INotifyPropertyChanged

        public event PropertyChangedEventHandler PropertyChanged;
        private string _propertyName;
        private string p2;
        private string p3;

        protected void RaisePropertyChanged(PropertyChangedEventArgs e)
        {
            if (PropertyChanged != null) PropertyChanged(this, e);
        }

        #endregion
    }
}
