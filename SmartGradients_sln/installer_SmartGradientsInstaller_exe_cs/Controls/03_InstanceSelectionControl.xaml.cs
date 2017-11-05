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
    /// Interaction logic for InstanceSelectionControl.xaml
    /// </summary>
    public partial class InstanceSelectionControl : UserControl
    {
        public InstanceSelectionControl()
        {
            InitializeComponent();
        }

        private bool noInstanceFlag = false;

        private List<PhotoshopInstallationInstance> _PhotoshopInstanceList;

        public List<PhotoshopInstallationInstance> PhotoshopInstanceList
        {
            get
            {
                return _PhotoshopInstanceList;
            }
            set
            {
                _PhotoshopInstanceList = value;
                if (_PhotoshopInstanceList.Count == 0)
                {

                    InfoTxtBx.Text = "Installer could not find any instance of Adobe® Photoshop® installed on your machine. Select the Photoshop folder manually by clicking the button under.";
                    noInstanceFlag = true;
                }
                else
                {
                    InstanceList.Children.Clear();
                    InfoTxtBx.Text = "Installer has detected following instances of Adobe® Photoshop® installed on your machine. Please, select instances for which you want to install plug-in. If installer has not found instance automatically, you may add it manually by clicking button under.";
                    foreach (PhotoshopInstallationInstance p in _PhotoshopInstanceList)
                    {
                        AddInstalationInstance(p);
                    }
                }
            }
        }

        public Int32 NumberOfSelectedInstances()
        {
            int o = 0;
            foreach (PhotoshopInstallationInstance p in PhotoshopInstanceList)
            {
                if (p.IncludeInInstall) o++;
            }
            return o;
        }

        private void AddInstalationInstance(PhotoshopInstallationInstance p)
        {
            InstanceControl ic = new InstanceControl(p);
            ic.Width = 400;
            ic.Height = 55;
            ic.HorizontalAlignment = HorizontalAlignment.Left;
            InstanceList.Children.Add(ic);
        }

        private bool CheckIfIncluded(string photoshopPath)
        {
            foreach (PhotoshopInstallationInstance p in _PhotoshopInstanceList)
            {
                if (p.Path == photoshopPath)
                {
                    return true;
                }
            }
            return false;
        }

        private void SelectPhotoshopFolderButton_Click(object sender, RoutedEventArgs e)
        {
            System.Windows.Forms.FolderBrowserDialog dlg = new System.Windows.Forms.FolderBrowserDialog();
            if (dlg.ShowDialog() != System.Windows.Forms.DialogResult.OK)
            {
                return;
            }
            if (CheckIfIncluded(dlg.SelectedPath))
            {
                MessageBox.Show("Selected folder already inlcuded!");
                return;
            }
            PhotoshopInstallationInstance p = MainRoutines.IsPhotoshopFolder(dlg.SelectedPath);
            if (p == null)
            {
                MessageBox.Show("Not correct Photoshop folder!");
                return;
            }
            if (noInstanceFlag)
            {
                InstanceList.Children.Clear();
                noInstanceFlag = false;
            }
            _PhotoshopInstanceList.Add(p);
            AddInstalationInstance(p);
        }
    }
}
