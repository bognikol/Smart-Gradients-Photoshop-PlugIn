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
    /// Interaction logic for InstanceControl.xaml
    /// </summary>
    public partial class InstanceControl : UserControl
    {
        public InstanceControl()
        {
            InitializeComponent();
        }

        public InstanceControl(PhotoshopInstallationInstance p)
        {
            InitializeComponent();
            InstanceVersion = p.GetFullName();
            InstancePath = p.Path;
            VersionChckBx.IsChecked = p.IncludeInInstall;
            Instance = p;
        }

        private PhotoshopInstallationInstance Instance;

        public string InstanceVersion
        {
            get
            {
                return VersionChckBx.Content.ToString();
            }
            set
            {
                VersionChckBx.Content = value;
            }
        }

        public string InstancePath
        {
            get
            {
                return PathLabel.Content.ToString();
            }
            set
            {
                PathLabel.Content = value;
            }
        }

        public bool SelectedForInstalation
        {
            get
            {
                Instance.IncludeInInstall = (bool)VersionChckBx.IsChecked;
                return (bool)VersionChckBx.IsChecked;
                
            }
            set
            {
                VersionChckBx.IsChecked = value;
            }
        }

        private void VersionChckBx_Click(object sender, RoutedEventArgs e)
        {
            Instance.IncludeInInstall = (bool)VersionChckBx.IsChecked;
        }
    }
}
