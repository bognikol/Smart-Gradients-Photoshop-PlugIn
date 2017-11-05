using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace SmartGradientsInstaller.Controls
{
    /// <summary>
    /// Interaction logic for LicenceControl.xaml
    /// </summary>
    public partial class LicenceControl : UserControl
    {
        public LicenceControl()
        {
            InitializeComponent();
        }


        public class LicenceEventArgs : EventArgs
        {
            public bool State { get; set; }
        }

        public delegate void LicenceEventHandler(Object sender, LicenceEventArgs e);

        public event LicenceEventHandler AgreeStateChanged;

        private void AgreeChckBx_Click(object sender, RoutedEventArgs e)
        {
            LicenceEventArgs lea = new LicenceEventArgs();
            lea.State = (bool)AgreeChckBx.IsChecked;
            if (AgreeStateChanged != null)
            {
                AgreeStateChanged(this, lea);
            }
        }
    }
}
